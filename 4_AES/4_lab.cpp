#include "iostream"
#include <string>
#include <conio.h>
#include "cryptopp565\rsa.h"
#include "cryptopp565\cryptlib.h"
#include "cryptopp565\modes.h"
#include "cryptopp565\osrng.h"
#include "cryptopp565\filters.h"
#include "cryptopp565\cbcmac.h"
#include <vector>
#include "MyFile.h"

#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS


class AES
{
private:
    MyFile plaintext;
    MyFile keys;
    MyFile ivs;
    MyFile ciphertext;
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
    AES()
        : in_file_name(""), out_file_name("")
    {

    }
    ~AES()
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
        byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];

        rng.GenerateBlock(key, sizeof(key));

        for each (byte bit in key)
        {
            keys.GetData().push_back(bit);
        }
        if (!keys.Write("key.txt"))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "Key is ready!\nLook at 4_lab/4_lab/key.txt\n";
        }
        std::cout << "Press any key...";
        _getch();
        system("cls");
        ciphertext.GetData().resize(plaintext.GetData().size() + CryptoPP::AES::BLOCKSIZE);
        CryptoPP::ArraySink cs(&ciphertext.GetData()[0], ciphertext.GetData().size());
        int flag;
        std::cout << "ECB(1), CBC(2), CFB(3), OFB(4) or CTR(5) mode?\n";
        std::cin >> flag;
        switch (flag)
        {
        case 1:
        {
                  CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption Encryptor(key, sizeof(key));
                  CryptoPP::ArraySource(plaintext.GetData().data(), plaintext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Encryptor,
                      new CryptoPP::Redirector(cs))
                      );
                  break;
        }
        case 2:
        {
                  byte iv[CryptoPP::AES::BLOCKSIZE];
                  generate_ivs(iv);
                  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption Encryptor(key, sizeof(key), iv);
                  CryptoPP::ArraySource(plaintext.GetData().data(), plaintext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Encryptor,
                      new CryptoPP::Redirector(cs))
                      );
                  break;
        }
        case 3:
        {
                  byte iv[CryptoPP::AES::BLOCKSIZE];
                  generate_ivs(iv);
                  CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption Encryptor(key, sizeof(key), iv);
                  CryptoPP::ArraySource(plaintext.GetData().data(), plaintext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Encryptor,
                      new CryptoPP::Redirector(cs))
                      );
                  break;
        }
        case 4:
        {
                  byte iv[CryptoPP::AES::BLOCKSIZE];
                  generate_ivs(iv);
                  CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption Encryptor(key, sizeof(key), iv);
                  CryptoPP::ArraySource(plaintext.GetData().data(), plaintext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Encryptor,
                      new CryptoPP::Redirector(cs))
                      );
                  break;
        }
        case 5:
        {
                  byte iv[CryptoPP::AES::BLOCKSIZE];
                  generate_ivs(iv);
                  CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption Encryptor(key, sizeof(key), iv);
                  CryptoPP::ArraySource(plaintext.GetData().data(), plaintext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Encryptor,
                      new CryptoPP::Redirector(cs))
                      );
                  break;
        }
        default:
            break;
        }
        ciphertext.GetData().resize(cs.TotalPutLength());
        std::cout << "File encrypted!\nSave as:\n";
        std::cin >> out_file_name;
        system("cls");
        if (!ciphertext.Write(out_file_name))
        {
            std::cout << "error";
        }
        else
        {
            std::cout << "File saved as 4_lab/4_lab!" << "\n";
        }
        std::cout << "Press any key for ending...";
        _getch();
    }
    void Decrypt()
    {
        byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
        std::cout << "Move encrypted file at 4_lab/4_lab\n";
        std::cout << "Filename for decrypting:\n";
        std::cin >> in_file_name;
        if (!ciphertext.Open(in_file_name))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        system("cls");
        std::cout << "Move a key for decrypting at 4_lab/4_lab\n as key.txt";
        std::cout << "\nPress any key...";
        _getch();
        system("cls");
        if (!keys.Open("key.txt"))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        int flag;
        std::cout << "ECB(1), CBC(2), CFB(3), OFB(4) or CTR(5) mode?\n";
        std::cin >> flag;
        for (int i = 0; i < CryptoPP::AES::DEFAULT_KEYLENGTH; i++)
        {
            key[i] = keys.GetData().at(i);
        }
        plaintext.GetData().resize(ciphertext.GetData().size() + CryptoPP::AES::BLOCKSIZE);
        CryptoPP::ArraySink ds(&plaintext.GetData()[0], plaintext.GetData().size());
        switch (flag)
        {
        case 1:
        {
                  CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption Decryptor(key, sizeof(key));
                  CryptoPP::ArraySource(ciphertext.GetData().data(), ciphertext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Decryptor,
                      new CryptoPP::Redirector(ds))
                      );
                  break;
        }
        case 2:
        {

                  byte iv[CryptoPP::AES::BLOCKSIZE];
                  get_ivs(iv);
                  CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption Decryptor(key, sizeof(key), iv);
                  CryptoPP::ArraySource(ciphertext.GetData().data(), ciphertext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Decryptor,
                      new CryptoPP::Redirector(ds))
                      );
                  break;
        }
        case 3:
        {
                  byte iv[CryptoPP::AES::BLOCKSIZE];
                  get_ivs(iv);
                  CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption Decryptor(key, sizeof(key), iv);
                  CryptoPP::ArraySource(ciphertext.GetData().data(), ciphertext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Decryptor,
                      new CryptoPP::Redirector(ds))
                      );
                  break;
        }
        case 4:
        {
                  byte iv[CryptoPP::AES::BLOCKSIZE];
                  get_ivs(iv);
                  CryptoPP::OFB_Mode<CryptoPP::AES>::Decryption Decryptor(key, sizeof(key), iv);
                  CryptoPP::ArraySource(ciphertext.GetData().data(), ciphertext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Decryptor,
                      new CryptoPP::Redirector(ds))
                      );
                  break;
        }
        case 5:
        {

                  byte iv[CryptoPP::AES::BLOCKSIZE];
                  get_ivs(iv);
                  CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption Decryptor(key, sizeof(key), iv);
                  CryptoPP::ArraySource(ciphertext.GetData().data(), ciphertext.GetData().size(), true,
                      new CryptoPP::StreamTransformationFilter(Decryptor,
                      new CryptoPP::Redirector(ds))
                      );
                  break;
        }
        default:
            break;
        }
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
            std::cout << "File saved at 4_lab/4_lab!\n";
        }
        std::cout << "Press any key for ending...";
        _getch();
    }

};

void main()
{
    int flag;
    AES AES;
    system("cls");
    std::cout << "Encrypt(1) or Decrypt(2) a file by AES?\n";
    std::cin >> flag;
    system("cls");
    if (flag == 1)
        AES.Encrypt();
    if (flag == 2)
        AES.Decrypt();
}

