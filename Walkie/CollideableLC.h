#pragma once
#include "MSF.h"

class CollideableLC : public msf::LogicComponent {
public:
	CollideableLC(float width, float height);
	CollideableLC(const CollideableLC& other);

	void update() override;
	virtual std::unique_ptr<msf::LogicComponent> clone() override;
};