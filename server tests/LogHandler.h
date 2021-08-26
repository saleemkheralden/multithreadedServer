#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

class LogHandler
{

public:
	LogHandler();
	~LogHandler();

	void run();


	static queue<string> log_queue;

private:
	ofstream LogFile;
};

