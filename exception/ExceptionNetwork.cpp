#include "ExceptionNetwork.h"

ExceptionNetwork::ExceptionNetwork(const int& codeError, const std::string& descriptionError) {
    this->codeError = codeError;
    this->descriptionError = descriptionError;
}
int ExceptionNetwork::getCodeError()
{
    return this->codeError;
}
std::string ExceptionNetwork::getDescriptionError()
{
    return this->descriptionError;
}
