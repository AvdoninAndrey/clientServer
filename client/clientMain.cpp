#include "client.h"

int main() {

	setlocale(LC_ALL, "Russian");
	std::string clientFilePath;
	bool selectFileFlag = true;
	while (selectFileFlag) {
		std::cout << "Specify the absolute (full) path to the file you want to send to the server:";
		std::cin >> clientFilePath;
		clientFilePath = "D:\\Programming\\JScourse\\Summer2022\\jsTrening\\1.js";
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
					std::cout << "you have entered a non-existent command, try again" << std::endl;
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
			std::cout << "Try to specify the path again" << std::endl;
		}
	}
	Sleep(2000);
    std::cout << "The program-client has finished its work" << std::endl;
    return 0;
}