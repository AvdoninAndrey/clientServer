#include "dataExchange.h"

void DataExchange::sendData(SOCKET clientConnection, ProtocolAAD data) {
    const char* header = data.header.c_str();
    const char* messageText = data.message.c_str();
    send(clientConnection, header, 64, 0);
    send(clientConnection, messageText, 1024, 0);
}
ProtocolAAD DataExchange::recvData(SOCKET clientConnection) {
    char header[64];
    char messageText[1024];

    recv(clientConnection, reinterpret_cast<char*>(&header), 64, 0);
    recv(clientConnection, reinterpret_cast<char*>(&messageText), 1024, 0);
    string headerStr(header), messageStr(messageText);


    ProtocolAAD getData(headerStr, messageStr);
    return getData;
}
