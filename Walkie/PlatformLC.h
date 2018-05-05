#pragma once
#include "MSF.h"

class PlatformLC : public msf::LogicComponent {
public:
	PlatformLC(float width, float height);
	PlatformLC(const PlatformLC& other);

	void update() override;
	std::unique_ptr<msf::LogicComponent> clone() override;
};