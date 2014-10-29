#pragma once
#include "ExportHeader.h"
#include "DebugMemory.h"
#include "ProfilerCategory.h"
#include <fstream>
using std::ifstream;

class ENGINE_SHARED Profiler
{
//#define MAXCATEGORIES ((UINT_MAX / sizeof(ProfilerCategory)) / 10)
#define MAXCATEGORIES 10

	ProfilerCategory** categories;
	unsigned int numberCategories;

	const char* fileName;
public:
	Profiler(const char* fileName);
	~Profiler();
	void addEntry(const char* category, const float& time);
};