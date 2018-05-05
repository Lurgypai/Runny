#pragma once
#include "MSF.h"
#include "WalkieC.h"
class WalkieLC : public msf::LogicComponent {
public:
	WalkieLC();
	WalkieLC(const WalkieLC& other);
	~WalkieLC();

	void update() override;
	std::unique_ptr<msf::LogicComponent> clone() override;
private:
	enum ControllerState {
		left = 1 << 0,
		right = 1 << 1,
		up = 1 << 2,
		down = 1 << 3

	};
	enum PlayerState {
		free,
		dropping,
		rolling
	};
	//controller
	WalkieC controller;
	unsigned int controllerState;
	//movement
	msf::MVector velocity;
	PlayerState state;
	bool onLWall;
	bool onGround;
	bool onRWall;
	int rollingCounter;
	int dropCounter;
	int jumps;
	int dropDelay;
	const int rollingCounterMax;
	const int dropCounterMax;
	const int maxJumps;
	const int dropDelayMax;
	float speed;
};

