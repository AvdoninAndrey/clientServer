#include "client.h"

int main() {
	std::string clientFilePath;
	bool selectFileFlag = true;
	Client inputAlphabet;
	std::cout << "When working with the server, use the following alphabet to enter:\n" << inputAlphabet.getCorectAphabetInputClient() << std::endl;
	while (selectFileFlag) {

		std::cout << "Specify the path to the file you want to want to the server: ";
		std::cin >> clientFilePath;
		std::filesystem::path convertFilePath(clientFilePath);

		if (!convertFilePath.is_absolute()) {
			convertFilePath = std::filesystem::absolute(convertFilePath);
		}

		try {
			Client client(convertFilePath);
			client.connectToServer();
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
	std::cout << "The program-client has finished its work" << std::endl;
	Sleep(2000);
    return 0;
}