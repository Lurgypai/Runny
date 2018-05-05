#include "stdafx.h"
#include "PlatformGC.h"

PlatformGC::PlatformGC(float width, float height) : sprite{sf::Vector2f{width, height} } {
	sprite.setFillColor(sf::Color::White);
}

PlatformGC::PlatformGC(const PlatformGC & other) : sprite{other.sprite} {
}

void PlatformGC::update(std::vector<msf::Action*>& act, sf::RenderWindow & window)	{
	sprite.setPosition(owner->getPos());
	window.draw(sprite);
}

std::unique_ptr<msf::GraphicsComponent> PlatformGC::clone() {

	return std::make_unique<PlatformGC>(*this);
}
