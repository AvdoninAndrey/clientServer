#include "client.h"

int main() {
	setlocale(LC_ALL, "Russian");
	string clientFilePath = "C:\\Programming\\�++\\clientServer\\client\\text.txt";
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