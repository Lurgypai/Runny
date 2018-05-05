#include "stdafx.h"
#include "SquareCollider.h"
#include "SquareShape.h"

SquareCollider::SquareCollider(float left, float top, float width, float height) {
	this->setShape<SquareShape>(left, top, width, height);
}

SquareCollider::~SquareCollider() {}

bool SquareCollider::intersects(Collider & other) {
	if (other.getShape()->getType() == 0) {
		SquareShape* oShape = static_cast<SquareShape*>(other.getShape());
		return shape->getBoundingBox().left < oShape->getBoundingBox().left + oShape->getBoundingBox().width &&
			shape->getBoundingBox().left + shape->getBoundingBox().width > oShape->getBoundingBox().left &&
			shape->getBoundingBox().top < oShape->getBoundingBox().top + oShape->getBoundingBox().height &&
			shape->getBoundingBox().top + shape->getBoundingBox().height > oShape->getBoundingBox().top;
	}
	return false;
}

bool SquareCollider::contains(const sf::Vector2f & point)
{
	return false;
}
