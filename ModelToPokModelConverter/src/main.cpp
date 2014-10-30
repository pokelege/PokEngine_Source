#include <iostream>
#include <Windows.h>
#include "FBXConverter.h"
#include <Qt\qapplication.h>
#include <Graphics\CommonGraphicsCommands.h>
#include "ConverterWindow.h"
#include "DebugHeap.h"
int main( int argc , char** argv )
{
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif
	QApplication app( argc , argv );
	ConverterWindow* converterWindow = 0;
	if ( argc > 1 )
	{
		std::string theOutput( argv[1] );
		while ( theOutput.length() > 0 && theOutput.back() != '.' )
		{
			theOutput.pop_back();
		}
		if ( theOutput.length() > 0 && theOutput.back() == '.' ) theOutput.pop_back();
		FBXConverter a;
		a.convert( argv[1] , ( theOutput + std::string( ".pmd" ) ).c_str() );
	}
	else
	{
		converterWindow = new ConverterWindow;
		converterWindow->showMaximized();
	}
	int toreturn = app.exec();
	if ( converterWindow ) delete converterWindow;
	return toreturn;
}