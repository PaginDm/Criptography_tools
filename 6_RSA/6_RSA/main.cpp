#include "iostream"
#include <string>
#include <conio.h>
#include "..\..\MyFile.h"
#include "..\..\cryptopp565\rsa.h"
#include "..\..\cryptopp565\cryptlib.h"
#include "..\..\cryptopp565\modes.h"
#include "..\..\cryptopp565\osrng.h"
#include "..\..\cryptopp565\filters.h"
#include "..\..\cryptopp565\base64.h"
#include "..\..\cryptopp565\cbcmac.h"
#include "..\..\MyFile.h"
#include <vector>

#pragma warning(disable : 4996)
#pragma comment(lib,"cryptlib.lib")
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

        for each  (byte var in publickey.GetData())
        {
            string_pub += var;
        }

		CryptoPP::StringSource pubString(string_pub, true, new CryptoPP::Base64Decoder);
		CryptoPP::RSAES_OAEP_SHA_Encryptor Encryptor(pubString);
		CryptoPP::AutoSeededRandomPool rnd;
		std::string encryptText = "";
		std::string string_plaintext = "";
		int block_size = 85;
		std::vector<byte> tmp;
		tmp.resize(block_size);
		int count_of_blocks = plaintext.GetData().size() / block_size;
		for (int i = 0; i < count_of_blocks; i++)
		{
			encryptText = "";
			string_plaintext = "";
			for (int j = 0; j < block_size; j++)
			{
				tmp.at(j) = plaintext.GetData().at(i * block_size + j);
			}
			for each  (byte var in tmp)
			{
				string_plaintext += var;
			}
			CryptoPP::StringSource(string_plaintext, true,
				new CryptoPP::PK_EncryptorFilter(rnd, Encryptor,
					new CryptoPP::StringSink(encryptText)));
			for (int i = 0; i < encryptText.size(); i++)
			{
				ciphertext.GetData().push_back(encryptText[i]);
			}
		}
		encryptText = "";
		string_plaintext = "";
		tmp.resize(plaintext.GetData().size() - (count_of_blocks*block_size));
		for(int j= 0; count_of_blocks * block_size + j<plaintext.GetData().size();j++)
		{
			tmp.at(j) = plaintext.GetData().at(count_of_blocks * block_size + j);
		} 
		for each  (byte var in tmp)
		{
			string_plaintext += var;
		}
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
		system("pause");

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


        for each  (byte var in privatekey.GetData())
        {
            string_priv += var;
        }

        CryptoPP::StringSource privString(string_priv, true, new CryptoPP::Base64Decoder);
        CryptoPP::RSAES_OAEP_SHA_Decryptor Decryptor(privString);
        CryptoPP::AutoSeededRandomPool randPool;
		std::string decryptText = "";
		std::string string_ciphertext = "";
		int block_size = 128;
		std::vector<byte> tmp;
		tmp.resize(block_size);
		int count_of_blocks = ciphertext.GetData().size() / block_size;
		for (int i = 0; i < count_of_blocks; i++)
		{
			decryptText = "";
			string_ciphertext = "";
			for (int j = 0; j < block_size; j++)
			{
				tmp.at(j) = ciphertext.GetData().at(i * block_size + j);
			}
			for each  (byte var in tmp)
			{
				string_ciphertext += var;
			}
			CryptoPP::StringSource(string_ciphertext, true,
				new CryptoPP::PK_DecryptorFilter(randPool, Decryptor,
					new CryptoPP::StringSink(decryptText)));
			for (int i = 0; i < decryptText.size(); i++)
			{
				plaintext.GetData().push_back(decryptText[i]);
			}
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
	
}

