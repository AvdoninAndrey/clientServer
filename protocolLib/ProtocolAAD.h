
#pragma once
#include <string>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <nlohmann/json.hpp>
#include "..\Exception\ExceptionNetwork.h"

// Данный класс реализует протокол, который содержит возможные сообщения, headers и методы для отправки и приёма данных
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

	//============СЕТТЕРЫ===========
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
	const std::string headerConnection = "Connection"; // header, который используется при запросе на подключение клиента
	const std::string headerSelectOption = "SelectOperation"; // header, который используется при запросе операции
	const std::string headerAuthentication = "Authentication";  // header, который используется при запросе аутентификации
	const std::string headerRegistration = "Registration"; // header, который используется при запросе регистрации
	const std::string headerPathInServer = "DirectoryToSave"; // header, который используется при запросе каталога, где нужно сохранить файл
	const std::string headerFileSend = "FileSend"; // header, который когда используется при запросе на передачу файла
	
	const std::string headerResponsive = "Response";  // header, который используется для ответа сервера
	
	std::string getDateAndTimeNow();

	void sendData(SOCKET clientConnection, ProtocolAAD data); // метод, который отправляет данные
	ProtocolAAD recvData(SOCKET clientConnection); // метод, который принимает данные

public:

	ProtocolAAD() = default;
	//=====Конструктуры==========
	ProtocolAAD(const std::string& header, const int & clientID, const double & versionProtocol, const std::string & dateAndTimeConnection); // Запрос на подключение
	ProtocolAAD(const std::string& header, const int& clientID, const std::string& operation); // Запрос на операцию
	ProtocolAAD(const std::string& header, const int& clientID, const std::string& login, const std::string& password, const std::string& dateAndTimeAuthentication); // Запрос аутентификации
	ProtocolAAD(const std::string& header, const int& clientID, const std::string& newLogin, const std::string& newPassword); // Запрос регистрации
	ProtocolAAD(const std::string& header, const int& clientID, const bool isDefaultServerPath, const std::string& pathDirectory); // Запрос пути для сохранения 
	ProtocolAAD(const std::string& header, const int& clientID, const bool sendFile, const std::string& filename, const int & sizeFile, const std::string & bytesFormatBase64); // запрос на передачу файла

	ProtocolAAD(const std::string& header, const std::string & dateAndTimeResponse, const int& responceCode, const double& versionProtocol, const std::string& responseText); // ответ сервера

	//=====Геттеры==========
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
