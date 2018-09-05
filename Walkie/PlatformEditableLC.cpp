#include "stdafx.h"
#include "PlatformEditableLC.h"
#include "SquareShape.h"
#include "SquareCollider.h"

PlatformEditable::PlatformEditable(sf::RenderWindow * window, const sf::View* viewHandle_, float width, float height) :
	viewHandle{viewHandle_},
	prevLClick{false},
	prevRClick{false} {
	setCollider<SquareCollider>(-100, -100, width, height);
	windowHandle = window;
}

PlatformEditable::PlatformEditable(const PlatformEditable & PlatformEditable) : viewHandle{PlatformEditable.viewHandle} {
}

void PlatformEditable::update() {
	SquareShape* shape = static_cast<SquareShape*>(collide->getShape());
	shape->setLeft(owner->x());
	shape->setTop(owner->y());

	sf::Vector2f mousePos = sf::Vector2f{windowHandle->mapPixelToCoords( sf::Mouse::getPosition(*windowHandle), *viewHandle) };

	bool currentLClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if (currentLClick != prevLClick) {
		//toggled to on
		if (currentLClick) {
			if (shape->getBoundingBox().contains(mousePos)) {
				isHeld = true;
				xOffset = mousePos.x - shape->getBoundingBox().left;
				yOffset = mousePos.y - shape->getBoundingBox().top;
			}
		}
		//toggled to off
		else {
			isHeld = false;
		}
		prevLClick = currentLClick;
	}
	if (isHeld) {
		owner->setPos(static_cast<int>(mousePos.x - xOffset), static_cast<int>(mousePos.y - yOffset));
	}

	bool currentRClick = sf::Mouse::isButtonPressed(sf::Mouse::Right);
	if (currentRClick != prevRClick) {
		//toggled on
		if (currentRClick) {
			if (shape->getBoundingBox().contains(mousePos)) {
				owner->destroy();
			}
		}
		//toggled off
		else {

		}
	}
}

std::unique_ptr<msf::LogicComponent> PlatformEditable::clone() {
	return std::make_unique<PlatformEditable>(*this);
}
