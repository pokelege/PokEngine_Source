#pragma once
#include <PokEngineExportHeader.h>

struct POKENGINE_SHARED AnimationFrameRangeInfo
{
	unsigned int nextAnimationFrameInfo;
	unsigned int firstFrame;
	unsigned int lastFrame;

	AnimationFrameRangeInfo() : nextAnimationFrameInfo(0) , firstFrame(0) , lastFrame(0) {}
};

typedef AnimationFrameRangeInfo FrameRange;