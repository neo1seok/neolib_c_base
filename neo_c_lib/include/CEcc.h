#ifndef __CECC__
#define __CECC__
#include <openssl/ec.h>
#include<string>
#include "IRSD1410_DEFINE.h" 
#include "CBaseCrypt.h"

//#include <string.h>

using namespace std;
//int generateSNECC(BYTE *pbOut, DWORD *pdwLenOut)추가
//seed 암호화 부분 추가
//int  SelAesSeedECC(int sel_aes) 추가
//int E_ECB_K1ECC(BYTE* in, BYTE* out, BYTE sel_aes)
//int D_ECB_K1ECC(BYTE* in, BYTE* out, BYTE sel_aes) 추가
// openssl/evp.h 추가



//글로벌 변수

class CEcc: public CBaseCrypt
{

public:
    CEcc(void);
    ~CEcc(void);
private:
	EC_KEY		*G_ECCkey_TSM;
	EC_KEY		*G_ECCkey_CHIP;		//개인키는 모름
	EC_KEY		*G_ECCkey_A;		//개인키는 모름
	EC_KEY		*G_ECCkey_B;
	EC_GROUP	*G_ECCgroup;
	EC_POINT	*G_puk_A;
#if 0
	BYTE K1[16];
	BYTE K2[16];
	BYTE IV[16];
	//BYTE G_MSG[];
	int g_msgPos;
	BYTE SEL_AES;
	BYTE SN[16];
	BYTE RN[16];

#endif
public:
	//BYTE SN[16];
	//BYTE m_certext.cert.ID[16];
	//CERTEXT_ECC m_certext;
	//BYTE G_PIN[16];
	//BYTE G_INFORM[16];
	//BYTE RN[16];

//	BYTE K1[16];
//	BYTE K2[16];
//	BYTE IV[16];
//	BYTE SEL_AES;

	//BYTE EHK_device[32];
	


	void UpdatePUK(BYTE * q, EC_KEY		* ecckey);

	


public:

	void  TestECC();

//////////////////////////// 초기화
	int ECX_genkey(BYTE* d, BYTE* G, BYTE* Q);


	
	//int ECC_keyinit(int gen);

	//int ECC_keyfree();

	int ECC_KeyGen_TSM();
	int ECC_Keyinit_TSM();
	
	/////////////////////////////  제조
	bool verifySign(CERT_ECC * pcertinfo);

	//////////////////////////// 발급

	int requestIdECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut);

	int GenCertECC(CERT_ECC* cert, EC_KEY* key_tsm);


	int requestCertECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut);


	int requestRegCertECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut);


	//////////////////////////// 사용

	int requestRNECC(BYTE *pbOut, DWORD *pdwLenOut);



	int requestSessionECC(BYTE* pbIn, int iLenIn);


	int requestHS1ECC(BYTE* pbOut, DWORD* pdwLenOut);




	int requestHS2ECC(BYTE* pbIn, int iLenIn);

	//int requestEHKdevice(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut);





	int storeEHKdeviceECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut);


	int requestSignECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE* pbOTP);

	virtual void SetID(const char  *strID);
	virtual 	void SetID(BYTE *pID, int idSize);

	//string viewCETEXT(CERTEXT_ECC * pcertextecc);
	virtual string viewCETEXT();

	//virtual void ISSUE(const unsigned char * btID, int IdSize, const unsigned char * btQchip, int qChipSize);

	//CERTEXT_ECC * getCERTEXT_ECC();

	//virtual void UpdateSDATA();
	virtual int keyinit(int gen);
	virtual int keyfree();

	virtual void UpdateCERT();
	virtual bool  UpdateC1();
	virtual void UpdateTSM();
	virtual void UpdatePRIKEY();
	virtual void UpdateChip();
	//void GETCERT(const unsigned char * btNchip, int nChipSize);
	//void ISSUE(const unsigned char * btID, int IdSize);
	//bool GET_C1(BYTE * C_ECB, BYTE *ID, int nID, BYTE *C, int nC, BYTE *E, int nE, BYTE *S, int nS);
	virtual BYTE*	getHKDevice();
	virtual bool UpdateSign();
};



#endif //#ifndef __CECC__