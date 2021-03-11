#pragma once
#ifndef DEBUG
#define DEBUG

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

static class Debug
{
	//TODO: singleton-ify
/*public:
	static Debug* getInstance();
	Debug(Debug const&) = delete;
	void operator=(Debug const&) = delete;
private:
	static Debug* SingletonInstance;
	Debug();
	~Debug();*/
public:
	template <typename T>
	/// <summary>
	/// put things in the log queue
	/// </summary>
	static void Log(T info) {
		// straight from first year
		std::ofstream file("debug_log.txt");
		if (!file.is_open()) {
			std::cout << "Debug log file couldn't be opened!";
			return;
		}
		file << info;
		file.close();
		return;
	}
	// todo coloured text

	template <typename T>
	void LogWarning(T info);

	template <typename T>
	void LogError(T info);

	static std::stringstream ReadLog();
};
#endif DEBUG