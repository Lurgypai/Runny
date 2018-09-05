#pragma once
#include "MSF.h"

class Button {
public:
	Button(std::string filePath, const std::string &text_ = "")	{
		tex.loadFromFile(filePath);
		sprite.setTexture(tex);
		font.loadFromFile("fonts/bit5x5.ttf");

		if (text_ != "") {
			text = sf::Text{ text_, font, 100 * 8 };
			text.setFillColor(sf::Color::Black);
			text.scale(.01, .01);
			text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height);
			text.setPosition(sprite.getPosition().x + (sprite.getGlobalBounds().width / 2), sprite.getPosition().y + (sprite.getGlobalBounds().height / 2));
		}
	}

	Button(const Button& other) :
		tex{ other.tex },
		font{ other.font },
		text{ other.text },
		sprite{ other.sprite }
	{
		sprite.setTexture(tex);
		sprite.setPosition(other.sprite.getPosition());


		text.setFont(font);
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height);
		text.setPosition(sprite.getPosition().x + (sprite.getGlobalBounds().width / 2), sprite.getPosition().y + (sprite.getGlobalBounds().height / 2));
	}

	void setPos(sf::Vector2f pos) {
		sprite.setPosition(pos);
		text.setPosition(sprite.getPosition().x + (sprite.getGlobalBounds().width / 2), sprite.getPosition().y + (sprite.getGlobalBounds().height / 2));
	}

	bool contains(sf::Vector2f pos) {
		return sprite.getGlobalBounds().contains(pos);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
		if (text.getString() != "")
			window.draw(text);
	}
private:
	sf::Sprite sprite;
	sf::Texture tex;
	sf::Font font;
	sf::Text text;
};