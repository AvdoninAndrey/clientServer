
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
	string textAuth = "Для отправки файла на сервер необходимо авторизоваться или зарегестрироваться\n"
		"Для авторизации введите \"logIn\"\n"
		"Для регистрации введите \"register\"\n"
		"Для отключения введите \"disconnect\"";

	string textLogin = "Введите логин: ";
	string textPassword = "Введите пароль: ";

	string textNewLogin = "Придумайте логин: ";
	string textNewPassword = "Придумайте пароль: ";

	string successfulLogIn = "Авторизация прошла успешна!";
	string failedLogIn = "Неправильный логин или пароль, попробуйте ещё раз.";

	string enteredDirectoryFailed = "Такого каталога на сервере не сущетсвует";
	string enteredDirectoryForClient = "Укажите каталог, в котором будет храниться файл: ";

	string successfulRegistration = "Регистрация прошла успешна! Можете авторизоваться на сервере.";
	string failedRegistration = "Пользователь с таким логином уже существует или вы использованили недопустимый символ \"_\"";

	string incorrectInput = "Вы ввели несуществующую команду, попробуйте ещё раз";

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
