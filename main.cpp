#include <iostream>
#include <stdio.h>
#include "RSA.h"

using namespace std;
//n - n-bit RSA keys 
int n = 256;

int main(int argc, char *args[])
{

    string fnameEnc, fnameDec, fnamePubKey("pubkey.txt"), fnamePrivKey("privkey.txt");
    
    //command string should be
    //-e dec.txt enc.txt key.txt
    //or
    //-d enc.txt dec.txt 
    //if commans string other
    //print example of use and exit
    if (argc!=4 && (strncmp(args[1],"-e",2)==0 ||strncmp(args[1],"-d",2)==0)) {
        
        cout<<"example for encypt ./main -e dec.txt enc.txt"<<endl;
        cout<<"for decrypt ./main -d enc.txt dec.txt"<<endl;
        return 0;
        
    }
   
    //-e dec.txt enc.txt key.txt
    if (strncmp(args[1],"-e",2)==0) {
        fnameDec=args[2]; fnameEnc=args[3];
        //Generate n-bit keys and save keys if files
        RSA r(n,fnamePubKey,fnamePrivKey);
         r.encryptFile(fnameDec, fnameEnc);
        
    }
    else
    //-d enc.txt dec.txt 
     {
         fnameDec=args[3]; fnameEnc=args[2];
         //Get keys from files
        RSA r(fnamePubKey,fnamePrivKey);
         r.decryptFile( fnameEnc,fnameDec);
    }
    return 0;
}
