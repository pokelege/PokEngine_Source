#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <Windows.h>
#include <glm.hpp>
struct POKENGINE_SHARED CommonGraphicsCommands
{
	static int base;
	static void initialize();
	static void initializeGlobalGraphics();
	static void initializePrinting( HDC hdc , int sizex = 24 , int sizey = 0 , int fontWeight = FW_BOLD );
	static void clear( unsigned int toClear );
	static void clearCommon();
	static void enable( unsigned int toEnable );
	static void setViewPort( int x , int y , int width , int height );

	static void printString( const char* string , const float& x = -1 , const float& y = 0 , glm::vec4 color = glm::vec4( 1 , 1 , 1 , 1 ) );
	static void resetGlobalGraphics();
};