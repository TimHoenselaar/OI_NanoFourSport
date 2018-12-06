#pragma comment (lib, "ws2_32.lib")
// Include the Winsock library (lib) file

#include <iostream>
#include <WS2tcpip.h>

class UDPServer
{
public:
	UDPServer();
	~UDPServer();
	void Run();

private:
	// Structure to store the WinSock version. This is filled in
	// on the call to WSAStartup()
	WSADATA data;

	// To start WinSock, the required version must be passed to
	// WSAStartup(). This server is going to use WinSock version
	// 2 so I create a word that will store 2 and 2 in hex i.e.
	// 0x0202
	WORD version = MAKEWORD(2, 2);

	sockaddr_in serverHint; // Create a server hint structure for the server

	sockaddr_in client; // Use to hold the client information (port / ip address)
	int clientLength;

	SOCKET in;
};