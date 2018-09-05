#include "stdafx.h"
#include "CollideableLC.h"
#include "SquareCollider.h"
#include "SquareShape.h"

CollideableLC::CollideableLC(float width, float height) {
	setCollider<SquareCollider>(-100, -100, width, height);
}

CollideableLC::CollideableLC(const CollideableLC & other) {
}

void CollideableLC::update() {
	SquareShape* shape = static_cast<SquareShape*>(collide->getShape());
	shape->setLeft(owner->x());
	shape->setTop(owner->y());
}

std::unique_ptr<msf::LogicComponent> CollideableLC::clone()
{
	return std::make_unique<CollideableLC>(*this);
}
