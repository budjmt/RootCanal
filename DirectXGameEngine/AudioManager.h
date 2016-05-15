#pragma once

#include <Audio.h>

using namespace DirectX;

class AudioManager {
public:
	AudioManager();
	void update();
private:
	std::unique_ptr<AudioEngine> m_audEngine;
};