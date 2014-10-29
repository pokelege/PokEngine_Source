#pragma once
#include <PokEngineExportHeader.h>
#include <windows.h>

class POKENGINE_SHARED Clock
{
	LARGE_INTEGER clockSpeed, startTick, intervalTick;

public:
	Clock();

	void Start();
	float Stop();
	float Interval();
	float fromStart();

	static Clock globalClock;
	static float dt;
	static void update();
};