#pragma once
#include "MSF.h"
#include "PlatformPattern.h"
#include <random>

class FlatPattern : public PlatformPattern {
public:
	FlatPattern(msf::Scene* scene, int xMin = 50, int xMax = 200, int yMin = 0, int yMax = 50, int widthMin = 5, int widthMax = 20, int heightMin = 1, int heightMax = 7, int countMin = 3, int countMax = 6);
	FlatPattern(const FlatPattern& other);
	sf::FloatRect generate(sf::FloatRect lastPlatBox) override;
	void clearGeneration() override;
	sf::FloatRect getLast() const override;
	 std::unique_ptr<PlatformPattern> clone() const override;
private:
	std::default_random_engine gen;
	std::uniform_int_distribution<int> xDistr;
	std::uniform_int_distribution<int> yDistr;
	std::uniform_int_distribution<int> widthDistr;
	std::uniform_int_distribution<int> heightDistr;
	std::uniform_int_distribution<int> countDistr;
	msf::Scene * scene;
	sf::FloatRect last;
	std::vector<msf::GameObject*> platforms;
};