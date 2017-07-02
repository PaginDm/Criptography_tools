
#include "mpir.h"
#include "mpirxx.h"
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <winsock.h>
#include <sstream>


#define PORT 666 
#define SERVER "127.0.0.1" 

//#pragma comment(lib,"..\\..\\..\\third_party\\src\\mpir-2.7.2\\build.vc14\\lib_mpir_cxx\\Win32\\Debug\\mpirxx.lib")



int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bool is_Server = false;
    srand(time(0));
    std::stringstream ss;

    char buff[1024];
    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
    printf("Hi, I'am Alice =)\n");
    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));

    WSAStartup(0x0202, (WSADATA *)&buff[0]);

    SOCKET mysocket;
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    HOSTENT *hst;

    if (inet_addr(SERVER) != INADDR_NONE)
    {
        dest_addr.sin_addr.s_addr = inet_addr(SERVER);
    }
    else
    {
        if (hst = gethostbyname(SERVER))
        {
            ((unsigned long *)&dest_addr.sin_addr)[0] = ((unsigned long **)hst->h_addr_list)[0][0];
        }
    }

    if (connect(mysocket, (sockaddr *)&dest_addr, sizeof(dest_addr)))
    {
        std::cout << "Bob is offline =(" << std::endl;
        std::cout << "Waiting for Bob...\n" << std::endl;
        is_Server = true;
    }

    if (!is_Server)
    {
        std::cout << std::endl;
        gmp_randstate_t state;
        mp_bitcnt_t init = rand() % 100;
        gmp_randinit_lc_2exp_size(state, init);
        int nsize;
        //---
        nsize = recv(mysocket, &buff[0], sizeof(buff), 0);
        mpz_t p;
        mpz_init_set_str(p, buff, 10);
        std::cout << "p: " << p << std::endl;

        nsize = recv(mysocket, &buff[0], sizeof(buff), 0);
        mpz_t n;
        mpz_init_set_str(n, buff, 10);
        std::cout << "N: " << n << std::endl;
        //---
        mpz_t B;
        std::cout << "Get B from Bob" << std::endl;
        nsize = recv(mysocket, &buff[0], sizeof(buff), 0);
        mpz_init_set_str(B, buff, 10);
        std::cout << "B: " << B << std::endl;
        //---
        mpz_t a;
        mpz_init(a);
        mpz_urandomb(a, state, 64);
        mpz_t A;
        mpz_init(A);
        mpz_powm(A, p, a, n);

        ss.str(std::string());
        ss << A;
        std::cout << "Send A to Bob" << std::endl;
        send(mysocket, ss.str().c_str(), sizeof(ss.str()), 0);
        std::cout << "A: " << A << std::endl;
        //---
        mpz_t KA;
        mpz_init(KA);
        mpz_powm(KA, B, a, n);
        SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
        std::cout << "Our key is: " << KA << std::endl;
        SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));

        system("pause");

    }
    else
    {
        SOCKET mysocket2;
        mysocket2 = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in local_addr;
        local_addr.sin_family = AF_INET;
        local_addr.sin_port = htons(PORT);
        local_addr.sin_addr.s_addr = 0;

        bind(mysocket2, (sockaddr *)&local_addr, sizeof(local_addr));

        listen(mysocket2, 0x100);

        SOCKET client_socket;
        sockaddr_in client_addr;

        int client_addr_size = sizeof(client_addr);
        int nsize;

        while ((client_socket = accept(mysocket2, (sockaddr *)&client_addr, &client_addr_size)))
        {
            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
            printf("Bob is here!\n");
            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));

            gmp_randstate_t state;
            mp_bitcnt_t init = rand() % 100;
            gmp_randinit_lc_2exp_size(state, init);

            mpz_t p;
            mpz_t n;
            mpz_t a;
            mpz_init(p);
            mpz_init(n);
            mpz_init(a);

            char buff[20 * 1024];
            //---
            mpz_urandomb(p, state, 64);
            mpz_urandomb(n, state, 64);

            ss.str(std::string());
            ss << p;
            std::cout << "p: " << ss.str() << std::endl;
            send(client_socket, ss.str().c_str(), sizeof(ss.str()), 0);

            ss.str(std::string());
            ss << n;
            std::cout << "N: " << ss.str() << std::endl;
            send(client_socket, ss.str().c_str(), sizeof(ss.str()), 0);
            //---
            mpz_urandomb(a, state, 64);
            mpz_t A;
            mpz_init(A);
            mpz_powm(A, p, a, n);

            ss.str(std::string());
            ss << A;
            std::cout << "Sending A to Bob" << std::endl;
            send(client_socket, ss.str().c_str(), sizeof(ss.str()), 0);
            std::cout << "A: " << ss.str() << std::endl;
            //---
            mpz_t B;
            nsize = recv(client_socket, &buff[0], sizeof(buff), 0);
            mpz_init_set_str(B, buff, 10);
            std::cout << "Get B from Bob: " << B << std::endl;
            //---
            mpz_t KA;
            mpz_init(KA);
            mpz_powm(KA, B, a, n);
            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
            std::cout << "Our key is: " << KA << std::endl;
            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
            //---
            system("pause");
        }
    }
}