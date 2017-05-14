#include "stdafx.h"
#include "iostream"
#include <vector>

using namespace std;

class MyFile
{
private:
	vector<char> _data;
	FILE* _file;

public:
	MyFile()
		: _file(NULL)
	{
	}
	~MyFile(){
		_data.clear();
	}
	vector<char> &GetData() { return _data; }

	bool Open(string name)
	{
		const char *_name = name.c_str();
		_file = fopen(_name, "rb");

		if (_file != NULL)
		{
			fseek(_file, 0, SEEK_END); //get a size of file
			int size = ftell(_file);
			rewind(_file);
			_data.resize(size);
			fread(_data.data(), 1, size, _file);
			//cout << size;
			//cout << fread(_data.data(), 1, size, _file);
			//for each (char data in _data)
			//{
			//	cout << data;
			//}
			fclose(_file);
		}
		return !_data.empty();
	}
	bool Write(string name)
	{
		const char *_name = name.c_str();
		_file = fopen(_name, "w+b");
		int flag = fwrite(_data.data(), 1, _data.size(), _file); // �������� � ���� ���������� ������
		fclose(_file);
		return !(flag == 0);
	}

};


void main()
{

	setlocale(LC_ALL, "Russian");
	MyFile plaintext;
	MyFile key;
	cout << "��������� ����, ��� �������� ���������� ������������� ���� � ���������� ������� Key_maker/Key_maker � ��������� text.txt\n";
	system("pause");
	if (!plaintext.Open("text.txt"))
	{
		cout << "error";
		system("pause");
		return;
	}
	cout << "������ ����� = "<< plaintext.GetData().size() << "\n���������..";
	for (int i = 0; i < plaintext.GetData().size(); i++)
	{
		key.GetData().push_back((char)rand() % 256);
	}
	if (!key.Write("key.txt"))
	{
		cout << "error";
	}
	else
	{
		cout << "������������� ��������� � ����� Key_maker/Key_maker/key.txt\n";
	}
	cout << system("pause");
	return;
}