#include <string>

void main( int argc , char** argv )
{
	if ( argc < 2 ) return;

	std::string theOutput( argv[1] );
	std::string theExtension;
	while ( theOutput.length() > 0 && theOutput.back() != '.' )
	{
		theExtension = theOutput.back() + theExtension;
		theOutput.pop_back();
	}
	if ( theOutput.length() > 0 && theOutput.back() == '.' )
	{
		theExtension = theOutput.back() + theExtension;
		theOutput.pop_back();
	}


}