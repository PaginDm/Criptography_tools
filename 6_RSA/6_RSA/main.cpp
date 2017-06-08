#include "iostream"
#include <string>
#include <conio.h>
#include "..\..\cryptopp565\rsa.h"
#include "..\..\cryptopp565\cryptlib.h"
#include "..\..\cryptopp565\modes.h"
#include "..\..\cryptopp565\osrng.h"
#include "..\..\cryptopp565\filters.h"
#include "..\..\cryptopp565\base64.h"
#include "..\..\cryptopp565\cbcmac.h"
#include <vector>

#pragma warning(disable : 4996)
#pragma comment(lib,"cryptlib.lib")
#define _CRT_NO_WARNINGS

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

class keygener
{
private:
    MyFile key;
public:
    void Generate(int size)
    {
        for (int i = 0; i < size; i++)
        {
            key.GetData().push_back((unsigned char)rand() % 256);
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

    void GenerateForRsa()
    {
        MyFile privatekey;
        MyFile publickey;
        CryptoPP::AutoSeededRandomPool rng;
        std::string strprivkey, strpubkey;
        CryptoPP::InvertibleRSAFunction privateKey;

        privateKey.Initialize(rng, 1024);

        CryptoPP::Base64Encoder privkeysink(new CryptoPP::StringSink(strprivkey), false);
        privateKey.DEREncode(privkeysink);
        privkeysink.MessageEnd();

        CryptoPP::RSAFunction publicKey(privateKey);

        CryptoPP::Base64Encoder pubkeysink(new CryptoPP::StringSink(strpubkey), false);
        publicKey.DEREncode(pubkeysink);
        pubkeysink.MessageEnd();

        for (int i = 0; i < strpubkey.size(); i++)
        {
            publickey.GetData().push_back(strpubkey[i]);
        }

        for (int i = 0; i < strprivkey.size(); i++)
        {
            privatekey.GetData().push_back(strprivkey[i]);
        }

        if (!privatekey.Write("..\\..\\private.txt"))
        {
            std::cout << "error";
        }


        if (!publickey.Write("..\\..\\public.txt"))
        {
            std::cout << "error";
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

class RSA
{
private:
    MyFile plaintext;
    MyFile privatekey;
    MyFile publickey;
    MyFile ivs;
    MyFile ciphertext;
    CryptoPP::AutoSeededRandomPool rng;
    std::string in_file_name;
    std::string out_file_name;
public:
    RSA()
        : in_file_name(""), out_file_name("")
    {

    }
    ~RSA()
    {

    }
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
        if (!publickey.Open("..\\..\\public.txt"))
        {
            return;
        }

        std::string string_pub = "";
        std::string string_plaintext = "";

        for each  (byte var in plaintext.GetData())
        {
            string_plaintext += var;
        }
        for each  (byte var in publickey.GetData())
        {
            string_pub += var;
        }

		CryptoPP::StringSource pubString(string_pub, true, new CryptoPP::Base64Decoder);
		CryptoPP::RSAES_OAEP_SHA_Encryptor Encryptor(pubString);
		CryptoPP::AutoSeededRandomPool rnd;
		std::string encryptText = "";

        CryptoPP::StringSource(string_plaintext, true,
            new CryptoPP::PK_EncryptorFilter(rnd, Encryptor,
            new CryptoPP::StringSink(encryptText)));

        for (int i = 0; i < encryptText.size(); i++)
        {
            ciphertext.GetData().push_back(encryptText[i]);
        }

        if (!ciphertext.Write("..\\..\\cipher.txt"))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "File saved at directory with project directory as cipher.txt" << "\n";
        }

    }
    void Decrypt()
    {
        system("cls");
        plaintext.GetData().clear();
        if (!privatekey.Open("..\\..\\private.txt"))
        {
            return;
        }
        if (!ciphertext.Open("..\\..\\cipher.txt"))
        {
            return;
        }
        std::string string_priv = "";
        std::string string_ciphertext = "";

        for each  (byte var in ciphertext.GetData())
        {
            string_ciphertext += var;
        }
        for each  (byte var in privatekey.GetData())
        {
            string_priv += var;
        }
        CryptoPP::StringSource privString(string_priv, true, new CryptoPP::Base64Decoder);
        CryptoPP::RSAES_OAEP_SHA_Decryptor Decryptor(privString);
        CryptoPP::AutoSeededRandomPool randPool;
        std::string decryptText = "";


        CryptoPP::StringSource(string_ciphertext, true,
            new CryptoPP::PK_DecryptorFilter(randPool, Decryptor,
            new CryptoPP::StringSink(decryptText)));

        for (int i = 0; i < decryptText.size(); i++)
        {
            plaintext.GetData().push_back(decryptText[i]);
        }

        std::cout << "File decrypted!\nSave as:\n";
        std::cin >> out_file_name;
        if (!plaintext.Write("..\\..\\"+out_file_name))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "File saved at directory with project directory.\n";
        }
    }

};

void main()
{
    int flag;
    RSA rsa;
    keygener keys;
    system("cls");
    keys.GenerateForRsa();
    rsa.Encrypt();
    rsa.Decrypt();
    std::cout << "Press any key for ending...";
    getch();
}

