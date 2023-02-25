#include "server.h"

int main() {
	setlocale(LC_ALL, "Russian");
	Server server;
	try {
		server.receiving—onnections();
	}
	catch (ExceptionNetwork& error) {
		std::cerr << "Code error: " << error.getCodeError() << std::endl;
		std::cerr << "Description error: " << error.getDescriptionError() << std::endl;
	}
	
	return 0;
}