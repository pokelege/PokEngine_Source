#pragma once
#ifdef LOGGING

#include <vector>
#include <PokEngineDebugExportHeader.h>
using std::vector;
using std::string;

enum Severity { Info, Warning, Error, Severe };

#define LOG( severity, message) Logger::Log( severity, message, __FILE__, __LINE__ );
#define END_LOG Logger::Shutdown();

#define ASSERT(expression, ...) if (!(expression)) {Logger::Log(Severe, #expression ## " " ## __VA_ARGS__, __FILE__, __LINE__); Logger::Shutdown(); exit(1);} 

#pragma warning ( disable : 4100)

template class POKENGINEDEBUGTOOLS_SHARED vector <string>;
template class POKENGINEDEBUGTOOLS_SHARED vector <Severity>;

class POKENGINEDEBUGTOOLS_SHARED Logger
{
public:
	Logger(void);
	~Logger(void);
	static void StringReplace(string& str, const string& from, const string& to);
	static string Sanitize(string str);
	static void Log( Severity severity, const char* message, const char * logFile, int logLine);
	static void Shutdown();
private:
	static vector <string> logList;
	static vector <Severity> severityList;
	static void WriteFile(const char* filename);
};

#pragma warning ( default : 4100)

#else

#define LOG( severity, message)
#define END_LOG

#define ASSERT(expression, ...)

#endif