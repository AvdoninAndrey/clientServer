#pragma once
#include <string>

class MyException
{
protected:
    int codeError;
    std::string descriptionError;
public:
    virtual int getCodeError() = 0;
    virtual std::string getDescriptionError() = 0;
};