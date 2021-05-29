#pragma once
#include "Audio.h"

class DirectXWindow;

using namespace DirectX;
class AudioManager
{
	/* Singleton */
public:
	static AudioManager* GetInstance();
	AudioManager(AudioManager const&) = delete;
	void operator = (AudioManager const&) = delete;

	static AudioManager* mp_instance;
public:
	AudioManager();
	~AudioManager();

	void Initalize();

	void Update();

	//AudioEngine* GetAudioEngine() { return m_audioEngine; }
	void OnNewAudioDevice() noexcept { m_audioReset = true; }

public:

	std::unique_ptr<AudioEngine> m_audioEngine;
	DirectXWindow* mp_window;

	bool m_audioReset;
};