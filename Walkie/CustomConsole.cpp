#include "stdafx.h"
#include "CustomConsole.h"

std::string CustomConsole::line{};
sf::Vector2f CustomConsole::pos{};
bool CustomConsole::isOpen{ false };

void CustomConsole::open(const sf::Vector2f & pos_) {
	pos = pos_;
	isOpen = true;
}

void CustomConsole::close() {
	isOpen = false;
}

bool CustomConsole::getOpen() {
	return isOpen;
}

void CustomConsole::readEvent(const sf::Event & e) {
	if (isOpen) {
		if (e.type == sf::Event::TextEntered) {
			if (e.text.unicode < 128) {
				char c = static_cast<char>(e.text.unicode);
				if (c != '\b' && line.size() < 80 && c != '\r') {
					line += c;
				}
				else if(c == '\b' && line.size() != 0) {
					line.pop_back();
				}
			}
		}
	}
}

std::string CustomConsole::getLine() {
	return line;
}

sf::Vector2f CustomConsole::getPos() {
	return pos;
}

void CustomConsole::clearLine() {
	line.clear();
}
