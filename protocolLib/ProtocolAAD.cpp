#include "ProtocolAAD.h"

void ProtocolAAD::sendDataText(SOCKET clientConnection, jsonDataFormat data, std::string typeData) {
    std::string serialiazedStr = serialization(data, typeData);
    int countSendBytes = send(clientConnection, serialiazedStr.data(), serialiazedStr.length(), 0);
    if (countSendBytes == -1) {
        throw ExceptionNetwork(10, "an error occurred while sending the data");
    }
}

jsonDataFormat ProtocolAAD::recvDataText(SOCKET clientConnection, std::string typeData) {
    char buffer[4096] = {};
    int countRecvBytes = recv(clientConnection, buffer, 4096, 0);
    if (countRecvBytes == -1) {
        throw ExceptionNetwork(9, "an error occurred while receiving the data");
    }
    std::string serialiazedStr(buffer);
    serialiazedStr[countRecvBytes] = '\0';
    return deserialization(serialiazedStr, typeData);
}
