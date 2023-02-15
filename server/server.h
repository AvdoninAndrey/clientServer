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
	const string fileAuthPath = "C:\\Programming\\�++\\clientServer\\server\\usersDataAuthentication.txt";
	
	string getMessageFromClient(ProtocolAAD data);
	bool checkFirectoryEnteredClient(const string& enteredDirectory);

	bool authorizationClient(const string& loginClient, const string& passwdClient); // �����, ������� ���������� ����������� �������
	bool registrationClient(const string& newLoginClient, const string& newPasswdClient); // �����, ������� ������������ �������


	void recvFileFromClient(); // �������� � ��������� ���� �������

	void handleClient(SOCKET* clientConnection); // ������ � ������������ ��������	
public:
	string textAuth = "��� �������� ����� �� ������ ���������� �������������� ��� ������������������\n"
		"��� ����������� ������� \"logIn\"\n"
		"��� ����������� ������� \"register\"\n"
		"��� ���������� ������� \"disconnect\"";
	string textLogin = "������� �����: ";
	string textPassword = "������� ������: ";
	string textNewLogin = "���������� �����: ";
	string textNewPassword = "���������� ������: ";

	void receiving�onnections(); // ��������� ���� ��������

};