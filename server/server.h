#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <windows.h> 
#include <Ws2tcpip.h>
#include <string>
#include <filesystem>
#include <fstream>
//#include <thread>
#include "..\ProtocolAAD\ProtocolAAD.h"
using namespace std;

class Server : public ProtocolAAD
{
private:
	PCWSTR  ip = (L"127.0.0.1");
	const int port = 5000;
	const string fileAuthPath = "D:\\Programming\\C++\\clientServer\\server\\usersDataAuthentication.txt";

	string getMessageFromClient(ProtocolAAD *data);
	
	bool checkDirectoryEnteredClient(const string& enteredDirectory);
	
	bool checkRegistrationClientStr(const string& str); // проверка, чтобы в логине и пароли не была символа "_"

	bool authorizationClient(const string& loginClient, const string& passwdClient); // метод, который производит авторизацию клиента
	bool registrationClient(const string& newLoginClient, const string& newPasswdClient); // метод, который производит регистрирацию клиента
	void recvFile(SOCKET client, const string& enteredDirectory);
	
	//void recvFileFromClient(SOCKET client, const string& enteredDirectory); // получаем и сохряняем файл клиента

	void handleClient(SOCKET *clientConnection); // работа с подключённым клиентом	
public:
	void receivingСonnections(); // принимает всех клиентов
};