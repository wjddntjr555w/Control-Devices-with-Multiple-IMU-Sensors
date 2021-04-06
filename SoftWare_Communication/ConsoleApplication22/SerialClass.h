#ifndef SERIALCLASS_H_INCLUDED
#define SERIALCLASS_H_INCLUDED

#define ARDUINO_WAIT_TIME 2000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class Serial
{
private:
	//Serial comm handler
	
	//Connection status
	bool connected;
	//Get various information about the connection
	COMSTAT status;
	//Keep track of last error
	DWORD errors;

public:
	HANDLE hSerial;
	Serial(char *portName);
	
	~Serial();

	int ReadData(char* buffer, unsigned int nbChar);
	
	
	bool WriteData(char *buffer, unsigned int nbChar);
	
	bool IsConnected();


};

#endif