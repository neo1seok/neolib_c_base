#ifndef __CRSA__
#define __CRSA__
#include <openssl/rsa.h>
#include<string>
#include "IRSD1410_DEFINE.h" 
#include "CBaseCrypt.h"




class CRsa: public CBaseCrypt
{

public:
    CRsa(void);
    ~CRsa(void);

	


//글로벌 변수
RSA *G_RSAkey_TSM;




#define PRINTF 0
#define RSA_XOR 0
public:
	//////////////////////////////////////// 초기화
	//int RSA_Keyfree();
	//int RSA_Keyinit(int gen);
	int RSA_Keyinit_TSM();
	
	int RSA_KeyGen_TSM();	
	
	
	int generateSN(BYTE *pbOut, DWORD *pdwLenOut);
	
	int processSNRSA(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut);
	
	
	////////////////////////////	발급
	
	
	int requestIdRSA( BYTE* pbOut, DWORD* pdwLenOut );
	
	int RSA_Sign_tsm(BYTE* SIGN, BYTE *ID, BYTE *N_chip, RSA *key_tsm);
	
	
	int GenCertRSA(CERT_RSA* cert, BYTE* ID, BYTE* N_chip, RSA* key_tsm);
	
	int requestCertRSA(BYTE* pbIn, int iLenIn);
	
	
	int requestRegCertRSA( BYTE* pbOut, DWORD* pdwLenOut);
	
	
	// 발급
	////////////////////////////////////////////////////////////
	// 사용
	
	
	int requestRN(BYTE *pbOut, DWORD *pdwLenOut);
	
	
	int requestSessionRSA(BYTE* pbIn, int iLenIn);
	
	
	
	int requestHS1RSA(BYTE * pbOut, DWORD * pdwLenOut);
	
	
	
	//int requestEHKdevice(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut);
	
	int requestSignRSA(BYTE* pbIn, int iLenIn, BYTE *pbOut, DWORD *pdwLenOut, BYTE* pbOTP);
	
	
	
	
	
	////////// test 용 간소화 RSA연산	
	/*
	 지금  RSA_sim.v의 내용은 다음과 같으므로 이에 따라 소프트웨어 쪽도 수정해서 FPGA 테스트 요망
	
	1.	암호화 또는 서명 검증 : 칩으로부터 받은 E와 N을 이용해서 2048-bit 메시지와 xor함 (E는 128-bit이므로 16번 반복 후에 xor); 
		o_data = i_data ^ {16{E}} ^N
	
		(칩에서 자신의 개인키로 암호화 시 필요한 입력은 개인키 d이나 실제로는 E를 입력 받아 d를 생성하므로
			칩에서 암호화 시 입력이 제대로 들어왔는지 테스트하기 위해서는 E와 N이 사용되면 됨);
	
	2.	복호화 또는 서명 생성 : TSM의 E와 N을 이용하여 2048-bit 메시지와 xor(마찬가지로 E는 16번 반복 후 xor);
		o_data = i_data ^ {16{E}} ^N
	*/
	
	
	int RSA_xor_chip(BYTE* i_data, BYTE* Nc, BYTE* Ec, BYTE* o_data);
	
	int RSA_xor_tsm(BYTE* i_data, BYTE* o_data);

	//CERTEXT_RSA * getCERTEXT_RSA();

	virtual  BYTE*	getHKDevice();

	
	//virtual void ISSUE(const unsigned char * btID, int IdSize);

	//virtual void GETCERT(const unsigned char * btNchip, int nChipSize);

	bool  GET_C1(BYTE * c1,BYTE *ID, int nID, BYTE *C, int nC, BYTE *E, int nE, BYTE *S, int nS);
	
	virtual void SetID(const char  *strID);
	virtual 	void SetID(BYTE *pID, int idSize);
	virtual 	bool		SetRN(const char * hexRN);

	
	bool SetDIGITSign(BYTE * E_prime, BYTE *C_otp,BYTE *S_prime, BYTE* pbOTP);
	virtual string viewCETEXT();

	bool VerifySign(BYTE * E, BYTE * N, BYTE * EM, BYTE * sign);

	
	//virtual void UpdateSDATA();
	virtual int keyinit(int gen);
	virtual int keyfree();

	virtual void UpdateCERT();
	virtual bool  UpdateC1();
	virtual void UpdateTSM();
	virtual void UpdatePRIKEY();
	
	virtual bool UpdateSign();
};

#endif __CRSA__