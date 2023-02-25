#pragma once
#include "Exception.h"

class ExceptionNetwork : public MyException {

public:
    ExceptionNetwork() = delete;
    ExceptionNetwork(const int& codeError, const std::string& descriptionError);
    int getCodeError() override;
    std::string getDescriptionError()  override;
};