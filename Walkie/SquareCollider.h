#pragma once
#include "MSF.h"
class SquareCollider : public msf::Collider
{
public:
	SquareCollider(float left, float top, float width, float height);
	~SquareCollider();

	bool intersects(Collider & other) override;
	bool contains(const sf::Vector2f & point) override;
};

//set this to use a square shape, set the logic component to use collider logic isntead
