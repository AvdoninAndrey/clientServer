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
	const std::string FILE_AUTH_PATH = "D:\\Programming\\C++\\clientServer\\server\\usersDataAuthentication.txt"; // ���� �� ����� � �������� �������� ��������
	const std::string DEFAULT_DIRECTORY_FOR_SAVE_FILES = "D:\\Programming\\C++\\clientServer\\server\\filesClients"; // ����������� ������� ���� ����� ����������� ����� ��������
	const int MAX_CLIENT_CONNECTIONS = 3; // ������������ ���������� ����������
	const double USE_VERSION_PROTOCOL = 1.1; // ������������ ������ ���������

	const std::map<std::string, std::string> OPERATIONS_SERVER = { {"OPERATION_AUTHENTICATION", "logIn"}, {"OPERATION_REGISTRATION", "register"}, {"DISCONNECT", "disconnect"}}; // ������ �������� �� ������� ��� �����������

	int currentConnectionClient = 0; // ������� ������� �����������

	bool checkDirectoryEnteredClient(const std::string& enteredDirectory); // ��������, ��� ��������� ������� ��� ���������� ����������
	
	bool checkRegistrationClientStr(const std::string& str); // ��������, ����� � ������ � ������ �� ���� ������� "_"

	bool authorizationClient(const std::string& loginClient, const std::string& passwdClient); // �����, ������� ���������� �������������� �������
	bool registrationClient(const std::string& newLoginClient, const std::string& newPasswdClient); // �����, ������� ���������� ������������� �������
	void recvFile(ProtocolAAD requestFileSendFromClient, const bool flagDefaultPath, std::string pathForSave = ""); // �����, ������� ��������� � ��������� ���� �� �������
	
	
	
	
	bool checkAuthorizationClientAlreadyExists(const std::string & loginClient); // �����, ������� ��������� ���� ����� ������ ��� ��� ���������������� �� �������
	void removeLoginFromTheListOfAuthorizedemoveLogin(const std::string& loginClient); // ��� ���������� �� ������� ����� ������� ��������� �� ������ �������������������� ��������

	bool checkRequestConnetionFromClient(ProtocolAAD requestConnetion); // ���������� �������� ������� �� �����������
	bool checkRequestFileSendFromClient(ProtocolAAD requestFileSend); // ���������� �������� ������� �� �������� �����

	std::vector<std::string> authorized�lientsList = {}; // ������ ������ ������������������� �������� �� �������

	class ClientInfo { //����� ������ ����� �������, ���� �� ������� �� ��������������� � ������������� � id �������, � ����� ���� ������ ������ �������������� � �����
	public:
		SOCKET clientConnection;
		USHORT portClient;
		int clientId;
		std::string loginClient = "";
		void printIncomingConnectionClient();
		ClientInfo(SOCKET clientConnection, USHORT portClient, const int ClientId);
	};
	void disconnectClient(ClientInfo dataClient); // ��� ���������� ������� �� �������
	void handleClient(ClientInfo client); //��������� ������������� �������	

	//=========��������� ������ �������======
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
	void receiving�onnections(); // ��������� ���������� ��������
};