#pragma once
#include <PokEngineExportHeader.h>

struct POKENGINE_SHARED AnimationFrameRangeInfo
{
	unsigned int nextAnimationFrameInfo;
	unsigned int firstFrame;
	unsigned int lastFrame;
};

typedef AnimationFrameRangeInfo FrameRange;