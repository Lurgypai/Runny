#pragma once
#include "MSF.h"

class TextGC : public msf::GraphicsComponent {
public:
	TextGC(const std::string& filePath, const std::string& text, unsigned int size);
	TextGC(const TextGC& other);
	virtual void update(std::vector<msf::Action*>& act, sf::RenderWindow & window) override;
	virtual std::unique_ptr<GraphicsComponent> clone() override;
private:
	sf::Text text;
	sf::Font font;
};