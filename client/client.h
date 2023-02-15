#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <windows.h> 
#include <Ws2tcpip.h>
#include <filesystem>
#include <fstream>


//#include "..\protocolAAD\ProtocolAAD.h"
#include "..\dataExchange\dataExchange.h"
using namespace std;


class Client
{
private:
	PCWSTR  ip = (L"127.0.0.1");
	const int port = 5000;

	//int clientID = 1;
	SOCKET clientSocketConnection;
	string clientFilePath;

	void exceptionNetworkConnections(const int& codeError, const string& descriptionError);
	
	void printMessageFromServer(ProtocolAAD data);
	

	void sendFileToServer();

	void workByServer();
public:
	Client(string & clientFilePath);
	bool checkingFileForExistence();
	void connectToServer();

};