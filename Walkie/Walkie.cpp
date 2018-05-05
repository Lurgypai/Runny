// Walkie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WalkieLC.h"
#include "WalkieGC.h"
#include "PlatformGC.h"
#include "PlatformLC.h"
#include "MSF.h"
#include "Animation.h"
#include "PlayerCamera.h"
#include "PlatformGenLC.h"
#include "FlatPattern.h"

#include "VerticalPattern.h"
int main()
{
	msf::Scene scene{};
	msf::Game game{ "Walkie" };
	game.addScene(scene, "scene");

	Animation animation{ "images/anim.png", 11, 13, 8, 3, 16 };
	animation.addSection("moveright", 0, 7);
	animation.addSection("moveleft", 8, 15);
	animation.addSection("stop", 16, 35);
	animation.addSection("midair", 36, 36);
	animation.addSection("drop", 37, 37);
	animation.addSection("rollright", 38, 41);
	animation.addSection("rollleft", 42, 45);
	animation.setSection("stop", true);

	std::shared_ptr<msf::GameObject> player = scene.addGObject("player");
	player->setGraphics<WalkieGC>(animation);
	player->setLogic<WalkieLC>();
	player->setPos(0,0);

	std::shared_ptr<msf::GameObject> base = scene.addGObject("stage");
	base->setGraphics<PlatformGC>(480, 10);
	base->setLogic<PlatformLC>(480, 10);
	base->setPos(0, 260);

	PlatformGenLC platGen{player.get()};
	platGen.addPattern<FlatPattern>(&scene);
	platGen.addPattern<VerticalPattern>(&scene);
	platGen.addPattern<FlatPattern>(&scene);
	platGen.addPattern<VerticalPattern>(&scene);
	platGen.generateInitial(sf::FloatRect{ {0,260}, {480,10} });

	std::shared_ptr<msf::DataObject> levelGen = scene.addDObject("stage");
	levelGen->setLogic<PlatformGenLC>(platGen);
	/*
	std::shared_ptr<msf::GameObject> plat1 = scene.addGObject("stage");
	plat1->setGraphics<PlatformGC>(40, 10);
	plat1->setLogic<PlatformLC>(40, 10);
	plat1->setPos(480 / 4, 270 * (3.0 / 4.0));

	std::shared_ptr<msf::GameObject> plat2 = scene.addGObject("stage");
	plat2->setGraphics<PlatformGC>(40, 270 * (3.0/4.0));
	plat2->setLogic<PlatformLC>(40, 270 * (3.0 / 4.0));
	plat2->setPos(480 * (3.0/ 4.0), 270 * (1.0 / 4.0));

	std::shared_ptr<msf::GameObject> plat3 = scene.addGObject("stage");
	plat3->setGraphics<PlatformGC>(40, 270 * (3.0 / 4.0));
	plat3->setLogic<PlatformLC>(40, 270 * (3.0 / 4.0));
	plat3->setPos(480 * (.5), 270 * (1.0 / 4.0));
	*/

	unsigned int width = 1440;
	unsigned int height = 810;

	game.prepareWindow(width, height);
	
	msf::Camera cam{ sf::View{{0, 0, 480, 270}} };
	//look into mutexes for pausing

	game.addCamera<PlayerCamera>(0, player.get());
	game.setCamera(0);
	game.start("scene", {"player", "stage"});
	game.waitFor();
	return 0;
}

