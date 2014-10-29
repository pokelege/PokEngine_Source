#pragma once
#include <Core\CommonWarningDisables.h>
#include <PokEngineExportHeader.h>
#include <glm.hpp>
namespace irrklang
{
	class ISoundEngine;
	class ISound;
}
class POKENGINE_SHARED AudioController
{
	irrklang::ISoundEngine* soundEngine;
public:
	AudioController();
	bool initialize();
	void destroy();
	bool initialized();
	irrklang::ISound* playSound(const char* filename, bool looping = false, bool pause = false, bool track = false);
	irrklang::ISound* playSound3D( const char* filename , bool looping = false , bool pause = false , bool track = false, glm::vec3 position = glm::vec3(), float minDistance = 0, float maxDistance = 10  );
	static AudioController globalAudioController;
};