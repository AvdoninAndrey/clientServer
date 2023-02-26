#include "server.h"

Server::ClientInfo::ClientInfo(SOCKET clientConnection, USHORT portClient) {
    this->clientConnection = clientConnection;
    this->portClient = portClient;
}
void Server::ClientInfo::printIncomingConnectionClient() {
    std::cout << "Incoming connection: Port=" << this->portClient << std::endl;
}

SOCKET Server::ClientInfo::getSocketClient() {
    return this->clientConnection;
}
		
USHORT Server::ClientInfo::getPortClient() {
    return this->portClient;
}

bool Server::checkRegistrationClientStr(const std::string& str) {
    return str.find('_') == std::string::npos ? true : false;
}

bool Server::authorizationClient(const std::string& loginClient, const std::string& passwdClient) {
    bool result = false;
    if (checkAuthorizationClientAlreadyExists(loginClient)) {
        const std::string clientLogInData = loginClient + "_" + passwdClient;
        std::string buffer;
        std::ifstream fileLogIn;
        fileLogIn.open(this->fileAuthPath, std::ios_base::in);
        while (!fileLogIn.eof()) {
            getline(fileLogIn, buffer);
            if (buffer == clientLogInData) {
                this->authorizedСlientsList.push_back(loginClient);
                result = true;
                break;
            }
        }
        fileLogIn.close();
        return result;
    }
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
    ExceptionDirOrFile checkAbsolute;
    if (checkAbsolute.isAbsolutePath(enteredDirectory)) {
        return std::filesystem::is_directory(pathDirectory.string());
    }
    return false;
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
    std::cout << "The file with the name \"" << dataFile.getFilename() << "\" has been successfully saved on the server" << std::endl;
    fileSave.close();
    delete[] bytes;
}

bool Server::checkAuthorizationClientAlreadyExists(const std::string& loginClient) {
    for (size_t i = 0; i < this->authorizedСlientsList.size(); i++) {
        if (authorizedСlientsList.at(i) == loginClient) {
            return false;
        }
    }
    return true;
}

void Server::removeLoginFromTheListOfAuthorizedemoveLogin(const std::string& loginClient) {
    int indexLoginRemove = 0;
    for (size_t i = 0; i < this->authorizedСlientsList.size(); i++) {
        if (authorizedСlientsList.at(i) == loginClient) {
            indexLoginRemove = i;
        }
    }
    auto iter = this->authorizedСlientsList.cbegin();
    this->authorizedСlientsList.erase(iter + indexLoginRemove);
}

void Server::disconnectClient(SOCKET clientConnection, USHORT portClient, const std::string &loginClient) {
    std::cout << "The client with the login \"" << loginClient <<"\" has been disabled" << std::endl;
    std::cout << "The port " << portClient << " was released" << std::endl;
    removeLoginFromTheListOfAuthorizedemoveLogin(loginClient);
    sendDataText(clientConnection, jsonDataFormat(headerError, disconnect));
    closesocket(clientConnection);
    this->currentConnectionClient -= 1;
}

void Server::handleClient(SOCKET clientConnection, USHORT portClient) {
    bool connect = true;
    std::string login;
    while (connect) {
         sendDataText(clientConnection, jsonDataFormat(headerSelect, textSelect));

         bool selectFlag = true; // используется как флаг
         while (selectFlag) {
             jsonDataFormat getData = recvDataText(clientConnection); // выбор клиента по поводу авторизации, регистрации или отключения
             if (getMessageFromClient(getData) == "logIn") { //если клиент решил авторизоваться
                 sendDataText(clientConnection, jsonDataFormat(headerAuthentication, textLogin)); // отправляем строку, что нужно вести логин
                 std::string enteredLoginClient = getMessageFromClient(recvDataText(clientConnection)); // принимаем введённый логин
                
                 sendDataText(clientConnection, jsonDataFormat(headerAuthentication, textPassword));// отправляем строку, что нужно вести пароль
                 std::string enteredPasswdClient = getMessageFromClient(recvDataText(clientConnection));  // принимаем введённый пароль

                 if (authorizationClient(enteredLoginClient, enteredPasswdClient)) { // авторизация Тут подумать об асинхронности
                     std::cout << "The client with the login " << "\"" << enteredLoginClient << "\"" <<  " successfully logged in to the server and is located on the port " << portClient  << std::endl;
                     login = enteredLoginClient;
                     sendDataText(clientConnection, jsonDataFormat(headerAuthentication, flagResultTrue)); // если авторизация прошла успешно отправляем флаг, чтобы было правильно взаимодействие в клиенте
                     bool directoryServer = true;
                     while (directoryServer) {
                         sendDataText(clientConnection, jsonDataFormat(headerPathInServer, enteredDirectoryForClient)); // отправляем строку, что нужно указать каталог, в котором будет храниться файл
                         std::string enteredDirectory = getMessageFromClient(recvDataText(clientConnection)); // получаем введённый каталог
                         enteredDirectory = "D:\\Programming";
                       
                        if (checkDirectoryEnteredClient(enteredDirectory)) { // если данный каталог существует на сервере
                             sendDataText(clientConnection, jsonDataFormat(headerFile, flagResultTrue)); // отправляем флаг результата
                             recvFile(clientConnection, enteredDirectory); // принимаем файл
                             directoryServer = false;
                        }
                         else {
                             sendDataText(clientConnection, jsonDataFormat(headerAuthentication, flagResultFalse));  // отправляем флаг, что путь не существует
                             sendDataText(clientConnection, jsonDataFormat(headerFile, enteredDirectoryFailed)); // отправляем сообщение, что путь указан не верно
                         }
                     }
                     selectFlag = false;
                 }
                 else {
                     sendDataText(clientConnection, jsonDataFormat(headerAuthentication, flagResultFalse));
                     sendDataText(clientConnection, jsonDataFormat(headerError, failedLogIn)); // отправляем, что авторизацая прошла неудачно
                 }
             }
             else if (getMessageFromClient(getData) == "register") { // если клиент решил зарегистрироваться
                 sendDataText(clientConnection, jsonDataFormat(headerRegistration, textNewLogin));  // отправляем строку, что нужно придумать логин
                 std::string enteredLoginClient = getMessageFromClient(recvDataText(clientConnection)); // принимаем (придуманный) введённый логин
                 sendDataText(clientConnection, jsonDataFormat(headerRegistration, textNewPassword)); // отправляем строку, что нужно придумать пароль
                 std::string enteredPasswdClient = getMessageFromClient(recvDataText(clientConnection));  // принимаем (придуманный) введённый пароль
                 if (registrationClient(enteredLoginClient, enteredPasswdClient)) { //регистрация
                     sendDataText(clientConnection, jsonDataFormat(headerRegistration, successfulRegistration)); // если регистрация прошла удачно, отправляем соответствующие сообщение
                 }
                 else {
                     sendDataText(clientConnection, jsonDataFormat(headerError, failedRegistration));  // если регистрация прошла неудачно, отправляем сообщение об ошибке
                 }
             }
             else if (getMessageFromClient(getData) == "disconnect") { // если клиент решил отключиться
                 selectFlag = false;
             }
             else { // если клиент указал несуществующую команду 
                 sendDataText(clientConnection, jsonDataFormat(headerError, incorrectInput));
             }
         }
        connect = false;
    } 
    disconnectClient(clientConnection, portClient, login);
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

   
    //this->vectorClientsInfo.resize(this->maxClientConnection);

    while (listen(socketListenConnections, this->maxClientConnection) != SOCKET_ERROR) {

        SOCKET newClientConnection = accept(socketListenConnections, reinterpret_cast<SOCKADDR*>(&addrInfo), &sizeAddrInfo);
        if (newClientConnection == 0) {
            throw ExceptionNetwork(7, "connection could not be accepted");
        }
        else {
            
            ClientInfo newClientInfo = ClientInfo(newClientConnection, addrInfo.sin_port);
           
            if (this->currentConnectionClient < this->maxClientConnection) {
                std::string serverNotOverloaded = "true";
                send(newClientInfo.getSocketClient(), serverNotOverloaded.c_str(), serverNotOverloaded.length(), 0);
             
                newClientInfo.printIncomingConnectionClient();
                std::thread newClient([&]() {
                    handleClient(newClientInfo.getSocketClient(), newClientInfo.getPortClient());
                    });
                newClient.detach();
                this->currentConnectionClient++;
            }
           else {
              std::string serverOverloaded = "The server is overloaded, try again later";
              send(newClientInfo.getSocketClient(), serverOverloaded.c_str(), serverOverloaded.length(), 0);
           }
        }
    }
}





