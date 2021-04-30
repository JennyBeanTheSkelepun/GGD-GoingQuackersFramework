#pragma once
#ifndef DEBUG
#define DEBUG

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../ImGui/ImGui SourceCode/imgui.h"

static class Debug
{
public:
	static Debug* getInstance();
	Debug(Debug const&) = delete;
	void operator=(Debug const&) = delete;
private:
	static Debug* SingletonInstance;
	Debug();
	~Debug();
	std::stringstream queue;
public:
	template <typename T>
	/// <summary>
	/// put things in the log queue; white text
	/// </summary>
	void Log(T info) {
		Log(info, 1.f, 1.f, 1.f);
	}
	// coloured text
	template <typename T>
	void Log(T info, float r, float g, float b) {
		queue << r << " " << g << " " << b << " " << info << "\n";
		return;
	}

	template <typename T>
	void LogWarning(T info){
		Log(info, 0.921f, 0.8f, 0.215f);
	}

	template <typename T>
	void LogError(T info) {
		Log(info, 1.f, 0.15f,0.15f);
	}
	std::stringstream ReadLog();
};
#endif DEBUG