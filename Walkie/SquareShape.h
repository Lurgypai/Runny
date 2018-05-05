#pragma once
#include "MSF.h"

class SquareShape : public msf::Shape {
public:
	SquareShape(float left_, float top_, float width, float height);
	~SquareShape();

	void setTop(float top);
	void setLeft(float left);
	void setWidth(float width);
	void setHeight(float height);

	sf::FloatRect getBoundingBox() const override;
	std::unique_ptr<msf::Shape> clone() override;

private:
	float width;
	float height;
};

