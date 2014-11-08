#include <QtCore\QDir>
#include <SOIL.h>
#include <fstream>
#include <FBXConverter.h>
void saveTexture( const char* input , const char* output );
void saveGeometry( const char* input , const char* output );
int main( int argc , char** argv )
{
	if ( argc < 2 ) return 0;

	QFileInfo inputFile( argv[1] );
	QFileInfo outputFile( argv[2] );
	QString extension = inputFile.suffix();
	if ( QString::compare( extension , "png" , Qt::CaseInsensitive ) == 0 )
	{
		saveTexture( inputFile.absoluteFilePath().toUtf8() , outputFile.absoluteFilePath().toUtf8() );
	}
	else if ( QString::compare( extension , "obj" , Qt::CaseInsensitive ) == 0 || QString::compare( extension , "fbx" , Qt::CaseInsensitive ) == 0 )
	{
		saveGeometry( inputFile.absoluteFilePath().toUtf8() , outputFile.absoluteFilePath().toUtf8() );
	}
	else
	{
		if ( outputFile.exists() )
		{
			QFile::remove( outputFile.absoluteFilePath() );
			QFile::copy( inputFile.absoluteFilePath() , outputFile.absoluteFilePath() );
		}
		else
		{
			QFile::copy( inputFile.absoluteFilePath() , outputFile.absoluteFilePath() );
		}
	}
	return 0;
}

void saveTexture( const char* input , const char* output )
{
	int width , height, channels;
	unsigned char* theTex = SOIL_load_image( input , &width , &height , &channels , SOIL_LOAD_RGBA );
	std::fstream outputStream( output , std::ios_base::binary | std::ios_base::out | std::ios_base::trunc );
	outputStream.write( ( char* ) &width , sizeof( int ) );
	outputStream.write( ( char* ) &height , sizeof( int ) );
	for ( int y = height - 1; y >= 0; --y )
	{
		for ( int x = 0; x < width; ++x )
		{
			outputStream.write( (char*)&theTex[( y * 4 * width ) + ( x * 4 )], sizeof(unsigned char) );
			outputStream.write( ( char* ) &theTex[(( y * 4 * width ) + ( x * 4 )) + 1] , sizeof( unsigned char ) );
			outputStream.write( ( char* ) &theTex[(( y * 4 * width ) + ( x * 4 )) + 2] , sizeof( unsigned char ) );
			outputStream.write( ( char* ) &theTex[(( y * 4 * width ) + ( x * 4 )) + 3] , sizeof( unsigned char ) );
		}
	}
	outputStream.close();
}

void saveGeometry( const char* input , const char* output )
{
	FBXConverter converter;
	converter.convert( input , output );
}