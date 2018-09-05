#pragma once
#include "CSound.h"

class SoundSystem {
public:
	SoundSystem(float volume_);
	void registerSound(const std::string & id, const std::string & filePath);
	void playSound(const std::string & id);
private:
	float volume;
	std::unordered_map<std::string, CSound> sounds;
};