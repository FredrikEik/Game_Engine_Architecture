#pragma once
// Initialization code found here:
// https://github.com/codetechandtutorials/openal-impl/releases/tag/vid1
#include "../CoreMinimal.h"
#include <AL/al.h>
#include <vector>

class SoundBuffer
{
public:
	static SoundBuffer* get();

	ALuint addSoundEffect(const char* filename);
	bool removeSoundEffect(const ALuint& buffer);

private:
	SoundBuffer();
	~SoundBuffer();

	std::vector<ALuint> p_SoundEffectBuffers;
};
