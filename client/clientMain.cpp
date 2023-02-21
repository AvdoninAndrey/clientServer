#include "client.h"

int main() {
	setlocale(LC_ALL, "Russian");
	string clientFilePath = "D:\\Programming\\C++\\clientServer\\client\\AvdoninAD_laba1.pdf";
	//string clientFilePath;
	//cout << "Укажите путь до файла, который вы хотите отправить на сервер: ";
	//cin >> clientFilePath;
	Client client(clientFilePath);
	if (client.checkingFileForExistence()) {
		cout << "Файл сущетсвует" << endl;
		cout << "Чтобы отправить файл, необходимо подключиться к серверу\n";
		cout << "Введите \"connect\" для подключения или \"exit\", чтобы завершить работу программы: ";
		string select;
		cin >> select;
		select = "connect";
		if (select == "connect") {
			client.connectToServer();
		}
	}
	else {
		cout << "Файл не сущетсвует" << endl;
	}
	system("pause");
	return 0;
}