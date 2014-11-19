#include <Graphics\CommonGraphicsCommands.h>
#include <Graphics\GraphicsBufferManager.h>
#include <Graphics\GraphicsShaderManager.h>
#include <Graphics\GraphicsGeometryManager.h>
#include <Graphics\GraphicsSharedUniformManager.h>
#include <Graphics\GraphicsTextureManager.h>
#include <Graphics\GraphicsRenderingManager.h>
#include <Graphics\GraphicsCameraManager.h>
#include <Graphics\GraphicsLightManager.h>
#include <GL\glew.h>
int CommonGraphicsCommands::base = 0;
void CommonGraphicsCommands::initialize()
{
	glewInit();
}

void CommonGraphicsCommands::initializeGlobalGraphics()
{
	initialize();
	GraphicsBufferManager::globalBufferManager.initialize();
	GraphicsShaderManager::globalShaderManager.initialize();
	GraphicsGeometryManager::globalGeometryManager.initialize();
	GraphicsSharedUniformManager::globalSharedUniformManager.initialize();
	GraphicsTextureManager::globalTextureManager.initialize();
	GraphicsRenderingManager::globalRenderingManager.initialize();
	GraphicsCameraManager::globalCameraManager.initialize();
	GraphicsLightManager::global.initialize();
	GraphicsSharedUniformManager::globalSharedUniformManager.lights = &GraphicsLightManager::global;
}

void CommonGraphicsCommands::destroyGlobalGraphics()
{
	GraphicsLightManager::global.destroy();

	GraphicsCameraManager::globalCameraManager.destroy();

	GraphicsRenderingManager::globalRenderingManager.destroy();

	GraphicsTextureManager::globalTextureManager.destroy();

	GraphicsSharedUniformManager::globalSharedUniformManager.destroy();

	GraphicsGeometryManager::globalGeometryManager.destroy();

	GraphicsShaderManager::globalShaderManager.destroy();

	GraphicsBufferManager::globalBufferManager.destroy();
}

void CommonGraphicsCommands::initializePrinting( HDC hdc , int sizex , int sizey , int fontWeight )
{
	if ( glIsList( base ) ) glDeleteLists( base , 96 );
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists( 96 );								// Storage For 96 Characters

	font = CreateFont( -sizex ,							// Height Of Font
					   sizey ,								// Width Of Font
					   0 ,								// Angle Of Escapement
					   0 ,								// Orientation Angle
					   fontWeight ,						// Font Weight
					   FALSE ,							// Italic
					   FALSE ,							// Underline
					   FALSE ,							// Strikeout
					   ANSI_CHARSET ,					// Character Set Identifier
					   OUT_TT_PRECIS ,					// Output Precision
					   CLIP_DEFAULT_PRECIS ,			// Clipping Precision
					   ANTIALIASED_QUALITY ,			// Output Quality
					   FF_DONTCARE | DEFAULT_PITCH ,		// Family And Pitch
					   "Courier New" );					// Font Name

	oldfont = ( HFONT ) SelectObject( hdc , font );           // Selects The Font We Want
	wglUseFontBitmaps( hdc , 32 , 96 , base );				// Builds 96 Characters Starting At Character 32
	SelectObject( hdc , oldfont );							// Selects The Font We Want
	DeleteObject( font );
}

void CommonGraphicsCommands::clear( unsigned int toClear )
{
	glClear( toClear );
}

void CommonGraphicsCommands::clearCommon()
{
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
}

void CommonGraphicsCommands::enable( unsigned int toEnable )
{
	glEnable( toEnable );
}

void CommonGraphicsCommands::setViewPort( int x , int y , int width , int height )
{
	glViewport( x , y , width , height );
	glScissor( x , y , width , height );
	glEnable( GL_SCISSOR_TEST );
}

void CommonGraphicsCommands::printString( const char* string , const float& x , const float& y , glm::vec4 color )
{
	if ( glIsList( base ) )
	{
		glUseProgram( 0 );
		glRasterPos2f( x , y );
		glColor4f( color.x , color.y , color.z , color.w );
		glPushAttrib( GL_LIST_BIT );
		glListBase( base - 32 );
		glCallLists( strlen( string ) , GL_UNSIGNED_BYTE , string );
		glPopAttrib();
	}
}

void CommonGraphicsCommands::resetGlobalGraphics()
{
	GraphicsBufferManager::globalBufferManager.destroy();
	GraphicsShaderManager::globalShaderManager.destroy();
	GraphicsGeometryManager::globalGeometryManager.destroy();
	GraphicsSharedUniformManager::globalSharedUniformManager.destroy();
	GraphicsTextureManager::globalTextureManager.destroy();
	GraphicsLightManager::global.destroy();
	initializeGlobalGraphics();
}