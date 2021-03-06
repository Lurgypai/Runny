// Walkie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "WalkieLC.h"
#include "WalkieGC.h"
#include "PlatformGC.h"
#include "CollideableLC.h"
#include "MSF.h"
#include "Animation.h"
#include "PlayerCamera.h"
#include "PlatformGenLC.h"
#include "FlatPattern.h"
#include "BuildModeLogic.h"
#include "VerticalPattern.h"
#include "PlatformEditableLC.h"
#include "BuildCamera.h"
#include "AnimationGC.h"
#include "ImageRenderGC.h"
#include "LevelProgressLC.h"
#include "Resources.h"
#include "Button.h"
#include "TextGC.h"
#include "CustomConsole.h"
#include "CustomConsoleGC.h"
#include <filesystem>
#include "SoundSystem.h"
#include "WalkieAC.h"

enum ver {
	INF,
	PLAY,
	BUILD
};

std::string openSelectMenu(msf::Game &game, SoundSystem& sounds) {

	std::vector<Button> menu;
	std::vector<std::string> ids;
	for (auto& file : std::filesystem::directory_iterator("level")) {
		if (file.path().extension() == ".lvl" || file.path().extension() == ".pak") {
			std::string fileName{ file.path().filename().string() };
			menu.emplace_back("images/lvl_button.png", fileName);
			ids.push_back(fileName);
		}
	}

	int i = 0;
	for (auto& button : menu) {
		float x = (i % 3) * 103 + 50;
		float y = (i / 3) * 10 + 50;
		button.setPos({ x, y });
		i++;
	}
	
	sf::RenderWindow* window = game.getWindow();
	sf::View view{ { 240, 135 },{ 480, 270 } };
	window->setView(view);
	
	window->setActive(true);
	bool cont{ true };
	while (cont) {

		window->clear(sf::Color::Black);
		for (auto& button : menu) {
			button.draw(*window);
		}
		window->display();


		//check click state
		bool toggleLClick{ false };
		static bool prevLClick{ false };
		bool lClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
		if (prevLClick != lClick) {
			prevLClick = lClick;
			if (lClick) {
				toggleLClick = true;
			}
		}

		int response = -1;
		int i = 0;
		for (auto& button : menu) {
			if (toggleLClick && button.contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
				sounds.playSound("menu.click");
				response = i;
				break;
			}
			i++;
		}

		if (response != -1) {
			window->setActive(false);
			return ids[response];
		}
	}
}

void buildScene(msf::Scene& scene, msf::Camera& menuCam, msf::Camera& buildCam, msf::Game &game) {
	scene = msf::Scene{};
	std::shared_ptr<msf::DataObject> consoleReader = scene.addDObject("stage");
	consoleReader->setLogic<BuildModeLogic>(&menuCam, &buildCam, &scene, game.getWindow());

	std::shared_ptr<msf::GameObject> playerplaceholder = scene.addGObject("icon");
	playerplaceholder->setGraphics<ImageRenderGC>("images/playerplaceholder.png");

	std::shared_ptr<msf::GameObject> consoleGfx = scene.addGObject("menu");
	consoleGfx->setGraphics<CustomConsoleGC>("fonts/bit5x5.ttf", 8);

	ResourceManager::loadMenu(scene, game.getWindow(), menuCam.getView(), buildCam.getView());
	scene.addSpecialCamera("menu", 0);
}

void playScene(msf::Scene& scene, msf::Game &game, msf::GameObject *&playerToFill, SoundSystem& sounds) {

	scene = msf::Scene{};
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
	player->setAudio<WalkieAC>(&sounds);
	player->setLogic<WalkieLC>(&game);
	player->setPos(0, 0);
	playerToFill = player.get();

	std::string lvl{openSelectMenu(game, sounds)};

	LevelProgressLC progressLC{ &game, playerToFill };
	progressLC.start(lvl);
	std::shared_ptr<msf::DataObject> levelProgresser = scene.addDObject("stage");
	levelProgresser->setLogic<LevelProgressLC>(progressLC);
}

std::shared_ptr<PlatformGenLC> infiniteScene(msf::Scene& scene, msf::Game &game, msf::GameObject *&playerToFill, SoundSystem& sounds) {

	scene = msf::Scene{};
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
	player->setLogic<WalkieLC>(&game);
	player->setAudio<WalkieAC>(&sounds);
	player->setPos(0, 0);

	playerToFill = player.get();

	std::shared_ptr<msf::GameObject> base = scene.addGObject("stage");
	base->setGraphics<PlatformGC>(480, 10);
	base->setLogic<CollideableLC>(480, 10);
	base->setPos(0, 260);

	std::shared_ptr<PlatformGenLC> platGen{new PlatformGenLC{ player.get()} };
	platGen->addPattern<FlatPattern>(&scene);
	platGen->addPattern<VerticalPattern>(&scene);
	platGen->addPattern<FlatPattern>(&scene);
	platGen->addPattern<VerticalPattern>(&scene);
	platGen->generateInitial(sf::FloatRect{ { 0,260 },{ 480,10 } });

	std::shared_ptr<msf::DataObject> levelGen = scene.addDObject("stage");
	levelGen->setLogic<PlatformGenLC>(*platGen);
	return platGen;
}

sf::Sprite menuBG;
sf::Texture menuBGTex;

int main() {
	msf::Scene infiniteS{};
	msf::Scene playS{};
	msf::Scene buildS{};

	std::vector<Button> buttons{};
	buttons.emplace_back("images/button0.png");
	buttons.emplace_back("images/button1.png");
	buttons.emplace_back("images/button2.png");
	buttons.emplace_back("images/button3.png");
	buttons.emplace_back("images/button4.png");

	int i = 0;
	for (auto& button : buttons) {
		float x = (i % 2) * 120 + 130;
		float y = (i / 2) * 45 + 135;
		button.setPos({ x, y });
		i++;
	}

	menuBGTex.loadFromFile("images/logo.png");
	menuBG.setTexture(menuBGTex);

	msf::Game game{ "runny" };
	game.getSettings().load("settings");
	msf::GameObject *player{ nullptr };
	SoundSystem soundSystem{static_cast<float>(game.getSettings().getField("volume"))};

	soundSystem.registerSound("menu.click", "sounds/Select.wav");
	soundSystem.registerSound("player.jump", "sounds/Jump.wav");
	soundSystem.registerSound("player.drop", "sounds/Drop.wav");
	soundSystem.registerSound("player.dot", "sounds/Dot.wav");
	soundSystem.registerSound("player.check", "sounds/Check.wav");
	soundSystem.registerSound("player.end", "sounds/End.wav");

	msf::Scene logo{};
	std::shared_ptr<msf::GameObject> bg = logo.addGObject();
	bg->setGraphics<ImageRenderGC>("images/logo.png");
	bg->setPos(0, 0);
	std::shared_ptr<msf::GameObject> text = logo.addGObject();
	text->setGraphics<ImageRenderGC>("images/text.png");
	text->setPos(0, 0);

	game.addScene(logo, "logo");
	game.addScene(playS, "play");
	game.addScene(buildS, "build");
	game.addScene(infiniteS, "infinite");

	ResourceManager::init();

	unsigned int width, height;
	sf::Uint32 style{ 0 };

	if (game.getSettings().getField("fullscreen")) {
		width = sf::VideoMode::getDesktopMode().width;
		height = sf::VideoMode::getDesktopMode().height;
		style = sf::Style::None;
	}
	else {
		width = game.getSettings().getField("width");
		height = game.getSettings().getField("height");
		style = sf::Style::Titlebar;
	}

	game.prepareWindow(width, height, style);
	msf::Camera cam{ sf::View{ { 240, 135 },{ 480, 270 } } };
	game.addCamera<msf::Camera>(0, cam);
	game.addCamera<PlayerCamera>(1);
	game.addCamera<BuildCamera>(2);
	game.setCamera(0);


	bool cont{ true };
	game.start("logo", "default");

	bool inMenu = false;

	//pointer to save the platform generator;
	std::shared_ptr<PlatformGenLC> platformGenerationPointer{ nullptr };
	while (cont) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			inMenu = true;
		}
		if (inMenu) {
			//pause the game so we can render to the window
			game.pause();
			sf::RenderWindow* window = game.getWindow();
			sf::View view{ { 240, 135 },{ 480, 270 } };
			window->setView(view);

			//check click state
			bool toggleLClick{ false };
			static bool prevLClick{ false };
			bool lClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
			if (prevLClick != lClick) {
				prevLClick = lClick;
				if (lClick) {
					toggleLClick = true;
				}
			}

			//draw
			window->clear(sf::Color::Black);
			window->draw(menuBG);
			int response = -1;
			int i = 0;
			for (auto& button : buttons) {
				button.draw(*window);
				//store response
				if (toggleLClick && button.contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
					response = i;
					soundSystem.playSound("menu.click");
					break;
				}
				i++;
			}
			window->display();

			//save the generator so that its patterns don't pass out of scope.
			switch (response) {
			case 0:
				//reset scene
				window->setActive(false);
				platformGenerationPointer = infiniteScene(infiniteS, game, player, soundSystem);
				game.setScene("infinite");
				game.getUpdater()->setCurrentGroups({ "player", "stage", "dots", "end", "check" });
				std::static_pointer_cast<PlayerCamera>(game.getCamera(1))->setPlayerHandle(player);
				game.setCamera(1);
				inMenu = false;
				game.unPause();
				break;
			case 1:
				//reset scene
				window->setActive(false);
				playScene(playS, game, player, soundSystem);
				game.setScene("play");
				game.getUpdater()->setCurrentGroups({ "player", "stage", "dots", "end", "check", "levels", "imgs" });
				std::static_pointer_cast<PlayerCamera>(game.getCamera(1))->setPlayerHandle(player);
				game.setCamera(1);
				inMenu = false;
				game.unPause();
				break;
			case 2:
				//reset scene
				window->setActive(false);
				buildScene(buildS, *game.getCamera(0), *game.getCamera(2), game);
				game.setScene("build");
				game.getUpdater()->setCurrentGroups({ "player", "stage", "dots", "icon", "end", "check", "menu" });
				game.setCamera(2);
				game.getCamera(2)->setView(sf::View{ { -240, -135, 480, 270 } });
				game.unPause();
				inMenu = false;
				break;
			case 3:
				game.unPause();
				window->setActive(false);
				cont = false;
				game.closeLoop();
				game.waitFor();
				inMenu = false;
				break;
			case 4:
				game.unPause();
				window->setActive(false);
				inMenu = false;
				break;
			}
		}
		sf::Event e;
		while (game.pollWindowEvent(e)) {
			CustomConsole::readEvent(e);
		}
	}
	return 0;
}

//pack isn't finding the level

//test spawning
//--instant death spikes
//test progressing
//--add finishing of packs
//test packing