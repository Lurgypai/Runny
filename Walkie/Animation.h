#pragma once
#include "SFML\Graphics.hpp"
#include <unordered_map>

class Animation : public sf::Drawable {
public:
	Animation(const std::string& filePath, int width, int height, int columns, int frameDelay = 2, int currentFrame = 0, std::unordered_map<std::string, std::pair<int, int>> animations_ = std::unordered_map<std::string, std::pair<int, int>>{});
	Animation(const Animation& other);

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void setSection(const std::string& id, bool pause);
	void addSection(const std::string& id, int start, int end);
	void setFrame(int frame, bool pause);
	void progress();

	sf::Sprite& getSprite();
private:
	sf::Texture texture;
	sf::Sprite sprite;
	bool paused;
	int frameDelay;
	int wait;
	int currentFrame;
	int frameW;
	int frameH;
	int columns;
	std::pair<int, int> currentAnimation;
	std::string currentAnimationId;
	std::unordered_map<std::string, std::pair<int, int>> animations;
};