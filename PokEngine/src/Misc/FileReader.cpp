#include "Misc\FileReader.h"
#include <fstream>
std::string FileReader(const char* filename)
{
	std::ifstream stream(filename);
	if (stream.good())
	{
		std::string filelines((std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>());
		return filelines;
	}
	else
	{
		return "";
	}
}