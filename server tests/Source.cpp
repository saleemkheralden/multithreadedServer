#include "serverTest.cpp"
// #include <thread>
#include <iostream>
// #include <fstream>

// #include <WS2tcpip.h>

// #pragma comment (lib, "ws2_32.lib")

// using namespace std;

int main() {

	// Server s = Server();
	// s.init();
	// s.start();

	SOCKET s = SOCKET();
	WSADATA ver;
	WORD word = MAKEWORD(2, 2);
	int wsOk = WSAStartup(word, &ver);
	if (wsOk != 0) {
		return 0;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	unsigned long nonBlocking = 1;
	ioctlsocket(listenSocket, FIONBIO, &nonBlocking);
	

	sockaddr_in sock;
	sock.sin_family = AF_INET;
	sock.sin_port = htons(5600);
	sock.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listenSocket, (const sockaddr*)&sock, sizeof(sock));
	listen(listenSocket, SOMAXCONN);
	sockaddr_in client;
	int clientSize = sizeof(client);
	cout << "waiting" << endl;
	SOCKET clientSocket = NULL;
	cout << "done waiting" << endl;

	ioctlsocket(clientSocket, FIONBIO, &nonBlocking);
	Sleep(5000);
	cout << "gonna sleep for another sec" << endl;
	Sleep(1000);
	char buff[4096];
	for (;;)
	{
		clientSocket = accept(listenSocket, (sockaddr *)&client, &clientSize);
		cout << "non blocking" << endl;
		cout << clientSocket << endl;
		Sleep(1000);
	}
	



	cout << "end" << endl;
	return 0;
}


