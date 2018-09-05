#pragma once
#include "MSF.h"
#include "Animation.h"

class AnimationGC : public msf::GraphicsComponent {
public:
	AnimationGC(const std::string& path, int width, int height, int columns, int frameDelay, int frames, int offsetx_ = 0, int offsety_ = 0);
	void update(std::vector<msf::Action*>& act, sf::RenderWindow & window) override;
	std::unique_ptr<GraphicsComponent> clone() override;
private:
	Animation anim;
	int offsetx;
	int offsety;
};