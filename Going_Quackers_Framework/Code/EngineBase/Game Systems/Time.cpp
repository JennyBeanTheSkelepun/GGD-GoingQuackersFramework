#include <algorithm>
#include "Time.h"
#include <algorithm>

float Time::mf_timeScale = 1.0f;
float Time::mf_totalTime = 0.0f;
std::chrono::time_point<std::chrono::steady_clock> Time::m_lastFrame = std::chrono::steady_clock::now();
std::chrono::duration<float> Time::m_deltaTime = std::chrono::duration<float>::zero();

void Time::FrameEnd() 
{
	std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();
	m_deltaTime = (currentTime - m_lastFrame) * mf_timeScale;
	mf_totalTime += m_deltaTime.count();

	m_lastFrame = currentTime;
}

void Time::SetTimeScale(float timeScale) 
{
	mf_timeScale = std::max(0.0f, timeScale);
}

float Time::GetTimeScale() 
{
	return mf_timeScale;
}

float Time::GetTime() 
{
	return mf_totalTime;
}

float Time::GetTimeMilliseconds() 
{
	return mf_totalTime * 1000.0f;
}

float Time::GetDeltaTime() 
{
	return m_deltaTime.count();
}