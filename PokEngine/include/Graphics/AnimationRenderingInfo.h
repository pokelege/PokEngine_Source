#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <glm.hpp>
#include <Core\Component.h>
struct BoneInfo;
struct RenderableInfo;
struct POKENGINE_SHARED AnimationRenderingInfo : public Component
{
private:
	GameObject* parent;
	RenderableInfo* renderable;
	void updateAnimationMatricesRecurse( unsigned int boneIndex , BoneInfo* bones , glm::mat4& parentMatrix );
protected:
	friend class GameObject;
	virtual void attatch( GameObject* parent ) ;
	virtual void detatch();
public:
	AnimationRenderingInfo();
	virtual void earlyUpdate();
	virtual void update();
	virtual void lateUpdate();
	virtual void earlyDraw();
	virtual void draw();
	virtual void lateDraw();

	glm::mat4* animationMatrices;
	unsigned int sizeofAnimationMatrices;
	unsigned int animationFrameRate;
	float currentFrame;
};

typedef AnimationRenderingInfo Animator;