#include "iostream"
#include <string>
#include <conio.h>
#include <ctime>
#include "..\..\cryptopp565\des.h"
#include "..\..\cryptopp565\cryptlib.h"
#include "..\..\cryptopp565\modes.h"
#include "..\..\cryptopp565\osrng.h"
#include "..\..\cryptopp565\filters.h"
#include "..\..\cryptopp565\base64.h"
#include "..\..\cryptopp565\cbcmac.h"
#include <vector>

#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS
#define UINT unsigned __int32
#define RAND_MAX 65535

class MITM
{
private:
    std::string plaintext="I hate your lifes";
    std::string ciphertext;
public:
    std::string Encrypt(byte key[], byte iv[], std::string plaintext)
    {
        std::string ciphertext;
        CryptoPP::AutoSeededRandomPool rng(true);
        //byte key[CryptoPP::DES::DEFAULT_KEYLENGTH];
        //rng.GenerateBlock(key, sizeof(key));
        //byte iv[CryptoPP::DES::BLOCKSIZE];
        //rng.GenerateBlock(iv, sizeof(iv));
        CryptoPP::CFB_Mode<CryptoPP::DES>::Encryption Encryptor(key, sizeof(key), iv);
        CryptoPP::StringSource(plaintext, true,
            new CryptoPP::StreamTransformationFilter(Encryptor,
            new CryptoPP::StringSink(ciphertext)));
        return ciphertext;
    }

    std::string Decrypt(byte key[], byte iv[], std::string ciphertext)
    {
        std::string plaintext;
        CryptoPP::AutoSeededRandomPool rng(true);
        //byte key[CryptoPP::DES::DEFAULT_KEYLENGTH];
        //rng.GenerateBlock(key, sizeof(key));
        //byte iv[CryptoPP::DES::BLOCKSIZE];
        //rng.GenerateBlock(iv, sizeof(iv));
        CryptoPP::CFB_Mode<CryptoPP::DES>::Decryption Decryptor(key, sizeof(key), iv);
        CryptoPP::StringSource(ciphertext, true,
            new CryptoPP::StreamTransformationFilter(Decryptor,
            new CryptoPP::StringSink(plaintext)));
        return plaintext;
    }

};

void main()
{
    CryptoPP::AutoSeededRandomPool rng(true);
    byte key[14];
    srand(time(0));
    UINT random = rand()*2;
    //std::cout << CryptoPP::DES_EDE2::DEFAULT_KEYLENGTH<<"\n";
    for each (byte var in key)
    {
        var = rand() % 256;
        std::cout << (int)var;
    }
    UINT first;
    UINT second;
    system("pause");
}