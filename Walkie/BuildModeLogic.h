#pragma once
#include "MSF.h"

class BuildModeLogic : public msf::DLogicComponent {
public:
	BuildModeLogic(msf::Camera* menuCam_, msf::Camera * buildCam_, msf::Scene* sceneHandle, sf::RenderWindow *window);
	
	void update() override;
	std::unique_ptr<DLogicComponent> clone() override;
private:
	void spawnPlatform(const sf::Vector2i& pos, int width, int height);
	void spawnDot(const sf::Vector2i& pos);
	void spawnEnd(const sf::Vector2i& pos);
	void spawnCheck(const sf::Vector2i& pos);

	msf::Scene* sceneHandle;
	sf::RenderWindow* windowHandle;
	msf::Camera *menuCam;
	msf::Camera *buildCam;


	int prevx;
	int prevy;
	int lastCommand;

	bool prevConsoleOpen;
	bool prevMid;
	bool prevLClick;
	bool prevC;
	bool prevEnter;
	bool hasConsoleOwnershp;
};