#include "base64.h"

std::string Base64::base64Encode(const std::string& text) {
    std::string result;

    int val = 0, valb = -6;
    for (unsigned char c : text) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(this->base64Alphabets[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(this->base64Alphabets[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    return result;
}

std::string Base64::base64Decode(const std::string& encodeString) {
    std::string result;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[this->base64Alphabets[i]] = i;

    int val = 0, valb = -8;
    for (unsigned char c : encodeString) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            result.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return result;
}
