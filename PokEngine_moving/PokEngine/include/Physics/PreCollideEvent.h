#pragma once
#include <PokEngineExportHeader.h>
#include <Core\Component.h>
class Particle;
class POKENGINE_SHARED PreCollideEvent :public Component
{
public:
	virtual bool preCollideEvent( Particle* particle ) = 0;
};