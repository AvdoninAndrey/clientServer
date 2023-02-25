#include "client.h"

int main() {

	setlocale(LC_ALL, "Russian");
	std::string clientFilePath = "D:\\Programming\\C++\\clientServer\\client\\AvdoninAD_laba1.pdf";
	
	//std::wstring t = L"D:\\�����\\�����1����\\�������\\��.pdf";
	//string clientFilePath;
	//cout << "������� ���� �� �����, ������� �� ������ ��������� �� ������: ";
	bool selectFileFlag = true;
	while (selectFileFlag) {
		//cin >> clientFilePath;
		std::filesystem::path convertFilePath(clientFilePath);
		try {
			Client client(convertFilePath);
			std::cout << "To send a file, you need to connect to the server\n";
			std::cout << "Enter \"connect\" to connect or \"exit\" to exit the program: ";
			bool clientEnterFlag = true;
			while (clientEnterFlag) {
				std::string select;
				std::cin >> select;
				if (select == "connect") {
					client.connectToServer();
					clientEnterFlag = false;
				}
				else if (select == "exit") {
					clientEnterFlag = false;
				}
				else {
					std::cout << "������������ ����..." << std::endl;
				}
			}
			selectFileFlag = false;
		}
		catch (ExceptionNetwork& error) {
			std::cerr << "Code error: " << error.getCodeError() << std::endl;
			std::cerr << "Description error: " << error.getDescriptionError() << std::endl;
			selectFileFlag = false;
		}
		catch (ExceptionDirOrFile & error) {
			std::cerr << "Code error: " << error.getCodeError() << std::endl;
			std::cerr << "Description error: " << error.getDescriptionError() << std::endl;
			selectFileFlag = false;
		}
	}
    std::cout << "��������� ��������� ���� ������" << std::endl;
    return 0;
}