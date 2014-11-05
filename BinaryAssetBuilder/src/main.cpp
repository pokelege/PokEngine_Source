#include <string>
#include <QtCore\QDir>
#include <QtCore\QDateTime>
void main( int argc , char** argv )
{
	if ( argc < 2 ) return;

	QDir inputDir( argv[1] );
	QDir outputDir( argv[2] );
	QFileInfoList files = inputDir.entryInfoList();
	for ( unsigned int i = 0; i < files.size(); ++i )
	{
		if ( files.at( i ).isDir() )
		{
			processDir( files.at( i ).absoluteDir() , outputDir, inputDir );
		}
	}
}

void processDir(QDir& input, QDir& output, QDir& mainInput)
{
	QFileInfoList files = input.entryInfoList();
	for ( unsigned int i = 0; i < files.size(); ++i )
	{
		if ( files.at( i ).isDir() )
		{
			processDir( files.at( i ).absoluteDir() , output, mainInput );
		}
	}
}

void processFile( QFileInfo& file , QDir& output , QDir& mainInput )
{
	QDir outputCopy = output;
	if ( !outputCopy.cd( mainInput.relativeFilePath( file.absoluteDir().absolutePath() ) ) )
	{
		output.mkdir( mainInput.relativeFilePath( file.absoluteDir().absolutePath() ) );
		outputCopy = output;
		outputCopy.cd( mainInput.relativeFilePath( file.absoluteDir().absolutePath() ) );
	}

	if ( )

	file.lastModified().toTime_t();
}