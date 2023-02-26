#include "client.h"

Client::Client(std::filesystem::path clientFilePath) {
    ExceptionDirOrFile check;
    if (check.checkingFileForExistence(clientFilePath)) {
        if (check.checkStrPath(clientFilePath.string())) {
            if (check.isAbsolutePath(clientFilePath)) {
                this->clientFilePath = clientFilePath;
            }
            else {
                throw ExceptionDirOrFile(13, "you specified a relative path instead of an absolute one");
            }
        }
        else {
            throw ExceptionDirOrFile(12, "invalid characters are used in the names of paths");
        }
    }
    else {
        throw ExceptionDirOrFile(11, "The specified file does not exist");
    }
}

void Client::printMessageFromServer(jsonDataFormat data) {
    std::cout << data.getMessage() << std::endl;
}

jsonDataFormat Client::generatingDataFileToSend() {
    auto filename = this->clientFilePath.filename();
    std::string filenameStr = filename.string(); // имя файла
    const unsigned int fileSizeBytes = std::filesystem::file_size(this->clientFilePath) + 1; // размер файла
    return jsonDataFormat(headerFile, filenameStr, fileSizeBytes);
}

void Client::sendFileToServer(jsonDataFormat dataFile) {
    sendDataText(this->clientSocketConnection, dataFile, "fileInfo");

    std::ifstream sendFile;
    sendFile.open(this->clientFilePath.string(), std::ifstream::ios_base::in | std::ifstream::ios_base::binary);
   
    int fileSizeBytes = dataFile.getFileSize();

    char* bytes = new char[fileSizeBytes];
    sendFile.read(bytes, fileSizeBytes);
    send(this->clientSocketConnection, bytes, fileSizeBytes, 0); // отправляем байты
    std::cout << "The file has been successfully sent to the server" << std::endl;
    delete[] bytes;
}


void Client::disconnect(SOCKET connect) {
    printMessageFromServer(recvDataText(this->clientSocketConnection));
    closesocket(connect);
}

void Client::workByServer() {
   bool connect = true;
   
   while (connect) {
        jsonDataFormat getData = recvDataText(this->clientSocketConnection); // получаем данные о том, какие команды нужно использовать
        printMessageFromServer(getData); // печатаем полученные данные о командах
        bool selectFlag = true; // используется как флаг
        while (selectFlag) {
            std::string selectClient;
            std::cin >> selectClient;
            sendDataText(this->clientSocketConnection, jsonDataFormat(headerSelect, selectClient));  // отправляем введённую команду серверу

            //В зависимости от того, что выбрал клиент, будет различное взаимодействие с сервером
            if (selectClient == "logIn") { // если клиент ввёл команду для авторизации
                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно вести логин
                std::string clientLogin;
                std::cin >> clientLogin;
                sendDataText(this->clientSocketConnection, jsonDataFormat(headerAuthentication, clientLogin)); // отправляем логин на сервер

                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно вести пароль
                std::string clientPasswd;
                std::cin >> clientPasswd;
                sendDataText(this->clientSocketConnection, jsonDataFormat(headerAuthentication, clientPasswd));  // отправляем пароль на сервер
                
                getData = recvDataText(this->clientSocketConnection); // получаем флаг авторизации
                
                if (getData.getMessage() == "true") { // если авторизация выполнена успешно
                   // printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем сообщение об успешной авторизации
                    printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно указать путь, где будет храниться файл на сервере
                    bool directoryServer = true;
                    while (directoryServer) {
                        std::string filePathInServer;
                        std::cin >> filePathInServer; // указываем путь на сервере, где должен сохраниться файл
                        sendDataText(this->clientSocketConnection, jsonDataFormat(headerPathInServer, filePathInServer)); // отправляем указанный путь на сервер
                        getData = recvDataText(this->clientSocketConnection); // тут получаем флаг допустимый ли путь был указан
                        if (getData.getMessage() == "true") { // если путь указан верно
                            sendFileToServer(generatingDataFileToSend()); 
                            directoryServer = false;
                        }
                        else {
                            printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что указанный путь недопустим
                        }
                    }
                    selectFlag = false;
                }
               else {
                   printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку что авторизация не пройдена
               }
            }
            else if (selectClient == "register") { // если клиент ввёл команду для регистрации
                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно придумать логин
                std::string clientLogin;
                std::cin >> clientLogin;
                sendDataText(this->clientSocketConnection, jsonDataFormat(headerRegistration, clientLogin)); // отправляем логин на сервер

                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно придумать пароль
                std::string clientPasswd;
                std::cin >> clientPasswd;
                sendDataText(this->clientSocketConnection, jsonDataFormat(headerRegistration, clientPasswd));  // отправляем пароль на сервер

                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем результат о регистрации
            }
            else if (selectClient == "disconnect") { // если клиент ввёл команду для отключения
                selectFlag = false;
            }
            else { // если клиент ввёл некорректную команду
               printMessageFromServer(recvDataText(this->clientSocketConnection)); // принимаем строку что нужно попробовать вести команду ещё раз
            }
        }
        connect = false;
    }
   disconnect(this->clientSocketConnection);
}

std::string Client::initialServerResponse() {
    char buffer[256];
    int coutRecvBytes = recv(this->clientSocketConnection, buffer, 256, 0);
    buffer[coutRecvBytes] = '\0';
    std::string initialResponse(buffer);
    return initialResponse;
}


void Client::connectToServer() {
    WSAData wsaData;
    WORD dllVersion = MAKEWORD(2, 1);

    if (WSAStartup(dllVersion, &wsaData) != 0) {
        throw ExceptionNetwork(1, "failed initialize WSADATA");
    }

    SOCKADDR_IN addrInfo;
    addrInfo.sin_family = AF_INET;
    addrInfo.sin_port = htons(port);
    InetPton(AF_INET, ip, &addrInfo.sin_addr.s_addr);

    int sizeAddrInfo = sizeof(addrInfo);

    this->clientSocketConnection = socket(AF_INET, SOCK_STREAM, 0);
    if (this->clientSocketConnection == -1) {
        throw ExceptionNetwork(2, "failed to create socket");
    }

    int connectStatus = connect(this->clientSocketConnection, reinterpret_cast<SOCKADDR*>(&addrInfo), sizeAddrInfo);
    if (connectStatus != 0) {
        throw ExceptionNetwork(3, "failed to connect to the server");
    }
    else {
        std::string initialResponse = initialServerResponse();
        if (initialResponse == "true") {
            workByServer();
        }
        else {
            std::cout << initialResponse << std::endl;
        } 
    }
}