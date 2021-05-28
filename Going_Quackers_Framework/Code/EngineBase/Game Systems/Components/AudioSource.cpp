#include "AudioSource.h"
#include "../../Rendering/Graphics.h"

AudioSource::AudioSource(GameObject* owner) : Component(owner, ComponentTypes::AUDIOSOURCE, "Audio Source")
{
	m_selectedAudioPath = new char[100]{ "" };
	submittedPath = false;

	SetVolume(5);
	m_prevVolume = 0;
	SetPitch(0.0f);
	SetPan(0.0f);
	SetLoop(false);
	SetMute(false);
}

AudioSource::~AudioSource()
{
}

void AudioSource::OnDestroy()
{
	this->~AudioSource();
}

void AudioSource::Update()
{

}

void AudioSource::ImGUIUpdate()
{
	ImGui::InputText("Audio Path", m_selectedAudioPath, 128);

	if (ImGui::Button("Submit Audio Path"))
	{
		submittedPath = true;
		SubmitPath();
	}

	if (!submittedPath)
		return;

	ImGui::Separator();

	ImGui::SliderInt("Volume Slider", &m_volume, 0.0f, 10.0f);
	ImGui::SliderFloat("Pitch Slider", &m_pitch, -1.0f, 1.0f);
	ImGui::SliderFloat("Panning Slider", &m_pan, -1.0f, 1.0f);

	m_instance->SetVolume(m_volume);
	m_instance->SetPitch(m_pitch);
	m_instance->SetPan(m_pan);

	ImGui::Separator();

	ImGui::Checkbox("Loop", &m_loop);
	ImGui::Checkbox("Mute", &m_muted);

	SetMute(m_muted);

	ImGui::Separator();

	if (ImGui::Button("Play"))
	{
		Play();
	}
	if (ImGui::Button("Pause"))
	{
		Pause();
	}
	if (ImGui::Button("Stop"))
	{
		Stop();
	}
}

json* AudioSource::SceneSave()
{
	return nullptr;
}

void AudioSource::SceneLoad(json* componentJSON)
{
}

void AudioSource::Play()
{
	m_instance->Play(m_loop);
}

void AudioSource::Stop()
{
	m_instance->Stop();
}

void AudioSource::Pause()
{
	m_instance->Pause();
}

void AudioSource::SetMute(bool value)
{
	m_muted = value;
	if (m_muted)
	{
		if (m_prevVolume == 0)
		{
			m_prevVolume = m_volume;
			SetVolume(m_volume - m_volume);
		}
	}
	else
	{
		if (m_prevVolume != 0)
		{
			SetVolume(m_prevVolume);
			m_prevVolume = 0;
		}
	}

	//If Volume is increased while muted
	if (m_volume != 0)
	{
		m_muted = false;
	}
}

const wchar_t* AudioSource::CharToWchar(const char* ch)
{
	const size_t len = strlen(ch) + 1;
	wchar_t* wch = new wchar_t[len];
	mbstowcs(wch, ch, len);
	return wch;
}

void AudioSource::SubmitPath()
{
	m_audioSource.release();
	m_audioSource.reset();
	m_audioSource = std::make_unique<SoundEffect>(AudioManager::GetInstance()->m_audioEngine.get(), CharToWchar(m_selectedAudioPath));
	m_instance = m_audioSource->CreateInstance();
}
