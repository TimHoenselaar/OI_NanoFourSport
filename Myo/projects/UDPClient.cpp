#include "UDPClient.h";




/// @brief initializes and starts a UDP client with IP: localhost (127.0.0.1) and port: 1111.
UDPClient::UDPClient()
{
	version = MAKEWORD(2, 2);
	
	this->ip = "127.0.0.1";
	this->port = 1111;
	Start();
}

/// @brief initializes and starts a UDP client with specified ip and port.
/// @param ip as string notation, example: "127.0.0.1".
/// @param port has to be within 1-9999
/// @warning Does not check if IP-adress is valid.
/// @exception ip, port, Winsock

UDPClient::UDPClient(char* ip, int port)
{
	if (ip == NULL)					throw "ip";
	if (port < 1 || port > 9999)	throw "port";
	 
	version = MAKEWORD(2, 2);

	this->ip = ip;
	this->port = port;

	Start();	
}



/// @brief starts udp connection if connection is not started.
/// @exception Winsock
bool UDPClient::Start()
{
	if (running) return false;

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)	throw "Winsock";

	// Create a hint structure for the server
	server.sin_family = AF_INET;				// AF_INET = IPv4 addresses
	server.sin_port = htons(port);				// LKDittle to big endian conversion
	inet_pton(AF_INET, ip, &server.sin_addr);	// Convert from string to byte array

	out = socket(AF_INET, SOCK_DGRAM, 0);
	running = true;
}


UDPClient::~UDPClient()
{
	Stop();
}

/// @brief 
/// @param message
/// @param messageLength
/// @post returns true if message is succesfully send. 
///       returns false if error has occurred. 
bool UDPClient::Write(const char *message, size_t messageLength)
{
	if (message == NULL) return false;

	int sendOk = sendto(out, message, messageLength + 1, 0, (sockaddr*)&server, sizeof(server));
	if (sendOk == SOCKET_ERROR) return false;
	else						return true;
}

/// @brief stops udp connection if connection is enabled.
void UDPClient::Stop()
{
	if (!running) return;

	closesocket(out);
	WSACleanup();
	running = false; 
}