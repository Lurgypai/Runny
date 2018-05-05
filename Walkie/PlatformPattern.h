#pragma once
#include "MSF.h"

class PlatformPattern {
public:
	virtual sf::FloatRect generate(sf::FloatRect lastPlatBox) = 0;
	virtual void clearGeneration() = 0;
	virtual sf::FloatRect getLast() const = 0;
	virtual std::unique_ptr<PlatformPattern> clone() const = 0;
private:
	bool complete;
};