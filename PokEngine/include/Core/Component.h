#pragma once
#include <PokEngineExportHeader.h>
class GameObject;
class POKENGINE_SHARED Component
{
protected:
	friend class GameObject;
	virtual void attatch( GameObject* parent ) = 0;
	virtual void detatch() = 0;
public:
	virtual void earlyUpdate() = 0;
	virtual void update() = 0;
	virtual void lateUpdate() = 0;
	virtual void earlyDraw() = 0;
	virtual void draw() = 0;
	virtual void lateDraw() = 0;
	virtual ~Component() = 0;
};