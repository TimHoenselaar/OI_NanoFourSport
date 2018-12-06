#include "pch.h"
#include "UDPServer.h"

UDPServer::UDPServer()
{
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		std::cout << "Can't start Winsock! " << wsOk;
		return;
	}

	// Create a socket, notice that it is a user datagram socket (UDP)
	in = socket(AF_INET, SOCK_DGRAM, 0);

	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	serverHint.sin_port = 1;//htons(54000); // Convert from little to big endian

	// Try and bind the socket to the IP and port
	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
		return;
	}

	clientLength = sizeof(client);

}

UDPServer::~UDPServer()
{
	closesocket(in);
	WSACleanup();
}

void UDPServer::Run()
{
	char buf[1024];

	std::cout << "server is running!" << std::endl;

	while (true)
	{
		ZeroMemory(&client, clientLength); // Clear the client structure
		ZeroMemory(buf, 1024); // Clear the receive buffer

		// Wait for message
		int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			std::cout << "Error receiving from client " << WSAGetLastError() << std::endl;
			continue;
		}

		// Display message and client info
		char clientIp[256]; // Create enough space to convert the address byte array
		ZeroMemory(clientIp, 256); // to string of characters

		// Convert from byte array to chars
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		std::cout << "Message recv from " << clientIp << " : " << buf << std::endl;
	}
}
