#include "stdafx.h"
#include "SoundSystem.h"

SoundSystem::SoundSystem(float volume_) : 
	volume{volume_}
{
}

void SoundSystem::registerSound(const std::string & id, const std::string & filePath) {
	sounds.emplace(id, filePath);
}

void SoundSystem::playSound(const std::string & id) {
	sounds[id].play(volume);
}
