#include "server.h"

/*char* Server::createBuffer(const string& text, const int& bufferDataSize) {
    //const int textSize = text.size();
    char* buffer = new char[bufferDataSize + 1];
    for (int i = 0; i < bufferDataSize; i++) {
        buffer[i] = text[i];
    }
    buffer[bufferDataSize] = '\0';
    return buffer;
}

void Server::sendDataTextClient(SOCKET* clientSocket, const string & transmittedData) {
    const int bufferSize = transmittedData.size();

    char* bufferData = createBuffer(textAuth, bufferSize);
    send(*clientSocket, bufferData, bufferSize + 1, 0);
    delete[] bufferData;
}

void Server::recvData(SOCKET* clientSocket) {
    char* buffer[256];
    recv(*clientSocket, *buffer, 256, 0);
    cout << buffer;
}*/

bool Server::authorizationClient(const string& loginClient, const string& passwdClient) {
    const string clientLogInData = loginClient + passwdClient;
    string buffer;
    ifstream fileLogIn;
    fileLogIn.open(this->fileAuthPath);
    while (!fileLogIn.eof()) {
        getline(fileLogIn, buffer);
        if (buffer == clientLogInData) {
            fileLogIn.close();
            return true;
        }
    }
    fileLogIn.close();
    return false;
}




bool Server::registrationClient(const string& newLoginClient, const string& newPasswdClient) {
    const string clientNewLogInData = newLoginClient + newPasswdClient;
    string buffer;
    ofstream addClientTofileLogIn;

    addClientTofileLogIn.open(this->fileAuthPath, ofstream::app);
    addClientTofileLogIn << clientNewLogInData + "\n";
    addClientTofileLogIn.close();
    return true;
}

bool Server::checkFirectoryEnteredClient(const string& enteredDirectory) {
    return filesystem::is_directory(enteredDirectory);
}

string Server::getMessageFromClient(ProtocolAAD data) {
    return data.message;
}


void Server::handleClient(SOCKET* clientConnection) {
    bool connect = true;
    while (connect) {

        Server message;
        DataExchange dataExchange;
        // ProtocolAAD* getData = recvDataTextFromClient(*clientConnection);
        dataExchange.sendData(*clientConnection, ProtocolAAD("auth", message.textAuth));

        bool authorization = true; // используется как флаг
        while (authorization) {
            ProtocolAAD getData = dataExchange.recvData(*clientConnection); // выбор клиента по поводу авторизации, регистрации или отключения


            if (getMessageFromClient(getData) == "logIn") { //если клиент решил авторизоваться
                dataExchange.sendData(*clientConnection, ProtocolAAD("auth", message.textLogin));
                string enteredLoginClient = getMessageFromClient(dataExchange.recvData(*clientConnection)); // принимаем введённый логин
                dataExchange.sendData(*clientConnection, ProtocolAAD("auth", message.textPassword));
                string enteredPasswdClient = getMessageFromClient(dataExchange.recvData(*clientConnection));  // принимаем введённый пароль
                if (authorizationClient(enteredLoginClient, enteredPasswdClient)) { // авторизация
                    string successfulLogIn = "Авторизация прошла успешна!";
                    dataExchange.sendData(*clientConnection, ProtocolAAD("auth", successfulLogIn)); // отправляем, что авторизацая прошла успешно

                   bool directoryServer = true;
                    while (directoryServer) {
                        dataExchange.sendData(*clientConnection, ProtocolAAD("file", "Укажите каталог, в котором будет храниться файл:")); // отправляем строку, что нужно указать каталог, в котором будет храниться файл
                        string enteredDirectory = getMessageFromClient(dataExchange.recvData(*clientConnection)); // получаем введённый каталог
                        enteredDirectory = "..\\filesClients";
                        if (checkFirectoryEnteredClient(enteredDirectory)) {
                             //если каталог сущетсвует
                            //принимаем файл, который будет передан клиентом
                            directoryServer = false;
                        }
                        else {
                            const string enteredDirectoryFailed = "Такого каталога на сервере не сущетсвует";
                            dataExchange.sendData(*clientConnection, ProtocolAAD("file", enteredDirectoryFailed));
                        }
                    }
                }
                else {
                    string failedLogIn = "Неправильный логин или пароль, попробуйте ещё раз.";
                    dataExchange.sendData(*clientConnection, ProtocolAAD("auth", failedLogIn)); // отправляем, что авторизацая прошла неудачно
                }

            }
            else if (getMessageFromClient(getData) == "register") { // если клиент решил зарегистрироваться
                dataExchange.sendData(*clientConnection, ProtocolAAD("auth", message.textNewLogin));
                string enteredLoginClient = getMessageFromClient(dataExchange.recvData(*clientConnection)); // принимаем (придуманный) введённый логин
                dataExchange.sendData(*clientConnection, ProtocolAAD("auth", message.textNewPassword));
                string enteredPasswdClient = getMessageFromClient(dataExchange.recvData(*clientConnection));  // принимаем (придуманный) введённый пароль
                if (registrationClient(enteredLoginClient, enteredPasswdClient)) { // авторизация
                    string successfulRegistration = "Регистрация прошла успешна! Можете авторизоваться на сервере.";
                    dataExchange.sendData(*clientConnection, ProtocolAAD("auth", successfulRegistration));
                }
                else {
                    string failedRegistration = "Что-то пошло не так, попробуйте ещё раз.";
                    dataExchange.sendData(*clientConnection, ProtocolAAD("auth", failedRegistration));
                }
            }
            else if (getMessageFromClient(getData) == "disconnect") { // если клиент решил отключиться
                authorization = false;
            }
            else {
                string incorrectInput = "Вы ввели несуществующую команду, попробуйте ещё раз";
                dataExchange.sendData(*clientConnection, ProtocolAAD("auth", incorrectInput));
            }
        }
        connect = false;
    }
    closesocket(*clientConnection);
}
    


void Server::receivingСonnections() {
    WSAData wsaData;
    WORD dllVersion = MAKEWORD(2, 1);

    if (WSAStartup(dllVersion, &wsaData) != 0) {
        cout << "Error" << endl;
    }
    SOCKADDR_IN addrInfo;

    addrInfo.sin_family = AF_INET;
    addrInfo.sin_port = htons(this->port);
    InetPton(AF_INET, this->ip, &addrInfo.sin_addr.s_addr);

    int sizeAddrInfo = sizeof(addrInfo);

    SOCKET socketListenConnections = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketListenConnections, (SOCKADDR*)&addrInfo, sizeof(addrInfo));

    listen(socketListenConnections, 2);

    while (true) {
        // возвращает указатель на новый сокет
        SOCKET newClientConnection = accept(socketListenConnections, reinterpret_cast<SOCKADDR*>(&addrInfo), &sizeAddrInfo);
        if (newClientConnection == 0) {
            cout << "Error" << endl;
        }
        else {
           // thread newClient([&]() {
                handleClient(&newClientConnection);
             //   });
            //newClient.detach();
        }
    }    
}





void recvFile(SOCKET* socket) {

    char fileSizeStr[16];
   // char filePath[128];

    recv(*socket, fileSizeStr, 16, 0);
    int fileSize = atoi(fileSizeStr);
    char* bytes = new char[fileSize];



    fstream file;
    file.open("test.txt", ios_base::out | ios_base::binary);
    if (file.is_open()) {
        recv(*socket, bytes, fileSize, 0);
        file.write(bytes, fileSize);
        cout << "Saved" << endl;
    }
    delete[] bytes;
    file.close();
}


