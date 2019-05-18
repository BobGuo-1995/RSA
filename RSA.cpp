//Jue Guo, Bob 
#include <iostream>
#include <fstream>
#include <assert.h>
#include <math.h>

#include "RSA.h"
#include "ModularArithmetic.h"
#include <gmpxx.h>

using namespace std;


RSA::RSA(int n)
{
    mpz_class p,q,N,s,e;
    //generate n-bit prime p
    p=ModularArithmetic::genPrime(n);
    //generate prime and distinct with p n-bit q
    do{
    q=ModularArithmetic::genPrime(n);
    }while(p==q);
    //Now we have two large and distinct (n-bit) random primes p and q.
    //Compute N and s
    N=p*q;
    s=(p-1)*(q-1);
    
    //text values for easy debug. We can remove it
    string sp=p.get_str(),sq=q.get_str(),sn=N.get_str(),ss=s.get_str(),se;
    
    //******
    //Choose an integer e such that 1 < e < s and e is relatively prime to s.
    gmp_randclass grnd(gmp_randinit_default);
    grnd.seed(time(NULL));
     mpz_class gcdVal;
    do
    {
        //1<e<s   because get_z_range (mpz class n) Generate a random integer in the range 0 to n − 1 inclusive.
        e=grnd.get_z_range(s-2)+2;

        se=e.get_str();//only for debug. We can remove it
        
        //test for relatively prime e and s. if gcd(e,s)==1 then e is relatively prime to s.
        mpz_gcd(gcdVal.get_mpz_t(),e.get_mpz_t(),s.get_mpz_t());
    }while(gcdVal!=1);
    //ok. We have e
    
   // Compute d to satisfy the relationship de=1(mod s), that is, d is the inverse of e modulo s.
    mpz_class d=ModularArithmetic::modinv(e,s);
    
    // Now we have keys
    pubKey.push_back(N);
    pubKey.push_back(e);
    
    privKey.push_back(N);
    privKey.push_back(d);
    cout<<"public key: (N="<<pubKey[0]<<", e="<<pubKey[1]<<")"<<endl;
    
}

//Same as RSA(n) but in addition it stores
//the public and private key pair in files pubfilename and privfilename. The two integers for
//each key should be separated by a line break.
RSA::RSA(int n, string privKeyFilename, string pubKeyFilename):RSA(n)//Call base constructor
{
    // Now we have private and pulic keys 
    
    //Save private key in privKeyFilename
    ofstream oPriv(privKeyFilename,ios_base::trunc);
    if(oPriv.is_open())
    {
        oPriv<<privKey[0]<<endl;
        oPriv<<privKey[1];
    }
    else{
    
        throw invalid_argument("can't write "+privKeyFilename);
    }
    //Save public key in privKeyFilename
    ofstream oPubl(pubKeyFilename,ios_base::trunc);
    if(oPubl.is_open())
    {
        oPubl<<pubKey[0]<<endl;
        oPubl<<pubKey[1];
    }else
    {throw invalid_argument("can't write "+pubKeyFilename);}
}

// reads in the public and private keys stored in
// files pubfilename and privfilename. The two integers should be separated by a line break.
RSA::RSA(string privKeyFilename, string pubKeyFilename)
{
    ifstream iPriv(privKeyFilename);
    if(iPriv.is_open()){
        mpz_class temp;
        iPriv>>temp;
        //cout<<"temp="<<temp<<endl;
        privKey.push_back(temp);//N
        iPriv>>temp;
        //cout<<"temp="<<temp<<endl;
        privKey.push_back(temp);//d
    }
    else
        throw invalid_argument("can't open "+privKeyFilename);
    
    
    ifstream iPub(pubKeyFilename);
    if(iPub.is_open()){
        mpz_class temp;
        iPub>>temp;
        //cout<<"temp="<<temp<<endl;
        pubKey.push_back(temp);//N
        iPub>>temp;
        //cout<<"temp="<<temp<<endl;
       pubKey.push_back(temp);//e
    }
    else
        throw invalid_argument("can't open "+pubKeyFilename);
    
}
mpz_class RSA::encrypt(string message)
{
    mpz_class m(message),c;
    c=ModularArithmetic::modexp(m,pubKey[1],pubKey[0]);//c=m^e (mod N)
    return c;
}
mpz_class RSA::decrypt(string data)
{
    mpz_class c(data),m;
    m=ModularArithmetic::modexp(c,privKey[1],privKey[0]);//m=c^d (mod N)
    return m;
}


void RSA::encryptFile(string messageFilename, string encryptedMessageFilename) {
    // Open the file and read the message contents
    string line;
    string message;
    ifstream messageFile(messageFilename);
    if (messageFile.is_open()) {
        getline(messageFile, line);
        message = line;
        messageFile.close();
    }
    else {
        cout << "Error opening file " << messageFilename << endl;
        assert( 0 );
    }
    
    // Convert each character's ASCII value to a binary string
    mpz_class charInt;
    string charBinary, msgBinary = "";
    
    for (int i = 0; i < message.size(); i++) {
        // convert char to integer
        charInt = (int) message[i];
        // convert integer to bitstring
        charBinary = charInt.get_str(2);
        // pad with zeroes if necessary
        while (charBinary.size() < 8)
            charBinary = "0" + charBinary;
        // append to full bitstring
        msgBinary = msgBinary + charBinary;
    }
    
    // Convert message bitstring to mpz_class number
    mpz_class msgInt = mpz_class(msgBinary,2);
    
    // Make sure the message as a number is smaller than N
    if (cmp(msgInt, pubKey[0]) >= 0) {
        cout << "Message size too large. Please choose a shorter message." << endl;
        assert( 0 );
    }
    
    // Encrypt the message
    mpz_class encryptedMessage = encrypt(msgInt.get_str());
    
    // Write the encrypted message to a file
    ofstream encryptedFile(encryptedMessageFilename, ofstream::out);
    if (encryptedFile.is_open()) {
        encryptedFile << encryptedMessage;
        encryptedFile.close();
    }
    else {
        cout << "Error opening file " << encryptedMessageFilename << endl;
        assert( 0 );
    }
}

void RSA::decryptFile(string encryptedMessageFilename, string decryptedMessageFilename) {
    // Open the file and read the encrypted message contents
    string line;
    mpz_class encryptedMessage;
    
    ifstream encryptedFile(encryptedMessageFilename);
    if (encryptedFile.is_open()) {
        getline(encryptedFile, line);
        encryptedMessage = line;
        encryptedFile.close();
    }
    else {
        cout << "Error opening file " << encryptedMessageFilename << endl;
        assert( 0 );
    }
    // Decrypt the message
    mpz_class msgInt = decrypt(encryptedMessage.get_str());
    
    // Convert the decrypted message to a bitstring
    string msgBinary = msgInt.get_str(2);
    // Pad with zeros if necessary
    while (msgBinary.size() % 8)
        msgBinary = "0" + msgBinary;
    
    string charBinary, message = "";
    mpz_class charInt;
    // loop over 8-bit chunks of bitstring
    for (int i = 0; i < msgBinary.size(); i += 8) {
        // extract substring chunk of 8 bits for single character
        charBinary = msgBinary.substr(i,8);
        // convert bitstring to integer
        charInt = mpz_class(charBinary,2);
        // convert integer to character and append to message
        message += (char) charInt.get_si();
    }
    
    // Write the decrypted message to a file
    ofstream decryptedFile(decryptedMessageFilename, ofstream::out);
    if (decryptedFile.is_open()) {
        decryptedFile << message;
        decryptedFile.close();
    }
    else {
        cout << "Error opening file " << decryptedMessageFilename << endl;
        assert( 0 );
    }
}
