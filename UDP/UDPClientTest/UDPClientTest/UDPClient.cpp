#include "pch.h"
#include "stdafx.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include "iostream"
#include "conio.h"
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27015

int main() {

	int Result;
	WSADATA wsaData;

	SOCKET SocketNumber = INVALID_SOCKET;
	struct sockaddr_in clientService;

	int recvbuflen = DEFAULT_BUFLEN;
	char *sendbuf = "Client: sending data test";
	char recvbuf[DEFAULT_BUFLEN] = "";

	Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != NO_ERROR) {
		cout << "WSAStartup failed with error: " << Result;
		getch();
	}

	SocketNumber = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SocketNumber == INVALID_SOCKET) {
		cout << "socket failed with error: " << WSAGetLastError();
		getch();
		WSACleanup();
	}

	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(DEFAULT_PORT);

	Result = connect(SocketNumber, (SOCKADDR*)&clientService, sizeof(clientService));
	if (Result == SOCKET_ERROR) {
		cout << "connect failed with error: " << WSAGetLastError();
		closesocket(SocketNumber);
		getch();
		WSACleanup();

	}
	Result = send(SocketNumber, sendbuf, (int)strlen(sendbuf), 0);
	if (Result == SOCKET_ERROR) {
		cout << "send failed with error: " << WSAGetLastError();
		closesocket(SocketNumber);
		getch();
		WSACleanup();

	}

	cout << "Bytes Sent: " << Result;

	Result = shutdown(SocketNumber, SD_SEND);
	if (Result == SOCKET_ERROR) {
		cout << "shutdown failed with error: " << WSAGetLastError();
		closesocket(SocketNumber);
		getch();
		WSACleanup();

	}
	do {

		Result = recv(SocketNumber, recvbuf, recvbuflen, 0);
		if (Result > 0)
			cout << "Bytes received: " << Result;
		else if (Result == 0)
			cout << "Connection closed";
		else
			cout << "recv failed with error: " << WSAGetLastError();
		getch();
	} while (Result > 0);


	Result = closesocket(SocketNumber);
	if (Result == SOCKET_ERROR) {
		wprintf(L"close failed with error: %d\n", WSAGetLastError());
		getch();
		WSACleanup();
		return 1;
	}

	WSACleanup();

	return 0;
}