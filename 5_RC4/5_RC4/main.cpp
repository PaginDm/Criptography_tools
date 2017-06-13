#include "iostream"
#include <string>
#include <conio.h>
#include <vector>
#include "..\..\MyFile.h"

#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS


class keygener
{
private:
    MyFile key;
public:
    void Generate(int size)
    {
        for (int i = 0; i < size; i++)
        {
            key.GetData().push_back((byte)rand() % 256);
        }
        if (!key.Write("..\\..\\key.txt"))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "Key is ready!\nLook at directory with project directory. \n";
        }
    }
    MyFile GetKey()
    {
        return key;
    }
    void OpenKey()
    {
        system("cls");
        std::cout << "Move a key for decrypting at directory with project directory as key.txt";
        std::cout << "\nPress any key...";
        _getch();
        system("cls");
        if (!key.Open("..\\..\\key.txt"))
        {
            std::cout << "error";
            system("pause");
            return;
        }

    }


};

class RC4
{
private:
    MyFile plaintext;
    keygener key;
    MyFile ciphertext;
    std::string in_file_name;
    std::string out_file_name;
    byte S_table[256];
    int i = 0;
    int j = 0;

    void metadata_shedule(MyFile key)
    {
        for (i = 0; i < 256; i++)
        {
            S_table[i] = (byte)i;
        }
        j = 0;
        for (i = 0; i < 256; i++)
        {
            j = (j + S_table[i] + key.GetData()[i%key.GetData().size()]) % 256;
            byte temp = S_table[i];
            S_table[i] = S_table[j];
            S_table[j] = temp;
        }
        i = 0;
        j = 0;
    }

    byte get_key()
    {
        i = (i + 1) % 256;
        j = (j + S_table[i]) % 256;
        byte temp = S_table[i];
        S_table[i] = S_table[j];
        S_table[j] = temp;
        return S_table[(S_table[i] + S_table[j]) % 256];
    }
public:
    RC4()
    {}

    void Encrypt()
    {
        
        std::cout << "Move a file at directory with project directory.\n";
        std::cout << "Filename for encrypt:\n";
        std::cin >> in_file_name;
        if (!plaintext.Open("..\\..\\" + in_file_name))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        system("cls");
        int size;
        std::cout << "Enter size of key:\n";
        std::cin >> size;
        key.Generate(size);
        metadata_shedule(key.GetKey());
        system("cls");

        for (int k = 0; k < plaintext.GetData().size(); k++)
        {
            ciphertext.GetData().push_back((plaintext.GetData()[k] ^ get_key()));
        }
        std::cout << "File encrypted!\nSave as:\n";
        std::cin >> out_file_name;
        system("cls");
        if (!ciphertext.Write("..\\..\\"+out_file_name))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "File saved at directory with project directory." << "\n";
        }
        std::cout << "Press any key for ending...";
        _getch();

    }

    void Decrypt()
    {
        key.OpenKey();
        metadata_shedule(key.GetKey());
        std::cout << "Move encrypted file at directory with project directory.\n";
        std::cout << "Filename for decrypting:\n";
        std::cin >> in_file_name;
        if (!ciphertext.Open("..\\..\\"+in_file_name))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        system("cls");

        for (int k = 0; k < ciphertext.GetData().size(); k++)
        {
            plaintext.GetData().push_back((ciphertext.GetData()[k] ^ get_key()));
        }
        std::cout << "File decrypted!\nSave as:\n";
        std::cin >> out_file_name;
        system("cls");
        if (!plaintext.Write("..\\..\\"+out_file_name))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "File saved at directory with project directory.\n";
        }
        std::cout << "Press any key for ending...";
        _getch();
    }
};

void main()
{
    int flag;
    RC4 rc4;
    system("cls");
    std::cout << "Encrypt(1) or Decrypt(2) a file by RC4?\n";
    std::cin >> flag;
    system("cls");
    if (flag == 1)
        rc4.Encrypt();
    if (flag == 2)
        rc4.Decrypt();
}

