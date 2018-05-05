#pragma once
#include "PlatformPattern.h"
#include <random>

class VerticalPattern : public PlatformPattern {
public:
	VerticalPattern(msf::Scene* scene, int xMin = 80, int xMax = 120, int yMin = 50, int yMax = 80, int width = 10, int heightMin = 10, int hieghtMax = 30);
	VerticalPattern(const VerticalPattern& other);
	sf::FloatRect generate(sf::FloatRect lastPlatBox) override;
	void clearGeneration() override;
	sf::FloatRect getLast() const override;
	std::unique_ptr<PlatformPattern> clone() const override;
private:
	std::default_random_engine eng;
	std::uniform_int_distribution<int> xDistr;
	std::uniform_int_distribution<int> yDistr;
	std::uniform_int_distribution<int> heightDistr;
	int width;
	msf::Scene * scene;
	sf::FloatRect last;
	std::vector<msf::GameObject*> platforms;
};