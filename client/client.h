#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <windows.h> 
#include <Ws2tcpip.h>
#include <fstream>
#include <random>
#include "..\ProtocolLib\ProtocolAAD.h"
#include "..\Exception\ExceptionDirOrFile.h"
#include "..\base64\base64\base64\base64.h"


class Client : public ProtocolAAD, public Base64
{
private:
	const PCWSTR  IP = (L"127.0.0.1"); // ip
	const int PORT = 55000; // port
	const double USE_VERSION_PROTOCOL = 1.1;
	const std::string PATH_FILE_ID_CLIENTS = "D:\\Programming\\C++\\clientServer\\client\\idClients.txt";
	const std::string CORRECT_ALPHABET_INPUT_CLIENT = " !\"#$ % &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	int generationIdForClient(); // генерирует id клиента
	bool checkIdClientAlreadyExists(const int& idClient); // проверяет есть ли клиент с таким же id
	void removeClientIdFromFile(const int& idClient); // когда клиент отключается удаляется id из списка
	bool checkInputClient(const std::string& input);
	void correctInput(std::string& input);
	int clientId;
	SOCKET clientSocketConnection; // сокет клиента
	std::filesystem::path clientFilePath; // путь до файла, который клиент хочет передать на сервер
	void printMessageFromResponseServer(ProtocolAAD data); // метод, который печатает ответ сервера

	void sendFileToServer(); // метод, который отправляет файл на сервер
	void disconnect(); // метод, который закрывает соединение с сервером
	void workByServer(); // метод, который осуществляет взаимодействие с сервером
public:
	Client(std::filesystem::path clientFilePath); // конструктор, инициализируется указанным путём до файла
	Client() = default;
	void connectToServer(); // подключение к серверу
	std::string getCorectAphabetInputClient();
};
