#include "utils.h"

int setupWinsock() {
    WSAData wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << "\n";
        return 1;
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        WSACleanup();
        std::cerr << "Version 2.2 of Winsock is not available.\n";
        return 2;
    }
    return 0;
}

void* getInAddr(sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((sockaddr_in*)sa)->sin_addr);
    }
    return &(((sockaddr_in6*)sa)->sin6_addr);
}

int receive(int sockfd, std::vector<char>& buf) {
    int nbytes = recv(sockfd, buf.data(), buf.size(), 0);
    if (nbytes <= 0) {
        if (nbytes == 0) {
            std::cout << "Socket " << sockfd << " hung up\n";
        }
        else {
            std::cerr << "Error receiving from socket " << sockfd << "\n";
        }
    }
    return nbytes;
}
