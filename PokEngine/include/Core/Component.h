#pragma once
#include <PokEngineExportHeader.h>
class GameObject;
class POKENGINE_SHARED Component
{
protected:
	friend class GameObject;
	virtual void attatch( GameObject* parent );
	virtual void detatch();
public:
	virtual void earlyUpdate() = 0;
	virtual void update() = 0;
	virtual void lateUpdate() = 0;
	virtual ~Component() = 0;
};