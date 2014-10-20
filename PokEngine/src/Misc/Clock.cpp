#include <Misc\Clock.h>
Clock Clock::globalClock;
Clock::Clock()
{
	QueryPerformanceFrequency(&clockSpeed);
}

void Clock::Start()
{
	QueryPerformanceCounter(&startTick);
	QueryPerformanceCounter(&intervalTick);
}

float Clock::Stop()
{
	QueryPerformanceCounter(&intervalTick);
	float result = (float)(intervalTick.QuadPart - startTick.QuadPart) / (float)clockSpeed.QuadPart;
	QueryPerformanceCounter(&startTick);
	return result;
}

float Clock::Interval()
{
	LARGE_INTEGER lastinterval = intervalTick;
	QueryPerformanceCounter(&intervalTick);
	return (float)(intervalTick.QuadPart - lastinterval.QuadPart) / (float)clockSpeed.QuadPart;
}

float Clock::fromStart()
{
	LARGE_INTEGER lastinterval;
	QueryPerformanceCounter(&lastinterval);
	return (float)(lastinterval.QuadPart - startTick.QuadPart) / (float)clockSpeed.QuadPart;
}

float Clock::dt = 0;
void Clock::update()
{
	Clock::dt = Clock::globalClock.Interval();
}