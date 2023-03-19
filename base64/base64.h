#pragma once
#include <string>
#include <vector>

class Base64 {
private:
	const std::string base64Alphabets = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
protected:
	std::string base64Encode(const std::string& text);
	std::string base64Decode(const std::string& encodeString);
};