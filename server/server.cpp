#include "server.h"

bool Server::checkRegistrationClientStr(const string& str) {
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '_') {
            return false;
        }
    }
    return true;
}

bool Server::authorizationClient(const string& loginClient, const string& passwdClient) {
    const string clientLogInData = loginClient + "_" + passwdClient;
    string buffer;
    ifstream fileLogIn;
    fileLogIn.open(this->fileAuthPath, ios_base::in);
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
    string buffer;
    if (checkRegistrationClientStr(newLoginClient) == true && checkRegistrationClientStr(newPasswdClient) == true) {

        ifstream fileLogIn;
        fileLogIn.open(this->fileAuthPath, ios_base::in);
        while (!fileLogIn.eof()) {
            getline(fileLogIn, buffer);
            if (buffer.substr(0, buffer.find('_')) == newLoginClient) {
                fileLogIn.close();
                return false;
            }
        }
        fileLogIn.close();

        const string clientNewLogInData = newLoginClient + "_" +  newPasswdClient;
        ofstream addClientTofileLogIn;
        addClientTofileLogIn.open(fileAuthPath, ofstream::app);
        addClientTofileLogIn << clientNewLogInData + "\n";
        addClientTofileLogIn.close();
        return true;
    }
    return false;
}

bool Server::checkDirectoryEnteredClient(const string& enteredDirectory) {
    const filesystem::path pathDirectory(enteredDirectory);
    return filesystem::is_directory(pathDirectory.string());
}

string Server::getMessageFromClient(ProtocolAAD* data) {
    return data->getMessage();
}

/*void Server::recvFileFromClient(SOCKET client, const string& enteredDirectory) {

    char filename[128];
    recv(client, filename, 128, 0); // получаем имя файла
    string getFilename(filename);

    char fileSizeStr[16];
    recv(client, fileSizeStr, 16, 0); // получаем размер файла
    int fileSize = atoi(fileSizeStr);

    char* bytes = new char[fileSize];
    fstream file;
    file.open(enteredDirectory + "\\" + getFilename, ios_base::out | ios_base::binary);
    recv(client, bytes, fileSize, 0);
    file.write(bytes, fileSize);
    cout << "Файл с названием " << getFilename << "успешно сохранён на сервере" << endl;
    delete[] bytes;
    file.close();
}*/


void Server::recvFile(SOCKET client, const string& enteredDirectory) {
    ProtocolAAD * dataFile = recvDataText(client);


    char* bytes = new char[dataFile->getFileSize()];
    recv(client, bytes, dataFile->getFileSize(), 0);

    fstream fileSave;
    fileSave.open(enteredDirectory + "\\" + dataFile->getFilename(), ios_base::out | ios_base::binary);
    fileSave.write(bytes, dataFile->getFileSize());
    cout << "Файл с названием " << dataFile->getFilename() << " успешно сохранён на сервере" << endl;
    fileSave.close();
    delete[] bytes;
}



void Server::handleClient(SOCKET * clientConnection) {
    bool connect = true;
    while (connect) {
        //const char  text = "dsadjgfklfjdgkl;fdjgkjf;dklg gfkdgjfdlkgj;dfgjkfdljhlh\nfdsfdsf\nfdsfdsf\nsad";
         sendDataText(*clientConnection, ProtocolAAD(headerSelect, textAuth));

         bool select = true; // используется как флаг
         while (select) {
             ProtocolAAD * getData = recvDataText(*clientConnection); // выбор клиента по поводу авторизации, регистрации или отключения

             if (getMessageFromClient(getData) == "logIn") { //если клиент решил авторизоваться
                 sendDataText(*clientConnection, ProtocolAAD(headerAuthentication, textLogin)); // отправляем строку, что нужно вести логин
                 string enteredLoginClient = getMessageFromClient(recvDataText(*clientConnection)); // принимаем введённый логин
                 sendDataText(*clientConnection, ProtocolAAD(headerAuthentication, textPassword));// отправляем строку, что нужно вести пароль
                 string enteredPasswdClient = getMessageFromClient(recvDataText(*clientConnection));  // принимаем введённый пароль
                 if (authorizationClient(enteredLoginClient, enteredPasswdClient)) { // авторизация
                     sendDataText(*clientConnection, ProtocolAAD(headerAuthentication, flagResultTrue)); // если авторизация прошла успешно отправляем флаг, чтобы было правильно взаимодействие в клиенте
                     sendDataText(*clientConnection, ProtocolAAD(headerAuthentication, successfulLogIn)); // отправляем сообщение, что авторизацая прошла успешно
                     bool directoryServer = true;
                     while (directoryServer) {
                         sendDataText(*clientConnection, ProtocolAAD(headerPathInServer, enteredDirectoryForClient)); // отправляем строку, что нужно указать каталог, в котором будет храниться файл
                         string enteredDirectory = getMessageFromClient(recvDataText(*clientConnection)); // получаем введённый каталог
                         enteredDirectory = "D:\\Programming\\C++\\clientServer\\server\\filesClients";
                        if (checkDirectoryEnteredClient(enteredDirectory)) { // если данный каталог существует на сервере
                             sendDataText(*clientConnection, ProtocolAAD(headerFile, flagResultTrue)); // отправляем флаг результата
                             //recvFileFromClient(*clientConnection, enteredDirectory); //принимаем файл, который будет передан клиентом
                             recvFile(*clientConnection, enteredDirectory);
                             directoryServer = false;
                        }
                         else {
                             sendDataText(*clientConnection, ProtocolAAD(headerAuthentication, flagResultFalse));  // отправляем флаг, что авторизация не прошла
                             sendDataText(*clientConnection, ProtocolAAD(headerFile, enteredDirectoryFailed)); // отправляем сообщение, что авторизация не пройдена
                         }
                     }
                 }
                 else {
                     sendDataText(*clientConnection, ProtocolAAD(headerAuthentication, flagResultFalse));
                     sendDataText(*clientConnection, ProtocolAAD(headerError, failedLogIn)); // отправляем, что авторизацая прошла неудачно
                 }

             }
             else if (getMessageFromClient(getData) == "register") { // если клиент решил зарегистрироваться
                 sendDataText(*clientConnection, ProtocolAAD(headerRegistration, textNewLogin));  // отправляем строку, что нужно придумать логин
                 string enteredLoginClient = getMessageFromClient(recvDataText(*clientConnection)); // принимаем (придуманный) введённый логин
                 sendDataText(*clientConnection, ProtocolAAD(headerRegistration, textNewPassword)); // отправляем строку, что нужно придумать пароль
                 string enteredPasswdClient = getMessageFromClient(recvDataText(*clientConnection));  // принимаем (придуманный) введённый пароль
                 if (registrationClient(enteredLoginClient, enteredPasswdClient)) { //регистрация
                     sendDataText(*clientConnection, ProtocolAAD(headerRegistration, successfulRegistration)); // если регистрация прошла удачно, отправляем соответствующие сообщение
                 }
                 else {
                     sendDataText(*clientConnection, ProtocolAAD(headerError, failedRegistration));  // если регистрация прошла неудачно, отправляем сообщение об ошибке
                 }
             }
             else if (getMessageFromClient(getData) == "disconnect") { // если клиент решил отключиться
                 string disconnect = "Вы были отключены от сервара";
                 sendDataText(*clientConnection, ProtocolAAD(headerError, disconnect));
                 select = false;
             }
             else { // если клиент указал несуществующую команду 
                 sendDataText(*clientConnection, ProtocolAAD(headerError, incorrectInput));
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





