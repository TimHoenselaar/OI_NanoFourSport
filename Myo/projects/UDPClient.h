#pragma once

#include <iostream>
#include <WS2tcpip.h>

#include "pch.h"

#pragma comment (lib, "ws2_32.lib")

/*!
 *  @brief     Sets up a udp client connection to send char[].
 *  @author    Jens Verstappen & Ryan Vrosch 
 *  @version   1.0
 *  @date      2018-11-30
 *  @copyright GNU Public License.
=======
/*!
 *  @brief     Sets up a udp client connection to send char[].
 *  @author    Jens Verstappen & Ryan Vrosch 
 *  @version   1.0
 *  @date      2018-11-30
 *  @copyright GNU Public License.
>>>>>>> Develop
 */
class UDPClient
{
public:
	UDPClient(); // localhost 
	UDPClient(char* ip, int port);

	~UDPClient();
	bool Write(const char* message, size_t messageLength);
	bool Start();
	void Stop();

	bool ConnectionEnabled() { return running; };
private:
	WSADATA		data;
	WORD		version;
	sockaddr_in server;
	SOCKET		out;

	char* ip;
	int port;
	bool running;
	
};