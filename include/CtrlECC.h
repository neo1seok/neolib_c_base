#pragma once

#include<map>
//#include "main.h"
#include "IRSD1410_DEFINE.h" 
#include "CtrlBASE.h"
#include"CEcc.h"
#include"CSerialBASE.h"
using namespace std;







class CCtrlECC : public CCtrlBASE
{
public:
	CCtrlECC(void);
	~CCtrlECC(void);
	virtual bool init(CSerialBASE * pserialBase);
	
	
	void TestMT(int param);
	
	
	//void PreIssue();
	virtual void PreIssue(BYTE *pID = NULL, int idsize = 0, BYTE*pQChip = NULL, int qchipSize = 0);
	virtual 	void PreIssue(string id, string pubchip);
	virtual void AfterAuth(BYTE * pbData, int iLength, BYTE * pbOut, int *piLengthOut);


	
	
	


	//std::string m_strlastmsg;
	//
	//ECCINTERPARAM m_eccinterparam;
	
	//BYTE * getLogBuffer();
	//void SetID(BYTE *pID,int idSize);
	//void SetID(const char  *strID);

	virtual void SetPARAM(string id, string qchip, string txtSEL_PUFECC);
	




	
	CEcc cecc;
	
	

	

protected:

	unsigned int getTickCount();

	

	
	
	int m_portindex;
	int m_delayTime;
	

	
	
	
	

public:

	
	
	static int Hex2ArrayBin(string strD, BYTE * pbyte);
	
	
	FUNC_CLASSGENDEC(CMD_REQ_TEST);
	FUNC_CLASSGENDEC(CMD_REQ_PUK_ECC);
	FUNC_CLASSGENDEC(CMD_REQ_SIGN);
	FUNC_CLASSGENDEC(CMD_REQ_REG_CERT_ECC);
	FUNC_CLASSGENDEC(CMD_REQ_AUTH_ECC);
	FUNC_CLASSGENDEC(CMD_REQ_HS1_ECC);
	FUNC_CLASSGENDEC(CMD_ERR_HS2_ECC);
	FUNC_CLASSGENDEC(CMD_REQ_EHK);

	FUNC_CLASSGPOSTDEC(CMD_REQ_TEST);
	FUNC_CLASSGPOSTDEC(CMD_REQ_PUK_ECC);
	virtual FUNC_CLASSGPOSTDEC(CMD_REQ_SIGN);
	FUNC_CLASSGPOSTDEC(CMD_REQ_REG_CERT_ECC);
	FUNC_CLASSGPOSTDEC(CMD_REQ_AUTH_ECC);
	FUNC_CLASSGPOSTDEC(CMD_REQ_HS1_ECC);
	FUNC_CLASSGPOSTDEC(CMD_ERR_HS2_ECC);
	FUNC_CLASSGPOSTDEC(CMD_REQ_EHK);
	
	

	//FUNCPOSTDEC(CMD_REQ_TEST);
	//FUNCPOSTDEC(CMD_ERR_TEST);
	//FUNCPOSTDEC(CMD_REQ_SEL_PUF);
	//FUNCPOSTDEC(CMD_REQ_CONFIG);
	//FUNCPOSTDEC(CMD_REQ_SN);
	//FUNCPOSTDEC(CMD_REQ_PUK_ECC);
	//FUNCPOSTDEC(CMD_REQ_REG_CERT_ECC);
	//FUNCPOSTDEC(CMD_REQ_AUTH_ECC);
	//FUNCPOSTDEC(CMD_REQ_HS1_ECC);
	//FUNCPOSTDEC(CMD_ERR_HS2_ECC);


		

#if 0
	static void genCMD_REQ_TEST(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);


	static void genCMD_ERR_TEST(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

	static void genCMD_REQ_SEL_PUF(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

	static void genCMD_REQ_HS1_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

	static void genCMD_REQ_CONFIG(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

	static void genCMD_REQ_SN(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);


	static void genCMD_REQ_PUK_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	

	static void genCMD_REQ_REG_CERT_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

	static void genCMD_REQ_AUTH_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);


	static void genCMD_ERR_HS2_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

	static void genCMD_REQ_EHK(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

	static void genCMD_REQ_SIGN(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);



	static void genCMD_REQ_CTR(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void genCMD_REQ_ENC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void genCMD_REQ_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void genCMD_REQ_CRYPTO_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void genCMD_REQ_HMAC_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void genCMD_REQ_CCM_GENERATE_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void genCMD_REQ_GENERATE_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void genCMD_REQ_DEC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void genCMD_REQ_CCM_VERIFY_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void genCMD_REQ_VERIFY_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

	

	static void postProcCMD_REQ_CTR(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_ENC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_CRYPTO_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_HMAC_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_CCM_GENERATE_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_GENERATE_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_DEC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_CCM_VERIFY_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_VERIFY_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

	
	
	static void postProcCMD_REQ_AUTH_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_PUK_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	
	static void postProcCMD_REQ_HS1_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_EHK(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);
	static void postProcCMD_REQ_SIGN(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlECC*pthis,void* param);

public:
#endif



	
};
