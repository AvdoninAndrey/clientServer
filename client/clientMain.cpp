#include "client.h"

int main() {

	setlocale(LC_ALL, "Russian");
	std::string clientFilePath = "D:\\Programming\\C++\\clientServer\\client\\AvdoninAD_laba1.pdf";
	
	//std::wstring t = L"D:\\Учёба\\Учёба1Курс\\Алгебра\\ФВ.pdf";
	//string clientFilePath;
	//cout << "Укажите путь до файла, который вы хотите отправить на сервер: ";
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
					std::cout << "Некорректный ввод..." << std::endl;
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
    std::cout << "Программа завершила свою работу" << std::endl;
    return 0;
}