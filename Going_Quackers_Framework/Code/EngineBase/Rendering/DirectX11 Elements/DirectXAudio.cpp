#include "DirectXAudio.h"
#include "DirectXGraphics.h"
#include "DirectXWindow.h"
#include <thread>

AudioManager* AudioManager::mp_instance = 0;

AudioManager* AudioManager::GetInstance()
{
	if (mp_instance == 0) 
	{
		mp_instance = new AudioManager();
	}
	return mp_instance;
}

AudioManager::AudioManager()
{
	//this->mp_window = mp_window;

	Initalize();
}

AudioManager::~AudioManager()
{
	m_audioEngine->Suspend();
}

void AudioManager::Initalize()
{
	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
	#ifdef _DEBUG
	flags |= AudioEngine_Debug;
	#endif
	m_audioEngine = std::make_unique<AudioEngine>(flags);
	m_audioReset = false;

	//Check For Silent Mode (No Audio Device)
	if (!m_audioEngine->IsAudioDevicePresent())
	{
		//TODO: ADD ERROR
		//MessageBox(mp_window->m_hwnd, L"No Audio Device? Running in Silent Mode", L"Error", MB_OK);
	}

}

void AudioManager::Update()
{
	//Check if the Audio Device needs to be reset
	if (m_audioReset)
	{
		m_audioReset = false;
		m_audioEngine->Reset();
	}

	if (!m_audioEngine->Update())
	{
		//The Audio Manager has ran into an error!
		if (m_audioEngine->IsCriticalError())
		{
			//Audio Device Lost!
			m_audioReset = true;
		}
	}
}
