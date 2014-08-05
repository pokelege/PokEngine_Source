#pragma once
#include "ExportHeader.h"
#include "DebugMemory.h"
#include <limits.h>

struct ENGINE_SHARED ProfilerCategory
{
//#define MAXSAMPLES ((UINT_MAX / sizeof(float)) / 10)
#define MAXSAMPLES 10
	const char* name;
	unsigned int numberofSamples;
	float** samples;

	ProfilerCategory();
	ProfilerCategory(const char* name);
	ProfilerCategory(const char* name, const float& time);
	~ProfilerCategory();
	void addSample(const float& time);
};