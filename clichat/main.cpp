#include "server.h"
#include "client.h"
#include <thread>
int main(int argc, char** argv){
	int iRes = setupWinsock();
	if (iRes != 0)
		return iRes;
    std::string host="localhost",PORT="3028";
    int maxdatasize = 100,backlog=10;
    #ifdef CLIENT
    if (argc > 1)
    {
      host = argv[1]; // Take host from arguments
    }
    if (argc > 2)
    {
      PORT = argv[2]; // Take port from arguments
    }
    std::thread t(bootListener,host,PORT,maxdatasize);
    #else 
    if (argc > 1)
    {
        PORT = argv[1]; // Take port from arguments
    }
    std::cout<<PORT<<"\n";
    std::thread t(bootServer,PORT,backlog);
    #endif
    t.join();
    WSACleanup();
}
