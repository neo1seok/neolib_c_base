
#include <iostream>
#include <map>
#include "rs232.h"
#include "Util.h"

#include "CtrlBASE.h"
#include "CtrlECC.h"
#include "CtrlRSA.h"

#include "commfunc.h"
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

CMD_REQ_EHK
CMD_REQ_SIGN


CMD_REQ_PUK_ECC
CMD_REQ_REG_CERT_ECC
CMD_REQ_AUTH_ECC
CMD_REQ_HS1_ECC
CMD_ERR_HS2_ECC


CMD_REQ_ID_RSA
CMD_REQ_PUK_RSA
CMD_REQ_CERT_RSA
CMD_REQ_REG_CERT_RSA
CMD_REQ_AUTH_RSA
CMD_REQ_SESSION_RSA
CMD_REQ_HS1_RSA
CMD_RES_SIGN_RSA




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




#if 0
#define FUNCGENDEF(cmd_) void tmp_gen##cmd_(int cmd, BYTE * pbuff, int & size, std::string &strlog, CCtrlBASE*pThis, void* param){	pThis->gen##cmd_(cmd, pbuff, size, strlog, pThis,param);}
#define FUNCPOSTDEF(cmd_) void tmp_postProc##cmd_(int cmd, BYTE * pbuff, int & size, std::string &strlog, CCtrlBASE*pThis, void* param){	pThis->postProc##cmd_(cmd, pbuff, size, strlog, pThis,param);}

#define FUNCGENDEF_ECC(cmd_) void tmp_gen##cmd_(int cmd, BYTE * pbuff, int & size, std::string &strlog, CCtrlECC*pThis, void* param){	pThis->gen##cmd_(cmd, pbuff, size, strlog, pThis,param);}
#define FUNCPOSTDEF_ECC(cmd_) void tmp_postProc##cmd_(int cmd, BYTE * pbuff, int & size, std::string &strlog, CCtrlECC*pThis, void* param){	pThis->postProc##cmd_(cmd, pbuff, size, strlog, pThis,param);}

#define FUNCGENDEF_RSA(cmd_) void tmp_gen##cmd_(int cmd, BYTE * pbuff, int & size, std::string &strlog, CCtrlRSA*pThis, void* param){	pThis->gen##cmd_(cmd, pbuff, size, strlog, pThis,param);}
#define FUNCPOSTDEF_RSA(cmd_) void tmp_postProc##cmd_(int cmd, BYTE * pbuff, int & size, std::string &strlog, CCtrlRSA*pThis, void* param){	pThis->postProc##cmd_(cmd, pbuff, size, strlog, pThis,param);}
#endif

FUNCDEF(gen, CCtrlBASE, DefCMD);
FUNCDEF(gen, CCtrlBASE, ErrCMD);

FUNCDEF(gen, CCtrlBASE, CMD_REQ_TEST);
FUNCDEF(gen, CCtrlBASE, CMD_ERR_TEST);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_SEL_PUF);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_CONFIG);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_SN);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_EHK);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_SIGN);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_CTR);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_ENC_CCM);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_HMAC);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_CRYPTO_CONT);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_HMAC_CONT);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_CCM_GENERATE_MAC);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_GENERATE_HMAC);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_DEC_CCM);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_CCM_VERIFY_MAC);
FUNCDEF(gen, CCtrlBASE, CMD_REQ_VERIFY_HMAC);


FUNCDEF(postProc, CCtrlBASE, DefCMD);
//FUNCDEF(postProc, CCtrlBASE, ErrCMD);

FUNCDEF(postProc, CCtrlBASE, CMD_REQ_TEST);
FUNCDEF(postProc, CCtrlBASE, CMD_ERR_TEST);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_SEL_PUF);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_CONFIG);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_SN);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_EHK);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_SIGN);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_CTR);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_ENC_CCM);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_HMAC);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_CRYPTO_CONT);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_HMAC_CONT);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_CCM_GENERATE_MAC);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_GENERATE_HMAC);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_DEC_CCM);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_CCM_VERIFY_MAC);
FUNCDEF(postProc, CCtrlBASE, CMD_REQ_VERIFY_HMAC);



FUNCDEF(gen, CCtrlECC, CMD_REQ_PUK_ECC);
FUNCDEF(gen, CCtrlECC, CMD_REQ_REG_CERT_ECC);
FUNCDEF(gen, CCtrlECC, CMD_REQ_AUTH_ECC);
FUNCDEF(gen, CCtrlECC, CMD_REQ_HS1_ECC);
FUNCDEF(gen, CCtrlECC, CMD_ERR_HS2_ECC);


FUNCDEF(postProc, CCtrlECC, CMD_REQ_PUK_ECC);
FUNCDEF(postProc, CCtrlECC, CMD_REQ_REG_CERT_ECC);
FUNCDEF(postProc, CCtrlECC, CMD_REQ_AUTH_ECC);
FUNCDEF(postProc, CCtrlECC, CMD_REQ_HS1_ECC);
FUNCDEF(postProc, CCtrlECC, CMD_ERR_HS2_ECC);



FUNCDEF(gen, CCtrlRSA, CMD_REQ_PUK_RSA);
FUNCDEF(gen, CCtrlRSA, CMD_REQ_REG_CERT_RSA);
FUNCDEF(gen, CCtrlRSA, CMD_REQ_AUTH_RSA);
FUNCDEF(gen, CCtrlRSA, CMD_REQ_HS1_RSA);


FUNCDEF(postProc, CCtrlRSA, CMD_REQ_PUK_RSA);
FUNCDEF(postProc, CCtrlRSA, CMD_REQ_REG_CERT_RSA);
FUNCDEF(postProc, CCtrlRSA, CMD_REQ_AUTH_RSA);
FUNCDEF(postProc, CCtrlRSA, CMD_REQ_HS1_RSA);



