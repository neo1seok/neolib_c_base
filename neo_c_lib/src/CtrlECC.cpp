
#include <iostream>
#include <map>
#include "rs232.h"
#include "Util.h"
#include "CEcc.h"
#include "CtrlECC.h"
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









CCtrlECC::CCtrlECC(void)
{
	CMDINFO cmdTable[] = {

#ifndef __TESTMODE__
		CMDTABLE_EXT(CMD_REQ_PUK_ECC, 1 + 33 + 2 + 1, COMM_REQUEST_TIME),
		CMDTABLE(CMD_REQ_REG_CERT_ECC, 2 , 2*COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_AUTH_ECC, 1 + 16 + 33 + 32 + 32 + 1, COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_HS1_ECC, 1 + 16 + 32 + 1, COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_SIGN,1 + 16 + 32 + 32+1,COMM_REQUEST_TIME),
#else
		CMDJUSTHARDCODING(CMD_REQ_SEL_PUF, 2, COMM_REQUEST_TIME),
		CMDJUSTHARDCODING(CMD_REQ_CONFIG, 2, COMM_REQUEST_TIME),
		CMDJUSTHARDCODING(CMD_REQ_SN, 18, COMM_REQUEST_TIME),
		CMDJUSTHARDCODING(CMD_REQ_PUK_ECC, 1 + 33 + 2 + 1, COMM_REQUEST_TIME),
		CMDJUSTHARDCODING(CMD_REQ_REG_CERT_ECC, 2 + 1, COMM_REQUEST_TIME),
		CMDJUSTHARDCODING(CMD_REQ_AUTH_ECC, 1 + 16 + 33 + 32 + 32 + 1, COMM_REQUEST_TIME),
		CMDJUSTHARDCODING(CMD_REQ_HS1_ECC, 1 + 16 + 32 + 1, COMM_REQUEST_TIME),


#endif
		CMDWITHSUBCMD(CMD_ERR_HS2_ECC, 0x00, 0, COMM_REQUEST_TIME),
		{ -1, 0, 0, NULL, NULL }
	};
	for (CMDINFO *pcmd = cmdTable; pcmd->cmd != -1; pcmd++){
		m_maptable[pcmd->cmd] = *pcmd;
	}

	_tesstvalue[CMD_REQ_SEL_PUF]="010100014A";
	_tesstvalue[CMD_REQ_CONFIG]="020101";
	_tesstvalue[CMD_REQ_SN]="03";
	_tesstvalue[CMD_REQ_PUK_ECC]="3531323331323300000000000000000000FFB4D4DE13FB94D336565F26C9D00DDB262805E04DE35A96BE95C6383BAC45EB02585FDBA42D56674FA1FFE4A32677D731C122CFB5FE07A37B9DBEEF0B4C8B98A53EDA";
	_tesstvalue[CMD_REQ_REG_CERT_ECC]="3731323331323300000000000000000000030EF9ACCF5C34B2727204A491CF5504A25B9E95D289E37AACFFB49A64C8F6C104CC7E800C21233D31D9EB3D813A6624DD089E9B48A000D613DB82A77694A3B538882923793F5AAADC359F5505690A9D53B625C4CDD86D9C35098C2AF588CDB1DC95BD";
	_tesstvalue[CMD_REQ_AUTH_ECC]=	"39FFB4D4DE13FB94D336565F26C9D00DDB262805E04DE35A96BE95C6383BAC45EB4646464646464646464646464646464600000000000000000000000000000000";
	//								"39FFB4D4DE13FB94D336565F26C9D00DDB262805E04DE35A96BE95C6383BAC45EB4646464646464646464646464646464600000000000000000000000000000000"

	_tesstvalue[CMD_REQ_HS1_ECC]="3B03341411C04DC71F7362217206686F8DB6C23D5B21EE8DC31FA9EBA9EDD0353A0B5FBFCA85E8BAA154BA777C84D768782742CABBE2335D2AFC4AE4944C0A9A42C1CA1AF4D923C7FAF94240CFBBD66F06193D7656816AEC63AA58D80A1EBFCCF86167F06295A187DD8E1714C7354A8D3DC1";
//	int size;
	string strlog = "";
	//m_maptable[CMD_REQ_SIGN].pfpostprocess(this, 0, NULL, size, strlog, NULL);
	memset(g_LogBuf,0,102400);

	pbaseCrypt = &cecc;
}



CCtrlECC::~CCtrlECC(void)
{
}






bool CCtrlECC::init(CSerialBASE * pserialBase)
{
	NEO_MARK(CCtrlECC::init);
	bool ret = CCtrlBASE::init(pserialBase);
 

	m_delayTime = 500;
	
	
	cecc.keyinit(0);
	cecc.SetID(m_cryptostrparam.txtID.c_str());

	
	return ret;

}



void CCtrlECC::SetPARAM(string id, string qchip, string txtSEL_PUFECC)
{
	if (!id.empty()) {
		this->m_cryptostrparam.txtID = id;
		cecc.SetID(m_cryptostrparam.txtID.c_str());
	}
	if (!qchip.empty()) this->m_cryptostrparam.txtQChip = qchip;
	if (!txtSEL_PUFECC.empty()) this->m_cryptostrparam.txtSEL_PUF = txtSEL_PUFECC;


}

#if 0
void CCtrlECC::SetID(BYTE *pID, int idSize)
{
	cecc.SetID(pID, idSize);

}
void CCtrlECC::SetID(const char  *strID)
{
	cecc.SetID(strID);

}
#endif // 0


#if 0
void CCtrlECC::Test(int param){

	

	CCtrlECC ctestecc;

	ctestecc.TestMT(0);



	ctestecc.init(param,115200);

	for(int i = 0 ; i < 20 ; i++){
		ctestecc.sleepChip();
		ctestecc.ViewLog();
		Sleep(200);
		int ret = ctestecc.eraseChip();
		ctestecc.ViewLog();
		if(ret != 0) break;
		Sleep(500);

	}

	//return;

	ctestecc.runSync80();
	
	
	ctestecc.runCMDINFO(CMD_REQ_TEST);

	ctestecc.runCMDINFO(CMD_ERR_TEST);

	



}
#endif
void CCtrlECC::PreIssue(string id, string pubchip)
{

	INTER_PARAM * pinterparam = cecc.GetInterParam();

	StringtoByteArray(id.c_str(), pinterparam->certECC.ID, id.length());
	StringtoByteArray(pubchip.c_str(), pinterparam->certECC.Qchip, pubchip.length());


	cecc.UpdateChip();
	cecc.UpdateTSM();

	cecc.UpdateSDATA();
	cecc.UpdateCERT();

	this->ViewLog();
}

void CCtrlECC::PreIssue(BYTE *pID,int idsize,BYTE*pQChip,int qchipSize)
{
		INTER_PARAM * pinterparam = cecc.GetInterParam();
		memcpy(pinterparam->certECC.ID, pID, idsize);
		memcpy(pinterparam->certECC.Qchip, pQChip, qchipSize);


		cecc.UpdateSDATA();
		cecc.UpdateCERT();
			
		
		//cecc.ISSUE(pID, idsize, pQChip, qchipSize);
		


		m_strlastmsg = "";
		this->ViewLog();

		return;
		BYTE inbuffer[BUFFSIZE];
		BYTE outbuffer[BUFFSIZE];
		unsigned long outsize = 0;
		int size = 0;

		memset(inbuffer, 0, BUFFSIZE);
		memset(outbuffer,0,BUFFSIZE);


		//INTER_PARAM * pinterparam = cecc.GetInterParam();

	//	memcpy(pinterparam->certECC.Qchip, pQChip, qchipSize);
		
		//size = hex2bin("8378787878787878787878787878787878",buffer);
#if 1
		


		VECBYTE vecID = ::HexStr2Byte(m_cryptostrparam.txtID);
		VECBYTE vecbyte = ::HexStr2Byte(m_cryptostrparam.txtQChip);

		pID = &vecID[0];
		idsize = (int)vecID.size();
		//pQChip = &vecbyte[0];
		//qchipSize = vecbyte.size();
#endif // 0


		
		cecc.SetID(pID,idsize);
		
		//cecc.verifySign(pID, idsize, pQChip, qchipSize);


		
		cecc.flushLog();


		m_strlastmsg = cecc.viewCETEXT();
		this->ViewLog();

		
		cecc.UpdateChip();

		cecc.requestIdECC(NULL,size,outbuffer,&outsize);
		this->m_strlastmsg += cecc.getLog();
		this->ViewLog();
		
		memset(inbuffer, 0, BUFFSIZE);
		memset(outbuffer,0,BUFFSIZE);
		
		
		if (pQChip){
			memset(inbuffer, 0xB5, 1);
			memcpy(inbuffer + 1, pQChip, qchipSize);
			DWORD CRC = calcCRC(pQChip, qchipSize);


			memset(inbuffer + 1 + qchipSize, CRC>>8, 1);
			memset(inbuffer + 1 + qchipSize+1, CRC , 1);
			
			size = 1 + qchipSize +2;
		}
		else{
			size = hex2bin("B5030EF9ACCF5C34B2727204A491CF5504A25B9E95D289E37AACFFB49A64C8F6C10455A3", inbuffer);
		}

		//size = hex2bin("B5030EF9ACCF5C34B2727204A491CF5504A25B9E95D289E37AACFFB49A64C8F6C10455A3",buffer);
		cecc.requestCertECC(inbuffer, size, outbuffer, &outsize);
		this->m_strlastmsg = cecc.getLog();
		this->ViewLog();

		m_strlastmsg = cecc.viewCETEXT();
		this->ViewLog();

		memset(inbuffer, 0, BUFFSIZE);
		memset(outbuffer,0,BUFFSIZE);
		size = outsize;
		memcpy(inbuffer, outbuffer, size);
		cecc.requestRegCertECC(inbuffer, size, outbuffer, &outsize);
		this->m_strlastmsg += cecc.getLog();
		this->ViewLog();

}

void CCtrlECC::AfterAuth(unsigned char * pbData, int iLength, unsigned char * pbOut, int *piLengthOut){
	BYTE outbuff[BUFFSIZE] = { 0, };
	unsigned long outsize = 0;
	this->m_strlastmsg = "";
	cecc.requestRNECC(outbuff, &outsize);
	this->m_strlastmsg = cecc.getLog();
	this->ViewLog();

	cecc.requestSessionECC(pbData, iLength);
	this->m_strlastmsg = cecc.getLog();
	this->ViewLog();
	cecc.requestHS1ECC(pbOut, (unsigned long*)piLengthOut);
	this->m_strlastmsg = cecc.getLog();
	this->ViewLog();
	


}

void CCtrlECC::TestMT(int param)
{
	if(param == 0){
	
		


	}
	else if (param == 1){


		char sztmp[] = "abcdefghijklmnopqrstuvzefghijklmfdasfs";;
		//char sztmp[] = "abcdefghijklmnopqrstuvzefghijklmsdfhsdfjklsajfklsjfsmflsjfklsajfklsadfjksal";;
		BYTE outbyte[BUFFSIZE] = {0,};
		BYTE tmputbyte[BUFFSIZE] = { 0, };
		BYTE orgoutbyte[BUFFSIZE] = { 0, };
		BYTE *poutbuffer = outbyte;
		BYTE *porgoutbyte = orgoutbyte;

		unsigned long dwLenCrypt = 0;
		unsigned long dwLenEnCrypt = 0;
		cecc.encryptCCM((BYTE*)sztmp, (DWORD)strlen(sztmp), tmputbyte, &dwLenCrypt);
		PrintViewLine("%s", PrintfBuff(tmputbyte, dwLenCrypt).c_str());
		cecc.decryptCCM(tmputbyte, dwLenCrypt, orgoutbyte, &dwLenEnCrypt);
		PrintViewLine("%s", PrintfBuff(orgoutbyte, dwLenCrypt - 16).c_str());


		PrintViewLine("Encrypt");



		int size = runBlockCrypt(BLOCKCRYPT_ENC_CCM, (BYTE*)sztmp, (int)strlen(sztmp), &poutbuffer);
		PrintViewLine("%s\n%d",PrintfBuff(poutbuffer, size).c_str(),size);

	

			
		PrintViewLine("Decrypt");

		//memcpy(cecc.G_MSG, tmputbyte, dwLenCrypt);
		//dwLenCrypt -= 16;

		size = runBlockCrypt(BLOCKCRYPT_DEC_CCM, (BYTE*)poutbuffer, size, &porgoutbyte);
		PrintViewLine("%s", PrintfBuff(porgoutbyte, size).c_str());

		PrintViewLine("%s",porgoutbyte);



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

		if(encblockinfo.lastCmd != -1) runCMDINFO(encblockinfo.lastCmd, &encblockinfo);
#endif



		
	}
	else if (param == 2){
		
	}



}



#if 0
void CCtrlECC::ChipEnc_ctr(int iSelected)
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
void CCtrlECC::ChipEnc_CCM(int iSelected)
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
void CCtrlECC::ChipEnc_hmap(int iSelected)
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