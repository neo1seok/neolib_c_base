
#include <iostream>
#include <map>
#include "rs232.h"
#include"comdef.h"
#include "Util.h"

#include "CtrlBASE.h"

#include "neoCoLib.h"

#ifndef _WIN32
#include <sys/time.h>
#endif




void CCtrlBASE::genDefCMD(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param){
	pbuff[0] = cmd;
	size = 1;
}

void CCtrlBASE::postProcDefCMD(int cmd, BYTE * pbuff, int & size, std::string &strlog,  void* param){
}

void CCtrlBASE::genErrCMD(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param){

	CMDINFO cmdinfo = m_maptable[cmd];
	

	pbuff[0] = cmd;
	pbuff[1] = cmdinfo.subcmd;
	size = 2;
}

//void CCtrlBASE::genSyn80(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
//{
//	genDefCMD(cmd,pbuff,size,strlog,param);
//}
void CCtrlBASE::postSyn80(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
}

void CCtrlBASE::genCMD_REQ_TEST(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	int num = Hex2ArrayBin(m_cryptostrparam.txtSN, pbuff + 1);

	pbaseCrypt->SetID(m_cryptostrparam.txtID.c_str());
	//memset(pbuff+1, 0x78, 16);

	size = 17;


}
void CCtrlBASE::postProcCMD_REQ_TEST(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	int num = Hex2ArrayBin(m_cryptostrparam.txtSN, pbuff + 1);

	

	//memset(pbuff+1, 0x78, 16);

	size = 17;


}




void CCtrlBASE::genCMD_ERR_TEST(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genErrCMD(cmd,pbuff,size,strlog,param);
}





void CCtrlBASE::genCMD_REQ_SEL_PUF(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	
	genDefCMD(cmd,pbuff,size,strlog,param);
	int num = Hex2ArrayBin(m_cryptostrparam.txtSEL_PUF, pbuff + 1);
	size = 1 + num;

}




void CCtrlBASE::genCMD_REQ_CONFIG(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genDefCMD(cmd,pbuff,size,strlog,param);
	int num = Hex2ArrayBin(m_cryptostrparam.txtCONFIG, pbuff + 1);
	size = 1 + num;
}



void CCtrlBASE::genCMD_REQ_SN(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genDefCMD(cmd,pbuff,size,strlog,param);
}





void CCtrlBASE::genCMD_REQ_EHK(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	
	genDefCMD(cmd,pbuff,size,strlog,param);
}

void CCtrlBASE::postProcCMD_REQ_EHK(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	pbaseCrypt->requestEHKdevice(pbuff, size, g_InBuf, (unsigned long*)&g_lenIn);			//++ error


}

//오류	19	error LNK2001 : "public: virtual void __thiscall CCtrlBASE::postProcCMD_ERR_TEST(int,unsigned char *,int &,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > &,void *)" (? postProcCMD_ERR_TEST@CCtrlBASE@@UAEXHPAEAAHAAV ? $basic_string@DU ? $char_traits@D@std@@V ? $allocator@D@2@@std@@PAX@Z) 외부 기호를 확인할 수 없습니다.D : \PROJECT\POWERPC\SRC\eccproj\ECCConsole\libIRSD - 1410.lib(CtrlBASE.obj)	ECCConsole






void CCtrlBASE::genCMD_REQ_SIGN(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	
	genDefCMD(cmd,pbuff,size,strlog,param);
	for (int i = 0; i < 3; i++)
	{
		//CCtrlBASE::m_eccinterparam.abOTP[i] = (BYTE)Convert::ToInt32(CCtrlBASE::m_cryptostrparam.txtECCOTP.Substring(i * 2, 2), 16);
		pbuff[i + 1] = m_cryptostrparam.abOTP[i];
		//strRd += abOTP[i].ToString("X2");
	}

	size = 4;
}

void CCtrlBASE::postProcCMD_REQ_SIGN(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	
	



}


void CCtrlBASE::genHardCoding(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{

	string str = _tesstvalue[cmd];

	size = hex2bin(str.c_str(),pbuff);

	strlog += "SEND:";
	strlog += str;




}
void CCtrlBASE::genCMD_REQ_DEFCRypt(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genDefCMD(cmd,pbuff++,size,strlog);
	ENCBLOCKINFO *pencpblocinfo = (ENCBLOCKINFO *)param;
#if 0
	pencpblocinfo->datalength;
	pencpblocinfo->blocklength;
	int n = pencpblocinfo->datalength / pencpblocinfo->blocklength;
	int r = pencpblocinfo->datalength % pencpblocinfo->blocklength;
	int realn = n + pencpblocinfo->blocklength + ((r > 0) ? 1 : 0);
	int totalsize = realn*pencpblocinfo->blocklength;
#endif
	int totalsize = 0;
	int realn = 0;
	

	calculate(pencpblocinfo->datalength, pencpblocinfo->blocklength, totalsize, realn);


	size = 1 + 2 + pencpblocinfo->blocklength;
	
	
	*pbuff++ = (BYTE)(totalsize >> 8);
	*pbuff++ = (BYTE)(totalsize);

	BYTE * pbyte = pencpblocinfo->pInBuff;

	memcpy(pbuff, pencpblocinfo->pInBuff, min(pencpblocinfo->blocklength, pencpblocinfo->remainsize));


	


}


void CCtrlBASE::genCMD_REQ_DEFCRyptCont(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genDefCMD(cmd,pbuff++,size,strlog);
	ENCBLOCKINFO *pencpblocinfo = (ENCBLOCKINFO *)param;
	size = 1 + pencpblocinfo->blocklength;

	memcpy(pbuff, pencpblocinfo->pInBuff, min(pencpblocinfo->blocklength, pencpblocinfo->remainsize));
	


}

void CCtrlBASE::genCMD_REQ_CTR(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{

	genCMD_REQ_DEFCRypt(cmd,pbuff,size,strlog,param);
}
void CCtrlBASE::genCMD_REQ_ENC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genCMD_REQ_DEFCRypt(cmd,pbuff,size,strlog,param);
}
void CCtrlBASE::genCMD_REQ_DEC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genCMD_REQ_DEFCRypt(cmd,pbuff,size,strlog,param);
}

void CCtrlBASE::genCMD_REQ_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genCMD_REQ_DEFCRypt(cmd,pbuff,size,strlog,param);
}


void CCtrlBASE::genCMD_REQ_CRYPTO_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genCMD_REQ_DEFCRyptCont(cmd,pbuff,size,strlog,param);
}

void CCtrlBASE::genCMD_REQ_HMAC_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genCMD_REQ_DEFCRyptCont(cmd,pbuff,size,strlog,param);
}

void CCtrlBASE::genCMD_REQ_CCM_GENERATE_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genDefCMD(cmd,pbuff,size,strlog);
}

void CCtrlBASE::genCMD_REQ_GENERATE_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genDefCMD(cmd,pbuff,size,strlog);
}



void CCtrlBASE::genCMD_REQ_CCM_VERIFY_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genCMD_REQ_DEFCRyptCont(cmd,pbuff,size,strlog,param);
}

void CCtrlBASE::genCMD_REQ_VERIFY_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	genCMD_REQ_DEFCRyptCont(cmd,pbuff,size,strlog,param);
}


void CCtrlBASE::postProcCMD_REQ_DEFCRypt(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{
	ENCBLOCKINFO *pencpblocinfo = (ENCBLOCKINFO *)param;

	;
	pencpblocinfo->outlength = size - 1;

	memcpy(pencpblocinfo->pOutBuff, pbuff+1,size-1);
}

void CCtrlBASE::postProcCMD_REQ_CTR(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	postProcCMD_REQ_DEFCRypt(cmd, pbuff, size, strlog,param);
}

void CCtrlBASE::postProcCMD_REQ_ENC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	postProcCMD_REQ_DEFCRypt(cmd, pbuff, size, strlog,param);
}

void CCtrlBASE::postProcCMD_REQ_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	postProcCMD_REQ_DEFCRypt(cmd, pbuff, size, strlog,param);
}

void CCtrlBASE::postProcCMD_REQ_CRYPTO_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	postProcCMD_REQ_DEFCRypt(cmd, pbuff, size, strlog,param);
}

void CCtrlBASE::postProcCMD_REQ_HMAC_CONT(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	postProcCMD_REQ_DEFCRypt(cmd, pbuff, size, strlog,param);
}

void CCtrlBASE::postProcCMD_REQ_CCM_GENERATE_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	postProcCMD_REQ_DEFCRypt(cmd, pbuff, size, strlog,param);
}

void CCtrlBASE::postProcCMD_REQ_GENERATE_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	postProcCMD_REQ_DEFCRypt(cmd, pbuff, size, strlog,param);
}

void CCtrlBASE::postProcCMD_REQ_DEC_CCM(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	postProcCMD_REQ_DEFCRypt(cmd, pbuff, size, strlog,param);
}

void CCtrlBASE::postProcCMD_REQ_CCM_VERIFY_MAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
	postProcCMD_REQ_DEFCRypt(cmd, pbuff, size, strlog,  param);


}

void CCtrlBASE::postProcCMD_REQ_VERIFY_HMAC(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param)
{
}


void CCtrlBASE::postProcCMD_ERR_TEST(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{

}

void CCtrlBASE::postProcCMD_REQ_CONFIG(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param){

}

void CCtrlBASE::postProcCMD_REQ_SEL_PUF(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param){

}

void CCtrlBASE::postProcCMD_REQ_SN(int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param)
{

}













