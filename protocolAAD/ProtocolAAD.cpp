#include "ProtocolAAD.h"

ProtocolAAD::ProtocolAAD(string &header, string &message) {
    size_t sizeHeader = header.length();
    size_t sizeMessage = message.length();

    for (size_t i = 0; i < header.length(); i++) {
        this->headArray[i] = header[i];
    }
    this->headArray[sizeHeader] = '\0';
    for (size_t i = 0; i < message.length(); i++) {
        this->messageArray[i] = message[i];
    }
    this->messageArray[sizeMessage] = '\0';
}


ProtocolAAD::ProtocolAAD(string& header, string& filename, int fileSize) {
    size_t sizeHeader = header.length();
    size_t sizeNameFile = filename.length();
    for (size_t i = 0; i < header.length(); i++) {
        this->headArray[i] = header[i];
    }
    this->headArray[sizeHeader] = '\0';
    for (size_t i = 0; i < filename.length(); i++) {
        this->filenameArray[i] = filename[i];
    }
    this->filenameArray[sizeNameFile] = '\0';

    this->sizeFileBytes = fileSize;
}

void ProtocolAAD::printMessage() {
    for (size_t i = 0; messageArray[i] != '\0'; i++) {
        cout << messageArray[i];
    }
    cout << endl;
}

string ProtocolAAD::getMessage() {
    string resultMessage = "";
    for (size_t i = 0; messageArray[i] != '\0'; i++) {
        resultMessage += messageArray[i];
    }
    return resultMessage;
}
string ProtocolAAD::getFilename() {
    string resultFilename = "";
    for (size_t i = 0; filenameArray[i] != '\0'; i++) {
        resultFilename += filenameArray[i];
    }
    return resultFilename;
}
int ProtocolAAD::getFileSize() {
    return this->sizeFileBytes;
}

//char* ProtocolAAD::getBytesFile() {
 //   return this->bytesFile;
//}
/**ProtocolAAD::ProtocolAAD(string header, int clientID) {
	this->header = header;
	this->clientID = clientID;
}*/
void ProtocolAAD::sendDataText(SOCKET clientConnection, ProtocolAAD data) {
    char* tmp = reinterpret_cast<char*>(&data);
    send(clientConnection, tmp, sizeof(ProtocolAAD), 0);
}

ProtocolAAD* ProtocolAAD::recvDataText(SOCKET clientConnection) {
    char buf[sizeof(ProtocolAAD)];
    recv(clientConnection, buf, sizeof(ProtocolAAD), 0);
    ProtocolAAD* getData = reinterpret_cast<ProtocolAAD*>(buf);
    return getData;
}
