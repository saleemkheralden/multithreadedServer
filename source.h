#include <iostream>
#include <thread>
#include <queue>
#include <WS2tcpip.h> // sockets library

#define PORT 56000

#pragma comment (lib, "ws2_32.lib")

using namespace std;

class Server {
    public:
        Server();
        ~Server();
        boolean init();
        void start();
        void handle_client(SOCKET clientSocket, string host, string port, sockaddr_in client);
        virtual void handle_data(SOCKET clientSocket, string host, string port, string requestStr);
        void response_to_client(SOCKET clientSocket, string requestStr);

    private:
        SOCKET listening_socket;
        queue<thread> clients_thread_queue;
}