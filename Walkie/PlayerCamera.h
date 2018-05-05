#pragma once
#include "MSF.h"

class PlayerCamera : public msf::Camera {
public:
	PlayerCamera(msf::GameObject* playerHandle_);
	PlayerCamera(const PlayerCamera& other);
	void update() override;
private:
	msf::GameObject* playerHandle;
	float prevDistance{0};
	float offset{0};
	float width{40};
	float height{100};
	bool readjust{ false };
};