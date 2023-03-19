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

	int generationIdForClient(); // ���������� id �������
	bool checkIdClientAlreadyExists(const int& idClient); // ��������� ���� �� ������ � ����� �� id
	void removeClientIdFromFile(const int& idClient); // ����� ������ ����������� ��������� id �� ������


	int clientId;
	SOCKET clientSocketConnection; // ����� �������
	std::filesystem::path clientFilePath; // ���� �� �����, ������� ������ ����� �������� �� ������
	void printMessageFromResponseServer(ProtocolAAD data); // �����, ������� �������� ����� �������

	void sendFileToServer(); // �����, ������� ���������� ���� �� ������
	void disconnect(); // �����, ������� ��������� ���������� � ��������
	void workByServer(); // �����, ������� ������������ �������������� � ��������
public:
	Client(std::filesystem::path clientFilePath); // �����������, ���������������� ��������� ���� �� �����
	void connectToServer(); // ����������� � �������
};
