#include "client.h"

int main() {
	setlocale(LC_ALL, "Russian");
	string clientFilePath = "D:\\Programming\\C++\\clientServer\\client\\AvdoninAD_laba1.pdf";
	//string clientFilePath;
	//cout << "������� ���� �� �����, ������� �� ������ ��������� �� ������: ";
	//cin >> clientFilePath;
	Client client(clientFilePath);
	if (client.checkingFileForExistence()) {
		cout << "���� ����������" << endl;
		cout << "����� ��������� ����, ���������� ������������ � �������\n";
		cout << "������� \"connect\" ��� ����������� ��� \"exit\", ����� ��������� ������ ���������: ";
		string select;
		cin >> select;
		select = "connect";
		if (select == "connect") {
			client.connectToServer();
		}
	}
	else {
		cout << "���� �� ����������" << endl;
	}
	system("pause");
	return 0;
}