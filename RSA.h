#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "ModularArithmetic.h"

using namespace std;

class RSA {
	private:
	//My opinion is vector is better storage for keys (no leaks of memory)
	vector<mpz_class> pubKey, privKey;
	void init(int n);

	public:
	RSA(int n);
	RSA(int n, string privKeyFilename, string pubKeyFilename);
	RSA(string privKeyFilename, string pubKeyFilename);
	RSA(string pubKeyFilename);
	//mpz_class* RSAkeys(int n);
	mpz_class encrypt(string message);
	mpz_class decrypt(string data);

	// Message functions
	void encryptFile(string messageFilename, string encryptedMessageFilename);
	void decryptFile(string encryptedMessageFilename, string decryptedMessageFilename);
};

#endif