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
		_file = fopen(_name,"rb");

		if (_file != NULL)
		{
			fseek(_file, 0, SEEK_END); //get a size of file
			int size= ftell(_file); 
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
	MyFile ciphertext;
	string text;
	string endtext;

	cout << "Переместите файл в директорию проекта Crypto_1/Crypto_1\n";
	cout << "Имя файла для шифрования/дешифрования:\n";
	cin >> text;
	cout << "Поместите известный ключ key.txt (в случае шифрования создайте в модуле Key_maker) в директорию Crypto_1/Crypto_1\n";
	system("pause");
	if (!plaintext.Open(text))
	{
		cout << "error";
		system("pause");
		return;
		}
	if (!key.Open("key.txt"))
	{
		cout << "error";
		system("pause");
		return;
	}
	for (int i = 0; i < plaintext.GetData().size(); i++)
	{
		ciphertext.GetData().push_back(plaintext.GetData().at(i) ^ key.GetData().at(i));
	}
	cout << "Зашифровано/Расшифровано!!\nСохранить как:\n";
	cin >> endtext;
	if (!ciphertext.Write(endtext))
	{
		cout << "error";
	}
	else
	{
		cout << "Смотри файл " << endtext <<"\n";
	}
	cout <<system("pause");
	delete &ciphertext;
	delete &plaintext;
	delete &key;
	return;
}