#include "SerializationDeserialization.h"

std::string SerializationDeserialization::serialization(jsonDataFormat data, std::string &typeData) {
    nlohmann::json jsonObject{};
    if (typeData == "text") {
        jsonObject["header"] = data.getHeader();
        jsonObject["message"] = data.getMessage();
    }
    else if (typeData == "fileInfo") {
        jsonObject["header"] = data.getHeader();
        jsonObject["filename"] = data.getFilename();
        jsonObject["fileSize"] = data.getFileSize();
    }
    return  jsonObject.dump();
}

jsonDataFormat SerializationDeserialization::deserialization(const std::string& serializationStr, std::string& typeData) {
    nlohmann::json jsonResultObject;
    jsonResultObject = nlohmann::json::parse(serializationStr);
    jsonDataFormat getData;
    if (typeData == "text") {
        getData.setHeader(jsonResultObject["header"].get<std::string>());
        getData.setMessage(jsonResultObject["message"].get<std::string>());
    }
    else if (typeData == "fileInfo") {
        getData.setHeader(jsonResultObject["header"].get<std::string>());
        getData.setFilename(jsonResultObject["filename"].get<std::string>());
        getData.setfileSize(jsonResultObject["fileSize"].get<int>());
    }
    return getData;
}
