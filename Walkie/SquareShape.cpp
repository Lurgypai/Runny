#include "stdafx.h"
#include "SquareShape.h"


SquareShape::SquareShape(float left_, float top_, float width, float height) : Shape{ {left_, top_}, 0 }, width{ width }, height{ height } {}

SquareShape::~SquareShape() {}

void SquareShape::setTop(float top) {
	origin.y = top;
}

void SquareShape::setLeft(float left) {
	origin.x = left;
}

void SquareShape::setWidth(float width) {
	width = width;
}

void SquareShape::setHeight(float height) {
	height = height;
}

sf::FloatRect SquareShape::getBoundingBox() const {
	return {origin.x, origin.y, width, height};
}

std::unique_ptr<msf::Shape> SquareShape::clone() {
	return std::make_unique<SquareShape>(*this);
}
