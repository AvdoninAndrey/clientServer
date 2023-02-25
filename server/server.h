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
	const std::string fileAuthPath = "D:\\Programming\\C++\\clientServer\\server\\usersDataAuthentication.txt"; // ���� �� ����� � �������� �������� ��������

	std::string getMessageFromClient(jsonDataFormat data); // �����, ��������� ���������, ������� �������� ������
	bool checkDirectoryEnteredClient(const std::string& enteredDirectory); // ��������, ��� ��������� ������� �������� ����������
	
	bool checkRegistrationClientStr(const std::string& str); // ��������, ����� � ������ � ������ �� ���� ������� "_"

	bool authorizationClient(const std::string& loginClient, const std::string& passwdClient); // �����, ������� ���������� ����������� �������
	bool registrationClient(const std::string& newLoginClient, const std::string& newPasswdClient); // �����, ������� ���������� ������������� �������
	void recvFile(SOCKET client, const std::string& enteredDirectory); // �����, ������� ��������� � ��������� ���� �� �������
	
	void disconnectClient(SOCKET clientConnection); // ��� ���������� ������� �� �������
	
	void handleClient(SOCKET *clientConnection); // ������ � ������������ ��������	
public:
	void receiving�onnections(); // ��������� ���� ��������
};