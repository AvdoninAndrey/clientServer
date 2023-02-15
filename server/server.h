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
#include "..\dataExchange\dataExchange.h"
using namespace std;

class Server {
private:
	PCWSTR  ip = (L"127.0.0.1");
	const int port = 5000;
	const string fileAuthPath = "C:\\Programming\\с++\\clientServer\\server\\usersDataAuthentication.txt";
	
	string getMessageFromClient(ProtocolAAD data);
	bool checkFirectoryEnteredClient(const string& enteredDirectory);

	bool authorizationClient(const string& loginClient, const string& passwdClient); // метод, который производит авторизацию клиента
	bool registrationClient(const string& newLoginClient, const string& newPasswdClient); // метод, который регистрирует клиента


	void recvFileFromClient(); // получаем и сохряняем файл клиента

	void handleClient(SOCKET* clientConnection); // работа с подключённым клиентом	
public:
	string textAuth = "Для отправки файла на сервер необходимо авторизоваться или зарегестрироваться\n"
		"Для авторизации введите \"logIn\"\n"
		"Для регистрации введите \"register\"\n"
		"Для отключения введите \"disconnect\"";
	string textLogin = "Введите логин: ";
	string textPassword = "Введите пароль: ";
	string textNewLogin = "Придумайте логин: ";
	string textNewPassword = "Придумайте пароль: ";

	void receivingСonnections(); // принимает всех клиентов

};