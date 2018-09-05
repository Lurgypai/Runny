#include "stdafx.h"
#include "CSound.h"

CSound::CSound(const std::string filePath) {
	sb.loadFromFile(filePath);
	s.setBuffer(sb);
}

CSound::CSound() {}

void CSound::play(float volume) {
	if (volume > 100.0f)
		volume = 100.0f;
	else if (volume < 0.0f)
		volume = 0.0f;
	s.setVolume(volume);
	s.play();
}

const sf::Sound & CSound::getSound() const {
	return s;
}
