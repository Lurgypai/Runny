#include "stdafx.h"
#include "PlatformLC.h"
#include "SquareCollider.h"
#include "SquareShape.h"

PlatformLC::PlatformLC(float width, float height) {
	setCollider<SquareCollider>(0, 0, width, height);
}

PlatformLC::PlatformLC(const PlatformLC & other) {
}

void PlatformLC::update() {
	SquareShape* shape = static_cast<SquareShape*>(collide->getShape());
	shape->setLeft(owner->x());
	shape->setTop(owner->y());
}

std::unique_ptr<msf::LogicComponent> PlatformLC::clone()
{
	return std::make_unique<PlatformLC>(*this);
}
