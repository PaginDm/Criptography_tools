#pragma once
#include <vector>

#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS
typedef unsigned char byte;

class MyFile
{
private:
	std::vector<byte> _data;
	FILE* _file;

public:
	MyFile()
		: _file(NULL), _data(NULL)
	{
	}
	~MyFile()
	{
		_data.clear();
	}
	std::vector<byte> &GetData() { return _data; }

	bool Open(std::string name)
	{
		const char *_name = name.c_str();
		_file = fopen(_name, "rb");

		if (_file != NULL)
		{
			fseek(_file, 0, SEEK_END);
			int size = ftell(_file);
			rewind(_file);
			_data.resize(size);
			fread(_data.data(), 1, size, _file);
			fclose(_file);
		}
		return !_data.empty();
	}
	bool Write(std::string name)
	{
		const char *_name = name.c_str();
		_file = fopen(_name, "w+b");
		int flag = fwrite(_data.data(), 1, _data.size(), _file);
		fclose(_file);
		return !(flag == 0);
	}

};