#include "ConverterWindow.h"
#include <QtGui\QLineEdit>
#include <QtGui\QVBoxLayout>
#include <QtGui\QHBoxLayout>
#include <QtGui\QPushButton>
#include "FBXConverter.h"
#include <QtGui\QFileDialog>
#include <Qt\qcoreapplication.h>
#include <Qt\qtimer.h>
#include <Preview.h>
#include <DebugHeap.h>
#include <QtGui\QComboBox>
#include <Graphics\GeometryInfo.h>
#include <QtGui\QSpinBox>
#include <Graphics\AnimationFrameRangeInfo.h>
#include <iostream>
#include <Graphics\GraphicsGeometryManager.h>
#include <Core\GameObject.h>
#include <Graphics\AnimationRenderingInfo.h>

ConverterWindow::ConverterWindow() : changing(false)
{
	QHBoxLayout* mainLayout = new QHBoxLayout;
	QVBoxLayout* layout = new QVBoxLayout;
	QHBoxLayout* dialogLayout = new QHBoxLayout;
	fileName = new QLineEdit;
	QPushButton* browse = new QPushButton( "Browse..." );
	dialogLayout->addWidget( fileName );
	dialogLayout->addWidget( browse );
	layout->addLayout( dialogLayout );
	QPushButton* convert = new QPushButton( "Convert!" );
	layout->addWidget( convert );
	QPushButton* colorTexture = new QPushButton( "Color Texture" );
	layout->addWidget( colorTexture );

	QHBoxLayout* comboBoxLayout = new QHBoxLayout;
	frameList = new QComboBox;
	comboBoxLayout->addWidget( frameList );
	QPushButton* addFrames = new QPushButton( "Add" );
	comboBoxLayout->addWidget( addFrames );
	//QPushButton* removeFrames = new QPushButton( "Remove" );
	//comboBoxLayout->addWidget( removeFrames );

	layout->addLayout( comboBoxLayout );
	nextFrame = new QSpinBox;
	nextFrame->setRange( 0, INT_MAX );
	nextFrame->setValue( 0 );
	layout->addWidget( nextFrame );
	startFrame = new QSpinBox;
	startFrame->setRange( 0 , INT_MAX );
	startFrame->setValue( 0 );
	layout->addWidget( startFrame );
	endFrame = new QSpinBox;
	endFrame->setRange( 0 , INT_MAX );
	endFrame->setValue(INT_MAX);
	layout->addWidget( endFrame );

	QPushButton* save = new QPushButton( "Save" );
	layout->addWidget( save );

	preview = new Preview;
	preview->setMinimumWidth( 1280 );
	preview->setMinimumHeight( 720 );
	mainLayout->addWidget( preview );
	mainLayout->addLayout( layout );

	setLayout( mainLayout );
	connect( browse , SIGNAL( pressed() ) , this , SLOT( browse() ) );
	connect( convert , SIGNAL( pressed() ) , this , SLOT( startConverting() ) );
	connect( colorTexture , SIGNAL( pressed() ) , this , SLOT( colorTextureButton() ) );
	connect( frameList , SIGNAL( currentIndexChanged( int )) , this , SLOT( currentIndexChanged( int ) ) );
	connect( addFrames , SIGNAL( pressed() ) , this , SLOT( addFrameRange() ) );
	//connect( removeFrames , SIGNAL( pressed() ) , this , SLOT( deleteFrameRange() ) );

	connect( nextFrame , SIGNAL( valueChanged( int ) ) , this , SLOT( changeParamsNext() ) );
	connect( startFrame , SIGNAL( valueChanged( int ) ) , this , SLOT( changeParamsStart() ) );
	connect( endFrame , SIGNAL( valueChanged( int ) ) , this , SLOT( changeParamsEnd() ) );
	connect( save , SIGNAL( pressed() ) , this , SLOT( save() ) );
}

void ConverterWindow::browse()
{
	QFileDialog dialogbox;
	fileName->setText( dialogbox.getOpenFileName( NULL , "Open Model" ) );
}
void ConverterWindow::startConverting()
{
	std::string theOutput( fileName->text().toUtf8() );
	while ( theOutput.length() > 0 && theOutput.back() != '.' )
	{
		theOutput.pop_back();
	}
	if ( theOutput.length() > 0 && theOutput.back() == '.' ) theOutput.pop_back();
	FBXConverter a;
	a.convert( fileName->text().toUtf8() , ( theOutput + std::string( ".pmd" ) ).c_str() );
	preview->setModel( ( theOutput + std::string( ".pmd" ) ) );

	while ( frameList->count() )
	{
		frameList->removeItem( frameList->count() - 1);
	}

	unsigned int numFrames = 0;
	AnimationFrameRangeInfo* frameRange = preview->renderable->geometryInfo->modelData->getAnimationFrameRange( &numFrames );
	if ( frameRange )
	{
		for ( unsigned int i = 0; i < numFrames; ++i )
		{
			frameList->addItem( QString::number( i ) );
		}
		frameList->setCurrentIndex( 0 );
		nextFrame->setMaximum( numFrames - 1 );
	}
}

void ConverterWindow::currentIndexChanged( int index )
{
	unsigned int numFrames;
	AnimationFrameRangeInfo* frameRange = preview->renderable->geometryInfo->modelData->getAnimationFrameRange( &numFrames );

	if ( frameRange )
	{

		blockSignals( true );
		changing = true;
		startFrame->setMaximum( INT_MAX );
		endFrame->setMinimum( 0 );
		nextFrame->setValue( frameRange[index].nextAnimationFrameInfo );
		startFrame->setValue( frameRange[index].firstFrame );
		endFrame->setValue( frameRange[index].lastFrame );
		//startFrame->setMaximum( frameRange[index].lastFrame );
		//endFrame->setMinimum( frameRange[index].firstFrame );
		changing = false;
		blockSignals( false );
	}
}

void ConverterWindow::addFrameRange()
{
	if ( !( preview && preview->renderable && preview->renderable->geometryInfo && preview->renderable->geometryInfo->modelData ) ) return;
	unsigned int numFrames;
	AnimationFrameRangeInfo* frameRange = preview->renderable->geometryInfo->modelData->getAnimationFrameRange( &numFrames );

	char* newFrameRange = new char[sizeof( AnimationFrameRangeInfo ) * ( numFrames + 1 )];
	ZeroMemory( newFrameRange , sizeof( AnimationFrameRangeInfo ) * ( numFrames + 1 ) );
	memcpy( newFrameRange , reinterpret_cast< char* >( frameRange ) , sizeof( AnimationFrameRangeInfo ) * ( numFrames ) );
	preview->renderable->geometryInfo->modelData->setNewAnimationFrameRangeInfo( reinterpret_cast<AnimationFrameRangeInfo* >( newFrameRange ) , numFrames + 1 );
	delete[] newFrameRange;
	frameRange = preview->renderable->geometryInfo->modelData->getAnimationFrameRange( &numFrames );

	frameList->addItem( QString::number( numFrames - 1 ) );

	frameList->setCurrentIndex( numFrames - 1 );
	nextFrame->setMaximum( numFrames - 1 );

	//nextFrame->setValue( frameRange[numFrames - 1].nextAnimationFrameInfo );
	//startFrame->setValue( frameRange[numFrames - 1].firstFrame );
	//endFrame->setValue( frameRange[numFrames - 1].lastFrame );
}

void ConverterWindow::deleteFrameRange()
{
	if ( !( preview && preview->renderable && preview->renderable->geometryInfo && preview->renderable->geometryInfo->modelData ) ) return;
	unsigned int numFrames;
	AnimationFrameRangeInfo* frameRange = preview->renderable->geometryInfo->modelData->getAnimationFrameRange( &numFrames );
	int indexToDelete = frameList->currentIndex();
	if ( !numFrames || indexToDelete < 0 ) return;
	char* newFrameRange = new char[sizeof( AnimationFrameRangeInfo ) * ( numFrames - 1 )];
	ZeroMemory( newFrameRange , sizeof( AnimationFrameRangeInfo ) * ( numFrames - 1 ) );

	if ( indexToDelete > 0 ) memcpy( newFrameRange , reinterpret_cast< char* >( frameRange ) , sizeof( AnimationFrameRangeInfo ) * ( indexToDelete ) );
	//5 6
	if ( ( unsigned int ) indexToDelete < numFrames - 1 ) memcpy( &newFrameRange[sizeof( AnimationFrameRangeInfo ) * ( indexToDelete )] , reinterpret_cast< char* >( &frameRange[indexToDelete + 1] ) , sizeof( AnimationFrameRangeInfo ) *  ( numFrames - ( indexToDelete - 1 ) ) );

	preview->renderable->geometryInfo->modelData->setNewAnimationFrameRangeInfo( reinterpret_cast< AnimationFrameRangeInfo* >( newFrameRange ) , numFrames - 1 );
	delete[] newFrameRange;
	frameRange = preview->renderable->geometryInfo->modelData->getAnimationFrameRange( &numFrames );

	while ( frameList->count() )
	{
		frameList->removeItem( frameList->count() - 1 );
	}

	if ( frameRange )
	{
		for ( unsigned int i = 0; i < numFrames; ++i )
		{
			frameList->addItem( QString::number( i ) );
		}
		frameList->setCurrentIndex( 0 );
	}
	nextFrame->setMaximum( numFrames - 1 );
}

void ConverterWindow::changeParamsNext()
{
	if ( changing ) return;
	int index = frameList->currentIndex();
	if ( index < 0 ) return;
	unsigned int numFrames;
	AnimationFrameRangeInfo* frameRange = preview->renderable->geometryInfo->modelData->getAnimationFrameRange( &numFrames );

	frameRange[index].nextAnimationFrameInfo = nextFrame->value();
	preview->renderable->parent->getComponent<Animator>()->play( frameList->currentIndex() );
}
void ConverterWindow::changeParamsStart()
{
	if ( changing ) return;
	int index = frameList->currentIndex();
	if ( index < 0 ) return;
	unsigned int numFrames;
	AnimationFrameRangeInfo* frameRange = preview->renderable->geometryInfo->modelData->getAnimationFrameRange( &numFrames );

	frameRange[index].firstFrame = startFrame->value();
	endFrame->setMinimum( frameRange[index].firstFrame );
	preview->renderable->parent->getComponent<Animator>()->play( frameList->currentIndex() );
}
void ConverterWindow::changeParamsEnd()
{
	if ( changing ) return;
	std::cout << "changed " << frameList->currentIndex() << std::endl;
	std::cout << endFrame->value() << std::endl;
	int index = frameList->currentIndex();
	if ( index < 0 ) return;
	unsigned int numFrames;
	AnimationFrameRangeInfo* frameRange = preview->renderable->geometryInfo->modelData->getAnimationFrameRange( &numFrames );

	frameRange[index].lastFrame = endFrame->value();
	startFrame->setMaximum( frameRange[index].lastFrame );
	preview->renderable->parent->getComponent<Animator>()->play( frameList->currentIndex() );
}

void ConverterWindow::save()
{
	if ( !( preview && preview->renderable && preview->renderable->geometryInfo && preview->renderable->geometryInfo->modelData ) ) return;
	std::string theOutput( fileName->text().toUtf8() );
	while ( theOutput.length() > 0 && theOutput.back() != '.' )
	{
		theOutput.pop_back();
	}
	if ( theOutput.length() > 0 && theOutput.back() == '.' ) theOutput.pop_back();
	GraphicsGeometryManager::globalGeometryManager.saveGeometry( preview->renderable->geometryInfo , ( theOutput + std::string( ".pmd" ) ) );
	preview->setModel( ( theOutput + std::string( ".pmd" ) ) );
}

void ConverterWindow::colorTextureButton()
{
	preview->setTexture( dialogbox.getOpenFileName( NULL , "Open Color Texture", QCoreApplication::applicationDirPath(), "*.tex"  ).toUtf8() );
}
ConverterWindow::~ConverterWindow()
{
	delete preview;
}
