#pragma once
#ifdef	POKENGINE_EXPORTS
#define POKENGINE_SHARED __declspec( dllexport )
#else
#define POKENGINE_SHARED __declspec( dllimport )
#endif