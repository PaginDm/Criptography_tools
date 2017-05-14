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
	int dialog;
	cout << "Зашифровать(1) или расшифровать(2)?\n";
	cin >> dialog;
	while (dialog != 1 && dialog != 2)
	{	
		cout << "Неверный ответ.\n";
		cout << "Зашифровать(1) или расшифровать(2)?\n";
		cin >> dialog;
	}
	if (dialog == 1)
	{	
		cout << "Создайте файл text.txt в директории проекта Crypto_1/Crypto_1\n";
		cout << "С помощью модуля Key_maker создайте ключ шифрования и поместите в директорию Crypto_1/Crypto_1\n";
		system("pause");
		if (!plaintext.Open("text.txt"))
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
			cout << ciphertext.GetData()[i];
		}

		if (!ciphertext.Write("ciphertext.txt"))
		{
			cout << "error";
		}
		else
		{
			cout << "Зашифрованное сообщение в файле ciphertext.txt\n";
		}
		cout <<system("pause");
		return;
	}


	if (dialog == 2)
	{
		cout << "Поместите зашифрованный файл ciphertext.txt и ключ шифрования key.txt в директорию проекта Crypto_1/Crypto_1\n";
		system("pause");
		if (!ciphertext.Open("ciphertext.txt"))
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
		for (int i = 0; i < ciphertext.GetData().size(); i++)
		{
			plaintext.GetData().push_back(ciphertext.GetData().at(i) ^ key.GetData().at(i));
			//cout << ciphertext.GetData()[i];
		}

		if (!plaintext.Write("text.txt"))
		{
			cout << "error";
		}
		else
		{
			cout << "Расшифрованное сообщение в файле text.txt\n";
		}
		cout <<system("pause");
		return;

	}

	delete &ciphertext;
	delete &plaintext;
	delete &key;
}