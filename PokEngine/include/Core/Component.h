#pragma once
#include <PokEngineExportHeader.h>
class GameObject;
class POKENGINE_SHARED Component
{
protected:
	friend class GameObject;
	GameObject* parent;
	virtual void attatch( GameObject* parent )
	{
		this->parent = parent;
	};
	virtual void detatch()
	{
		parent = 0;
	};
public:
	virtual void earlyUpdate() {};
	virtual void update() {};
	virtual void lateUpdate() {};
	virtual void earlyDraw() {};
	virtual void lateDraw() {};
	virtual ~Component() {};
};