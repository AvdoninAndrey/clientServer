#include "server.h"

/*std::wstring Server::convertStrToWstr(const std::string& str) {
    int wchars_num = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    std::wstring resultWstr;
    resultWstr.resize(wchars_num); // или через конструктор wide(wchars_num, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &resultWstr[0], wchars_num);
    return resultWstr;
}*/

bool Server::checkRegistrationClientStr(const std::string& str) {
    return str.find('_') == std::string::npos ? true : false;
}

bool Server::authorizationClient(const std::string& loginClient, const std::string& passwdClient) {
    const std::string clientLogInData = loginClient + "_" + passwdClient;
    bool result = false;
    std::string buffer;
    std::ifstream fileLogIn;
    fileLogIn.open(this->fileAuthPath, std::ios_base::in);
    while (!fileLogIn.eof()) {
        getline(fileLogIn, buffer);
        if (buffer == clientLogInData) {
            result = true;
            break;
        }
    }
    fileLogIn.close();
    return result;
}


bool Server::registrationClient(const std::string& newLoginClient, const std::string& newPasswdClient) {
    std::string buffer;
    if (checkRegistrationClientStr(newLoginClient) && checkRegistrationClientStr(newPasswdClient)) {
        std::ifstream fileLogIn;
        fileLogIn.open(this->fileAuthPath, std::ios_base::in);
        while (!fileLogIn.eof()) {
            getline(fileLogIn, buffer);
            if (buffer.substr(0, buffer.find('_')) == newLoginClient) {
                fileLogIn.close();
                return false;
            }
        }
        fileLogIn.close();
        const std::string clientNewLogInData = newLoginClient + "_" +  newPasswdClient + "\n";
        std::ofstream addClientTofileLogIn;
        addClientTofileLogIn.open(this->fileAuthPath, std::ofstream::app);
        addClientTofileLogIn << clientNewLogInData;
        addClientTofileLogIn.close();
        return true;
    }
    return false;
}

bool Server::checkDirectoryEnteredClient(const std::string& enteredDirectory) {
    const std::filesystem::path pathDirectory(enteredDirectory);
    return std::filesystem::is_directory(pathDirectory.string());
}

std::string Server::getMessageFromClient(jsonDataFormat data) {
    return data.getMessage();
}

void Server::recvFile(SOCKET client, const std::string& enteredDirectory) {
    jsonDataFormat dataFile = recvDataText(client, "fileInfo");
    char* bytes = new char[dataFile.getFileSize()];
    int recvBytes = recv(client, bytes, dataFile.getFileSize(), 0);
    if (recvBytes == -1) {
        throw ExceptionNetwork(8, "failed to accept file");
    }
    

    std::fstream fileSave;
    fileSave.open(enteredDirectory + "\\" + dataFile.getFilename(), std::fstream::ios_base::out | std::fstream::ios_base::binary);
    fileSave.write(bytes, dataFile.getFileSize());
    std::cout << "The file with the name " << dataFile.getFilename() << " has been successfully saved on the server" << std::endl;
    fileSave.close();
    delete[] bytes;
}



void Server::disconnectClient(SOCKET clientConnection) {
    sendDataText(clientConnection, jsonDataFormat(headerError, disconnect));
    closesocket(clientConnection);
}

void Server::handleClient(SOCKET * clientConnection) {
    bool connect = true;
    while (connect) {
         sendDataText(*clientConnection, jsonDataFormat(headerSelect, textSelect));

         bool selectFlag = true; // используется как флаг
         while (selectFlag) {
             jsonDataFormat getData = recvDataText(*clientConnection); // выбор клиента по поводу авторизации, регистрации или отключения
             if (getMessageFromClient(getData) == "logIn") { //если клиент решил авторизоваться
                 sendDataText(*clientConnection, jsonDataFormat(headerAuthentication, textLogin)); // отправляем строку, что нужно вести логин
                 std::string enteredLoginClient = getMessageFromClient(recvDataText(*clientConnection)); // принимаем введённый логин
                
                 sendDataText(*clientConnection, jsonDataFormat(headerAuthentication, textPassword));// отправляем строку, что нужно вести пароль
                 std::string enteredPasswdClient = getMessageFromClient(recvDataText(*clientConnection));  // принимаем введённый пароль

                 if (authorizationClient(enteredLoginClient, enteredPasswdClient)) { // авторизация Тут подумать об асинхронности
                     std::cout << "The client with the login " << "\"" << enteredLoginClient << "\"" <<  " successfully logged in to the server" << std::endl;
                     sendDataText(*clientConnection, jsonDataFormat(headerAuthentication, flagResultTrue)); // если авторизация прошла успешно отправляем флаг, чтобы было правильно взаимодействие в клиенте
                     //sendDataText(*clientConnection, jsonDataFormat(headerAuthentication, successfulLogIn)); // отправляем сообщение, что авторизацая прошла успешно
                     bool directoryServer = true;
                     while (directoryServer) {
                         sendDataText(*clientConnection, jsonDataFormat(headerPathInServer, enteredDirectoryForClient)); // отправляем строку, что нужно указать каталог, в котором будет храниться файл
                         std::string enteredDirectory = getMessageFromClient(recvDataText(*clientConnection)); // получаем введённый каталог
                         enteredDirectory = "D:\\Programming";
                        if (checkDirectoryEnteredClient(enteredDirectory)) { // если данный каталог существует на сервере
                             sendDataText(*clientConnection, jsonDataFormat(headerFile, flagResultTrue)); // отправляем флаг результата
                             recvFile(*clientConnection, enteredDirectory); // принимаем файл
                             directoryServer = false;
                        }
                         else {
                             sendDataText(*clientConnection, jsonDataFormat(headerAuthentication, flagResultFalse));  // отправляем флаг, что авторизация не прошла
                             sendDataText(*clientConnection, jsonDataFormat(headerFile, enteredDirectoryFailed)); // отправляем сообщение, что авторизация не пройдена
                         }
                     }
                     selectFlag = false;
                 }
                 else {
                     sendDataText(*clientConnection, jsonDataFormat(headerAuthentication, flagResultFalse));
                     sendDataText(*clientConnection, jsonDataFormat(headerError, failedLogIn)); // отправляем, что авторизацая прошла неудачно
                 }
             }
             else if (getMessageFromClient(getData) == "register") { // если клиент решил зарегистрироваться
                 sendDataText(*clientConnection, jsonDataFormat(headerRegistration, textNewLogin));  // отправляем строку, что нужно придумать логин
                 std::string enteredLoginClient = getMessageFromClient(recvDataText(*clientConnection)); // принимаем (придуманный) введённый логин
                 sendDataText(*clientConnection, jsonDataFormat(headerRegistration, textNewPassword)); // отправляем строку, что нужно придумать пароль
                 std::string enteredPasswdClient = getMessageFromClient(recvDataText(*clientConnection));  // принимаем (придуманный) введённый пароль
                 if (registrationClient(enteredLoginClient, enteredPasswdClient)) { //регистрация
                     sendDataText(*clientConnection, jsonDataFormat(headerRegistration, successfulRegistration)); // если регистрация прошла удачно, отправляем соответствующие сообщение
                 }
                 else {
                     sendDataText(*clientConnection, jsonDataFormat(headerError, failedRegistration));  // если регистрация прошла неудачно, отправляем сообщение об ошибке
                 }
             }
             else if (getMessageFromClient(getData) == "disconnect") { // если клиент решил отключиться
                 selectFlag = false;
             }
             else { // если клиент указал несуществующую команду 
                 sendDataText(*clientConnection, jsonDataFormat(headerError, incorrectInput));
             }
         }
        connect = false;
    } 
    disconnectClient(*clientConnection);
}



void Server::receivingСonnections() {
    WSAData wsaData;
    WORD dllVersion = MAKEWORD(2, 1);

    if (WSAStartup(dllVersion, &wsaData) != 0) {
        throw ExceptionNetwork(1, "failed initialize WSADATA");
    }
    SOCKADDR_IN addrInfo;

    addrInfo.sin_family = AF_INET;
    addrInfo.sin_port = htons(this->port);
    InetPton(AF_INET, this->ip, &addrInfo.sin_addr.s_addr);
    int sizeAddrInfo = sizeof(addrInfo);

    SOCKET socketListenConnections = socket(AF_INET, SOCK_STREAM, 0);
    if (socketListenConnections == -1) {
        throw ExceptionNetwork(4, "failed to create a socket to listen for connections");
    }
 
    int bindResult = ::bind(socketListenConnections, reinterpret_cast<SOCKADDR*>(&addrInfo), sizeof(addrInfo));
    if (bindResult == -1) {
        throw ExceptionNetwork(5, "failed to bind");
    }


    int listenResult = listen(socketListenConnections, 1);
    if (listenResult == -1) {
        throw ExceptionNetwork(6, "failed to listen for incoming connections");
    }

    while (true) {
        SOCKET newClientConnection = accept(socketListenConnections, reinterpret_cast<SOCKADDR*>(&addrInfo), &sizeAddrInfo);
        std::cout << "Incoming connection: Port=" << addrInfo.sin_port <<  std::endl;
  

        if (newClientConnection == 0) {
            throw ExceptionNetwork(7, "connection could not be accepted");
        }
        else {
           // thread newClient([&]() {
                handleClient(&newClientConnection);
              //  });
           // newClient.detach();
        }
    }
}





