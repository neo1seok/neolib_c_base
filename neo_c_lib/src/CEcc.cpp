//#include "main.h"
#include <string>

#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <memory.h> 

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/crypto.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/ecdh.h>
#include <openssl/objects.h>
#include <openssl/aes.h>
#include <openssl/evp.h> //seed
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include "IRSD1410_DEFINE.h" 

#include "Util.h"

#include"CEcc.h"
#include"commfunc.h"

#define ECC_XOR 0


using namespace std;

//BYTE HexCharToByte(char hexChar);
//char ByteToHexChar(BYTE byte);

//int E_ECB_K1(BYTE* in, BYTE* out, BYTE sel_aes);
//int D_ECB_K1(BYTE* in, BYTE* out, BYTE sel_aes);


//int RSA_xor_chip(BYTE* i_data, BYTE* Nc, BYTE* Ec, BYTE* o_data);
//int RSA_xor_tsm(BYTE* i_data, BYTE* o_data);

const BYTE ecx_G[32] = {
	0x6B, 0x17, 0xD1, 0xF2, 0xE1, 0x2C, 0x42, 0x47,
	0xF8, 0xBC, 0xE6, 0xE5, 0x63, 0xA4, 0x40, 0xF2,
	0x77, 0x03, 0x7D, 0x81, 0x2D, 0xEB, 0x33, 0xA0,
	0xF4, 0xA1, 0x39, 0x45, 0xD8, 0x98, 0xC2, 0x96
};
const BYTE ecx_d[32] = {
	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
};

const BYTE ecx_K[32] = {
	0xF1, 0x34, 0x1f, 0xAB, 0xCD, 0xEC, 0xA1, 0x89,
	0xF2, 0x35, 0x2f, 0xAC, 0xCD, 0xBC, 0xA2, 0xA9,
	0xF3, 0x36, 0x3f, 0xAD, 0xFD, 0xAC, 0xA3, 0x89,
	0xF4, 0x37, 0x4f, 0xAE, 0xED, 0xEC, 0xA1, 0xD9,
};


int ECX_genkey(BYTE* d, BYTE* G, BYTE* Qchip);

CEcc::CEcc(void)
{
	
}

CEcc::~CEcc(void)
{
}



//////////////////////////// 초기화


int CEcc::keyinit(int gen)
{
	CBaseCrypt::keyinit(gen);
	G_ECCgroup = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);		//그룹할당

	G_ECCkey_CHIP	= EC_KEY_new();					//키할당
	G_ECCkey_A		= EC_KEY_new();
	G_ECCkey_B		= EC_KEY_new();
    G_puk_A         = EC_POINT_new(G_ECCgroup);
	EC_KEY_set_group(G_ECCkey_CHIP	,G_ECCgroup);	//그룹설정
	EC_KEY_set_group(G_ECCkey_A		,G_ECCgroup);
	EC_KEY_set_group(G_ECCkey_B		,G_ECCgroup);


	
	memset(&mInterParam.certECC, 0x00, sizeof(CERT_ECC));
	memset(&mInterParam.SDATA, 0x00, CERT_SDATASIZE);
	memset(&mInterParam.EHKDevice, 0x00, CERT_SDATASIZE);

	//memset(mInterParam.G_PIN, 0x00, 16);
	//memset(mInterParam.G_INFORM, 0x00, 16);
	
	//strcpy((char*)mInterParam.certECC.ID, "123123");
	//strcpy((char*)mInterParam.G_PIN, "456789");
	//strcpy((char*)mInterParam.G_INFORM, "ABCDEF");

	
	

	//memset(EHK_device,      0x46, 16);
    //memset(EHK_device + 16, 0x00, 16);
	PARAM_Clear();

	if(gen)
        ECC_KeyGen_TSM();
	else
        ECC_Keyinit_TSM();


	//Bn2Bin(G_RSAkey_TSM->e, mInterParam.Etsm, CERT_ETSMSIZE);
	//Bn2Bin(G_RSAkey_TSM->n, mInterParam.Ntsm, CERT_NTSMSIZE);

//    int field_size = EC_GROUP_get_degree(EC_KEY_get0_group(G_ECCkey_CHIP));
	
	

#if ECC_XOR
	ECX_genkey(ecx_d,ecx_G,ecx_Q);
#endif

	return 1;
}



int CEcc::keyfree()
{
	EC_KEY_free(G_ECCkey_TSM);
	EC_KEY_free(G_ECCkey_CHIP);
	EC_KEY_free(G_ECCkey_A);
	EC_KEY_free(G_ECCkey_B);
	EC_POINT_free(G_puk_A);
	return 1;
}

int CEcc::ECC_KeyGen_TSM()
{
	//TSM의 ECC키 생성
	G_ECCkey_TSM = EC_KEY_new();											//키할당
	EC_KEY_set_group(G_ECCkey_TSM,G_ECCgroup);							//그룹설정

	BN_CTX *CTX;
	CTX=BN_CTX_new();

	EC_KEY_generate_key(G_ECCkey_TSM); 	// 키 생성
	EC_KEY_check_key(G_ECCkey_TSM);		// 키 체크


	const EC_POINT	*ec_puk;
	const BIGNUM	*ec_prv;
	ec_puk = EC_KEY_get0_public_key(G_ECCkey_TSM); 		// 공개키 획득
	ec_prv = EC_KEY_get0_private_key(G_ECCkey_TSM); 		// 개인키 획득

	//파일에 씀
	FILE *fp = fopen("TSM_ECC_KEY.txt","w");
	//fopen_s(&fp,"TSM_ECC_KEY.txt","w");
	fprintf(fp,"%s\n%s\n",EC_POINT_point2hex(G_ECCgroup,ec_puk,POINT_CONVERSION_COMPRESSED,CTX),BN_bn2hex(ec_prv));
	fclose(fp);

	return 1;
}
bool CEcc::verifySign(CERT_ECC * pcertinfo)
{

	bool ret = true;
//	BYTE *C_otp;
//	BYTE *R_pp;
//	BYTE *S_pp;
	BYTE msg_Vc[32];
	SHA256_CTX c;
	ECDSA_SIG *sign_Vc;
	//BYTE P_otp[16];	// 6byte || zero_padding
//	SHA256_CTX ck;





	SHA256_Init(&c);
	//if (g_msgPos)	SHA256_Update(&c, G_MSG, g_msgPos);
	SHA256_Update(&c, pcertinfo->ID, 16);
	SHA256_Update(&c, pcertinfo->Qchip, 33);

	//SHA256_Update(&c, C_otp, 16);
	SHA256_Final(msg_Vc, &c);


	sign_Vc = ECDSA_SIG_new();


	BN_bin2bn(pcertinfo->SIGN_R, 32, sign_Vc->r);
	BN_bin2bn(pcertinfo->SIGN_S, 32, sign_Vc->s);

	if (1 != ECDSA_do_verify(msg_Vc, 32, sign_Vc, G_ECCkey_TSM))
	{
		char szTemp[1024];
		unsigned long err = ERR_get_error();

		const char *strerr2 = ERR_reason_error_string(err);
		char *strerr = ERR_error_string(err, szTemp);
		//printf("%s \n%s", strerr, strerr2);
		ret = false;
		
	}
	

	ECDSA_SIG_free(sign_Vc);


	return ret;




}
int CEcc::ECC_Keyinit_TSM()
{
//    EC_POINT *puk_t;		//공개키
//    BIGNUM *prv_t;			//개인키
	G_ECCkey_TSM = EC_KEY_new();						//키 할당
	EC_KEY_set_group(G_ECCkey_TSM,G_ECCgroup);			//그룹설정

	BN_CTX *CTX;
	CTX=BN_CTX_new();

	char Qchip[33*2+2];
	char d[32*2+2];

	//파일에서 읽어옴
	FILE *fp;
	fp = fopen("TSM_ECC_KEY.txt","r");
	if(fp){
			//fopen_s(&fp,"TSM_ECC_KEY.txt","r");
		fscanf(fp,"%s\n%s\n",Qchip,d);
		fclose(fp);

	}else{
		strcpy(Qchip,"02585FDBA42D56674FA1FFE4A32677D731C122CFB5FE07A37B9DBEEF0B4C8B98A5");
		strcpy(d,"FF054CD28B41B253C27592D3C5F191FCD899B625F8BE352A31C25772CA2595CE");

	}


	StringtoByteArray(Qchip, mInterParam.Qtsm, 2*sizeof(mInterParam.Qtsm));
	StringtoByteArray(d, mInterParam.Decc, 2*sizeof(mInterParam.Decc));


	UpdateTSM();
	UpdatePRIKEY();

#if 0
	//키값을 키에 저장
	puk_t = EC_POINT_new(G_ECCgroup);
	puk_t = EC_POINT_hex2point(G_ECCgroup, Qchip, puk_t, CTX);
	EC_KEY_set_public_key(G_ECCkey_TSM, puk_t);	//puk key setting
	//EC_POINT_free(puk_t);

	prv_t = BN_new();
	BN_hex2bn(&prv_t, d);
	EC_KEY_set_private_key(G_ECCkey_TSM, prv_t); //prv key setting
	//BN_free(prv_t);
	StringtoByteArray((const char *)EC_POINT_point2hex(G_ECCgroup, puk_t, POINT_CONVERSION_COMPRESSED, CTX), mInterParam.Qtsm, 33 * 2);
#endif // false


	return 1;
}


//////////////////////////// 발급

int CEcc::requestIdECC(BYTE* pbIna, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut)
{

	REQ_TITLE(requestIdECC);
	//ECC 기반 Chip에서 전송된 REQ_ID에서 SN이 list에 있는 것인지 확인하고 RES_ID packet을 생성한다.
	//선언
	BYTE *SDATA = mInterParam.SDATA;
	BYTE Q_TSM[33];
	DWORD CRC;

	BN_CTX *CTX;
	CTX=BN_CTX_new();
	const EC_POINT	*ec_puk;

	//프로세싱
	ec_puk = EC_KEY_get0_public_key(G_ECCkey_TSM); 		// 공개키 획득
	char * asdfasf =EC_POINT_point2hex(G_ECCgroup, ec_puk, POINT_CONVERSION_COMPRESSED, CTX);
	StringtoByteArray((const char *)EC_POINT_point2hex(G_ECCgroup, ec_puk, POINT_CONVERSION_COMPRESSED, CTX), Q_TSM, 33 * 2);	//공개키를 BYTE로 변형

  

	Gen_SDATA(SDATA, 
		mInterParam.certECC.ID, 
		mInterParam.G_PIN, 
		mInterParam.g_pinSize, 
		mInterParam.G_INFORM, 
		mInterParam.g_pinformSize);

	//머징
	*pdwLenOut = 1+16+32+33+2;
	pbOut[0] = CMD_REQ_PUK_ECC;
	memcpy(pbOut+1			,mInterParam.certECC.ID	,16);
	memcpy(pbOut+1+16		,SDATA	,32);
	memcpy(pbOut+1+16+32  	,Q_TSM	,33);

	//CRC = calcCRC(pbOut, 1+16+32+1+33);
	CRC = calcCRC(pbOut+1, 16+32+33);

	pbOut[1+16+32+33  ] = (BYTE)(CRC >> 8);
	pbOut[1+16+32+33+1] = (BYTE)(CRC);
	//로그
	appendLog( "  out data : ");
	B2Str(pbOut	,*pdwLenOut	);
	

	appendLog( "    REQ_PUK_ECC : %02X\n    ID : ", CMD_REQ_PUK_ECC);
	B2Str(mInterParam.certECC.ID	,16		);
	

	appendLog( "    SDATA : ");
	B2Str(SDATA	,32		);
	

	appendLog( "    Qchip(TSM) : ");
	B2Str(Q_TSM	,33		);
	

    appendLog( "    CRC : %04X\n", CRC);
	

	BN_CTX_free(CTX);
	return 1;
}

int CEcc::GenCertECC(CERT_ECC* pcert, EC_KEY* key_tsm)
{

	BYTE msg[32];

	SHA256_CTX c;
	SHA256_Init(&c);
	SHA256_Update(&c, pcert->ID, 16);
	SHA256_Update(&c, pcert->Qchip, 33);
	SHA256_Final(msg,&c);




	ECDSA_SIG *sign;
	sign = ECDSA_SIG_new();
	sign = ECDSA_do_sign((const unsigned char*)msg,32,key_tsm);

	//StringtoByteArray(BN_bn2hex(sign->r),cert->SIGN_R,32*2);
	//StringtoByteArray(BN_bn2hex(sign->s),cert->SIGN_S,32*2);
	Bn2Bin(sign->r, pcert->SIGN_R, 32);
	Bn2Bin(sign->s, pcert->SIGN_S, 32);


	//memcpy(cert->ID,mInterParam.certECC.ID,16);
	//memcpy(cert->Qchip,Q_chip,33);

	ECDSA_SIG_free(sign);



	return 1;
}


int CEcc::requestCertECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut)
{
	//ECC 기반 Chip에서 전송된 REQ_CERT를 확인하고 RES_CERT packet을 생성한다.
	//선언
	REQ_TITLE(requestCertECC);

	BYTE	*Q_chip;		//공개키 바이트
	char	Qc[33*2+2];		//공개키 캐릭터
	EC_POINT *puk_t;		//공개키 포인트

	DWORD CRC;

	BN_CTX *CTX;
	int qchipsize = sizeof(this->mInterParam.certECC.Qchip);


	//파싱
	appendLog( "  input data : ");
	B2Str(pbIn	,iLenIn		);

	if(pbIn[0] != CMD_RES_PUK_ECC)
    {
        appendLog( "  not RES_PUK_ECC : %02X\n", pbIn[0]);
        return -1;
    }
	if (iLenIn != 1+33+2)
    {
        appendLog( "  length error\n");
        return -1;
    }
	appendLog( "    RES_PUK_ECC : %02X\n  Qchip(Chip) : ", pbIn[0]);
	B2Str(pbIn + 1	,33		);

	CTX=BN_CTX_new();
	Q_chip = pbIn+1;
	
	ByteArraytoString(Q_chip, Qc, qchipsize);
	puk_t = EC_POINT_new(G_ECCgroup);
	puk_t = EC_POINT_hex2point(G_ECCgroup, Qc, puk_t, CTX);
	EC_KEY_set_public_key(G_ECCkey_CHIP, puk_t);	//puk key setting

	memcpy(this->mInterParam.certECC.Qchip, Q_chip, qchipsize);

	//crc check
	//CRC = calcCRC(pbIn, iLenIn - 2);
	CRC = calcCRC(pbIn+1, iLenIn - 2 - 1);

	BN_CTX_free(CTX);
	EC_POINT_free(puk_t);

    if (CRC != ((pbIn[iLenIn - 2] << 8) + pbIn[iLenIn - 1]))
    {
        appendLog( "  CRC error(calculated CRC : %04X)\n", CRC);
		//에러 신호
		*pdwLenOut =2;
		pbOut[0] = CMD_ERR_CERT_ECC;
		pbOut[1] = EC_ER_CRC;

        return -1;
    }

	return 1;
}

int CEcc::requestRegCertECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut)
{
	//ECC 기반 Chip에서 전송된 REQ_CERT를 확인하고 RES_CERT packet을 생성한다.
	//선언

	REQ_TITLE(requestRegCertECC);

	CERT_ECC cert;
	DWORD CRC;

    //데이터 프로세싱
	GenCertECC(&mInterParam.certECC, G_ECCkey_TSM);

	cert = mInterParam.certECC;

	//머징
	//RES_CERT(1) || CERT(113) || CRC(2)
	*pdwLenOut = 1+113+2;
	pbOut[0] = CMD_REQ_REG_CERT_ECC;
	memcpy(pbOut+1		,&cert	,113);

    CRC = calcCRC(pbOut+1, 113);
	pbOut[1+113  ] = (BYTE)(CRC >> 8);
	pbOut[1+113+1] = (BYTE)(CRC		);


	//로그
	appendLog( "  out data : ");
	B2Str(pbOut	,*pdwLenOut	);

	appendLog( "    REQ_REG_CERT_ECC : %02X\n    ID : ", CMD_REQ_REG_CERT_ECC);
	B2Str(cert.ID	,16		);

	appendLog( "    Qchip(Chip) : ");
	B2Str(cert.Qchip	,33		);

	appendLog( "    Sign_R  : ");
	B2Str(cert.SIGN_R	,32		);

	appendLog( "    Sign_S  : ");
	B2Str(cert.SIGN_S	,32		);

	appendLog( "    CRC : %04X\n", CRC);

	return 1;
}

//////////////////////////// 사용
 

int CEcc::requestRNECC(BYTE *pbOut, DWORD *pdwLenOut)
{
	REQ_TITLE(requestRNECC);
	BYTE *SDATA = mInterParam.SDATA;// [32];
	//선언
	//프로세싱

	//머징
	//pbOut = REQ_AUTH(1) || SDATA(32) || EHK_device(32)
	*pdwLenOut = 1+32+32;
	pbOut[0] = CMD_REQ_AUTH_ECC;
	
	//Gen_SDATA(SDATA,mInterParam.certECC.ID,G_PIN,G_INFORM);


	memcpy(pbOut + 1, SDATA, CERT_SDATASIZE);
	memcpy(pbOut + 1 + CERT_SDATASIZE, mInterParam.EHKDevice, CERT_EHKSIZE);


	//로그
    appendLog( "  out data : ");
	B2Str(pbOut		,*pdwLenOut	);

	appendLog( "    REQ_AUTH_ECC %02X\n  SDATA  : ", CMD_REQ_AUTH_ECC);
	B2Str(SDATA, CERT_SDATASIZE);

	appendLog( "    EHK(device)  : ");
	B2Str(mInterParam.EHKDevice, CERT_EHKSIZE);

	return 1;
}



int CEcc::requestSessionECC(BYTE* pbIn, int iLenIn)
{
	REQ_TITLE(requestSessionECC);

	//ECC 기반 Chip에서 전송된 REQ_SESSION를 확인하고 REQ_HS1 packet을 생성한다.
	//선언
	BYTE *ID;
	BYTE *A;
	BYTE *SIGN_R;
	BYTE *SIGN_S;

	BN_CTX *CTX;

	////sign
	BYTE msg_Vc[32];
	char Rc[32*2+2];
	char Sc[32*2+2];
	char Ac[33*2+2];

	SHA256_CTX c_Vc;
	ECDSA_SIG *sign_Vc;


	CTX=BN_CTX_new();

	//파싱
	appendLog( "  input data : ");
	B2Str(pbIn	,iLenIn		);

	if(pbIn[0] != CMD_REQ_SESSION_ECC)
	{
		appendLog( "  not REQ_SESSION_ECC : %02X\n", pbIn[0]);
		return -1;
	}

	if (iLenIn != (1 + 16 + 33 + 32 + 32))
	{
		appendLog( "  length error\n");
		return -1;
	}
    appendLog( "    REQ_SESSION_ECC : %02X\n", pbIn[0]);

	ID = pbIn+1;
	A  = pbIn+1+16;
	SIGN_R  = pbIn+1+16+33;
	SIGN_S  = pbIn+1+16+33+32;

	appendLog( "    ID : ");	B2Str(ID,16		);
	appendLog( "    A : ");	B2Str(A	,33		);
	appendLog( "    SIGN_R : ");	B2Str(SIGN_R	,32		);
	appendLog( "    SIGN_S : ");	B2Str(SIGN_S	,32		);

	SHA256_Init(&c_Vc);
	SHA256_Update(&c_Vc,ID,16);
	SHA256_Update(&c_Vc,A,33);
	SHA256_Final(msg_Vc,&c_Vc);

	sign_Vc = ECDSA_SIG_new();
	ByteArraytoString(SIGN_R,Rc,32);
	ByteArraytoString(SIGN_S,Sc,32);
	BN_hex2bn(&sign_Vc->r,Rc);
	BN_hex2bn(&sign_Vc->s,Sc);

	////EC_KEY A
	ByteArraytoString(A,Ac,33);
	memcpy(mInterParam.A, A, 33);

	G_puk_A = EC_POINT_hex2point(G_ECCgroup, Ac, G_puk_A, CTX);
	EC_KEY_set_public_key(G_ECCkey_A,G_puk_A);	//puk key setting


	//프로세싱
	////서명검증

	if(1 != ECDSA_do_verify(msg_Vc,32,sign_Vc,G_ECCkey_CHIP))
	{
		appendLog( "  sign error\n");
		return -1;
	}
	////키확인
	if(1 != EC_KEY_check_key(G_ECCkey_A))	// 키 체크
	{
		appendLog( "  A public key error\n");
		return -1;
	}

	BN_CTX_free(CTX);
	ECDSA_SIG_free(sign_Vc);
	return 1;
}
#if 0
//for test old
int requestSessionECCold(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut)
{
	//ECC 기반 Chip에서 전송된 REQ_SESSION를 확인하고 REQ_HS1 packet을 생성한다.
	//선언
	BYTE *ID;
	BYTE *A;
	BYTE *SIGN_R;
	BYTE *SIGN_S;

	BN_CTX *CTX;
	CTX=BN_CTX_new();

	//머징
	*pdwLenOut =2;
	pbOut[0] = CMD_REQ_HS1_ECC;
	pbOut[1] = EC_NO_ERR;


	//파싱
	appendLog( "input data : ");
	B2Str(pbIn	,iLenIn		);

	if(pbIn[0] != CMD_REQ_SESSION_ECC)
	{
		appendLog( "not REQ_SESSION_ECC : %02X\n", pbOut[0]);
		return -1;
	}

	if (iLenIn != (1 + 16 + 33 + 32 + 32))
	{
		appendLog( "length error\n");
		return -1;
	}
    appendLog( "  REQ_SESSION_ECC : %02X\n", pbIn[0]);

	ID = pbIn+1;
	A  = pbIn+1+16;
	SIGN_R  = pbIn+1+16+33;
	SIGN_S  = pbIn+1+16+33+32;

	appendLog( "  ID : ");	B2Str(ID,16		);
	appendLog( "  A : ");	B2Str(A	,33		);
	appendLog( "  SIGN_R : ");	B2Str(SIGN_R	,32		);
	appendLog( "  SIGN_S : ");	B2Str(SIGN_S	,32		);

	////sign
	BYTE msg_Vc[32];
	char Rc[32*2+2];
	char Sc[32*2+2];

	SHA256_CTX c_Vc;
	SHA256_Init(&c_Vc);
	SHA256_Update(&c_Vc,ID,16);
	SHA256_Update(&c_Vc,A,33);
	SHA256_Final(msg_Vc,&c_Vc);

	ECDSA_SIG *sign_Vc;
	sign_Vc = ECDSA_SIG_new();
	ByteArraytoString(SIGN_R,Rc,32);
	ByteArraytoString(SIGN_S,Sc,32);
	BN_hex2bn(&sign_Vc->r,Rc);
	BN_hex2bn(&sign_Vc->s,Sc);

	////EC_KEY A
	char Ac[32*2+2];
	ByteArraytoString(A,Ac,33);

	EC_POINT	*puk_A;
	puk_A = EC_POINT_new(G_ECCgroup);
	puk_A = EC_POINT_hex2point(G_ECCgroup,Ac,puk_A,CTX);
	EC_KEY_set_public_key(G_ECCkey_A,puk_A);	//puk key setting


	//프로세싱
	////서명검증

	if(1 != ECDSA_do_verify(msg_Vc,32,sign_Vc,G_ECCkey_CHIP))
	{
		appendLog( "sign error\n");
		pbOut[0] = CMD_ERR_SESSION_ECC;
		pbOut[1] = EC_ER_ECC_SIGN;
		return -1;
	}
	////키확인
	if(1 != EC_KEY_check_key(G_ECCkey_A))	// 키 체크
	{
		appendLog( "A public key error\n");
		pbOut[0] = CMD_ERR_SESSION_ECC;
		pbOut[1] = EC_ER_PNT;
		return -1;
	}
	////키쌍 생성 B
	EC_KEY_generate_key(G_ECCkey_B);

	BYTE B[33];	//공개키 바이트
	EC_POINT_point2oct(G_ECCgroup, EC_KEY_get0_public_key(G_ECCkey_B), POINT_CONVERSION_COMPRESSED, B, 33, CTX);


	////ECDH
	EC_POINT *ECDH_P;
	ECDH_P = EC_POINT_new(G_ECCgroup);
	BYTE ECDH_P_x[33+4];

	EC_POINT_mul(G_ECCgroup, ECDH_P, NULL, puk_A, EC_KEY_get0_private_key(G_ECCkey_B),CTX);
	EC_POINT_point2oct(G_ECCgroup, ECDH_P, POINT_CONVERSION_COMPRESSED, ECDH_P_x, 33, CTX);

	////KDF로 키생성
	BYTE sharedkey1[32];
	BYTE sharedkey2[32];

	ECDH_P_x[29+4]= 0x00;	//KDF2 32bit counter
	ECDH_P_x[30+4]= 0x00;
	ECDH_P_x[31+4]= 0x00;
	ECDH_P_x[32+4]= 0x00;
	SHA256(ECDH_P_x+1,36,sharedkey1);

	ECDH_P_x[32+4]= 0x01;
	SHA256(ECDH_P_x+1,36,sharedkey2);


	memcpy(mInterParam.K1,sharedkey1		,16);
	memcpy(mInterParam.K2,sharedkey1+16	,16);
	memcpy(mInterParam.IV,sharedkey2		,16);

	appendLog( "key data\n ");
	appendLog( "  mInterParam.K1 : ");	B2Str(mInterParam.K1	 ,16	);
	appendLog( "  mInterParam.K2 : ");	B2Str(mInterParam.K2	 ,16	);
	appendLog( "  mInterParam.IV : ");	B2Str(mInterParam.IV	 ,16	);
	appendLog( "\n");

	////서명생성
	BYTE msg[32];
	BYTE R_prime[32];
	BYTE S_prime[32];

	SHA256_CTX c;
	SHA256_Init(&c);

	SHA256_Update(&c,ID,16);
	SHA256_Update(&c,A,33);
	SHA256_Update(&c,B,33);
#if 0
	SHA256_Update(&c,A,33);
	SHA256_Update(&c,B,33);
	SHA256_Update(&c,ID,16);
#endif
	SHA256_Final(msg,&c);

	ECDSA_SIG *sign;
	sign = ECDSA_SIG_new();

    sign = ECDSA_do_sign((const unsigned char*)msg,32,G_ECCkey_TSM);

    {
        int iLenR, iLenS;
        char *pbR = BN_bn2hex(sign->r);
        char *pbS = BN_bn2hex(sign->s);
        iLenR = strlen(pbR);
        iLenS = strlen(pbS);
        memset(R_prime, 0, 8);
        memset(S_prime, 0, 8);

        StringtoByteArray(pbR, R_prime + 32 - iLenR / 2, iLenR);
        StringtoByteArray(pbS, S_prime + 32 - iLenS / 2, iLenS);
    }

	////Ax암호화
	BYTE C_ECB[16];
	E_ECB_K1(A+1+16, C_ECB, mInterParam.SEL_AES);	//+++

	//머징
	////REQ_HS1(1) || B(33) || SIGN_R`(32) || SIGN_S`(32) || C_ECB(16);
	*pdwLenOut = 1+33+32+32+16;
	pbOut[0] = CMD_REQ_HS1_ECC;
	memcpy(pbOut+1			,B,33);
	memcpy(pbOut+1+33		,R_prime,32);
	memcpy(pbOut+1+33+32	,S_prime,32);
	memcpy(pbOut+1+33+32+32	,C_ECB,16);

	//로그
	appendLog( "out data : "); 	B2Str(pbOut	,*pdwLenOut	);
	appendLog( "  REQ_HS1 : %02X\n", CMD_REQ_HS1_ECC);
    appendLog( "  sha : ");	B2Str(msg			    ,32	);
    appendLog( "  B : ");	B2Str(pbOut+1			,33	);
	appendLog( "  SIGN_R`: ");	B2Str(pbOut+1+33		,32	);
	appendLog( "  SIGN_S`: ");	B2Str(pbOut+1+33+32		,32	);
	appendLog( "  C1 : ");	B2Str(pbOut+1+33+32+32	,16	);

	BN_CTX_free(CTX);
	ECDSA_SIG_free(sign_Vc);
	ECDSA_SIG_free(sign);
	EC_POINT_free(ECDH_P);
	EC_POINT_free(puk_A);
	return 1;
}
#endif

int CEcc::requestHS1ECC(BYTE* pbOut, DWORD* pdwLenOut)
{
	REQ_TITLE(requestHS1ECC);

	//ECC 기반 Chip에서 전송된 REQ_SESSION를 확인하고 REQ_HS1 packet을 생성한다.
	//선언
	BN_CTX *CTX;
	BYTE B[33];	//공개키 바이트
	EC_POINT *ECDH_P;
	BYTE sharedkey1[32];
	BYTE sharedkey2[32];
	BYTE ECDH_P_x[33+4];
	BYTE msg[32];
	BYTE R_prime[32];
	BYTE S_prime[32];
	BYTE C_ECB[16];
	ECDSA_SIG *sign;

	SHA256_CTX c;


	CTX=BN_CTX_new();

	//머징
	*pdwLenOut =2;
	pbOut[0] = CMD_REQ_HS1_ECC;
	pbOut[1] = EC_NO_ERR;



	////키쌍 생성 B
	EC_KEY_generate_key(G_ECCkey_B);

	EC_POINT_point2oct(G_ECCgroup, EC_KEY_get0_public_key(G_ECCkey_B), POINT_CONVERSION_COMPRESSED, B, 33, CTX);


	////ECDH
	ECDH_P = EC_POINT_new(G_ECCgroup);

	EC_POINT_mul(G_ECCgroup, ECDH_P, NULL, G_puk_A, EC_KEY_get0_private_key(G_ECCkey_B),CTX);
	EC_POINT_point2oct(G_ECCgroup, ECDH_P, POINT_CONVERSION_COMPRESSED, ECDH_P_x, 33, CTX);

	////KDF로 키생성

	ECDH_P_x[29+4]= 0x00;	//KDF2 32bit counter
	ECDH_P_x[30+4]= 0x00;
	ECDH_P_x[31+4]= 0x00;
	ECDH_P_x[32+4]= 0x00;
	SHA256(ECDH_P_x+1,36,sharedkey1);

	ECDH_P_x[32+4]= 0x01;
	SHA256(ECDH_P_x+1,36,sharedkey2);


	memcpy(mInterParam.K1,sharedkey1,16);
	memcpy(mInterParam.K2,sharedkey1+16	,16);
	memcpy(mInterParam.IV,sharedkey2,16);

	appendLog( "  key data\n");
	appendLog( "    mInterParam.K1 : ");	B2Str(mInterParam.K1	 ,16	);
	appendLog( "    mInterParam.K2 : ");	B2Str(mInterParam.K2	 ,16	);
	appendLog( "    mInterParam.IV : ");	B2Str(mInterParam.IV	 ,16	);
	appendLog( "\n");

	////서명생성
	SHA256_Init(&c);

	SHA256_Update(&c,mInterParam.certECC.ID,16);
	SHA256_Update(&c, mInterParam.A, 33);
	SHA256_Update(&c,B,33);
#if 0
	SHA256_Update(&c,G_A_CHIP,33);
	SHA256_Update(&c,B,33);
	SHA256_Update(&c,mInterParam.certECC.ID,16);
#endif
	SHA256_Final(msg,&c);


	sign = ECDSA_SIG_new();

    sign = ECDSA_do_sign((const unsigned char*)msg,32,G_ECCkey_TSM);

    {
        int iLenR, iLenS;
        const char *pbR = BN_bn2hex(sign->r);
		const char *pbS = BN_bn2hex(sign->s);
        iLenR = (int)strlen(pbR);
        iLenS = (int)strlen(pbS);
        memset(R_prime, 0, 8);
        memset(S_prime, 0, 8);

		StringtoByteArray((const char *)pbR, R_prime + 32 - iLenR / 2, iLenR);
		StringtoByteArray((const char *)pbS, S_prime + 32 - iLenS / 2, iLenS);
    }

	////Ax암호화
	E_ECB_K1(mInterParam.A + 1 + 16, C_ECB, mInterParam.SEL_AES);	//+++


	//머징
	////REQ_HS1(1) || B(33) || SIGN_R`(32) || SIGN_S`(32) || C_ECB(16);
	*pdwLenOut = 1+33+32+32+16;
	pbOut[0] = CMD_REQ_HS1_ECC;
	memcpy(pbOut+1			,B,33);
	memcpy(pbOut+1+33		,R_prime,32);
	memcpy(pbOut+1+33+32	,S_prime,32);
	memcpy(pbOut+1+33+32+32	,C_ECB,16);

	//로그
	appendLog( "  out data : "); 	B2Str(pbOut	,*pdwLenOut	);
	appendLog( "    REQ_HS1 : %02X\n", CMD_REQ_HS1_ECC);
    appendLog( "    sha : ");	B2Str(msg			    ,32	);
    appendLog( "    B : ");	B2Str(pbOut+1			,33	);
	appendLog( "    SIGN_R`: ");	B2Str(pbOut+1+33		,32	);
	appendLog( "    SIGN_S`: ");	B2Str(pbOut+1+33+32		,32	);
	appendLog( "    C1 : ");	B2Str(pbOut+1+33+32+32	,16	);

	BN_CTX_free(CTX);
	ECDSA_SIG_free(sign);
	EC_POINT_free(ECDH_P);
	return 1;
}

string CEcc::viewCETEXT(){
	string str = "";
	str += "ID:";
	str += BytetoHexStr(mInterParam.certECC.ID, CERT_IDSIZE);
	str += "\n";
	str += "QChip:";
	str += BytetoHexStr(mInterParam.certECC.Qchip, CERT_QCHIPSIZE);
	str += "\n";
	str += "Sign_R:";
	str += BytetoHexStr(mInterParam.certECC.SIGN_R, CERT_SIGNSIZE_ECC);
	str += "\n";
	str += "Sign_S:";
	str += BytetoHexStr(mInterParam.certECC.SIGN_S, CERT_SIGNSIZE_ECC);
	str += "\n";
	str += "SDATA:";
	str += BytetoHexStr(mInterParam.SDATA, CERT_SDATASIZE);
	str += "\n";
	str += "EHK:";
	str += BytetoHexStr(mInterParam.EHKDevice, CERT_EHKSIZE);
	str += "\n";

	return str;
}


int CEcc::requestHS2ECC(BYTE* pbIn, int iLenIn)
{
	REQ_TITLE(requestHS2ECC);

	//ECC 기반 Chip에서 전송된 REQ_HS2를 확인하고 response packet을 생성한다.
	//선언
	mInterParam.g_msgPos = 0;

	BYTE B[33];
	BYTE Bx[16];
	BYTE *C2;		//16 byte
	BYTE *C3;		//32 byte
	BN_CTX *CTX;
	CTX = BN_CTX_new();

	//파싱
	appendLog( "  input data : ");
	B2Str(pbIn	,iLenIn		);

	if(pbIn[0] != CMD_REQ_HS2_ECC)
    {
        appendLog( "  not CMD_REQ_HS2_ECC : %02X\n", pbIn[0]);
        return -1;
    }

    if (iLenIn != (1 + 16 + 32))
    {
        appendLog( "  length error\n");
        return -1;
    }
    appendLog( "    REQ_HS2_ECC : %02X\n", pbIn[0]);

	C2 = pbIn+1;
	C3 = pbIn+1+16;
	appendLog( "    C2 : ");	B2Str(C2,16	);
	appendLog( "    C3 : ");	B2Str(C3,32	);



	EC_POINT_point2oct(G_ECCgroup, EC_KEY_get0_public_key(G_ECCkey_B), POINT_CONVERSION_COMPRESSED, B, 33, CTX);

	//프로세싱
	////C2 복호화

	D_ECB_K1(C2, Bx, mInterParam.SEL_AES);

	////B를 비교

	if(0 != memcmp(Bx,B+1+16,16))
	{
		appendLog( "  Bx compare error\n");
		return -1;
	}

	appendLog( "    Bx : ");   B2Str(Bx	,16	);

	////C3 복호화

	D_ECB_K1(C3, mInterParam.HK_device, mInterParam.SEL_AES);
	D_ECB_K1(C3 + 16, mInterParam.HK_device + 16, mInterParam.SEL_AES);

	////HK 비교
	//처음은 비교 안함

	if (0 != memcmp(mInterParam.HK_device, mInterParam.HK_tsm, 32))
	{
		appendLog( "  HK compare error\n");
		//return -1;
	}
	appendLog( "    HK : ");   B2Str(mInterParam.HK_device, 32);

	BN_CTX_free(CTX);
	return 1;
}




int CEcc::storeEHKdeviceECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut)
{
	//파싱
	appendLog( "  input data : ");
	B2Str(pbIn	,iLenIn		);

	if(pbIn[0] != CMD_RES_SIGN_ECC)
	{
		appendLog( "  not CMD_RES_SIGN_ECC : %02X\n", pbOut[0]);
		return -1;
	}
	if (iLenIn != 1+32)
	{
		appendLog( "  length error\n");
		return -1;
	}
    appendLog( "    RES_SIGN : %02X\n", pbIn[0]);

    //저장
	memcpy(mInterParam.EHKDevice, pbIn + 1, CERT_EHKSIZE);
	//로그
	appendLog( "    EHK`(device) : ");
	B2Str(mInterParam.EHKDevice, CERT_EHKSIZE);
	return 1;
}

int CEcc::requestSignECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE* pbOTP)
{
	REQ_TITLE(requestSignECC);

	//ECC 기반 Chip에서 전송된 REQ_CONFIRM를 확인하고 response packet을 생성한다.
	//선언
	BYTE *C_otp;
	BYTE *R_pp;
	BYTE *S_pp;
	BYTE msg_Vc[32];
	char Rc[32*2+2];
	char Sc[32*2+2];
	SHA256_CTX c;
	ECDSA_SIG *sign_Vc;
	BYTE P_otp[16];	// 6byte || zero_padding
	SHA256_CTX ck;

	//머징
	*pdwLenOut =2;
	pbOut[0] = CMD_ERR_SIGN;
	pbOut[1] = EC_NO_ERR;

	//파싱	cmd(1) ||  C_opt(16) || SIGN_R``(32) || SIGN_S``(32)
	appendLog( "  input data : ");
	B2Str(pbIn	,iLenIn		);
	if(pbIn[0] != CMD_RES_SIGN_ECC)
	{
		appendLog( "  not RES_SIGN_ECC : %02X\n", pbOut[0]);
		return -1;
	}
	if (iLenIn != 1+16+32+32)
	{
		appendLog( "  length error\n");
		return -1;
	}
    appendLog( "    RES_SIGN_ECC : %02X\n", pbIn[0]);

    C_otp	= pbIn + 1;
	R_pp	= pbIn + 1 + 16;
	S_pp	= pbIn + 1 + 16 + 32;

	appendLog( "    C(OTP) : ");	B2Str(C_otp	,16	);
	appendLog( "    SIGN_R(pp) : ");	B2Str(R_pp	,32	);
	appendLog( "    SIGN_S(pp) : ");	B2Str(S_pp	,32	);

	//프로세싱
	////1. ECDSA_Vchip( msg || ID || C_otp)
/*
	SHA256_CTX c_Vc;
	SHA256_Init(&c_Vc);
	SHA256_Update(&c_Vc,ID,16);
	SHA256_Update(&c_Vc,msg,33);
	SHA256_Update(&c_Vc,C_otp,16);
	SHA256_Final(msg_Vc,&c_Vc);
*/
	SHA256_Init(&c);
	if (mInterParam.g_msgPos)	SHA256_Update(&c, mInterParam.G_MSG, mInterParam.g_msgPos);
	SHA256_Update(&c, mInterParam.certECC.ID, 16);
	//SHA256_Update(&c, C_otp, 16);
	SHA256_Final(msg_Vc,&c);



	appendLog( "    Hasg(msg||ID) : ");	B2Str(msg_Vc	,32	);

	sign_Vc = ECDSA_SIG_new();

	ByteArraytoString(R_pp,Rc,32);
	ByteArraytoString(S_pp,Sc,32);
	BN_hex2bn(&sign_Vc->r,Rc);
	BN_hex2bn(&sign_Vc->s,Sc);

	if(1 != ECDSA_do_verify(msg_Vc,32,sign_Vc,G_ECCkey_CHIP))
	{
		appendLog( "  ECC_sign verify error\n");
		pbOut[1] = EC_ER_ECC_SIGN;
		return -2;
	}


	////2. D_ECB:k1(C_otp) == GenerateUserOTP()

	D_ECB_K1(C_otp,		P_otp,	mInterParam.SEL_AES);


	if (memcmp(pbOTP, P_otp, 3) != 0)
	{
		appendLog( "  OTP compare error\n");
		pbOut[1] = EC_ER_OTP;
		return -1;
	}
	appendLog( "    OTP : ");	B2Str(P_otp		,6		);

	////3. HK`_tsm = hash(mInterParam.K1||mInterParam.K2||mInterParam.IV)
	SHA256_Init(&ck);
	SHA256_Update(&ck,mInterParam.K1,16);
	SHA256_Update(&ck,mInterParam.K2,16);
	SHA256_Update(&ck,mInterParam.IV,16);
	SHA256_Final(mInterParam.HK_tsm, &ck);
	appendLog( "    HK`(TSM) : ");	B2Str(mInterParam.HK_tsm, 32);

	////4. store(HK`_tsm)
	//3번에서 처리


	ECDSA_SIG_free(sign_Vc);

	pbOut[0] = CMD_ERR_SIGN;
   	pbOut[1] = EC_NO_ERR;


	//로그
	appendLog( "out data : ");	B2Str(pbOut	,*pdwLenOut	);
    appendLog( "  %02X(ERR_SIGN), %02X\n",
        CMD_ERR_SIGN, pbOut[1]);


	return 1;
}

#if 0
CERTEXT_ECC * CEcc::getCERTEXT_ECC(){
	return &this->m_certext;

}


void CEcc::ISSUE(const unsigned char * btID,int IdSize, const unsigned char * btQchip,int qChipSize)
{
	ISSUE(btID,IdSize);
	GETCERT( btQchip,qChipSize);
#if 0
	BYTE *SDATA = mInterParam.SDATA;
	BYTE Q_TSM[33];
	DWORD CRC;

	memcpy(mInterParam.certECC.ID, btID, IdSize);
	memcpy(mInterParam.certECC.Qchip, btQchip, qChipSize);

	BN_CTX *CTX;
	CTX = BN_CTX_new();
	const EC_POINT	*ec_puk;

	//프로세싱
	ec_puk = EC_KEY_get0_public_key(G_ECCkey_TSM); 		// 공개키 획득
	StringtoByteArray(EC_POINT_point2hex(G_ECCgroup, ec_puk, POINT_CONVERSION_COMPRESSED, CTX), Q_TSM, 33 * 2);	//공개키를 BYTE로 변형

	Gen_SDATA(SDATA, mInterParam.certECC.ID, mInterParam.G_PIN, mInterParam.G_INFORM);
	BN_CTX_free(CTX);

	//requestCertECC(inbuffer, size, outbuffer, &outsize, g_LogBuf);

	BYTE	*Q_chip = mInterParam.certECC.Qchip;		//공개키 바이트
	//BYTE	*tailCRC;
	char	Qc[33 * 2 + 2];		//공개키 캐릭터
	EC_POINT *puk_t;		//공개키 포인트

	CRC = 0;;

	//BN_CTX *CTX;
	int qchipsize = sizeof(this->mInterParam.certECC.Qchip);


	CTX = BN_CTX_new();
	//Q_chip = pbIn + 1;

	ByteArraytoString(Q_chip, Qc, qchipsize);
	puk_t = EC_POINT_new(G_ECCgroup);
	puk_t = EC_POINT_hex2point(G_ECCgroup, Qc, puk_t, CTX);
	EC_KEY_set_public_key(G_ECCkey_CHIP, puk_t);	//puk key setting

	memcpy(this->mInterParam.certECC.Qchip, Q_chip, qchipsize);

	//crc check
	//CRC = calcCRC(pbIn, iLenIn - 2);
	//CRC = calcCRC(Q_chip, qchipsize);

	BN_CTX_free(CTX);
	EC_POINT_free(puk_t);



	//requestRegCertECC(inbuffer, size, outbuffer, &outsize, g_LogBuf);

	//DWORD CRC;

	//데이터 프로세싱
	GenCertECC(&mInterParam.certECC, G_ECCkey_TSM);
#endif

}
#endif

void CEcc::UpdateCERT()
{
	
	
#if 0
	//BYTE	*Q_chip = mInterParam.certECC.Qchip;		//공개키 바이트
	//BYTE	*tailCRC;
	//char	Qc[33 * 2 + 2];		//공개키 캐릭터
	EC_POINT *puk_t;		//공개키 포인트

	DWORD CRC = 0;;

	//BN_CTX *CTX;
	int qchipsize = sizeof(this->mInterParam.certECC.Qchip);

	BN_CTX *CTX;
	CTX = BN_CTX_new();
	//Q_chip = pbIn + 1;

	ByteArraytoString(mInterParam.certECC.Qchip, Qc, qchipsize);
	puk_t = EC_POINT_new(G_ECCgroup);
	puk_t = EC_POINT_hex2point(G_ECCgroup, Qc, puk_t, CTX);
	EC_KEY_set_public_key(G_ECCkey_CHIP, puk_t);	//puk key setting

	//memcpy(this->mInterParam.certECC.Qchip, Q_chip, qchipsize);

	//crc check
	//CRC = calcCRC(pbIn, iLenIn - 2);
	//CRC = calcCRC(Q_chip, qchipsize);

	BN_CTX_free(CTX);
	EC_POINT_free(puk_t);
#endif // 0
	//UpdateTSM();
	resetSignHash();
	UpdateChip();

	//requestRegCertECC(inbuffer, size, outbuffer, &outsize, g_LogBuf);

	//DWORD CRC;

	//데이터 프로세싱
	GenCertECC(&mInterParam.certECC, G_ECCkey_TSM);

}
#if 0
bool CEcc::GET_C1(BYTE * C_ECB, BYTE *ID, int nID, BYTE *C, int nC, BYTE *E, int nE, BYTE *S, int nS)
{
	//requestSessionRSA();



	return true;
}
#endif // 0

void CEcc::TestECC(){

	

	//requestIdECC(NULL, size, outbuffer, &outsize, g_LogBuf);
	

	//머징
	//RES_CERT(1) || CERT(113) || CRC(2)
	
	


	
	

}

void CEcc::SetID(BYTE *pID, int idSize)
{
	
	memset(mInterParam.certECC.ID, 0, sizeof(mInterParam.certECC.ID));
	memcpy(mInterParam.certECC.ID, pID, idSize);

}
void CEcc::SetID(const char  *strID)
{
	
	memset(mInterParam.certECC.ID, 0, sizeof(mInterParam.certECC.ID));
	::hex2bin(strID, mInterParam.certECC.ID);


}

BYTE*	CEcc::getHKDevice(){

	return mInterParam.EHKDevice;

}

void CEcc::UpdatePUK(BYTE * q,EC_KEY		* ecckey)
{

	string strqtsm = ::BytetoHexStr(q, DEFECCSIZE);
	BN_CTX *CTX;
	CTX = BN_CTX_new();
	EC_POINT *puk_t;
	puk_t = EC_POINT_new(G_ECCgroup);
	puk_t = EC_POINT_hex2point(G_ECCgroup, strqtsm.c_str(), puk_t, CTX);

	EC_KEY_set_public_key(ecckey, puk_t);	//puk key setting
	EC_POINT_free(puk_t);
	BN_CTX_free(CTX);
}


void CEcc::UpdateTSM(){

	UpdatePUK(mInterParam.Qtsm, G_ECCkey_TSM);
	
#if 0
	string strqtsm = ::BytetoHexStr(mInterParam.Qtsm, sizeof(mInterParam.Qtsm));
	BN_CTX *CTX;
	CTX = BN_CTX_new();
	EC_POINT *puk_t;
	puk_t = EC_POINT_new(G_ECCgroup);
	puk_t = EC_POINT_hex2point(G_ECCgroup, strqtsm.c_str(), puk_t, CTX);

	EC_KEY_set_public_key(G_ECCkey_TSM, puk_t);	//puk key setting
	EC_POINT_free(puk_t);
	BN_CTX_free(CTX);
#endif // 0

}
void CEcc::UpdatePRIKEY(){

	BIGNUM *prv_t;
	prv_t = BN_new();
	BN_bin2bn(mInterParam.Decc, sizeof(mInterParam.Decc), prv_t);
	//BN_hex2bn(&prv_t, mInterParam.Decc);
	EC_KEY_set_private_key(G_ECCkey_TSM, prv_t); //prv key setting
}
void CEcc::UpdateChip()
{
	UpdatePUK(mInterParam.certECC.Qchip, G_ECCkey_CHIP);


#if 0
	string strqtsm = ::BytetoHexStr(mInterParam.Qtsm, sizeof(mInterParam.Qtsm));
	BN_CTX *CTX;
	CTX = BN_CTX_new();
	EC_POINT *puk_t;
	puk_t = EC_POINT_new(G_ECCgroup);
	puk_t = EC_POINT_hex2point(G_ECCgroup, strqtsm.c_str(), puk_t, CTX);

	EC_KEY_set_public_key(G_ECCkey_TSM, puk_t);	//puk key setting
	EC_POINT_free(puk_t);
	BN_CTX_free(CTX);
#endif // 0

}

bool  CEcc::UpdateC1()
{
	REQ_TITLE(UpdateC1);

	//ECC 기반 Chip에서 전송된 REQ_SESSION를 확인하고 REQ_HS1 packet을 생성한다.
	//선언
	

	BN_CTX *CTX;

	////sign
	BYTE msg_Vc[32];
	//char Rc[32 * 2 + 2];
	//char Sc[32 * 2 + 2];
	//char Ac[33 * 2 + 2];

	SHA256_CTX c_Vc;
	ECDSA_SIG *sign_Vc;


	CTX = BN_CTX_new();

	//파싱




	appendLog("    ID : ");	B2Str(mInterParam.ID, 16);
	appendLog("    A : ");	B2Str(mInterParam.A, 33);
	appendLog("    SIGN_R : ");	B2Str(mInterParam.SR, 32);
	appendLog("    SIGN_S : ");	B2Str(mInterParam.SS, 32);

	SHA256_Init(&c_Vc);
	SHA256_Update(&c_Vc, mInterParam.certECC.ID, 16);
	SHA256_Update(&c_Vc, mInterParam.A, 33);
	SHA256_Final(msg_Vc, &c_Vc);

	sign_Vc = ECDSA_SIG_new();
	
#if 0
	ByteArraytoString(mInterParam.Sr, Rc, 32);
	ByteArraytoString(mInterParam.Ss, Sc, 32);

	BN_hex2bn(&sign_Vc->r, Rc);
	BN_hex2bn(&sign_Vc->s, Sc);
#endif // 0


	sign_Vc->r = BN_bin2bn(mInterParam.SR,DEFECCSIZE_SIGN,NULL);
	sign_Vc->s = BN_bin2bn(mInterParam.SS,DEFECCSIZE_SIGN,NULL);

	////EC_KEY A
	//ByteArraytoString(A, Ac, 33);
	string Ac = BytetoHexStr(mInterParam.A, DEFECCSIZE);
	//memcpy(mInterParam.G_A_CHIP, mInterParam.A, 33);

	G_puk_A = EC_POINT_hex2point(G_ECCgroup, Ac.c_str(), G_puk_A, CTX);
	EC_KEY_set_public_key(G_ECCkey_A, G_puk_A);	//puk key setting


	//프로세싱
	////서명검증

	if (1 != ECDSA_do_verify(msg_Vc, 32, sign_Vc, G_ECCkey_CHIP))
	{
		errlog = "  sign error\n";
		return false;
	}

	////키확인
	if (1 != EC_KEY_check_key(G_ECCkey_A))	// 키 체크
	{
		errlog = "A public key error\n";
		return false;
	}

	BN_CTX_free(CTX);
	ECDSA_SIG_free(sign_Vc);
//hs1



	//BN_CTX *CTX;
	
	EC_POINT *ECDH_P;
	BYTE sharedkey1[32];
	BYTE sharedkey2[32];
	BYTE ECDH_P_x[33 + 4];
	BYTE msg[32];
	
	
//	BYTE C_ECB[16];
	ECDSA_SIG *sign;

	SHA256_CTX c;


	CTX = BN_CTX_new();

	

	////키쌍 생성 B
	EC_KEY_generate_key(G_ECCkey_B);

	EC_POINT_point2oct(G_ECCgroup, EC_KEY_get0_public_key(G_ECCkey_B), POINT_CONVERSION_COMPRESSED, mInterParam.B, 33, CTX);


	////ECDH
	ECDH_P = EC_POINT_new(G_ECCgroup);
	EC_POINT_mul(G_ECCgroup, ECDH_P, NULL, G_puk_A, EC_KEY_get0_private_key(G_ECCkey_B), CTX);
	EC_POINT_point2oct(G_ECCgroup, ECDH_P, POINT_CONVERSION_COMPRESSED, ECDH_P_x, 33, CTX);

	////KDF로 키생성

	ECDH_P_x[29 + 4] = 0x00;	//KDF2 32bit counter
	ECDH_P_x[30 + 4] = 0x00;
	ECDH_P_x[31 + 4] = 0x00;
	ECDH_P_x[32 + 4] = 0x00;
	SHA256(ECDH_P_x + 1, 36, sharedkey1);

	ECDH_P_x[32 + 4] = 0x01;
	SHA256(ECDH_P_x + 1, 36, sharedkey2);


	memcpy(mInterParam.K1, sharedkey1, 16);
	memcpy(mInterParam.K2, sharedkey1 + 16, 16);
	memcpy(mInterParam.IV, sharedkey2, 16);

	appendLog("  key data\n");
	appendLog("    mInterParam.K1 : ");	B2Str(mInterParam.K1, 16);
	appendLog("    mInterParam.K2 : ");	B2Str(mInterParam.K2, 16);
	appendLog("    mInterParam.IV : ");	B2Str(mInterParam.IV, 16);
	appendLog("\n");

	////서명생성
	SHA256_Init(&c);
	SHA256_Update(&c, mInterParam.certECC.ID, 16);
	SHA256_Update(&c, mInterParam.A, 33);
	SHA256_Update(&c, mInterParam.B, 33);
	SHA256_Final(msg, &c);


	sign = ECDSA_SIG_new();

	sign = ECDSA_do_sign((const unsigned char*)msg, 32, G_ECCkey_TSM);

	Bn2Bin(sign->r, mInterParam.SRp,DEFECCSIZE_SIGN);
	Bn2Bin(sign->s, mInterParam.SSp, DEFECCSIZE_SIGN);

	
	////Ax암호화
	E_ECB_K1(mInterParam.A + 1 + 16, mInterParam.C1, mInterParam.SEL_AES);	//+++




		//로그
	
	
	appendLog("    sha : ");	B2Str(msg, 32);
	appendLog("    B : ");	B2Str(mInterParam.B, 33);
	appendLog("    SIGN_R`: ");	B2Str(mInterParam.SRp, 32);
	appendLog("    SIGN_S`: ");	B2Str(mInterParam.SSp, 32);
	appendLog("    C1 : ");	B2Str(mInterParam.C1, 16);

	BN_CTX_free(CTX);
	ECDSA_SIG_free(sign);
	EC_POINT_free(ECDH_P);


	return true;

}

bool CEcc::UpdateSign()
{
	//BYTE EM[32];
//	updateSignHash(mInterParam.ID, CERT_IDSIZE);
//	getHashValue4Sign(EM);

	//BYTE *C_otp;
	//BYTE *R_pp;
	//BYTE *S_pp;
	BYTE msg_Vc[32];
	//char Rc[32 * 2 + 2];
	//char Sc[32 * 2 + 2];
	
	
	ECDSA_SIG *sign_Vc;

	BYTE P_otp[16];	// 6byte || zero_padding




	appendLog("    C(OTP) : ");	B2Str(mInterParam.Cotp, 16);
	appendLog("    SIGN_R(pp) : ");	B2Str(mInterParam.SRpp, 32);
	appendLog("    SIGN_S(pp) : ");	B2Str(mInterParam.SSpp, 32);

	
	updateSignHash(mInterParam.ID, CERT_IDSIZE);
	getHashValue4Sign(msg_Vc);

	appendLog("    Hasg(msg||ID) : ");	B2Str(msg_Vc, 32);

	sign_Vc = ECDSA_SIG_new();
#if 0
	ByteArraytoString(R_pp, Rc, 32);
	ByteArraytoString(S_pp, Sc, 32);
	BN_hex2bn(&sign_Vc->r, Rc);
	BN_hex2bn(&sign_Vc->s, Sc);
#endif

	sign_Vc->r = BN_bin2bn(mInterParam.SRpp, DEFECCSIZE_SIGN, NULL);
	sign_Vc->s = BN_bin2bn(mInterParam.SSpp, DEFECCSIZE_SIGN, NULL);


	if (1 != ECDSA_do_verify(msg_Vc, 32, sign_Vc, G_ECCkey_CHIP))
	{
		errlog = "  ECC_sign verify error";
		return  false ;
	}


	////2. D_ECB:k1(C_otp) == GenerateUserOTP()

	D_ECB_K1(mInterParam.Cotp, P_otp, mInterParam.SEL_AES);


	if (memcmp(mInterParam.OTP, P_otp, 3) != 0)
	{
		errlog = "  OTP compare error\n";
//		return false;
	}
	appendLog("    OTP : ");	B2Str(P_otp, 6);
	SHA256_CTX ck;
	////3. HK`_tsm = hash(mInterParam.K1||mInterParam.K2||mInterParam.IV)
	SHA256_Init(&ck);
	SHA256_Update(&ck, mInterParam.K1, 16);
	SHA256_Update(&ck, mInterParam.K2, 16);
	SHA256_Update(&ck, mInterParam.IV, 16);
	SHA256_Final(mInterParam.HK_tsm, &ck);
	appendLog("    HK`(TSM) : ");	B2Str(mInterParam.HK_tsm, 32);

	////4. store(HK`_tsm)
	//3번에서 처리


	ECDSA_SIG_free(sign_Vc);


	//로그
	
	appendLog("  %02X(ERR_SIGN), %02X\n",CMD_ERR_SIGN, 0);


	return true;
}