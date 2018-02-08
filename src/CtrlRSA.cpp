
#include <iostream>
#include <map>
#include "rs232.h"
#include "Util.h"
#include "CEcc.h"
#include "CtrlRSA.h"
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











CCtrlRSA::CCtrlRSA(void)
{

	CMDINFO cmdTable[] = {

		CMDTABLE_EXT(CMD_REQ_PUK_RSA, 1 + 256 + 2, COMM_RSA_KEY_GEN_TIME),
		//CMDTABLE_EXT(CMD_REQ_PUK_RSA, 1 + 33 + 2+1, COMM_RSA_KEY_GEN_TIME),
		CMDTABLE(CMD_REQ_REG_CERT_RSA, 1 + 1, COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_AUTH_RSA, 1 + 16 + 256 + 16 + 256 + 1, COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_HS1_RSA, 2 + 1, COMM_REQUEST_TIME),
		CMDTABLE_EXT(CMD_REQ_SIGN, 1 + 16 + 16 + 256 + 1, COMM_REQUEST_TIME),
		
		{ -1, 0, 0, NULL, NULL }
	};
	for (CMDINFO *pcmd = cmdTable; pcmd->cmd != -1; pcmd++){
		m_maptable[pcmd->cmd] = *pcmd;
	}
//	int size;
	string strlog = "";
	//m_maptable[CMD_REQ_SIGN].pfpostprocess(this, 0, NULL, size, strlog, NULL);


	pbaseCrypt = &crsa;

	
}



CCtrlRSA::~CCtrlRSA(void)
{
}




bool CCtrlRSA::init(CSerialBASE * pserialBase)
{
	//NEO_MARK(CCtrlRSA::init);
	bool ret = CCtrlBASE::init(pserialBase);
	
	crsa.keyinit(0);
	crsa.SetID(m_cryptostrparam.txtID.c_str());
 
	return ret;
}


void CCtrlRSA::SetPARAM(string id, string nchip, string txtSEL_PUF)
{
	if (!id.empty()) this->m_cryptostrparam.txtID = id;
	if (!nchip.empty()) this->m_cryptostrparam.txtNChip = nchip;
	if (!txtSEL_PUF.empty()) this->m_cryptostrparam.txtSEL_PUF = txtSEL_PUF;


}

void CCtrlRSA::PreIssue(string id, string pubchip) 
{
	
	INTER_PARAM * pinterparam = crsa.GetInterParam();
	
	StringtoByteArray(id.c_str(), pinterparam->certRSA.ID, id.length());
	StringtoByteArray(pubchip.c_str(), pinterparam->certRSA.Nchip, pubchip.length());


	crsa.UpdateChip();
	crsa.UpdateTSM();
	crsa.UpdateSDATA();
	crsa.UpdateCERT();

	this->ViewLog();
}

void CCtrlRSA::PreIssue(BYTE *pID, int idsize, BYTE*pNChip, int nchipSize)
{

	//cecc.ISSUEfromString(m_cryptostrparam.txtIDECC.c_str(), "030EF9ACCF5C34B2727204A491CF5504A25B9E95D289E37AACFFB49A64C8F6C104");
	INTER_PARAM * pinterparam = crsa.GetInterParam();
	memcpy(pinterparam->certRSA.ID, pID, idsize);
	memcpy(pinterparam->certRSA.Nchip, pNChip, nchipSize);
	crsa.UpdateChip();
	crsa.UpdateTSM();
	crsa.UpdateSDATA();
	crsa.UpdateCERT();
	//crsa.GETCERT( pNChip, nchipSize);
	//cecc.requestEHKdevice(NULL,0,NULL,0,NULL);


//	m_strlastmsg = crsa.viewCETEXT(&crsa.m_certext);
	this->ViewLog();

	return;
}