#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <windows.h> 
#include <Ws2tcpip.h>
#include <fstream>
#include "..\ProtocolLib\ProtocolAAD.h"
#include "..\Exception\ExceptionDirOrFile.h"

class Client : public ProtocolAAD
{
private:
	PCWSTR  ip = (L"127.0.0.1"); // ip
	const int port = 5000; // port
	SOCKET clientSocketConnection;
	std::filesystem::path clientFilePath;

	void printMessageFromServer(jsonDataFormat data); // метод, который печатает сообщение отправленное сервером
	jsonDataFormat generatingDataFileToSend(); // метод, который генерирует информацию о файле, которую нужно отправить
	void sendFileToServer(jsonDataFormat dataFile); // метод, который отправляет файл на сервер
	void disconnect(SOCKET connect);
	void workByServer(); // метод, который осуществляет взаимодействие с сервером
	
public:
	Client(std::filesystem::path clientFilePath); // конструктор, инициализируется указанным путём до файла
	void connectToServer(); // подключение к серверу
};
