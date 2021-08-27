#include "server.h"


Server::Server(int port=PORT) {
    this->clients_thread_list = list<thread>();
    this->clients_sockets_list = list<SOCKET>();
    this->running = true;
    this->cmd = "";
    this->port = port;
}

// destructor

boolean Server::init() {
    WSADATA wsData;
    WORD word = MAKEWORD(2, 2);
    int wsOk = WSAStartup(word, &wsData);
	
    if (wsOk != 0) {
		cerr << "Can't initialize winwock! quitting" << endl;
		return false;
	}

	cout << "Winsock initiated!" << endl;
	return true; 
}

void Server::start() {
	cout << "Starting server..." << endl;
    
    while (this->running) {
        
        this->listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        unsigned long nonBlocking = 1;
        ioctlsocket(this->listening_socket, FIONBIO, &nonBlocking);

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
        
        this->clientSocket = NULL;
        ioctlsocket(this->clientSocket, FIONBIO, &nonBlocking);
		
        while (this->running) {
            this->clientSocket = accept(this->listening_socket, (sockaddr*)&client, &clientSize);

            if (this->clientSocket != (unsigned int)(-1) && this->running) {
                // start thread
                cout << "connected" << endl;
                break;
            }
        }
        
        closesocket(this->listening_socket);
    } // end while running
    
}


int main() {
    Server s = Server();
    s.init();
    s.start();
    return 0;
}