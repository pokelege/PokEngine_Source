#include <string>
#include <QtCore\QDir>
#include <QtCore\QDateTime>
#include <QtCore\QDebug>
#include <iostream>
void processDir( QDir input , QDir output , QDir mainInput );
void processFile( QFileInfo file , QDir output , QDir mainInput );
void main( int argc , char** argv )
{
	for ( int i = 0; i < argc; ++i )
	{
		std::cout << i << " " << argv[i] << std::endl;
	}
	if ( argc < 2 ) return;

	QDir inputDir( argv[1] );
	QDir outputDir( argv[2] );
	QFileInfoList files = inputDir.entryInfoList();
	//std::cout << "numfiles " << files.size() << std::endl;
	for ( int i = 0; i < files.size(); ++i )
	{
		if ( files.at( i ).isDir() )
		{
			qDebug( files.at( i ).fileName().toUtf8() );
			processDir( files.at( i ).fileName() , outputDir , inputDir );
		}
		else
		{
			processFile( files.at( i ) , outputDir , inputDir );
		}
	}
}

void processDir(QDir input, QDir output, QDir mainInput)
{
	QFileInfoList files = input.entryInfoList();
	//std::cout << "numfiles " << files.size() << std::endl;
	for ( int i = 0; i < files.size(); ++i )
	{
		if ( files.at( i ).isDir() )
		{
			qDebug( files.at( i ).fileName().toUtf8() );
			processDir( files.at( i ).fileName() , output , mainInput );
		}
		else
		{
			processFile( files.at( i ) , output , mainInput );
		}
	}
}

void processFile( QFileInfo file , QDir output , QDir mainInput )
{
	std::cout << "copyed" << std::endl;
	QDir outputCopy = output;
	if ( !outputCopy.cd( mainInput.relativeFilePath( file.absoluteDir().absolutePath() ) ) )
	{
		output.mkdir( mainInput.relativeFilePath( file.absoluteDir().absolutePath() ) );
		outputCopy = output;
		outputCopy.cd( mainInput.relativeFilePath( file.absoluteDir().absolutePath() ) );
	}
	QFileInfo outPutFile = outputCopy.absolutePath() + file.fileName();
	if ( outPutFile.exists() && outPutFile.lastModified().toTime_t() < file.lastModified().toTime_t() )
	{
		QFile::remove( outPutFile.absoluteFilePath() );
		QFile::copy( file.absoluteFilePath() , outPutFile.absoluteFilePath() );
	}
	else
	{
		QFile::copy( file.absoluteFilePath() , outPutFile.absoluteFilePath() );
	}
}