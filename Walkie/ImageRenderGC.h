#pragma once
#include "MSF.h"

class ImageRenderGC : public msf::GraphicsComponent {
public:
	ImageRenderGC(std::string path);
	virtual void update(std::vector<msf::Action*>& act, sf::RenderWindow & window) override;
	virtual std::unique_ptr<GraphicsComponent> clone() override;
private:
	sf::Texture tex;
	sf::Sprite sprite;
};