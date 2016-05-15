#include "AudioManager.h"

AudioManager::AudioManager()
{
	// This is only needed in Windows desktop apps
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
	#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
	#endif
	std::unique_ptr<AudioEngine> audEngine(new AudioEngine(eflags));

	std::unique_ptr<SoundEffect> m_music(new SoundEffect(audEngine.get(), L"../Assets/Music.wav"));
	auto Music = m_music->CreateInstance();
	Music->Play(true);
}

void AudioManager::update()
{

}
