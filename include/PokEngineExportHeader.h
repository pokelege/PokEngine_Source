#pragma once
#ifdef	POKENGINE_EXPORTS
#define POKENGINE_SHARED __declspec( dllexport )
#ifdef _DEBUG
#include <crtdbg.h>

#define DEBUG_NEW new(_CLIENT_BLOCK, __FILE__, __LINE__)

#define new DEBUG_NEW

#endif
#else
#define POKENGINE_SHARED __declspec( dllimport )
#endif