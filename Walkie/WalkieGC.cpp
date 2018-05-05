#include "stdafx.h"
#include "WalkieGC.h"
#include "WalkieC.h"
#include <iostream>

WalkieGC::WalkieGC(const Animation& animation_) :
	animation{animation_} {
}

WalkieGC::WalkieGC(const WalkieGC & other) :
	animation{ other.animation }
{
}

WalkieGC::~WalkieGC() {}


void WalkieGC::update(std::vector<msf::Action *>& acts, sf::RenderWindow & window) {
	animation.getSprite().setPosition(owner->getPos().x-4, owner->getPos().y - 1);
	AnimationFlag prevState = state;
	for (auto& act : acts) {
		state = static_cast<AnimationFlag>(act->id);
	}
	if (prevState != state) {
		switch (state)
		{
		case WalkieGC::MoveLeft:
			animation.setSection("moveleft", false);
			break;
		case WalkieGC::MoveRight:
			animation.setSection("moveright", false);
			break;
		case WalkieGC::Stopped:
			animation.setSection("stop", false);
			break;
		case WalkieGC::InMidair:
			animation.setSection("midair", false);
			break;
		case WalkieGC::RollRight:
			animation.setSection("rollright", false);
			break;
		case WalkieGC::RollLeft:
			animation.setSection("rollleft", false);
			break;
		case WalkieGC::Dropping:
			animation.setSection("drop", false);
			break;
		default:
			break;
		}
	}
	animation.progress();
	window.draw(animation);
}

std::unique_ptr < msf::GraphicsComponent > WalkieGC::clone() {
	return std::make_unique<WalkieGC>(*this);
}
