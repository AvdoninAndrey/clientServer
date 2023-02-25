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
}
/*bool ExceptionDirOrFile::checkStrPath(const std::string& pathStr) {
    for (size_t i = 0; i < pathStr.size(); i++) {
        if (static_cast<int>(pathStr[i]) < 32 || static_cast<int>(pathStr[i]) > 126) {
            return false;
        }
    }
    return true;
}*/
bool ExceptionDirOrFile::checkingFileForExistence(std::filesystem::path path) {
    return std::filesystem::exists(path.string());
}