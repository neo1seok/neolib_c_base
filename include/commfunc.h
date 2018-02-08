
#include <openssl/ec.h>


int StringtoByteArray(const char* str, BYTE* Barray, int Blen);
int ByteArraytoString(BYTE* Barray, const char* str, int Blen);
BYTE* B2Str(BYTE* Barray,  int Blen, BYTE* str);

//int CheckSNisValid(unsigned char* SN);

//int RSA_Keyinit(int gen);
//int RSA_Keyfree();
//int RSA_Keyinit_TSM();
//int RSA_KeyGen_TSM();

//int RSA_Sign_tsm(BYTE* SIGN, BYTE *ID, BYTE *N_chip, RSA *key_tsm);
//int GenCertRSA(Cert_RSA* cert, BYTE* ID, BYTE* N_chip, RSA* key_tsm);

int Gen_SDATA(BYTE *SDATA, BYTE *ID,BYTE *PIN, BYTE *INFORM);
int Gen_SDATA(BYTE *SDATA, BYTE *ID, BYTE *PIN, int pinSize, BYTE *INFORM, int informSize);
int Gen_SDATA(BYTE* SDATA);

//int ECC_keyinit(int gen);
//int ECC_keyfree();
//int ECC_KeyGen_TSM();
//int ECC_Keyinit_TSM();

//int GenCertECC(CERT_ECC* cert, BYTE* ID, BYTE* Q_chip, EC_KEY* key_tsm);

DWORD calcCRC(BYTE* data, int iLength);

int SelAesSeed(int sel_aes);

int Bn2Bin(const BIGNUM * pbignum,BYTE *ppbuffe,int bytesize);