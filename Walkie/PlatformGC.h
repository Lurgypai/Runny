#pragma once
#include "MSF.h"

class PlatformGC : public msf::GraphicsComponent {
public:
	PlatformGC(float width, float height);
	PlatformGC(const PlatformGC& other);
	// Inherited via GraphicsComponent
	void update(std::vector<msf::Action*>& act, sf::RenderWindow & window) override;
	std::unique_ptr<GraphicsComponent> clone() override;
private:
	sf::RectangleShape sprite;
};