
#include <iostream>
#include <map>
#include "rs232.h"
#include"comdef.h"
#include "Util.h"
#include "CRsa.h"
#include "CtrlRSA.h"

#include "neoCoLib.h"


#ifndef _WIN32
#include <sys/time.h>
#endif



void CCtrlRSA::genCMD_REQ_PUK_RSA(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	crsa.requestIdRSA(pbuff, (unsigned long*)&size);
}


void CCtrlRSA::postProcCMD_REQ_PUK_RSA(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	crsa.requestCertRSA(pbuff, size);
}
void CCtrlRSA::genCMD_REQ_REG_CERT_RSA(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	crsa.requestRegCertRSA(pbuff, (unsigned long*)&size);
}

void CCtrlRSA::genCMD_REQ_AUTH_RSA(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	crsa.requestRN(pbuff, (unsigned long*)&size);
	//strlog += crsa.flushLog();
}
void CCtrlRSA::postProcCMD_REQ_AUTH_RSA(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	crsa.requestSessionRSA(pbuff, size);
}
void CCtrlRSA::genCMD_REQ_HS1_RSA(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	crsa.requestHS1RSA(pbuff, (unsigned long*)&size);
}

void CCtrlRSA::genCMD_REQ_EHK(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	
}



void CCtrlRSA::postProcCMD_REQ_REG_CERT_RSA(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	
}

void CCtrlRSA::postProcCMD_REQ_HS1_RSA(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	
}
void CCtrlRSA::postProcCMD_REQ_EHK(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	
}

void CCtrlRSA::postProcCMD_REQ_SIGN(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	crsa.requestSignRSA(pbuff, size, g_InBuf,
		(unsigned long*)&g_lenIn,
		m_cryptostrparam.abOTP);

}