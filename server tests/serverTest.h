#include <iostream>
#include <thread>
#include <queue>
#include <list>
#include <WS2tcpip.h> // sockets library
// #include "LogHandler.h"

#define PORT 56000

#pragma comment (lib, "ws2_32.lib")

using namespace std;

class Server {
public:
    Server();
    ~Server();
    boolean init();
    void start();
    void handle_client(SOCKET, string, string, sockaddr_in);
    void handle_data(SOCKET, string, string, string);
    void response_to_client(SOCKET, string);
    
    void server_control();

    


private:
    void close(string, SOCKET);

    bool running;

    string cmd;
    
    SOCKET listening_socket;
    SOCKET clientSocket;

    queue<thread> clients_thread_queue;
    list<SOCKET> clients_sockets_list;
    // LogHandler log();
};





