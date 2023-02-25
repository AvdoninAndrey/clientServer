#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <windows.h> 
#include <Ws2tcpip.h>
#include <fstream>
#include "..\ProtocolLib\ProtocolAAD.h"
//#include "..\Exception\ExceptionNetwork.h"


class Server : public ProtocolAAD
{
private:
	PCWSTR  ip = (L"127.0.0.1"); // ip
	const int port = 5000; // port
	const std::string fileAuthPath = "D:\\Programming\\C++\\clientServer\\server\\usersDataAuthentication.txt"; // путь до файла с учётными записями клиентов

	std::string getMessageFromClient(jsonDataFormat data); // метод, получащий сообщение, который отправил клиент
	bool checkDirectoryEnteredClient(const std::string& enteredDirectory); // проверка, что указанный каталог клиентом существует
	
	bool checkRegistrationClientStr(const std::string& str); // проверка, чтобы в логине и пароли не была символа "_"

	bool authorizationClient(const std::string& loginClient, const std::string& passwdClient); // метод, который производит авторизацию клиента
	bool registrationClient(const std::string& newLoginClient, const std::string& newPasswdClient); // метод, который производит регистрирацию клиента
	void recvFile(SOCKET client, const std::string& enteredDirectory); // метод, который принимает и сохраняет файл на сервере
	
	void disconnectClient(SOCKET clientConnection); // для отключения клиента от сервара
	
	void handleClient(SOCKET *clientConnection); // работа с подключённым клиентом	
public:
	void receivingСonnections(); // принимает всех клиентов
};