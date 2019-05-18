#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <stdio.h>
#include "RSA.h"
#include "ModularArithmetic.h"

using namespace std;

int n = 64;

void message(string mes, bool isTestPassed) {
   if (isTestPassed)
      printf("\t%-25s ......... passed\n", mes.c_str());
   else
      printf("\t%-25s ......... failed\n", mes.c_str());
}  

// ------------------------
bool test_rsaGen() {
   RSA rsa(n);
   bool isTestPassed = true;
   message("test_rsaGen", isTestPassed);
   return isTestPassed;
}
 
bool test_rsaGenFiles() {
   RSA rsa(n, "privKey.txt", "pubKey.txt");
   bool isTestPassed = true;
   message("test_rsaGenFiles", isTestPassed);
   remove("privKey.txt");
   remove("pubKey.txt");
   return isTestPassed;
}
 
bool test_rsaReadFiles() {
   RSA rsa(n, "privKey.txt", "pubKey.txt");
   RSA rsa2("privKey.txt", "pubKey.txt");
   bool isTestPassed = true;
   message("test_rsaReadFiles", isTestPassed);
   remove("privKey.txt");
   remove("pubKey.txt");
   return isTestPassed;
}
 
bool test_rsaEncryptDecrypt() {
   RSA rsa(n);
   gmp_randclass r(gmp_randinit_default);
   mpz_class msgval;
   // Find smallest n-bit integer
   mpz_class smallnbitNum;
   mpz_ui_pow_ui(smallnbitNum.get_mpz_t(), 2, n - 1);

   do {
      // Pick a random n-bit number N
      msgval = r.get_z_bits(n);
   } while ( msgval >= smallnbitNum );

   string msg = msgval.get_str();
   mpz_class c, d;
   c = rsa.encrypt(msg);
   d = rsa.decrypt(c.get_str());
   bool isTestPassed = msg.compare(d.get_str()) == 0;
   message("test_rsaEncryptDecrypt", isTestPassed);
   return isTestPassed;
}

bool test_rsaEncDecFromFile() {
    string msgFilename = "testing.txt";
    string encFilename = "testEnc.txt";
    string decFilename = "testDec.txt";
    string msg = "Testing...";
    ofstream msgFile(msgFilename, ofstream::out);
    if (msgFile.is_open()) {
        msgFile << msg;
        msgFile.close();
    }
    else {
        cout << "Error opening file " << msgFilename << endl;
        assert( 0 );
    }
    RSA r(n);
    r.encryptFile(msgFilename, encFilename);
    r.decryptFile(encFilename, decFilename);
    ifstream decFile(decFilename);
    string decryptedMessage;
    if (decFile.is_open()) {
        getline(decFile, decryptedMessage);
        decFile.close();
    }
    else {
        cout << "Error opening file " << decFilename << endl;
        assert( 0 );
    }
    remove(msgFilename.c_str());
    remove(encFilename.c_str());
    remove(decFilename.c_str());
    bool isTestPassed = !msg.compare(decryptedMessage);
    message("test_rsaEncDecFromFile", isTestPassed);
    return isTestPassed;
}
 
// ------------------------------------------------------------------------

// Define a function pointer type
typedef bool (*f_ptr_t)();

int main() {
   int testsPassed = 0;
   int numTests = 5;

   // Array of function pointers
   f_ptr_t funcArray[20]; 
   funcArray[0] = &test_rsaGen; 
   funcArray[1] = &test_rsaGenFiles; 
   funcArray[2] = &test_rsaReadFiles; 
   funcArray[3] = &test_rsaEncryptDecrypt;
   funcArray[4] = &test_rsaEncDecFromFile;

   cout << "\nRunning Unit Tests for RSA\n\n";

   // Run each unit test
   for (int i = 0; i < numTests; i++) {
      if ( funcArray[i]() ) {
         testsPassed++;
      }
   }

   cout << endl;
   cout << "Tests passed: " << testsPassed << " out of " << numTests << endl;
   cout << "\tPassed " << (double)testsPassed/numTests*100 
		<< "% of the tests" << endl << endl;

   return 0;
}

