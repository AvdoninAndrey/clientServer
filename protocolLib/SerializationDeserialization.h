#pragma once
#include "jsonDataFormat.h"
#include <nlohmann/json.hpp>
#include <iostream>

//Данный класса предназначен для сериализации данных при отправке и десериализации данных на приёме
class SerializationDeserialization {
protected:
	std::string serialization(jsonDataFormat data, std::string &typeData);
	jsonDataFormat deserialization(const std::string & serializationStr, std::string& typeData);
};