#pragma once
#include <PokEngineExportHeader.h>
enum POKENGINE_SHARED ParameterType
{
	// These values matter:
	PT_INT = ( int )sizeof( int ) * -1 ,
	PT_FLOAT = sizeof( float ) * 1 ,
	PT_IVEC2 = ( int )sizeof( int ) * -2 ,
	PT_VEC2 = sizeof( float ) * 2 ,
	PT_IVEC3 = ( int )sizeof( int ) * -3 ,
	PT_VEC3 = sizeof( float ) * 3 ,
	PT_IVEC4 = ( int )sizeof( int ) * -4 ,
	PT_VEC4 = sizeof( float ) * 4 ,
	//PT_IMAT3 = -sizeof(int) * 9,
	PT_MAT3 = sizeof( float ) * 9 ,
	//PT_IMAT4 = -sizeof(int) * 16,
	PT_MAT4 = sizeof( float ) * 16 ,
};