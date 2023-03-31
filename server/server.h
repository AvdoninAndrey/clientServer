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
#include "..\base64\base64\base64\base64.h"
#include <map>


class Server : public ProtocolAAD, public Base64
{
private:
	const PCWSTR  IP = (L"127.0.0.1"); // IP
	const int PORT = 55000; // PORT
	const std::string FILE_AUTH_PATH = "D:\\Programming\\C++\\clientServer\\server\\usersDataAuthentication.txt"; // путь до файла с учётными записями клиентов
	const std::string DEFAULT_DIRECTORY_FOR_SAVE_FILES = "D:\\Programming\\C++\\clientServer\\server\\filesClients"; // стандартный каталог куда могут сохраняться файлы клиентов
	const int MAX_CLIENT_CONNECTIONS = 3; // максимальное количество соединений
	const double USE_VERSION_PROTOCOL = 1.1; // используемая версия протокола

	const std::map<std::string, std::string> OPERATIONS_SERVER = { {"OPERATION_AUTHENTICATION", "logIn"}, {"OPERATION_REGISTRATION", "register"}, {"DISCONNECT", "disconnect"}}; // список операций на сервере при подключении

	int currentConnectionClient = 0; // счётчик текущих подключений

	bool checkDirectoryEnteredClient(const std::string& enteredDirectory); // проверка, что указанный каталог для сохранения существует
	
	bool checkRegistrationClientStr(const std::string& str); // проверка, чтобы в логине и пароли не была символа "_"

	bool authorizationClient(const std::string& loginClient, const std::string& passwdClient); // метод, который производит аутентификации клиента
	bool registrationClient(const std::string& newLoginClient, const std::string& newPasswdClient); // метод, который производит регистрирацию клиента
	void recvFile(ProtocolAAD requestFileSendFromClient, const bool flagDefaultPath, std::string pathForSave = ""); // метод, который принимает и сохраняет файл на сервере
	
	
	
	
	bool checkAuthorizationClientAlreadyExists(const std::string & loginClient); // метод, который проверяет если такой клиент уже был аутентифицирован на сервере
	void removeLoginFromTheListOfAuthorizedemoveLogin(const std::string& loginClient); // при отключении от сервера логин клиента удаляется из списка аутентифицированнных клиентов

	bool checkRequestConnetionFromClient(ProtocolAAD requestConnetion); // производит проверку запроса на подключения
	bool checkRequestFileSendFromClient(ProtocolAAD requestFileSend); // производит проверку запроса на передачу файла

	std::vector<std::string> authorizedСlientsList = {}; // хранит список аутентифицированных клиентов на сервере

	class ClientInfo { //Класс хранит сокет клиента, порт на котором он взаимодействует с пользователем и id клиента, а также если клиент прошёл аутентификацию и логин
	public:
		SOCKET clientConnection;
		USHORT portClient;
		int clientId;
		std::string loginClient = "";
		void printIncomingConnectionClient();
		ClientInfo(SOCKET clientConnection, USHORT portClient, const int ClientId);
	};
	void disconnectClient(ClientInfo dataClient); // для отключения клиента от сервара
	void handleClient(ClientInfo client); //обработка подключённого клиента	

	//=========Возможные ответы сервера======
	const std::string responseSuccessfulConnection = "You have successfully connected to the server. To send a file to the server, you need to log in or register:\n"
		"to log in, enter \"logIn\", to register, enter \"register\", to disconnect, enter \"disconnect\"";
	const std::string responseServerOverloaded = "the server is overloaded";
	
	const std::string responseSuccessfulOperationAuthentication = "Enter your username and password";
	const std::string responseSuccessfulOperationRegistration = "Come up with a username and password and enter";
	const std::string responseSuccessfulOperationDisconnect = "You have been disconnected from the server";
	const std::string responseFailedOperation = "You requested a non-existent operation";

	const std::string responseSuccessfulAuthentication = "You have successfully authenticated.\nSpecify the directory to save the file on the server or enter \"default\" \nif you want to save the file in a standard directory";
	const std::string responseFailedAuthentication = "The data is incorrect \n or is the client with this login already authenticated, please try again";
	const std::string responseSuccessfulRegistration = "You have successfully registered";
	const std::string responseFailedRegistration = "registration failed: a client with this username already exists\nor you used a prohibited one symbol \"_\", please try again";

	const std::string responseSuccessfulSelectPath = "The path is specified correctly";
	const std::string responseFailedSelectPath = "The path is incorrect, try again";

	const std::string responseSuccessfulFileSend = "The file was saved successfully";
	const std::string responseFailedFileSend = "Request incorrect file not accepted";
public:
	void receivingСonnections(); // принимает соединения клиентов
};