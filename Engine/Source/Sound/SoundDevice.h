#pragma once
// Initialization code found here:
// https://github.com/codetechandtutorials/openal-impl/releases/tag/vid1

//#include <AL\alc.h>

class SoundDevice
{
public:
	static SoundDevice* get();

private:
	SoundDevice();
	~SoundDevice();

	struct ALCdevice* p_ALCDevice{};
	struct ALCcontext* p_ALCContext{};

};