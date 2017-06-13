#include "iostream"
#include <string>
#include <conio.h>
#include <vector>
#include <stdint.h>
#include "..\..\MyFile.h"

#pragma warning(disable : 4996)
#define _CRT_NO_WARNINGS


class entropy
{
private:
	double intlog(double base, double x) {
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
		for (int i = 0; i < ent.GetData().size() / 2; i++)
		{
			a[(int)*pData]++;
			pData++;

		}

		for (int i = 0; i < 65536; i++)
		{
			if (a[i] != 0)
				entropy -= (a[i] * 1.0) / ((int)ent.GetData().size() / 2) * intlog(16, a[i] * 1.0 / (ent.GetData().size() / 2));
		}

		return entropy;
	}


	double GetEntropy32()
	{
		uint16_t **a = new uint16_t*[65536];
		for (int i = 0; i < 65536; i++)
			a[i] = new uint16_t[65536];

		double entropy = 0;
		for (int i = 0; i < 65536; i++)
		{
			for(int j = 0;j<65536;j++)
				a[i][j] = 0;
		}
		uint32_t *pData = (uint32_t*)ent.GetData().data();
		for (int i = 0; i < ent.GetData().size() / 4; i++)
		{
			a[(uint32_t)*pData%65536][(uint32_t)*pData/65536]++;
			pData++;
		}

		for (int i = 0; i < 65536; i++)
		{
			for (int j = 0; j<65536; j++)
				if (a[i][j] != 0)
					entropy -= (a[i][j] * 1.0) / ((int)ent.GetData().size() / 4) * intlog(32, a[i][j] * 1.0 / (ent.GetData().size() / 4));
		}

		return entropy;

	}

};



void main()
{

	entropy ENT;
	ENT.OpenFile();
	std::cout << "Entopy 8 bit: "<< ENT.GetEntropy8();
	std::cout << "\nEntopy 16 bit: " << ENT.GetEntropy16();
	std::cout << "\nEntopy 32 bit: " << ENT.GetEntropy32();
    system("pause");
}