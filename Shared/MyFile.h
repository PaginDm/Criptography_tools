#ifndef _MYFILE_H_
#define _MYFILE_H_
#include <vector>
typedef unsigned char byte;

class MyFile
{
private:
    std::vector<byte> _data;
    FILE* _file;

public:
    std::vector<byte> &GetData();
    MyFile();
    bool Open(std::string name);
    bool Write(std::string name);

};
#endif