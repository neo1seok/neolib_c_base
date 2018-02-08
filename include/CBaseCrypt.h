#pragma once
//#include <openssl/ec.h>
//#include <openssl/rsa.h>
#include "IRSD1410_DEFINE.h" 
#include "cryptodefine.h"
#include <string>
#include <openssl/sha.h>

using namespace std;
//#include <string.h>
#define REQ_TITLE(t_) appendLog( "\n" #t_ "\n");
//int generateSNECC(BYTE *pbOut, DWORD *pdwLenOut)추가
//seed 암호화 부분 추가
//int  SelAesSeedECC(int sel_aes) 추가
//int E_ECB_K1ECC(BYTE* in, BYTE* out, BYTE sel_aes)
//int D_ECB_K1ECC(BYTE* in, BYTE* out, BYTE sel_aes) 추가
// openssl/evp.h 추가
#if 0
#define DEFUNITSIZE		16
#define CERT_IDSIZE 16
#define DEVICE_SIZE 32
#define TSM_SIZE	32
#define ACHIP_SIZE	33


#define CERT_QCHIPSIZE 33
#define CERT_SIGNSIZE_ECC 32

#define CERT_NCHIPSIZE 256
#define CERT_SIGNSZE_RSA 256

#define CERT_SDATASIZE 32
#define CERT_EHKSIZE	32
#define CERT_ETSMSIZE	16
#define CERT_NTSMSIZE	256

#define CERT_C1SIZE 16

#define G_MSG_SIZE 65600

#pragma pack(push, 1)   
struct CERT_ECC	//size 113 byte
{
	BYTE ID[CERT_IDSIZE];
	BYTE Q[CERT_QCHIPSIZE];
	BYTE R[CERT_SIGNSIZE_ECC];
	BYTE S[CERT_SIGNSIZE_ECC];
	//BYTE option_data[256];
};


struct CERTEXT_ECC
{
	CERT_ECC cert;

	unsigned char SDATA[CERT_SDATASIZE];
	unsigned char EHK[CERT_EHKSIZE];
};

struct CERT_RSA	//size 113 byte
{
	BYTE ID[CERT_IDSIZE];
	BYTE N[CERT_NCHIPSIZE];
	BYTE SIGN[CERT_SIGNSZE_RSA];
	//BYTE option_data[256];
};


struct CERTEXT_RSA
{
	CERT_RSA cert;

	unsigned char SDATA[CERT_SDATASIZE];
	unsigned char EHK[CERT_EHKSIZE];
	unsigned char Etsm[CERT_ETSMSIZE];
	unsigned char Ntsm[CERT_NTSMSIZE];

};
#endif
#pragma pack(push, 1)   
struct INTER_PARAM
{
	union{
		CERT_RSA certRSA;
		CERT_ECC certECC;
		unsigned char ID[CERT_IDSIZE];

	};

	unsigned char SDATA[CERT_SDATASIZE];
	unsigned char EHKDevice[CERT_EHKSIZE];
	unsigned char Etsm[CERT_ETSMSIZE];
	unsigned char Ntsm[CERT_NTSMSIZE];
	unsigned char Qtsm[CERT_QCHIPSIZE];

	unsigned char Decc[DEFECCSIZE_SIGN];
	unsigned char Drsa[CERT_NCHIPSIZE];


	BYTE K1[DEFUNITSIZE];
	BYTE K2[DEFUNITSIZE];
	BYTE IV[DEFUNITSIZE];
	BYTE SEL_AES;
	BYTE SN[DEFUNITSIZE];
	BYTE RN[DEFUNITSIZE];
	
	//BYTE G_A_CHIP[ACHIP_SIZE];
	//BYTE EHK_device[33];

	BYTE HK_device[DEVICE_SIZE];
	BYTE HK_tsm[TSM_SIZE];
	//	BYTE HK_chip[64+1];
	BYTE G_MSG[G_MSG_SIZE];
	int g_msgPos;

	BYTE *G_PIN;
	int g_pinSize;// = DEFUNITSIZE;

	BYTE *G_INFORM;
	int g_pinformSize;// = 16;
	
	BYTE S[DEFRSASIZE];
	BYTE C[DEFRSASIZE];
	BYTE E[DEFUNITSIZE];

	BYTE A[DEFECCSIZE];
	BYTE SR[DEFECCSIZE_SIGN];
	BYTE SS[DEFECCSIZE_SIGN];

	//BYTE SHA[DEFECCSIZE_SIGN];
	BYTE B[DEFECCSIZE];	//공개키 바이트
	BYTE SRp[DEFECCSIZE_SIGN];
	BYTE SSp[DEFECCSIZE_SIGN];


	BYTE C1[DEFUNITSIZE];

	BYTE Ep[DEFUNITSIZE];
	BYTE Cotp[DEFUNITSIZE];
	BYTE Sp[DEFRSASIZE];
	BYTE OTP[OTPSIZE];


	BYTE SRpp[DEFECCSIZE_SIGN];
	BYTE SSpp[DEFECCSIZE_SIGN];



	


};
#pragma pack(pop)  


//글로벌 변수

class CBaseCrypt
{

public:
    CBaseCrypt(void);
    ~CBaseCrypt(void);
	void startLog();
	void appendLog(const char * fmt, ...);
	string flushLog();
	string getLog();
	void  B2Str(BYTE* Barray, int Blen);

	void resetSignHash();
	void updateSignHash(BYTE * msg, int size);
	void getHashValue4Sign(BYTE *EM);
	void ViewMsgHash(BYTE * msg, int size);

protected:

	INTER_PARAM mInterParam;

	string mstrlog;
	string errlog;
	SHA256_CTX ctx4Sign;
	//BYTE G_MSG[];
	
	
	
	

	

	void bytecpy(BYTE * pdst, int dstSize, BYTE *psrc, int srcSize);
	
	void PARAM_Clear();

	//virtual BYTE*  GetID();

	//	BYTE ecx_Q[33];
	//	BYTE ecx_Qchip[33];

	//	BYTE ecx_bd[32];
	//	BYTE ecx_BQ[33];
	//	BYTE ecx_AQ[33];
	//	BYTE G_Q_CHIP[33];
	
	//BYTE G_N_chip[256];
	

	

	//BYTE HK_chip[64 + 1];
	
	





public:

	int encryptSEEDCTR(BYTE* pbIn, int iLenIn, BYTE* pbOut, BYTE* pbCTR, unsigned int* dwNumCTR);


	int encryptCTR(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut);


	int encryptCCM(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut);



	int decryptCCM(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut);

	static int cryptoCBC(BYTE *symkey, int lengthsymkey, BYTE* pbIn, int iLenIn, BYTE* pbOut, int* pdwLenOut, bool isEnc);

	int generateHMAC(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut);


	int data2paddingdata_AES(BYTE* data, int len);


	int data2paddingdata_HMAC(BYTE* data, int len);

	virtual void SetID(const char  *strID){

	}
	virtual void SetID(BYTE *pID, int idSize){

	}


///// ECB 암호화(AES, SEED)

	int SelAesSeed(int sel_aes);

	int E_ECB_K1(BYTE* in, BYTE* out, BYTE sel_aes);
	int D_ECB_K1(BYTE* in, BYTE* out, BYTE sel_aes);

	int requestEHKdevice(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut);

	virtual int keyinit(int gen) 
	{
		resetSignHash();
		return 0;
	};
	virtual int keyfree() 	{
		return 0;
	};


	virtual BYTE*	getHKDevice(){
		return NULL;
	}
	//virtual void ISSUE(const unsigned char * btID, int IdSize, const unsigned char * btQchip, int qChipSize){

	//}
	
	virtual void UpdateSDATA();
	virtual void UpdateCERT(){
	}
	virtual bool  UpdateC1(){
		return true;
	}

	//void ISSUEfromString(const char * hsId, const char * hsqChip);

	void SetPINnINFORM(BYTE * pin, int pinSize, BYTE *inform, int informSize);

	virtual 	bool		SetRN(const char * hexRN){

		return true;
	}
	const char * getErrMsg(){
		return errlog.c_str();
	}
	
	virtual INTER_PARAM * GetInterParam();
	virtual string viewCETEXT();

	virtual void UpdateTSM(){


	}
	virtual void UpdatePRIKEY(){


	}
	
	virtual bool UpdateSign(){

		errlog = "not define";
		return false;
	}
	virtual void UpdateChip(){

	}

};



