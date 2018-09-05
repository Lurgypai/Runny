#pragma once
#include "MSF.h"


//the view passed should probably be a const ptr so that it can be nullable....


enum Resources {
	platform,
	dot,
	end,
	check
};

class Resource {
protected:
	friend class ResourceManager;

	Resource( const std::string& groupId_, const std::string fileHandle_ );
	std::string groupId;
	std::string fileHandle;
		
public:
	virtual std::shared_ptr<msf::GameObject> init(msf::Scene &scene, const sf::Vector2i& pos) = 0;
	virtual std::shared_ptr<msf::GameObject> initEditable(msf::Scene &scene, sf::RenderWindow* windowHandle, const sf::View &view, const sf::Vector2i& pos) = 0;
	virtual std::shared_ptr<msf::GameObject> initMenuIcon(msf::Scene &scene, sf::RenderWindow* windowHandle, const sf::View &menuView, const sf::View & editorView, const sf::Vector2i& pos, Resources r) =0;
	virtual std::string serialize(msf::Scene& scene) = 0;
	virtual void unserialize(std::string line, msf::Scene& toScene, sf::RenderWindow *window, const sf::View & view) = 0;
};

class AnimatedResource : public Resource{
private:
	int w;
	int h;
	int frameW;
	int frameH;
	std::string filePath;
	int columns;
	int frameDelay;
	int frames;
	int xOffset;
	int yOffset;
public:
	AnimatedResource(const std::string& groupId_, const std::string &fileHandle_, const std::string &filePath, int w_, int h_, int frameW_, int frameH_, int columns_, int frameDelay_, int frames_, int xOffset_ = 0, int yOffset_ = 0);
	std::shared_ptr<msf::GameObject> init(msf::Scene &scene, const sf::Vector2i& pos);
	std::shared_ptr<msf::GameObject> initEditable(msf::Scene &scene, sf::RenderWindow *windowHandle, const sf::View &view, const sf::Vector2i& pos);
	std::shared_ptr<msf::GameObject> initMenuIcon(msf::Scene &scene, sf::RenderWindow* windowHandle, const sf::View &menuView, const sf::View & editorView, const sf::Vector2i& pos, Resources r);
	std::string serialize(msf::Scene& fromScene);
	void unserialize(std::string line, msf::Scene& toScene, sf::RenderWindow *window, const sf::View &view);
};

class BlankResource : public Resource {
private:
	int w;
	int h;
public:
	void setRes(int w_, int h_);
	BlankResource(const std::string &groupId_, const std::string &fileHandle_);
	std::shared_ptr<msf::GameObject> init(msf::Scene &scene, const sf::Vector2i& pos);
	std::shared_ptr<msf::GameObject> initEditable(msf::Scene &scene, sf::RenderWindow *windowHandle, const sf::View &view, const sf::Vector2i& pos);
	std::shared_ptr<msf::GameObject> initMenuIcon(msf::Scene &scene, sf::RenderWindow* windowHandle, const sf::View &menuView, const sf::View & editorView, const sf::Vector2i& pos, Resources r);
	std::string serialize(msf::Scene& fromScene);
	void unserialize(std::string line, msf::Scene& toScene, sf::RenderWindow *window, const sf::View &view);
};

class ResourceManager {
private:
	static std::unordered_map<Resources, std::unique_ptr<Resource>> resourceList;
public:
	static void init();
	static Resource* getResource(Resources r);
	static void load(const std::string &fileHandle, msf::Scene& scene, sf::RenderWindow* window, const sf::View &view);
	static void save(const std::string &fileHandle, msf::Scene& fromeScene);
	static void loadMenu(msf::Scene & scene, sf::RenderWindow * window, const sf::View & menuView, const sf::View & editorView);
};
