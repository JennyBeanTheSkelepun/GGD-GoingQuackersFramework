#ifndef TIME_H
#define TIME_H

#include <chrono>

static class Time {
public:
	static void FrameEnd();
	static void SetTimeScale(float timeScale);
	static float GetTimeScale();
	static float GetTime();
	static float GetTimeMilliseconds();
	static float GetDeltaTime();
private:
	static float mf_timeScale;
	static float mf_totalTime;
	static std::chrono::duration<float> m_deltaTime;
	static std::chrono::time_point<std::chrono::steady_clock> m_lastFrame;
};

#endif

