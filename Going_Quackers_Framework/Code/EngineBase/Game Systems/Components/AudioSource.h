#ifndef _AUDIOSOURCE_H
#define _AUDIOSOURCE_H

#include "Component.h"
#include "../../Rendering/DirectX11 Elements/DirectXAudio.h"

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
class AudioSource : public Component
{

public:
	AudioSource(GameObject* owner);
	~AudioSource();

	//- Basic Loops -//
	void OnDestroy() override;
	void Update() override;

	//- ImGui UpdateLoop -//
	void ImGUIUpdate() override;

	//- Scene Save and Load -//
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

	void Play();
	void Stop();
	void Pause();

	void SetVolume(int volume) { m_volume = volume; }
	int GetVolume() { return m_volume; }
	void SetPitch(float pitch) { m_pitch = pitch; }
	float GetPitch() { return m_pitch; }
	void SetPan(float pan) { m_pan = pan; }
	float GetPan() { return m_pan; }

	void SetLoop(bool value) { m_loop = value; }
	bool IsLooping() { return m_loop; }

	void SetMute(bool value);
	bool IsMuted() { return m_muted; }

private:
	const wchar_t* CharToWchar(const char* ch);

	void SetPath();
	void SubmitPath();

private:
	std::string m_inputPath;
	char* m_selectedAudioPath;

	std::unique_ptr<SoundEffect> m_audioSource;
	std::unique_ptr<SoundEffectInstance> m_instance;

	int m_prevVolume;
	int m_volume;
	float m_pitch;
	float m_pan;

	bool m_muted;
	bool m_loop;
	
	bool m_submittedPath; //Used at Initlization to state if the first path has been set
};
#endif