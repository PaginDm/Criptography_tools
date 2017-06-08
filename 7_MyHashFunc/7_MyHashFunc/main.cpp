#include "iostream"
#include <string>
#include <conio.h>
#include <vector>

#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS

#define byte unsigned char 

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


class My_Hash
{
private:
    MyFile plaintext;
    MyFile Hash;
    std::string in_file_name;
    std::string out_file_name;
    std::vector<byte> hash_temp;
    int block_size=512;
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
    My_Hash()
        : in_file_name(""), out_file_name("")
    {
    }
    ~My_Hash()
    {
    }
    std::vector<byte> GetHashAsVector()
    {
        return Hash.GetData();
    }

    void SetBlockSize(int size)
    {
        block_size = size;
    }

    void GetHashForFile()
    {
        LoadFile(); 
        int count_of_blocks = plaintext.GetData().size() / block_size;
        int add_bytes = plaintext.GetData().size() % block_size;
        plaintext.GetData().resize(block_size*(count_of_blocks + 1));
        for (int i = count_of_blocks * block_size + add_bytes; i < (count_of_blocks + 1) * block_size; i++)
        {
            plaintext.GetData().at(i) = 0;
        }

        for (int i = 0; i < block_size; i++)
        {
            hash_temp.at(i) = plaintext.GetData().at(i);
        }

        for (int i = 1; i < count_of_blocks + 1; i++)
        {
            for (int j = 0; j < block_size; j++)
            {
                hash_temp.at(j) = hash_temp.at(j) ^ plaintext.GetData().at(i * block_size + j) ^ (hash_temp.at(j) >> (1) | hash_temp.at(j) << (7));
            }
        }

        Hash.GetData() = hash_temp;
        SaveHashAsFile(); 
    }

    void GetHashForVector(std::vector<byte> plaintext)
    {
        int count_of_blocks = plaintext.size() / block_size;
        int add_bytes = plaintext.size() % block_size;
		hash_temp.resize(block_size);
        plaintext.resize(block_size*(count_of_blocks + 1));
        for (int i = count_of_blocks * block_size + add_bytes; i < (count_of_blocks + 1) * block_size; i++)
        {
            plaintext.at(i) = 0;
        }
        for (int i = 0; i < block_size; i++)
        {
            hash_temp.at(i) = plaintext.at(i);
        }

        for (int i = 1; i < count_of_blocks + 1; i++)
        {
            for (int j = 0; j < block_size; j++)
            {
                hash_temp.at(j) = hash_temp.at(j) ^ plaintext.at(i * block_size + j) ^ (hash_temp.at(j) >> (1) | hash_temp.at(j) << (7));
            }
        }

        Hash.GetData() = hash_temp;
    }

};
void main()
{
    My_Hash hash;
	hash.GetHashForFile();
	/*MyFile Hash;
	MyFile plaintext;
	std::string out_file_name;
	std::string in_file_name;
	std::vector<byte> tmp;
    hash.GetHashForFile();
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
	tmp.resize(2048);
	hash.SetBlockSize(256);
	int count_of_blocks = plaintext.GetData().size() / 2048;
	int add_bytes = plaintext.GetData().size() % 2048;
	plaintext.GetData().resize(2048*(count_of_blocks + 1));
	for (int i = count_of_blocks * 2048 + add_bytes; i < (count_of_blocks + 1) * 2048; i++)
	{
		plaintext.GetData().at(i) = 0;
	}

	for (int i = 0; i < count_of_blocks + 1; i++)
	{
		for (int j = 0; j < 2048; j++)
		{
			tmp.at(j) = plaintext.GetData().at(i * 2048 + j);
		}
		hash.GetHashForVector(tmp);
		for each  (byte bit in hash.GetHashAsVector())
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
	_getch();*/

}