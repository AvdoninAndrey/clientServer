
#pragma once
#include <string>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include "jsonDataFormat.h"
#include "SerializationDeserialization.h"
#include "..\Exception\ExceptionNetwork.h"

// Данный класс реализует протокол, который содержит возможные сообщения, headers и методы для отправки и приёма данных
class ProtocolAAD : public SerializationDeserialization {
private:
protected:
	std::string headerAuthentication = "Authentication";  // header, который используется при авторизации
	std::string headerFile = "File"; // header, который когда отправляются принимаются и отправляются данные о файле
	std::string headerRegistration = "Registration"; // header, который используется при регистрации
	std::string headerError = "Error";  // header, который используется когда происходят ошибки (например неудачная регистрация или авторизации и т.д)
	std::string headerSelect = "Select"; // header, который используется когда клиент выбирает авторизацию, регистрацию или отключение
	std::string headerPathInServer = "pathFileInServer"; // header, который используется указании каталога, где нужно сохранить файл
	
	std::string textSelect = "To send a file to the server, you need to log in or register:\n"
		"to log in, enter(logIn), to register, enter(register), to disconnect, enter (disconnect)";

	std::string textLogin = "Enter your username:";
	std::string textPassword = "Enter your password: ";

	std::string textNewLogin = "Come up with a login: ";
	std::string textNewPassword = "Come up with a password: ";

	//std::string successfulLogIn = "Authorization was successful!";
	std::string failedLogIn = "Incorrect username or password, try again";

	std::string successfulRegistration = "Registration was successful! You can log in to the server.";
	std::string failedRegistration = "A user with this username already exists or you used an invalid character (_)";

	std::string enteredDirectoryForClient = "Specify the directory where the file will be stored:";
	std::string enteredDirectoryFailed = "There is no such directory on the server";
	

	std::string incorrectInput = "You have entered a non-existent command, try again";

	std::string disconnect = "You have been disconnected from the server";

	std::string flagResultTrue = "true";
	std::string flagResultFalse = "false";
	void sendDataText(SOCKET clientConnection, jsonDataFormat data, std::string typeData = "text"); // метод, который отправляет данные
	jsonDataFormat recvDataText(SOCKET clientConnection, std::string typeData = "text"); // метод, который принимает данные

public:
	ProtocolAAD() = default;
};
