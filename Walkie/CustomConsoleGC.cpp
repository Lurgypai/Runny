#include "stdafx.h"
#include "CustomConsoleGC.h"
#include "CustomConsole.h"
#include <ctype.h>

CustomConsoleGC::CustomConsoleGC(const std::string & filePath, unsigned int size) :
	underline{ {400, 1} },
	textLine{ {1, 5} }
{
	font.loadFromFile(filePath);
	text = sf::Text{ "", font, 100 * size };
	text.setFillColor(sf::Color::White);
	text.scale(.01, .01);

	underline.setFillColor(sf::Color::White);
	textLine.setFillColor(sf::Color::White);
}

CustomConsoleGC::CustomConsoleGC(const CustomConsoleGC & other) :
	font(other.font),
	text(other.text)
{
	text.setFont(other.font);
}

void CustomConsoleGC::update(std::vector<msf::Action*>& act, sf::RenderWindow & window) {

	text.setPosition(CustomConsole::getPos());
	if (CustomConsole::getOpen()) {
		int xOffset{0};
		for (const auto& c : CustomConsole::getLine()) {
			if (c != ' ' && !ispunct(c)) {
				xOffset += 6;
			}
			else if (c == '{' || c == '}' || c == '[' || c == ']' || c == '(' || c == ')'|| c == '"'){
				xOffset += 4;
			}
			else if (c == '\\' || c == '/') {
				xOffset += 6;
			}
			else {
				xOffset += 2;
			}
		}
		underline.setPosition({ CustomConsole::getPos().x, CustomConsole::getPos().y + 6 + 3 });
		textLine.setPosition({ CustomConsole::getPos().x + xOffset, CustomConsole::getPos().y + 3 });

		text.setString(CustomConsole::getLine());
		window.draw(text);
		window.draw(textLine);
		window.draw(underline);
	}
}

std::unique_ptr<msf::GraphicsComponent> CustomConsoleGC::clone() {
	return std::make_unique<CustomConsoleGC>(*this);
}
