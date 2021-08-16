#include "serverTest.h"


Server::Server() {
	cout << "When calling the function start it must be called in a new thread!" << endl;
	
	this->clients_thread_queue = queue<thread>();
	this->clients_sockets_queue = list<SOCKET>();

	this->LogFile.open("log.txt", ios_base::app);
}

Server::~Server() {
	this->LogFile.close();
	
	cout << "dump sockets" << endl;

	while (!this->clients_sockets_queue.empty()) {
		cout << "close sockets" << this->clients_sockets_queue.front() << endl;
		closesocket(this->clients_sockets_queue.front());
		this->clients_sockets_queue.pop_front();
	}

	//while (!this->clients_thread_queue.empty()) {
	//	cout << "kill thread" << this->clients_thread_queue.front().get_id() << endl;
	//	this->clients_thread_queue.front().~thread();
	//	this->clients_thread_queue.pop();
	//}



	
	WSACleanup();


}


boolean Server::init() {
	// init winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		cerr << "Can't initialize winwock! quitting" << endl;
		return false;
	}

	cout << "Winsock initiated!" << endl;
	return true;
}

void Server::server_control() {
	string input = "";
	while (input != "exit")
		cin >> input;
	this->~Server();
	exit(0);
}

void Server::start() {
	cout << "Starting server..." << endl;
	
	thread th(&Server::server_control, this);

	while (true) {

		this->listening_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (this->listening_socket == INVALID_SOCKET) {
			cerr << "Can't create socket! quitting" << endl;
			return;
		}

		// bind the ip and port to a socket
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(PORT);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(this->listening_socket, (sockaddr*)&hint, sizeof(hint));

		cout << "Server is listening to client..." << endl;

		// tell winsock the socket is for listening
		listen(this->listening_socket, SOMAXCONN);

		// wait for connection
		sockaddr_in client; // client sockaddr_in
		int clientSize = sizeof(client);

		SOCKET clientSocket = accept(this->listening_socket, (sockaddr*)&client, &clientSize);

		this->clients_sockets_queue.push_back(clientSocket);

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

		this->clients_thread_queue.push(thread(&Server::handle_client, this, clientSocket, host, service, client, this->clients_sockets_queue.size() - 1));
	}
}

void Server::handle_client(SOCKET clientSocket, string host, string port, sockaddr_in client, int vector_index) {
	string requestStr;  // client's message
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
			cerr << "Error in receive_data()! qutting " << host + " " + port << endl;
			closesocket(clientSocket);
			return;
		}

		if (requestStr != "") {
			if (requestStr[requestStr.size() - 1] == '\n') {
				this->LogFile << host + " " + port + "> " + requestStr;
				//cout << host + " " + port + "> " + requestStr;
			}
			else {
				this->LogFile << host + " " + port + "> " + requestStr + "\n";
				//cout << host + " " + port + "> " + requestStr << endl;
			}

			handle_data(clientSocket, host, port, requestStr);
		}
		else {
			cout << "Client disconnected!" << endl;
			this->clients_sockets_queue.remove(clientSocket);
			this->LogFile.close();
			closesocket(clientSocket);
			return;
		}
	}
}

// here all the logic of handling the client's request will be implemented
void Server::handle_data(SOCKET clientSocket, string host, string port, string requestStr) {
	response_to_client(clientSocket, requestStr);
}

void Server::response_to_client(SOCKET clientSocket, string res) {
	string response = "Server> " + res + "\n";
	send(clientSocket, (response).c_str(), response.size() + 1, 0);
}
