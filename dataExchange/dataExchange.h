#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include "..\protocolAAD\ProtocolAAD.h"

class DataExchange  {
public:
	DataExchange() = default;
	void sendData(SOCKET clientConnection, ProtocolAAD data);
	ProtocolAAD recvData(SOCKET clientConnection);
};
