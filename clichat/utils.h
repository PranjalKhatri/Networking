#pragma once
/// @author Pranjal
/// @version 0.0.0

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <vector>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

/// <summary>
/// Initializes the use of WS2_32.dll and version 2.2 of Winsock.
/// </summary>
/// <returns>
/// <para>0: Success - Initialization completed successfully.</para>
/// <para>1: Failure - WSAStartup failed to initialize Winsock.</para>
/// <para>2: Failure - Version 2.2 of Winsock is not available.</para>
/// </returns>
int setupWinsock();

/// <summary>
/// Retrieves the address from a sockaddr structure, supporting IPv4 and IPv6.
/// </summary>
/// <param name="sa">Pointer to a sockaddr structure.</param>
/// <returns>Pointer to the address portion of the sockaddr structure.</returns>
void* getInAddr(sockaddr* sa);

/// <summary>
/// Receives data from a socket into a buffer.
/// </summary>
/// <param name="sockfd">Socket file descriptor.</param>
/// <param name="buf">Buffer to store received data.</param>
/// <returns>
/// Number of bytes received, or -1 if an error occurred.
/// </returns>
int receive(int sockfd, std::vector<char>& buf);
