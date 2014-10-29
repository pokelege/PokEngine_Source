#pragma once

#ifdef	POKENGINEDEBUGTOOLS_EXPORTS
#define POKENGINEDEBUGTOOLS_SHARED __declspec( dllexport )
#else
#define POKENGINEDEBUGTOOLS_SHARED __declspec( dllimport )
#endif