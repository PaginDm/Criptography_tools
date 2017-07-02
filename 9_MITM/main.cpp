#include "iostream"
#include <windows.h>
#include <string>
#include <conio.h>
#include <map>   
#include <stdint.h>
#include <ctime>
#include <thread>
#include <vector>
#include "cryptopp565\des.h"
#include "cryptopp565\cryptlib.h"
#include "cryptopp565\modes.h"
#include "cryptopp565\osrng.h"
#include "cryptopp565\filters.h"
#include "cryptopp565\cbcmac.h"




class MITM
{

public:
    static std::vector<byte> Encrypt(std::vector<byte> &plaintext, uint64_t &key)
    {
        std::vector<byte> ciphertext;
        ciphertext.resize(plaintext.size() + CryptoPP::DES::BLOCKSIZE);
        CryptoPP::ArraySink cs(&ciphertext[0], ciphertext.size());
        CryptoPP::ArraySource(plaintext.data(), plaintext.size(), true,
            new CryptoPP::StreamTransformationFilter(CryptoPP::ECB_Mode<CryptoPP::DES>::Encryption((byte *)&key, 8),
            new CryptoPP::Redirector(cs)));
        ciphertext.resize(cs.TotalPutLength());

        return ciphertext;
    }


    static std::vector<byte> Decrypt(std::vector<byte> &ciphertext, uint64_t &key)
    {
        std::vector<byte> plaintext;
        plaintext.resize(ciphertext.size() + CryptoPP::DES::BLOCKSIZE);
        CryptoPP::ArraySink ds(&plaintext[0], plaintext.size());
        CryptoPP::ArraySource(ciphertext.data(), ciphertext.size(), true,
            new CryptoPP::StreamTransformationFilter(CryptoPP::ECB_Mode<CryptoPP::DES>::Decryption((byte *)&key, 8),
            new CryptoPP::Redirector(ds)));
        plaintext.resize(ds.TotalPutLength());

        return plaintext;
    }

    static bool summ(uint64_t &part1of1, uint64_t &part2of1, uint64_t part1of2, uint64_t part2of2)
    {
        uint64_t b2 = part2of1 + part2of2;
        uint64_t b1 = part1of1 + part1of2;
        if (b2 < part2of1)
        {
            b1++;
        }
        if (b1 < part1of1)
        {
            throw std::overflow_error("key overflow");
        }
        part1of1 = b1;
        part2of1 = b2;
        return true;
    }

    static void print_v(std::vector<byte> &out)
    {
        for (int j = 0; j < out.size(); j++)
        {
            std::cout << out.at(j);
        }
        std::cout << std::endl;
    }


    static void CreateTable(uint64_t &start, uint64_t &end, std::vector<byte> &plain_v, std::vector<byte> &cipher_v, uint64_t key1, uint64_t key2, std::map <std::vector<byte>, uint64_t> &e, std::map <std::vector<byte>, uint64_t> &d)
    {
        MITM::summ(key1, key2, 0, start);
        for (int i = start; i < end; i++)
        {
            e.insert(std::pair <std::vector<byte>, uint64_t >(MITM::Encrypt(plain_v, key2), key2));
            d.insert(std::pair <std::vector<byte>, uint64_t >(MITM::Decrypt(cipher_v, key1), key1));
            MITM::summ(key1, key2, 0, 1);
        }
    }


};



int main()
{
    uint64_t shift_max = 65536;
	uint64_t start_key_part1;
	uint64_t start_key_part2;
	uint64_t shift;
	uint64_t my_key1;
	uint64_t my_key2;
	byte plain[] = "I'm very love a cryptography!";
	std::vector<byte> plain_v;
	std::vector<byte> cipher_v;
	std::vector<byte> decrypt_v;

	CryptoPP::AutoSeededRandomPool rand(true);
    rand.GenerateBlock((byte *)&start_key_part1, 8);
    rand.GenerateBlock((byte *)&start_key_part2, 8);
	rand.GenerateBlock((byte *)&shift, 8);
	shift = shift % (shift_max + 1);

    my_key1 = start_key_part1;
    my_key2 = start_key_part2;

	MITM::summ(my_key1, my_key2, 0, shift);

	plain_v.resize(sizeof(plain));

	memcpy(plain_v.data(), plain, sizeof(plain));

    cipher_v = MITM::Encrypt(MITM::Encrypt(plain_v, my_key2), my_key1);
    decrypt_v = MITM::Decrypt(MITM::Decrypt(cipher_v, my_key1), my_key2);
    std::cout << "Plaintext:" << std::endl;
    MITM::print_v(plain_v);
    std::cout << "Ciphertext" << std::endl;
    MITM::print_v(cipher_v);

    std::cout << "Start key"<<  std::endl;
	std::cout << std::hex << my_key1 << std::endl << my_key2 << std::endl;


	std::map <std::vector<byte>, uint64_t> d1;
	std::map <std::vector<byte>, uint64_t> e1;
    std::map <std::vector<byte>, uint64_t> d2;
    std::map <std::vector<byte>, uint64_t> e2;
    std::map <std::vector<byte>, uint64_t> d3;
    std::map <std::vector<byte>, uint64_t> e3;
    std::map <std::vector<byte>, uint64_t> d4;
    std::map <std::vector<byte>, uint64_t> e4;
    uint64_t key1 = start_key_part1;
    uint64_t key2 = start_key_part2;

    std::thread Thread1(MITM::CreateTable, (uint64_t)0, shift_max/4, plain_v, cipher_v, key1, key2, std::ref(e1), std::ref(d1));
    std::thread Thread2(MITM::CreateTable, shift_max/4, shift_max/2, plain_v, cipher_v, key1, key2, std::ref(e2), std::ref(d2));
    std::thread Thread3(MITM::CreateTable, shift_max/2, shift_max/4*3, plain_v, cipher_v, key1, key2, std::ref(e3), std::ref(d3));
    std::thread Thread4(MITM::CreateTable, shift_max / 4 * 3, shift_max, plain_v, cipher_v, key1, key2, std::ref(e4), std::ref(d4));

    std::cout << "Create tables...\n";
    Thread1.join();
    Thread2.join();
    Thread3.join();
    Thread4.join();

	std::cout << std::endl;

    std::vector<std::map <std::vector<byte>, uint64_t>> e;
    std::vector<std::map <std::vector<byte>, uint64_t>> d;
    e.push_back(e1);
    e.push_back(e2);
    e.push_back(e3);
    e.push_back(e4);
    d.push_back(d1);
    d.push_back(d2);
    d.push_back(d3);
    d.push_back(d4);

    uint64_t pair[2] = {NULL,NULL};
    for each (std::map <std::vector<byte>, uint64_t> dd in d)
    {
        for each(std::map <std::vector<byte>, uint64_t> ee in e)
        {
            if ((ee.find((dd.begin())->first)) != ee.end())
            {
                pair[0] = (ee.find((dd.begin())->first))->second;
                pair[1] = (dd.begin())->second;
                break;
            }
        }
        if (pair[0] != NULL)
            break;
    }
    std::cout << "Find key" << std::endl;
    std::cout << std::hex << pair[1];
    std::cout << std::endl;
    std::cout << std::hex << pair[0];
    std::cout << std::endl;

	decrypt_v.clear();
    decrypt_v = MITM::Decrypt(MITM::Decrypt(cipher_v, pair[1]), pair[0]);
    std::cout << "Decrypted text:" << std::endl;
    MITM::print_v(decrypt_v);

	std::cout << std::endl;
	system("pause");
	return 0;

}