#include "client.h"

Client::Client(std::filesystem::path clientFilePath) {
    ExceptionDirOrFile check;

    if (check.checkingFileForExistence(clientFilePath)) {
        if (check.checkStrPath(clientFilePath.string())) {
            this->clientFilePath = clientFilePath;
        }
        else {
            throw ExceptionDirOrFile(12, "invalid characters are used in the names of paths");
        }
    }
    else {
        throw ExceptionDirOrFile(11, "The specified file does not exist");
    }
}

int Client::generationIdForClient() {
    int randIdClient = rand() % 3 + 1;

    while (checkIdClientAlreadyExists(randIdClient) != true) {
        randIdClient = rand() % 3 + 1;
    }
    std::ofstream fileWriteIdClients;
    fileWriteIdClients.open(this->PATH_FILE_ID_CLIENTS, std::ofstream::ios_base::app);
    fileWriteIdClients << std::to_string(randIdClient);
    fileWriteIdClients.close();

    return randIdClient;
}
bool Client::checkIdClientAlreadyExists(const int& idClient) {
    std::ifstream fileReadIdClients;
    fileReadIdClients.open(this->PATH_FILE_ID_CLIENTS, std::ifstream::ios_base::in);
    std::string buffer = "";
    bool result = true;
    char id;
    while (fileReadIdClients.get(id)) {
        buffer += id;
    }
    fileReadIdClients.close();

    for (size_t i = 0; i < buffer.size(); i++) {
        if (buffer[i] == atoi(std::to_string(idClient).c_str())) {
            result = false;
        }
    }
    return result;
}

void Client::removeClientIdFromFile(const int& idClient) {
    std::vector<int> allIdClients;
    std::string buffer = "";
    int indexElementRemove = 0;

    std::ifstream fileReadIdClients;
    fileReadIdClients.open(this->PATH_FILE_ID_CLIENTS, std::ifstream::in);
    char id;
    while (fileReadIdClients.get(id)) {
        buffer += id;
    }
    fileReadIdClients.close();

    for (size_t i = 0; i < buffer.size(); i++) {
        if (buffer[i] == atoi(std::to_string(idClient).c_str())) {
            indexElementRemove = i;
        }
    }

    buffer.erase(indexElementRemove, 1);
   
    std::ofstream fileWriteIdClients;
    fileWriteIdClients.open(this->PATH_FILE_ID_CLIENTS, std::ofstream::out);
    fileWriteIdClients << buffer;
    fileWriteIdClients.close();
}



void Client::printMessageFromResponseServer(ProtocolAAD data) {
    std::cout << data.getFieldsResponseText() << std::endl;
}


void Client::sendFileToServer() {
    std::string filename = this->clientFilePath.filename().string(); // имя файла
    const uintmax_t fileSizeBytes = std::filesystem::file_size(this->clientFilePath); // размер файла
   
    std::ifstream sendFile;
    sendFile.open(this->clientFilePath.string(), std::ifstream::ios_base::in | std::ifstream::ios_base::binary);
    char* bytesChar = new char[fileSizeBytes];
    sendFile.read(bytesChar, fileSizeBytes);

    std::string bytes = "";
    for (uintmax_t i = 0; i < fileSizeBytes; i++) {
        bytes += bytesChar[i];
    }

    sendData(this->clientSocketConnection, ProtocolAAD(headerFileSend, this->clientId, true, filename, fileSizeBytes, base64Encode(bytes)));
    delete[] bytesChar;
}


void Client::disconnect() {
    removeClientIdFromFile(this->clientId);
    closesocket(this->clientSocketConnection);
}

void Client::workByServer() {
   bool connect = true;
   int id = this->clientId;

   while (connect) {

       std::string select;
       std::cin >> select;
       ProtocolAAD requestSelectOpetation(headerSelectOption, id, select);
       sendData(this->clientSocketConnection, requestSelectOpetation); // отправляем запрос на какую-либо операцию

       if (requestSelectOpetation.getFieldsOperation() == "logIn") {
           printMessageFromResponseServer(recvData(this->clientSocketConnection)); // пришёл ответ "Введите логин и пароль"
           std::string login, password;
           std::cin >> login >> password;
           sendData(this->clientSocketConnection, ProtocolAAD(headerAuthentication, id, login, password, getDateAndTimeNow())); // отправляем запрос аутентификации
           ProtocolAAD responseServerRequestAuthentication = recvData(this->clientSocketConnection); // ответ сервера на запрос аутентификации
           printMessageFromResponseServer(responseServerRequestAuthentication); // выводим полученный ответ
           if (responseServerRequestAuthentication.getFieldsResponseCode() == 200) { // если аутентификация прошла успешно
               bool selectDirectoryToSave = true;
               while (selectDirectoryToSave) {
                   std::string selectDirectory;
                   std::cin >> selectDirectory;
                   if (selectDirectory == "default") {
                       sendData(this->clientSocketConnection, ProtocolAAD(headerPathInServer, id, true, "")); // отправляем запрос на выбор каталога (выбран стандартный каталог)
                       printMessageFromResponseServer(recvData(this->clientSocketConnection)); // выводим ответ "путь указан верно"
                       sendFileToServer(); //отправляем файл (здесь отправляется запрос на передачу)
                       ProtocolAAD responseServerRequestFileSend = recvData(this->clientSocketConnection);
                       printMessageFromResponseServer(responseServerRequestFileSend); // выводим полученный ответ
                       selectDirectoryToSave = false;
                   }
                   else {
                       sendData(this->clientSocketConnection, ProtocolAAD(headerPathInServer, id, false, selectDirectory)); // отправляем запрос на выбор каталога (выбран нестандартный каталог)
                       
                       ProtocolAAD responseServerRequestDirectoryToSave = recvData(this->clientSocketConnection); // получаем ответ "путь указан верно" или "путь указан неверно"
                       if (responseServerRequestDirectoryToSave.getFieldsResponseCode() == 200) {
                           printMessageFromResponseServer(responseServerRequestDirectoryToSave); // выводим ответ "путь указан верно"
                           sendFileToServer();   //отправляем файл (здесь отправляется запрос на передачу)
                           ProtocolAAD responseServerRequestFileSend = recvData(this->clientSocketConnection);
                           printMessageFromResponseServer(responseServerRequestFileSend); // выводим полученный ответ
                           selectDirectoryToSave = false;
                       }
                       else {
                            printMessageFromResponseServer(responseServerRequestDirectoryToSave);  // выводим ответ "путь указан неверно"
                       }
                   }
               }
               connect = false;
           }
       }
       else if (requestSelectOpetation.getFieldsOperation() == "register") {
           printMessageFromResponseServer(recvData(this->clientSocketConnection)); // пришёл ответ "Придумайте и введите логин и пароль"
           std::string newLogin, newPassword;
           std::cin >> newLogin >> newPassword;
           sendData(this->clientSocketConnection, ProtocolAAD(headerRegistration, id, newLogin, newPassword)); // отправляем запрос регистрации
           ProtocolAAD responseServerRequestRegistration = recvData(this->clientSocketConnection); // ответ сервера на запрос регистрации
           printMessageFromResponseServer(responseServerRequestRegistration); // выводим полученный ответ о результате регистрации
       }
       else if (requestSelectOpetation.getFieldsOperation() == "disconnect") {
           printMessageFromResponseServer(recvData(this->clientSocketConnection)); // пришёл ответ "Вы были отключены"
           connect = false;
       }
       else {
           printMessageFromResponseServer(recvData(this->clientSocketConnection));  // пришёл ответ "операции не существует"
       }
   }
   disconnect();
}

void Client::connectToServer() {
    WSAData wsaData;
    WORD dllVersion = MAKEWORD(2, 1);

    if (WSAStartup(dllVersion, &wsaData) != 0) {
        throw ExceptionNetwork(1, "failed initialize WSADATA");
    }

    SOCKADDR_IN addrInfo;
    addrInfo.sin_family = AF_INET;
    addrInfo.sin_port = htons(this->PORT);
    InetPton(AF_INET, this->IP, &addrInfo.sin_addr.s_addr);

    int sizeAddrInfo = sizeof(addrInfo);

    this->clientSocketConnection = socket(AF_INET, SOCK_STREAM, 0);
    if (this->clientSocketConnection == -1) {
        throw ExceptionNetwork(2, "failed to create socket");
    }

    int connectStatus = connect(this->clientSocketConnection, reinterpret_cast<SOCKADDR*>(&addrInfo), sizeAddrInfo);
    if (connectStatus != 0) {
        throw ExceptionNetwork(3, "failed to connect to the server"); // ошибка подключения
    }
    else {
        srand(time(NULL));
        this->clientId = generationIdForClient(); // генерируем id клиента

        sendData(this->clientSocketConnection, ProtocolAAD(headerConnection, clientId, USE_VERSION_PROTOCOL, getDateAndTimeNow())); // запрос на подключение
        ProtocolAAD response = recvData(this->clientSocketConnection); // ответ сервера
        if (response.getFieldsResponseCode() == 200) { // ответ: удачное подключение
            printMessageFromResponseServer(response); 
            workByServer();
        }
        else if (response.getFieldsResponseCode() == 400) { // ответ: сервер перегружен
            printMessageFromResponseServer(response);
        }
    }
}