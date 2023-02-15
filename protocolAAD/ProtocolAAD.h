#pragma once
#include <string>

using namespace std;

class ProtocolAAD {
public:
	string header;
	string message;
	int clientID;
	ProtocolAAD() = default;
	ProtocolAAD(string header, string message);
	ProtocolAAD(string header, const int clientID);
};
