#include "stdafx.h"
#include "Animation.h"

Animation::Animation(const std::string & filePath, int width, int height, int columns, int frameDelay_, int currentFrame, std::unordered_map<std::string, std::pair<int, int>> animations_) :
	frameW { width },
	frameH{ height },
	frameDelay{ frameDelay_ },
	currentFrame{ currentFrame },
	animations{ animations_ },
	columns{ columns } {

	texture.loadFromFile(filePath);
	sprite.setTexture(texture);
}

Animation::Animation(const Animation & other) :
	frameW{ other.frameW },
	frameH{ other.frameH },
	frameDelay{ other.frameDelay },
	currentFrame{ other.currentFrame },
	animations{ other.animations },
	columns{ other.columns },
	texture {other.texture} {

	sprite.setTexture(texture);
}

void Animation::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(sprite, states);
}

void Animation::setSection(const std::string & id, bool pause) {
	currentAnimation = animations[id];
	currentAnimationId = id;
	currentFrame = currentAnimation.first;
	paused = pause;
	wait = frameDelay;
}

void Animation::addSection(const std::string & id, int start, int end) {
	animations[id] = { start, end };
}

void Animation::setFrame(int frame, bool pause) {
	currentAnimation = { 0,0 };
	currentAnimationId = "";
	currentFrame = frame;
	sprite.setTextureRect({ (frameW * (currentFrame % columns)), frameH * (currentFrame / columns), frameW, frameH });
	paused = pause;
	wait = frameDelay;
}

void Animation::progress() {
	if (!paused) {
		if (wait != frameDelay) {
			wait++;
		}
		else {
			sprite.setTextureRect({ (frameW * (currentFrame % columns)), frameH * (currentFrame / columns), frameW, frameH });
			wait = 0;
			if (currentAnimation.first <= currentFrame && currentFrame < currentAnimation.second) {
				currentFrame++;
			}
			else {
				currentFrame = currentAnimation.first;
			}
		}
	}
	else {
		wait = 0;
	}
}

sf::Sprite & Animation::getSprite() {
	return sprite;
}
