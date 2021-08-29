#include "serverTest.cpp"
// #include <thread>
#include <iostream>
// #include <fstream>

// #include <WS2tcpip.h>

// #pragma comment (lib, "ws2_32.lib")

// using namespace std;

int main() {

	Server s = Server();
	s.init();
	s.start();
	
	cout << "end" << endl;
	return 0;
}


