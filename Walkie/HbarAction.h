#pragma once
#include "MSF.h"

class HbarAction : public msf::Action {
public:
	HbarAction(float health_, uint64_t tag, bool isNothing) : health{ health_ }, Action{tag, isNothing} {};
	float health;
};