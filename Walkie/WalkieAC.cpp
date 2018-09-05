#include "stdafx.h"
#include "WalkieAC.h"

WalkieAC::WalkieAC(SoundSystem * sounds_) : sounds{sounds_}
{
}

WalkieAC::WalkieAC(WalkieAC & other) :
	sounds{other.sounds}
{
}

void WalkieAC::update(std::vector<msf::Action*>& act) {
	for (auto& action : act) {
		switch (action->id) {
		case jump:
			sounds->playSound("player.jump");
			break;
		case drop:
			sounds->playSound("player.drop");
			break;
		case dot:
			sounds->playSound("player.dot");
			break;
		case check:
			sounds->playSound("player.check");
			break;
		case end:
			sounds->playSound("player.end");
			break;
		}
	}

}

std::unique_ptr<msf::AudioComponent> WalkieAC::clone()
{
	return std::make_unique<WalkieAC>(*this);
}
