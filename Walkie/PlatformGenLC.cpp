#include "stdafx.h"
#include "PlatformGenLC.h"

PlatformGenLC::PlatformGenLC(msf::GameObject* playerHandle) :
	patterns{},
	player{playerHandle},
	current{NULL},
	middle{NULL},
	last{NULL} {
}

//need a way to copy unique ptrs
PlatformGenLC::PlatformGenLC(const PlatformGenLC& other) : 
	lastPlat{ other.lastPlat },
		player{other.player},
	current{other.current},
	middle{other.middle},
	last{other.last} {
	for (auto& pattern : other.patterns) {
		patterns.push_back(pattern->clone());
	}

}

void PlatformGenLC::update() {
	float platx = lastPlat.left;
	float platy = lastPlat.top;

	float playerX = player->getPos().x;
	float playerY = player->getPos().y;
	float distance = hypot(playerX - platx, playerY - platy);

	if (playerX > platx && distance > threshold) {

		last->clearGeneration();
		last = middle;
		middle = current;
		current = selectPattern();
		current->generate(middle->getLast());
		lastPlat = last->getLast();
	}
}

std::unique_ptr<msf::DLogicComponent> PlatformGenLC::clone()
{
	return std::make_unique<PlatformGenLC>(*this);
}

void PlatformGenLC::generateInitial(const sf::FloatRect& rect) {
	last = patterns[lastId].get();
	middle = patterns[middleId].get();
	current = patterns[currentId].get();

	lastPlat = last->generate(rect);
	middle->generate(last->getLast());
	current->generate(middle->getLast());
}

PlatformPattern * PlatformGenLC::selectPattern() {
	//fix
	srand(time(NULL));
	int size = patterns.size();
	int i = rand() % size;
	while (i == currentId || i == middleId || i == lastId) {
		i = rand() % size;
	}

	std::cout << "last: " << lastId << std::endl;
	std::cout << "middle: " << middleId << std::endl;
	std::cout << "current: " << currentId << std::endl;
	std::cout << "new: " << i << std::endl;
	std::cout << std::endl;
	lastId = middleId;
	middleId = currentId;
	currentId = i;
	return patterns[i].get();
}
