#include <iostream>
#include <gmpxx.h>
#include "ModularArithmetic.h"

using namespace std;

void message(string mes, bool isTestPassed) {
   if (isTestPassed)
      printf("\t%-20s ......... passed\n", mes.c_str());
   else
      printf("\t%-20s ......... failed\n", mes.c_str());
}  

// Test 12 + 8 (mod 15) = 5
// ------------------------
bool test_modadd() {
   mpz_class a, b, c, N;
   a = "12"; b = "8"; N = "15";
   c = ModularArithmetic::modadd(a, b, N);
   bool isTestPassed = c == 5;
   message("test_modadd", isTestPassed);
   return isTestPassed;
}
 
// Test 952545454 + 834352542 (mod 5643436545) = 1786897996
// --------------------------------------------------------
bool test_modadd_large() {
   mpz_class a, b, c, N, res;
   a = "952545454"; b = "834352542"; N = "5643436545";
   c = ModularArithmetic::modadd(a, b, N);
   res = "1786897996";
   bool isTestPassed = c == res;
   message("test_modadd_large", isTestPassed);
   return isTestPassed;
}

// Test 12 * 8 (mod 15) = 6
// ------------------------
bool test_modmult() {
   mpz_class a, b, c, N;
   a = "12"; b = "8"; N = "15";
   c = ModularArithmetic::modmult(a, b, N);
   bool isTestPassed = c == 6;
   message("test_modmult", isTestPassed);
   return isTestPassed;
}

// Test 952545454 * 834352542 (mod 5643436545) = 2731065003
// --------------------------------------------------------
bool test_modmult_large() {
   mpz_class a, b, c, N, res;
   a = "952545454"; b = "834352542"; N = "5643436545";
   c = ModularArithmetic::modmult(a, b, N);
   res = "2731065003";
   bool isTestPassed = c == res;
   message("test_modmult_large", isTestPassed);
   return isTestPassed;
}

// Test 8^12 (mod 15) = 1
// ------------------------
bool test_modexp() {
   mpz_class a, b, c, N;
   a = "8"; b = "12"; N = "15";
   c = ModularArithmetic::modexp(a, b, N);
   bool isTestPassed = c == 1;
   message("test_modexp", isTestPassed);
   return isTestPassed;
}

// Test 952545454 ^ 834352542 (mod 5643436545) = 4626426241
// --------------------------------------------------------
bool test_modexp_large() {
   mpz_class a, b, c, N, res;
   a = "952545454"; b = "834352542"; N = "5643436545";
   c = ModularArithmetic::modexp(a, b, N);
   res = "4626426241";
   bool isTestPassed = c == res;
   message("test_modexp_large", isTestPassed);
   return isTestPassed;
}

// Test 7^{-1} (mod 15) = 13
// ------------------------
bool test_modinv() {
   mpz_class a, c, N;
   a = "7"; N = "15";
   c = ModularArithmetic::modinv(a, N);
   bool isTestPassed = c == 13;
   message("test_modinv", isTestPassed);
   return isTestPassed;
}

// Test 952545454^{-1} (mod 5643436545) = 5453352994
// ------------------------
bool test_modinv_large() {
   mpz_class a, c, N;
   a = "952545454"; N = "5643436545";
   c = ModularArithmetic::modinv(a, N);
   bool isTestPassed = c == 5453352994;
   message("test_modinv_large", isTestPassed);
   return isTestPassed;
}

// Test primality of 23 (true), 25 (false) and 91 (true)
// -----------------------------------------------------
bool test_isPrime() {
   mpz_class N;
   N = "23";
   int k = 4;
   bool isTestPassed = ModularArithmetic::isPrime(N, k);
   N = "25";
   isTestPassed = isTestPassed && !ModularArithmetic::isPrime(N, k);
   N = "97";
   isTestPassed = isTestPassed && ModularArithmetic::isPrime(N, k);
   message("test_isPrime", isTestPassed);
   return isTestPassed;

}

// Test primality of 285044579391958034764329004428399141293
// -----------------------------------------------------
bool test_isPrime_large() {
   mpz_class N;
   N = "285044579391958034764329004428399141293";
   int k = 128;
   bool isTestPassed = ModularArithmetic::isPrime(N, k);
   message("test_isPrime_large", isTestPassed);
   return isTestPassed;

}

// Test generation of 7-bit primes
// -----------------------------------------------------
bool test_genPrime() {
   int n = 7;
   mpz_class N;
   mpz_class primes[13];
   primes[0] = "67"; primes[1] = "71"; primes[2] = "73"; primes[3] = "79";
   primes[4] = "83"; primes[5] = "89"; primes[6] = "97"; primes[7] = "101";
   primes[8] = "103"; primes[9] = "107"; primes[10] = "109"; primes[11] = "113";
   primes[12] = "127";
   N = ModularArithmetic::genPrime(n);
   bool isTestPassed = false;
   for (int i = 0 ; i < 13; i++) {
      isTestPassed = isTestPassed || N == primes[i];
   }
//   cout << "rand prime is " << N << endl;
   message("test_genPrime", isTestPassed);
   return isTestPassed;
}

// ------------------------------------------------------------------------

// Define a function pointer type
typedef bool (*f_ptr_t)();

int main() {
   int testsPassed = 0;
   int numTests = 11;

   // Array of function pointers
   f_ptr_t funcArray[20]; 
   funcArray[0] = &test_modadd; 
   funcArray[1] = &test_modadd_large; 
   funcArray[2] = &test_modmult; 
   funcArray[3] = &test_modmult_large; 
   funcArray[4] = &test_modexp; 
   funcArray[5] = &test_modexp_large; 
   funcArray[6] = &test_modinv; 
   funcArray[7] = &test_modinv_large; 
   funcArray[8] = &test_isPrime; 
   funcArray[9] = &test_isPrime_large; 
   funcArray[10] = &test_genPrime; 

   cout << "\nRunning Unit Tests for Modular Arithmetic\n\n";

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

