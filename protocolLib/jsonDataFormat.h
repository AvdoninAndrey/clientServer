#pragma once
#include <string>

//������ ����� ������������ ��� ����� � �������� ������ � ������� "JSON"
//�� ����, ������ ����� ��������� ������, � ����� "ProtocolAAD" ��������� �������� ��������
class jsonDataFormat {
private:
	std::string header = "";
	std::string message = "";
	std::string filename = "";
	unsigned int fileSize = 0;
public:
	jsonDataFormat() = default;
	jsonDataFormat(const std::string& header, const std::string& message);
	jsonDataFormat(const std::string& header, const std::string& filename, const unsigned int& fileSize);

	std::string getHeader();
	std::string getMessage();
	std::string getFilename();
	unsigned int getFileSize();

	void setHeader(const std::string& header);
	void setMessage(const std::string& message);
	void setFilename(const std::string& filename);
	void setfileSize(const int& fileSize);
};
