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

	


//�۷ι� ����
RSA *G_RSAkey_TSM;




#define PRINTF 0
#define RSA_XOR 0
public:
	//////////////////////////////////////// �ʱ�ȭ
	//int RSA_Keyfree();
	//int RSA_Keyinit(int gen);
	int RSA_Keyinit_TSM();
	
	int RSA_KeyGen_TSM();	
	
	
	int generateSN(BYTE *pbOut, DWORD *pdwLenOut);
	
	int processSNRSA(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut);
	
	
	////////////////////////////	�߱�
	
	
	int requestIdRSA( BYTE* pbOut, DWORD* pdwLenOut );
	
	int RSA_Sign_tsm(BYTE* SIGN, BYTE *ID, BYTE *N_chip, RSA *key_tsm);
	
	
	int GenCertRSA(CERT_RSA* cert, BYTE* ID, BYTE* N_chip, RSA* key_tsm);
	
	int requestCertRSA(BYTE* pbIn, int iLenIn);
	
	
	int requestRegCertRSA( BYTE* pbOut, DWORD* pdwLenOut);
	
	
	// �߱�
	////////////////////////////////////////////////////////////
	// ���
	
	
	int requestRN(BYTE *pbOut, DWORD *pdwLenOut);
	
	
	int requestSessionRSA(BYTE* pbIn, int iLenIn);
	
	
	
	int requestHS1RSA(BYTE * pbOut, DWORD * pdwLenOut);
	
	
	
	//int requestEHKdevice(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut);
	
	int requestSignRSA(BYTE* pbIn, int iLenIn, BYTE *pbOut, DWORD *pdwLenOut, BYTE* pbOTP);
	
	
	
	
	
	////////// test �� ����ȭ RSA����	
	/*
	 ����  RSA_sim.v�� ������ ������ �����Ƿ� �̿� ���� ����Ʈ���� �ʵ� �����ؼ� FPGA �׽�Ʈ ���
	
	1.	��ȣȭ �Ǵ� ���� ���� : Ĩ���κ��� ���� E�� N�� �̿��ؼ� 2048-bit �޽����� xor�� (E�� 128-bit�̹Ƿ� 16�� �ݺ� �Ŀ� xor); 
		o_data = i_data ^ {16{E}} ^N
	
		(Ĩ���� �ڽ��� ����Ű�� ��ȣȭ �� �ʿ��� �Է��� ����Ű d�̳� �����δ� E�� �Է� �޾� d�� �����ϹǷ�
			Ĩ���� ��ȣȭ �� �Է��� ����� ���Դ��� �׽�Ʈ�ϱ� ���ؼ��� E�� N�� ���Ǹ� ��);
	
	2.	��ȣȭ �Ǵ� ���� ���� : TSM�� E�� N�� �̿��Ͽ� 2048-bit �޽����� xor(���������� E�� 16�� �ݺ� �� xor);
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