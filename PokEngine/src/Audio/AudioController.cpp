#include <Audio\AudioController.h>
#include <irrKlang.h>

AudioController::AudioController() : soundEngine(0)
{

}
bool AudioController::initialize()
{
	if ( soundEngine ) destroy();
	soundEngine = irrklang::createIrrKlangDevice();
	return soundEngine != 0;
}

void AudioController::destroy()
{
	if ( !soundEngine ) return;
	soundEngine->drop();
}
bool AudioController::initialized()
{
	return soundEngine != 0;
}

irrklang::ISound* AudioController::playSound( const char* filename , bool looping, bool pause, bool track )
{
	if ( !soundEngine ) return 0;
	return soundEngine->play2D( filename, looping, pause, track );
}

irrklang::ISound* AudioController::playSound3D( const char* filename , bool looping , bool pause , bool track , glm::vec3 position , float minDistance , float maxDistance )
{
	if ( !soundEngine ) return 0;
	irrklang::ISound* toreturn = soundEngine->play3D( filename , irrklang::vec3df(position.x, position.y, position.z) , looping , pause , track );
	toreturn->setMinDistance(minDistance);
	toreturn->setMaxDistance(maxDistance);
	return toreturn;
}