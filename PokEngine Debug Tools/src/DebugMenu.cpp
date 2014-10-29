#include "DebugMenu.h"
#ifdef DEBUGMENU
#include <GL\glew.h>
#include <sstream>
ArrayList<DebugMenu::ElementInfo> DebugMenu::elements;

unsigned int DebugMenu::base;

unsigned int DebugMenu::bufferId;

unsigned int DebugMenu::checkBoxId;
unsigned int DebugMenu::checkBoxIndex;
unsigned int DebugMenu::checkBoxNumIndices;
unsigned int DebugMenu::checkBoxDisIndex;
unsigned int DebugMenu::checkBoxDisNumIndices;

unsigned int DebugMenu::lineId;
unsigned int DebugMenu::lineIndex;
unsigned int DebugMenu::lineNumIndecies;

unsigned int DebugMenu::programID;

void DebugMenu::Initialize(HDC hdc)
{
	glewInit();

	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists( 96 );								// Storage For 96 Characters

	font = CreateFont( -24 ,							// Height Of Font
					   0 ,								// Width Of Font
					   0 ,								// Angle Of Escapement
					   0 ,								// Orientation Angle
					   FW_BOLD ,						// Font Weight
					   FALSE ,							// Italic
					   FALSE ,							// Underline
					   FALSE ,							// Strikeout
					   ANSI_CHARSET ,					// Character Set Identifier
					   OUT_TT_PRECIS ,					// Output Precision
					   CLIP_DEFAULT_PRECIS ,			// Clipping Precision
					   ANTIALIASED_QUALITY ,			// Output Quality
					   FF_DONTCARE | DEFAULT_PITCH ,		// Family And Pitch
					   LPCWSTR( "Courier New" ) );					// Font Name

	oldfont = ( HFONT ) SelectObject( hdc , font );           // Selects The Font We Want
	wglUseFontBitmaps( hdc , 32 , 96 , base );				// Builds 96 Characters Starting At Character 32
	SelectObject( hdc , oldfont );							// Selects The Font We Want
	DeleteObject( font );


	const char* vertexShader =
		"#version 400\r\n"
		"in layout( location = 0 ) vec2 position;"
		"uniform vec2 translate;"
		"void main()"
		"{"
		"gl_Position = vec4(position + translate, 0, 1);"
		"};";

	const char* shapeFragmentShader =
		"#version 400\r\n"

		"uniform vec4 color;"
		"out vec4 newColor;"

		"void main()"
		"{"
		"newColor = color;"
		"};";

	unsigned int shader = glCreateShader( GL_VERTEX_SHADER );
	unsigned int fShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( shader , 1 , &vertexShader , 0 );
	glShaderSource( fShader , 1 , &shapeFragmentShader , 0 );
	glCompileShader( shader );
	glCompileShader( fShader );

	programID = glCreateProgram();
	glAttachShader( programID , shader );
	glAttachShader( programID , fShader );
	glLinkProgram( programID );
	glDeleteShader( shader );
	glDeleteShader( fShader );

	float checkBoxSquare[] =
	{
		0.025f , 0.025f ,
		0.025f , -0.025f ,
		-0.025f , -0.025f ,
		-0.025f , 0.025f
	};

	unsigned short checkBoxSquareIndex[] =
	{
		0,1,2,
		0,3,2
	};

	unsigned short checkBoxSquareDisIndex[] =
	{
		0,1,
		1,2,
		2,3,
		3,0
	};

	float line[] =
	{
		0 , 0,
		1.0f,0
	};

	unsigned short lineIndex[] =
	{
		0,1
	};

	glGenBuffers( 1 , &bufferId );
	glBindBuffer( GL_ARRAY_BUFFER , bufferId );
	glBufferData( GL_ARRAY_BUFFER , sizeof( checkBoxSquare ) +sizeof( checkBoxSquareIndex ) +sizeof( checkBoxSquareDisIndex ) + sizeof(line) + sizeof(lineIndex) , 0 , GL_DYNAMIC_DRAW );
	GLint currentoffset = 0;
	
	glGenVertexArrays( 1 , &checkBoxId );
	glBindVertexArray( checkBoxId );
	glBindBuffer( GL_ARRAY_BUFFER , bufferId );
	
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0 , 2 , GL_FLOAT , GL_FALSE , sizeof( float ) * 2 , ( void* ) ( currentoffset ) );
	
	glBufferSubData( GL_ARRAY_BUFFER , currentoffset , sizeof( checkBoxSquare ) , checkBoxSquare );
	currentoffset += sizeof( checkBoxSquare );
	checkBoxIndex = currentoffset;
	checkBoxNumIndices = sizeof( checkBoxSquareIndex ) / sizeof( unsigned short );
	
	glBufferSubData( GL_ARRAY_BUFFER , currentoffset , sizeof( checkBoxSquareIndex ) , checkBoxSquareIndex );
	currentoffset += sizeof( checkBoxSquareIndex );
	checkBoxDisIndex = currentoffset;
	checkBoxDisNumIndices = sizeof( checkBoxSquareDisIndex ) / sizeof( unsigned short );

	glBufferSubData( GL_ARRAY_BUFFER , currentoffset , sizeof( checkBoxSquareDisIndex ) , checkBoxSquareDisIndex );
	currentoffset += sizeof( checkBoxSquareDisIndex );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , bufferId );

	glGenVertexArrays( 1 , &lineId );
	glBindVertexArray( lineId );
	glBindBuffer( GL_ARRAY_BUFFER , bufferId );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0 , 2 , GL_FLOAT , GL_FALSE , sizeof( float ) * 2 , ( void* ) ( currentoffset ) );

	glBufferSubData( GL_ARRAY_BUFFER , currentoffset , sizeof( line ) , line );
	currentoffset += sizeof( line );
	DebugMenu::lineIndex = currentoffset;
	lineNumIndecies = sizeof( lineIndex ) / sizeof( unsigned short );

	glBufferSubData( GL_ARRAY_BUFFER , currentoffset , sizeof( lineIndex ) , lineIndex );
	currentoffset += sizeof( lineIndex );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferId );
}

void DebugMenu::printGL( const char * string )					// Custom GL "Print" Routine
{
	glPushAttrib( GL_LIST_BIT );
	glListBase( base - 32 );
	glCallLists( strlen( string ) , GL_UNSIGNED_BYTE , string );
	glPopAttrib();
}

void DebugMenu::FloatControl( const char* label , float* toDynamicChange, const float& min, const float& max)
{
			SliderInfo* slider = new SliderInfo;
			slider->label = label;
			slider->min = min;
			slider->max = max;
			slider->location = 0;
			slider->localValue = toDynamicChange;
			slider->percent = 0;
			elements.add(slider);
}
void DebugMenu::BoolControl( const char* label , bool *toDynamicChange )
{
			CheckBoxInfo* checkBox = new CheckBoxInfo;
			checkBox->label = label;
			checkBox->checked = false;
			checkBox->localValue = toDynamicChange;
			checkBox->location = 0;
			elements.add(checkBox);
}
void DebugMenu::FloatWatch( const char* label , float *toWatch )
{
			WatchInfo* watcher = new WatchInfo;
			watcher->label = label;
			watcher->localValue = toWatch;
			watcher->location = 0;
			watcher->copyFromLocalValue();
			elements.add(watcher);
}

void DebugMenu::updateFromLocalValue()
{
	for ( unsigned int i = 0; i < elements.size(); i++ )
	{
		elements.get(i)->copyFromLocalValue();
	}
}

void DebugMenu::updateToLocalValue()
{
	for ( unsigned int i = 0; i < elements.size(); i++ )
	{
		elements.get(i)->copyToLocalValue();
	}
}

void DebugMenu::drawMenu()
{
	glDisable( GL_DEPTH_TEST );
	for ( unsigned int i = 0; i < elements.size(); i++ )
	{
		elements.get(i)->draw();
	}
}

void DebugMenu::updateLocation( float toScroll )
{
	for ( unsigned int i = 0; i < elements.size(); i++ )
	{
		elements.get(i)->location = toScroll - ( ( float ) i * 0.1f );
	}
}

void DebugMenu::ElementInfo::updateLocation( float& location ) { this->location = location; }

void DebugMenu::CheckBoxInfo::copyFromLocalValue()
{
	checked = *localValue;
}
void DebugMenu::CheckBoxInfo::copyToLocalValue()
{
	*localValue = checked;
}

void DebugMenu::CheckBoxInfo::draw()
{
	glUseProgram( 0 );
	glRasterPos2f( -1 , location );
	printGL( std::string( label ).c_str() );

	glUseProgram( programID );
	GLint cuniform = glGetUniformLocation( programID , "color" );
	glUniform4f( cuniform , 1.0f , 1.0f , 1.0f , 1.0f );

	glBindVertexArray( checkBoxId );
	float translate[] = { 0 , location };
	GLint uniform = glGetUniformLocation( programID , "translate" );
	glUniform2fv( uniform , 1 , translate );
	if ( checked ) glDrawElements( GL_TRIANGLES , checkBoxNumIndices , GL_UNSIGNED_SHORT , ( GLvoid* ) checkBoxIndex );
	else glDrawElements( GL_LINES , checkBoxDisNumIndices , GL_UNSIGNED_SHORT , ( GLvoid* ) checkBoxDisIndex );
}

void DebugMenu::CheckBoxInfo::change( float& x )
{
	if ( x >= -0.025f && x <= 0.025f ) checked = checked == false;
}

void DebugMenu::SliderInfo::copyFromLocalValue() 
{
	percent = ( *localValue - min ) / ( max - min );
}
void DebugMenu::SliderInfo::copyToLocalValue()
{
	*localValue = min + ( max - min ) * ( percent );
}

void DebugMenu::SliderInfo::draw()
{
	glUseProgram( 0 );
	std::stringstream stream( std::stringstream::in | std::stringstream::out );
	stream << (min + ( max - min ) * ( percent ));

	glRasterPos2f( -1 , location );
	printGL( std::string( label + " " + stream.str() ).c_str() );

	glUseProgram( programID );
	GLint cuniform = glGetUniformLocation( programID , "color" );
	glUniform4f( cuniform , 1.0f , 1.0f , 1.0f , 1.0f );
	GLint uniform = glGetUniformLocation( programID , "translate" );

	glBindVertexArray( lineId );
	glUniform2f( uniform , 0 , location );
	glDrawElements( GL_LINES , lineNumIndecies , GL_UNSIGNED_SHORT , ( GLvoid* ) lineIndex );

	glBindVertexArray( checkBoxId );
	glUniform2f( uniform , ( *localValue - min ) / ( max - min ) , location );
	glDrawElements( GL_TRIANGLES , checkBoxNumIndices , GL_UNSIGNED_SHORT , (GLvoid*)checkBoxIndex );
}

void DebugMenu::SliderInfo::change( float& x )
{
	if ( x >= 0 ) percent = x;
	else percent = 0;
}

void DebugMenu::WatchInfo::copyFromLocalValue()
{
	std::stringstream stream( std::stringstream::in | std::stringstream::out );
	stream << *localValue;
	value = stream.str();
}

void DebugMenu::WatchInfo::copyToLocalValue() {}

void DebugMenu::WatchInfo::draw()
{
	glUseProgram( 0 );
	glRasterPos2f( -1, location );
	printGL( std::string(label + " " + value).c_str());
}

void DebugMenu::WatchInfo::change( float& x ) { x; }

void DebugMenu::recieveClick( float x , float y )
{
	int toChange = 0;

	for ( unsigned int i = 0; i < elements.size(); i++ )
	{
			if ( abs( elements.get(toChange)->location - y) > abs(elements.get(i)->location - y) )
			{
				toChange = i;
			}
	}

	if ( elements.get( toChange )->location - y < 0.025f && elements.get( toChange )->location - y > -0.025f )
	{
		elements.get(toChange)->change( x );
	}
}

void DebugMenu::recieveHold( float x , float y, bool toRefresh )
{
	static int toChange = 0;

	if ( toRefresh )
	{
		for ( unsigned int i = 0; i < elements.size(); i++ )
		{
				if ( abs( elements.get(toChange)->location - y ) > abs( elements.get(i)->location - y ) )
				{
					toChange = i;
				}
		}
	}
	else
	{
			if ( typeid( *elements.get(toChange) ) == typeid( SliderInfo ) ) elements.get(toChange)->change( x );
	}
}

void DebugMenu::cleanUp()
{
	for (unsigned int i = 0; i < elements.size(); i++ ) delete elements.get( i );
}

void DebugMenu::deleteItem( const char* label )
{
	for ( unsigned int i = 0; i < elements.size(); i++ )
	{
		if ( strcmp( label , elements.get( i )->label.c_str()) == 0 )
		{
			delete elements.get( i );
			elements.remove( i );
			break;
		}
	}
}
#endif