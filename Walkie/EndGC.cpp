#include "stdafx.h"
#include "EndGC.h"

EndGC::EndGC() : anim{ "images/end.png", 7, 8, 8, 3 } {
	anim.addSection("anim", 0, 17);
	anim.setSection("anim", false);
}

void EndGC::update(std::vector<msf::Action*>& act, sf::RenderWindow & window) {
	anim.getSprite().setPosition(owner->x(), owner->y());
	anim.progress();
	window.draw(anim);
}

std::unique_ptr<msf::GraphicsComponent> EndGC::clone() {
	return std::make_unique<EndGC>(*this);
}
