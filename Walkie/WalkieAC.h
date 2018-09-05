#pragma once
#include "MSF.h"
#include "SoundSystem.h"

class WalkieAC : public msf::AudioComponent {
public:
	WalkieAC(SoundSystem* sounds_);
	WalkieAC(WalkieAC& other);
	enum AudioFlag {
		jump = 100,
		drop = 101,
		dot = 102,
		check = 103,
		end = 104
	};


	// Inherited via AudioComponent
	virtual void update(std::vector<msf::Action*>& act) override;

	virtual std::unique_ptr<AudioComponent> clone() override;
private:
	SoundSystem * sounds;
};