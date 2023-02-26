#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <windows.h> 
#include <Ws2tcpip.h>
#include <fstream>
#include <thread>
#include "..\ProtocolLib\ProtocolAAD.h"
#include "..\Exception\ExceptionDirOrFile.h"
#include <vector>

class Server : public ProtocolAAD
{
private:
	PCWSTR  ip = (L"127.0.0.1"); // ip
	const int port = 5000; // port
	const std::string fileAuthPath = "D:\\Programming\\C++\\clientServer\\server\\usersDataAuthentication.txt"; // путь до файла с учётными записями клиентов
	
	const int maxClientConnection = 2;
	int currentConnectionClient = 0;


	std::string getMessageFromClient(jsonDataFormat data); // метод, получащий сообщение, который отправил клиент
	bool checkDirectoryEnteredClient(const std::string& enteredDirectory); // проверка, что указанный каталог клиентом существует
	
	bool checkRegistrationClientStr(const std::string& str); // проверка, чтобы в логине и пароли не была символа "_"

	bool authorizationClient(const std::string& loginClient, const std::string& passwdClient); // метод, который производит авторизацию клиента
	bool registrationClient(const std::string& newLoginClient, const std::string& newPasswdClient); // метод, который производит регистрирацию клиента
	void recvFile(SOCKET client, const std::string& enteredDirectory); // метод, который принимает и сохраняет файл на сервере
	
	void disconnectClient(SOCKET clientConnection, USHORT portClient, const std::string& loginClient); // для отключения клиента от сервара
	
	void handleClient(SOCKET clientConnection, USHORT portClient); // работа с подключённым клиентом	
	
	bool checkAuthorizationClientAlreadyExists(const std::string & loginClient); // проверяет если такой клиент уже был авторизован на сервере
	void removeLoginFromTheListOfAuthorizedemoveLogin(const std::string& loginClient); // когда клиент отправил файл и отключается его логин удаляется из списка авторизованных клиентов


	std::vector<std::string> authorizedСlientsList;

	class ClientInfo { //Класс хранит сокет клиента и порт на котором он взаимодействует с пользователем
	private:
		SOCKET clientConnection;
		USHORT portClient;
	public:
		void printIncomingConnectionClient();
		SOCKET getSocketClient();
		USHORT getPortClient();
		ClientInfo() = default;
		ClientInfo(SOCKET clientConnection, USHORT portClient);
	};

public:
	void receivingСonnections(); // принимает клиента
};