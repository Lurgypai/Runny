#pragma once
#include "MSF.h"
#include "Animation.h"

class EndGC : public msf::GraphicsComponent {
public:
	EndGC();
	void update(std::vector<msf::Action*>& act, sf::RenderWindow & window) override;
	std::unique_ptr<GraphicsComponent> clone() override;
private:
	Animation anim;
};