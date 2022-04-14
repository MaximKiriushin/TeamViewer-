#pragma once

#include <queue>
#include <string>
#include <list>
#include <time.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <windows.h>

//#define INT_SIZE 100000
#define INT_SIZE 2000000
#define PRIME_RANGE 50

#define MSG_CODE 0x03
#define MSG_SIZE 0x0A
#define STRING_IDENTIFIER 0x01
#define ENC_KEY 0xE6

#define RSA_KEY_CODE 0xD0

#define RSA_KEY_N_SIZE 0x0A
#define RSA_KEY_E_SIZE 0x0A


namespace bmp = boost::multiprecision;
typedef bmp::number <bmp::cpp_int_backend<INT_SIZE, INT_SIZE, bmp::unsigned_magnitude, bmp::checked, void> > checked_uint_t;
typedef std::pair <std::pair <unsigned int, unsigned int>, std::pair <unsigned int, unsigned int>> RsaKeys;
typedef std::pair <unsigned int, unsigned int> RsaKey;

RsaKeys createKeys();
std::pair <unsigned int, unsigned int> createPublicKey(unsigned int p, unsigned int q);
unsigned int createPrivateKey(std::pair <unsigned int, unsigned int> publicKey, unsigned int phi);
std::queue<unsigned int> SieveofEratosthenes(unsigned int n);
std::pair<unsigned int, unsigned int> choosePrimes();

unsigned int eulersTotient(std::pair <unsigned int, unsigned int> primes);
unsigned int smallExponent(unsigned int phi);
unsigned int gcd(unsigned int u, unsigned int v);

std::string encrypt(std::pair <unsigned int, unsigned int> publicKey, char* buff, unsigned int length);
std::string decrypt(std::pair <unsigned int, unsigned int> privateKey, char* buff, unsigned int length);
int charToInt(char ch);
void zeroPadding(checked_uint_t msg, int sizeOfPadding, char** buff);
void copyCharStr(const char* src, char* dst, int size);
void copyCharStrOffset(const char* src, char* dst, int size, int offset);
void recevCharStrOffset(const char* src, char* dst, int size, int offset);
unsigned int digitCount(checked_uint_t num);
std::string toString(checked_uint_t num);
std::string rsaKeySendMsg(std::pair <unsigned int, unsigned int> key);
RsaKey rsaKeyReciveMsg(std::string msg);
char* stringToChar(std::string str);
std::string zeroPaddingStr(std::string msg);
std::string buildingMessage(std::string msg, int code);