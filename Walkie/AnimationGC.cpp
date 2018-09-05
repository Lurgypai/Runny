#include "stdafx.h"
#include "AnimationGC.h"

AnimationGC::AnimationGC(const std::string& path, int width, int height, int columns, int frameDelay, int frames, int offsetx_, int offsety_) :
	anim{ path, width, height, columns, frameDelay },
	offsetx{offsetx_},
	offsety{offsetx_} {
	anim.addSection("anim", 0, frames - 1);
	anim.setSection("anim", false);
}

void AnimationGC::update(std::vector<msf::Action*>& act, sf::RenderWindow & window) {
	anim.getSprite().setPosition(owner->x() - offsetx, owner->y() - offsety);
	anim.progress();
	window.draw(anim);
}

std::unique_ptr<msf::GraphicsComponent> AnimationGC::clone() {
	return std::make_unique<AnimationGC>(*this);
}
