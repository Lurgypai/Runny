#pragma once
#include "MSF.h"

class WalkieC : public msf::Controller {
public:
	enum Input {
		upd,
		upu,
		downd,
		downu,
		leftd,
		leftu,
		rightd,
		rightu
	};
	WalkieC();
	~WalkieC();

	void readInputs() override;

private:
	bool up;
	bool down;
	bool left;
	bool right;
};

