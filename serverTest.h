#include <iostream>
#include <fstream>
#include <thread>
#include <queue>
#include <WS2tcpip.h> // sockets library
#include <vector>
#include <list>

#define PORT 56000

#pragma comment (lib, "ws2_32.lib")

using namespace std;

class Server {
public:
    Server();
    Server(Server&&) = default;
    Server& operator=(Server&&) = default;

    ~Server();
    boolean init();
    void start();
    void handle_client(SOCKET, string, string, sockaddr_in);
    void handle_data(SOCKET, string, string, string);
    void response_to_client(SOCKET, string);
    void server_control();

private:
    void close(string, SOCKET);

    SOCKET listening_socket;
    queue<thread> clients_thread_queue;
    //vector<SOCKET> clients_sockets_queue;
    list<SOCKET> clients_sockets_queue;
    ofstream LogFile;

};

