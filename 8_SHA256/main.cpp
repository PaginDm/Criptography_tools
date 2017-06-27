#include "iostream"
#include <string>
#include <conio.h>
#include "cryptopp565\sha.h"
#include "cryptopp565\cryptlib.h"
#include "cryptopp565\modes.h"
#include "cryptopp565\osrng.h"
#include "cryptopp565\filters.h"
#include "cryptopp565\hex.h"
#include "cryptopp565\cbcmac.h"
#include "MyFile.h"
#include <vector>

#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS


class SHA
{
private:
    MyFile plaintext;
    MyFile Hash;
    std::string in_file_name;
    std::string out_file_name;
    std::vector<byte> hash_temp;
    int block_size = 512;

    void LoadFile()
    {
        std::cout << "Move a file at directory with project directory.\n";
        std::cout << "Filename for hashing:\n";
        std::cin >> in_file_name;
        if (!plaintext.Open("..\\..\\" + in_file_name))
        {
            std::cout << "error";
            system("pause");
            return;
        }
        system("cls");
    }

    void SaveHashAsFile()
    {
        std::cout << "File hasned!\nSave as:\n";
        std::cin >> out_file_name;
        system("cls");
        if (!Hash.Write("..\\..\\" + out_file_name))
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

public:
    SHA()
        : in_file_name(""), out_file_name("")
    {
    }
    ~SHA()
    {
    }
    std::vector<byte> GetHashAsVector()
    {
        return Hash.GetData();
    }

    void GetHashForFile()
    {
        LoadFile();
        CryptoPP::SHA256 hash;
        Hash.GetData().resize(32);
        CryptoPP::ArraySink cs(&Hash.GetData()[0], Hash.GetData().size());
        CryptoPP::ArraySource(plaintext.GetData().data(), plaintext.GetData().size(), true,
            new CryptoPP::HashFilter(hash, new CryptoPP::ArraySink(cs)
            ));
        SaveHashAsFile(); 
    }
    void GetHashForVector(std::vector<byte> plaintext)
    {
        Hash.GetData().clear();
        CryptoPP::SHA256 hash;
        Hash.GetData().resize(32);
        CryptoPP::ArraySink cs(&Hash.GetData()[0], Hash.GetData().size());
        CryptoPP::ArraySource(plaintext.data(), plaintext.size(), true,
            new CryptoPP::HashFilter(hash, new CryptoPP::ArraySink(cs)
            ));
    }
};

void main()
{
    SHA sha;
    //sha.GetHashForFile();
    MyFile Hash;
    MyFile plaintext;
    std::string out_file_name;
    std::string in_file_name;
    std::vector<byte> tmp;
	std::string int_face[3] = { "In process.","In process..","In process..." };
    std::cout << "Move a file at directory with project directory.\n";
    std::cout << "Filename for hashing:\n";
    std::cin >> in_file_name;
    if (!plaintext.Open("..\\..\\" + in_file_name))
    {
    std::cout << "error";
    system("pause");
    return;
    }
    system("cls");
    tmp.resize(256);
    int count_of_blocks = plaintext.GetData().size() - 255;
    for (int i = 0; i < count_of_blocks; i=i+32)
    {
    for (int j = 0; j < 256; j++)
    {
		tmp.at(j) = plaintext.GetData().at(i + j);
    }
	system("cls");
	std::cout << int_face[i % 3];
    sha.GetHashForVector(tmp);

    for each  (byte bit in sha.GetHashAsVector())
    {
        Hash.GetData().push_back(bit);
    }
    }

    std::cout << "File hasned!\nSave as:\n";
    std::cin >> out_file_name;
    system("cls");
    if (!Hash.Write("..\\..\\" + out_file_name))
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
