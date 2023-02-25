#pragma once
#include "Exception.h"
#include <filesystem>

class ExceptionDirOrFile: MyException {
private:
    const std::string correctAlphabets = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
public:
    ExceptionDirOrFile() = default;
    ExceptionDirOrFile(const int& codeError, const std::string& descriptionError);
    int getCodeError() override;
    std::string getDescriptionError()  override;
    bool checkingFileForExistence(std::filesystem::path path);
    //bool checkStrPath(const std::string& pathStr);
};