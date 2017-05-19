#include "stdafx.h"
#include "iostream"
#include <string>
#include <conio.h>
#include <vector>

using namespace std;

class MyFile
{
private:
	vector<char> _data;
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
	vector<char> &GetData() { return _data; }

	bool Open(string name)
	{
		const char *_name = name.c_str();
		_file = fopen(_name,"rb");

		if (_file != NULL)
		{
			fseek(_file, 0, SEEK_END);
			int size= ftell(_file); 
			rewind(_file);
			_data.resize(size);
			fread(_data.data(), 1, size, _file);
			fclose(_file);
		}
		return !_data.empty();
	}
	bool Write(string name)
	{	
		const char *_name = name.c_str();
		_file = fopen(_name, "w+b");
		int flag = fwrite(_data.data(), 1, _data.size(), _file);
		fclose(_file);
		return !(flag == 0);
	}

};

class One_Time_Pad
{
private:
	MyFile plaintext;
	MyFile key;
	MyFile ciphertext;
	string in_file_name;
	string out_file_name;

public:
	One_Time_Pad()
		: in_file_name(""), out_file_name("")
	{
	}
    ~One_Time_Pad()
    {
    }
	void Encrypt()
	{
		cout << "����������� ���� � ���������� ������� Crypto_1/Crypto_1\n";
		cout << "��� ����� ��� ����������:\n";
		cin >> in_file_name;
		if (!plaintext.Open(in_file_name))
		{
			cout << "error";
			system("pause");
			return;
		}
		system("cls");
		cout << "������ ����� = " << plaintext.GetData().size() <<"\n";
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
			cout << "���� ������������!\n��. ���� Key_maker/Key_maker/key.txt\n";
		}
		cout << "��� ����������� ������ ��������� ������� ����� �������...";
		_getch();
		system("cls");
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
		cout << "���� ����������!\n��������� ���:\n";
		cin >> out_file_name;
		if (!ciphertext.Write(out_file_name))
		{
			cout << "error";
		}
		else
		{
			cout << "���� �������� � ���������� Crypto_1/Crypto_1!" << "\n";
		}
		cout << "��� ���������� ������ ��������� ������� ����� �������...";
		_getch();
		ciphertext.~MyFile();
		plaintext.~MyFile();
		key.~MyFile();
	}
	void Decrypt()
	{
		cout << "����������� ������������� ���� � ���������� ������� Crypto_1/Crypto_1\n";
		cout << "��� ����� ��� �������������:\n";
		cin >> in_file_name;
		if (!ciphertext.Open(in_file_name))
		{
			cout << "error";
			system("pause");
			return;
		}
		system("cls");
		cout << "����������� ���� ��� ������������� ����� � ���������� ������� Crypto_1/Crypto_1\n ��� key.txt";
		cout << "\n��� ����������� ������ ��������� ������� ����� �������...";
		_getch();
		system("cls");
		if (!key.Open("key.txt"))
		{
			cout << "error";
			system("pause");
			return;
		}
		if (!key.GetData().size()==ciphertext.GetData().size())
		{
			cout << "error";
			system("pause");
			return;
		}
		for (int i = 0; i < ciphertext.GetData().size(); i++)
		{
			plaintext.GetData().push_back(ciphertext.GetData().at(i) ^ key.GetData().at(i));
		}
		cout << "���� �����������!\n��������� ���:\n";
		cin >> out_file_name;
		system("cls");
		if (!plaintext.Write(out_file_name))
		{
			cout << "error";
		}
		else
		{
			cout << "���� �������� � ���������� Crypto_1/Crypto_1!\n";
		}
		cout << "��� ���������� ������ ��������� ������� ����� �������...";
		_getch();
		delete &ciphertext;
		delete &plaintext;
		delete &key;
	}
	
};

void main()
{
	setlocale(LC_ALL, "Russian");
	int flag;
	One_Time_Pad Work_Pad;
	cout << "�����������(1) ��� ������������(2) ����?\n";
	cin >> flag;
	system("cls");
	if (flag == 1)
		Work_Pad.Encrypt();
	if (flag == 2)
		Work_Pad.Decrypt();
    Work_Pad.~One_Time_Pad();
	return;
}