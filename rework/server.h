#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include <thread>
#include <list>

using namespace std;

class Server {
    public:
    Server();
    Server(int port);


    private:
    int port;

    list<SOCKET> li;
    list<thread> liTh;

};