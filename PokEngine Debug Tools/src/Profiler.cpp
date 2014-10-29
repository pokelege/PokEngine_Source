#include "Profiler.h"

Profiler::Profiler(const char* fileName):
fileName(fileName),
numberCategories(0)
{
	categories = new ProfilerCategory*[0];
}

Profiler::~Profiler()
{
	if (numberCategories > 0)
	{
		std::ofstream file(fileName);
		for (unsigned int i = 0; i < numberCategories; i++)
		{
			file << categories[i]->name << ',';
		}


		file << '\n';

		for (unsigned int i = 0; i < categories[0]->numberofSamples; i++)
		{
			for (unsigned int j = 0; j < numberCategories; j++)
			{
				file << *(categories[j]->samples[i]) << ',';
			}
			file << '\n';
		}

		file.close();
	}
	for (unsigned int i = 0; i < numberCategories; i++)
	{
		delete categories[i];
	}

	delete[] categories;
}

void Profiler::addEntry(const char* category, const float& time)
{
	if (numberCategories != 0)
	{
		for (unsigned int i = 0; i < numberCategories; i++)
		{
			std::string categoryname = categories[i]->name;
			if (categoryname.compare(category) == 0)
			{
				categories[i]->addSample(time);
				return;
			}
		}
	}

	if (numberCategories < MAXCATEGORIES)
	{
		ProfilerCategory** newList = new ProfilerCategory*[numberCategories + 1];

		for (unsigned int i = 0; i < numberCategories; i++)
		{
			newList[i] = categories[i];
		}

		newList[numberCategories] = new ProfilerCategory(category, time);
		numberCategories++;
		delete[] categories;
		categories = newList;
	}
}