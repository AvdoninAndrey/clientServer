#include "ProtocolAAD.h"

ProtocolAAD::ProtocolAAD(string header, string message) {
	this->header = header;
	this->message = message;
}

ProtocolAAD::ProtocolAAD(string header, const int clientID) {
	this->header = header;
	this->clientID = clientID;
}
