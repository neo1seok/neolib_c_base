#include "main.h"
#include <string.h>

//int generateSNECC(BYTE *pbOut, DWORD *pdwLenOut, BYTE *pbLog)추가
//seed 암호화 부분 추가
//int  SelAesSeedECC(int sel_aes) 추가
//int E_ECB_K1ECC(BYTE* in, BYTE* out, BYTE sel_aes)
//int D_ECB_K1ECC(BYTE* in, BYTE* out, BYTE sel_aes) 추가
// openssl/evp.h 추가


//글로벌 변수
EC_KEY		*G_ECCkey_TSM;		
EC_KEY		*G_ECCkey_CHIP;		//개인키는 모름
EC_KEY		*G_ECCkey_A;		//개인키는 모름
EC_KEY		*G_ECCkey_B;	
EC_GROUP	*G_ECCgroup;
EC_POINT	*G_puk_A;


BYTE SN[16];
BYTE G_ID[16];
BYTE G_PIN[16];
BYTE G_INFORM[16];
BYTE RN[16];

BYTE K1[16];
BYTE K2[16];
BYTE IV[16];
BYTE SEL_AES;

BYTE EHK_device[32];
BYTE HK_device[32];
BYTE HK_tsm[32];
BYTE HK_chip[64+1];

BYTE G_MSG[65600];
int g_msgPos;

#define ECC_XOR 0


BYTE ecx_G[32] = {
	0x6B, 0x17, 0xD1, 0xF2, 0xE1, 0x2C, 0x42, 0x47, 
	0xF8, 0xBC, 0xE6, 0xE5, 0x63, 0xA4, 0x40, 0xF2,
	0x77, 0x03, 0x7D, 0x81, 0x2D, 0xEB, 0x33, 0xA0,
	0xF4, 0xA1, 0x39, 0x45, 0xD8, 0x98, 0xC2, 0x96 
};
BYTE ecx_d[32] = {
	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 
	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 
	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 
	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 
};

BYTE ecx_K[32] = {
	0xF1, 0x34, 0x1f, 0xAB, 0xCD, 0xEC, 0xA1, 0x89, 
	0xF2, 0x35, 0x2f, 0xAC, 0xCD, 0xBC, 0xA2, 0xA9, 
	0xF3, 0x36, 0x3f, 0xAD, 0xFD, 0xAC, 0xA3, 0x89, 
	0xF4, 0x37, 0x4f, 0xAE, 0xED, 0xEC, 0xA1, 0xD9, 
};
BYTE ecx_Q[33];
BYTE ecx_Qchip[33];

BYTE ecx_bd[32];
BYTE ecx_BQ[33];
BYTE ecx_AQ[33];
BYTE G_Q_CHIP[33];
BYTE G_A_CHIP[33];

int ECX_genkey(BYTE* d, BYTE* G, BYTE* Q);


	

//////////////////////////// 초기화


int ECC_keyinit(int gen)
{
	G_ECCgroup = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);		//그룹할당

	G_ECCkey_CHIP	= EC_KEY_new();					//키할당
	G_ECCkey_A		= EC_KEY_new();	
	G_ECCkey_B		= EC_KEY_new();	
    G_puk_A         = EC_POINT_new(G_ECCgroup);
	EC_KEY_set_group(G_ECCkey_CHIP	,G_ECCgroup);	//그룹설정
	EC_KEY_set_group(G_ECCkey_A		,G_ECCgroup);	
	EC_KEY_set_group(G_ECCkey_B		,G_ECCgroup);	

	if(gen)	
        ECC_KeyGen_TSM();
	else	
        ECC_Keyinit_TSM();

//    int field_size = EC_GROUP_get_degree(EC_KEY_get0_group(G_ECCkey_CHIP));
	

#if ECC_XOR
	ECX_genkey(ecx_d,ecx_G,ecx_Q);
#endif

	return 1;
}
int ECC_keyfree()
{
	EC_KEY_free(G_ECCkey_TSM);
	EC_KEY_free(G_ECCkey_CHIP);
	EC_KEY_free(G_ECCkey_A);
	EC_KEY_free(G_ECCkey_B);
	EC_POINT_free(G_puk_A);
	return 1;
}

int ECC_KeyGen_TSM()
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
int ECC_Keyinit_TSM()
{	
    EC_POINT *puk_t;		//공개키
    BIGNUM *prv_t;			//개인키
	G_ECCkey_TSM = EC_KEY_new();						//키 할당
	EC_KEY_set_group(G_ECCkey_TSM,G_ECCgroup);			//그룹설정

	BN_CTX *CTX;
	CTX=BN_CTX_new();

	char Q[33*2+2];
	char d[32*2+2];

	//파일에서 읽어옴
	FILE *fp;
	fp = fopen("TSM_ECC_KEY.txt","r");
	//fopen_s(&fp,"TSM_ECC_KEY.txt","r");
	fscanf(fp,"%s\n%s\n",Q,d);
	fclose(fp);

	//키값을 키에 저장
	puk_t = EC_POINT_new(G_ECCgroup);
	puk_t = EC_POINT_hex2point(G_ECCgroup, Q, puk_t, CTX);
	EC_KEY_set_public_key(G_ECCkey_TSM, puk_t);	//puk key setting
	//EC_POINT_free(puk_t);

	prv_t = BN_new();
	BN_hex2bn(&prv_t,d);
	EC_KEY_set_private_key(G_ECCkey_TSM, prv_t); //prv key setting
	//BN_free(prv_t);


	return 1;
}
/////////////////////////////  제조


//////////////////////////// 발급

int requestIdECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog, int iExcep)
{
	//ECC 기반 Chip에서 전송된 REQ_ID에서 SN이 list에 있는 것인지 확인하고 RES_ID packet을 생성한다.
	//선언
	BYTE SDATA[32];
	BYTE Q_TSM[33];
	DWORD CRC;

	BN_CTX *CTX;
	CTX=BN_CTX_new();
	const EC_POINT	*ec_puk;

	//프로세싱
	ec_puk = EC_KEY_get0_public_key(G_ECCkey_TSM); 		// 공개키 획득
	StringtoByteArray(EC_POINT_point2hex(G_ECCgroup,ec_puk,POINT_CONVERSION_COMPRESSED,CTX),Q_TSM,33*2);	//공개키를 BYTE로 변형

    if (iExcep == EXCEP_TSM_Q)
    {
        EC_POINT	*puk_A = NULL;
	    BYTE Qc[70];
	    ByteArraytoString(Q_TSM, (char*)Qc, 33);
	    puk_A = EC_POINT_hex2point(G_ECCgroup, (const char*)Qc, puk_A, CTX);

        if (puk_A != NULL)
		    pbLog += sprintf((char*)pbLog, "  original Valid Key\n");

        puk_A = NULL;
        Q_TSM[32]++;
	    ByteArraytoString(Q_TSM, (char*)Qc, 33);
	    puk_A = EC_POINT_hex2point(G_ECCgroup, (const char*)Qc, puk_A, CTX);
	    ////키확인
	    if(puk_A == NULL)	// 키 체크	
	    {	
		    pbLog += sprintf((char*)pbLog, "  Not Valid Key TEST!\n");
	    }
        else
        {
		    pbLog += sprintf((char*)pbLog, "  Valid Key = TEST value ERROR!\n");
        }
    }

	Gen_SDATA(SDATA,m_certext.cert.ID,G_PIN,G_INFORM);

	//머징
	*pdwLenOut = 1+16+32+33+2;
	pbOut[0] = CMD_REQ_PUK_ECC;
	memcpy(pbOut+1			,m_certext.cert.ID	,16);
	memcpy(pbOut+1+16		,SDATA	,32);
	memcpy(pbOut+1+16+32  	,Q_TSM	,33);
		
	//CRC = calcCRC(pbOut, 1+16+32+1+33);
	CRC = calcCRC(pbOut+1, 16+32+33);
    if (iExcep == EXCEP_CRC)
        CRC++;

	pbOut[1+16+32+33  ] = (BYTE)(CRC >> 8);
	pbOut[1+16+32+33+1] = (BYTE)(CRC);
	//로그
	pbLog += sprintf((char*)pbLog, "  out data : ");
	pbLog = B2Str(pbOut	,*pdwLenOut	,pbLog);

	pbLog += sprintf((char*)pbLog, "    REQ_PUK_ECC : %02X\n    ID : ", CMD_REQ_PUK_ECC);
	pbLog = B2Str(m_certext.cert.ID	,16		,pbLog);

	pbLog += sprintf((char*)pbLog, "    SDATA : ");
	pbLog = B2Str(SDATA	,32		,pbLog);

	pbLog += sprintf((char*)pbLog, "    Q(TSM) : ");
	pbLog = B2Str(Q_TSM	,33		,pbLog);

    pbLog += sprintf((char*)pbLog, "    CRC : %04X\n", CRC);

	BN_CTX_free(CTX);
	return 1;
}

int GenCertECC(Cert_ECC* cert, BYTE* ID, BYTE* Q_chip, EC_KEY* key_tsm)
{

	BYTE msg[32];

	SHA256_CTX c;
	SHA256_Init(&c);
	SHA256_Update(&c,ID,16);
	SHA256_Update(&c,Q_chip,33);
	SHA256_Final(msg,&c);




	ECDSA_SIG *sign;
	sign = ECDSA_SIG_new();
	sign = ECDSA_do_sign((const unsigned char*)msg,32,key_tsm);	

	StringtoByteArray(BN_bn2hex(sign->r),cert->R,32*2);
	StringtoByteArray(BN_bn2hex(sign->s),cert->S,32*2);


	memcpy(cert->ID,m_certext.cert.ID,16);
	memcpy(cert->Q,Q_chip,33);
	ECDSA_SIG_free(sign);



	return 1;
}


int requestCertECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog)
{
	//ECC 기반 Chip에서 전송된 REQ_CERT를 확인하고 RES_CERT packet을 생성한다.
	//선언

	BYTE	*Q_chip;		//공개키 바이트
	char	Qc[33*2+2];		//공개키 캐릭터
	EC_POINT *puk_t;		//공개키 포인트

	DWORD CRC;

	BN_CTX *CTX;


	//파싱
	pbLog += sprintf((char*)pbLog, "  input data : ");
	pbLog = B2Str(pbIn	,iLenIn		,pbLog);

	if(pbIn[0] != CMD_RES_PUK_ECC)
    {
        pbLog += sprintf((char*)pbLog, "  not RES_PUK_ECC : %02X\n", pbIn[0]);
        return -1;
    }
	if (iLenIn != 1+33+2) 
    {
        pbLog += sprintf((char*)pbLog, "  length error\n");
        return -1;
    }
	pbLog += sprintf((char*)pbLog, "    RES_PUK_ECC : %02X\n  Q(Chip) : ", pbIn[0]);
	pbLog = B2Str(pbIn + 1	,33		,pbLog);

	CTX=BN_CTX_new();
	Q_chip = pbIn+1;
	ByteArraytoString(Q_chip, Qc, 33);
	puk_t = EC_POINT_new(G_ECCgroup);
	puk_t = EC_POINT_hex2point(G_ECCgroup, Qc, puk_t, CTX);
	EC_KEY_set_public_key(G_ECCkey_CHIP, puk_t);	//puk key setting

    memcpy(G_Q_CHIP, Q_chip, 33);

	//crc check
	//CRC = calcCRC(pbIn, iLenIn - 2);
	CRC = calcCRC(pbIn+1, iLenIn - 2 - 1);

	BN_CTX_free(CTX);
	EC_POINT_free(puk_t);

    if (CRC != ((pbIn[iLenIn - 2] << 8) + pbIn[iLenIn - 1]))
    {
        pbLog += sprintf((char*)pbLog, "  CRC error(calculated CRC : %04X)\n", CRC);
		//에러 신호
		*pdwLenOut =2;
		pbOut[0] = CMD_ERR_CERT_ECC;
		pbOut[1] = EC_ER_CRC;

        return -1;
    }

	return 1;
}

int requestRegCertECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog, int iExcep)
{
	//ECC 기반 Chip에서 전송된 REQ_CERT를 확인하고 RES_CERT packet을 생성한다.
	//선언

	Cert_ECC cert;
	DWORD CRC;

    if (iExcep == EXCEP_CERT_Q_CHIP)
    {
        BYTE FakeQ_Chip[33];
        memcpy(FakeQ_Chip, G_Q_CHIP, 33);
        FakeQ_Chip[8]++;
        GenCertECC(&cert, m_certext.cert.ID, FakeQ_Chip, G_ECCkey_TSM);	//cert 생성
    }
    else
    {
	    //데이터 프로세싱
        GenCertECC(&cert, (iExcep == EXCEP_CERT_ID) ? (BYTE*)"1111222233334444" : m_certext.cert.ID, 
            G_Q_CHIP, G_ECCkey_TSM);
    }

    if (iExcep == EXCEP_CERT_SIGN)
        cert.R[7]++;

	//머징
	//RES_CERT(1) || CERT(113) || CRC(2)
	*pdwLenOut = 1+113+2;
	pbOut[0] = CMD_REQ_REG_CERT_ECC;
	memcpy(pbOut+1		,&cert	,113);

    CRC = calcCRC(pbOut+1, 113);
	pbOut[1+113  ] = (BYTE)(CRC >> 8);
	pbOut[1+113+1] = (BYTE)(CRC		);
    if (iExcep == EXCEP_CRC)
        pbOut[1+113+1]++;


	//로그
	pbLog += sprintf((char*)pbLog, "  out data : ");
	pbLog = B2Str(pbOut	,*pdwLenOut	,pbLog);

	pbLog += sprintf((char*)pbLog, "    REQ_REG_CERT_ECC : %02X\n    ID : ", CMD_REQ_REG_CERT_ECC);
	pbLog = B2Str(cert.ID	,16		,pbLog);

	pbLog += sprintf((char*)pbLog, "    Q(Chip) : ");
	pbLog = B2Str(cert.Q	,33		,pbLog);

	pbLog += sprintf((char*)pbLog, "    Sign_R  : ");
	pbLog = B2Str(cert.R	,32		,pbLog);

	pbLog += sprintf((char*)pbLog, "    Sign_S  : ");
	pbLog = B2Str(cert.S	,32		,pbLog);

	pbLog += sprintf((char*)pbLog, "    CRC : %04X\n", CRC);

	return 1;
}

//////////////////////////// 사용

int requestRNECC(BYTE *pbOut, DWORD *pdwLenOut, BYTE *pbLog, int iExcep)
{
	BYTE SDATA[32];
	//선언
	//프로세싱
		
	//머징
	//pbOut = REQ_AUTH(1) || SDATA(32) || EHK_device(32) 
	*pdwLenOut = 1+32+32;
	pbOut[0] = CMD_REQ_AUTH_ECC;
	Gen_SDATA(SDATA,m_certext.cert.ID,G_PIN,G_INFORM);
    if (iExcep == EXCEP_SDATA)
        SDATA[5]++;

	memcpy(pbOut+1		,SDATA,32);
	memcpy(pbOut+1+32	,EHK_device,32);

    if (iExcep == EXCEP_EHK)
        pbOut[1+32]++;

	//로그
    pbLog += sprintf((char*)pbLog, "  out data : ");
	pbLog = B2Str(pbOut		,*pdwLenOut	,pbLog);

	pbLog += sprintf((char*)pbLog, "    REQ_AUTH_ECC %02X\n  SDATA  : ", CMD_REQ_AUTH_ECC);
	pbLog = B2Str(SDATA		,32		,pbLog);

	pbLog += sprintf((char*)pbLog, "    EHK(device)  : ");
	pbLog = B2Str(EHK_device,32		,pbLog);

	return 1;
}


int requestSessionECC(BYTE* pbIn, int iLenIn, BYTE *pbLog)
{
	//ECC 기반 Chip에서 전송된 REQ_SESSION를 확인하고 REQ_HS1 packet을 생성한다.
	//선언
	BYTE *ID;
	BYTE *A;
	BYTE *R;
	BYTE *S;

	BN_CTX *CTX;

	////sign
//	BYTE msg_Vc[32];
//	char Rc[32*2+2];
//	char Sc[32*2+2];
//	char Ac[32*2+2];
	BYTE msg_Vc[32];
	char Rc[32*2+2];
	char Sc[32*2+2];
	char Ac[33*2+2];


	SHA256_CTX c_Vc;
	ECDSA_SIG *sign_Vc;


	CTX=BN_CTX_new();

	//파싱
	pbLog += sprintf((char*)pbLog, "  input data : ");
	pbLog = B2Str(pbIn	,iLenIn		,pbLog);

	if(pbIn[0] != CMD_REQ_SESSION_ECC)
	{
		pbLog += sprintf((char*)pbLog, "  not REQ_SESSION_ECC : %02X\n", pbIn[0]);
		return -1;
	}

	if (iLenIn != (1 + 16 + 33 + 32 + 32)) 
	{
		pbLog += sprintf((char*)pbLog, "  length error\n");
		return -1;
	}
    pbLog += sprintf((char*)pbLog, "    REQ_SESSION_ECC : %02X\n", pbIn[0]);

	ID = pbIn+1;
	A  = pbIn+1+16;
	R  = pbIn+1+16+33;
	S  = pbIn+1+16+33+32;

	pbLog += sprintf((char*)pbLog, "    ID : ");	pbLog = B2Str(ID,16		,pbLog);
	pbLog += sprintf((char*)pbLog, "    A : ");	pbLog = B2Str(A	,33		,pbLog);
	pbLog += sprintf((char*)pbLog, "    R : ");	pbLog = B2Str(R	,32		,pbLog);
	pbLog += sprintf((char*)pbLog, "    S : ");	pbLog = B2Str(S	,32		,pbLog);

	SHA256_Init(&c_Vc);
	SHA256_Update(&c_Vc,ID,16);
	SHA256_Update(&c_Vc,A,33);
	SHA256_Final(msg_Vc,&c_Vc);

	sign_Vc = ECDSA_SIG_new();
	ByteArraytoString(R,Rc,32);
	ByteArraytoString(S,Sc,32);
	BN_hex2bn(&sign_Vc->r,Rc);
	BN_hex2bn(&sign_Vc->s,Sc);

	////EC_KEY A	
	ByteArraytoString(A,Ac,33);
    memcpy(G_A_CHIP, A, 33);

	G_puk_A = EC_POINT_hex2point(G_ECCgroup, Ac, G_puk_A, CTX);
	EC_KEY_set_public_key(G_ECCkey_A,G_puk_A);	//puk key setting


	//프로세싱
	////서명검증

	if(1 != ECDSA_do_verify(msg_Vc,32,sign_Vc,G_ECCkey_CHIP))
	{
		pbLog += sprintf((char*)pbLog, "  sign error\n");
		return -1;
	}
	////키확인
	if(1 != EC_KEY_check_key(G_ECCkey_A))	// 키 체크	
	{	
		pbLog += sprintf((char*)pbLog, "  A public key error\n");
		return -1;
	}

	BN_CTX_free(CTX);
	ECDSA_SIG_free(sign_Vc);
	return 1;
}

#if 0
//for test old
int requestSessionECCold(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog)
{
	//ECC 기반 Chip에서 전송된 REQ_SESSION를 확인하고 REQ_HS1 packet을 생성한다.
	//선언
	BYTE *ID;
	BYTE *A;
	BYTE *R;
	BYTE *S;

	BN_CTX *CTX;
	CTX=BN_CTX_new();

	//머징
	*pdwLenOut =2;
	pbOut[0] = CMD_REQ_HS1_ECC;
	pbOut[1] = EC_NO_ERR;


	//파싱
	pbLog += sprintf((char*)pbLog, "input data : ");
	pbLog = B2Str(pbIn	,iLenIn		,pbLog);

	if(pbIn[0] != CMD_REQ_SESSION_ECC)
	{
		pbLog += sprintf((char*)pbLog, "not REQ_SESSION_ECC : %02X\n", pbOut[0]);
		return -1;
	}

	if (iLenIn != (1 + 16 + 33 + 32 + 32)) 
	{
		pbLog += sprintf((char*)pbLog, "length error\n");
		return -1;
	}
    pbLog += sprintf((char*)pbLog, "  REQ_SESSION_ECC : %02X\n", pbIn[0]);

	ID = pbIn+1;
	A  = pbIn+1+16;
	R  = pbIn+1+16+33;
	S  = pbIn+1+16+33+32;

	pbLog += sprintf((char*)pbLog, "  ID : ");	pbLog = B2Str(ID,16		,pbLog);
	pbLog += sprintf((char*)pbLog, "  A : ");	pbLog = B2Str(A	,33		,pbLog);
	pbLog += sprintf((char*)pbLog, "  R : ");	pbLog = B2Str(R	,32		,pbLog);
	pbLog += sprintf((char*)pbLog, "  S : ");	pbLog = B2Str(S	,32		,pbLog);

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
	ByteArraytoString(R,Rc,32);
	ByteArraytoString(S,Sc,32);
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
		pbLog += sprintf((char*)pbLog, "sign error\n");
		pbOut[0] = CMD_ERR_SESSION_ECC;
		pbOut[1] = EC_ER_ECC_SIGN;
		return -1;
	}
	////키확인
	if(1 != EC_KEY_check_key(G_ECCkey_A))	// 키 체크	
	{	
		pbLog += sprintf((char*)pbLog, "A public key error\n");
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


	memcpy(K1,sharedkey1		,16);
	memcpy(K2,sharedkey1+16	,16);
	memcpy(IV,sharedkey2		,16);
	
	pbLog += sprintf((char*)pbLog, "key data\n ");
	pbLog += sprintf((char*)pbLog, "  K1 : ");	pbLog = B2Str(K1	 ,16	,pbLog);
	pbLog += sprintf((char*)pbLog, "  K2 : ");	pbLog = B2Str(K2	 ,16	,pbLog);
	pbLog += sprintf((char*)pbLog, "  IV : ");	pbLog = B2Str(IV	 ,16	,pbLog);
	pbLog += sprintf((char*)pbLog, "\n");

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
	E_ECB_K1(A+1+16, C_ECB, SEL_AES);	//+++
	
	//머징
	////REQ_HS1(1) || B(33) || R`(32) || S`(32) || C_ECB(16);
	*pdwLenOut = 1+33+32+32+16;
	pbOut[0] = CMD_REQ_HS1_ECC;
	memcpy(pbOut+1			,B,33);
	memcpy(pbOut+1+33		,R_prime,32);
	memcpy(pbOut+1+33+32	,S_prime,32);
	memcpy(pbOut+1+33+32+32	,C_ECB,16);

	//로그
	pbLog += sprintf((char*)pbLog, "out data : "); 	pbLog = B2Str(pbOut	,*pdwLenOut	,pbLog);
	pbLog += sprintf((char*)pbLog, "  REQ_HS1 : %02X\n", CMD_REQ_HS1_ECC);
    pbLog += sprintf((char*)pbLog, "  sha : ");	pbLog = B2Str(msg			    ,32	,pbLog);
    pbLog += sprintf((char*)pbLog, "  B : ");	pbLog = B2Str(pbOut+1			,33	,pbLog);
	pbLog += sprintf((char*)pbLog, "  R`: ");	pbLog = B2Str(pbOut+1+33		,32	,pbLog);
	pbLog += sprintf((char*)pbLog, "  S`: ");	pbLog = B2Str(pbOut+1+33+32		,32	,pbLog);
	pbLog += sprintf((char*)pbLog, "  C1 : ");	pbLog = B2Str(pbOut+1+33+32+32	,16	,pbLog);
	
	BN_CTX_free(CTX);
	ECDSA_SIG_free(sign_Vc);
	ECDSA_SIG_free(sign);
	EC_POINT_free(ECDH_P);
	EC_POINT_free(puk_A);
	return 1;
}
#endif

int requestHS1ECC(BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog, int iExcep)
{
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


	memcpy(K1,sharedkey1,16);
	memcpy(K2,sharedkey1+16	,16);
	memcpy(IV,sharedkey2,16);
	
	pbLog += sprintf((char*)pbLog, "  key data\n");
	pbLog += sprintf((char*)pbLog, "    K1 : ");	pbLog = B2Str(K1	 ,16	,pbLog);
	pbLog += sprintf((char*)pbLog, "    K2 : ");	pbLog = B2Str(K2	 ,16	,pbLog);
	pbLog += sprintf((char*)pbLog, "    IV : ");	pbLog = B2Str(IV	 ,16	,pbLog);
	pbLog += sprintf((char*)pbLog, "\n");

	////서명생성
	SHA256_Init(&c);
	
	SHA256_Update(&c,m_certext.cert.ID,16);
	SHA256_Update(&c,G_A_CHIP,33);
	SHA256_Update(&c,B,33);
#if 0
	SHA256_Update(&c,G_A_CHIP,33);
	SHA256_Update(&c,B,33);
	SHA256_Update(&c,m_certext.cert.ID,16);
#endif
	SHA256_Final(msg,&c);


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
	E_ECB_K1(G_A_CHIP+1+16, C_ECB, SEL_AES);	//+++
	
    if (iExcep == EXCEP_C1)
        C_ECB[4]++;

    if (iExcep == EXCEP_ERR_SIGN)
        R_prime[4]++;

	//머징
	////REQ_HS1(1) || B(33) || R`(32) || S`(32) || C_ECB(16);
	*pdwLenOut = 1+33+32+32+16;
	pbOut[0] = CMD_REQ_HS1_ECC;
	memcpy(pbOut+1			,B,33);
	memcpy(pbOut+1+33		,R_prime,32);
	memcpy(pbOut+1+33+32	,S_prime,32);
	memcpy(pbOut+1+33+32+32	,C_ECB,16);

	//로그
	pbLog += sprintf((char*)pbLog, "  out data : "); 	pbLog = B2Str(pbOut	,*pdwLenOut	,pbLog);
	pbLog += sprintf((char*)pbLog, "    REQ_HS1 : %02X\n", CMD_REQ_HS1_ECC);
    pbLog += sprintf((char*)pbLog, "    sha : ");	pbLog = B2Str(msg			    ,32	,pbLog);
    pbLog += sprintf((char*)pbLog, "    B : ");	pbLog = B2Str(pbOut+1			,33	,pbLog);
	pbLog += sprintf((char*)pbLog, "    R`: ");	pbLog = B2Str(pbOut+1+33		,32	,pbLog);
	pbLog += sprintf((char*)pbLog, "    S`: ");	pbLog = B2Str(pbOut+1+33+32		,32	,pbLog);
	pbLog += sprintf((char*)pbLog, "    C1 : ");	pbLog = B2Str(pbOut+1+33+32+32	,16	,pbLog);
	
	BN_CTX_free(CTX);
	ECDSA_SIG_free(sign);
	EC_POINT_free(ECDH_P);
	return 1;
}



int requestHS2ECC(BYTE* pbIn, int iLenIn, BYTE* pbLog)
{
	//ECC 기반 Chip에서 전송된 REQ_HS2를 확인하고 response packet을 생성한다.
	//선언
	g_msgPos = 0;

	BYTE B[33];
	BYTE Bx[16];
	BYTE *C2;		//16 byte
	BYTE *C3;		//32 byte
	BN_CTX *CTX;
	CTX = BN_CTX_new();

	//파싱
	pbLog += sprintf((char*)pbLog, "  input data : ");
	pbLog = B2Str(pbIn	,iLenIn		,pbLog);

	if(pbIn[0] != CMD_REQ_HS2_ECC)
    {
        pbLog += sprintf((char*)pbLog, "  not CMD_REQ_HS2_ECC : %02X\n", pbIn[0]);
        return -1;
    }

    if (iLenIn != (1 + 16 + 32)) 
    {
        pbLog += sprintf((char*)pbLog, "  length error\n");
        return -1;
    }
    pbLog += sprintf((char*)pbLog, "    REQ_HS2_ECC : %02X\n", pbIn[0]);

	C2 = pbIn+1;
	C3 = pbIn+1+16;
	pbLog += sprintf((char*)pbLog, "    C2 : ");	pbLog = B2Str(C2,16	,pbLog);
	pbLog += sprintf((char*)pbLog, "    C3 : ");	pbLog = B2Str(C3,32	,pbLog);
	


	EC_POINT_point2oct(G_ECCgroup, EC_KEY_get0_public_key(G_ECCkey_B), POINT_CONVERSION_COMPRESSED, B, 33, CTX);

	//프로세싱
	////C2 복호화
	
	D_ECB_K1(C2, Bx, SEL_AES);
	
	////B를 비교		
	
	if(0 != memcmp(Bx,B+1+16,16))
	{
		pbLog += sprintf((char*)pbLog, "  Bx compare error\n");
		return -1;
	}
	
	pbLog += sprintf((char*)pbLog, "    Bx : ");   pbLog = B2Str(Bx	,16	,pbLog);

	////C3 복호화

	D_ECB_K1(C3,	HK_device	,SEL_AES);
	D_ECB_K1(C3+16,	HK_device+16,SEL_AES);

	////HK 비교
	//처음은 비교 안함

	if(0 != memcmp(HK_device, HK_tsm,32))
	{
		pbLog += sprintf((char*)pbLog, "  HK compare error\n");
		//return -1;
	}
	pbLog += sprintf((char*)pbLog, "    HK : ");   pbLog = B2Str(HK_device	,32	,pbLog);

	BN_CTX_free(CTX);
	return 1;
}




int storeEHKdeviceECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog)
{
	//파싱
	pbLog += sprintf((char*)pbLog, "  input data : ");
	pbLog = B2Str(pbIn	,iLenIn		,pbLog);

	if(pbIn[0] != CMD_RES_SIGN_ECC)
	{
		pbLog += sprintf((char*)pbLog, "  not CMD_RES_SIGN_ECC : %02X\n", pbOut[0]);
		return -1;
	}
	if (iLenIn != 1+32) 
	{
		pbLog += sprintf((char*)pbLog, "  length error\n");
		return -1;
	}
    pbLog += sprintf((char*)pbLog, "    RES_SIGN : %02X\n", pbIn[0]);

    //저장
	memcpy(EHK_device,pbIn+1,32);
	//로그
	pbLog += sprintf((char*)pbLog, "    EHK`(device) : ");
	pbLog = B2Str(EHK_device	,32		,pbLog);
	return 1;
}

int requestSignECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog, BYTE* pbOTP, int iExcep)
{
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

	//파싱	cmd(1) ||  C_opt(16) || R``(32) || S``(32)
	pbLog += sprintf((char*)pbLog, "  input data : ");
	pbLog = B2Str(pbIn	,iLenIn		,pbLog);
	if(pbIn[0] != CMD_RES_SIGN_ECC)
	{
		pbLog += sprintf((char*)pbLog, "  not RES_SIGN_ECC : %02X\n", pbOut[0]);
		return -1;
	}
	if (iLenIn != 1+16+32+32) 
	{
		pbLog += sprintf((char*)pbLog, "  length error\n");
		return -1;
	}
    pbLog += sprintf((char*)pbLog, "    RES_SIGN_ECC : %02X\n", pbIn[0]);

    C_otp	= pbIn + 1;
	R_pp	= pbIn + 1 + 16;
	S_pp	= pbIn + 1 + 16 + 32;
	
	pbLog += sprintf((char*)pbLog, "    C(OTP) : ");	pbLog = B2Str(C_otp	,16	,pbLog);
	pbLog += sprintf((char*)pbLog, "    R(pp) : ");	pbLog = B2Str(R_pp	,32	,pbLog);
	pbLog += sprintf((char*)pbLog, "    S(pp) : ");	pbLog = B2Str(S_pp	,32	,pbLog);

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
	if(g_msgPos)	SHA256_Update(&c,G_MSG,g_msgPos);
	SHA256_Update(&c, m_certext.cert.ID, 16);
	//SHA256_Update(&c, C_otp, 16);
	SHA256_Final(msg_Vc,&c);
	
	

	pbLog += sprintf((char*)pbLog, "    Hasg(msg||ID) : ");	pbLog = B2Str(msg_Vc	,32	,pbLog);

	sign_Vc = ECDSA_SIG_new();

	ByteArraytoString(R_pp,Rc,32);
	ByteArraytoString(S_pp,Sc,32);
	BN_hex2bn(&sign_Vc->r,Rc);
	BN_hex2bn(&sign_Vc->s,Sc);

	if(1 != ECDSA_do_verify(msg_Vc,32,sign_Vc,G_ECCkey_CHIP))
	{
		pbLog += sprintf((char*)pbLog, "  ECC_sign verify error\n");
		pbOut[1] = EC_ER_ECC_SIGN;
		return -2;
	}
	

	////2. D_ECB:k1(C_otp) == GenerateUserOTP()
	
	D_ECB_K1(C_otp,		P_otp,	SEL_AES);


	if (memcmp(pbOTP, P_otp, 3) != 0)
	{		
		pbLog += sprintf((char*)pbLog, "  OTP compare error\n");
		pbOut[1] = EC_ER_OTP;
		return -1;
	}  
	pbLog += sprintf((char*)pbLog, "    OTP : ");	pbLog = B2Str(P_otp		,6		,pbLog);
	
	////3. HK`_tsm = hash(K1||K2||IV)
	SHA256_Init(&ck);
	SHA256_Update(&ck,K1,16);
	SHA256_Update(&ck,K2,16);
	SHA256_Update(&ck,IV,16);
	SHA256_Final(HK_tsm,&ck);
	pbLog += sprintf((char*)pbLog, "    HK`(TSM) : ");	pbLog = B2Str(HK_tsm, 32		,pbLog);

	////4. store(HK`_tsm)
	//3번에서 처리

	
	ECDSA_SIG_free(sign_Vc);

	pbOut[0] = CMD_ERR_SIGN;
    switch (iExcep)
    {
    case EXCEP_ERR_SIGN_OTP:
        pbOut[1] = EC_ER_OTP;
        pbLog += sprintf((char*)pbLog, "  invalid ERR_SIGN || ER_OTP\n");
        break;
    case EXCEP_ERR_PNT:
        pbOut[1] = EC_ER_PNT;
        pbLog += sprintf((char*)pbLog, "  invalid ERR_SIGN || ER_PNT\n");
        break;
    case EXCEP_ERR_SIGN_SIGN:
        pbOut[1] = EC_ER_ECC_SIGN;
        pbLog += sprintf((char*)pbLog, "  invalid ERR_SIGN || ER_SIGN\n");
        break;
    default:
    	pbOut[1] = EC_NO_ERR;
    }

	//로그
	pbLog += sprintf((char*)pbLog, "out data : ");	pbLog = B2Str(pbOut	,*pdwLenOut	,pbLog);
    pbLog += sprintf((char*)pbLog, "  %02X(ERR_SIGN), %02X\n", 
        CMD_ERR_SIGN, pbOut[1]);


	return 1;
}

int requestEHKdevice(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog)
{
	//파싱
	pbLog += sprintf((char*)pbLog, "  input data : ");
	pbLog = B2Str(pbIn	,iLenIn		,pbLog);
	
	if(pbIn[0] != CMD_RES_EHK)
    {
        pbLog += sprintf((char*)pbLog, "  not RES_EHK : %02X\n", pbIn[0]);
        return -1;
    }
    if (iLenIn != 1+32) 
    {
        pbLog += sprintf((char*)pbLog, "  length error\n");
        return -1;
    }
	//저장
	memcpy(EHK_device, pbIn+1, 32);
	//로그
	pbLog += sprintf((char*)pbLog, "    EHK`(device) : ");
	pbLog = B2Str(EHK_device	,32		,pbLog);

	return 1;
}


