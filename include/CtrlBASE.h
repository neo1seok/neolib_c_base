#pragma once

#include<map>
//#include "main.h"
#include "CBaseCrypt.h"
#include "IRSD1410_DEFINE.h" 
#include "CSerialBASE.h"

using namespace std;


#define BUFFSIZE 4096
#define     COMM_DEF_TIME                   200
#define     COMM_TWICE_TIME                   400
//#define     COMM_REQUEST_TIME               500


#define CMDDEF(cmd_,r_,dt_) {cmd_,-1,r_,dt_,#cmd_,tmp_genDefCMD,tmp_postProcDefCMD}

#define CMDSYSTEM(cmd_,dt_) {cmd_,-1,2,dt_,#cmd_,tmp_genDefCMD,tmp_postProcDefCMD}

#define CMDWITHSUBCMD(cmd_,e_,r_,dt_) {cmd_,e_,r_,dt_,#cmd_,tmp_genErrCMD,tmp_postProcDefCMD}

#define CMDTABLE(cmd_,r_,dt_) {cmd_,-1,r_,dt_,#cmd_,tmp_gen##cmd_,tmp_postProcDefCMD}
#define CMDJUSTHARDCODING(cmd_,r_,dt_) {cmd_,-1,r_,dt_,#cmd_,tmp_genHardCoding,tmp_postProcDefCMD}
#define CMDTABLE_EXT(cmd_,r_,dt_) {cmd_,-1,r_,dt_,#cmd_,tmp_gen##cmd_,tmp_postProc##cmd_}


#define FUNC_CLASSGENDEC(cmd_)  void gen##cmd_(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param);
#define FUNC_CLASSGPOSTDEC(cmd_) void postProc##cmd_(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param)


#define FUNCDEC(cmd_) void tmp_gen##cmd_(void *pThis,int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param);void tmp_postProc##cmd_( void *pThis,int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param);
#define FUNCDEF(genpost_,class_,cmd_) void tmp_##genpost_##cmd_(void *pThis,int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param){	 ((class_*)pThis)->genpost_##cmd_(cmd, pbuff, size, strlog, param);};


FUNCDEC(ErrCMD);
FUNCDEC(DefCMD);
FUNCDEC(CMD_REQ_TEST);
FUNCDEC(CMD_REQ_TEST);
FUNCDEC(CMD_ERR_TEST);
FUNCDEC(CMD_REQ_SEL_PUF);
FUNCDEC(CMD_REQ_CONFIG);
FUNCDEC(CMD_REQ_SN);
FUNCDEC(CMD_REQ_EHK);
FUNCDEC(CMD_REQ_SIGN);

FUNCDEC(CMD_REQ_CTR);
FUNCDEC(CMD_REQ_ENC_CCM);
FUNCDEC(CMD_REQ_HMAC);
FUNCDEC(CMD_REQ_CRYPTO_CONT);
FUNCDEC(CMD_REQ_HMAC_CONT);
FUNCDEC(CMD_REQ_CCM_GENERATE_MAC);
FUNCDEC(CMD_REQ_GENERATE_HMAC);
FUNCDEC(CMD_REQ_DEC_CCM);
FUNCDEC(CMD_REQ_CCM_VERIFY_MAC);
FUNCDEC(CMD_REQ_VERIFY_HMAC);


FUNCDEC(DefCMD);
//FUNCPOSTDEF(CMD_REQ_TEST);
//FUNCPOSTDEF(CMD_ERR_TEST);
//FUNCPOSTDEF(CMD_REQ_SEL_PUF);
//FUNCPOSTDEF(CMD_REQ_CONFIG);
//FUNCPOSTDEF(CMD_REQ_SN);

//FUNCPOSTDEF(CMD_REQ_REG_CERT_ECC);




FUNCDEC(CMD_REQ_PUK_ECC);
FUNCDEC(CMD_REQ_REG_CERT_ECC);
FUNCDEC(CMD_REQ_AUTH_ECC);
FUNCDEC(CMD_REQ_HS1_ECC);
FUNCDEC(CMD_ERR_HS2_ECC);

FUNCDEC(CMD_REQ_PUK_RSA);
FUNCDEC(CMD_REQ_REG_CERT_RSA);
FUNCDEC(CMD_REQ_AUTH_RSA);
FUNCDEC(CMD_REQ_HS1_RSA);







typedef void(*PFPROCESS)(void *pThis, int cmd, BYTE * pbuff, int & size, std::string & strlog,  void* param);

struct CMDINFO{
	int cmd;
	int subcmd;
	int retSize;
	int recvDelayTime;
	const char * cmdname;
	PFPROCESS pfgenbuffer;
	PFPROCESS pfpostprocess;

};
enum BLOCKCRYPT_CMDTYPE{
	BLOCKCRYPT_ENC_CTR,
	BLOCKCRYPT_ENC_CCM,
	BLOCKCRYPT_ENC_HMAC,
	BLOCKCRYPT_DEC_STR,
	BLOCKCRYPT_DEC_CCM,
	BLOCKCRYPT_DEC_HMAC,
	BLOCKCRYPT_MAX_NUM

};

struct ENCBLOCKINFO{
	BLOCKCRYPT_CMDTYPE cmdtype;
	int cmd;
	int nextCmd;
	int lastCmd;
	int blocklength;
	bool isVerify;

	BYTE * pInBuff;
	BYTE * pOutBuff;
	int datalength;
	int outlength;
	int remainsize;
};

struct CYPTORPARAM{
	string txtSEL_PUF;
	string txtCONFIG;
	string txtID;
	string txtChipEnc;
	string txtTSMEnc;
	string txtECCOTP;
	string txtabOTP;
	string txtDATA;
	string txtQChip;
	string txtNChip;
	string txtSN;
	BYTE abOTP[8];
	int len_abOTP;

};

#if 0
struct INTER_STR_PARAM{
	string txtSEL_PUFECC;
	string txtCONFIGECC;
	string txtIDECC;
	string txtChipEncECC;
	string txtTSMEncECC;
	string txtECCOTP;
	string txtabOTP;
	string txtDATA;
	string txtQChip;
	string txtSN;
	BYTE abOTP[8];
	int len_abOTP;

};
#endif

#if 0
struct ECCINTERPARAM{
	BYTE abOTP[8];
	//BYTE qChip[64];
	BYTE ID[64];
	BYTE SDATA[64];

	int len_abOTP;
	int len_qChip;
	int len_ID;
	int len_SDATA;
};

#endif
class CCtrlBASE
{
public:
	CCtrlBASE(void);
	~CCtrlBASE(void);
	virtual bool init(CSerialBASE * pserialBase);

	int runCMDINFO(int cmd, int sleep = 100 , void* param = NULL, BYTE *pSendBuff = NULL, int sendbuffsize = 0, BYTE *pRecvBuff = NULL, int *precvbuffsize = NULL);
	int runSync80();
	static void Sleep(int msec);
	void TestMT(int param);
	
	int sleepChip();
	int wakeupChip();
	int eraseChip();
	int restartChip();
	void HWReset();

	//void PreIssue();
	virtual void PreIssue(BYTE *pID = NULL, int idsize = 0, BYTE*pQChip = NULL, int qchipSize = 0) {
	}
	virtual void PreIssue(string id,string pubchip) {
	}

	virtual void AfterAuth(BYTE * pbData, int iLength, BYTE * pbOut, int *piLengthOut) {

	}

//	virtual void close();
//	virtual bool open(int portindex, int bdrate);
	virtual int Write(unsigned char * pbuff, int length);
	virtual int Read(unsigned char * pbuff, int length);
//	virtual bool isopen();
//	const char *  getportnmae();


	void ViewLog();
	void ViewLogP(string  title,string  log);

	string getCMDINFOTitle(int cmd);

	int runBlockCrypt(BLOCKCRYPT_CMDTYPE cmdtype, BYTE * pinput, int inputsize, BYTE ** ppoutbuffer);

	void SetPARAM(CYPTORPARAM *pcryptostrparam);

	CYPTORPARAM * GetPARAM();

	std::string m_strlastmsg;
	//ECCSTRPARAM m_eccstrparam;
	//ECCINTERPARAM m_eccinterparam;
	
	BYTE * getLogBuffer();
	//void SetID(BYTE *pID,int idSize);
	//void SetID(const char  *strID);

	virtual void SetPARAM(string id, string qchip, string txtSEL_PUFECC);





	BYTE g_LogBuf[102400];
	
	BYTE g_InBuf[65600];
	long g_lenIn;

	BYTE g_testTemp[65600];
	long g_lenInTestTemp;
	map<int,string> _tesstvalue;

	BYTE g_Input[BUFFSIZE];
	BYTE g_Output[BUFFSIZE];
	
	

	CYPTORPARAM m_cryptostrparam;
	CBaseCrypt *getcrypto(){
		return pbaseCrypt;
	}

	CSerialBASE * getserial(){
		return pserialBase;
	}
	

	

protected:

	CBaseCrypt *pbaseCrypt;
	CSerialBASE * pserialBase;

	unsigned int getTickCount();

	void SerialWrite(BYTE *lpbuffer,int length);
	bool SerialReceive(BYTE *lpbuffer,int & length,int delaytime);

	void WriteLogLine(const char * fmt, ...);
	void WriteLog(const char * fmt, ...);


	std::string PrintfBuff(BYTE * pbuff,int  size);

	

	
	string getStr2HexStr(string str, int length);
	
	//int m_portindex;
	int m_delayTime;
	

	std::map<int,CMDINFO> m_maptable;
	map<BLOCKCRYPT_CMDTYPE, ENCBLOCKINFO> mapblockcrypt;
	
	//INTER_STR_PARAM m_eccstrparam;

public  :
	static void calculate(int datalength, int blocklength, int & reallength, int & realn);

	static void Convert2TBCmd(BYTE * ptbcmd,BYTE cmd);
	static BYTE Convert2Cmd(BYTE * ptbcmd);
	static bool isTBCmd(BYTE * ptbcmd);
	

	static void Test(int param);	
	
	static int Hex2ArrayBin(string strD, BYTE * pbyte);
	
	void genDefCMD(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param=NULL);

	void genCMD_REQ_DEFCRypt(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param);

	void genCMD_REQ_DEFCRyptCont(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param);


	void postProcDefCMD(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param);

	void genHardCoding(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param);

	void genErrCMD(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param);
	
	
	
	static void postSyn80(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param);


	FUNC_CLASSGENDEC(CMD_REQ_TEST);
	FUNC_CLASSGENDEC(CMD_ERR_TEST);
	FUNC_CLASSGENDEC(CMD_REQ_SEL_PUF);
	FUNC_CLASSGENDEC(CMD_REQ_CONFIG);
	FUNC_CLASSGENDEC(CMD_REQ_SN);
	FUNC_CLASSGENDEC(CMD_REQ_EHK);
	FUNC_CLASSGENDEC(CMD_REQ_SIGN);
	FUNC_CLASSGENDEC(CMD_REQ_CTR);
	FUNC_CLASSGENDEC(CMD_REQ_ENC_CCM);
	FUNC_CLASSGENDEC(CMD_REQ_HMAC);
	FUNC_CLASSGENDEC(CMD_REQ_CRYPTO_CONT);
	FUNC_CLASSGENDEC(CMD_REQ_HMAC_CONT);
	FUNC_CLASSGENDEC(CMD_REQ_CCM_GENERATE_MAC);
	FUNC_CLASSGENDEC(CMD_REQ_GENERATE_HMAC);
	FUNC_CLASSGENDEC(CMD_REQ_DEC_CCM);
	FUNC_CLASSGENDEC(CMD_REQ_CCM_VERIFY_MAC);
	FUNC_CLASSGENDEC(CMD_REQ_VERIFY_HMAC);


	FUNC_CLASSGPOSTDEC(CMD_REQ_TEST);
	FUNC_CLASSGPOSTDEC(CMD_ERR_TEST);
	FUNC_CLASSGPOSTDEC(CMD_REQ_SEL_PUF);
	FUNC_CLASSGPOSTDEC(CMD_REQ_CONFIG);
	FUNC_CLASSGPOSTDEC(CMD_REQ_SN);
	FUNC_CLASSGPOSTDEC(CMD_REQ_EHK);
	virtual FUNC_CLASSGPOSTDEC(CMD_REQ_SIGN);
	FUNC_CLASSGPOSTDEC(CMD_REQ_CTR);
	FUNC_CLASSGPOSTDEC(CMD_REQ_ENC_CCM);
	FUNC_CLASSGPOSTDEC(CMD_REQ_HMAC);
	FUNC_CLASSGPOSTDEC(CMD_REQ_CRYPTO_CONT);
	FUNC_CLASSGPOSTDEC(CMD_REQ_HMAC_CONT);
	FUNC_CLASSGPOSTDEC(CMD_REQ_CCM_GENERATE_MAC);
	FUNC_CLASSGPOSTDEC(CMD_REQ_GENERATE_HMAC);
	FUNC_CLASSGPOSTDEC(CMD_REQ_DEC_CCM);
	FUNC_CLASSGPOSTDEC(CMD_REQ_CCM_VERIFY_MAC);
	FUNC_CLASSGPOSTDEC(CMD_REQ_VERIFY_HMAC);



	

	void postProcCMD_REQ_DEFCRypt(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param);
		

#if 0
	static void genCMD_REQ_TEST(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);


	static void genCMD_ERR_TEST(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

	static void genCMD_REQ_SEL_PUF(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

	static void genCMD_REQ_HS1_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

	static void genCMD_REQ_CONFIG(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

	static void genCMD_REQ_SN(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);


	static void genCMD_REQ_PUK_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	

	static void genCMD_REQ_REG_CERT_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

	static void genCMD_REQ_AUTH_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);


	static void genCMD_ERR_HS2_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

	static void genCMD_REQ_EHK(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

	static void genCMD_REQ_SIGN(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);



	static void genCMD_REQ_CTR(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void genCMD_REQ_ENC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void genCMD_REQ_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void genCMD_REQ_CRYPTO_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void genCMD_REQ_HMAC_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void genCMD_REQ_CCM_GENERATE_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void genCMD_REQ_GENERATE_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void genCMD_REQ_DEC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void genCMD_REQ_CCM_VERIFY_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void genCMD_REQ_VERIFY_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

	

	static void postProcCMD_REQ_CTR(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_ENC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_CRYPTO_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_HMAC_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_CCM_GENERATE_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_GENERATE_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_DEC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_CCM_VERIFY_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_VERIFY_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

	
	
	static void postProcCMD_REQ_AUTH_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_PUK_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	
	static void postProcCMD_REQ_HS1_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_EHK(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);
	static void postProcCMD_REQ_SIGN(int cmd, BYTE * pbuff,int & size,std::string &strlog,CCtrlBASE*pthis,void* param);

public:
#endif



	
};
