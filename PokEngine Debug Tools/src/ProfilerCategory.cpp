#include "ProfilerCategory.h"

ProfilerCategory::ProfilerCategory()
{
	samples = new float*[0];
	numberofSamples = 0;
}

ProfilerCategory::ProfilerCategory(const char* name) : name(name)
{
	samples = new float*[0];
	numberofSamples = 0;
}

ProfilerCategory::ProfilerCategory(const char* name, const float& time) : name(name)
{
	samples = new float*[0];
	numberofSamples = 0;
	addSample(time);
}

ProfilerCategory::~ProfilerCategory()
{
	for (unsigned int i = 0; i < numberofSamples; i++)
	{
		delete samples[i];
	}

	delete[] samples;
}

void ProfilerCategory::addSample(const float& time)
{
	if (numberofSamples < MAXSAMPLES)
	{
		float** newList = new float*[numberofSamples + 1];

		for (unsigned int i = 0; i < numberofSamples; i++)
		{
			newList[i] = samples[i];
		}

		newList[numberofSamples] = new float(time);
		numberofSamples++;
		delete[] samples;
		samples = newList;
	}
}