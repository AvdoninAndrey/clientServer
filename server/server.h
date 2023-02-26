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
	const std::string fileAuthPath = "D:\\Programming\\C++\\clientServer\\server\\usersDataAuthentication.txt"; // ���� �� ����� � �������� �������� ��������
	
	const int maxClientConnection = 2;
	int currentConnectionClient = 0;


	std::string getMessageFromClient(jsonDataFormat data); // �����, ��������� ���������, ������� �������� ������
	bool checkDirectoryEnteredClient(const std::string& enteredDirectory); // ��������, ��� ��������� ������� �������� ����������
	
	bool checkRegistrationClientStr(const std::string& str); // ��������, ����� � ������ � ������ �� ���� ������� "_"

	bool authorizationClient(const std::string& loginClient, const std::string& passwdClient); // �����, ������� ���������� ����������� �������
	bool registrationClient(const std::string& newLoginClient, const std::string& newPasswdClient); // �����, ������� ���������� ������������� �������
	void recvFile(SOCKET client, const std::string& enteredDirectory); // �����, ������� ��������� � ��������� ���� �� �������
	
	void disconnectClient(SOCKET clientConnection, USHORT portClient, const std::string& loginClient); // ��� ���������� ������� �� �������
	
	void handleClient(SOCKET clientConnection, USHORT portClient); // ������ � ������������ ��������	
	
	bool checkAuthorizationClientAlreadyExists(const std::string & loginClient); // ��������� ���� ����� ������ ��� ��� ����������� �� �������
	void removeLoginFromTheListOfAuthorizedemoveLogin(const std::string& loginClient); // ����� ������ �������� ���� � ����������� ��� ����� ��������� �� ������ �������������� ��������


	std::vector<std::string> authorized�lientsList;

	class ClientInfo { //����� ������ ����� ������� � ���� �� ������� �� ��������������� � �������������
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
	void receiving�onnections(); // ��������� �������
};