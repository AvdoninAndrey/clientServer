
#pragma once
#include <string>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <nlohmann/json.hpp>
#include "..\Exception\ExceptionNetwork.h"

// ������ ����� ��������� ��������, ������� �������� ��������� ���������, headers � ������ ��� �������� � ����� ������
class ProtocolAAD {
private:
	class ProtocolAADFields {
	public:
		std::string header = "";
		std::string dateAndTimeConnection = "", dateAndTimeAuthentication = "", dateAndTimeResponse = "";


		double versionProtocol = 0.0;
		int clientID = 0;

		std::string operation = "";

		std::string login = "";
		std::string password = "";

		std::string newLogin = "";
		std::string newPassword = "";

		bool isDefaultServerPath = false;
		std::string pathDirectory = "";

		bool sendFile = false;
		std::string filename = "";
		uintmax_t sizeFile = 0;

		std::string bytesFormatBase64 = "";

		int responseCode = 0;
		std::string responseText = "";
	};
	ProtocolAADFields fieldsRequestOrResponsive;

	class SerializationDeserializationFormatJSON {
	public:
		std::string serializationData(ProtocolAAD data);
		ProtocolAAD deserializationData(const std::string& serializationStr);
	};

	//============�������===========
	void setFieldsHeader(const std::string& header);
	void setFieldsDateAndTimeConnection(const std::string& dateAndTimeConnection);
	void setFieldsClientID(const int& clientID);
	void setFieldsVersionProtocol(const double& versionProtocol);
	void setFieldsDateAndTimeAuthentication(const std::string& dateAndTimeAuthentication);
	void setFieldsDateAndTimeResponse(const std::string& dateAndTimeResponse);
	void setFieldsResponseText(const std::string& responseText);
	void setFieldsResponseCode(const int &  responseCode);
	void setFieldsOperation(const std::string& operation);
	void setFieldsLogin(const std::string& login);
	void setFieldsPassword(const std::string& password);
	void setFieldsNewLogin(const std::string& newLogin);
	void setFieldsNewPassword(const std::string& newPassword);
	void setFieldsIsDefaultServerPath(const bool isDefaultServerPath);
	void setFieldsPathDirectory(const std::string& pathDirectory);
	void setFieldsFilename(const std::string & filename);
	void setFieldsSizeFile(const uintmax_t& sizeFile);
	void setFieldsFileSend(const bool fileSend);
	void setFieldsBytesFormatBase64(const std::string& bytesFormatBase64);
protected:
	//=====Headers========
	const std::string headerConnection = "Connection"; // header, ������� ������������ ��� ������� �� ����������� �������
	const std::string headerSelectOption = "SelectOperation"; // header, ������� ������������ ��� ������� ��������
	const std::string headerAuthentication = "Authentication";  // header, ������� ������������ ��� ������� ��������������
	const std::string headerRegistration = "Registration"; // header, ������� ������������ ��� ������� �����������
	const std::string headerPathInServer = "DirectoryToSave"; // header, ������� ������������ ��� ������� ��������, ��� ����� ��������� ����
	const std::string headerFileSend = "FileSend"; // header, ������� ����� ������������ ��� ������� �� �������� �����
	
	const std::string headerResponsive = "Response";  // header, ������� ������������ ��� ������ �������
	
	std::string getDateAndTimeNow();

	void sendData(SOCKET clientConnection, ProtocolAAD data); // �����, ������� ���������� ������
	ProtocolAAD recvData(SOCKET clientConnection); // �����, ������� ��������� ������

public:

	ProtocolAAD() = default;
	//=====������������==========
	ProtocolAAD(const std::string& header, const int & clientID, const double & versionProtocol, const std::string & dateAndTimeConnection); // ������ �� �����������
	ProtocolAAD(const std::string& header, const int& clientID, const std::string& operation); // ������ �� ��������
	ProtocolAAD(const std::string& header, const int& clientID, const std::string& login, const std::string& password, const std::string& dateAndTimeAuthentication); // ������ ��������������
	ProtocolAAD(const std::string& header, const int& clientID, const std::string& newLogin, const std::string& newPassword); // ������ �����������
	ProtocolAAD(const std::string& header, const int& clientID, const bool isDefaultServerPath, const std::string& pathDirectory); // ������ ���� ��� ���������� 
	ProtocolAAD(const std::string& header, const int& clientID, const bool sendFile, const std::string& filename, const int & sizeFile, const std::string & bytesFormatBase64); // ������ �� �������� �����

	ProtocolAAD(const std::string& header, const std::string & dateAndTimeResponse, const int& responceCode, const double& versionProtocol, const std::string& responseText); // ����� �������

	//=====�������==========
	std::string getFieldsHeader();
	int getFieldsClientID();
	double getFieldsVersionProtocol();
	std::string getFieldsDateAndTimeConnection();
	std::string getFieldsDateAndTimeAuthentication();
	std::string getFieldsDateAndTimeResponse();
	std::string getFieldsResponseText();
	int getFieldsResponseCode();
	std::string getFieldsOperation();
	std::string getFieldsLogin();
	std::string getFieldsPassword();
	std::string getFieldsNewLogin();
	std::string getFieldsNewPassword();
	bool getFieldsIsDefaultServerPath();
	std::string getFieldsPathDirectory();
	std::string getFieldsFilename();
	uintmax_t  getFieldsSizeFile();
	bool getFieldsFileSend();
	std::string getFieldsBytesFormatBase64();
};
