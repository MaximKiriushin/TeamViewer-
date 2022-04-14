#pragma once 

#include "rsaEncryption.h"

std::pair <std::pair <unsigned int, unsigned int>, std::pair <unsigned int, unsigned int>> createKeys()
{
	std::pair <unsigned int, unsigned int> primes;
	std::pair <std::pair <unsigned int, unsigned int>, std::pair <unsigned int, unsigned int>> keys;
	primes = choosePrimes();
	keys.first = createPublicKey(primes.first, primes.second);
	keys.second.first = createPrivateKey(keys.first, eulersTotient(primes));
	keys.second.second = keys.first.first;
	return keys;
}

std::pair <unsigned int, unsigned int> createPublicKey(unsigned int p, unsigned int q)
{
	std::pair <unsigned int, unsigned int> key;
	unsigned int n;
	unsigned int phi;
	unsigned int e;

	n = p * q;
	phi = eulersTotient(std::pair <unsigned int, unsigned int>(p, q));
	e = smallExponent(phi);

	key.first = n;
	key.second = e;

	return key;
}

unsigned int createPrivateKey(std::pair <unsigned int, unsigned int> publicKey, unsigned int phi)
{
	unsigned int i = 1;
	unsigned int d = 0;

	d = 1 + i * phi;

	while (d % publicKey.second != 0)
	{
		i++;
		d = 1 + i * phi;
	}
	d = d / publicKey.second;

	return d;
}

unsigned int eulersTotient(std::pair <unsigned int, unsigned int> primes)
{
	unsigned int phi;
	phi = (primes.first - 1) * (primes.second - 1);
	return phi;
}

unsigned int smallExponent(unsigned int phi)
{
	unsigned int e = 0;
	bool found = false;
	for (e = 1; e < phi && !found; e++)
	{
		if (gcd(phi, e + 1) == 1)
		{
			found = true;
		}
	}
	return e;
}

unsigned int gcd(unsigned int u, unsigned int v)
{
	if (v > 0)
	{
		gcd(v, u % v);
	}
	else if (v == 0)
	{
		return u;
	}
}

std::string encrypt(std::pair<unsigned int, unsigned int> publicKey, char* buff, unsigned int length)
{
	std::string encMsg = "";
	checked_uint_t enc = 0;
	unsigned int encChunkSize = std::to_string(publicKey.first).length();
	char* encBuff = new char[(length * encChunkSize) + 1];
	char* encChunck = new char[encChunkSize];
	memset(encBuff, 0, (length * encChunkSize) + 1);

	for (int i = 0; i < length; i++)
	{
		enc = charToInt(buff[i]);
		enc = bmp::pow(enc, publicKey.second) % publicKey.first;
		zeroPadding(enc, encChunkSize, &encChunck);
		copyCharStrOffset(encChunck, encBuff, encChunkSize, i * encChunkSize);
	}
	encMsg = std::string(encBuff);
	delete[]  encBuff;
	delete[] encChunck;
	return encMsg;
}

std::string decrypt(std::pair<unsigned int, unsigned int> privateKey, char* buff, unsigned int length)
{
	std::string encBuff(buff);
	std::string decMsg = "";
	checked_uint_t dec = 0;
	unsigned int encChunkSize = std::to_string(privateKey.second).length();
	unsigned int chunkAmount = encBuff.length() / encChunkSize;
	std::string encPart = "";
	for (int i = 0; i < chunkAmount; i++)
	{
		encPart = encBuff.substr(i * encChunkSize, encChunkSize);
		dec = atoi(encPart.c_str());
		dec = bmp::pow(dec, privateKey.first) % privateKey.second;
		decMsg += char(dec.convert_to<int>());
	}
	return decMsg;
}

int charToInt(char ch)
{
	return ch;
}

void zeroPadding(checked_uint_t msg, int sizeOfPadding, char** buff)
{
	std::string msgStr = toString(msg);
	std::string padding(size_t(sizeOfPadding) - msgStr.size(), '0');
	*buff = new char[sizeOfPadding];
	copyCharStr((padding + msgStr).c_str(), *buff, sizeOfPadding);
}

void copyCharStr(const char* src, char* dst, int size)
{
	for (int i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}
}

void copyCharStrOffset(const char* src, char* dst, int size, int offset)
{
	for (int i = 0; i < size; i++)
	{
		dst[i + offset] = src[i];
	}
}

void recevCharStrOffset(const char* src, char* dst, int size, int offset)
{
	for (int i = 0; i < size; i++)
	{
		dst[i] = src[i + offset];
	}
}

unsigned int digitCount(checked_uint_t num)
{
	unsigned int digitCount = 0;
	while (num != 0)
	{
		num = num / 10;
		digitCount++;
	}
	return digitCount;
}

std::string toString(checked_uint_t num)
{
	int digit = 0;
	std::string numStringReversed = "";
	std::string numString = "";
	while (num != 0)
	{
		digit = (num % 10).convert_to<int>();
		numStringReversed = numStringReversed + std::to_string(digit);
		num = num / 10;
	}
	for (int i = numStringReversed.length() - 1; i >= 0; i--)
	{
		numString += numStringReversed.at(i);
	}
	return numString;
}

std::queue<unsigned int> SieveofEratosthenes(unsigned int n)
{
	unsigned int i = 0;
	unsigned int j = 0;
	bool* sieve = new bool[n + 1];
	std::queue<unsigned int> primes;
	for (i = 0; i < n + 1; i++)
	{
		sieve[i] = false;
	}
	for (i = 2; i < n + 1; i++)
	{
		if (!sieve[i])
		{
			primes.push(i);
		}
		for (j = i; j < n + 1; j += i)
		{
			sieve[j] = true;
		}
	}
	delete[] sieve;
	return primes;
}

std::pair<unsigned int, unsigned int> choosePrimes()
{
	std::pair<unsigned int, unsigned int> chosenPrimes;
	std::pair<unsigned int, unsigned int> primesIndex;
	std::queue<unsigned int> primes;
	unsigned int i = 0;

	//initialize random seed
	srand(time(NULL));

	//get all the prime in a pre determined range
	primes = SieveofEratosthenes(PRIME_RANGE);

	//choose two random indexes of primes
	primesIndex.first = rand() % (int)primes.size();
	primesIndex.second = rand() % (int)primes.size();

	//check if the indexes are the same
	while (primesIndex.first == primesIndex.second)
	{
		primesIndex.second = rand() % (int)primes.size();
	}

	//sort the indexes by small to big
	if (primesIndex.first > primesIndex.second)
	{
		primesIndex.second += primesIndex.first;
		primesIndex.first = primesIndex.second - primesIndex.first;
		primesIndex.second -= primesIndex.first;
	}

	//get the primes value from the queue by index
	for (i = 0; i <= primesIndex.second; i++)
	{
		if (i == primesIndex.first)
		{
			chosenPrimes.first = primes.front();
		}
		if (i == primesIndex.second)
		{
			chosenPrimes.second = primes.front();
		}
		primes.pop();
	}

	return chosenPrimes;
}

std::string rsaKeySendMsg(std::pair <unsigned int, unsigned int> key)
{
	std::string msg = "";
	char* codeBuff = nullptr;
	char* lengthBuff = nullptr;
	char* nBuff = nullptr;
	char* eBuff = nullptr;
	char* msgBuff = nullptr;

	msgBuff = new char[MSG_CODE + MSG_SIZE + RSA_KEY_N_SIZE + RSA_KEY_E_SIZE + STRING_IDENTIFIER];

	ZeroMemory(msgBuff, MSG_CODE + MSG_SIZE + RSA_KEY_N_SIZE + RSA_KEY_E_SIZE + STRING_IDENTIFIER);

	zeroPadding(RSA_KEY_CODE, MSG_CODE, &codeBuff);
	zeroPadding(RSA_KEY_N_SIZE + RSA_KEY_E_SIZE, MSG_SIZE, &lengthBuff);
	zeroPadding(key.first, RSA_KEY_N_SIZE, &nBuff);
	zeroPadding(key.second, RSA_KEY_E_SIZE, &eBuff);

	copyCharStr(codeBuff, msgBuff, MSG_CODE);
	copyCharStrOffset(lengthBuff, msgBuff, MSG_SIZE, MSG_CODE);
	copyCharStrOffset(nBuff, msgBuff, RSA_KEY_N_SIZE, MSG_CODE + MSG_SIZE);
	copyCharStrOffset(eBuff, msgBuff, RSA_KEY_E_SIZE, MSG_CODE + MSG_SIZE + RSA_KEY_N_SIZE);

	msg = std::string(msgBuff);

	delete[] codeBuff;
	delete[] lengthBuff;
	delete[] nBuff;
	delete[] eBuff;
	delete[] msgBuff;
	return msg;
}

RsaKey rsaKeyReciveMsg(std::string msg)
{
	RsaKey key;

	char nBuff[RSA_KEY_N_SIZE + STRING_IDENTIFIER] = { 0 };
	char eBuff[RSA_KEY_E_SIZE + STRING_IDENTIFIER] = { 0 };
	const char* msgBuff = msg.c_str();

	
	recevCharStrOffset(msgBuff, nBuff, RSA_KEY_N_SIZE, 0);
	recevCharStrOffset(msgBuff, eBuff, RSA_KEY_E_SIZE, RSA_KEY_N_SIZE);

	key.first = atoi(nBuff);
	key.second = atoi(eBuff);
	return key;
}

char* stringToChar(std::string str)
{
	char* msgBuff = new char[str.length() + STRING_IDENTIFIER];
	ZeroMemory(msgBuff, str.length() + STRING_IDENTIFIER);
	copyCharStr(str.c_str(), msgBuff, str.length());
	return msgBuff;
}

std::string zeroPaddingStr(std::string msg)
{
	std::string padding(size_t(10) - msg.size(), '0');
	return padding + msg;

}

std::string buildingMessage(std::string msg, int code)
{
	return std::to_string(code) + zeroPaddingStr(std::to_string(msg.length())) + msg;
}
