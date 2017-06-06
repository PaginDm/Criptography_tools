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
    void generate_ivs(byte* iv)
    {
        CryptoPP::AutoSeededRandomPool rng(true);
        rng.GenerateBlock(iv, sizeof(iv));
        system("cls");
        for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; i++)
        {
            ivs.GetData().push_back(iv[i]);
        }
        if (!ivs.Write("ivs.txt"))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "Initialize vector is ready!\nLook at 4_lab/4_lab//iv.txt\n";
        }
        std::cout << "Press any key...";
        _getch();
    }
    void get_ivs(byte* iv)
    {
        system("cls");
        std::cout << "Move a initialize vector for decrypting at 4_lab/4_lab\n as ivs.txt";
        std::cout << "\nPress any key...";
        _getch();
        system("cls");
        if (!ivs.Open("ivs.txt"))
        {
            std::cout << "error";
            system("pause");
        }
        for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; i++)
        {
            iv[i] = ivs.GetData().at(i);
        }
    }
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
        std::cout << "Move a file at 3_laba/3_laba\n";
        std::cout << "Filename for encrypt:\n";
        std::cin >> in_file_name;
        if (!plaintext.Open("..\\..\\" + in_file_name))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        system("cls");
        std::cout << "Filesize = " << plaintext.GetData().size() << "\n";

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


        ciphertext.GetData().resize(plaintext.GetData().size());
        CryptoPP::ArraySink cs(&ciphertext.GetData()[0], ciphertext.GetData().size());
        CryptoPP::RSAES_OAEP_SHA_Encryptor Encryptor(publicKey);

        CryptoPP::ArraySource(plaintext.GetData().data(), plaintext.GetData().size(), true,
            new CryptoPP::PK_EncryptorFilter(rng, Encryptor, new CryptoPP::ArraySink(cs))
            );

        if (!ciphertext.Write("..\\..\\cipher.txt"))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "File saved as 4_lab/4_lab!" << "\n";
        }
        std::cout << "Press any key for ending...";
        getch();

    }
    void Decrypt()
    {
        std::cout << "Filename for decrypt:\n";
        std::cin >> in_file_name;
        if (!ciphertext.Open("..\\..\\" + in_file_name))
        {
            std::cout << "error";
            system("pause");
            return;
        }

        //CryptoPP::LoadPrivateKey("rsa-roundtrip.key", key2);
        if (!privatekey.Open("..\\..\\private.txt"))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        //std::cout << privatekey.GetData().size();
        CryptoPP::ArraySource privkey(privatekey.GetData().data(), privatekey.GetData().size(), true, new CryptoPP::Base64Decoder);

        CryptoPP::RSA::PrivateKey privateKey;
        privateKey.BERDecode(privkey);

        plaintext.GetData().resize(ciphertext.GetData().size());
        CryptoPP::ArraySink ds(&plaintext.GetData()[0], plaintext.GetData().size());
        CryptoPP::RSAES_OAEP_SHA_Decryptor Decryptor(privateKey);

        CryptoPP::ArraySource(ciphertext.GetData().data(), ciphertext.GetData().size(), true,
            new CryptoPP::PK_DecryptorFilter(rng, Decryptor, new CryptoPP::ArraySink(ds))
            );
        if (!plaintext.Write("..\\..\\plain.txt"))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "File saved as 4_lab/4_lab!" << "\n";
        }
        std::cout << "Press any key for ending...";
        getch();
    }

};

void main()
{
    int flag;
    RSA rsa;
    system("cls");
    std::cout << "Encrypt(1) or Decrypt(2) a file by AES?\n";
    std::cin >> flag;
    system("cls");
    if (flag==1)
        rsa.Encrypt();
    if (flag == 2)
        rsa.Decrypt();
}

