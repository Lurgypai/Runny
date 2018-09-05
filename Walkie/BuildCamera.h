#pragma once
#include "MSF.h"

class BuildCamera : public msf::Camera {
public:
	BuildCamera();
	void update() override;
private:
	float moveSpeed;
	float zoomSpeed;
};