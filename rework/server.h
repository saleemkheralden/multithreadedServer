#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include <thread>
#include <list>

#define PORT 56000

#pragma comment (lib, "ws2_32.lib")

using namespace std;

class Server {
public:
    Server(int);

    boolean init();
    void start();
    // void handle_client(SOCKET, string, string, sockaddr_in);
    // void handle_data(SOCKET, string, string, string);
    // void response_to_client(SOCKET, string);
    
    // void server_control();


private:
    // void close(string, SOCKET);

    bool running;
    int port;

    string cmd;
    
    SOCKET listening_socket;
    SOCKET clientSocket;

    list<SOCKET> clients_sockets_list;
    list<thread> clients_thread_list;

};