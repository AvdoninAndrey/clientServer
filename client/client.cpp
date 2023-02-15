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


void Client::printMessageFromServer(ProtocolAAD data) {
    cout << data.message << endl;
}


void Client::sendFileToServer() {
    ifstream sendFile;
    sendFile.open(this->clientFilePath, ios_base::in | ios_base::binary);
    int fileSizeBytes = filesystem::file_size(this->clientFilePath)  + 1;
    char* bytes = new char[fileSizeBytes];
    sendFile.read(bytes, fileSizeBytes);

    send(this->clientSocketConnection, to_string(fileSizeBytes).c_str(), 16, 0);
    send(this->clientSocketConnection, bytes, fileSizeBytes, 0);
    sendFile.close();
}



void Client::workByServer() {
    //sendDataTextToServer(&(this->clientSocketConnection), ProtocolAAD("connect", 1));
    bool connect = true;
    
    DataExchange dataExchange;

    while (connect) {
        ProtocolAAD  getData = dataExchange.recvData(this->clientSocketConnection); // получаем данные о том, какие команды нужно использовать
        //cout << getData.message << endl;
        printMessageFromServer(getData); // печатаем полученные данные о командах

     
        
        bool authorization = true;
        while (authorization) {

            string selectClient;
            cin >> selectClient;

            dataExchange.sendData(this->clientSocketConnection, ProtocolAAD("auth", selectClient));  // отправляем введённую команду серверу

            //В зависимости от того, что выбрал клиент, будет различное взаимодействие с сервером
            if (selectClient == "logIn") { // если клиент ввёл команду для авторизации
                printMessageFromServer(dataExchange.recvData(this->clientSocketConnection)); // получаем строку, что нужно вести логин
                string clientLogin;
                cin >> clientLogin;
                dataExchange.sendData(this->clientSocketConnection, ProtocolAAD("auth", clientLogin)); // отправляем логин на сервер

                printMessageFromServer(dataExchange.recvData(this->clientSocketConnection)); // получаем строку, что нужно вести пароль
                string clientPasswd;
                cin >> clientPasswd;
                dataExchange.sendData(this->clientSocketConnection, ProtocolAAD("auth", clientPasswd));  // отправляем пароль на сервер

                printMessageFromServer(dataExchange.recvData(this->clientSocketConnection)); // получаем результат авторизации


                printMessageFromServer(dataExchange.recvData(this->clientSocketConnection)); // получаем строку, что нужно указать путь, где будет храниться файл на сервере
                string filePathInServer;
                cin >> filePathInServer;

                dataExchange.sendData(this->clientSocketConnection, ProtocolAAD("pathFileServer", filePathInServer)); // отправляем указнный путь на сервер

                //printMessageFromServer(dataExchange.recvData(this->clientSocketConnection)); // тут будет получена строка что

                //sendFileToServer(); // если никаких ошибок не возникает, то отправляем файл на сервер.
                cout << "Отправляем файл" << endl;

            }
            else if (selectClient == "register") { // если клиент ввёл команду для регистрации
                printMessageFromServer(dataExchange.recvData(this->clientSocketConnection)); // получаем строку, что нужно придумать логин
                string clientLogin;
                cin >> clientLogin;
                dataExchange.sendData(this->clientSocketConnection, ProtocolAAD("auth", clientLogin)); // отправляем логин на сервер

                printMessageFromServer(dataExchange.recvData(this->clientSocketConnection)); // получаем строку, что нужно придумать пароль
                string clientPasswd;
                cin >> clientPasswd;
                dataExchange.sendData(this->clientSocketConnection, ProtocolAAD("auth", clientPasswd));  // отправляем пароль на сервер

                printMessageFromServer(dataExchange.recvData(this->clientSocketConnection)); // получаем результат о регистрации
            }

            else if (selectClient == "disconnect") { // если клиент ввёл команду для отключения

            }
            else { // если клиент ввёл некорректную команду
                printMessageFromServer(dataExchange.recvData(this->clientSocketConnection)); // принимаем строку что нужно попробовать вести команду ещё раз
            }
        }
  
    }
}

void Client::connectToServer() {
    WSAData wsaData;
    WORD dllVersion = MAKEWORD(2, 1);

    if (WSAStartup(dllVersion, &wsaData) != 0) {
        exceptionNetworkConnections(1, "не удалось иницилизировать WSAData.");
    }

    SOCKADDR_IN addrInfo;

    addrInfo.sin_family = AF_INET;
    addrInfo.sin_port = htons(this->port);
    InetPton(AF_INET, this->ip, &addrInfo.sin_addr.s_addr);

    int sizeAddrInfo = sizeof(addrInfo);


    this->clientSocketConnection = socket(AF_INET, SOCK_STREAM, 0);
    int connectStatus = connect(this->clientSocketConnection, reinterpret_cast<SOCKADDR*>(&addrInfo), sizeAddrInfo);

    if (connectStatus != 0) {
        exceptionNetworkConnections(2, "не удалось подключиться к серверу.");
    }
    else {
        workByServer();
    }
    
}