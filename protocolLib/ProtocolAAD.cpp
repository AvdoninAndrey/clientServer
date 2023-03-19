#include "ProtocolAAD.h"

std::string ProtocolAAD::SerializationDeserializationFormatJSON::serializationData(ProtocolAAD data) {
    nlohmann::json jsonObject{};
    
    jsonObject["header"] = data.getFieldsHeader();


    if (data.getFieldsHeader() == data.headerConnection) {
        jsonObject["clienId"] = data.getFieldsClientID();
        jsonObject["versionProtocol"] = data.getFieldsVersionProtocol();
        jsonObject["dateAndTimeConnection"] = data.getFieldsDateAndTimeConnection();
    }
    else if (data.getFieldsHeader() == data.headerSelectOption) {
        jsonObject["clienId"] = data.getFieldsClientID();
        jsonObject["operation"] = data.getFieldsOperation();
    }
    else if (data.getFieldsHeader() == data.headerAuthentication) {
        jsonObject["clienId"] = data.getFieldsClientID();
        jsonObject["login"] = data.getFieldsLogin();
        jsonObject["password"] = data.getFieldsPassword();
        jsonObject["dateAndTimeAuthentication"] = data.getFieldsDateAndTimeAuthentication();
    }
    else if (data.getFieldsHeader() == data.headerRegistration) {
        jsonObject["clienId"] = data.getFieldsClientID();
        jsonObject["newLogin"] = data.getFieldsNewLogin();
        jsonObject["newPassword"] = data.getFieldsNewPassword();
    }
    else if (data.getFieldsHeader() == data.headerPathInServer) {
        jsonObject["isDefaultServerPath"] = data.getFieldsIsDefaultServerPath();
        jsonObject["clienId"] = data.getFieldsClientID();
        jsonObject["pathDirectory"] = data.getFieldsPathDirectory();
    }
    else if (data.getFieldsHeader() == data.headerFileSend) {
        jsonObject["clienId"] = data.getFieldsClientID();
        jsonObject["sendFile"] = data.getFieldsFileSend();
        jsonObject["filename"] = data.getFieldsFilename();
        jsonObject["sizeFile"] = data.getFieldsSizeFile();
        jsonObject["bytesFormatBase64"] = data.getFieldsBytesFormatBase64();
    }
    else if (data.getFieldsHeader() == data.headerResponsive) {
        jsonObject["dateAndTimeResponse"] = data.getFieldsDateAndTimeResponse();
        jsonObject["responseCode"] = data.getFieldsResponseCode();
        jsonObject["versionProtocol"] = data.getFieldsVersionProtocol();
        jsonObject["responseText"] = data.getFieldsResponseText();
    }
    return  jsonObject.dump();
}


ProtocolAAD ProtocolAAD::SerializationDeserializationFormatJSON::deserializationData(const std::string& serializationStr) {
    ProtocolAAD setFields;
    
    nlohmann::json jsonResultObject = nlohmann::json::parse(serializationStr);
    std::string defineHeader;


    for (nlohmann::json::iterator it = jsonResultObject.begin(); it != jsonResultObject.end(); ++it) {
        if (it.key() == "header") {
            defineHeader = it.value();
            break;
        }
    }
    if (defineHeader == setFields.headerConnection) {
        setFields.setFieldsHeader(jsonResultObject["header"].get<std::string>());
        setFields.setFieldsClientID(jsonResultObject["clienId"].get<int>());
        setFields.setFieldsVersionProtocol(jsonResultObject["versionProtocol"].get<double>());
        setFields.setFieldsDateAndTimeConnection(jsonResultObject["dateAndTimeConnection"].get<std::string>());
    }
    else if (defineHeader == setFields.headerSelectOption) {
        setFields.setFieldsHeader(jsonResultObject["header"].get<std::string>());
        setFields.setFieldsClientID(jsonResultObject["clienId"].get<int>());
        setFields.setFieldsOperation(jsonResultObject["operation"].get<std::string>());
    }
    else if (defineHeader == setFields.headerAuthentication) {
        setFields.setFieldsHeader(jsonResultObject["header"].get<std::string>());
        setFields.setFieldsClientID(jsonResultObject["clienId"].get<int>());
        setFields.setFieldsLogin(jsonResultObject["login"].get<std::string>());
        setFields.setFieldsPassword(jsonResultObject["password"].get<std::string>());
        setFields.setFieldsDateAndTimeAuthentication(jsonResultObject["dateAndTimeAuthentication"].get<std::string>());
    }
    else if (defineHeader == setFields.headerRegistration) {
        setFields.setFieldsHeader(jsonResultObject["header"].get<std::string>());
        setFields.setFieldsClientID(jsonResultObject["clienId"].get<int>());
        setFields.setFieldsNewLogin(jsonResultObject["newLogin"].get<std::string>());
        setFields.setFieldsNewPassword(jsonResultObject["newPassword"].get<std::string>());
    }
    else if (defineHeader == setFields.headerPathInServer) {
        setFields.setFieldsHeader(jsonResultObject["header"].get<std::string>());
        setFields.setFieldsClientID(jsonResultObject["clienId"].get<int>());
        setFields.setFieldsIsDefaultServerPath(jsonResultObject["isDefaultServerPath"].get<bool>());
        setFields.setFieldsPathDirectory(jsonResultObject["pathDirectory"].get<std::string>());
    }
    else if (defineHeader == setFields.headerFileSend) {
        setFields.setFieldsHeader(jsonResultObject["header"].get<std::string>());
        setFields.setFieldsClientID(jsonResultObject["clienId"].get<int>());
        setFields.setFieldsFileSend(jsonResultObject["sendFile"].get<bool>());
        setFields.setFieldsFilename(jsonResultObject["filename"].get<std::string>());
        setFields.setFieldsSizeFile(jsonResultObject["sizeFile"].get<uintmax_t>());
        setFields.setFieldsBytesFormatBase64(jsonResultObject["bytesFormatBase64"].get<std::string>());
    }
    else if (defineHeader == setFields.headerResponsive) {
        setFields.setFieldsHeader(jsonResultObject["header"].get<std::string>());
        setFields.setFieldsDateAndTimeResponse(jsonResultObject["dateAndTimeResponse"].get<std::string>());
        setFields.setFieldsResponseCode(jsonResultObject["responseCode"].get<int>());
        setFields.setFieldsVersionProtocol(jsonResultObject["versionProtocol"].get<double>());
        setFields.setFieldsResponseText(jsonResultObject["responseText"].get<std::string>());
    }
  
    return setFields;
}

//std::string ProtocolAAD::getHeader() {
 //   return this->header;
//}

//=====================Конструкторы================================
ProtocolAAD::ProtocolAAD(const std::string& header, const int& clientID, const double& versionProtocol, const std::string& dateAndTimeConnection) {
    this->fieldsRequestOrResponsive.header = header;
    this->fieldsRequestOrResponsive.clientID = clientID;
    this->fieldsRequestOrResponsive.versionProtocol = round(versionProtocol * 10) / 10;
    this->fieldsRequestOrResponsive.dateAndTimeConnection = dateAndTimeConnection;
}

ProtocolAAD::ProtocolAAD(const std::string& header, const int& clientID, const std::string& operation) {
    this->fieldsRequestOrResponsive.header = header;
    this->fieldsRequestOrResponsive.clientID = clientID;
    this->fieldsRequestOrResponsive.operation = operation;
}

ProtocolAAD::ProtocolAAD(const std::string& header, const int& clientID, const std::string& login, const std::string& password, const std::string& dateAndTimeAuthentication) {
    this->fieldsRequestOrResponsive.header = header;
    this->fieldsRequestOrResponsive.clientID = clientID;
    this->fieldsRequestOrResponsive.login = login;
    this->fieldsRequestOrResponsive.password = password;
    this->fieldsRequestOrResponsive.dateAndTimeAuthentication = dateAndTimeAuthentication;
}
ProtocolAAD::ProtocolAAD(const std::string& header, const int& clientID, const std::string& newLogin, const std::string& newPassword) {
    this->fieldsRequestOrResponsive.header = header;
    this->fieldsRequestOrResponsive.clientID = clientID;
    this->fieldsRequestOrResponsive.newLogin = newLogin;
    this->fieldsRequestOrResponsive.newPassword = newPassword;
}
ProtocolAAD::ProtocolAAD(const std::string& header, const int& clientID, const bool isDefaultServerPath, const std::string& pathDirectory) {
    this->fieldsRequestOrResponsive.header = header;
    this->fieldsRequestOrResponsive.clientID = clientID;
    this->fieldsRequestOrResponsive.isDefaultServerPath = isDefaultServerPath;
    this->fieldsRequestOrResponsive.pathDirectory = pathDirectory;
}
ProtocolAAD::ProtocolAAD(const std::string & header, const int& clientID, const bool sendFile, const std::string& filename, const int& sizeFile, const std::string& bytesFormatBase64) {
    this->fieldsRequestOrResponsive.header = header;
    this->fieldsRequestOrResponsive.clientID = clientID;
    this->fieldsRequestOrResponsive.sendFile = sendFile;
    this->fieldsRequestOrResponsive.filename = filename;
    this->fieldsRequestOrResponsive.sizeFile = sizeFile;
    this->fieldsRequestOrResponsive.bytesFormatBase64 = bytesFormatBase64;
}



ProtocolAAD::ProtocolAAD(const std::string& header, const std::string& dateAndTimeResponse, const int& responceCode, const double& versionProtocol, const std::string& responseText) {
    this->fieldsRequestOrResponsive.header = header;
    this->fieldsRequestOrResponsive.dateAndTimeResponse = dateAndTimeResponse;
    this->fieldsRequestOrResponsive.responseCode = responceCode;
    this->fieldsRequestOrResponsive.versionProtocol = versionProtocol;
    this->fieldsRequestOrResponsive.responseText = responseText;
}




std::string ProtocolAAD::getDateAndTimeNow() {
    time_t now = time(NULL);
    char timeDate[26];
    ctime_s(timeDate, sizeof(timeDate), &now);
    std::string result(timeDate);
    return result.replace(result.find('\n'), 1, "\0");
}


//===========Геттеры=============
std::string ProtocolAAD::getFieldsHeader() {
    return this->fieldsRequestOrResponsive.header;
}
std::string ProtocolAAD::getFieldsDateAndTimeConnection() {
    return this->fieldsRequestOrResponsive.dateAndTimeConnection;
}
int ProtocolAAD::getFieldsClientID() {
    return this->fieldsRequestOrResponsive.clientID;
}
double ProtocolAAD::getFieldsVersionProtocol() {
    return round(this->fieldsRequestOrResponsive.versionProtocol * 10) / 10;
}
std::string ProtocolAAD::getFieldsDateAndTimeAuthentication() {
    return this->fieldsRequestOrResponsive.dateAndTimeAuthentication;
}

std::string ProtocolAAD::getFieldsResponseText() {
    return this->fieldsRequestOrResponsive.responseText;
}
int ProtocolAAD::getFieldsResponseCode() {
    return this->fieldsRequestOrResponsive.responseCode;
}

std::string ProtocolAAD::getFieldsOperation() {
    return this->fieldsRequestOrResponsive.operation;
}
std::string ProtocolAAD::getFieldsLogin() {
    return this->fieldsRequestOrResponsive.login;
}
std::string ProtocolAAD::getFieldsPassword() {
    return this->fieldsRequestOrResponsive.password;
}
std::string ProtocolAAD::getFieldsNewLogin() {
    return this->fieldsRequestOrResponsive.newLogin;
}
std::string ProtocolAAD::getFieldsNewPassword() {
    return this->fieldsRequestOrResponsive.newPassword;
}

bool ProtocolAAD::getFieldsIsDefaultServerPath() {
    return this->fieldsRequestOrResponsive.isDefaultServerPath;
}
std::string ProtocolAAD::getFieldsPathDirectory() {
    return this->fieldsRequestOrResponsive.pathDirectory;
}
std::string ProtocolAAD::getFieldsDateAndTimeResponse() {
    return this->fieldsRequestOrResponsive.dateAndTimeResponse;
}
std::string ProtocolAAD::getFieldsFilename() {
    return this->fieldsRequestOrResponsive.filename;
}
uintmax_t  ProtocolAAD::getFieldsSizeFile() {
    return this->fieldsRequestOrResponsive.sizeFile;
}
bool ProtocolAAD::getFieldsFileSend() {
    return this->fieldsRequestOrResponsive.sendFile;
}
std::string ProtocolAAD::getFieldsBytesFormatBase64() {
    return this->fieldsRequestOrResponsive.bytesFormatBase64;
}

//=============Сеттеры===========
void ProtocolAAD::setFieldsHeader(const std::string& header) {
    this->fieldsRequestOrResponsive.header = header;
}
void ProtocolAAD::setFieldsDateAndTimeConnection(const std::string& dateAndTimeConnection) {
    this->fieldsRequestOrResponsive.dateAndTimeConnection = dateAndTimeConnection;
}
void ProtocolAAD::setFieldsClientID(const int& clientID) {
    this->fieldsRequestOrResponsive.clientID = clientID;
}
void ProtocolAAD::setFieldsVersionProtocol(const double& versionProtocol) {
    this->fieldsRequestOrResponsive.versionProtocol = round(versionProtocol * 10) / 10;
}
void ProtocolAAD::setFieldsDateAndTimeAuthentication(const std::string& dateAndTimeAuthentication) {
    this->fieldsRequestOrResponsive.dateAndTimeAuthentication = dateAndTimeAuthentication;
}
void ProtocolAAD::setFieldsDateAndTimeResponse(const std::string& dateAndTimeResponse) {
    this->fieldsRequestOrResponsive.dateAndTimeResponse = dateAndTimeResponse;
}
void ProtocolAAD::setFieldsResponseText(const std::string& responseText) {
    this->fieldsRequestOrResponsive.responseText = responseText;
}
void ProtocolAAD::setFieldsResponseCode(const int& responseCode) {
    this->fieldsRequestOrResponsive.responseCode = responseCode;
}
void ProtocolAAD::setFieldsOperation(const std::string& operation) {
    this->fieldsRequestOrResponsive.operation = operation;
}

void ProtocolAAD::setFieldsLogin(const std::string& login) {
    this->fieldsRequestOrResponsive.login = login;

}
void ProtocolAAD::setFieldsPassword(const std::string& password) {
    this->fieldsRequestOrResponsive.password = password;
}
void ProtocolAAD::setFieldsNewLogin(const std::string& newLogin) {
    this->fieldsRequestOrResponsive.newLogin = newLogin;
}
void ProtocolAAD::setFieldsNewPassword(const std::string& newPassword) {
    this->fieldsRequestOrResponsive.newPassword = newPassword;
}
void ProtocolAAD::setFieldsIsDefaultServerPath(const bool isDefaultServerPath) {
    this->fieldsRequestOrResponsive.isDefaultServerPath = isDefaultServerPath;
}
void ProtocolAAD::setFieldsPathDirectory(const std::string& pathDirectory) {
    this->fieldsRequestOrResponsive.pathDirectory = pathDirectory;
}
void ProtocolAAD::setFieldsFilename(const std::string& filename) {
    this->fieldsRequestOrResponsive.filename = filename;
}
void ProtocolAAD::setFieldsSizeFile(const uintmax_t& sizeFile) {
    this->fieldsRequestOrResponsive.sizeFile = sizeFile;
}
void ProtocolAAD::setFieldsFileSend(const bool fileSend) {
    this->fieldsRequestOrResponsive.sendFile = fileSend;
}

void ProtocolAAD::setFieldsBytesFormatBase64(const std::string& bytesFormatBase64) {
    this->fieldsRequestOrResponsive.bytesFormatBase64 = bytesFormatBase64;
}


void ProtocolAAD::sendData(SOCKET clientConnection, ProtocolAAD data) {
    SerializationDeserializationFormatJSON serializationDataToJson;
    
    std::string serialiazedStr = serializationDataToJson.serializationData(data);
   // std::cout << serialiazedStr << std::endl;


    std::string sizeMessageStr = std::to_string(serialiazedStr.length());
    
    int countSendBytes = 0;
    countSendBytes = send(clientConnection, sizeMessageStr.data(), 16, 0); 
    if (countSendBytes == -1) {
        throw ExceptionNetwork(7, "an error occurred while sending the count Byte Data");
    }

    countSendBytes = send(clientConnection, serialiazedStr.data(), serialiazedStr.length(), 0);
    if (countSendBytes == -1) {
       throw ExceptionNetwork(8, "an error occurred while sending the data");
    }
}

ProtocolAAD ProtocolAAD::recvData(SOCKET clientConnection) {
    char sizeMessage[16] = {};

    int countRecvBytes = recv(clientConnection, sizeMessage, 16, 0);
  
    if (countRecvBytes == -1) {
        throw ExceptionNetwork(9, "an error occurred while receiving the data");
    }
    int sizeMessageINT = std::stoi(std::string(sizeMessage));
   
    char* recvDataChar = new char[sizeMessageINT];


    countRecvBytes = recv(clientConnection, recvDataChar, sizeMessageINT, 0);
    if (countRecvBytes == -1) {
       throw ExceptionNetwork(10, "an error occurred while receiving the data");
    }
    std::string recvJsonObject(recvDataChar);
    delete[] recvDataChar;

    recvJsonObject[countRecvBytes]= '\0';
   
    SerializationDeserializationFormatJSON deserialization;

    ProtocolAAD getData = deserialization.deserializationData(recvJsonObject);
    return getData;
}
