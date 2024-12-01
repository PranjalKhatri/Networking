#include<WinSock2.h>
#include<iphlpapi.h>
#include<WS2tcpip.h>
#include<iostream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main(int argc, char** argv)
{
	WSAData wsaData;
	int status;
	if (argc != 2)
	{
		cerr << "[Error]: Invalid Usage!\n showip host";
		return 1;
	}

	if ((status = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		cerr << "[Error]: WSAStartup failed!\n";
		exit(1);
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		cerr << "[Error]: Version 2.2 can't be initialised!\n";
		exit(1);
	}

	struct addrinfo hints, * res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	//char* ai_error;
	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
		cerr << "[Error]: " << gai_strerror(status) << endl;
		WSACleanup();
	}
	char buf[40];
	void* addr; 
	cout << "IP address for " << argv[1] << "\n";
	for (struct addrinfo* i = res; i != nullptr;i = i->ai_next)
	{
		if (i->ai_family == AF_INET) {
			cout << "\tIPv4 ";
			struct sockaddr_in *ain = (sockaddr_in*)i->ai_addr;
			addr = (void*)(&ain->sin_addr);
		}
		else {
			cout << "\tIPv6 ";
			struct sockaddr_in6* ain = (sockaddr_in6*)i->ai_addr;
			addr = (void*)(&ain->sin6_addr);
		}
		inet_ntop(i->ai_family, addr, buf, sizeof buf);
		cout << buf<< endl;

	}
	freeaddrinfo(res);
	WSACleanup();
	return 0;
}
