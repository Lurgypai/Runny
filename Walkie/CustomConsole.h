#pragma once
#include "MSF.h"

class CustomConsole {
public:
	static void open(const sf::Vector2f & pos);
	static void close();
	static bool getOpen();
	static void readEvent(const sf::Event& e);
	static std::string getLine();
	static sf::Vector2f getPos();
	static void clearLine();
private:
	static std::string line;
	static sf::Vector2f pos;
	static bool isOpen;
};