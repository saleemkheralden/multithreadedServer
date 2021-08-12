/*
 * nicer look of the onefile
 * by: saleem kheralden
 * 12/8/2021 10:20PM
 */

#include <iostream>
#include <thread>
#include <queue>
#include <WS2tcpip.h> // sockets library

#define PORT 56000

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void start();
void handle_client(SOCKET clientSocket, string host, string port, sockaddr_in client);
void handle_data(SOCKET clientSocket, string host, string port, string requestStr);
void response_to_client(SOCKET clientSocket, string res);

queue<SOCKET> sockets_q;
queue<thread> clients_thread_queue;
SOCKET listening_socket;


void start() {

	cout << "Starting server..." << endl;

	while (true) {

		listening_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (listening_socket == INVALID_SOCKET) {
			cerr << "Can't create socket! quitting" << endl;
			return;
		}

		// bind the ip and port to a socket
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(PORT);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(listening_socket, (sockaddr*)&hint, sizeof(hint));

		cout << "Server is up and listening..." << endl;

		// tell winsock the socket is for listening
		listen(listening_socket, SOMAXCONN);

		// wait for connection
		sockaddr_in client;
		int clientSize = sizeof(client);

		SOCKET clientSocket = accept(listening_socket, (sockaddr*)&client, &clientSize);

		// sockets_q.push(clientSocket);

		char host[NI_MAXHOST];		// client's remote name
		char service[NI_MAXSERV];	// service (i.e. port) the client is connected on

		ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
		ZeroMemory(service, NI_MAXSERV);

		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			cout << host << " connected on port " << service << endl;
		}
		else
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			cout << host << " connected on port " <<
				ntohs(client.sin_port) << endl;
		}

		closesocket(listening_socket);

		clients_thread_queue.push(thread(handle_client, clientSocket, host, service, client));

	}
}

void handle_client(SOCKET clientSocket, string host, string port, sockaddr_in client) {

	string requestStr;
	const unsigned int buffer_size = 4096;

	char buffer[buffer_size];

	while (true) {
		ZeroMemory(buffer, buffer_size);
		int bytesReceived;

		try {
			bytesReceived = recv(clientSocket, buffer, buffer_size, 0);
		}
		catch (exception e) {
			cout << "Client disconnected! " << endl;
			closesocket(clientSocket);
			return;
		}

		requestStr = string(buffer, 0, bytesReceived);

		if (bytesReceived == SOCKET_ERROR) {
			cerr << "Error in handle_client()! qutting " << host + " " + port << endl;
			closesocket(clientSocket);
			return;
		}

		if (requestStr != "") {
			// don't know why deleted the last char. check it...
			// cout << host + " " + port + "> " + requestStr.replace(requestStr.length() - 1, 1, "") << endl;
			if (requestStr[requestStr.size() - 1] == '\n')
				cout << host + " " + port + "> " + requestStr;
			else
				cout << host + " " + port + "> " + requestStr << endl;

			handle_data(clientSocket, host, port, requestStr);
		}
		else {
			cout << "Client disconnected!" << endl;
			closesocket(clientSocket);
			return;
		}

		/*closesocket(clientSocket);
		return;*/
	}

}

// implement the logic here
void handle_data(SOCKET clientSocket, string host, string port, string requestStr) {
	response_to_client(clientSocket, requestStr);
}

void response_to_client(SOCKET clientSocket, string res) {
	string response = "Server> " + res + "\n";
	send(clientSocket, (response).c_str(), response.size() + 1, 0);
}

int main() {
	sockets_q = queue<SOCKET>();

	// init winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		cerr << "Can't initialize winwock! quitting" << endl;
		return 0;
	}
	// winsock is initialized

	start();

	WSACleanup();

	return 0;
}


