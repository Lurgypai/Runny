#include "stdafx.h"
#include "FlatPattern.h"
#include "PlatformGC.h"
#include "PlatformLC.h"

FlatPattern::FlatPattern(msf::Scene* scene, int xMin, int xMax, int yMin, int yMax, int widthMin, int widthMax, int heightMin, int heightMax, int countMin, int countMax) :
	scene{scene},
	gen{time(nullptr)},
	xDistr{xMin, xMax},
	yDistr{ yMin, yMax },
	widthDistr{ widthMin, widthMax },
	heightDistr{ heightMin, heightMax },
	countDistr{ countMin, countMax } {
}

FlatPattern::FlatPattern(const FlatPattern & other) :
	scene{ other.scene },
	gen{ other.gen },
	xDistr{other.xDistr},
	yDistr{ other.yDistr },
	widthDistr{ other.widthDistr },
	heightDistr{ other.heightDistr },
	countDistr{ other.countDistr } {
}

sf::FloatRect FlatPattern::generate(sf::FloatRect lastPlatBox) {
	if (platforms.size() != 0) {
		platforms.clear();
	}

	int platformCount = countDistr(gen);

	for (int i = 0; i != platformCount; i++) {

		int xMove = xDistr(gen);
		int yMove = yDistr(gen);
		float width = widthDistr(gen) * 10.0;
		float height = 10.0;

		float newX = lastPlatBox.left + lastPlatBox.width + xMove;
		float newY = lastPlatBox.top - yMove;

		std::shared_ptr<msf::GameObject> plat = scene->addGObject("stage");
		plat->setGraphics<PlatformGC>(width, height);
		plat->setLogic<PlatformLC>(width, height);
		plat->setPos(newX, newY);

		lastPlatBox = sf::FloatRect{ {newX, newY}, {width, height} };

		platforms.push_back(plat.get());

		if (i == platformCount - 1) {
			last = lastPlatBox;
		}
	}

	return last;
}

void FlatPattern::clearGeneration() {
	for (auto& plat : platforms) {
		scene->removeGObject(plat->getTag(), "stage");
	}
	platforms.clear();
}

sf::FloatRect FlatPattern::getLast() const
{
	return last;
}

std::unique_ptr<PlatformPattern> FlatPattern::clone() const {
	return std::make_unique<FlatPattern>(*this);
}