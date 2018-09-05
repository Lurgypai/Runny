#include "stdafx.h"
#include "BuildCamera.h"

BuildCamera::BuildCamera() :
	msf::Camera{ sf::View{ { 0, 0, 480, 270 } } },
	moveSpeed{5}, 
	zoomSpeed{0.05f}
{
	view.setCenter(0, 0);
}

void BuildCamera::update() {

	float xMove{}, yMove{};
	float zoom{1.0};
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		yMove -= moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		yMove += moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		xMove -= moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		xMove += moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		zoom += zoomSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		zoom -= zoomSpeed;
	}

	if (xMove != 0 || yMove != 0)
		view.move(xMove, yMove);

	view.zoom(zoom);
}
