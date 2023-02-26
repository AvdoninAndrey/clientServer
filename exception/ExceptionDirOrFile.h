#pragma once
#include "Exception.h"
#include <filesystem>
#include <iostream>
class ExceptionDirOrFile: MyException {
private:
    const std::string correctAlphabets = " !\"#$ % &'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
public:
    ExceptionDirOrFile() = default;
    ExceptionDirOrFile(const int& codeError, const std::string& descriptionError);
    int getCodeError() override;
    std::string getDescriptionError()  override;
    bool checkingFileForExistence(std::filesystem::path path);
    bool checkStrPath(const std::string& pathStr);
    bool isAbsolutePath(const std::filesystem::path pathStr);
};