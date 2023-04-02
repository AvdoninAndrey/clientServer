#include "server.h"

Server::ClientInfo::ClientInfo(SOCKET clientConnection, USHORT portClient, const int clientId) {
    this->clientConnection = clientConnection;
    this->portClient = portClient;
    this->clientId = clientId;
}
void Server::ClientInfo::printIncomingConnectionClient() {
    std::cout << "Incoming connection: Port=" << this->portClient << "\tIdClient=" << this->clientId << std::endl;
}

bool Server::checkRegistrationClientStr(const std::string& str) {
    return  str.find('_') == std::string::npos ? true : false;
}

bool Server::authorizationClient(const std::string& loginClient, const std::string& passwdClient) {
    bool result = false;
    if (checkAuthorizationClientAlreadyExists(loginClient)) {
        const std::string clientLogInData = loginClient + "_" + passwdClient;
        std::string buffer;
        std::ifstream fileLogIn;
        fileLogIn.open(this->FILE_AUTH_PATH, std::ios_base::in);
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
        fileLogIn.open(this->FILE_AUTH_PATH, std::ios_base::in);
        while (!fileLogIn.eof()) {
            getline(fileLogIn, buffer);
            if (buffer.substr(0, buffer.find('_')) == newLoginClient) {
                fileLogIn.close();
                return false;
            }
        }
        fileLogIn.close();
        const std::string clientNewLogInData = newLoginClient + "_" + newPasswdClient + "\n";
        std::ofstream addClientTofileLogIn;
        addClientTofileLogIn.open(this->FILE_AUTH_PATH, std::ofstream::app);
        addClientTofileLogIn << clientNewLogInData;
        addClientTofileLogIn.close();
        return true;
    }
    return false;
}

bool Server::checkDirectoryEnteredClient(const std::string& enteredDirectory) {
    std::filesystem::path pathDirectory(enteredDirectory);
    if (!pathDirectory.is_absolute()) {
        pathDirectory = std::filesystem::absolute(pathDirectory);
    }
    return std::filesystem::is_directory(pathDirectory);
}

void Server::recvFile(ProtocolAAD requestFileSendFromClient, const bool flagDefaultPath, std::string pathForSave) {
     const uintmax_t sizeFile = requestFileSendFromClient.getFieldsSizeFile();

     char* bytesChar = new char[sizeFile];
       
    std::string bytesBase64decode = base64Decode(requestFileSendFromClient.getFieldsBytesFormatBase64());
     
   for (int i = 0; i < sizeFile; i++) {
         bytesChar[i] = bytesBase64decode[i];
   }

   std::fstream fileSave;
   if (flagDefaultPath) {
       fileSave.open(DEFAULT_DIRECTORY_FOR_SAVE_FILES + "\\" + requestFileSendFromClient.getFieldsFilename(), std::fstream::ios_base::out | std::fstream::ios_base::binary);
   }
   else {
       fileSave.open(pathForSave + "\\" + requestFileSendFromClient.getFieldsFilename(), std::fstream::ios_base::out | std::fstream::ios_base::binary);
   }

   fileSave.write(bytesChar, requestFileSendFromClient.getFieldsSizeFile());
   std::cout << "The file with the name \"" << requestFileSendFromClient.getFieldsFilename() << "\" has been successfully saved on the server" << std::endl;
   fileSave.close();
   delete[] bytesChar;
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
        if (authorizedСlientsList[i] == loginClient) {
            indexLoginRemove = i;
        }
    }
    auto iter = this->authorizedСlientsList.cbegin();
    this->authorizedСlientsList.erase(iter + indexLoginRemove);
}

void Server::disconnectClient(ClientInfo dataClient) {
    if (dataClient.loginClient == "") {
        std::cout << "The client with the id \"" << dataClient.clientId << "\" has been disabled" << std::endl;
    }
    else {
        std::cout << "The client with the login \"" << dataClient.loginClient << "\" has been disabled" << std::endl;
        removeLoginFromTheListOfAuthorizedemoveLogin(dataClient.loginClient);
    }
    closesocket(dataClient.clientConnection);
    this->currentConnectionClient -= 1;
}

void Server::handleClient(ClientInfo client) {
    bool connect = true;
    SOCKET socketClient = client.clientConnection;
    const int id = client.clientId;

    while (connect) {
        ProtocolAAD requestSelectOpetationFromClient = recvData(socketClient); // принимаем запрос на операцию
        if (requestSelectOpetationFromClient.getFieldsOperation() == this->OPERATIONS_SERVER.at("OPERATION_AUTHENTICATION")) { // пришёл запрос на операцию аутентификации
            sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, USE_VERSION_PROTOCOL, responseSuccessfulOperationAuthentication)); // отправляем ответ "Введите логин и пароль"
            ProtocolAAD requestAuthenticationFromClient = recvData(socketClient);  // пришёл запрос аутентификации
            if (authorizationClient(requestAuthenticationFromClient.getFieldsLogin(), requestAuthenticationFromClient.getFieldsPassword())) { // производится проверка данных от учётной записи
               
                sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, USE_VERSION_PROTOCOL, responseSuccessfulAuthentication)); // отправляем ответ "удачная аутентификация"
                client.loginClient = requestAuthenticationFromClient.getFieldsLogin();
                std::cout << "The client with the ID \"" << id << "\" has authenticated under the login \"" << client.loginClient << "\"" << std::endl;
                bool selectDirectoryFile = true;
                while (selectDirectoryFile) {
                    ProtocolAAD requestDirectoryToSaveFromClient = recvData(socketClient); // принимаем запрос на выбор каталога для сохранения
                    bool flagDefaultPath = true;
                    if (requestDirectoryToSaveFromClient.getFieldsIsDefaultServerPath()) {
                        sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, USE_VERSION_PROTOCOL, responseSuccessfulSelectPath)); // отправляем ответ "путь указан верно"
                        ProtocolAAD requestFileSendFromClient = recvData(socketClient); // принимаем запрос на передачу файла
                        if (checkRequestFileSendFromClient(requestFileSendFromClient)) { // проверям запрос на передачу файла
                            recvFile(requestFileSendFromClient, flagDefaultPath); // принимаем файл
                            sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, USE_VERSION_PROTOCOL, responseSuccessfulFileSend)); // отправляем ответ "файл успешно сохранён"
                            selectDirectoryFile = false;
                        }
                        else {
                            sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 400, USE_VERSION_PROTOCOL, responseFailedFileSend)); // отправляем ответ "запрос неправильный, файл не будет принят"
                            selectDirectoryFile = false;
                        }
                    }
                    else {
                        std::string enteredPathClient = requestDirectoryToSaveFromClient.getFieldsPathDirectory(); // получаем указанный клиентом каталог

                        if (checkDirectoryEnteredClient(enteredPathClient)) { // проверяем каталог на корректность
                            flagDefaultPath = false;
                            sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, USE_VERSION_PROTOCOL, responseSuccessfulSelectPath)); // отправляем ответ "путь указан верно"
                            ProtocolAAD requestFileSendFromClient = recvData(socketClient); // принимаем запрос на передачу файла
                            if (checkRequestFileSendFromClient(requestFileSendFromClient)) {
                                recvFile(requestFileSendFromClient, flagDefaultPath, enteredPathClient); // тут будем принимать файл
                                sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, USE_VERSION_PROTOCOL, responseSuccessfulFileSend)); // отправляем ответ "файл успешно сохранён"
                                selectDirectoryFile = false;
                               
                            }
                            else {
                                sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 400, USE_VERSION_PROTOCOL, responseFailedFileSend)); // отправляем ответ "запрос неправильный, файл не будет принят"
                                selectDirectoryFile = false;
                            }                                                                                                                                            
                        }
                        else {
                            sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 400, USE_VERSION_PROTOCOL, responseFailedSelectPath)); // отправляем ответ "путь указан неверно"
                        }
                    }
                }
                connect = false;
            }
            else {
                sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 400, USE_VERSION_PROTOCOL, responseFailedAuthentication)); // отправляем ответ "данные неверные"
            }

        }
        else if (requestSelectOpetationFromClient.getFieldsOperation() == this->OPERATIONS_SERVER.at("OPERATION_REGISTRATION")) { // пришёл запрос на операцию регистрации
            sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, this->USE_VERSION_PROTOCOL, responseSuccessfulOperationRegistration)); // отправляем ответ "Придумайте логин и пароль"
            ProtocolAAD requestRegistrationFromClient = recvData(socketClient);  // пришёл запрос регистрации
            if (registrationClient(requestRegistrationFromClient.getFieldsNewLogin(), requestRegistrationFromClient.getFieldsNewPassword())) {
                sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, USE_VERSION_PROTOCOL, responseSuccessfulRegistration)); // отправляем ответ "регистрация пройдена"
            }
            else {
                sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, USE_VERSION_PROTOCOL, responseFailedRegistration)); // отправляем ответ "регистрация  не пройдена"
            }
        }
        else if (requestSelectOpetationFromClient.getFieldsOperation() == this->OPERATIONS_SERVER.at("DISCONNECT")) { // пришёл запрос на операцию отключения
            sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, USE_VERSION_PROTOCOL, responseSuccessfulOperationDisconnect)); // отправляем ответ "вы были отключены"
            connect = false;
        }
        else { // пришёл запрос на не существующую операцию
            sendData(socketClient, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 400, USE_VERSION_PROTOCOL, responseFailedOperation)); // отправляем ответ "указанной операции не существует"
        }
    }
    disconnectClient(client); // отключаем клиента
}


bool Server::checkRequestConnetionFromClient(ProtocolAAD requestConnetion) {
    if (requestConnetion.getFieldsHeader() == headerConnection) {
        if (requestConnetion.getFieldsVersionProtocol() == this->USE_VERSION_PROTOCOL) {
            if (requestConnetion.getFieldsClientID() >= 1 && requestConnetion.getFieldsClientID() <= 9) {
                if (requestConnetion.getFieldsDateAndTimeConnection() != "") {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Server::checkRequestFileSendFromClient(ProtocolAAD requestFileSend) {
    if (requestFileSend.getFieldsClientID()) {
        if (requestFileSend.getFieldsFileSend()) {
            if (requestFileSend.getFieldsFilename() != "") {
                if (requestFileSend.getFieldsSizeFile() != 0) {
                    if (requestFileSend.getFieldsBytesFormatBase64() != "") {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


void Server::receivingСonnections() {
    WSAData wsaData;
    WORD dllVersion = MAKEWORD(2, 1);

    if (WSAStartup(dllVersion, &wsaData) != 0) {
        throw ExceptionNetwork(1, "failed initialize WSADATA");
    }
    SOCKADDR_IN addrInfo;

    addrInfo.sin_family = AF_INET;
    addrInfo.sin_port = htons(PORT);
    InetPton(AF_INET, IP, &addrInfo.sin_addr.s_addr);
    int sizeAddrInfo = sizeof(addrInfo);

    SOCKET socketListenConnections = socket(AF_INET, SOCK_STREAM, 0);
    if (socketListenConnections == -1) {
        throw ExceptionNetwork(4, "failed to create a socket to listen for connections");
    }
 
    int bindResult = ::bind(socketListenConnections, reinterpret_cast<SOCKADDR*>(&addrInfo), sizeof(addrInfo));
    if (bindResult == -1) {
        throw ExceptionNetwork(5, "failed to bind");
    }

   
    while (listen(socketListenConnections, MAX_CLIENT_CONNECTIONS) != SOCKET_ERROR) {

        SOCKET newClientConnection = accept(socketListenConnections, reinterpret_cast<SOCKADDR*>(&addrInfo), &sizeAddrInfo);
        if (newClientConnection == 0) {
            throw ExceptionNetwork(6, "connection could not be accepted");
        }
        else {
            ProtocolAAD requestConnetionFromClient = recvData(newClientConnection); // принимаем запрос на подключение
            if (checkRequestConnetionFromClient(requestConnetionFromClient)) { // проверяем запрос на правильность
                if (currentConnectionClient < MAX_CLIENT_CONNECTIONS) { // проверяем загруженность сервера
                    ClientInfo newClientInfo(newClientConnection, addrInfo.sin_port, requestConnetionFromClient.getFieldsClientID()); // создаём объект класса, который хранит информацию о подключившемся клиенте
                    
                    sendData(newClientInfo.clientConnection, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 200, this->USE_VERSION_PROTOCOL, responseSuccessfulConnection)); // отправляем ответ "удачное подключение + команды"

                    newClientInfo.printIncomingConnectionClient(); // выводим информацию о подключившемся клиенте

                    std::thread newClient([&]() {
                        handleClient(newClientInfo); // обрабатываем клиента в отдельном потоке
                        });

                    newClient.detach();
                    this->currentConnectionClient++; // увеличиваем счётчик подключившихся клиентов
                }
                else {
                    sendData(newClientConnection, ProtocolAAD(headerResponsive, getDateAndTimeNow(), 400, this->USE_VERSION_PROTOCOL, responseServerOverloaded)); // если сервер перегружен, отправляем ответ "сервер перегружен"
                    closesocket(newClientConnection);
                }
            }
        }
    }
}





