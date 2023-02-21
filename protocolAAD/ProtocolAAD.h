
#pragma once
#include <string>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
using namespace std;

class ProtocolAAD {
private:
	char headArray[64] = {};
	char messageArray[512] = {};
	char filenameArray[32] = {};
	int sizeFileBytes = 0;
protected:
	//char* bytesFile;
	string headerAuthentication = "Authentication";
	string headerFile = "File";
	string headerRegistration = "Registration";
	string headerError = "Error";
	string headerSelect = "Select";
	string headerPathInServer = "pathFileInServer";
	string textAuth = "��� �������� ����� �� ������ ���������� �������������� ��� ������������������\n"
		"��� ����������� ������� \"logIn\"\n"
		"��� ����������� ������� \"register\"\n"
		"��� ���������� ������� \"disconnect\"";

	string textLogin = "������� �����: ";
	string textPassword = "������� ������: ";

	string textNewLogin = "���������� �����: ";
	string textNewPassword = "���������� ������: ";

	string successfulLogIn = "����������� ������ �������!";
	string failedLogIn = "������������ ����� ��� ������, ���������� ��� ���.";

	string enteredDirectoryFailed = "������ �������� �� ������� �� ����������";
	string enteredDirectoryForClient = "������� �������, � ������� ����� ��������� ����: ";

	string successfulRegistration = "����������� ������ �������! ������ �������������� �� �������.";
	string failedRegistration = "������������ � ����� ������� ��� ���������� ��� �� �������������� ������������ ������ \"_\"";

	string incorrectInput = "�� ����� �������������� �������, ���������� ��� ���";

	string flagResultTrue = "true";
	string flagResultFalse = "false";
	void sendDataText(SOCKET clientConnection, ProtocolAAD data);
	ProtocolAAD* recvDataText(SOCKET clientConnection);
public:
	void printMessage();
	string getMessage();
	string getFilename();
	int getFileSize();
	ProtocolAAD() = default;
	ProtocolAAD(string &header, string &message);
	ProtocolAAD(string& header, string& filename, int fileSize);
};
