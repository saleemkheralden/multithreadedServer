#include "serverTest.h"


Server::Server() {
	cout << "When calling the function start it must be called in a new thread!" << endl;
	this->clients_thread_queue = queue<thread>();
	this->clients_sockets_list = list<SOCKET>();
	this->running = true;
}

Server::~Server() {
	cout << "deconstructor" << endl;
	for	(SOCKET soc : this->clients_sockets_list) {
		this->clients_sockets_list.remove(soc);
		closesocket(soc);
	}
	// shutdown(socket, how)
	/**
	 * how : 
	 * 	0	recive
	 *  1	send
	 * 	2	both
	 * */
	
	WSACleanup(); 
	cout << "end deconstructor" << endl;
}

void Server::server_control() {
	const unsigned int MAX_LEN = 200;
	for (;;) {
		cin >> this->cmd;

		if (false)
			continue;
		else if (this->cmd.find("broadcast") != string::npos) {
			// else if (this->cmd == "broadcast")
			// might change it to this
			char str_arr[MAX_LEN] = "";
			cin.getline(str_arr, MAX_LEN);
			string msg(str_arr);
			msg = msg.erase(0, 1);

			if (msg != "") {
				cout << "broadcast [" << msg << "] sent" << endl;
			
				for (SOCKET soc : this->clients_sockets_list)
					this->response_to_client(soc, msg);
			}
		}
		else if (this->cmd == "exit") {
			this->running = false;
			return;
		}
		 
	}
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

void Server::start() {
	cout << "Starting server..." << endl;
	
	// starting threads
	thread serverControlThread = thread(&Server::server_control, this);

	while (this->running) {

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

		cout << "Server is listening to clients..." << endl;

		// tell winsock the socket is for listening
		listen(this->listening_socket, SOMAXCONN);

		// wait for connection
		sockaddr_in client; // client sockaddr_in
		int clientSize = sizeof(client);

		this->clientSocket = accept(this->listening_socket, (sockaddr*)&client, &clientSize);

		if (!this->running) {
			closesocket(clientSocket);
			closesocket(listening_socket);
			this->~Server();
			break;
		}

		this->clients_sockets_list.push_back(clientSocket);

		cout << "sockets list: " << endl;
		for (SOCKET s : this->clients_sockets_list)
			cout << s << endl;

		char host[NI_MAXHOST];		// client's remote name
		char service[NI_MAXSERV];	// service (i.e. port) the client is connected on

		ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
		ZeroMemory(service, NI_MAXSERV);

		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
			cout << host << " connected on port " << service << endl;
		}
		else {
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			cout << host << " connected on port " <<
				ntohs(client.sin_port) << endl;
		}

		closesocket(listening_socket);

		this->clients_thread_queue.push(thread(&Server::handle_client, this, clientSocket, host, service, client));
	}

	Sleep(2000);
}

void Server::close(string msg, SOCKET clientSocket) {
	cout << msg << endl;
	this->clients_sockets_list.remove(clientSocket);
	closesocket(clientSocket);
}

void Server::handle_client(SOCKET clientSocket, string host, string port, sockaddr_in client) {
	string requestStr;  // client's message
	const unsigned int buffer_size = 4096;

	char buffer[buffer_size];

	while (this->running) {
		ZeroMemory(buffer, buffer_size);
		int bytesReceived;

		try {
			bytesReceived = recv(clientSocket, buffer, buffer_size, 0);
		}
		catch (exception e) {
			close("Client disconnected! in exception", clientSocket);
			return;
		}

		requestStr = string(buffer, 0, bytesReceived);

		if (bytesReceived == SOCKET_ERROR) {
			close("Error in receive_data()! qutting " + host + " " + port, clientSocket);
			return;
		}

		if (requestStr != "") {
			if (requestStr[requestStr.size() - 1] == '\n')
				cout << host + " " + port + "> " + requestStr;
			else
				cout << host + " " + port + "> " + requestStr << endl;

			handle_data(clientSocket, host, port, requestStr);
		}
		else {
			cout << bytesReceived << endl;
			close("Client disconnected! ", clientSocket);
			return;
		}
	}
}

// here all the logic of handling the client's request will be implemented
void Server::handle_data(SOCKET clientSocket, string host, string port, string requestStr) {
	response_to_client(clientSocket, "Server> " + requestStr + "\n");
}

void Server::response_to_client(SOCKET clientSocket, string response) {
	send(clientSocket, (response).c_str(), response.size() + 1, 0);
}

