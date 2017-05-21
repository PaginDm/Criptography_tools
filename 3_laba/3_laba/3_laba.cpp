// 3_laba.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "iostream"

#include <string>
#include <conio.h>
#include "..\cryptopp565\des.h"
#include "..\cryptopp565\cryptlib.h"
#include "..\cryptopp565\modes.h"
#include "..\cryptopp565\osrng.h"
#include "..\cryptopp565\filters.h"
#include "..\cryptopp565\cbcmac.h"
#include <vector>



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

class DES
{
private:
    MyFile plaintext;
    MyFile keys;
    MyFile ivs;
    MyFile ciphertext;
    std::string in_file_name;
    std::string out_file_name;
public:
    DES()
        : in_file_name(""), out_file_name("")
    {

    }
    ~DES()
    {

    }
    void Encrypt()
    {
        std::cout << "Move a file at 3_laba/3_laba\n";
        std::cout << "Filename for encrypt:\n";
        std::cin >> in_file_name;
        if (!plaintext.Open(in_file_name))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        system("cls");
        std::cout << "Filesize = " << plaintext.GetData().size() << "\n";
        CryptoPP::AutoSeededRandomPool rng(true);
        byte key[CryptoPP::DES::DEFAULT_KEYLENGTH];
        byte iv[CryptoPP::DES::BLOCKSIZE];

        rng.GenerateBlock(key, sizeof (key));
        rng.GenerateBlock(iv, sizeof (iv));

        for each (byte bit in key)
        {
            keys.GetData().push_back(bit);
        }
        for each (byte ivect in iv)
        {
            ivs.GetData().push_back(ivect);
        }
        if (!keys.Write("key.txt"))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "Key is ready!\nLook at 3_lab/3_lab/key.txt\n";
        }
        if (!ivs.Write("ivs.txt"))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "Initialize vector is ready!\nLook at 3_lab/3_lab//iv.txt\n";
        }
        std::cout << "Press any key...";
        _getch();
        system("cls");
        ciphertext.GetData().resize(plaintext.GetData().size() + CryptoPP::DES::BLOCKSIZE);
        CryptoPP::ArraySink cs(&ciphertext.GetData()[0], ciphertext.GetData().size());
        CryptoPP::CFB_Mode<CryptoPP::DES>::Encryption Encryptor(key, sizeof (key), iv);
        CryptoPP::ArraySource(plaintext.GetData().data(), plaintext.GetData().size(), true,
            new CryptoPP::StreamTransformationFilter(Encryptor,
            new CryptoPP::Redirector(cs))
            );
        ciphertext.GetData().resize(cs.TotalPutLength());
        std::cout << "File encryptedн!\nSave as:\n";
        std::cin >> out_file_name;
        if (!ciphertext.Write(out_file_name))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "File saved as 3_lab/3_lab!" << "\n";
        }
        std::cout << "Press any key for ending...";
        _getch();
        ciphertext.~MyFile();
        plaintext.~MyFile();
        keys.~MyFile();
    }
    void Decrypt()
    {
        byte key[CryptoPP::DES::DEFAULT_KEYLENGTH];
        byte iv[CryptoPP::DES::BLOCKSIZE];
        std::cout << "Move encrypted file at 3_lab/3_lab\n";
        std::cout << "Filename for decrypting:\n";
        std::cin >> in_file_name;
        if (!ciphertext.Open(in_file_name))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        system("cls");
        std::cout << "Move a key for decrypting at 3_lab/3_lab\n as key.txt";
        std::cout << "\nPress any key...";
        _getch();
        system("cls");
        if (!keys.Open("key.txt"))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        std::cout << "Move a initialize vector for decrypting at 3_lab/3_lab\n as ivs.txt";
        std::cout << "\nPress any key...";
        _getch();
        system("cls");
        if (!ivs.Open("ivs.txt"))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        for (int i = 0; i < CryptoPP::DES::DEFAULT_KEYLENGTH; i++)
        {
            key[i] = keys.GetData().at(i);
        }
        for (int i = 0; i < CryptoPP::DES::BLOCKSIZE; i++)
        {
            iv[i] = ivs.GetData().at(i);
        }
        ciphertext.Write("output.txt");
        plaintext.GetData().resize(ciphertext.GetData().size() + CryptoPP::DES::BLOCKSIZE);
        CryptoPP::ArraySink ds(&plaintext.GetData()[0], plaintext.GetData().size());
        CryptoPP::CFB_Mode<CryptoPP::DES>::Decryption Decryptor(key, sizeof (key), iv);
        CryptoPP::ArraySource(ciphertext.GetData().data(), ciphertext.GetData().size(), true,
            new CryptoPP::StreamTransformationFilter(Decryptor,
            new CryptoPP::Redirector(ds))
            );
        plaintext.GetData().resize(ds.TotalPutLength());
        std::cout << "File decrypted!\nSave as:\n";
        std::cin >> out_file_name;
        system("cls");
        if (!plaintext.Write(out_file_name))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "File saved at 3_lab/3_lab!\n";
        }
        std::cout << "Press any key for ending...";
        _getch();
        ciphertext.~MyFile();
        plaintext.~MyFile();
        keys.~MyFile();
    }

};
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
        std::cout << "Filesize = " << plaintext.GetData().size() << "\n";
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
        ciphertext.~MyFile();
        plaintext.~MyFile();
        key.~MyFile();
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
        std::cout << "Move a key for decrypting at Crypto_1/Crypto_1\n as key.txt";
        std::cout << "\nPress any key...";
        _getch();
        system("cls");
        if (!key.Open("key.txt"))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        if (!key.GetData().size() == ciphertext.GetData().size())
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
        ciphertext.~MyFile();
        plaintext.~MyFile();
        key.~MyFile();
    }

};

void main()
{   
    int flag;
    int flag2;
    One_Time_Pad timepad;
    DES des;
    std::cout << "OneTime Pad(1) or DES(2)?";
    std::cin >> flag2;
    if (flag2 == 1)
    {
        system("cls");
        std::cout << "Encrypt(1) or Decrypt(2) a file?\n";
        std::cin >> flag;
        system("cls");
        if (flag == 1)
            timepad.Encrypt();
        if (flag == 2)
            timepad.Decrypt();
        timepad.~One_Time_Pad();
    }
    else 
    {
        system("cls");
        std::cout << "Encrypt(1) or Decrypt(2) a file?\n";
        std::cin >> flag;
        system("cls");
        if (flag == 1)
            des.Encrypt();
        if (flag == 2)
            des.Decrypt();
        des.~DES();
    }
}

