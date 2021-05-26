#pragma once
#ifndef DEBUG
#define DEBUG

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../ImGui/ImGui SourceCode/imgui.h"
#include "../Data Structures/Vectors.h"

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
	std::stringstream converter;
	enum class LOG_TYPE 
	{
		TEXT = 0,
		WARNING,
		ERR
	};
	struct LogData
	{
		LOG_TYPE type;
		float r;
		float g;
		float b;
		std::string data;
		int count;
		bool operator==(const LogData& compare) {
			if (type != compare.type) return false;
			if (r != compare.r) return false;
			if (g != compare.g) return false;
			if (b != compare.b) return false;
			if (data != compare.data) return false;
			return true;
		}
	};
	LogData queue[10000];
	int queueEnd;
public:
	/// <summary>
	/// Add something to the log queue.
	/// </summary>
	/// <param name="info">Data to be logged.</param>
	/// <param name="r">Amount of red (0-1).</param>
	/// <param name="g">Amount of green (0-1).</param>
	/// <param name="b">Amount of blue (0-1).</param>
	/// <param name="type">This parameter is for internal use within the Debug class. Use LogWarning or LogError to log a different type of message.</param>
	template <typename T>
	void Log(T info, float r = 1.f, float g = 1.f, float b = 1.f, LOG_TYPE type = LOG_TYPE::TEXT)
	{
		// convert info to string
		converter.str(std::string());
		converter << info;
		// bounding colours
		if (r > 1.f) r = 1.f;
		else if (r < 0.f) r = 0.f;
		// pass to private version
		Log(converter.str(), r, g, b, type);
	}
	/// <summary>
	/// String-specific logging that avoids unnecessary conversions.
	/// </summary>
	template<>
	void Log<std::string>(std::string info, float r, float g, float b, LOG_TYPE type)
	{
		Log(type, info, r, g, b);
	}
	/// <summary>
	/// Log the X and Y value of a 2D vector.
	/// </summary>
	template<>
	void Log<Vector2>(Vector2 info, float r, float g, float b, LOG_TYPE type) {
		std::string temp = "X: "+std::to_string(info.X) + " Y: "+std::to_string(info.Y);
		Log(type, temp, r, g, b);
	}
	template <typename T>
	void LogWarning(T info)
	{
		Log(info, 0.921f, 0.8f, 0.215f, LOG_TYPE::WARNING);
	}
	template <typename T>
	void LogError(T info)
	{
		Log(info, 1.f, 0.15f, 0.15f, LOG_TYPE::ERR);
	}
	void ReadLog();
	void FlushLog();
private:
	// the true base function; don't expose this
	void Log(LOG_TYPE category, std::string info, float r, float g, float b);
};
#endif DEBUG