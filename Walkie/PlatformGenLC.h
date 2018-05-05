#pragma once
#include "MSF.h"
#include "PlatformPattern.h"

class PlatformGenLC : public msf::DLogicComponent {
public:
	PlatformGenLC(msf::GameObject* playerHandle);
	PlatformGenLC(const PlatformGenLC& other);
	void update() override;
	std::unique_ptr<DLogicComponent> clone() override;
	void generateInitial(const sf::FloatRect& rect);

	template<typename T, typename... Args>
	void addPattern(Args... args) {
		patterns.push_back(std::make_unique<T>(args...));
	}
private:
	PlatformPattern* selectPattern();
	sf::FloatRect lastPlat;
	msf::GameObject* player;
	std::vector<std::unique_ptr<PlatformPattern>> patterns;
	PlatformPattern* current;
	PlatformPattern* middle;
	PlatformPattern* last;
	float threshold{300};
	int currentId{2};
	int middleId{1};
	int lastId{0};
};