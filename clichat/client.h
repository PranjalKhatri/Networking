#include "utils.h"
#include <fstream>
#include <thread>

inline void recieveAndPrint(int sockfd, int max_data_size, const std::string &out_file)
{
	std::ofstream chat_of(out_file);
	if (!chat_of.is_open())
	{
		std::cerr << "Unable to create file for output\n";
		return;
	}
	std::vector<char> buf(max_data_size);
	while (true)
	{
		int numbytes = receive(sockfd, buf);
		if (numbytes <= 0)
		{
			std::cerr << "Closed connection with server\n";
			closesocket(sockfd);
			return;
		}
		else
		{
			buf[numbytes] = '\0';
			chat_of << buf.data() << "\n"; // Write to file
			std::cout<<buf.data()<<std::endl;
			chat_of.flush();			   // Flush at runtime level
		}
	}
}

inline void inputSender(int sockfd, int max_data_size)
{
	std::string input;
	std::cout<<"Enter your alias : ";
	std::string name;
	std::cin>>name;
	std::string msg = "[" + name + "] : ";
	std::cin.clear();
	std::cin.ignore();
	while (true)
	{
		std::cout << "Msg : ";
		std::getline(std::cin, input); // Read input from console
		if (input.empty())
		{
			std::cerr << "Empty input. Exiting sender.\n";
			break;
		}

		// Send input to socket
		int bytesSent = send(sockfd, std::string(msg+input).c_str(), input.size()+msg.size(), 0);
		if (bytesSent == SOCKET_ERROR)
		{
			std::cerr << "Error sending data\n";
			break;
		}
	}
}

inline int bootListener(const std::string &host, const std::string &PORT, int MAXDATASIZE)
{
	int sockfd{}, numbytes{};
	std::vector<char> server_name(INET6_ADDRSTRLEN);
	addrinfo hints{}, *serverinfo{}, *tmp{};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int rv = getaddrinfo(host.c_str(), PORT.c_str(), &hints, &serverinfo);
	if (rv != 0)
	{
		std::cerr << "getaddrinfo " << gai_strerror(rv) << "\n";
		return 2;
	}

	for (tmp = serverinfo; tmp != NULL; tmp = tmp->ai_next)
	{
		if ((sockfd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol)) == SOCKET_ERROR)
		{
			std::cerr << "client:socket\n";
			continue;
		}
		if (connect(sockfd, tmp->ai_addr, tmp->ai_addrlen) == SOCKET_ERROR)
		{
			closesocket(sockfd);
			std::cerr << "client:connect\n";
			continue;
		}
		break;
	}
	if (tmp == NULL)
	{
		std::cerr << "client: failed to connect\n";
		return 3;
	}
	inet_ntop(tmp->ai_family, getInAddr((sockaddr *)tmp->ai_addr), server_name.data(), server_name.size());
	std::cout << "listener : connecting to " << server_name.data() << "\n";
	freeaddrinfo(serverinfo);

	std::string outfile = "chat.log";

	#ifdef LISTENER
		std::cout<<"Reciever\n";
		recieveAndPrint(sockfd,MAXDATASIZE,outfile);
	#else 
		std::cout<<"talker\n";
		inputSender(sockfd,MAXDATASIZE);
	#endif 

	return 0;
}
