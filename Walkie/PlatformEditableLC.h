#pragma once
#include "MSF.h"

class PlatformEditable : public msf::LogicComponent {
public:
	PlatformEditable(sf::RenderWindow *window, const sf::View* viewHandle_, float width, float height);
	PlatformEditable(const PlatformEditable &PlatformEditable);
	virtual void update() override;
	virtual std::unique_ptr<LogicComponent> clone() override;
protected:
	bool prevLClick;
	bool prevRClick;
	bool isHeld;
	sf::RenderWindow* windowHandle;
	const sf::View* viewHandle;
	float xOffset;
	float yOffset;
};