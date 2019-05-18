#include "ModularArithmetic.h"
#include  <iostream>
using namespace std;

/* did some digging find out that using +, -, *, /, % are overloaded to work on objects of class mpz_class.
 Mixed expressions with ordinary integers are Ok. */


//Based on that, this is what I am going to do for modadd function call

//c = modadd(a, b, N):returnsc=a+b(modN)

mpz_class ModularArithmetic::modadd(mpz_class& a, mpz_class& b, mpz_class& N){
    
     mpz_class z;
     mpz_class c;
     
     z = a + b ;
     while (z > N )
     {
     z = z - N;
     }
     
     c = z % N;
  
    return c;
};

//Since division is better with the time complexity in the modmult case

mpz_class ModularArithmetic::modmult(mpz_class& a, mpz_class& b, mpz_class& N){
    
    mpz_class c;
    
    c = ((a%N)*(b%N))%N;
    
    return c;
};

// returns c such that ac ≡ 1 (mod N), or 0 if inverse doesn’t exist
mpz_class ModularArithmetic::modinv(mpz_class& a, mpz_class& N){
    
    mpz_class  x,y,d; 
  // extended_euclid(a,N,res,res1,d);
    d= gcdExt (a, N,  x,  y);
   if (d!=1) return 0;
    if(x>=0)  return x;
    return N+x;
    
};


// returns c = a^b (mod N)
mpz_class ModularArithmetic::modexp(mpz_class& a, mpz_class& b, mpz_class& N){
    
    /*mpz_class rr;
    mpz_powm(rr.get_mpz_t(),a.get_mpz_t(),b.get_mpz_t(),N.get_mpz_t());
    return rr;*/
     if (b == 0) return 1;
     mpz_class bt=b/2;
  mpz_class z = ModularArithmetic::modexp(a, bt, N);
  if (b % 2 == 0)
    return (z*z) % N;
  else
    return (a*z*z) % N;
};


//  returns true if N is prime with probability 1/2k, or false if N is not prime.
bool ModularArithmetic::isPrime(mpz_class& N, int k){
    //return MillerRabinTest(N,k);
    return Primality2(N,k);
    
};

// returns a n-bit prime number N.
//example n=7; val in [64,127]
mpz_class ModularArithmetic::genPrime(int n){
    --n;
    gmp_randclass grnd(gmp_randinit_default);
    grnd.seed(time(NULL));
    mpz_class val;
    do{
        val=grnd.get_z_bits(n);//val in [0,2^n) if n==7 val=xxxxxx x={0,1}
        mpz_class highBit=1;
        highBit=(highBit<<n);
        val|=highBit;//set highBit. val must have n bits. now val=1xxxxxx x={0,1}
         
        //if val is not a prime repeat
    }while(!ModularArithmetic::isPrime(val,100));
return val;    
};


//*************************************
//******extend functions***************

//exactly as in Figure 1.8
bool Primality2(mpz_class& n,int k)
{
    gmp_randclass grnd(gmp_randinit_default);
    grnd.seed(time(NULL));
    mpz_class n1(n-1);
    // repeat k times
    for (int i = 0; i < k; i++)
    {
         mpz_class a=grnd.get_z_range(n1)+1;//1<=a<N
         mpz_class x = ModularArithmetic::modexp(a, n1, n);
         if(x!=1) return false;
    }
    return true;
}

//https://en.wikipedia.org/wiki/Miller–Rabin_primality_test
//probability 1/4^k
 bool MillerRabinTest(mpz_class& n, int k)
{
    if (n == 2 || n == 3)
        return true;
 
    if (n < 2 || n % 2 == 0)
        return false;
 
    //  n − 1 => (2^s)·t, where t is odd
    mpz_class t = n - 1;
 
    int s = 0;
 
    while (t % 2 == 0)
    {
        t /= 2;
        s += 1;
    }
    
    gmp_randclass grnd(gmp_randinit_default);
    grnd.seed(time(NULL));
    mpz_class two(2);
    // repeat k times
    for (int i = 0; i < k; i++)
    {
        // a in [2, n − 2]
        mpz_class a=grnd.get_z_range(n-4)+2;
        // x ← a^t mod n
        mpz_class x = ModularArithmetic::modexp(a, t, n);
 
        if (x == 1 || x == n - 1)
            continue;
 
        // repeat s − 1 times
        for (int r = 1; r < s; r++)
        {
            // x ← x^2 mod n
            x = ModularArithmetic::modexp(x, two, n);
 
            if (x == 1)
                return false;
 
            if (x == n - 1)
                break;
        }
 
        if (x != n - 1)
            return false;
    }
 
    return true;
}

/* calculates a*x + b*y = gcd(a, b) = d */
//https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
//recursive
mpz_class gcdExt (mpz_class a, mpz_class b, mpz_class & x, mpz_class & y) {
	if (a == 0) {
		x = 0; y = 1;
		return b;
	}
	mpz_class x1, y1;
	mpz_class d = gcdExt (b%a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}

