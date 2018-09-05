#include "stdafx.h"

#include "MenuIconLC.h"
#include "SquareCollider.h"
#include "Customconsole.h"
#include "StringUtil.h"

MenuIconLC::MenuIconLC(sf::RenderWindow * windowHandle_, const sf::View * menuView_, const sf::View * editorView_, int w, int h, const Resources & r_) :
	windowHandle{ windowHandle_ },
	menuView{ menuView_ },
	editorView{ editorView_ },
	r{ r_ },
	hasConsoleOwnership{false},
	prevEnter{false}
{
	setCollider<SquareCollider>(0, 0, w, h);
}

MenuIconLC::MenuIconLC(const MenuIconLC & other) :
	windowHandle{other.windowHandle},
	menuView{ other.menuView },
	editorView{other.editorView},
	r{other.r},
	hasConsoleOwnership{ other.hasConsoleOwnership },
	prevEnter{other.prevEnter}
{
	setCollider<SquareCollider>(other.collide->getShape()->getBoundingBox().left, other.collide->getShape()->getBoundingBox().top, other.collide->getShape()->getBoundingBox().width, other.collide->getShape()->getBoundingBox().height);
}

void MenuIconLC::update() { 
	collide->setOrigin(owner->getPos());
	bool currentLClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if (currentLClick != prevLClick) {
		prevLClick = currentLClick;
		if (currentLClick) {
			if (collide->contains(windowHandle->mapPixelToCoords(sf::Mouse::getPosition(*windowHandle), *menuView))) {

				if (r != Resources::platform) {
					ResourceManager::getResource(r)->initEditable(*(owner->getScene()), windowHandle, *editorView, sf::Vector2i{ editorView->getCenter() });
				}
				else {
					if (!CustomConsole::getOpen()) {
						CustomConsole::open({ 10, 255 });
						hasConsoleOwnership = true;
					}
				}
			}
		}
	}
	if (hasConsoleOwnership) {
		bool currentEnter = sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
		if (currentEnter != prevEnter) {
			if (currentEnter) {
				if (CustomConsole::getOpen()) {
					std::string line{ CustomConsole::getLine() };
					std::vector<std::string> split = splitLine(line);
					if (split.size() == 2) {
						try {
							prevRes.x = std::stoi(split[0]);
							prevRes.y = std::stoi(split[1]);
							if (prevRes.x != 0 && prevRes.y != 0) {
								BlankResource* plat = static_cast<BlankResource*>(ResourceManager::getResource(Resources::platform));
								plat->setRes(prevRes.x, prevRes.y);
								ResourceManager::getResource(r)->initEditable(*(owner->getScene()), windowHandle, *editorView, sf::Vector2i{ editorView->getCenter() });
							}
						}
						catch (std::invalid_argument e) {
						}
					}
					CustomConsole::clearLine();
					CustomConsole::close();
					hasConsoleOwnership = false;
				}
			}
			prevEnter = currentEnter;
		}
	}
}

std::unique_ptr<msf::LogicComponent> MenuIconLC::clone() {
	return std::make_unique<MenuIconLC>(*this);
}

Resources MenuIconLC::getResource() {
	return r;
}

sf::Vector2i MenuIconLC::getPrevRes() {
	return prevRes;
}
