#pragma once
#ifdef DEBUGCONFIGFILE
#include <PokEngineDebugExportHeader.h>
#include <string>
class POKENGINEDEBUGTOOLS_SHARED DebugConfigFile
{
	std::string* linkedVariables;
};

#else
#endif