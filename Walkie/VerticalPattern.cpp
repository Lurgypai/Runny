#include "stdafx.h"
#include "VerticalPattern.h"
#include "PlatformGC.h"
#include "PlatformLC.h"

VerticalPattern::VerticalPattern(msf::Scene * scene, int xMin, int xMax, int yMin, int yMax, int width, int heightMin, int heightMax) :
	scene{scene},
	eng{ time(nullptr) },
	xDistr{ xMin, xMax },
	yDistr{ yMin, yMax },
	width{ width },
	heightDistr{ heightMin, heightMax } {
}

VerticalPattern::VerticalPattern(const VerticalPattern & other) : 
	scene{other.scene},
	eng{ other.eng },
	xDistr{other.xDistr},
	yDistr{ other.yDistr },
	width{ other.width },
	heightDistr{ other.heightDistr } {
}

sf::FloatRect VerticalPattern::generate(sf::FloatRect lastPlatBox) {
	if (platforms.size() != 0)
		platforms.clear();

	float x = xDistr(eng) + lastPlatBox.left + lastPlatBox.width;
	float y = lastPlatBox.top;

	float xDistance = xDistr(eng);
	float yDistance = yDistr(eng);

	float width_ = width;
	float height = heightDistr(eng) * 10;

	y -= height;

	std::shared_ptr<msf::GameObject> lPlat = scene->addGObject("stage");
	lPlat->setGraphics<PlatformGC>(width, height);
	lPlat->setLogic<PlatformLC>(width, height);
	lPlat->setPos(x, y);

	std::shared_ptr<msf::GameObject> rPlat = scene->addGObject("stage");
	rPlat->setGraphics<PlatformGC>(width, height);
	rPlat->setLogic<PlatformLC>(width, height);
	rPlat->setPos(x - xDistance, y - yDistance);

	platforms.push_back(lPlat.get());
	platforms.push_back(rPlat.get());

	last = sf::FloatRect{ {x, y}, {width_, height} };


	return last;
}

void VerticalPattern::clearGeneration() {
	for (auto& plat : platforms) {
		scene->removeGObject(plat->getTag(), "stage");
	}
	platforms.clear();
}

sf::FloatRect VerticalPattern::getLast() const
{
	return last;
}

std::unique_ptr<PlatformPattern> VerticalPattern::clone() const
{
	return std::make_unique<VerticalPattern>(*this);
}
