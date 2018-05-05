#include "stdafx.h"
#include "PlayerCamera.h"

PlayerCamera::PlayerCamera(msf::GameObject* playerHandle_) :
	msf::Camera{ sf::View{ { 0, 0, 1920, 1080 } } },
	playerHandle{ playerHandle_} {
}

PlayerCamera::PlayerCamera(const PlayerCamera & other) :
	msf::Camera{other.view},
	playerHandle{other.playerHandle} {
}

void PlayerCamera::update() {

	sf::Vector2f playerPos{ playerHandle->getPos() };
	sf::Vector2f currentPos = view.getCenter();
	sf::Vector2f pos;
	

	if (currentPos.x - playerPos.x > width / 2) {
		pos.x = playerPos.x + width / 2;
	}
	else if (currentPos.x - playerPos.x < -(width / 2)) {
		pos.x = playerPos.x - width / 2;
	}
	else {
		pos.x = currentPos.x;
	}

	if (currentPos.y - playerPos.y > height / 2) {
		pos.y = playerPos.y + height / 2;
		readjust = true;
	}
	else if (currentPos.y - playerPos.y < -(height / 2)) {
		pos.y = playerPos.y - height / 2;
		readjust = true;
	}
	else {
		if (readjust) {
			if (currentPos.y < (playerPos.y - .4)) {
				pos.y = currentPos.y + .7;
			}
			else if (currentPos.y > (playerPos.y + .4)) {
				pos.y = currentPos.y - .7;
			}
			else {
				readjust = false;
				pos.y = playerPos.y;
			}
		}
		else {
			pos.y = currentPos.y;
		}
	}
	view.setCenter(pos);
}
