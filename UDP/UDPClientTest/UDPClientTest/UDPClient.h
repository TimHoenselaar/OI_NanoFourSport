#pragma comment (lib, "ws2_32.lib")
// Include the Winsock library (lib) file

#include <iostream>
#include <WS2tcpip.h>
#include <string>

class UDPClient
{
public:
	UDPClient();
	~UDPClient();
	bool Write(std::string s);
	void Run();

private:
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	sockaddr_in server;
	int serverLength;
	SOCKET out;
};