
#include "Communicator.h"


Communicator::Communicator(uint8_t portNumber, DWORD CBR_baudRate)
{
	sprintf(ComPort, "\\\\.\\COM%d", portNumber);
	COM_Settings.BaudRate	= CBR_baudRate;    
	COM_Settings.ByteSize	= 8;             
	COM_Settings.Parity		= NOPARITY;      
	COM_Settings.StopBits	= ONESTOPBIT;    
	StartCOM();
	
}
Communicator::Communicator(uint8_t portNumber, DWORD CBR_baudRate, byte byteSize)
{
	sprintf(ComPort, "\\\\.\\COM%d", portNumber);
	COM_Settings.BaudRate	= CBR_baudRate;     
	COM_Settings.ByteSize	= byteSize;
	COM_Settings.Parity		= NOPARITY;      
	COM_Settings.StopBits	= ONESTOPBIT;    
	StartCOM();	
}
Communicator::Communicator(uint8_t portNumber, DWORD CBR_baudRate, byte byteSize, byte parity)
{
	sprintf(ComPort, "\\\\.\\COM%d", portNumber);
	COM_Settings.BaudRate	= CBR_baudRate;    
	COM_Settings.ByteSize	= byteSize;
	COM_Settings.Parity		= parity;
	COM_Settings.StopBits	= ONESTOPBIT;    
	StartCOM();
}

void Communicator::StartCOM()
{
	COM_Handler = CreateFile(ComPort, GENERIC_READ | GENERIC_WRITE, 
							 0, NULL,OPEN_EXISTING, 0, NULL);
	if (COM_Handler == INVALID_HANDLE_VALUE)
	{
		throw std::exception("ERROR: cannot start COM Handler on specified PORT");
	}

}

bool Communicator::Write(const char *message, int messageLength)
{
	if (message == NULL || messageLength <= 0) return false;

	unsigned long nBytesWritten;

	WriteFile(COM_Handler, message, messageLength, &nBytesWritten, NULL);
	if (nBytesWritten == messageLength)	return true;
	
	return false;
}



Communicator::~Communicator()
{
	CloseHandle(COM_Handler);
}

