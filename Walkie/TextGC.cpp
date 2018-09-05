#include "stdafx.h"
#include "TextGC.h"

TextGC::TextGC(const std::string & filePath, const std::string & line, unsigned int size) {
	font.loadFromFile(filePath);
	text = sf::Text{line, font, 100 * size};
	text.setFillColor(sf::Color::White);
	text.scale(.01, .01);
}

TextGC::TextGC(const TextGC & other) : 
	font(other.font),
	text(other.text)
{
	text.setFont(other.font);
}

void TextGC::update(std::vector<msf::Action*>& act, sf::RenderWindow & window) {
	text.setPosition(owner->getPos());
	window.draw(text);
}

std::unique_ptr<msf::GraphicsComponent> TextGC::clone() {
	return std::make_unique<TextGC>(*this);
}
