#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <exception>  


#pragma once
class Communicator
{
	
private: 
	DCB COM_Settings;
	HANDLE COM_Handler;
	BOOL fSuccess;
	char ComPort[20];
	void StartCOM();
	

public:
	Communicator(uint8_t portNumber, DWORD CBR_baudRate);
	Communicator(uint8_t portNumber, DWORD CBR_baudRate, byte byteSize);
	Communicator(uint8_t portNumbe, DWORD CBR_baudRate, byte byteSize, byte parity);
	bool Write(const char *message, int messageLength);
	

	~Communicator();
};
#endif //COMMUNICATOR_H
