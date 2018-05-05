#pragma once
#include "MSF.h"
#include "Animation.h"
class WalkieGC : public msf::GraphicsComponent {
public:
	enum AnimationFlag {
		MoveLeft = 20,
		MoveRight = 21,
		InMidair = 22,
		MidairJump = 23,
		Stopped = 24,
		RollRight = 25,
		RollLeft = 26,
		Dropping = 27
	};

	WalkieGC(const Animation& animation);
	WalkieGC(const WalkieGC& other);
	~WalkieGC();

	void update(std::vector<msf::Action *>& act, sf::RenderWindow & window) override;
	std::unique_ptr<GraphicsComponent> clone() override;
private:
	AnimationFlag state;
	Animation animation;
};

