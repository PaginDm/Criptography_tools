#include "iostream"
#include <string>
#include <conio.h>
#include <vector>
#include <stdint.h>
#include "..\..\MyFile.h"
#include "..\..\zip.h"

#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS


class entropy
{
private:
	double intlog(double base, double x) 
	{
		return (log(x) / log(base));
	}
	MyFile ent;
public: 
	void OpenFile()
	{
		std::string in_file_name;
		std::cout << "Move a file at directory with project directory.\n";
		std::cout << "Filename for hashing:\n";
		std::cin >> in_file_name;
		if (!ent.Open("..\\..\\" + in_file_name))
		{
			std::cout << "error";
			system("pause");
			return;
		}
		system("cls");
	}

	void OpenFile(std::string in_file_name)
	{
		if (!ent.Open("..\\..\\" + in_file_name))
		{
			std::cout << "error";
			system("pause");
			return;
		}
		system("cls");
	}

	entropy()
	{

	}

	double GetEntropy8()
	{
		int a[256];
		double entropy = 0;
		for (int i = 0; i < 256; i++)
		{
			a[i] = 0;
		}
		uint8_t *pData = (uint8_t*)ent.GetData().data();
		for (int i = 0; i < ent.GetData().size(); i++)
		{
			a[(int)*pData]++;
			pData++;

		}

		for (int i = 0; i < 256; i++)
		{
			if (a[i] != 0)
				entropy -= (a[i] * 1.0) / ((int)ent.GetData().size()) * intlog(8, a[i] * 1.0 / ((int)ent.GetData().size()));
		}

		return entropy;
	}

	double GetEntropy16()
	{
		int a[65536];
		double entropy = 0;
		for (int i = 0; i < 65536; i++)
		{
			a[i] = 0;
		}
		uint16_t *pData = (uint16_t*)ent.GetData().data();
		for (int i = 0; i < ent.GetData().size()/2 ; i++)
		{
			a[(int)*pData]++;
			pData++;

		}

		for (int i = 0; i < 65536; i++)
		{
			if (a[i] != 0)
				entropy -= (a[i] * 1.0) / (ent.GetData().size() / 2) * intlog(16, a[i] * 1.0 / (ent.GetData().size() / 2));
		}

		return entropy;
	}
	double GetEntropy16_intersect()
	{
		int a[65536];
		double entropy = 0;
		for (int i = 0; i < 65536; i++)
		{
			a[i] = 0;
		}
		uint8_t *pData = (uint8_t*)ent.GetData().data();
		uint16_t *pData2 = (uint16_t*)ent.GetData().data();

		for (int i = 0; i < ent.GetData().size()-1; i++)
		{
			pData2 = (uint16_t*)pData;
			a[(int)*pData2]++;
			pData++;

		}

		for (int i = 0; i < 65536; i++)
		{
			if (a[i] != 0)
				entropy -= (a[i] * 1.0) / (ent.GetData().size()-1) * intlog(16, a[i] * 1.0 / (ent.GetData().size()-1));
		}

		return entropy;
	}


};


void Zip(TCHAR *archiveName, TCHAR *inArchiveFileName, TCHAR *sourceFile)
{
	HZIP hz = CreateZip(archiveName, 0);
	ZipAdd(hz, inArchiveFileName, sourceFile);
	CloseZip(hz);
}

void main()
{
	entropy ENT;
	entropy ENT2;
	entropy ENT3;
	ENT.OpenFile("myhash.txt");
	ENT2.OpenFile("sha_hash.txt");
	ENT3.OpenFile("test.docx");
	std::cout << "Entropy for MyHash 8 bit: "<< ENT.GetEntropy8();
	std::cout << "\nEntropy for MyHash 16 bit: " << ENT.GetEntropy16();
	std::cout << "\nEntropy for MyHash 16 bit with intersect: " << ENT.GetEntropy16_intersect() <<"\n";

	std::cout << "\nEntropy for ShaHash 8 bit: " << ENT2.GetEntropy8();
	std::cout << "\nEntropy for ShaHash 16 bit: " << ENT2.GetEntropy16();
	std::cout << "\nEntropy for ShaHash 16 bit with intersect: " << ENT2.GetEntropy16_intersect() << "\n";

	std::cout << "\nEntropy for PlainText 8 bit: " << ENT3.GetEntropy8();
	std::cout << "\nEntropy for PlainText 16 bit: " << ENT3.GetEntropy16();
	std::cout << "\nEntropy for PlainText 16 bit with intersect: " << ENT3.GetEntropy16_intersect() << "\n";


	Zip(L"..\\..\\plain.zip", L"test.docx", L"..\\..\\test.docx");
	Zip(L"..\\..\\myhash.zip", L"myhash.txt", L"..\\..\\myhash.txt");
	Zip(L"..\\..\\sha.zip", L"sha.txt", L"..\\..\\sha_hash.txt");

	std::cout << "\nFiles added to archive. Look at directory with project";
    system("pause");
}