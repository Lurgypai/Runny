#pragma once
#include "MSF.h"
#include "SFML/Audio.hpp"

class CSound {
public:
	CSound(const std::string filePath);
	CSound();
	void play(float volume);
	const sf::Sound& getSound() const;
private:
	sf::Sound s;
	sf::SoundBuffer sb;
};