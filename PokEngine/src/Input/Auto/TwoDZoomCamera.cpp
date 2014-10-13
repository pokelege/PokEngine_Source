#include <Input\Auto\TwoDZoomCamera.h>
#include <Graphics\Camera.h>
#include <Core\GameObject.h>
TwoDZoomCamera::TwoDZoomCamera() : parent(0) , minDistance(1) , maxDistance(10) , zoomScale(1), tracking(0) {}

TwoDZoomCamera::~TwoDZoomCamera()
{
	destroy();
}

void TwoDZoomCamera::attatch( GameObject* parent )
{
	this->parent = parent;
}

void TwoDZoomCamera::detatch()
{
	parent = 0;
}

void TwoDZoomCamera::initialize( unsigned int numObjectsToTrack )
{
	if ( tracking ) destroy();
	numTracking = numObjectsToTrack;
	tracking = new GameObject*[numTracking];
	for ( unsigned int i = 0; i < numTracking; ++i )
	{
		tracking[i] = 0;
	}
}
void TwoDZoomCamera::destroy()
{
	if ( !tracking ) return;
	delete[] tracking;
	tracking = 0;
	numTracking = 0;
}
bool TwoDZoomCamera::initialized()
{
	return tracking != 0;
}

bool TwoDZoomCamera::addGameObjectToTrack( GameObject* object )
{
	for ( unsigned int i = 0; i < numTracking; ++i )
	{
		if ( !tracking[i] )
		{
			tracking[i] = object;
			return true;
		}
	}
	return false;
}

void TwoDZoomCamera::earlyUpdate() {}
void TwoDZoomCamera::update(){}
void TwoDZoomCamera::lateUpdate()
{
	if ( !parent || !tracking ) return;
	Camera* theCamera = parent->getComponent<Camera>();
	if ( !theCamera ) return;
	glm::vec3 averagePosition;
	unsigned int numPass = 0 ;
	for ( unsigned int i = 0; i < numTracking; ++i )
	{
		if ( tracking[i] && tracking[i]->active )
		{
			averagePosition += tracking[i]->translate;
			++numPass;
		}
	}
	averagePosition /= numPass;
	float highestDistance = minDistance;
	for ( unsigned int i = 0; i < numTracking; ++i )
	{
		if ( tracking[i] && tracking[i]->active )
		{
			float theLength = glm::length( tracking[i]->translate - averagePosition );
			if ( highestDistance < theLength )
			{
				highestDistance = theLength;
			}
		}
		if ( highestDistance > maxDistance )
		{
			highestDistance = maxDistance;
			break;
		}
	}

	parent->translate = averagePosition + ( -theCamera->direction * ( highestDistance * zoomScale ) );
}
void TwoDZoomCamera::earlyDraw() {}
void TwoDZoomCamera::draw() {}
void TwoDZoomCamera::lateDraw() {}