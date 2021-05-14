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
	// coloured text
	template <typename T>
	void Log(T info, float r, float g, float b)
	{
		converter.str(std::string());
		converter << info;
		Log(LOG_TYPE::TEXT, converter.str(), r, g, b);
	}
	// specialisation for strings
	template <>
	void Log<std::string>(std::string info, float r, float g, float b)
	{
		Log(LOG_TYPE::TEXT, info, r, g, b);
	}
	template <typename T>
	void Log(T info) 
	{
		Log(info, 1.f, 1.f, 1.f);
	}
	template <typename T>
	void LogWarning(T info)
	{
		converter.str(std::string());
		converter << info;
		LogWarning(converter.str());
	}
	template <>
	void LogWarning<std::string>(std::string info)
	{
		converter.str(std::string());
		converter << info;
		Log(LOG_TYPE::WARNING, info, 0.921f, 0.8f, 0.215f);
	}
	template <typename T>
	void LogError(T info)
	{
		converter.str(std::string());
		converter << info;
		LogError(converter.str());
	}
	template <>
	void LogError<std::string>(std::string info)
	{
		Log(LOG_TYPE::ERR, info, 1.f, 0.15f, 0.15f);
	}
	void ReadLog();
private:
	// the true base function; don't expose this
	void Log(LOG_TYPE category, std::string info, float r, float g, float b);
	void LogToFile();
};
#endif DEBUG