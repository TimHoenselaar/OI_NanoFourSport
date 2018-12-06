#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include "pch.h"

class UDPClient
{
public:
	UDPClient();
	~UDPClient();
	void Write(char[] message);
	void Stop();

	

private:
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	sockaddr_in server;
	SOCKET out;
};