#include <iostream>
#include <Windows.h>
#include "FBXConverter.h"
#include <Qt\qapplication.h>
int main( int argc , char** argv )
{
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
		return 0;
	}
	else
	{
		QApplication app( argc , argv );
		return app.exec();
	}
}