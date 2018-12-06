#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

// Saves us from typing std::cout << etc. etc. etc.
using namespace std;

void main(int argc, char* argv[])
{
	WSADATA data;

	WORD version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// Not ok! Get out quickly
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	// Create a hint structure for the server
	sockaddr_in server;
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(1111); // LKDittle to big endian conversion
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

	// Socket creation, note that the socket type is datagram
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	// Write out to that socket

	for (int i = 0; i < 100; i++)
	{
		string s = "test\0";
		int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

		if (sendOk == SOCKET_ERROR)
		{
			cout << "That didn't work! " << WSAGetLastError() << endl;
		}
		Sleep(1000);
	}


	// Close the socket
	//closesocket(out);

	// Close down Winsock
	//WSACleanup();
}