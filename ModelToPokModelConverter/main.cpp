#include <iostream>
#include <Windows.h>
#include "FBXConverter.h"
#include <Qt\qapplication.h>
#include "Preview.h"
#include <Graphics\CommonGraphicsCommands.h>

int main( int argc , char** argv )
{
	QApplication app( argc , argv );
	Preview* lolz = 0;
	if ( argc > 1 )
	{
		std::string theOutput( argv[1] );
		while ( theOutput.length() > 0 && theOutput.back() != '.' )
		{
			theOutput.pop_back();
		}
		if ( theOutput.length() > 0 && theOutput.back() == '.' ) theOutput.pop_back();
		FBXConverter a;
		a.convert( argv[1] , (theOutput + std::string(".pmd")).c_str() );
		lolz = new Preview( ( theOutput + std::string( ".pmd" ) ) );
		lolz->show();
	}
	else
	{
		lolz = new Preview( ( std::string( "flag.pmd" ) ) );
		//lolz = new Preview( ( std::string( "Ogre.pmd" ) ) );
		lolz->show();
	}
	int toreturn = app.exec();
	if ( lolz ) delete lolz;
	return toreturn;
}