#include "stdafx.h"
#include "WalkieLC.h"
#include "WalkieGC.h"
#include "WalkieC.h"
#include "SquareShape.h"
#include "SquareCollider.h"
#include "HbarAction.h"
#include "WalkieAC.h"

WalkieLC::WalkieLC(msf::Game *game_) :
	controllerState{ 0 },
	state{ spawning },
	rollingCounter{ 0 },
	rollingCounterMax{ 40 },
	dropCounter{ 0 },
	dropCounterMax{ 120 },
	jumps{ 0 },
	maxJumps{ 2 },
	speed{ 1.0 },
	dropDelay{ 0 },
	dropDelayMax{ 80 },
	life{ 1800 },
	game{ game_ },
	fallDeathDelay{0},
	fallDeathDelayMax{240},
	respawnPos{0,0}
{
	setCollider<SquareCollider>(0, 0, 3, 12);
}

WalkieLC::WalkieLC(const WalkieLC & other) :
	controllerState{ 0 },
	state{ spawning },
	rollingCounter{ 0 },
	rollingCounterMax{ 40 },
	dropCounter{ 0 },
	dropCounterMax{ 120 },
	jumps{ 0 },
	maxJumps{ 2 },
	speed{ 1.0 },
	dropDelay{ 0 },
	dropDelayMax{ 80 },
	life{ 1800 },
	game{other.game},
	fallDeathDelay{ 0 },
	fallDeathDelayMax{ 240 },
	respawnPos{0,0}
{
	setCollider<SquareCollider>(0, 0, 3, 12);
}


WalkieLC::~WalkieLC()
{
}

void WalkieLC::update() {
	//steps that should be taken
	//grab current vectors
	//grab current hitbox
	//create new vectors
	//move copied hitbox
	//check for collision
	//respond

	//if midair speed is -.5 < v < .5 round to 0;

	if (collide->getShape()->getType() == 0) {
		collide->setOrigin(owner->getPos());
		SquareShape* shape = static_cast<SquareShape*>(collide->getShape());

		//projections
		std::unique_ptr<SquareCollider> projection = std::make_unique<SquareCollider>(shape->getBoundingBox().left, shape->getBoundingBox().top, shape->getBoundingBox().width, shape->getBoundingBox().height);
		msf::MVector projectVel = velocity;

		//gravity
		if(state != spawning && state != victory)
		projectVel += {90, .05};

		if (dropDelay != 0)
			dropDelay--;
		//update controller state
		bool jumped{ false };
		controller.readInputs();
		for (auto& act : controller.getBuffer()) {
			switch (act.id) {
			case WalkieC::leftd:
				controllerState |= left;
				break;
			case WalkieC::rightd:
				controllerState |= right;
				break;
			case WalkieC::leftu:
				controllerState &= ~left;
				break;
			case WalkieC::rightu:
				controllerState &= ~right;
				break;
			case WalkieC::upd:
				jumped = true;
				break;
			case WalkieC::downd:
				if (!onGround && dropDelay == 0 && state == free ) {
					buffer.push_back(new msf::Action{WalkieAC::drop, false});
					dropDelay = dropDelayMax;
					state = dropping;
					projectVel += {90, 3};
				}
				break;
			}
		}
		controller.clearBuffer();

		//read controller state
		int dir{ 0 };
		if (controllerState & left) dir += -1;
		if (controllerState & right) dir += 1;

		//react to controller state
		switch (state) {
			//free
		case free:
			switch (dir) {
				//right
			case 1:
				if (projectVel.xComp() <= (speed - .2)) {
					projectVel += {0, .3};
				}
				else if (projectVel.xComp() >= (speed + .2)) {
					if (onGround)
						projectVel -= {0, .3};
				}
				else {
					projectVel.xComp(speed);
				}
				break;
				//left
			case -1:
				if (projectVel.xComp() <= (-speed - .2)) {
					if (onGround)
						projectVel += {0, .3};
				}
				else if (projectVel.xComp() >= (-speed + .2)) {
					projectVel -= {0, .3};
				}
				else {
					projectVel.xComp(-speed);
				}
				break;
				//still
			case 0:
				if (onGround) {
					if (projectVel.xComp() <= -.2)
						projectVel += {0, .3};
					else if (projectVel.xComp() >= .2)
						projectVel -= {0, .3};
					else
						projectVel.xComp(0);
				}
				else {
					if (projectVel.xComp() >= -(speed + .2) && projectVel.xComp() < 0) {
						projectVel += {0, .05};
					} 
					else if (projectVel.xComp() <= (speed + .2) && projectVel.xComp() > 0) {
						projectVel -= {0, .05};
					}
					else if (projectVel.xComp() >= .11 && projectVel.xComp() <= .11) {
						projectVel.xComp(0);
					}
				}
				break;
			}

			//jumping
			if (jumped) {
				if (onGround) {
					projectVel.yComp(-2);
					buffer.push_back(new msf::Action{ WalkieAC::jump, false });
				}
				else if (onLWall) {
					projectVel.yComp(0);
					projectVel += {-45, 3};
					buffer.push_back(new msf::Action{ WalkieAC::jump, false });
				}
				else if (onRWall) {
					projectVel.yComp(0);
					projectVel += {-135, 3};
					buffer.push_back(new msf::Action{ WalkieAC::jump, false });
				}
				else if (!onGround && !onLWall && !onRWall && (jumps != 0)) {
					jumps--;
					projectVel.yComp(-2);
					buffer.push_back(new msf::Action{ WalkieAC::jump, false });
				}
			}
			break;
			//dropping
		case dropping:
			if (!onGround) {
				projectVel.xComp(0);
			}
			else {
				if (dir != 0) {
					state = rolling;
					rollingCounter = 0;
					projectVel += {0, (2.5 * dir)};
				}
				else {
					state = free;
				}
			}
			break;
		case rolling:
			if (rollingCounter == rollingCounterMax) {
				rollingCounter = 0;
				state = free;
			}
			else {
				rollingCounter++;
			}

			if (jumped) {
				if (onGround) {
					rollingCounter = 0;
					state = free;
					projectVel.yComp(-2);
					buffer.push_back(new msf::Action{ WalkieAC::jump, false });
				}
				else if (jumps != 0) {
					jumps--;
					rollingCounter = 0;
					state = free;
					projectVel.yComp(-2);
					buffer.push_back(new msf::Action{ WalkieAC::jump, false });
				}
			}
			break;
		case spawning:
			if (spawnFrame != 120) {
				life = 1800;
				spawnFrame++;
			}
			else {
				spawnFrame = 0;
				state = free;
			}
			break;
		case victory:
			life = 1800;
		//rolling
		}

		//if we are on the ground decelerate, if we are in the air, and velocity is higher than max, maintain, if its lower, accelerate
		float previousX = shape->getOrigin().x;
		float previousY = shape->getOrigin().y;

		float xMovement = projectVel.xComp();
		float yMovement = projectVel.yComp();
		
		//move projection
		float newX = previousX + xMovement;
		float newY = previousY + yMovement;

		projection->setOrigin(newX, newY);


		//reset and update state booleans, resolve collisions
		onGround = false;
		onRWall = false;
		onLWall = false;

		//obtain a list of intersecting gobjs
		std::vector<msf::GameObject*> intersectingGobjs{};
		for (auto& gobj : owner->getScene()->getGOGroup("stage")) {
			if (gobj->getTag() != owner->getTag()) {
				if (gobj->getLogic()->getCollider()->intersects(*projection)) {
					intersectingGobjs.push_back(gobj.get());
				}
				//if we didn't move, check if we are next to wall for wall jumping
				if (xMovement == 0) {
					if (gobj->getLogic()->getCollider()->getShape()->getType() == 0) {
						SquareShape* otherShape = static_cast<SquareShape*>(gobj->getLogic()->getCollider()->getShape());
						if (newY <= otherShape->getBoundingBox().top + otherShape->getBoundingBox().height) {
							if (newX + shape->getBoundingBox().width == otherShape->getOrigin().x) {
								onRWall = true;
							}
							else if (newX == otherShape->getOrigin().x + otherShape->getBoundingBox().width) {
								onLWall = true;
							}
						}
					}
				}
			}
		}


		//dots
		for (auto& gobj : owner->getScene()->getGOGroup("dots")) {
			if (gobj->getLogic()->getCollider()->intersects(*projection)) {
				gobj->destroy();
				life += 120;
				buffer.push_back(new msf::Action{ WalkieAC::dot, false });
			}
		}

		//dots
		for (auto& gobj : owner->getScene()->getGOGroup("end")) {
			if (gobj->getLogic()->getCollider()->intersects(*projection)) {
				gobj->destroy();
				respawnPos = { 0,0 };
				spawn();
				levelComplete = true;
				buffer.push_back(new msf::Action{ WalkieAC::end, false });
			}
		}

		//checkpoint
		for (auto& gobj : owner->getScene()->getGOGroup("check")) {
			if (gobj->getLogic()->getCollider()->intersects(*projection)) {
				gobj->destroy();
				sf::FloatRect bound = gobj->getLogic()->getCollider()->getShape()->getBoundingBox();
				//spawn inside check
				sf::Vector2f pos = { (bound.left + (bound.width / 2)) - 1.5f, (bound.top + (bound.height / 2))  - 10.0f};
				respawnPos = pos;
				life += 1800;
				buffer.push_back(new msf::Action{ WalkieAC::check, false });
			}
		}

		//resolve y collisions first, so that we can properly resolve x collisons
		for (auto gobj = intersectingGobjs.begin(); gobj != intersectingGobjs.end(); gobj++) {
			if ((*gobj)->getLogic()->getCollider()->getShape()->getType() == 0) {
				if ((*gobj)->getLogic()->getCollider()->intersects(*projection)) {
					SquareShape* otherShape = static_cast<SquareShape*>((*gobj)->getLogic()->getCollider()->getShape());
					//falling
					if (yMovement > 0) {
						if (previousY + shape->getBoundingBox().height <= otherShape->getOrigin().y) {
							onGround = true;
							jumps = maxJumps;
							projectVel.yComp(0);
							newY = (otherShape->getOrigin().y - shape->getBoundingBox().height);
						}
					}
					//jumping
					if (yMovement < 0) {
						if (previousY >= otherShape->getOrigin().y + otherShape->getBoundingBox().height) {
							projectVel.yComp(0);
							newY = (otherShape->getOrigin().y + otherShape->getBoundingBox().height);
						}
					}
					projection->setOrigin(newX, newY);
				}
			}
		}

		//resolve y collisions first, so that we can properly resolve x collisons
		for (auto gobj = intersectingGobjs.begin(); gobj != intersectingGobjs.end(); gobj++) {
			if ((*gobj)->getLogic()->getCollider()->getShape()->getType() == 0) {
				if ((*gobj)->getLogic()->getCollider()->intersects(*projection)) {
					SquareShape* otherShape = static_cast<SquareShape*>((*gobj)->getLogic()->getCollider()->getShape());

					//moving right
					if (xMovement > 0) {
						//these check if the object was to the left right, above or beneath the wall dummy

						if (previousX + shape->getBoundingBox().width <= otherShape->getOrigin().x) {
							onRWall = true;
							if (state == rolling)
								state = free;
							projectVel.xComp(0);
							newX = (otherShape->getOrigin().x - shape->getBoundingBox().width);
						}
					}
					//moving left
					else if (xMovement < 0) {
						if (previousX >= otherShape->getOrigin().x + otherShape->getBoundingBox().width) {
							onLWall = true;
							if (state == rolling)
								state = free;
							projectVel.xComp(0);
							newX = (otherShape->getOrigin().x + otherShape->getBoundingBox().width);
						}
					}
					projection->setOrigin(newX, newY);
				}
			}
		}

		//use velocities and new position
		velocity = projectVel;
		owner->setPos(newX, newY);

		//fall death
		if (fallDeathDelay != fallDeathDelayMax) {
			//if we are falling, add, else reset
			if (!onGround && previousY < newY) {
				fallDeathDelay++;
			}
			else {
				fallDeathDelay = 0;
			}
		}
		else {
			//respawn, restart counter
			spawn();
			fallDeathDelay = 0;
		}

		//place graphics actions
		switch (state) {
		case free:
			if (onGround) {
				float val = velocity.xComp();
				if (val == 0) {
					buffer.push_back(new msf::Action(WalkieGC::Stopped, false));
				}
				if (val > 0) {
					buffer.push_back(new msf::Action(WalkieGC::MoveRight, false));
				}
				if (val < 0) {
					buffer.push_back(new msf::Action(WalkieGC::MoveLeft, false));
				}
			}
			else {
				buffer.push_back(new msf::Action(WalkieGC::InMidair, false));
			}
			break;
		case rolling:
			if (dir == 1) {
				buffer.push_back(new msf::Action(WalkieGC::RollRight, false));
			} if (dir == -1) {
				buffer.push_back(new msf::Action(WalkieGC::RollLeft, false));
			}
			break;
		case dropping:
			buffer.push_back(new msf::Action(WalkieGC::Dropping, false));
			break;
		}
	}
	if (state != spawning) {
		life--;
		if (life == 0) {
			spawn();
		}
		buffer.push_back(new HbarAction{ static_cast<float>(life), WalkieGC::Hbar, false });
	}
	else {
		buffer.push_back(new msf::Action(WalkieGC::Stopped, false));
		buffer.push_back(new HbarAction{ static_cast<float>(spawnFrame * 15), WalkieGC::Hbar, false });
	}
}

std::unique_ptr<msf::LogicComponent> WalkieLC::clone() {
	return std::make_unique<WalkieLC>(*this);
}

void WalkieLC::spawn() {
	state = spawning;
	levelComplete = false;
	owner->setPos(respawnPos);
	velocity = { 0,0 }; 
}

void WalkieLC::setVictor() {
	state = victory;
	levelComplete = false;
	velocity = { 0,0 };
}

bool WalkieLC::complete() { 
	return levelComplete;
}

bool WalkieLC::isSpawning() {
	return state == spawning;
}

bool WalkieLC::isVictor() {
	return state == victory;
}

sf::Vector2f WalkieLC::getSpawnPoint() {
	return respawnPos;
}
