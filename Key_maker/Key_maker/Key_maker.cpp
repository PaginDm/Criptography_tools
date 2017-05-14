#include "stdafx.h"
#include "iostream"
#include <string>
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
		int flag = fwrite(_data.data(), 1, _data.size(), _file); // записать в файл содержимое буфера
		fclose(_file);
		return !(flag == 0);
	}

};


void main()
{

	setlocale(LC_ALL, "Russian");
	MyFile plaintext;
	MyFile key;
	string text;
	string keytxt;
	cout << "Поместите файл, для которого необходимо сгенерировать ключ в директорию проекта Key_maker/Key_maker\nИмя файла:";
	cin >> text;
	if (!plaintext.Open(text))
	{
		cout << "error";
		system("pause");
		return;
	}
	cout << "Размер ключа = "<< plaintext.GetData().size() << "\nГенерация..";
	for (int i = 0; i < plaintext.GetData().size(); i++)
	{
		key.GetData().push_back((char)rand() % 256);
	}
	cout << "\nСохранить как:";
	cin >> keytxt;
	if (!key.Write(keytxt))
	{
		cout << "error";
	}
	else
	{
		cout << "Ключ в файле Key_maker/Key_maker/" <<keytxt << "\n";
	}
	cout << system("pause");
	delete &plaintext;
	delete &key;
	return;
}