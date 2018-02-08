#pragma once
#if 0
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <memory.h> 

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/crypto.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/ecdh.h>
#include <openssl/objects.h>
#include <openssl/aes.h>
#include <openssl/evp.h> //seed
#include <openssl/rsa.h>
#include <openssl/bn.h>

#include "IRSD1410_DEFINE.h" 


struct Cert_RSA	//size 528
{
	char ID[16];
	BYTE N[256];
	BYTE signiture[256];
	//BYTE option_data[256];
};
struct Cert_ECC	//size 113 byte
{
	BYTE ID[16];
	BYTE Q[33];
	BYTE R[32];
	BYTE S[32];
	//BYTE option_data[256];
};




BYTE HexCharToByte(char hexChar);
char ByteToHexChar(BYTE byte);
int StringtoByteArray(char* str, BYTE* Barray, int Blen);
int ByteArraytoString(BYTE* Barray, char* str, int Blen);
BYTE* B2Str(BYTE* Barray,  int Blen, BYTE* str);

int CheckSNisValid(unsigned char* SN);

int RSA_Keyinit(int gen);
int RSA_Keyfree();
int RSA_Keyinit_TSM();
int RSA_KeyGen_TSM();

int RSA_Sign_tsm(BYTE* SIGN, BYTE *ID, BYTE *N_chip, RSA *key_tsm);
int GenCertRSA(Cert_RSA* cert, BYTE* ID, BYTE* N_chip, RSA* key_tsm);

int Gen_SDATA(BYTE *SDATA, BYTE *ID,BYTE *PIN, BYTE *INFORM);
int Gen_SDATA(BYTE* SDATA);

int ECC_keyinit(int gen);
int ECC_keyfree();
int ECC_KeyGen_TSM();
int ECC_Keyinit_TSM();

int GenCertECC(Cert_ECC* cert, BYTE* ID, BYTE* Q_chip, EC_KEY* key_tsm);

DWORD calcCRC(BYTE* data, int iLength);

int SelAesSeed(int sel_aes);
int E_ECB_K1(BYTE* in, BYTE* out, BYTE sel_aes);
int D_ECB_K1(BYTE* in, BYTE* out, BYTE sel_aes);


int RSA_xor_chip(BYTE* i_data, BYTE* Nc, BYTE* Ec, BYTE* o_data);
int RSA_xor_tsm(BYTE* i_data, BYTE* o_data);
#endif