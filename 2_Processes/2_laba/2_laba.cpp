#include <windows.h>
#include <conio.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>


class Snapshot
{
private:
	std::vector<std::string> pid;
	std::vector<std::string> name;
public:
	Snapshot() : pid(NULL), name(NULL)
	{	}
    ~Snapshot()
    {
        pid.clear();
        name.clear();
    }
	std::vector<std::string> &GetPid() { return pid; }
	std::vector<std::string> &GetName() { return name; };

};



VOID CreateProcessList(HANDLE CONST hStdOut, Snapshot &MyProc) 
{
	PROCESSENTRY32 peProcessEntry;
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) 
    {
		return;
	}

	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &peProcessEntry);
	do 
    {
		char pid_tmp[10];
		sprintf(pid_tmp, "%x", peProcessEntry.th32ProcessID);
		char exe_tmp[sizeof(peProcessEntry.szExeFile)];
		wcstombs(exe_tmp, peProcessEntry.szExeFile, sizeof(peProcessEntry.szExeFile));
		std::string pid = std::string(pid_tmp);
		std::string exe = std::string(exe_tmp);
		MyProc.GetPid().push_back(pid);
		MyProc.GetName().push_back(exe);
	} while (Process32Next(hSnapshot, &peProcessEntry));
	CloseHandle(hSnapshot);
}



int main(int argc, char* argv[])
{
	HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	Snapshot first,second; 
    bool flag = true;
	CreateProcessList(hStdOut, first); 
	while (flag)
	{
        if (_kbhit())
        {
            flag = false;
        }
            Sleep(100);
            Snapshot second;
            CreateProcessList(hStdOut, second);
            for (int i = 0; i < first.GetPid().size(); i++)
            {
                if (find(second.GetPid().begin(), second.GetPid().end(), first.GetPid().at(i)) == second.GetPid().end()) //проверка на исчезновение процесса
                {
                    std::cout << "Process closed:\nID: " << first.GetPid().at(i) <<
                        "\nExecute File: " << first.GetName().at(i) << std::endl << std::endl;
                }
            }
            for (int i = 0; i < second.GetPid().size(); i++)
            {
                if (find(first.GetPid().begin(), first.GetPid().end(), second.GetPid().at(i)) == first.GetPid().end()) //проверка на появление процесса
                {
                    std::cout << "Process started:\nID: " << second.GetPid().at(i) <<
                        "\nExecute File: " << second.GetName().at(i) << std::endl << std::endl;
                }
            }
            first = second;
        
	}
	system("pause");
	return 0;
}