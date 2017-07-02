#include "iostream"
#include <windows.h>
#include <string>
#include <conio.h>
#include <map>   
#include <stdint.h>
#include <ctime>
#include <vector>
#include <fstream>
#include "cryptopp565\sha.h"
#include "cryptopp565\cryptlib.h"
#include "cryptopp565\modes.h"
#include "cryptopp565\osrng.h"
#include "cryptopp565\filters.h"
#include "cryptopp565\hex.h"
#include "cryptopp565\cbcmac.h"
#include "MyFile.h"


#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS
#define BUFSIZE MAX_PATH

class File 
{
private:
    std::string path;
    std::string altername;
    std::string creationTime;
    std::string lastAccessTime;
    std::string size;
	bool isdirectory;
	bool ishidden;
	bool issystem;
	bool isarchive;
	bool iscompressed;

public:


    File()
    {
    }

	~File()
	{

	}

	std::string GetPath()
	{
		return path;
	}

    File(std::string path,std::string altername,std::string creationTime,std::string lastAccessTime,std::string size, bool isdirectory, bool ishidden, bool issystem,bool isarchive,bool iscompressed)
    {
        this->path = path;
        this->altername = altername;
        this->creationTime = creationTime;
        this->lastAccessTime = lastAccessTime;
        this->size = size;
		this->isdirectory = isdirectory;
		this->ishidden = ishidden;
		this->issystem = issystem;
		this->isarchive = isarchive;
		this->iscompressed = iscompressed;
    }


};


class HashTable
{
private:
    std::vector<byte> hash_temp;
    int block_size = 16;

public:
    HashTable()
    {
    }
    ~HashTable()
    {
    }


	uint16_t MyHashAsIndex(std::string str_plaintext)
	{
		std::vector<byte> plaintext;
		for (int i = 0; i < str_plaintext.size(); i++)
		{
			plaintext.push_back(str_plaintext[i]);
		}
		int count_of_blocks = plaintext.size() / block_size;
		int add_bytes = plaintext.size() % block_size;
		hash_temp.resize(block_size);
		plaintext.resize(block_size*(count_of_blocks + 1));
		for (int i = count_of_blocks * block_size + add_bytes; i < (count_of_blocks + 1) * block_size; i++)
		{
			plaintext.at(i) = plaintext.size()%block_size;
		}
		for (int i = 0; i < block_size; i++)
		{
			hash_temp.at(i) = plaintext.at(i);
		}

		for (int i = 1; i < count_of_blocks + 1; i++)
		{
			for (int j = 0; j < block_size; j++)
			{
				hash_temp.at(j) = hash_temp.at(j) ^ plaintext.at(str_plaintext.size()*j%block_size) ^(hash_temp.at(j) >> (str_plaintext.size()%8) | hash_temp.at(j) << (8- (str_plaintext.size() % 8)));
			}
		}

		uint16_t *pData = (uint16_t*)hash_temp.data();
		return *pData;
	}

	uint16_t ShaHashAsIndex(std::string str_plaintext)
	{
		std::vector<byte> plaintext;
		for (int i = 0; i < str_plaintext.size(); i++)
		{
			plaintext.push_back(str_plaintext[i]);
		}
		std::vector<byte> hash;
		CryptoPP::SHA256 hash_sha;
		hash.resize(32);
		CryptoPP::ArraySink cs(&hash[0], hash.size());
		CryptoPP::ArraySource(plaintext.data(), plaintext.size(), true,
			new CryptoPP::HashFilter(hash_sha, new CryptoPP::ArraySink(cs)
			));
		uint16_t *pData = (uint16_t*)hash.data();
		return *pData;

	}

	static void MakeTable(std::vector<std::vector<File>> &hashTable, bool &flag, std::string pathdir, int type, int &i, int &j, float &a)
	{
		HashTable hashFunc;
		WIN32_FIND_DATAA findData;
		HANDLE hf;
		hf = FindFirstFileA((pathdir + "*").c_str(), &findData);
		std::string path;
		std::string altername;
		std::stringstream creationTime;
		std::stringstream lastAccessTime;
		bool isdirectory = false;
		bool ishidden = false;
		bool issystem = false;
		bool isarchive = false;
		bool iscompressed = false;
		std::string size;
		if (hf == INVALID_HANDLE_VALUE)
		{
			return;
		}
		do
		{
			bool isdirectory = false;
			bool ishidden = false;
			bool issystem = false;
			bool isarchive = false;
			bool iscompressed = false;

			char buff[sizeof(findData.cFileName)];
			sprintf(buff, "%s", findData.cFileName);
			std::string path = std::string(buff);
			if (findData.dwFileAttributes && FILE_ATTRIBUTE_DIRECTORY)
			{
				if (path != "." && path != "..") {
					MakeTable(hashTable, flag, pathdir + path + "\\", 0, i, j, a);
				}
				isdirectory = true;
			}
			if (findData.dwFileAttributes && FILE_ATTRIBUTE_HIDDEN)
			{
				ishidden = true;
			}
			if (findData.dwFileAttributes && FILE_ATTRIBUTE_SYSTEM)
			{
				issystem = true;
			}
			if (findData.dwFileAttributes && FILE_ATTRIBUTE_ARCHIVE)
			{
				isarchive = true;
			}
			if (findData.dwFileAttributes && FILE_ATTRIBUTE_COMPRESSED)
			{
				iscompressed = true;
			}

			sprintf(buff, "%s", findData.cAlternateFileName);
			std::string altername = std::string(buff);
			SYSTEMTIME LcreationTime;
			FileTimeToSystemTime(&findData.ftCreationTime, &LcreationTime);
			creationTime << LcreationTime.wHour << ":" << LcreationTime.wMinute << " " << LcreationTime.wDay << "." << LcreationTime.wMonth << "." << LcreationTime.wYear;
			SYSTEMTIME LlastaccessTime;
			FileTimeToSystemTime(&findData.ftLastAccessTime, &LlastaccessTime);
			lastAccessTime << LlastaccessTime.wHour << ":" << LlastaccessTime.wMinute << " " << LlastaccessTime.wDay << "." << LlastaccessTime.wMonth << "." << LlastaccessTime.wYear;
			sprintf(buff, "%x", findData.nFileSizeLow);
			std::string size = std::string(buff);
			File *file = new File(pathdir + path, altername, creationTime.str(), lastAccessTime.str(), size, isdirectory, ishidden, issystem, isarchive, iscompressed);
			i++;
			if (flag)
			{
				if (hashTable[hashFunc.MyHashAsIndex(pathdir + path)].size() == 0)
					j++;
				hashTable[hashFunc.MyHashAsIndex(pathdir + path)].push_back(*file);
			}
			else
			{
				if (hashTable[hashFunc.ShaHashAsIndex(pathdir + path)].size() == 0)
					j++;
				hashTable[hashFunc.ShaHashAsIndex(pathdir + path)].push_back(*file);
			}
			if (i % 50 == 0)
			{
				system("cls");
				std::cout << "Files:" << i << ". A=" << (float)i / j;
			}
		} while (FindNextFileA(hf, &findData) && (((float)i / j) < a));
		FindClose(hf);

	}

	static void SaveTable(std::vector<std::vector<File>> &hashTable, std::string &FileName)
	{
		std::ofstream out;
		out.open("..\\Resources\\"+FileName);
		for (int i = 0; i < hashTable.size(); i++)
		{
			if (hashTable.at(i).size() != 0)
			{
				out << i << " ";
				for each (File File in hashTable.at(i))
				{
					out << File.GetPath() << " ";
				}
				out << "\n\n";
			}
		}
		out.close();
	}
};


void main()
{
    setlocale(LC_ALL, "Russian");
	std::vector<std::vector<File>> hashTable;
	hashTable.resize(65536);
	bool flag = 0;
	int i = 0;
	int j = 0;
	float a = 1.3;
	std::cout << "Choose a hash-func:\nSHA(0)\nMyHashFunc(1)\n";
	std::cin >> (bool)flag;
	system("cls");
	std::cout << "Write a Max-alpha for hash-table:\n";
	std::cin >> a;
    std::string startdir;
    std::cout << "Write a start directory:\n";
    std::cin >> startdir;
	system("cls");
	HashTable::MakeTable(hashTable, flag, startdir, 0, i, j, a);
	if(((float)i / j) > a)
	{
		std::cout << "\nAlpha more than Alpha-max. Starting re-build hash-table. \nPress any key!";
		getch();
		hashTable.clear();
		hashTable.resize(65536);
		flag = !flag;
		i = 0;
		j = 0;
		HashTable::MakeTable(hashTable, flag, startdir, 0, i, j, a);
	}
	if (((float)i / j) > a)
	{
		std::cout << "\nBoth function have a bad results. \nPress any key!";
		getch();
	}
	else
	{
		std::string path="";
		std::cout << "\nHash-table are ready! Save as:";
		std::cin >> path;
		HashTable::SaveTable(hashTable,path);
	}

    system("pause");
}
