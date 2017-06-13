#include "iostream"
#include<windows.h>
#include <string>
#include <conio.h>
#include <map>   
#include <ctime>
#include <vector>


#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS

class File 
{
private:
    File *next; // ѕри возникновении коллизии элементы будут помещены в односв€зный список.
    std::string path;
    std::string altername;
    std::string atributes;
    std::string creationTime;
    std::string lastAccessTime;
    std::string size;

public:


    File()
    {
        this->next = NULL;
    }

    File(std::string path,std::string altername,std::string atributes,std::string creationTime,std::string lastAccessTime,std::string size)
    {
        this->path = path;
        this->altername = altername;
        this->atributes = atributes;
        this->creationTime = creationTime;
        this->lastAccessTime = lastAccessTime;
        this->size = size;
        this->next = NULL;
    }

    ~File()
    {
        if (this->next != NULL)
        {
            delete this->next;
        }
    }
};


class SHA
{
private:
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
    setlocale(LC_ALL, "Russian");

    std::map<std::string, std::map<std::string, File>> HashTable;

    WIN32_FIND_DATAA findData;
    HANDLE hf;
    hf = FindFirstFileA("D:\\*", &findData); 
    std::string path;
    std::string altername;
    std::string atributes;
    std::string creationTime;
    std::string lastAccessTime;
    std::string size;

    if (hf == INVALID_HANDLE_VALUE)
    {
        std::cout << "Cannot find file" << std::endl;
        return ;
    }
    int k = 0;
    do
    {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            std::cout << k << ' ' << "[DIR] ";
            k++;
        }
        sprintf(path.c_str, "%d", findData.cFileName);
        sprintf(altername.c_str, "%d", findData.cAlternateFileName);
        sprintf(atributes.c_str, "%d", findData.dwFileAttributes);
        sprintf(creationTime.c_str, "%d", findData.ftCreationTime.dwLowDateTime);
        sprintf(lastAccessTime.c_str, "%d", findData.ftLastAccessTime.dwLowDateTime);
        sprintf(size.c_str, "%d", findData.nFileSizeLow);

        File *file = new File(path, altername, atributes, creationTime, lastAccessTime, size);


    } while (FindNextFileA(hf, &findData));
    std::cout << k << std::endl;
    getch();
    FindClose(hf);

    system("pause");
}

//http://www.cyberforum.ru/cpp-beginners/thread284051.html

//https://ru.stackoverflow.com/questions/265627/%D0%9E%D1%86%D0%B5%D0%BD%D0%B8%D1%82%D0%B5-%D1%80%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D1%8E-%D1%85%D0%B5%D1%88-%D1%82%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D1%8B