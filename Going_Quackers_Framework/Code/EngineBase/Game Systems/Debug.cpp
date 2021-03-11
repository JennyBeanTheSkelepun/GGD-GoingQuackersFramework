#include "Debug.h"

/*Debug::~Debug() {
	if (SingletonInstance != nullptr) {
		delete SingletonInstance;
		SingletonInstance = nullptr;
	}
}

Debug* Debug::getInstance() {
	if (SingletonInstance == 0)
		SingletonInstance = new Debug();
	return SingletonInstance;
}*/

std::stringstream Debug::ReadLog() {
	std::string line;
	std::stringstream buffer;
	std::ifstream file("debug_log.txt");
	if (file.is_open()) {
		while (getline(file, line)) {
			buffer << line << std::endl;
		}
		file.close();
	}
	else {
		std::cout << "Unable to open debug log file!";
	}
	return buffer;
}