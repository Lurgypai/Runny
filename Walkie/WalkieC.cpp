#include "stdafx.h"
#include "WalkieC.h"


WalkieC::WalkieC() {}

WalkieC::~WalkieC() {}

void WalkieC::readInputs() {
	using Keyb = sf::Keyboard;
	//up
	if (!up && Keyb::isKeyPressed(Keyb::Space)) {
		buffer.emplace_back(Input::upd, false);
		up = true;
	}
	else if (up && !Keyb::isKeyPressed(Keyb::Space)) {
		buffer.emplace_back(Input::upu, false);
		up = false;
	}
	//down
	if (!down && Keyb::isKeyPressed(Keyb::S)) {
		buffer.emplace_back(Input::downd, false);
		down = true;
	}
	else if (down && !Keyb::isKeyPressed(Keyb::S)) {
		buffer.emplace_back(Input::downu, false);
		down = false;
	}
	//left
	if (!left && Keyb::isKeyPressed(Keyb::A)) {
		buffer.emplace_back(Input::leftd, false);
		left = true;
	}
	else if (left && !Keyb::isKeyPressed(Keyb::A)) {
		buffer.emplace_back(Input::leftu, false);
		left = false;
	}
	//right
	if (!right && Keyb::isKeyPressed(Keyb::D)) {
		buffer.emplace_back(Input::rightd, false);
		right = true;
	}
	else if (right && !Keyb::isKeyPressed(Keyb::D)) {
		buffer.emplace_back(Input::rightu, false);
		right = false;
	}
}
