#include "Misc\RandomItemsGenerator.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime>
namespace Engine
{
	const float RandomItemsGenerator::PI2 = (float)M_PI * 2;

	void RandomItemsGenerator::setSeed(int seed)
	{
		if ( seed < 0 )
		{
			srand( clock() );
		}
		else
		{
			srand( seed );
		}
	}

	float RandomItemsGenerator::RandomFloat()
	{
		return (float)rand() / RAND_MAX;
	}

	float RandomItemsGenerator::RandomRangedFloat(const float& min, const float& max)
	{
		if (max < min)
		{
			return (RandomFloat() * (min - max)) + max;
		}
		else
		{
			return (RandomFloat() * (max - min)) + min;
		}
	}

	int RandomItemsGenerator::RandomRangedInt( const int& min , const int& max )
	{
		if ( max < min )
		{
			return (int)(( RandomFloat() * ( min - max ) ) + max + 0.5f);
		}
		else
		{
			return (int)(( RandomFloat() * ( max - min ) ) + min + 0.5f);
		}
	}

	glm::vec4 RandomItemsGenerator::RandomVector()
	{
		return glm::vec4(RandomRangedFloat(-1, 1), RandomRangedFloat(-1, 1), RandomRangedFloat(-1, 1), RandomRangedFloat(-1, 1));
	}
}