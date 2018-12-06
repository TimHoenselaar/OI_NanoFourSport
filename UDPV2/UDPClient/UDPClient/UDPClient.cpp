#include "UDPClient.h";

UDPClient::UDPClient()
{
	data;

	version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// Not ok! Get out quickly
		printf("Can't start Winsock! ");
		return;
	}

	// Create a hint structure for the server
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(1111); // LKDittle to big endian conversion
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

	// Socket creation, note that the socket type is datagram
	out = socket(AF_INET, SOCK_DGRAM, 0);
}

UDPClient::~UDPClient()
{
	closesocket(out);
	WSACleanup();
}

void UDPClient::Write(char[] message)
{
	int sendOk = sendto(out, message.c_str(), message.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR)
	{
		printf("That didn't work! ");
	}
}

void UDPClient::Stop()
{
	closesocket(out);
	WSACleanup();
}