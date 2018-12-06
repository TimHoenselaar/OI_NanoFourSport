// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SerialPort.h"

/*Portname must contain these backslashes, and remember to
replace the following com port*/
const char *port_name = "\\\\.\\COM4";

//String for incoming data
char incomingData[MAX_DATA_LENGTH];

std::vector<std::string> Split(char s[], char delimiter);

int main()
{
	SerialPort arduino(port_name);
	if (arduino.isConnected()) std::cout << "Connection Established" << std::endl;
	else std::cout << "ERROR, check port name";

	std::vector<std::string> accelData = Split(incomingData, ','); //x = 0, y = 1, z = 2

	while (arduino.isConnected()) 
	{
		char send = '1';
		arduino.writeSerialPort(&send, 1);
		int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);

		std::cout << incomingData << std::endl;

		Sleep(100);
	}
}

std::vector<std::string> Split(char s[], char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}