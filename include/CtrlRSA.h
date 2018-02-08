#pragma once

#include<map>
//#include "main.h"
#include "IRSD1410_DEFINE.h" 
#include"CtrlBASE.h"
#include"CSerialBASE.h"
#include"CRsa.h"

using namespace std;




class CCtrlRSA : public CCtrlBASE
{
public:
	CCtrlRSA(void);
	~CCtrlRSA(void);
	virtual bool init(CSerialBASE * pserialBase);



	
	CRsa crsa;
	
	

public  :
	virtual void PreIssue(BYTE *pID = NULL, int idsize = 0, BYTE*pQChip = NULL, int qchipSize = 0);
	virtual 	void PreIssue(string id, string pubchip);
	virtual void AfterAuth(BYTE * pbData, int iLength, BYTE * pbOut, int *piLengthOut){

	}

	virtual void SetPARAM(string id, string nchip, string txtSEL_PUFECC);

	static void Test(int param);	

	
	FUNC_CLASSGENDEC(CMD_REQ_PUK_RSA);
	
	FUNC_CLASSGENDEC(CMD_REQ_REG_CERT_RSA);
	FUNC_CLASSGENDEC(CMD_REQ_AUTH_RSA);
	FUNC_CLASSGENDEC(CMD_REQ_HS1_RSA);
	FUNC_CLASSGENDEC(CMD_REQ_EHK);
	//FUNC_CLASSGENDEC(CMD_RES_SIGN_RSA);

	FUNC_CLASSGPOSTDEC(CMD_REQ_PUK_RSA);
	
	FUNC_CLASSGPOSTDEC(CMD_REQ_REG_CERT_RSA);
	FUNC_CLASSGPOSTDEC(CMD_REQ_AUTH_RSA);
	FUNC_CLASSGPOSTDEC(CMD_REQ_HS1_RSA);
	FUNC_CLASSGPOSTDEC(CMD_REQ_EHK);
	//FUNC_CLASSGPOSTDEC(CMD_RES_SIGN_RSA);
	virtual FUNC_CLASSGPOSTDEC(CMD_REQ_SIGN);



	



	
};
