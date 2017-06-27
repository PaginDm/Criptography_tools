#include "iostream"
#include <string>
#include <conio.h>
#include <vector>
#include "MyFile.h"


class One_Time_Pad
{
private:
	MyFile plaintext;
	MyFile key;
	MyFile ciphertext;
	std::string in_file_name;
	std::string out_file_name;

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
		std::cout << "Move a file at Crypto_1/Crypto_1\n";
		std::cout << "Filename for encrypting:\n";
		std::cin >> in_file_name;
		if (!plaintext.Open(in_file_name))
		{
			std::cout << "error";
			system("pause");
			return;
		}
		system("cls");
		std::cout << "Filesize = " << plaintext.GetData().size() <<"\n";
		for (int i = 0; i < plaintext.GetData().size(); i++)
		{
			key.GetData().push_back((char)rand() % 256);
		}
		if (!key.Write("key.txt"))
		{
			std::cout << "error";
		}
		else
		{
			std::cout << "Key is ready!\nLook at Key_maker/Key_maker/key.txt\n";
		}
		std::cout << "Press any key...";
		_getch();
		system("cls");
		if (!key.Open("key.txt"))
		{
			std::cout << "error";
			system("pause");
			return;
		}
		for (int i = 0; i < plaintext.GetData().size(); i++)
		{
			ciphertext.GetData().push_back(plaintext.GetData().at(i) ^ key.GetData().at(i));
		}
		std::cout << "File encrypted!\nSave as:\n";
		std::cin >> out_file_name;
		if (!ciphertext.Write(out_file_name))
		{
			std::cout << "error";
		}
		else
		{
			std::cout << "File saved at Crypto_1/Crypto_1!" << "\n";
		}
		std::cout << "Press any key...";
		_getch();
	}
	void Decrypt()
	{
		std::cout << "Move encrypted file at Crypto_1/Crypto_1\n";
		std::cout << "Filename for decrypting:\n";
		std::cin >> in_file_name;
		if (!ciphertext.Open(in_file_name))
		{
			std::cout << "error";
			system("pause");
			return;
		}
		system("cls");
		std::cout << "Move a key for decrypting at Crypto_1/Crypto_1\n как key.txt";
		std::cout << "\nPress any key...";
		_getch();
		system("cls");
		if (!key.Open("key.txt"))
		{
			std::cout << "error";
			system("pause");
			return;
		}
		if (!key.GetData().size()==ciphertext.GetData().size())
		{
			std::cout << "error";
			system("pause");
			return;
		}
		for (int i = 0; i < ciphertext.GetData().size(); i++)
		{
			plaintext.GetData().push_back(ciphertext.GetData().at(i) ^ key.GetData().at(i));
		}
		std::cout << "File decrypted!\nSave as:\n";
		std::cin >> out_file_name;
		system("cls");
		if (!plaintext.Write(out_file_name))
		{
			std::cout << "error";
		}
		else
		{
			std::cout << "File saved at Crypto_1/Crypto_1!\n";
		}
		std::cout << "Press any key for ending...";
		_getch();

	}
	
};

void main()
{
	int flag;
	One_Time_Pad Work_Pad;
	std::cout << "Encrypt(1) or decrypt(2) file?\n";
	std::cin >> flag;
	system("cls");
	if (flag == 1)
		Work_Pad.Encrypt();
	if (flag == 2)
		Work_Pad.Decrypt();
	return;
}