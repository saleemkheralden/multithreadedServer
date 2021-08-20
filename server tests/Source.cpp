#include "serverTest.h"
#include <thread>
#include <iostream>
#include <fstream>


int main() {

	Server s = Server();
	s.init();
	s.start();

	cout << "end" << endl;
	return 0;
}


