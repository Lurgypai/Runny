#include "stdafx.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "BuildModeLogic.h"
#include "PlatformGC.h"
#include "PlatformEditableLC.h"
#include "AnimationGC.h"
#include "Resources.h"
#include "CustomConsole.h"
#include "MenuIconLC.h"

BuildModeLogic::BuildModeLogic(msf::Camera* menuCam_, msf::Camera *buildCam_, msf::Scene* scene, sf::RenderWindow *window) :
	menuCam{menuCam_},
	buildCam{buildCam_},
	sceneHandle{ scene },
	windowHandle{ window },
	lastCommand {-1},
	hasConsoleOwnershp{false},
	prevLClick{false}
{}

void BuildModeLogic::update() {

	std::string line{""};
	if (!hasConsoleOwnershp) {
		bool currentC = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
		if (currentC != prevC) {
			if (currentC) {
				if (!CustomConsole::getOpen()) {
					CustomConsole::open({ 10, 255 });
					hasConsoleOwnershp = true;
				}
			}
			prevC = currentC;
		}
	}

	if (hasConsoleOwnershp) {
		bool currentEnter = sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
		if (currentEnter != prevEnter) {
			if (currentEnter) {
				if (CustomConsole::getOpen()) {
					line = CustomConsole::getLine();
					CustomConsole::clearLine();
					CustomConsole::close();
					hasConsoleOwnershp = false;
				}
			}
			prevEnter = currentEnter;
		}
	}

	std::string word{};
	std::vector<std::string> splitLine{};
	for (int i = 0; i != line.size(); i++) {
		if (isspace(line[i]) || line[i] == '\0' && word != "") {
			splitLine.push_back(word);
			word.clear();
		}
		else {
			word += line[i];
		}
		if (i == line.size() - 1 && word != "") {
			splitLine.push_back(word);
			word.clear();
		}
	}
	if (!splitLine.empty()) {
		if (splitLine[0] == "spawn") {
			if (splitLine.size() == 3) {
				try {
					prevx = std::stoi(splitLine[1]);
					prevy = std::stoi(splitLine[2]);
					if (prevx != 0 && prevy != 0)
						spawnPlatform(sf::Vector2i{ buildCam->getView().getCenter() }, prevx, prevy);
				}
				catch (std::invalid_argument e) {
				}
			}
			else if (splitLine.size() == 2) {
				if (splitLine[1] == "dot") {
					spawnDot(sf::Vector2i{ buildCam->getView().getCenter() });
				}
				else if (splitLine[1] == "end") {
					spawnEnd(sf::Vector2i{ buildCam->getView().getCenter() });
				}
				else if (splitLine[1] == "check") {
					spawnCheck(sf::Vector2i{ buildCam->getView().getCenter() });
				}
			}
			else if (splitLine.size() == 1) {
				if (prevx != 0 && prevy != 0)
					spawnPlatform(sf::Vector2i{ buildCam->getView().getCenter() }, prevx, prevy);
			}
			else {
			}
		}
		else if (splitLine[0] == "clear") {
			for (auto& gobj : sceneHandle->getGOGroup("stage")) {
				gobj->destroy();
			}
			for (auto& gobj : sceneHandle->getGOGroup("dots")) {
				gobj->destroy();
			}
			for (auto& gobj : sceneHandle->getGOGroup("end")) {
				gobj->destroy();
			}
			for (auto& gobj : sceneHandle->getGOGroup("check")) {
				gobj->destroy();
			}
		}
		else if (splitLine[0] == "save") {
			if (splitLine.size() == 2) {

				std::string filePath{splitLine[1]};
				if(splitLine[1].size() < 4 || splitLine[1].substr(splitLine[1].size() - 4, 4) != ".lvl") 
					filePath += ".lvl";

				remove(filePath.c_str());
				ResourceManager::save(filePath, *sceneHandle);
			}
		}
		else if (splitLine[0] == "load") {
			if (splitLine.size() == 2) {
				ResourceManager::load(splitLine[1], *sceneHandle, windowHandle, buildCam->getView());
			}
		}
		else if (splitLine[0] == "pack") {
			if (splitLine.size() >= 4) {
				std::string packName{ splitLine[1] };
				if (splitLine[1].size() < 4 || splitLine[1].substr(splitLine[1].size() - 4, 4) != ".pak")
					packName += ".pak";

				remove(packName.c_str());
				std::ofstream file;
				file.open("level/" + packName);
				if (file.is_open()) {
					file << "pack" << std::endl;
					for (auto iter = splitLine.begin() + 2; iter != splitLine.end(); iter++) {
						file << "level:" << "     " << *iter << std::endl;
					}
					file.close();
				}
			}
		}
	}

	bool currentMid = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
	if (currentMid != prevMid) {
		prevMid = currentMid;
		if (currentMid) {
			switch (lastCommand)
			{
			case(0):
				if(prevx != 0 && prevy != 0)
					spawnPlatform(sf::Vector2i{windowHandle->mapPixelToCoords(sf::Mouse::getPosition(*windowHandle), buildCam->getView())}, prevx, prevy);
				break;
			case(1):
				spawnDot(sf::Vector2i{ windowHandle->mapPixelToCoords(sf::Mouse::getPosition(*windowHandle), buildCam->getView()) });
				break;
			case(2):
				spawnEnd(sf::Vector2i{ windowHandle->mapPixelToCoords(sf::Mouse::getPosition(*windowHandle), buildCam->getView()) });
			case(3):
				spawnCheck(sf::Vector2i{ windowHandle->mapPixelToCoords(sf::Mouse::getPosition(*windowHandle), buildCam->getView()) });
			default:
				break;
			}
		}
	}

	bool currentLClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	if (currentLClick != prevLClick) {
		if (currentLClick) {
			for (auto& gobj : sceneHandle->getGOGroup("menu")) {
				if (gobj->getLogic() != nullptr) {
					MenuIconLC* lc = static_cast<MenuIconLC*>(gobj->getLogic());
					if (lc->getCollider()->contains(windowHandle->mapPixelToCoords(sf::Mouse::getPosition(*windowHandle), menuCam->getView()))) {
						switch (lc->getResource())
						{
						case platform:
							lastCommand = 0;
							break;
						case dot:
							lastCommand = 1;
							break;
						case end:
							lastCommand = 2;
							break;
						case check:
							lastCommand = 3;
							break;
						}
					}
				}
			}
		}
	}

	bool currentConsoleOpen = CustomConsole::getOpen();
	if (prevConsoleOpen != currentConsoleOpen) {
		if (prevConsoleOpen) {
			if (lastCommand == 0) {
				for (auto& gobj : sceneHandle->getGOGroup("menu")) {
					if (gobj->getLogic() != nullptr) {
						MenuIconLC* lc = static_cast<MenuIconLC*>(gobj->getLogic());
						if (lc->getResource() == platform) {
							prevx = lc->getPrevRes().x;
							prevy = lc->getPrevRes().y;
						}
					}
				}
			}
		}
		prevConsoleOpen = currentConsoleOpen;
	}
}

std::unique_ptr<msf::DLogicComponent> BuildModeLogic::clone() {
	return std::make_unique<BuildModeLogic>(*this);
}

void BuildModeLogic::spawnPlatform(const sf::Vector2i& pos, int width, int height) {
	lastCommand = 0;
	BlankResource* plat = static_cast<BlankResource*>(ResourceManager::getResource(Resources::platform));
	plat->setRes(width, height);
	plat->initEditable(*sceneHandle, windowHandle, buildCam->getView(), pos);
}

void BuildModeLogic::spawnDot(const sf::Vector2i& pos) {
	lastCommand = 1;
	ResourceManager::getResource(Resources::dot)->initEditable(*sceneHandle, windowHandle, buildCam->getView(), pos);
}

void BuildModeLogic::spawnEnd(const sf::Vector2i & pos) {
	lastCommand = 2;
	ResourceManager::getResource(Resources::end)->initEditable(*sceneHandle, windowHandle, buildCam->getView(), pos);
}

void BuildModeLogic::spawnCheck(const sf::Vector2i & pos) {
	lastCommand = 3;
	ResourceManager::getResource(Resources::check)->initEditable(*sceneHandle, windowHandle, buildCam->getView(), pos);
}
