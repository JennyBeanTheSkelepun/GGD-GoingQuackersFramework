#include "Debug.h"

Debug* Debug::SingletonInstance = 0;

Debug::Debug()
{
	// clear log file
	std::ofstream file("debug_log.txt", std::ofstream::out | std::ofstream::trunc);
	file.close();
	queueEnd = 0;
}

Debug::~Debug() 
{
	// save log
	LogToFile();

	if (SingletonInstance != nullptr) 
	{
		delete SingletonInstance;
		SingletonInstance = nullptr;
	}
}

Debug* Debug::getInstance() 
{
	if (SingletonInstance == 0)
		SingletonInstance = new Debug();
	return SingletonInstance;
}

void Debug::ReadLog() 
{
	// just call ImGui::ColoredText from here
	for (int i = 0; i < queueEnd; i++)
	{
		std::string dataCount;
		if (queue[i].type == LOG_TYPE::WARNING) dataCount = "[WARNING] ";
		else if (queue[i].type == LOG_TYPE::ERR) dataCount = "[ERROR] ";
		dataCount += queue[i].data+" ("+std::to_string(queue[i].count)+")";
		ImGui::TextColored(ImVec4(queue[i].r, queue[i].g, queue[i].b, 1.f), dataCount.c_str());
	}
}

void Debug::Log(LOG_TYPE category, std::string info, float r, float g, float b)
{
	LogData tempData;
	tempData.type = category;
	tempData.data = info;
	tempData.r = r;
	tempData.g = g;
	tempData.b = b;

	for (int i = 0; i < queueEnd; i++)
	{
		if (queue[i] == tempData)
		{
			queue[i].count++;
			return;
		}
	}

	// no copy found
	if (queueEnd == 10000) LogToFile();
	tempData.count = 1;
	queue[queueEnd] = tempData;
	queueEnd++;
}

void Debug::LogToFile()
{
	std::ofstream ofile("debug_log.txt", std::ofstream::out | std::ofstream::app);
	if (!ofile.is_open())
	{
		std::cout << "Debug log file couldn't be opened!";
		return;
	}
	for (int i = 0; i <= 10000; i++) {
		if (queue[i].type == LOG_TYPE::WARNING) ofile << "[WARNING] ";
		else if (queue[i].type == LOG_TYPE::ERR) ofile << "[ERROR] ";
		ofile << queue[i].data << std::endl;
	}
	ofile.close();
	queueEnd = 0;
}