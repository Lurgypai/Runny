#pragma once
#include "MSF.h"
#include "WalkieC.h"
class WalkieLC : public msf::LogicComponent {
public:
	WalkieLC(msf::Game *window_);
	WalkieLC(const WalkieLC& other);
	~WalkieLC();

	void update() override;
	std::unique_ptr<msf::LogicComponent> clone() override;
	void spawn();
	void setVictor();
	bool complete();
	bool isSpawning();
	bool isVictor();
	sf::Vector2f getSpawnPoint();
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
		rolling,
		spawning,
		victory
	};
	//controller
	WalkieC controller;
	unsigned int controllerState;
	//movement
	msf::MVector velocity;
	PlayerState state;
	msf::Game *game;

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
	int life;

	int spawnFrame;
	bool levelComplete;
	int fallDeathDelay;
	int fallDeathDelayMax;

	sf::Vector2f respawnPos;
};

