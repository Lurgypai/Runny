
#include "stdafx.h"

#include <sstream>

#include "Resources.h"
#include "AnimationGC.h"
#include "CollideableLC.h"
#include "PlatformEditableLC.h"
#include "MenuIconLC.h"
#include "PlatformGC.h"
#include "StringUtil.h"

Resource::Resource(const std::string & groupId_, const std::string fileHandle_) :
	groupId{ groupId_ },
	fileHandle{fileHandle_}
{}

AnimatedResource::AnimatedResource(const std::string& groupId_, const std::string &fileHandle_, const std::string & filePath_,
	int w_, int h_, int frameW_, int frameH_, int columns_, int frameDelay_, int frames_, int xOffset_, int yOffset_) :
	Resource{groupId_, fileHandle_},
	filePath{ filePath_ },
	w{w_},
	h{h_},
	frameW{frameW_},
	frameH{frameH_},
	columns{columns_},
	frameDelay{frameDelay_},
	frames{frames_},
	xOffset{xOffset_},
	yOffset{yOffset_}
{}

std::shared_ptr<msf::GameObject>  AnimatedResource::init(msf::Scene& scene, const sf::Vector2i& pos) {
	if (w != 0 && h != 0) {
		std::shared_ptr<msf::GameObject> gobj = scene.addGObject(groupId);
		gobj->setGraphics<AnimationGC>(filePath, frameW, frameH, columns, frameDelay, frames, xOffset, yOffset);
		gobj->setLogic<CollideableLC>(w, h);
		gobj->setPos(sf::Vector2f{ pos });
		return gobj;
	}
	else {
		return std::shared_ptr<msf::GameObject>{nullptr};
	}
}

std::shared_ptr<msf::GameObject> AnimatedResource::initEditable(msf::Scene & scene, sf::RenderWindow * windowHandle,const sf::View &view, const sf::Vector2i & pos) {

	if (w != 0 && h != 0) {
		std::shared_ptr<msf::GameObject> gobj = scene.addGObject(groupId);
		gobj->setGraphics<AnimationGC>(filePath, frameW, frameH, columns, frameDelay, 1, xOffset, yOffset);
		gobj->setLogic<PlatformEditable>(windowHandle, &view, w, h);
		gobj->setPos(sf::Vector2f{ pos });
		return gobj;
	}
	else {
		return std::shared_ptr<msf::GameObject>{nullptr};
	}

}

std::shared_ptr<msf::GameObject> AnimatedResource::initMenuIcon(msf::Scene & scene, sf::RenderWindow * windowHandle, const sf::View &menuView, const sf::View & editorView, const sf::Vector2i & pos, Resources r) {
	if (w != 0 && h != 0) {
		std::shared_ptr<msf::GameObject> gobj = scene.addGObject("menu");
		gobj->setGraphics<AnimationGC>(filePath, frameW, frameH, columns, frameDelay, 1, xOffset, yOffset);
		gobj->setLogic<MenuIconLC>(windowHandle, &menuView, &editorView, w, h, r);
		gobj->setPos(sf::Vector2f{ static_cast<float>(pos.x - (w / 2)), static_cast<float>(pos.y - (h / 2)) });
		return gobj;
	}
	else {
		return std::shared_ptr<msf::GameObject>{nullptr};
	}
}

std::string AnimatedResource::serialize(msf::Scene& fromScene) {
	std::stringstream s;
	for (auto gobj : fromScene.getGOGroup(groupId)) {
		s << fileHandle << ":      x: " << gobj->x() << "     y: " <<gobj->y() << std::endl;
	}
	return s.str();
}

void AnimatedResource::unserialize(std::string line, msf::Scene & toScene, sf::RenderWindow *window,const sf::View &view) {

	std::vector<std::string> splitL = splitLine(line);

	try {
		int x = std::stoi(splitL[2]);
		int y = std::stoi(splitL[4]);
		if (window == nullptr) {
			init(toScene, { x, y });
		}
		else {
			initEditable(toScene, window, view, { x, y });
		}
	}
	catch (std::invalid_argument e) {
		std::cout << "Error, unable to parse integers." << std::endl;
	}
}

void BlankResource::setRes(int w_, int h_) {
	w = w_;
	h = h_;
}

BlankResource::BlankResource(const std::string &groupId_, const std::string &fileHandle_) :
	Resource{groupId_, fileHandle_},
	w{0},
	h{0}
{
}

std::shared_ptr<msf::GameObject> BlankResource::init(msf::Scene & scene, const sf::Vector2i & pos) {
	if (w != 0 && h != 0) {
		std::shared_ptr<msf::GameObject> gobj = scene.addGObject(groupId);
		gobj->setGraphics<PlatformGC>(w, h);
		gobj->setLogic<CollideableLC>(w, h);
		gobj->setPos(sf::Vector2f{ pos });
		return gobj;
	}
	else {
		return std::shared_ptr<msf::GameObject>{nullptr};
	}
}

std::shared_ptr<msf::GameObject> BlankResource::initEditable(msf::Scene & scene, sf::RenderWindow * windowHandle,const sf::View &view, const sf::Vector2i & pos) {
	if (w != 0 && h != 0) {
		std::shared_ptr<msf::GameObject> gobj = scene.addGObject(groupId);
		gobj->setGraphics<PlatformGC>(w, h);
		gobj->setLogic<PlatformEditable>(windowHandle, &view, w, h);
		gobj->setPos(sf::Vector2f{ pos });
		return gobj;
	}
	else {
		return std::shared_ptr<msf::GameObject>{nullptr};
	}
}

std::shared_ptr<msf::GameObject> BlankResource::initMenuIcon(msf::Scene & scene, sf::RenderWindow * windowHandle, const sf::View &menuView, const sf::View & editorView, const sf::Vector2i & pos, Resources r) {
	std::shared_ptr<msf::GameObject> gobj = scene.addGObject("menu");
	gobj->setGraphics<PlatformGC>(5, 5);
	gobj->setLogic<MenuIconLC>(windowHandle, &menuView, &editorView, 5, 5, r);
	gobj->setPos(sf::Vector2f{ static_cast<float>(pos.x - (5 / 2)), static_cast<float>(pos.y - (5 / 2)) });
	return gobj;
}

std::string BlankResource::serialize(msf::Scene & fromScene) {
	std::stringstream s;
	for (auto gobj : fromScene.getGOGroup(groupId)) {
		s << "plat:" << "     " << "x: " << gobj->getPos().x << "     " << "y: " << gobj->getPos().y << "     " <<
			"w: " << gobj->getLogic()->getCollider()->getShape()->getBoundingBox().width << "     " <<
			"h: " << gobj->getLogic()->getCollider()->getShape()->getBoundingBox().height << std::endl;
	}
	return s.str();
}

void BlankResource::unserialize(std::string line, msf::Scene & toScene, sf::RenderWindow * window,const sf::View &view) {

	std::vector<std::string> splitL{ splitLine(line) };

	try {
		int x = std::stoi(splitL[2]);
		int y = std::stoi(splitL[4]);
		int w = std::stoi(splitL[6]);
		int h = std::stoi(splitL[8]);
		if (window == nullptr) {
			setRes(w, h);
			init(toScene, { x, y });
		}
		else {
			setRes(w, h);
			initEditable(toScene, window, view, { x, y });
		}
	}
	catch (std::invalid_argument e) {
		std::cout << "Error, unable to parse integers." << std::endl;
	}
}


std::unordered_map<Resources, std::unique_ptr<Resource>> ResourceManager::resourceList;
void ResourceManager::init() {
	std::unique_ptr<AnimatedResource> dot = std::make_unique<AnimatedResource>("dots", "dot", "images/coin.png", 4, 4, 6, 6, 8, 3, 18, 1, 1);
	std::unique_ptr<AnimatedResource> end = std::make_unique<AnimatedResource>("end", "end", "images/end.png", 7, 8, 7, 8, 8, 3, 18);
	std::unique_ptr<AnimatedResource> check = std::make_unique<AnimatedResource>("check", "check", "images/check.png", 5, 6, 5, 6, 8, 3, 18);
	std::unique_ptr<BlankResource> plat = std::make_unique<BlankResource>("stage", "plat");

	ResourceManager::resourceList.insert({Resources::dot, std::move(dot)});
	ResourceManager::resourceList.insert({ Resources::end, std::move(end) });
	ResourceManager::resourceList.insert({ Resources::check, std::move(check) });
	ResourceManager::resourceList.insert({ Resources::platform, std::move(plat) });
}

Resource * ResourceManager::getResource(Resources r) {
	return ResourceManager::resourceList[r].get();
}

void ResourceManager::load(const std::string & fileHandle, msf::Scene & scene, sf::RenderWindow * window,const sf::View &view) {
	std::ifstream file;
	file.open("level/" + fileHandle);
	if (file.is_open() && file.good()) {
		std::stringstream data;
		data << file.rdbuf();
		file.close();


		for (auto& pair : resourceList) {
			for (auto gobj : scene.getGOGroup(pair.second->groupId)) {
				gobj->destroy();
			}
		}

		char c[256];
		while (data.getline(c, 256)) {
			std::string id;
			for (char i : c) {
				if (i >= -1 && i <= 255) {
					if (!isspace(i) && i != '\0') {
						id += i;
					}
					else {
						break;
					}
				}
			}
			for (auto& pair : resourceList) {
				if (pair.second->fileHandle + ':' == id) {
					pair.second->unserialize(c, scene, window, view);
				}
			}
		}
	}
	else {
		std::cout << "Error, unable to open file." << std::endl;
	}
}

void ResourceManager::save(const std::string & fileHandle, msf::Scene & fromScene) {
	std::ofstream file;
	file.open("level/" + fileHandle);
	file << "level" << std::endl;
	if (file.is_open() && file.good()) {
		for (auto& pair : resourceList) {
			file << pair.second->serialize(fromScene);
		}
	}
	else {
		std::cout << "Error, unable to open file." << std::endl;
	}
	file.close();
}

void ResourceManager::loadMenu(msf::Scene & scene, sf::RenderWindow * window, const sf::View & menuView, const sf::View & editorView) {
	int columnWidth{ 10 };
	int i{ 0 };
	for (auto& pair : resourceList) {
		pair.second->initMenuIcon(scene, window, menuView, editorView, { 10 + i * (columnWidth), 10 }, pair.first);
		i++;
	}
}
