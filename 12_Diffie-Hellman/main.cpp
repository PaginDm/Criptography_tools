#include "mpir.h"
#include "mpirxx.h"
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <winsock.h>
#include <sstream>
#include <vector>
#include "cryptopp565\des.h"
#include <cryptopp565\base64.h>
#include "cryptopp565\cryptlib.h"
#include "cryptopp565\modes.h"
#include "cryptopp565\osrng.h"
#include "cryptopp565\filters.h"
#include "cryptopp565\cbcmac.h"


#define PORT 1003
#define SERVER "127.0.0.1" 


std::string Encrypt(std::string &plaintext, std::string &key)
{
    std::vector<byte> v_plaintext;
    std::vector<byte> ciphertext;
    std::string s_ciphertext="";
    byte key2[CryptoPP::DES::DEFAULT_KEYLENGTH];

    for (int i = 0; i < CryptoPP::DES::DEFAULT_KEYLENGTH; i++)
    {
        key2[i] = key[i];
    }
    for each  (byte ch in plaintext)
    {
        v_plaintext.push_back(ch);
    }
    ciphertext.resize(v_plaintext.size() + CryptoPP::DES::BLOCKSIZE);
    CryptoPP::ArraySink cs(&ciphertext[0], ciphertext.size());
    CryptoPP::ECB_Mode<CryptoPP::DES>::Encryption e(key2, sizeof(key2));
    CryptoPP::ArraySource(v_plaintext.data(), v_plaintext.size(), true,
        new CryptoPP::StreamTransformationFilter(e,
        new CryptoPP::Redirector(cs), CryptoPP::StreamTransformationFilter::DEFAULT_PADDING));
    ciphertext.resize(cs.TotalPutLength());
    for each  (byte ch in ciphertext)
    {
        s_ciphertext+=ch;
    }

    return s_ciphertext;
}

std::string Decrypt(std::string &ciphertext, std::string &key)
{
    std::vector<byte> v_ciphertext;
    std::vector<byte> plaintext;
    std::string s_plaintext = "";
    for each  (byte ch in ciphertext)
    {
        v_ciphertext.push_back(ch);
    }
    byte key2[CryptoPP::DES::DEFAULT_KEYLENGTH];

    for (int i = 0; i < CryptoPP::DES::DEFAULT_KEYLENGTH; i++)
    {
        key2[i] = key[i];
    }
    plaintext.resize(v_ciphertext.size() + CryptoPP::DES::BLOCKSIZE);
    CryptoPP::ArraySink ds(&plaintext[0], plaintext.size());
    CryptoPP::ECB_Mode<CryptoPP::DES>::Decryption d(key2, sizeof(key2));
    CryptoPP::ArraySource(v_ciphertext.data(), v_ciphertext.size(), true,
        new CryptoPP::StreamTransformationFilter(d,
        new CryptoPP::Redirector(ds), CryptoPP::StreamTransformationFilter::DEFAULT_PADDING));
    plaintext.resize(ds.TotalPutLength());

    for each  (byte ch in plaintext)
    {
        s_plaintext += ch;
    }
    return s_plaintext;
}


int main(int argc, char* argv[])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bool is_Server = false;
    srand(time(0));
    std::string name = "someboby";
    if (argc > 1)
    {
        name = argv[1];
    }
    std::stringstream ss;
    char buff[1024];
    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
    std::cout << "Hello! I'm "<< name <<"\n";
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
        std::cout << "Waiting for my friend" << std::endl;
        is_Server = true;
    }

    if (!is_Server)
    {
        std::cout << std::endl;
        gmp_randstate_t state;
        mp_bitcnt_t init = rand() % 100;
        gmp_randinit_lc_2exp_size(state, init);
        int count;

        count = recv(mysocket, &buff[0], sizeof(buff), 0);
        mpz_t p;
        mpz_init_set_str(p, buff, 10);
        std::cout << "p: " << p << std::endl;
        

        count = recv(mysocket, &buff[0], sizeof(buff), 0);
        mpz_t n;
        mpz_init_set_str(n, buff, 10);
        std::cout << "N: " << n << std::endl;

        mpz_t B;
        std::cout << "Get B from my friend" << std::endl;
        count = recv(mysocket, &buff[0], sizeof(buff), 0);
        mpz_init_set_str(B, buff, 10);
        std::cout << "B: " << B << std::endl;

        mpz_t a;
        mpz_init(a);
        mpz_urandomb(a, state, 64);
        mpz_t A;
        mpz_init(A);

        mpz_powm(A, p, a, n);

        ss.str(std::string());
        ss << A;
        std::cout << "Send A to my friend" << std::endl;
        send(mysocket, ss.str().c_str(), sizeof(ss.str()), 0);
        std::cout << "A: " << A << std::endl;

        mpz_t KA;
        mpz_init(KA);
        mpz_powm(KA, B, a, n);
        char key[8];
        mpz_get_str(key, 10, KA);
        std::string key_str = "";
        for (int i = 0; i < CryptoPP::DES::DEFAULT_KEYLENGTH; i++)
        {
            key_str += (byte)key[i];
        }

        std::cout << "Our key is: " << KA << std::endl;

        std::string ciphertext="";
        count = recv(mysocket, &buff[0], sizeof(buff), 0);

        for (int i = 0; i < count; i++)
            ciphertext += (byte)buff[i];
        //ciphertext += '\0';
        std::string plaintext = Decrypt(ciphertext, key_str);

        std::cout << "Name of my friend is: " << plaintext << std::endl;
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
        int count;

        while ((client_socket = accept(mysocket2, (sockaddr *)&client_addr, &client_addr_size)))
        {
            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
            printf("My friend is here!\n");
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
            mpz_urandomb(a, state, 64);
            mpz_t A;
            mpz_init(A);
            mpz_powm(A, p, a, n);

            ss.str(std::string());
            ss << A;
            std::cout << "Sending A to my friend" << std::endl;
            send(client_socket, ss.str().c_str(), sizeof(ss.str()), 0);
            std::cout << "A: " << ss.str() << std::endl;
            mpz_t B;
            count = recv(client_socket, &buff[0], sizeof(buff), 0);
            mpz_init_set_str(B, buff, 10);
            std::cout << "Get B from my friend: " << B << std::endl;

            mpz_t KA;
            mpz_init(KA);
            mpz_powm(KA, B, a, n);

            std::cout << "Our key is: " << KA << std::endl;
            std::cout <<"I send my name" << std::endl;
            char key[8];
            mpz_get_str(key, 10, KA);
            std::string key_str = "";
            for (int i = 0; i < CryptoPP::DES::DEFAULT_KEYLENGTH; i++)
            {
                key_str += (byte)key[i];
            }

            std::string ciphertext = Encrypt(name, key_str);
            send(client_socket, ciphertext.c_str(), ciphertext.size(), 0);


            closesocket(client_socket);
            system("pause");
        }
    }

    closesocket(mysocket);
    WSACleanup();

}