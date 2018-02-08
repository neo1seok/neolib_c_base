#include <map>
#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/ec.h>
#include "comdef.h"



using namespace std;
using namespace neocolib;

struct Cert_RSA	//size 528
{
	char ID[16];
	unsigned char N[256];
	unsigned char signiture[256];
	//unsigned char option_data[256];
};
struct Cert_ECC	//size 113 byte
{
	unsigned char ID[16];
	unsigned char Q[33];
	unsigned char R[32];
	unsigned char S[32];
	//unsigned char option_data[256];
};
struct RSA_HEXSTR//size 113 byte
{
	string strN;
	string strE;
	string strD;

	string strP;
	string  strQ;

	string strDp;
	string strDq;
	string strQInv;
};





typedef std::map<std::string,std::string> MAPSTRKEVLAUE;
typedef MAPSTRKEVLAUE::iterator ITER_MAPSTRKEVLAUE;

//typedef ARRAYLIST<unsigned char> BYTE_ARRAY, *LPBYTE_ARRAY''


string BytetoHexStr(const unsigned char * Barray, int Blen);
neocolib::VECBYTE  HexStr2Byte(const string & str);

MAPSTRKEVLAUE getMAPfromARGS(int arc,char *argv[]);
string findValueFormMAP(string key,MAPSTRKEVLAUE & mapcmd,bool * pbool = NULL);

int  hex2bin(const char* src, unsigned char* target);
int char2int(char input);

void PrintView(const char* fmt, ...);
void PrintViewLine(const char * fmt, ...);

unsigned char HexCharToByte(char hexChar);
char ByteToHexChar(unsigned char byte);
int StringtoByteArray(const char* str, unsigned char* Barray, int Blen);
int ByteArraytoString(unsigned char* Barray, char* str, int Blen);
unsigned char* B2Str(unsigned char* Barray, int Blen, unsigned char* str);

int CheckSNisValid(unsigned char* SN);

int RSA_Keyinit(int gen);
int RSA_Keyfree();
int RSA_Keyinit_TSM();
int RSA_KeyGen_TSM();

int RSA_Sign_tsm(unsigned char* SIGN, unsigned char *ID, unsigned char *N_chip/*, RSA *key_tsm*/);
int GenCertRSA(Cert_RSA* cert, unsigned char* ID, unsigned char* N_chip, RSA* key_tsm);

int Gen_SDATA(unsigned char *SDATA, unsigned char *ID, unsigned char *PIN, unsigned char *INFORM);
int Gen_SDATA(unsigned char* SDATA);

int ECC_keyinit(int gen);
int ECC_keyfree();
int ECC_KeyGen_TSM();
int ECC_Keyinit_TSM();

int GenCertECC(Cert_ECC* cert, unsigned char* ID, unsigned char* Q_chip, EC_KEY* key_tsm);

unsigned long calcCRC(unsigned char* data, int iLength);
unsigned long calcCRCByTable(unsigned char* data, int iLength, unsigned short *crctable);

int SelAesSeed(int sel_aes);
int E_ECB_K1(unsigned char* in, unsigned char* out, unsigned char sel_aes);
int D_ECB_K1(unsigned char* in, unsigned char* out, unsigned char sel_aes);


int RSA_xor_chip(unsigned char* i_data, unsigned char* Nc, unsigned char* Ec, unsigned char* o_data);
int RSA_xor_tsm(unsigned char* i_data, unsigned char* o_data);
string GetSHA256Hexstr(const void *org, int orgSize);

string GetHMAC(const string & hexMsg, const string &  hexKey);


string  getRandHexStr(int length);
string  getRandAscii(int length);
void getRandBYTE(unsigned char * byte, int length);
LPBYTEARRAY getHexStr(const string & str);
short InverSHORT(short org);

//unsigned char *DER_encode_RSA_SubjectPublic(const unsigned char  *N, int nlen, const unsigned char  *E, int elen, int *outlength);
//unsigned char *DER_encode_RSA_public(const unsigned char  *N, int nlen, const unsigned char  *E, int elen, int *outlength);
#ifndef __NOTUSEOPENSSL__
VECBYTE DEREncodeRSApublic(const VECBYTE &N, const VECBYTE &E);
VECBYTE DEREncodeRSAprivate(const VECBYTE &N, const VECBYTE &E, const VECBYTE &D);
VECBYTE DEREncodeSubjectRSApublic(const VECBYTE &N, const VECBYTE &E);
int checkRSAKey(const string & strN, const string & strE, const string & strD);
RSA * CreateRSA(const unsigned char  *N, int nlen, const unsigned char  *E, int elen, const unsigned char  *D, int dlen);
RSA * CreateRSAByHexStr(const char * N, const char * E, const char * D);
bool getDecryptValue(VECBYTE & output, const VECBYTE & input);
unsigned char * getDecryptValuePtr(const unsigned char * input, int  inputSize, int *outsize);

void ConvertRsaToHexStr(RSA * rsa, RSA_HEXSTR & rsaHexStr);
RSA * GenerateRSA(int bytes, const unsigned char  *E, int elen);
RSA * GenerateRSAHexStr(int bytes, const char  *E);
int CalcRSAFactors(RSA_HEXSTR & rsaHexStr);
int CalcRSAFactorsType3(RSA_HEXSTR & rsaHexStr);
int UpdateFillRsaCrt(RSA *rsa);
//int CalcRSAFactors(string &strP, string & strQ, const string & strN, const string & strE, const string & strD);
//int CalcRSACRTParameterwithPQD(string & strDp, string & strDq, string & strQInv, const string & strP, const string & strQ, const string & strD);

#endif