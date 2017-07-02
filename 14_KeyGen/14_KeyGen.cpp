// keygen1.cpp: определяет точку входа для консольного приложения.
//

#include "iostream"
#include <string>
#include <conio.h>
#include <windows.h>
#include <vector>


int main()
{
    int v1;
    unsigned __int16 lov3 = 0; 
    unsigned __int16 hiv3 = 0; 
    unsigned __int16 v3 = 0; 
    int v4; 
    unsigned __int16 lov5 = 0; 
    unsigned __int16 hiv5 = 0; 
    __int16 v6; 
    std::string name2;
    std::cout << "Put a name:";
    std::cin >> name2;
    char *name = &name2[0];
    v1 = (int)name;
    std::vector<int> v2;
    lov3 = 0;
    hiv3 = 0;
    v4 = 0;
    while (*(BYTE *)v1)
    {
        lov3 = v4 ^ (v4 + *(BYTE *)v1);
        v6 = (lov3 + (hiv3 << 8)) % 0x1Au;
        lov5 = (lov3 + (hiv3 << 8)) / 0x1Au;
        hiv5 = v6;
        v3 = hiv5;
        lov3 = (v3 << 8) >> 8;
        hiv3 = (v3 >> 8);
        v2.push_back(v3 + 65);
        ++v1;
        ++v4;
    }
    std::cout << "Your serial:\n";
    for each (int c in v2)
    {
        std::cout << (char)c << " ";
    }
    system("pause");
}

