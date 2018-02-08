
#include <iostream>
#include <map>
#include "rs232.h"
#include "Util.h"

#include "CtrlBASE.h"
#include "commfunc.h"
#include "neoCoLib.h"
#include "neoDebug.h"
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

CMD_REQ_CTR
CMD_REQ_ENC_CCM
CMD_REQ_HMAC
CMD_REQ_CRYPTO_CONT
CMD_REQ_HMAC_CONT
CMD_REQ_CCM_GENERATE_MAC
CMD_REQ_GENERATE_HMAC
CMD_REQ_DEC_CCM
CMD_REQ_CCM_VERIFY_MAC
CMD_REQ_VERIFY_HMAC


*/


//CCtrlBASE * pThis;
//{cmd_,s_,r_,#cmd_,gen##cmd_,postProc##cmd_}
//CCtrlBASE *_pThis = null(CCtrlBASE *)param;












CCtrlBASE::CCtrlBASE(void)
{
	pserialBase = NULL;
	CMDINFO cmdTable[] = {
		{ 0x80, 1, 1, COMM_DEF_TIME, "Sync80", tmp_genDefCMD, tmp_postProcDefCMD },
		//CMDDEF(CMD_REQ_RESTART),
		CMDSYSTEM(CMD_REQ_SLEEP,COMM_TWICE_TIME),
		CMDSYSTEM(CMD_REQ_WAKEUP,COMM_DEF_TIME),
		CMDSYSTEM(CMD_REQ_FLASH_MODE,COMM_DEF_TIME),

		CMDWITHSUBCMD(CMD_FLASH_REQ_ERASE,0x10,1,COMM_DEF_TIME),
	
		CMDTABLE(CMD_REQ_TEST,18,COMM_REQUEST_TIME),
		//CMDTABLE(CMD_ERR_TEST,2,1),
		CMDWITHSUBCMD(CMD_ERR_TEST,0x00,1,COMM_REQUEST_TIME),

		CMDTABLE(CMD_REQ_SEL_PUF,2,COMM_REQUEST_TIME),
		CMDTABLE(CMD_REQ_CONFIG,2,COMM_REQUEST_TIME),
		CMDTABLE(CMD_REQ_SN,18,COMM_REQUEST_TIME),


		CMDTABLE_EXT(CMD_REQ_EHK,33+1,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_SIGN,1 + 16 + 32 + 32+1,COMM_REQUEST_TIME),
		
		CMDTABLE_EXT(CMD_REQ_CTR,17,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_ENC_CCM,17,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_HMAC,1,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_CRYPTO_CONT,17,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_HMAC_CONT,1,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_CCM_GENERATE_MAC,17,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_GENERATE_HMAC,33,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_DEC_CCM,17,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_CCM_VERIFY_MAC,2,COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_VERIFY_HMAC,2,COMM_REQUEST_TIME),
		{-1,0,0,NULL,NULL}

	};

	for(CMDINFO *pcmd = cmdTable ; pcmd->cmd != -1 ; pcmd++){
		m_maptable[pcmd->cmd] = *pcmd;
	}

	ENCBLOCKINFO encbloctablle[] = {
		{ BLOCKCRYPT_ENC_CTR, CMD_REQ_CTR, CMD_REQ_CRYPTO_CONT, -1, 16,false, NULL, NULL, 0 },
		{ BLOCKCRYPT_ENC_CCM, CMD_REQ_ENC_CCM, CMD_REQ_CRYPTO_CONT, CMD_REQ_CCM_GENERATE_MAC, 16, false, NULL, NULL, 0 },
		{ BLOCKCRYPT_ENC_HMAC, CMD_REQ_HMAC, CMD_REQ_HMAC_CONT, CMD_REQ_GENERATE_HMAC, 64, false, NULL, NULL, 0 },
		{ BLOCKCRYPT_DEC_STR, CMD_REQ_CTR, CMD_REQ_CRYPTO_CONT, -1, 16, false, NULL, NULL, 0 },
		{ BLOCKCRYPT_DEC_CCM, CMD_REQ_DEC_CCM, CMD_REQ_CRYPTO_CONT, CMD_REQ_CCM_VERIFY_MAC, 16, true, NULL, NULL, 0 },
		{ BLOCKCRYPT_DEC_HMAC, CMD_REQ_HMAC, CMD_REQ_HMAC_CONT, CMD_REQ_VERIFY_HMAC, 64, true, NULL, NULL, 0 },
		{ BLOCKCRYPT_MAX_NUM, -1, -1, 0, NULL, NULL, 0 }
	};

	
	for (ENCBLOCKINFO * pencblockinfo = encbloctablle; pencblockinfo->cmdtype != BLOCKCRYPT_MAX_NUM; pencblockinfo++){
		mapblockcrypt[pencblockinfo->cmdtype] = *pencblockinfo;
	}


									//0100014a
	_tesstvalue[CMD_REQ_SEL_PUF]="0100014A";
	_tesstvalue[CMD_REQ_CONFIG]="020101";
	_tesstvalue[CMD_REQ_SN]="03";
	_tesstvalue[CMD_REQ_PUK_ECC]="3531323331323300000000000000000000FFB4D4DE13FB94D336565F26C9D00DDB262805E04DE35A96BE95C6383BAC45EB02585FDBA42D56674FA1FFE4A32677D731C122CFB5FE07A37B9DBEEF0B4C8B98A53EDA";
	_tesstvalue[CMD_REQ_REG_CERT_ECC]="3731323331323300000000000000000000030EF9ACCF5C34B2727204A491CF5504A25B9E95D289E37AACFFB49A64C8F6C104CC7E800C21233D31D9EB3D813A6624DD089E9B48A000D613DB82A77694A3B538882923793F5AAADC359F5505690A9D53B625C4CDD86D9C35098C2AF588CDB1DC95BD";
	_tesstvalue[CMD_REQ_AUTH_ECC]=	"39FFB4D4DE13FB94D336565F26C9D00DDB262805E04DE35A96BE95C6383BAC45EB4646464646464646464646464646464600000000000000000000000000000000";
	//								"39FFB4D4DE13FB94D336565F26C9D00DDB262805E04DE35A96BE95C6383BAC45EB4646464646464646464646464646464600000000000000000000000000000000"

	_tesstvalue[CMD_REQ_HS1_ECC]="3B03341411C04DC71F7362217206686F8DB6C23D5B21EE8DC31FA9EBA9EDD0353A0B5FBFCA85E8BAA154BA777C84D768782742CABBE2335D2AFC4AE4944C0A9A42C1CA1AF4D923C7FAF94240CFBBD66F06193D7656816AEC63AA58D80A1EBFCCF86167F06295A187DD8E1714C7354A8D3DC1";
	pbaseCrypt = new CBaseCrypt();
	memset(g_LogBuf,0,102400);
}
//CYPTORPARAM m_cryptostrparam;
void CCtrlBASE::SetPARAM(CYPTORPARAM *pcryptostrparam)
{

	this->m_cryptostrparam.txtSEL_PUF = pcryptostrparam->txtSEL_PUF;
	this->m_cryptostrparam.txtCONFIG = pcryptostrparam->txtCONFIG;;
	this->m_cryptostrparam.txtID = pcryptostrparam->txtID;;
	this->m_cryptostrparam.txtChipEnc = pcryptostrparam->txtChipEnc;
	this->m_cryptostrparam.txtTSMEnc = pcryptostrparam->txtTSMEnc;;
	this->m_cryptostrparam.txtECCOTP = pcryptostrparam->txtECCOTP;;
	this->m_cryptostrparam.txtabOTP = pcryptostrparam->txtabOTP;;
	this->m_cryptostrparam.txtDATA = pcryptostrparam->txtDATA;;
	this->m_cryptostrparam.txtQChip = pcryptostrparam->txtQChip;;
	this->m_cryptostrparam.txtNChip = pcryptostrparam->txtNChip;;

	this->m_cryptostrparam.txtSN = pcryptostrparam->txtSN;;
	memcpy(this->m_cryptostrparam.abOTP, pcryptostrparam->abOTP, sizeof(pcryptostrparam->abOTP));;
	this->m_cryptostrparam.len_abOTP = pcryptostrparam->len_abOTP;;
	



}

CYPTORPARAM * CCtrlBASE::GetPARAM()
{

	return &m_cryptostrparam;



}

CCtrlBASE::~CCtrlBASE(void)
{
}




string CCtrlBASE::getStr2HexStr(string str, int length){
	string ret = "";
	BYTE * pbyte = (BYTE *)malloc(2 * (str.size()+length +1));
	memset(pbyte,0x00,length);
	strcpy((char*)pbyte,str.c_str());

	ret = BytetoHexStr(pbyte, length);
	free(pbyte);
	return ret;

}



bool CCtrlBASE::init(CSerialBASE * pserialBase)
{
	//NEO_MARK(CCtrlBASE::init);
	char mode[] = { '8', 'N', '1', 0 };
//      str[2][512];


	m_cryptostrparam.txtSEL_PUF = "0100014a";// "3031303030313461";
								 //010000014A
	m_cryptostrparam.txtCONFIG = "0101";// "30313031";
	m_cryptostrparam.txtID = getStr2HexStr("123123",16);;// "313233313233";
	m_cryptostrparam.txtSN = "78787878787878787878787878787878";
	
	g_lenInTestTemp = 0;


	m_delayTime = 500;
	//if (this->pserialBase) delete [] this->pserialBase;
	this->pserialBase = pserialBase;
	
	if (pserialBase->isopen()) return false;

	return pserialBase->open();

	//if (portindex < 0) return 0;
 //return open( portindex, bdrate);
	
	 

}



void CCtrlBASE::SetPARAM(string id, string qchip, string txtSEL_PUFECC)
{



}

#if 0
void CCtrlBASE::SetID(BYTE *pID, int idSize)
{
	cecc.SetID(pID, idSize);

}
void CCtrlBASE::SetID(const char  *strID)
{
	cecc.SetID(strID);

}


bool CCtrlBASE::open(int portindex,int bdrate){
	
	char mode[]={'8','N','1',0};
	m_portindex = portindex	;
	PrintView("comport:%s ,bdrate:%d is opening \n",RS232_getportname(m_portindex),bdrate);
	if(RS232_OpenComport(m_portindex, bdrate, mode))
	{
		PrintView("Can not open comport\n");

		return false;
	}
	return true;
 }

void CCtrlBASE::close()
{
	RS232_CloseComport(m_portindex);

 }
bool CCtrlBASE::isopen()
{
	if (m_portindex < 0) return false;
	return RS232_isOpen(m_portindex);

}

const char *  CCtrlBASE::getportnmae()
{
	static char szRet[1024];
	szRet[0] = 0;
	if (m_portindex < 0) return szRet;

	strcpy(szRet, RS232_getportname(m_portindex));

	return szRet;

}
#endif // 0

int CCtrlBASE::Write(unsigned char * pbuff, int length){
	return pserialBase->Write(pbuff,length);// RS232_SendBuf(m_portindex, pbuff, length);

}
int CCtrlBASE::Read(unsigned char * pbuff, int length){
	return pserialBase->Read(pbuff, length);//return RS232_PollComport(m_portindex, pbuff, length);
}
void CCtrlBASE::Convert2TBCmd(BYTE * ptbcmd,BYTE cmd)
{
	ptbcmd[0] = (BYTE)(cmd>> 4) | 0x50;
	ptbcmd[1] = (BYTE)((cmd << 4) | 0x0A);

}

BYTE CCtrlBASE::Convert2Cmd(BYTE * ptbcmd)
{
	return ((ptbcmd[0] & 0x000F)	<<4) | ((ptbcmd[1] & 0x00F0)>>4);

}

bool CCtrlBASE::isTBCmd(BYTE * ptbcmd){
	BYTE cmd = (ptbcmd[0] & 0xF0)	 | (ptbcmd[1] & 0x0F);
	return (cmd == 0x5a);

}


void CCtrlBASE::SerialWrite(BYTE *lpbuffer,int length)
{
    if (length < 1)
        return;
	BYTE cmd = lpbuffer[0];
/*
	if(cmd == 0x80 ){

		RS232_SendByte(m_portindex,cmd);
		return;
	}
*/
	if(cmd == 0x80  || cmd == CMD_FLASH_REQ_ERASE){
		Write(lpbuffer, length);
		return;
	}

	//BYTE cmdbuff[2] = {(BYTE)(cmd>> 4) | 0x50,(BYTE)((cmd << 4) | 0x0A)};
	BYTE cmdbuff[2] = {0,};

	Convert2TBCmd(cmdbuff,cmd);




	Write( cmdbuff, 2);
	Write(lpbuffer + 1, length - 1);
	
	//PrintViewLine("REAL SEND:%s%s",PrintfBuff(cmdbuff,2).c_str(),PrintfBuff(lpbuffer+1,length -1).c_str());





   // sSerialWCmd[0] = (Byte)((sSerialW[0] >> 4) | 0x50);
    //sSerialWCmd[1] = (Byte)((sSerialW[0] << 4) | 0x0A);

	//serialPort1->Write(sSerialWCmd, 0, 2);
	//serialPort1->Write(sSerialW, 1, sSent - 1);
}

bool CCtrlBASE::SerialReceive(BYTE *lpbuffer,int & length,int delaytime)
{
	int n = 0;
	int bufflength =  length;
	int remainsize = length;
	length = 0;
//	m_strlastmsg = "";

	unsigned int curtime = getTickCount();
	BYTE tmpBuff[BUFFSIZE] = {0,};
	BYTE resbuff[BUFFSIZE] = { 0, };
	BYTE * ptmpres = resbuff;

	int  takentime = 0;
	memset(lpbuffer, 0x00, BUFFSIZE);
	int startindex = 0;
	while(remainsize > 0){
		//PrintView("receiving\n");
		
		memset(tmpBuff,0x00,BUFFSIZE);
		//n = RS232_PollComport(m_portindex, tmpBuff, bufflength);
		n = Read(tmpBuff, BUFFSIZE);
		takentime = getTickCount() - curtime;
		if (takentime >= delaytime) {
			m_strlastmsg = NCL::GetStaticStrForm("TIME out!!!! %d \n", takentime);;
			return false;
		}

		if (n == 0) continue;
		remainsize -= n;
		memcpy(ptmpres + length, tmpBuff, n);

		length += n;
		//m_strlastmsg = NCL::GetStaticStrForm("SUCCESS!! legnth:%d (%x %x)->%x\n",length,tmpBuff[0],tmpBuff[1],lpbuffer[0]);
		//return true;
	
		

	}

	if (isTBCmd(ptmpres)){
		lpbuffer[0] = Convert2Cmd(ptmpres);
		length--;

		memcpy(lpbuffer + 1, ptmpres + 2, length);
	}
	else{
		memcpy(lpbuffer, ptmpres, length);

	}

	
	return true;


	
	
}



int  CCtrlBASE::runCMDINFO(int cmd, int sleep,void* param, BYTE *pSendBuff, int sendbuffsize, BYTE *pRecvBuff, int *precvbuffsize)
{
	CMDINFO cmdinfo = m_maptable[cmd];
	BYTE buffer[BUFFSIZE];
	BYTE tmpbuff;

	


	memset(buffer,0x00,BUFFSIZE);

	int length = 0;
	std::string log ; 


	memset(buffer,0x00,BUFFSIZE);
	m_strlastmsg = "";
	PrintView("*%s\n",cmdinfo.cmdname);
	

	if (pSendBuff) {
		length = sendbuffsize;
		memcpy(buffer, pSendBuff,length);
	}
	else cmdinfo.pfgenbuffer(this,cmdinfo.cmd, buffer,length,m_strlastmsg,param);
	
	m_strlastmsg += this->pbaseCrypt->flushLog();
	if(g_lenInTestTemp>0) {
		memset(buffer,0x00,BUFFSIZE);
		memcpy(buffer,g_testTemp,g_lenInTestTemp);
		g_lenInTestTemp = 0;

	}

	
	ViewLog();
	ViewLogP("snd:",PrintfBuff(buffer,length));
	
	int dummyread = Read(&tmpbuff,1);

	SerialWrite(buffer,length);

	if(cmdinfo.retSize == 0) return 0;


	length = cmdinfo.retSize;
	memset(buffer,0x00,BUFFSIZE);
	//PrintView("receiving.. %d\n",length);
	Sleep(sleep);

	bool ret = SerialReceive(buffer,length,cmdinfo.recvDelayTime);
	
	

	
	if(!ret){
		
		ViewLog();
		return -1;
	}

	ViewLogP("rcv:",PrintfBuff(buffer,length));
	//ViewLog();

	if (pRecvBuff) {
		memcpy(pRecvBuff, buffer, length);
		*precvbuffsize = length;
		
	}
	else cmdinfo.pfpostprocess(this,cmdinfo.cmd, buffer,length,m_strlastmsg,param);
	m_strlastmsg += this->pbaseCrypt->flushLog();
	ViewLog();

	
	if(isTBCmd(buffer)){
		return Convert2Cmd(buffer);

	}

	return buffer[0];

	return -1;
}
string CCtrlBASE::getCMDINFOTitle(int cmd){
	CMDINFO cmdinfo = m_maptable[cmd];

	return cmdinfo.cmdname;
}

std::string CCtrlBASE::PrintfBuff(BYTE * pbuff,int  size)
{
	string ret = "";

//	BYTE tmpBuffer[BUFFSIZE];
	BYTE *lpbuffer = pbuff;

		
	for(int i =0 ; i < size ; i++){
		

		ret += NCL::GetStaticStrForm("%.2X",*lpbuffer++);
		//ret += " ";
	}
	ret += NCL::GetStaticStrForm("(%d)", size);

	return ret;


}

int CCtrlBASE::sleepChip()
{

	BYTE g_result = runCMDINFO(CMD_REQ_SLEEP);

//	m_strlastmsg = "";
//	m_strlastmsg += NCL::GetStaticStrForm("  REQ_SLEEP 1D\n  Response : %02X\n", g_result);

	Sleep(200);

	if (g_result != CMD_RES_SLEEP)
    {
        m_strlastmsg += NCL::GetStaticStrForm( "RES_SLEEP ERROR\n", g_result);
        return 0;
    }

	
	runSync80();

	return 1;
}

int CCtrlBASE::wakeupChip()
{
	BYTE g_result = runCMDINFO(CMD_REQ_WAKEUP);

//	m_strlastmsg = "";
//	m_strlastmsg += NCL::GetStaticStrForm("  REQ_WAKEUP FB\n  Response : %02X\n", g_result);
	Sleep(200);

	
    if (g_result != CMD_RES_WAKEUP)
    {
        m_strlastmsg += NCL::GetStaticStrForm( "RES_WAKEUP ERROR\n", g_result);
        return 0;
    }

	

	runSync80();


	return 1;
}

int CCtrlBASE::runSync80(){
	int ret = 0;
	for(int i = 0 ; i <2 ; i++){
		int ret = runCMDINFO(0x80);
		Sleep(100);
		if (ret == 0x80 || ret == 0x00){
			return ret;
		}
	}
	return ret;
}
void CCtrlBASE::HWReset()
{

	int ret = 0;
	BYTE buffer[1] = { 0x80 };
	SerialWrite(buffer, 1);
	PrintView("HW resett\n");
	
}

void CCtrlBASE::ViewLog()
{
	if(m_strlastmsg.empty()) return;
	PrintView("%s\n",m_strlastmsg.c_str());
	//PrintView();
	m_strlastmsg = "";
}
void CCtrlBASE::ViewLogP(string  title,string  log)
{
	PrintView("%s%s\n",title.c_str(),log.c_str());
}
void CCtrlBASE::WriteLogLine(const char * fmt, ...)
{
	va_list args;
	


	va_start(args, fmt);
	p_circular_tstr str = NCL::GetNewStrFormV(fmt,args);
	va_end(args);

	WriteLog("%s\n",str.c_str());
	

	//return str;
}

void CCtrlBASE::WriteLog(const char * fmt, ...)
{
	va_list args;
	


	va_start(args, fmt);
	p_circular_tstr str = NCL::GetNewStrFormV(fmt,args);
	va_end(args);

	m_strlastmsg += str;

	
	//return str;
}

int CCtrlBASE::eraseChip()
{
//    int ret;
	
	BYTE g_result = runCMDINFO(CMD_REQ_FLASH_MODE);

//	m_strlastmsg = "";
//	m_strlastmsg += NCL::GetStaticStrForm("  REQ_FLASH_MODE F3\n  Response : %02X\n", g_result);
	Sleep(100);

	
    if (g_result != CMD_RES_FLASH_MODE)
    {
        m_strlastmsg += NCL::GetStaticStrForm( "RES_FLASH_MODE\n", g_result);


        return 0;
    }
	

	Sleep(100);

	runSync80();


	runCMDINFO(CMD_FLASH_REQ_ERASE);

	m_strlastmsg += NCL::GetStaticStrForm("  FLASH_REQ_ERASE E5 10(ALL)!\n");
	Sleep(300);
    //sLenRead = 0;
	//cecc.ECC_Clear();
//    memset(EHK_device,      0x46, 16);
    //memset(EHK_device + 16, 0x00, 16);
//    memset(EHK_device, 00, 32);

	return 1;
}
int CCtrlBASE::restartChip()
{

    int ret;
    int i;

    for (i = 0; i < 5 ;i++)    {
		Sleep(100);
		ret = sleepChip();
		ViewLog();
		if (ret <= 0)     continue;
		ret = wakeupChip();
		ViewLog();
		if (ret > 0)      return 1;
    }

    return 0;

}



void CCtrlBASE::Test(int param){

	

	CCtrlBASE ctestecc;

	ctestecc.TestMT(0);




	



}

void CCtrlBASE::calculate(int datalength, int blocklength, int & reallength, int & realn)
{
	int n = datalength / blocklength;
	int r = datalength % blocklength;
	realn = n + ((r > 0) ? 1 : 0);
	reallength = realn*blocklength;
}

int CCtrlBASE::runBlockCrypt(BLOCKCRYPT_CMDTYPE cmdtype, BYTE * pinput, int inputsize, BYTE ** ppoutbuffer)
{
	ENCBLOCKINFO encblockinfo = mapblockcrypt[cmdtype];

	int reallength = 0;
	int realn = 0;
	calculate(inputsize, encblockinfo.blocklength, reallength, realn);


	int maxoutdatasize = reallength + encblockinfo.blocklength;
	if (!*ppoutbuffer) *ppoutbuffer = (BYTE *)malloc(maxoutdatasize);
	memset(*ppoutbuffer,0x00,maxoutdatasize	);




	encblockinfo.pInBuff = pinput;
	encblockinfo.datalength = inputsize;
	encblockinfo.remainsize = inputsize;
	encblockinfo.pOutBuff = *ppoutbuffer;
	int realmain = encblockinfo.remainsize;


	int count = 0;

	int totaloutsize = 0;
	int cmd = encblockinfo.cmd;
	do{
		if (count > 0) cmd = encblockinfo.nextCmd;

		runCMDINFO(cmd,0, &encblockinfo);
		totaloutsize += encblockinfo.outlength;
		encblockinfo.pOutBuff += encblockinfo.outlength;



		encblockinfo.pInBuff += encblockinfo.blocklength;
		encblockinfo.remainsize -= encblockinfo.blocklength;
		if (encblockinfo.isVerify && encblockinfo.remainsize == encblockinfo.blocklength) break;

		count++;

	} while (encblockinfo.remainsize > 0);

	if (encblockinfo.lastCmd != -1 && encblockinfo.remainsize >= 0) {//neotest
		runCMDINFO(encblockinfo.lastCmd,0, &encblockinfo);
		totaloutsize += encblockinfo.outlength;

	}




	return totaloutsize;

}
void CCtrlBASE::TestMT(int param)
{
#if 1
	if (param == 0){
#if 0
		close();
		int bdrates[] = { 115200, 57600, 38400, 0 };


		for (int *pbd = bdrates; *pbd > 0; pbd++){
			for (int i = 0; i < 6; i++){
				PrintViewLine("testing %s bdrate:%d ", RS232_getportname(i), *pbd);
				if (!open(i, *pbd)) continue;

				int ret = this->runSync80();

				close();
				if (ret == 0x80){
					return;
				}
				Sleep(500);
				//PrintViewLine();
			}
		}

#endif


	}
	else if (param == 1){


		char sztmp[] = "abcdefghijklmnopqrstuvzefghijklmfdasfs";;
		//char sztmp[] = "abcdefghijklmnopqrstuvzefghijklmsdfhsdfjklsajfklsjfsmflsjfklsajfklsadfjksal";;
		BYTE outbyte[BUFFSIZE] = { 0, };
		BYTE tmputbyte[BUFFSIZE] = { 0, };
		BYTE orgoutbyte[BUFFSIZE] = { 0, };
		BYTE *poutbuffer = outbyte;
		BYTE *porgoutbyte = orgoutbyte;

		unsigned long dwLenCrypt = 0;
		unsigned long dwLenEnCrypt = 0;
		
		this->pbaseCrypt->encryptCCM((BYTE*)sztmp, (DWORD)strlen(sztmp), tmputbyte, &dwLenCrypt);
		PrintViewLine("%s", PrintfBuff(tmputbyte, dwLenCrypt).c_str());
		pbaseCrypt->decryptCCM(tmputbyte, dwLenCrypt, orgoutbyte, &dwLenEnCrypt);
		PrintViewLine("%s", PrintfBuff(orgoutbyte, dwLenCrypt - 16).c_str());


		PrintViewLine("Encrypt");



		int size = runBlockCrypt(BLOCKCRYPT_ENC_CCM, (BYTE*)sztmp, (int)strlen(sztmp), &poutbuffer);
		PrintViewLine("%s\n%d", PrintfBuff(poutbuffer, size).c_str(), size);




		PrintViewLine("Decrypt");

		//memcpy(cecc.G_MSG, tmputbyte, dwLenCrypt);
		//dwLenCrypt -= 16;

		size = runBlockCrypt(BLOCKCRYPT_DEC_CCM, (BYTE*)poutbuffer, size, &porgoutbyte);
		PrintViewLine("%s", PrintfBuff(porgoutbyte, size).c_str());

		PrintViewLine("%s", porgoutbyte);



#if 0

		int cmd = CMD_REQ_CTR;
		//int nextcmd = CMD_REQ_CRYPTO_CONT;


		ENCBLOCKINFO encblockinfo = mapblockcrypt[BLOCKCRYPT_ENC_CCM];

		encblockinfo.pInBuff = (BYTE *)malloc(encblockinfo.blocklength);
		encblockinfo.blocklength = 16;
		encblockinfo.datalength = strlen(sztmp);
		encblockinfo.pOutBuff = outbyte;
		BYTE * pstartbyte = encblockinfo.pInBuff;
		int count = 0;	
		int inputedsize = 0;
		bool isLast = false;
		BYTE * pbuff = (BYTE *)sztmp;


		do{
			if (count > 0) cmd = encblockinfo.nextCmd;
			runCMDINFO(cmd,&encblockinfo);
			encblockinfo.pInBuff += encblockinfo.blocklength;
			inputedsize = encblockinfo.pInBuff - pstartbyte ;
			count++;
			if (inputedsize >= encblockinfo.datalength)  isLast = true;
		}while(inputedsize < encblockinfo.datalength);

		if (encblockinfo.lastCmd != -1) runCMDINFO(encblockinfo.lastCmd, &encblockinfo);
#endif




	}
	else if (param == 2){

	}


#endif // 0


}


unsigned int CCtrlBASE::getTickCount()
{
#ifdef _WIN32
    return GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (unsigned int)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}
void CCtrlBASE::Sleep(int msec){
#ifdef _WIN32
	::Sleep(msec);
#else
    usleep(msec*1000);  /* sleep for 100 milliSeconds */
#endif

}

BYTE * CCtrlBASE::getLogBuffer(){
	memset(g_LogBuf,0x00,102400);
	return g_LogBuf;
}
int CCtrlBASE::Hex2ArrayBin(string strD, BYTE * pbyte){

	return hex2bin(strD.c_str(),pbyte);
	//hex2bin("31323331323300000000000000000000", m_eccinterparam.ID);
	//hex2bin("FFB4D4DE13FB94D336565F26C9D00DDB262805E04DE35A96BE95C6383BAC45EB", m_eccinterparam.SDATA);


}

#if 0
void CCtrlBASE::ChipEnc_ctr(int iSelected)
{
	int i;
	int iLen;
	int iLen_temp;
//	int iLenHMAC;
	int iPos;
	int iLenBlock;
	int iSelected;
	int iPosRes = 0;
    BYTE bNextCmd;
	int iResCmd;
	int iLenRes;
	string strRd;
	BYTE abResult[16000];


	string strInput;


	g_msgPos = 0;
	strInput = txtChipEncECC->Text;
	richLog = richECC;
	iSelected = cmbChipEncECC->SelectedIndex;


	try
	{
		checkPort();
		g_LogBuf[0] = 0;

		iLen = strInput->Length;
		iLen_temp = iLen;

		for (i = 0; i < iLen; i++)
			g_InBuf[i] = (Byte)strInput[i];

		if (iSelected == MODE_CRYPTO_HMAC)
		{
			iLen = data2paddingdata_HMAC(g_InBuf, iLen);
		}

        bNextCmd = CMD_REQ_CRYPTO_CONT;
        iResCmd  = CMD_RES_CRYPTO;
        iLenRes  = 16;

			sSerialW[0] = CMD_REQ_CTR;
			iLenBlock = 16;


		for (iPos = 0 ; (iPos + iLenBlock) <= iLen; iPos += iLenBlock)
		{
			if (iPos == 0)
			{ //first
				sSerialW[1] = (BYTE)(iLen >> 8);
				sSerialW[2] = (BYTE)(iLen);
				for (i = 0; i < iLenBlock; i++)
					sSerialW[i + 3] = (BYTE)(g_InBuf[iPos + i]);

				sSent = iLenBlock + 3;
			}
			else
			{ 
                sSerialW[0] = bNextCmd ;
				for (i = 0; i < iLenBlock; i++)
					sSerialW[i + 1] = (BYTE)(g_InBuf[iPos + i]);

				sSent = iLenBlock + 1;
			}

			sLenRead = 0;

          SerialWrite();

			strRd = "chip sent : ";
			for (i = 0; i < sSent; i++)
				strRd += sSerialW[i].ToString("X2");

			richLog->AppendText(strRd + "\n");

			g_result = receiveChip(iLenRes + 1, 0, COMM_REQUEST_TIME);
			printSerialIn(richRSA);

			if (g_result != iResCmd)
				throw %Exception("Receive Command Error!");

			for (i = 0; i < iLenRes; i++)
				abResult[iPosRes + i] = sSerialR[i + 1];

			iPosRes += i;
        }


			encryptCTR(abResult, iPosRes, g_OutBuf, &g_lenOut);
			
			memcpy(G_MSG, abResult, iPosRes);   g_msgPos = iPosRes;	
			
			g_OutBuf[g_lenOut] = 0;
			strRd = %String((const char*)g_OutBuf);

			richLog->AppendText(String::Format("result : {0}\n", strRd));

			if (strRd != strInput)
				throw %Exception("Receive Data Error!");


	
		richLog->AppendText(%String((const char*)g_LogBuf) + "\n");
		g_LogBuf[0] = 0;

		return;
	}
	catch (Exception^ ee)
	{
		if (g_LogBuf[0] != 0)
			richLog->AppendText(%String((const char*)g_LogBuf) + "\n" + ee->Message + "\n");

		MessageBox::Show(this, ee->Message, "ERROR!", MessageBoxButtons::OK );
	}
}
void CCtrlBASE::ChipEnc_CCM(int iSelected)
{
	int i;
	int iLen;
	int iLen_temp;
//	int iLenHMAC;
	int iPos;
	int iLenBlock;
	int iSelected;
	int iPosRes = 0;
    BYTE bNextCmd;
	int iResCmd;
	int iLenRes;
	string strRd;
	BYTE abResult[16000];


	string strInput;


	g_msgPos = 0;
	strInput = txtChipEncECC->Text;
	richLog = richECC;
	iSelected = cmbChipEncECC->SelectedIndex;


	try
	{
		checkPort();
		g_LogBuf[0] = 0;

		iLen = strInput->Length;
		iLen_temp = iLen;

		for (i = 0; i < iLen; i++)
			g_InBuf[i] = (Byte)strInput[i];

		if (iSelected == MODE_CRYPTO_HMAC)
		{
			iLen = data2paddingdata_HMAC(g_InBuf, iLen);
		}

        bNextCmd = CMD_REQ_CRYPTO_CONT;
        iResCmd  = CMD_RES_CRYPTO;
        iLenRes  = 16;

	

	
			sSerialW[0] = CMD_REQ_ENC_CCM;
			iLenBlock = 16;
	

	
		for (iPos = 0 ; (iPos + iLenBlock) <= iLen; iPos += iLenBlock)
		{
			if (iPos == 0)
			{ //first
				sSerialW[1] = (BYTE)(iLen >> 8);
				sSerialW[2] = (BYTE)(iLen);
				for (i = 0; i < iLenBlock; i++)
					sSerialW[i + 3] = (BYTE)(g_InBuf[iPos + i]);

				sSent = iLenBlock + 3;
			}
			else
			{ 
                sSerialW[0] = bNextCmd ;
				for (i = 0; i < iLenBlock; i++)
					sSerialW[i + 1] = (BYTE)(g_InBuf[iPos + i]);

				sSent = iLenBlock + 1;
			}

			sLenRead = 0;

          SerialWrite();

			strRd = "chip sent : ";
			for (i = 0; i < sSent; i++)
				strRd += sSerialW[i].ToString("X2");

			richLog->AppendText(strRd + "\n");

			g_result = receiveChip(iLenRes + 1, 0, COMM_REQUEST_TIME);
			printSerialIn(richRSA);

			if (g_result != iResCmd)
				throw %Exception("Receive Command Error!");

			for (i = 0; i < iLenRes; i++)
				abResult[iPosRes + i] = sSerialR[i + 1];

			iPosRes += i;
        }


		
			sLenRead = 0;
            sSerialW[0] = CMD_REQ_CCM_GENERATE_MAC;

            sSent = 1;
            SerialWrite();

			richLog->AppendText("chip sent : REQ_CCM_GENERATE_MAC(14)\n");

			g_result = receiveChip(16 + 1, 0, COMM_REQUEST_TIME);
            richLog->AppendText("chip response : ");
			printSerialIn(richRSA);

			if (g_result != CMD_RES_CCM_GENERATE_MAC)
				throw %Exception("Receive Command Error!");

			for (i = 0; i < 16; i++)
				abResult[iPosRes + i] = sSerialR[i + 1];

            iPosRes += 16;

			decryptCCM(abResult, iPosRes, g_OutBuf, &g_lenOut);

			memcpy(G_MSG, abResult, iPosRes);   g_msgPos = iPosRes;	

			g_OutBuf[g_lenOut] = 0;
			strRd = %String((const char*)g_OutBuf);

			richLog->AppendText(String::Format("result : {0}\n", strRd));

			if (strRd != strInput)
				throw %Exception("Receive Data Error!");

		

	

		richLog->AppendText(%String((const char*)g_LogBuf) + "\n");
		g_LogBuf[0] = 0;

		return;
	}
	catch (Exception^ ee)
	{
		if (g_LogBuf[0] != 0)
			richLog->AppendText(%String((const char*)g_LogBuf) + "\n" + ee->Message + "\n");

		MessageBox::Show(this, ee->Message, "ERROR!", MessageBoxButtons::OK );
	}
}
void CCtrlBASE::ChipEnc_hmap(int iSelected)
{
	int i;
	int iLen;
	int iLen_temp;
//	int iLenHMAC;
	int iPos;
	int iLenBlock;
	int iSelected;
	int iPosRes = 0;
    BYTE bNextCmd;
	int iResCmd;
	int iLenRes;
	string strRd;
	BYTE abResult[16000];


	string strInput;


	g_msgPos = 0;
	strInput = txtChipEncECC->Text;
	richLog = richECC;
	iSelected = cmbChipEncECC->SelectedIndex;


	try
	{
		checkPort();
		g_LogBuf[0] = 0;

		iLen = strInput->Length;
		iLen_temp = iLen;

		for (i = 0; i < iLen; i++)
			g_InBuf[i] = (Byte)strInput[i];

		if (iSelected == MODE_CRYPTO_HMAC)
		{
			iLen = data2paddingdata_HMAC(g_InBuf, iLen);
		}

        bNextCmd = CMD_REQ_CRYPTO_CONT;
        iResCmd  = CMD_RES_CRYPTO;
        iLenRes  = 16;

	
            bNextCmd = CMD_REQ_HMAC_CONT;
            iResCmd  = CMD_RES_HMAC;
            iLenRes  = 0;
			sSerialW[0] = CMD_REQ_HMAC;
			iLenBlock = 64;
			//iLen = iLenHMAC;
			break;

	
		for (iPos = 0 ; (iPos + iLenBlock) <= iLen; iPos += iLenBlock)
		{
			if (iPos == 0)
			{ //first
				sSerialW[1] = (BYTE)(iLen >> 8);
				sSerialW[2] = (BYTE)(iLen);
				for (i = 0; i < iLenBlock; i++)
					sSerialW[i + 3] = (BYTE)(g_InBuf[iPos + i]);

				sSent = iLenBlock + 3;
			}
			else
			{ 
                sSerialW[0] = bNextCmd ;
				for (i = 0; i < iLenBlock; i++)
					sSerialW[i + 1] = (BYTE)(g_InBuf[iPos + i]);

				sSent = iLenBlock + 1;
			}

			sLenRead = 0;

          SerialWrite();

			strRd = "chip sent : ";
			for (i = 0; i < sSent; i++)
				strRd += sSerialW[i].ToString("X2");

			richLog->AppendText(strRd + "\n");

			g_result = receiveChip(iLenRes + 1, 0, COMM_REQUEST_TIME);
			printSerialIn(richRSA);

			if (g_result != iResCmd)
				throw %Exception("Receive Command Error!");

			for (i = 0; i < iLenRes; i++)
				abResult[iPosRes + i] = sSerialR[i + 1];

			iPosRes += i;
        }


			//case MODE_CRYPTO_HMAC:
			sLenRead = 0;
            sSerialW[0] = CMD_REQ_GENERATE_HMAC;

            sSent = 1;
            SerialWrite();

			richLog->AppendText("chip sent : REQ_GENERATE_HMAC(18)\n");

			g_result = receiveChip(32 + 1, 0, COMM_REQUEST_TIME);
            richLog->AppendText("chip response : ");
			printSerialIn(richRSA);

			if (g_result != CMD_RES_GENERATE_HMAC)
				throw %Exception("Receive Command Error!");

            generateHMAC(g_InBuf, strInput->Length, g_OutBuf, &g_lenOut);

			for (i = 0; i < 32; i++)
				abResult[iPosRes + i] = sSerialR[i + 1];

			if (memcmp(g_OutBuf, abResult, 32) != 0) 
				throw %Exception("HMAC Data Error!");

		richLog->AppendText(%String((const char*)g_LogBuf) + "\n");
		g_LogBuf[0] = 0;

		return;
	}
	catch (Exception^ ee)
	{
		if (g_LogBuf[0] != 0)
			richLog->AppendText(%String((const char*)g_LogBuf) + "\n" + ee->Message + "\n");

		MessageBox::Show(this, ee->Message, "ERROR!", MessageBoxButtons::OK );
	}
}
#endif
