#pragma once
#include "MSF.h"
#include "Resources.h"
class MenuIconLC : public msf::LogicComponent{
public:
	MenuIconLC(sf::RenderWindow* windowHandle_, const sf::View * menuView_, const sf::View * editorView_, int w, int h, const Resources &r);
	MenuIconLC(const MenuIconLC & other);
	void update() override;
	std::unique_ptr<LogicComponent> clone() override;
	Resources getResource();
	sf::Vector2i getPrevRes();
private:
	sf::Vector2i prevRes;
	Resources r;
	sf::RenderWindow* windowHandle;
	const sf::View* menuView;
	const sf::View * editorView;
	bool prevLClick;
	bool hasConsoleOwnership;
	bool prevEnter;
};