#include "stdafx.h"
#include "ImageRenderGC.h"

ImageRenderGC::ImageRenderGC(std::string path) {
	tex.loadFromFile(path);
	sprite.setTexture(tex);
}

void ImageRenderGC::update(std::vector<msf::Action*>& act, sf::RenderWindow & window) {
	sprite.setPosition(owner->getPos());
	window.draw(sprite);
}

std::unique_ptr<msf::GraphicsComponent> ImageRenderGC::clone()
{
	return std::make_unique<ImageRenderGC>(*this);
}
