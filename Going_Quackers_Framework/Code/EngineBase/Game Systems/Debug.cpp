#include "Debug.h"

Debug* Debug::SingletonInstance = 0;

Debug::Debug(){
	// clear log file
	std::ofstream file("debug_log.txt", std::ofstream::out | std::ofstream::trunc);
	file.close();
}

Debug::~Debug() {
	// write buffer to file
	std::ofstream ofile("debug_log.txt");
	if (!ofile.is_open()) {
		std::cout << "Debug log file couldn't be opened!";
		return;
	}
	ofile << queue.str();
	ofile.close();

	if (SingletonInstance != nullptr) {
		delete SingletonInstance;
		SingletonInstance = nullptr;
	}
}

Debug* Debug::getInstance() {
	if (SingletonInstance == 0)
		SingletonInstance = new Debug();
	return SingletonInstance;
}

std::stringstream Debug::ReadLog() {
	std::stringstream buffer;
	buffer << queue.str();
	return buffer;
}