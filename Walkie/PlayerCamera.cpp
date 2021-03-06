#include "stdafx.h"
#include "PlayerCamera.h"
#include "WalkieLC.h"

PlayerCamera::PlayerCamera() :
	msf::Camera{ sf::View{ { 0, 0, 480, 270 } } },
	playerHandle{ nullptr } {
}

PlayerCamera::PlayerCamera(const PlayerCamera & other) :
	msf::Camera{other.view},
	playerHandle{other.playerHandle} {
}

void PlayerCamera::setPlayerHandle(msf::GameObject * playerHandle_) {
	playerHandle = playerHandle_;
}

void PlayerCamera::update() {

	sf::Vector2f playerPos{ playerHandle->getPos() };
	sf::Vector2f currentPos = view.getCenter();
	sf::Vector2f pos;
	
	WalkieLC *playerLogic = static_cast<WalkieLC*>(playerHandle->getLogic());
	
	if (!playerLogic->isSpawning()) {
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
	else {
		view.setCenter(playerLogic->getSpawnPoint());
	}
}
