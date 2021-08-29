<<<<<<< HEAD
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
    if (!this->init()) {
        cerr << "Server init failed!" << endl;
        return;
    }
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
                this->clients_sockets_list.push_back(this->clientSocket);
                this->clients_thread_list.push_back(thread(&Server::handle_client, this, clientSocket, client));

                cout << "connected" << endl;
                break;
            }
        }
        
        closesocket(this->listening_socket);
    } // end while running
    
}

void Server::handle_client(SOCKET clientSocket, sockaddr_in client) {
    string address = "", port = "";

    string client_request;
    const unsigned int buffer_size = 4096;
    char buffer[buffer_size];

    while (this->running) {
        ZeroMemory(buffer, buffer_size);
        int bytesReceived;

        try {
            bytesReceived = recv(clientSocket, buffer, buffer_size, 0);
        } catch (exception e) {
            cerr << "exception in handle_client" << endl;
            return;
        }

        if (bytesReceived > 0 ) {
            client_request = string(buffer, 0, bytesReceived);
            if (client_request != "") {
                cout << client_request << endl;
            } else {
                return;
            }
        }

    }

}




int main() {
    Server s = Server();
    s.start();

    return 0;
}

=======
>>>>>>> parent of 1f3f886 (remove logHandler)
