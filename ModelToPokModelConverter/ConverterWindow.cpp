#include "ConverterWindow.h"
#include <QtGui\QLineEdit>
#include <QtGui\QVBoxLayout>
#include <QtGui\QHBoxLayout>
#include <QtGui\QPushButton>
#include "FBXConverter.h"
#include <QtGui\QFileDialog>
#include <Qt\qcoreapplication.h>
#include <Qt\qtimer.h>
#include "Preview.h"
#include "DebugHeap.h"
ConverterWindow::ConverterWindow()
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
	
	preview = new Preview;
	preview->setMinimumWidth( 1280 );
	preview->setMinimumHeight( 720 );
	mainLayout->addWidget( preview );
	mainLayout->addLayout( layout );

	setLayout( mainLayout );
	connect( browse , SIGNAL( pressed() ) , this , SLOT( browse() ) );
	connect( convert , SIGNAL( pressed() ) , this , SLOT( startConverting() ) );
	connect( colorTexture , SIGNAL( pressed() ) , this , SLOT( colorTextureButton() ) );
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
}

void ConverterWindow::colorTextureButton()
{
	preview->setTexture( dialogbox.getOpenFileName( NULL , "Open Color Texture" ).toUtf8() );
}
ConverterWindow::~ConverterWindow()
{
	delete preview;
}
