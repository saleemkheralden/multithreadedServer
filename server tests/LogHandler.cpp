#include "LogHandler.h"

// queue<string> LogHandler::log_queue = queue<string>();

LogHandler::LogHandler() {
	this->LogFile.open("log.txt", ios_base::app);
	
}

LogHandler::~LogHandler() {
	this->LogFile.close();
}


void LogHandler::run() {
	for (;;) {
		if (!LogHandler::log_queue.empty()) {
			cout << "wrote to log file" << endl;
			this->LogFile << LogHandler::log_queue.front();
			LogHandler::log_queue.pop();
		}
	}
}



