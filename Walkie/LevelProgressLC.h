#pragma once
#include "MSF.h"

class LevelProgressLC : public msf::DLogicComponent {
public:
	LevelProgressLC(msf::Game *gameHandle, msf::GameObject *playerHandle_);
	LevelProgressLC(const LevelProgressLC& other);
	void start(const std::string& filePath);
	void progress();
	virtual void update() override;
	virtual std::unique_ptr<DLogicComponent> clone() override;
private:
	msf::Game *gameHandle;
	msf::GameObject *playerHandle;
	std::vector<std::string> levels;
	int currentLevel;
	bool wasSpawning;
};