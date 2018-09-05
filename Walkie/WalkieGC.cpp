#include "stdafx.h"
#include "WalkieGC.h"
#include "WalkieC.h"
#include "HbarAction.h"
#include <iostream>

WalkieGC::WalkieGC(const Animation& animation_) :
	animation{ animation_ },
	lbar{ {10, 2} } {
	lbar.setFillColor(sf::Color::White);
}

WalkieGC::WalkieGC(const WalkieGC & other) :
	animation{ other.animation }
{
}

WalkieGC::~WalkieGC() {}


void WalkieGC::update(std::vector<msf::Action *>& acts, sf::RenderWindow & window) {
	animation.getSprite().setPosition(owner->getPos().x-4, owner->getPos().y - 1);
	lbar.setPosition(owner->getPos().x - (lbar.getGlobalBounds().width / 2) + 1.5, owner->getPos().y - 5 - (lbar.getGlobalBounds().height / 2));
	AnimationFlag prevState = state;
	for (auto act : acts) {
		if (static_cast<AnimationFlag>(act->id) == WalkieGC::Hbar) {
			HbarAction * healthAction = static_cast<HbarAction*>(act);
			float health = healthAction->health;
			if (health > 1800)
				health = 1800;
			lbar.setSize({ health / 50.0f, 2 });
		}
		else {
			state = static_cast<AnimationFlag>(act->id);
		}
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
	window.draw(lbar);
}

std::unique_ptr < msf::GraphicsComponent > WalkieGC::clone() {
	return std::make_unique<WalkieGC>(*this);
}
