#include <Graphics\AnimationRenderingInfo.h>
#include <Graphics\BoneInfo.h>
#include <Graphics\PokEngineModelDataMap.h>
#include <Graphics\AnimationInfo.h>
#include <gtc\quaternion.hpp>
#include <gtc\matrix_transform.hpp>
#include <Misc\Clock.h>
#include <Graphics\RenderableInfo.h>
#include <Core\GameObject.h>
#include <Graphics\GeometryInfo.h>
#include <Graphics\CommonUniformNames.h>

AnimationRenderingInfo::AnimationRenderingInfo() :parent(0) , animationMatrices(0) , animationFrameRate(30) , currentFrame(0) , renderable(0)
{

}
void AnimationRenderingInfo::attatch( GameObject* parent )
{
	this->parent = parent;
	if ( !renderable )
	{
		renderable = parent->getComponent<Renderable>();
	}
}
void AnimationRenderingInfo::detatch()
{
	parent = 0;
}
void AnimationRenderingInfo::earlyUpdate() {}
void AnimationRenderingInfo::update()
{
	if ( !parent ) return;
	if ( !renderable )
	{
		renderable = parent->getComponent<Renderable>();
		if( !renderable ) return;
	}
	if ( !renderable->geometryInfo ) return;
	unsigned int boneDataSize;
	BoneInfo* bones = renderable->geometryInfo->modelData->getBoneData( &boneDataSize );
	if ( bones && boneDataSize > 0 )
	{
		if ( !animationMatrices )
		{
			animationMatrices = new glm::mat4[boneDataSize];
			sizeofAnimationMatrices = boneDataSize;
		}
		currentFrame += animationFrameRate * Clock::dt;
		glm::mat4 mainTransform;
		updateAnimationMatricesRecurse( 0 , bones , mainTransform );

		renderable->setRenderableUniform( ANIMATIONMATRICES , PT_MAT4 , reinterpret_cast< const void* >( animationMatrices ), sizeofAnimationMatrices );
	}
}

void AnimationRenderingInfo::lateUpdate() {}
void AnimationRenderingInfo::earlyDraw() {}
void AnimationRenderingInfo::draw() {}
void AnimationRenderingInfo::lateDraw() {}

void AnimationRenderingInfo::updateAnimationMatricesRecurse( unsigned int boneIndex , BoneInfo* bones , glm::mat4& parentMatrix )
{
	glm::mat4 animateTransform;
	if ( bones[boneIndex].animationSize() > 0 )
	{
		AnimationInfo* start = 0;
		AnimationInfo* end = 0;

		unsigned int animationEndTime = bones[boneIndex].getAnimation( bones[boneIndex].animationSize() - 1 , *renderable->geometryInfo->modelData )->frame;
		while ( currentFrame > animationEndTime ) currentFrame -= animationEndTime;
		if ( currentFrame < 1 ) currentFrame = 1;
		for ( unsigned int i = 0; i < bones[boneIndex].animationSize(); ++i )
		{
			AnimationInfo* test = bones[boneIndex].getAnimation( i , *renderable->geometryInfo->modelData );
			if ( test->frame <= ( unsigned int ) currentFrame )
			{
				start = test;
			}
			if (start != test && test->frame >= ( unsigned int ) currentFrame )
			{
				end = test;
				break;
			}
		}
		float interpolation = currentFrame;
		glm::vec3 lerpedTranslate;
		glm::vec3 lerpedScale;
		glm::vec3 lerpedRotation;
		if ( start )
		{
			interpolation = ( interpolation - start->frame ) / ( end->frame - start->frame );
			lerpedTranslate = ( ( 1 - interpolation ) * start->translation ) + ( interpolation * end->translation );
			lerpedScale = ( ( 1 - interpolation ) * start->scale ) + ( interpolation * end->scale );
			lerpedRotation = ( ( 1 - interpolation ) * start->rotation ) + ( interpolation * end->rotation );
		}
		else
		{
			interpolation /= end->frame;
			lerpedTranslate = ( interpolation * end->translation );
			lerpedScale = ( ( 1 - interpolation ) * glm::vec3( 1 , 1 , 1 ) ) + ( interpolation * end->scale );
			lerpedRotation = ( interpolation * end->rotation );
		}

		glm::quat quaternion = glm::rotate( glm::quat() , lerpedRotation.x , glm::vec3( 1 , 0 , 0 ) ) *
			glm::rotate( glm::quat() , lerpedRotation.y , glm::vec3( 0 , 1 , 0 ) ) *
			glm::rotate( glm::quat() , lerpedRotation.z , glm::vec3( 0 , 0 , 1 ) );

		animateTransform = parentMatrix * ( glm::translate( glm::mat4() , lerpedTranslate ) * glm::mat4_cast( quaternion ) * glm::scale( glm::mat4() , lerpedScale ) );
		animationMatrices[boneIndex] = animateTransform * bones[boneIndex].offsetMatrix;
	}
	else
	{
		animateTransform = parentMatrix;
		animationMatrices[boneIndex] = animateTransform * bones[boneIndex].offsetMatrix;
	}

	for ( unsigned int i = 0; i < bones[boneIndex].childrenSize(); ++i )
	{
		updateAnimationMatricesRecurse( renderable->geometryInfo->modelData->getBoneChildren()[bones[boneIndex].childDataStart + i] , bones , animateTransform );
	}
}

AnimationRenderingInfo::~AnimationRenderingInfo()
{
	if ( animationMatrices ) delete[] animationMatrices;
}