#include "client.h"

Client::Client(string& clientFilePath) {
    this->clientFilePath = clientFilePath;
}

bool Client::checkingFileForExistence() {
    return filesystem::exists(this->clientFilePath);
}

void Client::exceptionNetworkConnections(const int& codeError, const string& descriptionError) {
    cout << "Код ошибки: " << codeError << endl;
    cout << "Описание ошибки: " << descriptionError << endl;
}

void Client::printMessageFromServer(ProtocolAAD * data) {
   data->printMessage();
}

/*void Client::sendFileToServer() {
    auto filename = filesystem::path(this->clientFilePath).filename();
    string filenameStr = filename.string();
    send(this->clientSocketConnection, reinterpret_cast<char*>(&filenameStr), 128, 0);


    ifstream sendFile;
    sendFile.open(this->clientFilePath, ios_base::in | ios_base::binary);
    int fileSizeBytes = filesystem::file_size(this->clientFilePath) + 1;
    char* bytes = new char[fileSizeBytes];
    sendFile.read(bytes, fileSizeBytes);

    send(this->clientSocketConnection, to_string(fileSizeBytes).c_str(), 16, 0);
    send(this->clientSocketConnection, bytes, fileSizeBytes, 0);
    sendFile.close();
    delete[] bytes;
}*/

ProtocolAAD Client::generatingDataFileToSend() {
    auto filename = filesystem::path(this->clientFilePath).filename();
    string filenameStr = filename.string(); // имя файла
    int fileSizeBytes = filesystem::file_size(this->clientFilePath); // Размер файла
    return ProtocolAAD(headerFile, filenameStr, fileSizeBytes);
}

void Client::sendFileToServer(ProtocolAAD dataFile) {
    //char* tmp = reinterpret_cast<char*>(&dataFile);
    //send(this->clientSocketConnection, tmp, sizeof(ProtocolAAD), 0); // отправляем данный о файле
    sendDataText(this->clientSocketConnection, dataFile);

    ifstream sendFile;
    sendFile.open(this->clientFilePath, ios_base::in | ios_base::binary);
    int fileSizeBytes = dataFile.getFileSize();

    char* bytes = new char[fileSizeBytes];
    sendFile.read(bytes, fileSizeBytes);

    send(this->clientSocketConnection, bytes, fileSizeBytes, 0); // отправляем байты
}


void Client::workByServer() {
   bool connect = true;
   while (connect) {
        ProtocolAAD * getData = recvDataText(this->clientSocketConnection); // получаем данные о том, какие команды нужно использовать
        printMessageFromServer(getData); // печатаем полученные данные о командах

       bool select = true; // используется как флаг
        while (select) {

            string selectClient;
            cin >> selectClient;

            sendDataText(this->clientSocketConnection, ProtocolAAD(headerSelect, selectClient));  // отправляем введённую команду серверу

            //В зависимости от того, что выбрал клиент, будет различное взаимодействие с сервером
            if (selectClient == "logIn") { // если клиент ввёл команду для авторизации
                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно вести логин
                string clientLogin;
                cin >> clientLogin;
                sendDataText(this->clientSocketConnection, ProtocolAAD(this->headerAuthentication, clientLogin)); // отправляем логин на сервер

                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно вести пароль
                string clientPasswd;
                cin >> clientPasswd;
                sendDataText(this->clientSocketConnection, ProtocolAAD(this->headerAuthentication, clientPasswd));  // отправляем пароль на сервер
                
                getData = recvDataText(this->clientSocketConnection); // получаем результат (флаг) авторизации

                if (getData->getMessage() == "true") { // если авторизация выполнена успешно
                    printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем сообщение об успешной авторизации
                    printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно указать путь, где будет храниться файл на сервере
                    bool directoryServer = true;
                    while (directoryServer) {
                        string filePathInServer;
                        cin >> filePathInServer; // указываем путь на сервере, где должен сохраниться файл
                        sendDataText(this->clientSocketConnection, ProtocolAAD(this->headerPathInServer, filePathInServer)); // отправляем указанный путь на сервер
                        getData = recvDataText(this->clientSocketConnection); // тут получаем флаг допустимый ли путь был указан
                        if (getData->getMessage() == "true") { // если путь указан верно
                             //sendFileToServer(); // отправляем файл на сервер
                            //sendDataText(this->clientSocketConnection, generatingDataFileToSend());
                            sendFileToServer(generatingDataFileToSend());
                            cout << "Файл отправлен" << endl;
                            directoryServer = false;
                        }
                        else {
                            printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что указанный путь недопустим
                        }
                    }
                }
                else {
                    printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку что авторизация не пройдена
                }
            }
            else if (selectClient == "register") { // если клиент ввёл команду для регистрации
                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно придумать логин
                string clientLogin;
                cin >> clientLogin;
                sendDataText(this->clientSocketConnection, ProtocolAAD(this->headerRegistration, clientLogin)); // отправляем логин на сервер

                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что нужно придумать пароль
                string clientPasswd;
                cin >> clientPasswd;
                sendDataText(this->clientSocketConnection, ProtocolAAD(this->headerRegistration, clientPasswd));  // отправляем пароль на сервер

                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем результат о регистрации
            }
            else if (selectClient == "disconnect") { // если клиент ввёл команду для отключения
                printMessageFromServer(recvDataText(this->clientSocketConnection)); // получаем строку, что клиент отключён
                select = false;
            }
            else { // если клиент ввёл некорректную команду
               printMessageFromServer(recvDataText(this->clientSocketConnection)); // принимаем строку что нужно попробовать вести команду ещё раз
            }
        }
        connect = false;
    }
   closesocket(this->clientSocketConnection);
}

void Client::connectToServer() {
    WSAData wsaData;
    WORD dllVersion = MAKEWORD(2, 1);

    if (WSAStartup(dllVersion, &wsaData) != 0) {
        exceptionNetworkConnections(1, "не удалось иницилизировать WSAData");
    }

    SOCKADDR_IN addrInfo;

    addrInfo.sin_family = AF_INET;
    addrInfo.sin_port = htons(this->port);
    InetPton(AF_INET, this->ip, &addrInfo.sin_addr.s_addr);

    int sizeAddrInfo = sizeof(addrInfo);


    this->clientSocketConnection = socket(AF_INET, SOCK_STREAM, 0);
    int connectStatus = connect(this->clientSocketConnection, reinterpret_cast<SOCKADDR*>(&addrInfo), sizeAddrInfo);

    if (connectStatus != 0) {
        exceptionNetworkConnections(2, "не удалось подключиться к серверу");
    }
    else {
        workByServer();
        //closesocket(this->clientSocketConnection);
    }
}