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
public:
	template <typename T>
	/// <summary>
	/// put things in the log queue; black text
	/// </summary>
	static void Log(T info) {
		Log(info, 0f, 0f, 0f);
	}
	// coloured text
	template <typename T>
	static void Log(T info, float r, float g, float b) {
		// straight from first year
		std::ofstream file("debug_log.txt");
		if (!file.is_open()) {
			std::cout << "Debug log file couldn't be opened!";
			return;
		}
		file << r << " " << g << " " << b << " " << info;
		file.close();
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
	static std::stringstream ReadLog();
};
#endif DEBUG