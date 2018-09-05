#include "stdafx.h"
#include "LevelProgressLC.h"
#include <sstream>
#include "PlatformGC.h"
#include "CollideableLC.h"
#include "AnimationGC.h"
#include "WalkieLC.h"
#include "Resources.h"
#include "ImageRenderGC.h"

LevelProgressLC::LevelProgressLC(msf::Game *gameHandle_, msf::GameObject * playerHandle_) :
	gameHandle{ gameHandle_ },
	playerHandle{ playerHandle_ },
	currentLevel{ -1 },
	wasSpawning{ false },
	levels{}
{}

LevelProgressLC::LevelProgressLC(const LevelProgressLC & other) :
	gameHandle{ other.gameHandle },
	playerHandle{ other.playerHandle },
	currentLevel{ other.currentLevel },
	wasSpawning{false},
	levels{ other.levels }
{}

void LevelProgressLC::start(const std::string & filePath_) {

	std::ifstream file{};
	std::string filePath{filePath_ };
	file.open("level/" + filePath);
	if (file.good()) {
		std::stringstream data;
		data << file.rdbuf();
		file.close();

		char c[256];

		data.getline(c, 256);
		std::string line{ c };
		if (line == "level") {
			ResourceManager::load(filePath, *(playerHandle->getScene()), nullptr, sf::View{});
			levels.push_back(filePath);
			currentLevel = 0;
		}
		else if (line == "pack") {
			while (data.getline(c, 256)) {
				line = c;
				std::string word{};
				std::vector<std::string> splitLine{};
				for (int i = 0; i != line.size(); i++) {
					if (isspace(line[i]) || line[i] == '\0') {
						if (word != "") {
							splitLine.push_back(word);
							word.clear();
						}
					}
					else {
						word += line[i];
					}
					if (i == line.size() - 1) {
						splitLine.push_back(word);
						word.clear();
					}
				}
				levels.push_back(splitLine[1]);
			}
			progress();
		}
	}
	else {
		std::cout << "Error, could not find file or directory." << std::endl;
		std::cout << ":: ";
	}
}

void LevelProgressLC::progress() {
	if (currentLevel != levels.size() - 1) {
		currentLevel++;
		ResourceManager::load(levels[currentLevel], *(playerHandle->getScene()), nullptr, sf::View{});
	}
	else {
		WalkieLC *playerLogic = static_cast<WalkieLC*>(playerHandle->getLogic());
		playerLogic->setVictor();
		std::shared_ptr<msf::GameObject> gobj = playerHandle->getScene()->addGObject("imgs");
		gobj->setGraphics<ImageRenderGC>("images/victory.png");
		gobj->setPos({playerHandle->getPos().x - 24, playerHandle->getPos().y - 40});
	}
}

void LevelProgressLC::update() {

	WalkieLC *playerLogic = static_cast<WalkieLC*>(playerHandle->getLogic());

	if (playerLogic->complete()) {
		progress();
		if(!playerLogic->isVictor())
			playerLogic->spawn();
	}

	//check if we toggled to spawning
	bool isSpawning = playerLogic->isSpawning();
	if (isSpawning != wasSpawning) {
		wasSpawning = isSpawning;
		if (isSpawning) {

			//reset dots
			for (auto dot : playerHandle->getScene()->getGOGroup("dots")) {
				dot->destroy();
			}

			std::ifstream file{};
			file.open("level/" + levels[currentLevel]);
			if (file.good()) {
				std::stringstream data;
				data << file.rdbuf();
				file.close();

				char c[256];
				while (data.getline(c, 256)) {
					std::string line{ c };
					std::string word{};
					std::vector<std::string> splitLine{};
					for (int i = 0; i != line.size(); i++) {
						if (isspace(line[i]) || line[i] == '\0') {
							if (word != "") {
								splitLine.push_back(word);
								word.clear();
							}
						}
						else {
							word += line[i];
						}
						if (i == line.size() - 1) {
							splitLine.push_back(word);
							word.clear();
						}
					}
					if (splitLine[0] == "dot:") {
						try {
							int x = std::stoi(splitLine[2]);
							int y = std::stoi(splitLine[4]);
							std::shared_ptr<msf::GameObject> gobj = playerHandle->getScene()->addGObject("dots");
							gobj->setGraphics<AnimationGC>("images/coin.png", 6, 6, 8, 3, 18, 1, 1);
							gobj->setLogic<CollideableLC>(4, 4);
							gobj->setPos(x, y);
						}
						catch (std::invalid_argument e) {
							std::cout << "Error, unable to parse integers." << std::endl;
						}
					}
				}
			}
			else {
				std::cout << "Error, could not find file or directory." << std::endl;
				std::cout << ":: ";
			}
		}
	}
}

std::unique_ptr<msf::DLogicComponent> LevelProgressLC::clone() {
	return std::make_unique<LevelProgressLC>(*this);
}
