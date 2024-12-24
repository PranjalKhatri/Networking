#include "utils.h"

inline void receiveAndBroadcast(int sockfd, int listener, int fdmax, fd_set& master, std::vector<char>& buf) {
	int nbytes = receive(sockfd, buf);
	if (nbytes <= 0) {
		closesocket(sockfd);
		FD_CLR(sockfd, &master);
	}
	else {
		// Broadcast data to everyone else
		for (int j = 0; j <= fdmax; j++) {
			if (FD_ISSET(j, &master)) {
				if (j != listener /*&& j != sockfd*/) {
					if (send(j, buf.data(), nbytes, 0) == -1) {
						std::cerr << "send\n";
					}
				}
			}
		}
	}
}


inline int bootServer(std::string PORT = "3028", int backlog = 10) {
	fd_set master, read_fds;
	int fdmax{};

	int listener;	//listening socket descriptor
	int newfd;		//newly accepted socket descriptor

	sockaddr_storage remote_addr; //client address
	socklen_t addrlen;

	std::vector<char> buf(256);

	std::vector<char> remoteIP(INET6_ADDRSTRLEN);

	char yes = 1;

	addrinfo hints{}, * ai{}, * tmp{};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	FD_ZERO(&master);	//clean master and read fds
	FD_ZERO(&read_fds);

	int rv;
	if ((rv = getaddrinfo(NULL, PORT.c_str(), &hints, &ai)) != 0) {
		std::cerr << "getaddr " << gai_strerror(rv) << "\n";
		exit(1);
	}
	for (tmp = ai;tmp != NULL;tmp = tmp->ai_next)
	{
		listener = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (listener < 0)continue;

		//reusability in case server closes and needs to boot up fast
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

		if (bind(listener, tmp->ai_addr, tmp->ai_addrlen) == SOCKET_ERROR) {
			closesocket(listener);
			continue;
		}
		break;
	}
	if (tmp == NULL)
	{
		std::cerr << "unalbe to bind the listener\n";
		exit(2);
	}
	//free the address info
	freeaddrinfo(ai);

	if (listen(listener, backlog) == SOCKET_ERROR) {
		std::cerr << "Unable to listen on the port\n";
		exit(3);
	}

	//add the listener to the master set
	FD_SET(listener, &master);
	fdmax = listener;
	std::cout<<"Listening on PORT "<<PORT<<"\n";
	//main loop
	for (;;)
	{
		read_fds = master; // temporary copy
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == SOCKET_ERROR) {
			std::cerr << "select\n";
			exit(4);
		}
		for (int i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &read_fds)) {
				//new connection arrived on listener
				if (i == listener) {
					addrlen = sizeof(remote_addr);
					newfd = accept(listener, (sockaddr*)&remote_addr, &addrlen);
					if (newfd == SOCKET_ERROR) {
						std::cerr << "accept\n";
					}
					else {
						FD_SET(newfd, &master);
						if (newfd > fdmax)fdmax = newfd;
						std::cout << "new connection from "
							<< inet_ntop(remote_addr.ss_family, getInAddr((sockaddr*)&remote_addr), remoteIP.data(), INET6_ADDRSTRLEN)
							<< "on socket " << newfd << "\n";
					}
				}
				else {//some data arrived from client
					receiveAndBroadcast(i, listener, fdmax, master, buf);
				}
			}
		}
	}
	return 0;
}

