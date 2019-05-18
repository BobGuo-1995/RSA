#ifndef MODULARARITHMETIC_H
#define MODULARARITHMETIC_H

#include <random>
#include <gmpxx.h>
bool Primality2(mpz_class& n,int k);
 bool MillerRabinTest(mpz_class& n, int k);
mpz_class gcdExt (mpz_class a, mpz_class b, mpz_class & x, mpz_class & y);
class ModularArithmetic {
   public:
      static mpz_class modadd(mpz_class& a, mpz_class& b, mpz_class& N);
      static mpz_class modmult(mpz_class& a, mpz_class& b, mpz_class& N);
      static mpz_class modinv(mpz_class& a, mpz_class& N);
      static mpz_class modexp(mpz_class& a, mpz_class& b, mpz_class& N);
      static bool isPrime(mpz_class& N, int k);
      static mpz_class genPrime(int n);
};

#endif