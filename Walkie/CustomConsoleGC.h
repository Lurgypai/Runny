#pragma once
#include "MSF.h"

class CustomConsoleGC : public msf::GraphicsComponent {
public:
	CustomConsoleGC(const std::string& filePath, unsigned int size);
	CustomConsoleGC(const CustomConsoleGC& other);
	void update(std::vector<msf::Action*>& act, sf::RenderWindow & window) override;
	std::unique_ptr<GraphicsComponent> clone() override;
private:
	sf::RectangleShape underline;
	sf::RectangleShape textLine;
	sf::Text text;
	sf::Font font;
};