#include "ExceptionDirOrFile.h"

ExceptionDirOrFile::ExceptionDirOrFile(const int& codeError, const std::string& descriptionError) {
    this->codeError = codeError;
    this->descriptionError = descriptionError;
}
int ExceptionDirOrFile::getCodeError()
{
    return this->codeError;
}
std::string ExceptionDirOrFile::getDescriptionError()
{
    return this->descriptionError;
}bool ExceptionDirOrFile::checkStrPath(const std::string& pathStr) {
    return  pathStr.find_first_not_of(this->correctAlphabets, 0) == std::string::npos ? true : false;
}
bool ExceptionDirOrFile::checkingFileForExistence(std::filesystem::path path) {
    return std::filesystem::exists(path.string());
}

bool ExceptionDirOrFile::isAbsolutePath(const std::filesystem::path pathStr) {
    return pathStr.is_absolute();
}
