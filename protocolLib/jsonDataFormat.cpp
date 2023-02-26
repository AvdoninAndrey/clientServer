#include "jsonDataFormat.h"

jsonDataFormat::jsonDataFormat(const std::string& header, const std::string& message) {
    this->header = header;
    this->message = message;
}
jsonDataFormat::jsonDataFormat(const std::string& header, const std::string& filename, const unsigned int& fileSize) {
    this->header = header;
    this->filename = filename;
    this->fileSize = fileSize;
}

std::string jsonDataFormat::getMessage() {
    return this->message;
}
std::string jsonDataFormat::getHeader() {
    return this->header;
}

std::string jsonDataFormat::getFilename() {
    return this->filename;
}
unsigned int jsonDataFormat::getFileSize() {
    return this->fileSize;
}

void jsonDataFormat::setHeader(const std::string& header) {
    this->header = header;
}
void jsonDataFormat::setMessage(const std::string& message) {
    this->message = message;
}

void jsonDataFormat::setFilename(const std::string& filename) {
    this->filename = filename;
}
void jsonDataFormat::setfileSize(const int& fileSize) {
    this->fileSize = fileSize;
}