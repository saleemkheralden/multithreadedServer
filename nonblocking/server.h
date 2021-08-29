#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include <thread>
#include <list>
#include <string>

#define PORT 56000

#pragma comment (lib, "ws2_32.lib")

using namespace std;

class Server {
public:
    Server(int port=PORT);
    void start();

private:
    boolean init();
    void handle_client(SOCKET, sockaddr_in);
    string get_ip(sockaddr_in);
    void server_control();
    void response_to_client(SOCKET, string);
    void shutdown();

    bool running;
    int port;

    string cmd;

    SOCKET listening_socket;
    SOCKET clientSocket;

    list<SOCKET> clients_sockets_list;
    list<thread> clients_thread_list;

};