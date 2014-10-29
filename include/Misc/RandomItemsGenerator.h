#pragma once
#pragma warning ( disable : 4201)

#include <PokEngineExportHeader.h>
#include <glm.hpp>

namespace Engine
{
	struct POKENGINE_SHARED RandomItemsGenerator
	{
		static const float PI2;

		static void setSeed(int seed = -1);

		static float RandomFloat();

		static float RandomRangedFloat(const float& min, const float& max);

		static int RandomRangedInt( const int& min , const int& max );

		static glm::vec4 RandomVector();
	};	
}