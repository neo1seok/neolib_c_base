
#include <iostream>
#include <map>
#include "rs232.h"
#include"comdef.h"
#include "Util.h"
#include "CEcc.h"
#include "CtrlECC.h"

#include "neoCoLib.h"

#ifndef _WIN32
#include <sys/time.h>
#endif


/*
CMD_REQ_TEST
CMD_ERR_TEST
CMD_REQ_SEL_PUF
CMD_REQ_CONFIG
CMD_REQ_SN
CMD_REQ_PUK_ECC
CMD_REQ_REG_CERT_ECC
CMD_REQ_AUTH_ECC
CMD_REQ_HS1_ECC
CMD_ERR_HS2_ECC
CMD_REQ_EHK
CMD_REQ_SIGN
*/


//extern CCtrlECC * pThis;
//{cmd_,s_,r_,#cmd_,gen##cmd_,postProc##cmd_}
//CCtrlECC *_pThis = null(CCtrlECC *)param;

//#define CMDDEF(cmd_,r_,dt_) {cmd_,-1,r_,dt_,#cmd_,genDefCMD,procDefCMD}
//#define CMDSYSTEM(cmd_,dt_) {cmd_,-1,2,dt_,#cmd_,genDefCMD,procDefCMD}

//#//define CMDWITHSUBCMD(cmd_,e_,r_,dt_) {cmd_,e_,r_,dt_,#cmd_,genErrCMD,procDefCMD}

//#define CMDTABLE(cmd_,r_,dt_) {cmd_,-1,r_,dt_,#cmd_,gen##cmd_,procDefCMD}
//#define CMDTABLE_EXT(cmd_,r_,dt_) {cmd_,-1,r_,dt_,#cmd_,gen##cmd_,postProc##cmd_}

//#define REQPRE cecc.







void CCtrlECC::genCMD_REQ_TEST(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	
	CCtrlBASE::genCMD_REQ_TEST(cmd, pbuff, size, strlog, param);
	cecc.SetID(m_cryptostrparam.txtID.c_str());

	
	//memset(pbuff+1, 0x78, 16);

	size = 17;


}
















void CCtrlECC::genCMD_REQ_PUK_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	cecc.requestIdECC(pbuff, g_lenIn, pbuff, (unsigned long*)&size);
	strlog += cecc.flushLog();
}
void CCtrlECC::postProcCMD_REQ_PUK_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	
	//pbuff[1] = Convert2Cmd(pbuff);

	
	cecc.requestCertECC(pbuff, size, g_InBuf, (unsigned long*)&g_lenIn);
	//strlog += cecc.flushLog();

}









void CCtrlECC::genCMD_REQ_REG_CERT_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	//(unsigned long*)&size
	
	unsigned long len =0;
	cecc.requestRegCertECC(g_InBuf, 0, pbuff, (unsigned long*)&size);
	
	//strlog += cecc.flushLog();

}





void CCtrlECC::genCMD_REQ_AUTH_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	unsigned long len =0;
	//requestRNECC(pbuff, &g_lenOut, 0);
	
	cecc.requestRNECC(pbuff,(unsigned long*)&size);
	//strlog+= cecc.flushLog();


}

void CCtrlECC:: postProcCMD_REQ_AUTH_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	cecc.requestSessionECC(pbuff, size);
	//strlog+= cecc.flushLog();
}




void CCtrlECC::genCMD_REQ_HS1_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	cecc.requestHS1ECC(pbuff, (unsigned long*)&size);	
	//strlog+= cecc.flushLog();
}

void CCtrlECC::postProcCMD_REQ_HS1_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	cecc.requestHS2ECC(pbuff, size);
	//strlog += cecc.flushLog();

}





void CCtrlECC::genCMD_ERR_HS2_ECC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genErrCMD(cmd,pbuff,size,strlog,param);
}






void CCtrlECC::postProcCMD_REQ_SIGN(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	
	cecc.requestSignECC(pbuff, size, g_InBuf, 
		(unsigned long*)&g_lenIn, 
		m_cryptostrparam.abOTP);

	//strlog += cecc.flushLog();

}



void CCtrlECC::postProcCMD_ERR_HS2_ECC(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param){

}


void CCtrlECC::postProcCMD_REQ_REG_CERT_ECC(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param){
}

















