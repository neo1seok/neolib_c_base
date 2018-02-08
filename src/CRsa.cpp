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
#include "neoDebug.h"
#include "CRsa.h"
#include"commfunc.h"

static const unsigned char ID_SHA256[] = {
	0x30, 0x31, 0x30, 0x0d, 0x06,
	0x09, 0x60, 0x86, 0x48, 0x01,
	0x65, 0x03, 0x04, 0x02, 0x01,
	0x05, 0x00, 0x04, 0x20
};




#define PRINTF 0
#define RSA_XOR 0

CRsa::CRsa(void)
{

	

}

CRsa::~CRsa(void)
{
}

//////////////////////////////////////// 초기화
int CRsa::keyinit(int gen){
	//return 1;//test0326
	CBaseCrypt::keyinit(0);
	BYTE _G_N_chip[] =
#ifdef REAL_CHIP
	{
		0x9C, 0x63, 0xC3, 0x2B, 0x03, 0xB6, 0xBE, 0x58, 0xB8, 0x25, 0xE4, 0xD5, 0xF9, 0x6A, 0xED, 0x08,
		0x30, 0x37, 0x13, 0x01, 0xC6, 0xEF, 0x5E, 0xB8, 0x15, 0x35, 0x2E, 0x2D, 0xEE, 0xB9, 0xEA, 0x3C,
		0xB8, 0xEE, 0x9C, 0xC5, 0x83, 0x22, 0x39, 0xD9, 0x43, 0x30, 0x1B, 0x56, 0x7A, 0xA4, 0xA0, 0xE6,
		0xE1, 0x0C, 0xDB, 0x69, 0xAB, 0x09, 0xE5, 0x0F, 0xC7, 0x1C, 0xE0, 0x38, 0xAA, 0x51, 0x85, 0x6A,
		0x90, 0xEE, 0x4F, 0x08, 0x0E, 0x8B, 0x0B, 0x42, 0xE9, 0x97, 0x9B, 0x18, 0x00, 0x61, 0x26, 0xB9,
		0xB9, 0x5D, 0x8A, 0x6F, 0xB3, 0x5E, 0xCB, 0xC1, 0x2D, 0x77, 0x31, 0xEF, 0xF7, 0xD0, 0x5C, 0x97,
		0x98, 0x23, 0xE2, 0xD7, 0x9C, 0x0F, 0x4B, 0xB7, 0x1B, 0x37, 0x39, 0xDC, 0x5F, 0xD3, 0x9A, 0x16,
		0x66, 0x93, 0x62, 0xF3, 0x2D, 0x28, 0x2E, 0x51, 0x77, 0xFA, 0x19, 0x71, 0xC8, 0x8E, 0xE5, 0xE4,
		0x2E, 0x9E, 0xC1, 0x91, 0x1D, 0xFB, 0x94, 0xFE, 0xB1, 0x2E, 0xA1, 0x44, 0x0E, 0x31, 0xB1, 0x6E,
		0x9F, 0x59, 0x53, 0x7C, 0x55, 0xA1, 0xAF, 0xEB, 0x31, 0x32, 0xFB, 0xF7, 0x52, 0x35, 0xDA, 0x23,
		0xD0, 0xEC, 0x45, 0x13, 0x46, 0xAA, 0x05, 0x11, 0x77, 0x7C, 0x32, 0x26, 0x19, 0xFA, 0x4C, 0x68,
		0x29, 0x37, 0xAC, 0x6D, 0x62, 0x3D, 0xA4, 0x86, 0x04, 0x0B, 0x65, 0xA6, 0x19, 0x10, 0x4B, 0x13,
		0x6F, 0xED, 0xD8, 0xEF, 0x48, 0xF0, 0xA5, 0xA9, 0x5F, 0xCF, 0xB1, 0x59, 0x22, 0xBA, 0xF7, 0xEC,
		0x6B, 0xE7, 0x8F, 0x0C, 0x4B, 0xB9, 0x2A, 0xAE, 0x7B, 0xDA, 0x9B, 0x98, 0x25, 0x92, 0x0D, 0xE0,
		0xC6, 0x0D, 0x44, 0x07, 0x27, 0x58, 0xF2, 0x25, 0x50, 0xF9, 0xBF, 0xCB, 0xCD, 0x3B, 0xAA, 0x7B,
		0x62, 0x64, 0x45, 0x87, 0x50, 0xE6, 0x3D, 0x01, 0x45, 0xB2, 0x05, 0x3B, 0xE1, 0x5F, 0x0D, 0x25
	};
#else
	
	{
		0xD7, 0xA2, 0x23, 0x11, 0x71, 0xE5, 0x52, 0x31, 0x55, 0x0B, 0xF3, 0x87, 0x16, 0xBC, 0x9F, 0x43,
		0x47, 0x65, 0x66, 0x33, 0xBC, 0xDA, 0x62, 0x6A, 0x68, 0x63, 0x8E, 0x47, 0x43, 0x48, 0x14, 0x40,
		0xFB, 0x25, 0x76, 0x96, 0xAA, 0x64, 0x3F, 0x8D, 0x4E, 0xEB, 0x48, 0x03, 0xA3, 0x83, 0x22, 0x1F,
		0x82, 0x68, 0x6C, 0xB1, 0xDC, 0xC5, 0x67, 0x89, 0x15, 0x36, 0x18, 0x4A, 0xB3, 0xD6, 0xF3, 0xAF,
		0x5D, 0xD1, 0xCD, 0xB5, 0x06, 0x06, 0x97, 0xE3, 0xB2, 0xAC, 0xBA, 0xE6, 0x10, 0x79, 0x8F, 0xDA,
		0x80, 0xA4, 0x75, 0xBD, 0xE9, 0xCF, 0xEE, 0x98, 0x43, 0x8B, 0xE6, 0xC5, 0x99, 0x16, 0xF8, 0xEE,
		0x5D, 0x17, 0xF9, 0x9A, 0x93, 0xAC, 0x15, 0x81, 0xBB, 0x5D, 0x49, 0x0A, 0x67, 0x31, 0xFE, 0x56,
		0x6E, 0x3E, 0x8F, 0xA2, 0xDD, 0x06, 0x35, 0xC5, 0x4D, 0x16, 0x19, 0xC7, 0x7C, 0x71, 0xE4, 0xC5,
		0x32, 0x1F, 0x0E, 0xFA, 0xAF, 0x35, 0x05, 0x5D, 0x83, 0x9B, 0xF3, 0x4D, 0xF6, 0xE2, 0xA9, 0xDC,
		0xA7, 0xC0, 0xEC, 0x43, 0xA7, 0x2C, 0xEA, 0x8A, 0x11, 0xBE, 0x42, 0x51, 0xB9, 0xBC, 0xB6, 0x36,
		0x27, 0xD4, 0xE8, 0x2D, 0x33, 0xF6, 0x22, 0x44, 0x1E, 0x52, 0x8B, 0x9C, 0x11, 0x95, 0x9B, 0x1D,
		0x9C, 0x65, 0x95, 0xC9, 0x85, 0x06, 0x6A, 0x00, 0xA2, 0x88, 0x76, 0xB4, 0x8F, 0xFE, 0x2F, 0xC8,
		0xBA, 0xC4, 0x01, 0x34, 0x39, 0xDF, 0x93, 0xE1, 0xB7, 0xB5, 0xA4, 0xAC, 0x36, 0xF2, 0xD7, 0x3E,
		0xE9, 0x7E, 0x3A, 0xA0, 0x84, 0x6C, 0xFC, 0x68, 0xF8, 0x8A, 0x72, 0x81, 0xA9, 0x96, 0x63, 0x24,
		0xFD, 0x84, 0x02, 0xE9, 0x81, 0x85, 0xA6, 0x31, 0x05, 0x28, 0xAB, 0x7D, 0xC2, 0x63, 0xB9, 0x08,
		0x2A, 0x52, 0xC2, 0xA7, 0xA1, 0x95, 0xC4, 0xA8, 0xAA, 0x4F, 0xBC, 0x80, 0xF4, 0xE6, 0x05, 0xED,
	};
#endif

	

	PARAM_Clear();
	
	
	if(gen)	
        RSA_KeyGen_TSM();
	else	
        RSA_Keyinit_TSM();

	Bn2Bin(G_RSAkey_TSM->e, mInterParam.Etsm, CERT_ETSMSIZE);
	Bn2Bin(G_RSAkey_TSM->n, mInterParam.Ntsm, CERT_NTSMSIZE);
	
	return 1;
}
int CRsa::keyfree(){
	RSA_free((RSA*)G_RSAkey_TSM);
	return 1;
}

int CRsa::RSA_Keyinit_TSM()
{
	G_RSAkey_TSM = RSA_new();
	char n[2*256+2];
	char e[2*256+2];
	char d[2*256+2];
		
	//파일에서 읽어옴
	FILE *fp =fopen("TSM_RSA_KEY.txt","r");
	
	if (fp){
		//fopen("TSM_ECC_KEY.txt","r");
		fscanf(fp, "%s\n%s\n%s\n", n, e, d);
		fclose(fp);

	}else{
		strcpy(n,"CF3B58849C0A8B43A27E582AD7C6097AAF51445DAE1F397311B84359430D1E7B0D8C85E75B297C6E38FC83B1A7D5CA8B14C438C762693E8C2B09307ECE53214E40E1224D27C99F7DFA3D6077B8A4EB56ED7729DD7DAD099282BE394066D809C59ADC8CAE9C65AAF7CBF5948AEFDA62ECA4419037F5247719C930D6AFB979CDAC703756F17C9601C05CEE7EFD837B07C248CDCF90992D3A80E05D223CDD03298B0FD343214A2302B899587AE03093217CC2FB0D70BC7F55FF08E9237B6E2149EB10CD713CDC0B6D55031539825152FB6BA7D8AC1B26AD581D94CE026E972A589AD3303339F687FC44BA8128EFA598EB3467F4C0C24FA2A38AFE02005DEEA29D95");
		strcpy(e,"0000000000000000000000000000000B");

		strcpy(d, "4B5B6601AD1B1B5E69A24E9B3730BDA0F9EF01966DDCCF1292147594D290682CBF1BD399F29AB8E243448CFAC8AAD549D9018902AF6C16BE9B4928E84B06F4D6A33A981C0E77DCE7FDE7C5FCFD533E4E27CE3DC4E7E1D4EF755C71E8DF9460A4F27EBECB219955716170931B3FF2528498D205E5CD819FAC4929083FE65AD66C98471D6EC1A0CBB3CD5DD2B54F0B0DFD45557FDD759E32A7AE41B0A49E45B315B0FD1EDAEAF505017FFE78B22931A0D3752E7F5BE8E6DF71E6C112812D478375896C215A0CA5BC7052AEDFC5B99D31D96CDB4EEF46EAA46525E69C6F6FF2C9372512FDABF53988A8A57D843DBE3F7D4339D6B9462BDE6EBA04A34F5EEFB22823");

	}

	StringtoByteArray(n, mInterParam.Ntsm, 2*sizeof(mInterParam.Ntsm));
	StringtoByteArray(e, mInterParam.Etsm, 2*sizeof(mInterParam.Etsm));
	StringtoByteArray(d, mInterParam.Drsa, 2*sizeof(mInterParam.Drsa));


	//fclose(fp);

#if PRINTF
	printf("RSAkeyread_TSM\n");
	printf("Nchip : %s\n",n);
//	printf("Nchip : %c %c %x %x\n",n[0],n[1],n[2],n[3]);
	printf("E : %s\n",e);
	printf("D : %s\n",d);
#endif
	
	//BN_hex2bn(&G_RSAkey_TSM->d,d);
    


	UpdateTSM();
	UpdatePRIKEY();

     
//string to array test
/*
	StringtoByteArray(n,Nchip,256);
	printf("Nchip : ");
	for(int i=0;i<256; i++) printf("%2X",Nchip[i]);
	printf("\n");
	printf("\n");
	//
	ByteArraytoString(Nchip,n2,256);
	printf("Nchip : %s\n",n2);
*/

	return 0;
}
int CRsa::RSA_KeyGen_TSM()
{
	//TSM의RSA키 생성
	G_RSAkey_TSM = RSA_new();
	G_RSAkey_TSM = RSA_generate_key(2048,11,NULL,NULL);

#if PRINTF
	printf("RSAkeyGen_TSM\n");
	printf("Nchip : %s\n",BN_bn2hex(G_RSAkey_TSM->n));
	printf("E : %s\n",BN_bn2hex(G_RSAkey_TSM->e));
	
	printf("D : %s\n",BN_bn2hex(G_RSAkey_TSM->d));
	//printf("P : %s\n",BN_bn2hex(G_RSAkey_TSM->p));
	//printf("Q : %s\n",BN_bn2hex(G_RSAkey_TSM->q));
#endif
	
	//파일에 씀
	FILE *fp = fopen("TSM_RSA_KEY.txt","w");
	fprintf(fp,"%s\n%s\n%s\n",BN_bn2hex(G_RSAkey_TSM->n),BN_bn2hex(G_RSAkey_TSM->e),BN_bn2hex(G_RSAkey_TSM->d));
	fclose(fp);
	

	return 0;
}
////////////////////////////////	제조

int CRsa::generateSN(BYTE *pbOut, DWORD *pdwLenOut )
{

	//Chip의 SN를 생성하고 REQ_TEST packet을 생성한다.
	srand((unsigned)time(NULL)); 
//	for(i=0;i<16;i++)	SN[i]=rand(); 
	memset(mInterParam.SN, 0x78, 16);
	
	*pdwLenOut =17;
	pbOut[0] = CMD_REQ_TEST;
	memcpy(pbOut + 1, mInterParam.SN, 16);
	
	appendLog( "  REQ_TEST : %02X\n  Generated SN : ", pbOut[0]);
	
	//로그.
	B2Str(mInterParam.SN, 16);

	return 1;
}

int CRsa::processSNRSA(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut )
{
	//RSA 기반 Chip에서 전달된 SN을 Chip이 다시 전송했는지 RES_TEST packet을 확인하고 결과 packet을 생성한다.
	//선언
	int ERR_flag[2]= {0, 0};
	//파싱
	pbOut[0] = CMD_ERR_TEST;
	pbOut[1] = EC_NO_ERR;
	*pdwLenOut = 2;
	//프로세싱
	/*if(1 != pbIn[1]){		//++ 17 -> 1
        appendLog( "chip self test error\n");

		pbOut[1] = EC_ER_SN;
		memcpy(pbOut+2,SN,16);
		*pdwLenOut = 18;
	}
	
	else*/ if (memcmp(mInterParam.SN, pbIn + 2, 16) != 0){	//++ 1 ->2
		appendLog("  SN error\n");

//		ERR_flag[0] = 1;
//		pbOut[1] = EC_ER_DISUSE;
	}
    else
        appendLog( "  chip self test succeed\n");

	return 1;
}


////////////////////////////	발급


int CRsa::requestIdRSA( BYTE* pbOut, DWORD* pdwLenOut)
{
	//선언
	BYTE *SDATA = mInterParam.SDATA;
    //BYTE E_tsm[16] = {0};
	//BYTE N_tsm[256];
    //char* pbN;
    //char* pbE;
//    int iLenE;
	DWORD CRC;
#if 0
    pbN = BN_bn2hex(G_RSAkey_TSM->n);
    pbE = BN_bn2hex(G_RSAkey_TSM->e);
    iLenE = strlen(pbE);

	
	//StringtoByteArray(pbN, N_tsm, strlen(pbN));
    if (iExcep == EXCEP_TSM_E)
        memset(E_tsm, 0x02, 16);
    else
	    StringtoByteArray(pbE, E_tsm + 16 - iLenE / 2, iLenE);

#endif

	Bn2Bin(G_RSAkey_TSM->e, mInterParam.Etsm, CERT_ETSMSIZE);
	Bn2Bin(G_RSAkey_TSM->n, mInterParam.Ntsm, CERT_NTSMSIZE);


	Gen_SDATA(SDATA,mInterParam.certRSA.ID,mInterParam.G_PIN,mInterParam.G_INFORM);
	
	//머징
	//PbOut = REQ_PUK_RSA(1) || G_ID(16) || SDATA(32) || E_tsm(16) || N_tsm(256) || CRC(2)
	*pdwLenOut = 1+16+32+256+16+2;
	pbOut[0] = CMD_REQ_PUK_RSA;
	memcpy(pbOut+1			,mInterParam.certRSA.ID	,16);
	memcpy(pbOut+1+16		,SDATA	,32);
	memcpy(pbOut + 1 + 16 + 32, mInterParam.Etsm, 16);
	memcpy(pbOut + 1 + 16 + 32 + 16, mInterParam.Ntsm, 256);

    CRC = calcCRC(pbOut+1, 16+32+256+16);
	pbOut[1+16+32+16+256] = (BYTE)(CRC >> 8);
	pbOut[1+16+32+16+256+1] = (BYTE)(CRC);

	
	//로그
    appendLog( "  out data : ");
	B2Str(pbOut	,*pdwLenOut	);

    appendLog( "    REQ_PUK_RSA : %02X\n    ID : ", CMD_REQ_PUK_RSA);
	B2Str(mInterParam.certRSA.ID	,16		);

    appendLog( "    SDATA : ");
	B2Str(SDATA	,32		);

    appendLog( "    e(TSM) : ");
	B2Str(mInterParam.Etsm, 16);

    appendLog( "    Nchip(TSM) : ");
	B2Str(mInterParam.Ntsm, 256);

    appendLog( "    CRC : %04X\n", CRC);
	return 1;
}

int CRsa::RSA_Sign_tsm(BYTE* SIGN, BYTE *ID, BYTE *N_chip, RSA *key_tsm)
{
	// SIGN 256 byte
	// ID	16	byte
	// N_c	256 byte
	// key  RSA key_tsm(using Nchip, D)

	BYTE EM[256];
		
	EM[ 0] = 0x00;	EM[ 1] = 0x01;	
	memset(EM + 2, 0xFF, 256 - 3 - 19 - 32);	
	EM[204] = 0x00;
    memcpy(EM + 205, ID_SHA256, 19);

	SHA256_CTX c;
	SHA256_Init(&c);
	SHA256_Update(&c,ID,16);
	SHA256_Update(&c,N_chip,256);
	SHA256_Final(EM + 224, &c);

//	RSA_private_encrypt(51, EM + 205, SIGN, key_tsm, RSA_PKCS1_PADDING);
	RSA_private_encrypt(256, EM, SIGN, key_tsm, RSA_NO_PADDING);

#if RSA_XOR
	RSA_xor_tsm(EM, SIGN);
#endif

	return 1;
}

int CRsa::GenCertRSA(CERT_RSA* cert, BYTE* ID, BYTE* N_chip, RSA* key_tsm)
{
    memcpy(cert->ID, ID, 16);
    memcpy(cert->Nchip, N_chip, 256);
	RSA_Sign_tsm(cert->SIGN, ID, N_chip, key_tsm);
	return 1;
}

int CRsa::requestCertRSA(BYTE* pbIn, int iLenIn )
{
	//선언
	DWORD CRC;

	//파싱
    appendLog( "  input data : ");
	B2Str(pbIn	,iLenIn		);

	if(pbIn[0] != CMD_RES_PUK_RSA)
    {
        appendLog( "  not RES_PUK_RSA : %02X\n", pbIn[0]);
        return -1;
    }

    if (iLenIn != 259) 
    {
        appendLog( "  length error\n");
        return -1;
    }

    appendLog( "    RES_PUK_RSA : %02X\n    Nchip(Chip) : ", pbIn[0]);
	B2Str(pbIn + 1	,256		);

    //CRC = calcCRC(pbIn, iLenIn - 2);
	CRC = calcCRC(pbIn + 1, iLenIn - 2 - 1);

    if (CRC != ((pbIn[iLenIn - 2] << 8) + pbIn[iLenIn - 1]))
    {
        appendLog( "    CRC error(calculated CRC : %04X)\n", CRC);
        return -1;
    }

    memcpy(mInterParam.certRSA.Nchip, pbIn + 1, 256); //mInterParam.certRSA.Nchip은 파일로 저장?

	return 1;
}


int CRsa::requestRegCertRSA( BYTE* pbOut, DWORD* pdwLenOut)
{
	//선언
	CERT_RSA cert;
	DWORD CRC;

  
    {
	    //데이터 프로세싱
        GenCertRSA(&cert,   mInterParam.certRSA.ID, mInterParam.certRSA.Nchip, G_RSAkey_TSM);
    }

	//머징
	//REQ_REG_CERT(1) || CERT(528) || CRC(2)
	*pdwLenOut = 1+528+2;
	pbOut[0] = CMD_REQ_REG_CERT_RSA;
	memcpy(pbOut+1		,&cert	,528);

    //CRC = calcCRC(pbOut, 1+528);
	CRC = calcCRC(pbOut+1, 528);
	pbOut[1+528] = (BYTE)(CRC >> 8);
	pbOut[1+528+1] = (BYTE)(CRC);
	
	//로그
    appendLog( "  out data : ");
	B2Str(pbOut	,*pdwLenOut	);

    appendLog( "    REQ_REG_CERT_RSA : %02X\n    ID : ", CMD_REQ_REG_CERT_RSA);
	B2Str((BYTE*)cert.ID	,16		);

    appendLog( "    Nchip(chip) : ");
	B2Str(cert.Nchip	,256	);

    appendLog( "    signiture : ");
	B2Str(cert.SIGN, 256);

    appendLog( "    CRC : %04X\n", CRC);
	return 1;
}


// 발급
////////////////////////////////////////////////////////////
// 사용
bool CRsa::SetRN(const char * hexRN)
{

	
	if (hexRN){
		if (strlen(hexRN) != 2 * DEFUNITSIZE) {
			errlog = "INPUT HEXSTR SIZE is MUST BE 16x2 ";
			return false;
		}
		hex2bin(hexRN, mInterParam.RN);
		return true;
	}
	srand((unsigned)time(NULL));
	for (int i = 0; i<16; i++){
		mInterParam.RN[i] = rand();
	}
	//memset(mInterParam.RN, 0x32, 16);
	return true;

}

int CRsa::requestRN(BYTE *pbOut, DWORD *pdwLenOut)
{
    int i;
	//BYTE SDATA[32];
	//선언
	//프로세싱
	srand((unsigned)time(NULL)); 
	for(i=0;i<16;i++){
		mInterParam.RN[i] = rand();
	}
	memset(mInterParam.RN, 0x32, 16);
		
	//머징
	//pbOut = REQ_AUTH(1) || SDATA(32) || EHK_device(32) || RN (16)
	*pdwLenOut = 1+32+32+16;
	pbOut[0] = CMD_REQ_AUTH_RSA;
	Gen_SDATA(mInterParam.SDATA, mInterParam.certRSA.ID, mInterParam.G_PIN, mInterParam.G_INFORM);

	memcpy(pbOut + 1, mInterParam.SDATA, 32);
	memcpy(pbOut + 1 + 32, mInterParam.EHKDevice, 32);
	memcpy(pbOut + 1 + 32 + 32, mInterParam.RN, 16);

	//로그
    appendLog( "  out data : ");
	B2Str(pbOut		,*pdwLenOut	);

	appendLog( "    REQ_AUTH_RSA %02X\n    SDATA  : ", CMD_REQ_AUTH_RSA);
	B2Str(mInterParam.SDATA, 32);

	appendLog( "    EHKDevice(device)  : ");
	B2Str(pbOut+1+32,32		);

    appendLog( "    RN  : ");
	B2Str( mInterParam.RN		,16		);

	return 1;
}
#if 0
int RSA_Veri_chip(BYTE* S, BYTE *ID, BYTE *C, BYTE *E, RSA *key_chip, )
{
	// S	256 byte
	// ID	16	byte
	// C	256 byte
	// E	16 byte
	// key  RSA key_chip(using Nchip, E)

	int i;
	int num;
	BYTE EM[32];
	BYTE EM_prime[256];
		
	SHA256_CTX c;
	SHA256_Init(&c);
	SHA256_Update(&c,ID,16);
	SHA256_Update(&c,C,256);
	SHA256_Update(&c,E,256);
	SHA256_Final(EM,&c);	//EM 완성

	num = RSA_public_decrypt(256, S, EM_prime, key_chip, RSA_PKCS1_PADDING);
	//num = RSA_private_encrypt(sizeof(EM)-1,EM,SIGN,key_tsm,RSA_PKCS1_PADDING);

    for ( ; ; )
    {
        if ((EM_prime[0] != 0) || (EM_prime[1] != 1))
            break;

        for (i = 2; i < (256-32-19-1); i++)
            if (EM_prime[i] != 0xFF)
                break;

        if (EM_prime[i] != 0)
            break;

        if (memcmp(EM_prime + i + 1, ID_SHA256, 19) != 0)
            break;

        if (memcmp(EM_prime + i + 20, EM, 32) != 0)
            break;

        iFlagErr = 0;
        break;
    }

    if (iFlagErr != 0)
    {
        appendLog( "  sign error\n");
        return -1;
    }

	return 1;
}
#endif

int CRsa::requestSessionRSA(BYTE* pbIn, int iLenIn )
{
	//선언
	int i;
	BYTE *ID;
	BYTE *C;	// K1 || K2 || RN || IV || HK_device	=	16+16+16+16+32
	BYTE *E;
	BYTE *S;

	char e[2*16+2];
	char n[2*256+2];
	
	BYTE M[256];	
	BYTE RN_prime[16];
	
	RSA *key_chip;
	key_chip = RSA_new();

	//파싱
	if(pbIn[0] != CMD_REQ_SESSION_RSA)
    {
        appendLog( "  not REQ_SESSION_RSA : %02X\n", pbIn[0]);
        return -1;
    }

    if (iLenIn != (1 + 16 + 256 + 16 + 256)) 
    {
        appendLog( "  length error\n");
        return -1;
    }
    appendLog( "    REQ_SESSION_RSA : %02X\n", pbIn[0]);

	ID = pbIn+1;
	C  = pbIn+1+16;
	E  = pbIn+1+16+256;
	S  = pbIn+1+16+256+16;
	
    appendLog( "    ID : ");
	B2Str(ID	,16		);

    appendLog( "    C : ");
	B2Str(C	,256		);

    appendLog( "    E : ");
	B2Str(E	,16		);

    appendLog( "    S : ");
	B2Str(S	,256		);

	//프로세싱
	ByteArraytoString(mInterParam.certRSA.Nchip, n, 256);
	BN_hex2bn(&key_chip->n ,n);
	//key_chip->n = BN_bin2bn(mInterParam.certRSA.Nchip, 256, NULL);

	ByteArraytoString(E, e, 16);
	BN_hex2bn(&key_chip->e, e);

	//key_chip->e = BN_bin2bn(E, 16, NULL);

	
	
	
//	if(RSA_check_key(key_chip) == 0)
//	{
//		appendLog( "  rsa_key_chip error\n");
//		return -1;
//	}

	////2. RSA_Vchip
    {
	    // S	256 byte
	    // ID	16	byte
	    // C	256 byte
	    // E	16 byte
	    // key  RSA key_chip(using Nchip, E)

//	    int i;
	    int num;
	    BYTE EM[32];
	    BYTE EM_prime[256];
        int iFlagErr;
	
	    SHA256_CTX c;
	    SHA256_Init(&c);
	    SHA256_Update(&c,ID,16);
	    SHA256_Update(&c,C,256);
//	    SHA256_Update(&c,E,16);
	    SHA256_Final(EM,&c);	//EM 완성

		num = RSA_public_decrypt(256, S, EM_prime, key_chip, RSA_NO_PADDING);
	    //num = RSA_private_encrypt(sizeof(EM)-1,EM,SIGN,key_tsm,RSA_PKCS1_PADDING);

#if RSA_XOR
		RSA_xor_chip(S, mInterParam.certRSA.Nchip, E, EM_prime);
#endif

        appendLog( "    sign verified data : ");
	    B2Str(EM_prime	,256		);

        for (iFlagErr = 1 ; ; )
        {
            if ((EM_prime[0] != 0) || (EM_prime[1] != 1))
                break;

            for (i = 2; i < (256-32-19-1); i++)
                if (EM_prime[i] != 0xFF)
                    break;

            if (EM_prime[i] != 0)
                break;

            if (memcmp(EM_prime + i + 1, ID_SHA256, 19) != 0)
                break;

            if (memcmp(EM_prime + i + 20, EM, 32) != 0)
                break;

            iFlagErr = 0;
            break;
        }

        if (iFlagErr != 0)
        {
            appendLog( "  sign error\n");
            return -1;
        }
    }

	int	num = RSA_private_decrypt(256, C, M, G_RSAkey_TSM, RSA_NO_PADDING);	
	
#if RSA_XOR
	RSA_xor_tsm(C,M);
	//RSA_xor_chip(C, mInterParam.certRSA.Nchip, E, M);
	
#endif

    appendLog( "    C decrypted data : ");
    B2Str(M	,256		);

    if ((M[0] != 0) || (M[1] != 2))
    {
        appendLog( "  sign format error1\n");
        return -1;
    }

#ifdef REAL_CHIP
    if (M[256 - 96 - 1] != 0)
    {
        appendLog( "  sign format error2\n");
        return -1;
    }

    i = 256 - 96 - 1;
#else
    for (i = 2; i < (256-96-1); i++)
        if (M[i] == 0)
            break;

    if (M[i] != 0)
    {
        appendLog( "  sign format error2\n");
        return -1;
    }
#endif

	memcpy(mInterParam.K1, M + i + 1, 16);
	memcpy(mInterParam.K2, M + i + 1 + 16, 16);
    memcpy(RN_prime,    M + i + 1 + 32, 16);
	memcpy(mInterParam.IV, M + i + 1 + 48, 16);
	memcpy(mInterParam.HK_device, M + i + 1 + 64, 32);

    appendLog( "    K1 : ");
	B2Str(mInterParam.K1, 16);

    appendLog( "    K2 : ");
	B2Str(mInterParam.K2, 16);

    appendLog( "    RN : ");
	B2Str(RN_prime ,16		);

    appendLog( "    IV : ");
	B2Str(mInterParam.IV, 16);

    appendLog( "    HK(device) : ");
	B2Str(mInterParam.HK_device, 32);

	if (memcmp(mInterParam.RN, RN_prime, 16) != 0)
    {
        appendLog( "  RN error\n");
        return -1;
    }

	return 1;
}


int CRsa::requestHS1RSA(BYTE * pbOut, DWORD * pdwLenOut)
{
	//선언
	BYTE C_ECB[16];	
	mInterParam.g_msgPos = 0;

	////6. E_ECB:k1(IV)
	E_ECB_K1(mInterParam.IV, C_ECB, mInterParam.SEL_AES);


    //머징
	//REQ_HS1(1) || C_ECB(16);
	*pdwLenOut = 1+16;
	pbOut[0] = CMD_REQ_HS1_RSA;
	memcpy(pbOut+1,C_ECB,16);

    appendLog( "  out data : ");
	B2Str(pbOut	,*pdwLenOut	);

    appendLog( "    REQ_HS1 : %02X\n    C1 : ", CMD_REQ_HS1_RSA);
    appendLog( "    C1 : ");
	B2Str(pbOut + 1	,16		);

	return 1;
}



int CRsa::requestSignRSA(BYTE* pbIn, int iLenIn, BYTE *pbOut, DWORD *pdwLenOut, BYTE* pbOTP)
{
	//선언
	BYTE *E_prime;
	BYTE *C_otp;
	BYTE *S_prime;
	char e[2*16+2];
	char n[2*256+2];
	
	RSA *key_chip;
	int num,i;
	BYTE EM[32];
	BYTE EM_prime[256];
	int iFlagErr;

	SHA256_CTX c;

	//파싱	cmd(1) ||  E`(16) || C_opt(16) || S`(256)
	appendLog( "  input data : ");
	B2Str(pbIn	,iLenIn		);
	
	if(pbIn[0] != CMD_RES_SIGN_RSA)
    {
        appendLog( "  not RES_SIGN_RSA : %02X\n", pbIn[0]);
        return -1;
    }
    if (iLenIn != 1+16+16+256) 
    {
        appendLog( "  length error\n");
        return -1;
    }

    appendLog( "  RES_SIGN_RSA : %02X\n", pbIn[0]);

	E_prime = pbIn+1;
	C_otp	= pbIn+1+16;
	S_prime	= pbIn+1+16+16;

    appendLog( "    E' : ");
	B2Str(E_prime	,16		);

    appendLog( "    C(OTP) : ");
	B2Str(C_otp	,16		);

    appendLog( "    S : ");
	B2Str(S_prime	,256		);

	//프로세싱
	////1. E_prime check
	
	key_chip = RSA_new();

	ByteArraytoString(mInterParam.certRSA.Nchip, n, 256);
	BN_hex2bn(&key_chip->n ,n);
	ByteArraytoString(E_prime, e, 16);
	BN_hex2bn(&key_chip->e, e);

//	if(RSA_check_key(key_chip) ==0)
//	{
//		appendLog( "  rsa_key_chip error\n");
//		return -1;
//	}

	////2. RSA_Vchip(msg||ID||E`||C_otp) == S` 
	SHA256_Init(&c);
	if(mInterParam.g_msgPos) 
		SHA256_Update(&c, mInterParam.G_MSG, mInterParam.g_msgPos);
//	SHA256_Update(&c,E_prime,16);
	SHA256_Update(&c,mInterParam.certRSA.ID,16);
	
	//SHA256_Update(&c,C_otp,16);
	SHA256_Final(EM,&c);	//HASH(msg) 완성
		
	
    appendLog( "    hash_for_sign data1 : ");
	B2Str(EM	,32	);


	num = RSA_public_decrypt(256, S_prime, EM_prime, key_chip, RSA_NO_PADDING);

#if RSA_XOR
	RSA_xor_chip(S_prime, mInterParam.certRSA.Nchip, E_prime, EM_prime);
#endif

	
    RSA_free(key_chip);
	appendLog( "    hash_for_sign data2 : ");
	B2Str(EM_prime+256-32	,32	);

    appendLog( "    sign verified data : ");
    B2Str(EM_prime	,256		);

	for (iFlagErr = 1 ; ; )
	{
		if ((EM_prime[0] != 0) || (EM_prime[1] != 1))
			break;

		for (i = 2; i < (256-32-19-1); i++)
			if (EM_prime[i] != 0xFF)
				break;

		if (EM_prime[i] != 0)
			break;

		if (memcmp(EM_prime + i + 1, ID_SHA256, 19) != 0)
			break;

		if (memcmp(EM_prime + i + 20, EM, 32) != 0)
			break;

		iFlagErr = 0;
		break;
	}

	if (iFlagErr != 0)
	{
		appendLog( "  sign error\n");
		pbOut[0] = CMD_ERR_SIGN;
		pbOut[1] = EC_ER_RSA_SIGN;
		*pdwLenOut = 2;
		return -1;
	}    

    {
	    ////3. D_ECB:k1(C_otp) == GenerateUserOTP()
	    BYTE P_otp[16];	// 6byte || zero_padding

		D_ECB_K1(C_otp, P_otp, mInterParam.SEL_AES);
/*
	    AES_KEY AES_key;
	    AES_set_decrypt_key(K1, 128, &AES_key);
	    AES_decrypt(C_otp, P_otp, &AES_key);
*/

#ifndef OTP_6_BYTES
	    if (memcmp(pbOTP, P_otp, 3) != 0)
#else
	    if (memcmp(pbOTP, P_otp, 6) != 0)
#endif
	    {
		    appendLog( "  OTP compare error\n");
			pbOut[0] = CMD_ERR_SIGN;
			pbOut[1] = EC_ER_OTP;
			*pdwLenOut = 2;
		    return -1;
	    }    

	    ////4. HK`_tsm = hash(K1||K2||IV)
	    SHA256_CTX ck;
	    SHA256_Init(&ck);
		SHA256_Update(&ck, mInterParam.K1, 16);
		SHA256_Update(&ck, mInterParam.K2, 16);
		SHA256_Update(&ck, mInterParam.IV, 16);
		SHA256_Final(mInterParam.HK_tsm, &ck);

	    ////5. store(HK`_tsm)
	    //4번에서 처리

	    //로그
	    appendLog( "    OTP : ");
	    B2Str(P_otp	,3		);
	    appendLog( "    HK`_TSM: ");
		B2Str(mInterParam.HK_tsm, 32);
    }

	//머징
	pbOut[0] = CMD_ERR_SIGN;
   	pbOut[1] = EC_NO_ERR;


    *pdwLenOut = 2;

    appendLog( "  out data : ");
	B2Str(pbOut	,*pdwLenOut	);

    appendLog( "  %02X(ERR_SIGN), %02X\n", 
        CMD_ERR_SIGN, pbOut[1]);


	return 1;
}





////////// test 용 간소화 RSA연산	
/*
 지금  RSA_sim.v의 내용은 다음과 같으므로 이에 따라 소프트웨어 쪽도 수정해서 FPGA 테스트 요망

1.	암호화 또는 서명 검증 : 칩으로부터 받은 E와 Nchip을 이용해서 2048-bit 메시지와 xor함 (E는 128-bit이므로 16번 반복 후에 xor) 
	o_data = i_data ^ {16{E}} ^Nchip

	(칩에서 자신의 개인키로 암호화 시 필요한 입력은 개인키 d이나 실제로는 E를 입력 받아 d를 생성하므로
		칩에서 암호화 시 입력이 제대로 들어왔는지 테스트하기 위해서는 E와 Nchip이 사용되면 됨)

2.	복호화 또는 서명 생성 : TSM의 E와 Nchip을 이용하여 2048-bit 메시지와 xor(마찬가지로 E는 16번 반복 후 xor)
	o_data = i_data ^ {16{E}} ^Nchip
*/


int CRsa::RSA_xor_chip(BYTE* i_data, BYTE* Nc, BYTE* Ec, BYTE* o_data)
{
	for(int i=0; i<256; i++)
	{
		o_data[i] = i_data[i] ^ Nc[i] ^ Ec[i%16];
	}
	
	return 1;
}

int CRsa::RSA_xor_tsm(BYTE* i_data, BYTE* o_data)
{
	BYTE E_tsm[16] = {0};
	BYTE N_tsm[256];
    char* pbN;
    char* pbE;
    int iLenE;

	pbN = BN_bn2hex(G_RSAkey_TSM->n);
    pbE = BN_bn2hex(G_RSAkey_TSM->e);
    iLenE = (int)strlen(pbE);

	StringtoByteArray((const char *)pbN, N_tsm, (int)strlen(pbN));
	StringtoByteArray((const char *)pbE, E_tsm + 16 - iLenE / 2, iLenE);

	for(int i=0; i<256; i++)
	{
		o_data[i] = i_data[i] ^ N_tsm[i] ^ E_tsm[i%16];
	}

	return 1;
}
BYTE*	CRsa::getHKDevice(){

	return mInterParam.EHKDevice;

}
#if 0
void CRsa::UpdateSDATA()
{
	//requestIdRSA


	BYTE *SDATA = mInterParam.SDATA;
	//BYTE E_tsm[16] = { 0 };
	//BYTE N_tsm[256];
	//char* pbN;
	//char* pbE;
	int iLenE;
	DWORD CRC;


	Bn2Bin(G_RSAkey_TSM->e, mInterParam.Etsm, CERT_ETSMSIZE);
	Bn2Bin(G_RSAkey_TSM->n, mInterParam.Ntsm, CERT_NTSMSIZE);


	Gen_SDATA(SDATA, mInterParam.certRSA.ID, mInterParam.G_PIN, mInterParam.G_INFORM);



}
#endif // 0

void CRsa::UpdateCERT(){
	resetSignHash();
	
	appendLog("Nchip : ");
	B2Str(mInterParam.certRSA.Nchip,256);

	GenCertRSA(&mInterParam.certRSA, mInterParam.certRSA.ID, mInterParam.certRSA.Nchip, G_RSAkey_TSM);
}

bool CRsa::UpdateC1()
{
	//requestSessionRSA();

	int i;

//	char e[2 * 16 + 2];
//	char n[2 * 256 + 2];
	NEO_MARK(UpdateC1-0);
	BYTE M[256];
	BYTE RN_prime[16];
#if 0
	RSA *key_chip;
	key_chip = RSA_new();

	//프로세싱
	ByteArraytoString(mInterParam.certRSA.Nchip, n, nC);
	BN_hex2bn(&key_chip->n, n);

	

	ByteArraytoString(E, e, 16);
	BN_hex2bn(&key_chip->e, e);

	BN_bin2bn(mInterParam.certRSA.Nchip, CERT_NCHIPSIZE, key_chip->n);
	BN_bin2bn(E, nE, key_chip->e);
#endif

	
	////2. RSA_Vchip
	
		// S	256 byte
		// ID	16	byte
		// C	256 byte
		// E	16 byte
		// key  RSA key_chip(using Nchip, E)

		//	    int i;
		int num;
		BYTE EM[32];
//		BYTE EM_prime[256];
//		int iFlagErr;
		
		SHA256_CTX c;
		SHA256_Init(&c);
		SHA256_Update(&c, mInterParam.certRSA.ID, 16);
		SHA256_Update(&c, mInterParam.C, 256);
		//	    SHA256_Update(&c,E,16);
		SHA256_Final(EM, &c);	//EM 완성

		NEO_MARK(UpdateC1 - 1);
		NEO_DWORD(mInterParam.E[0]);
		NEO_DWORD(mInterParam.certRSA.Nchip[0]);
		NEO_DWORD(EM[0]);
		NEO_DWORD(mInterParam.S[0]);
		if (!VerifySign(mInterParam.E, mInterParam.certRSA.Nchip, EM, mInterParam.S)){
			return false;
		}

#if 0
		num = RSA_public_decrypt(256, S, EM_prime, key_chip, RSA_NO_PADDING);
		//num = RSA_private_encrypt(sizeof(EM)-1,EM,SIGN,key_tsm,RSA_PKCS1_PADDING);



		for (iFlagErr = 1;;)
		{
			if ((EM_prime[0] != 0) || (EM_prime[1] != 1))
				break;

			for (i = 2; i < (256 - 32 - 19 - 1); i++)
			if (EM_prime[i] != 0xFF)
				break;

			if (EM_prime[i] != 0)
				break;

			if (memcmp(EM_prime + i + 1, ID_SHA256, 19) != 0)
				break;

			if (memcmp(EM_prime + i + 20, EM, 32) != 0)
				break;

			iFlagErr = 0;
			break;
		}

		if (iFlagErr != 0)
		{
			//appendLog( "  sign error\n");
			return false;
		}
	
#endif

	num = RSA_private_decrypt(256, mInterParam.C, M, G_RSAkey_TSM, RSA_NO_PADDING);

	NEO_MARK(UpdateC1 - 3);
	if ((M[0] != 0) || (M[1] != 2))
	{
		errlog = "  sign format error2";
		return false;
	}


	for (i = 2; i < (256 - 96 - 1); i++)
	if (M[i] == 0)
		break;

	if (M[i] != 0)
	{
		errlog = "  sign format error2";
		return false;
	}


	memcpy(mInterParam.K1, M + i + 1, 16);
	memcpy(mInterParam.K2, M + i + 1 + 16, 16);
	memcpy(RN_prime, M + i + 1 + 32, 16);
	memcpy(mInterParam.IV, M + i + 1 + 48, 16);
	memcpy(mInterParam.HK_device, M + i + 1 + 64, 32);

	NEO_MARK(UpdateC1 - 4);

	if (memcmp(mInterParam.RN, RN_prime, 16) != 0)
	{
		errlog = "RN is NOT EQUAL";
		return false;
	}



	//requestHS1RSA

	//선언
//	BYTE C_ECB[16];
	mInterParam.g_msgPos = 0;
	////6. E_ECB:k1(IV)
	E_ECB_K1(mInterParam.IV, mInterParam.C1, mInterParam.SEL_AES);
	NEO_MARK(UpdateC1 - END);
	return true;
}

void CRsa::SetID(BYTE *pID, int idSize)
{

	memset(mInterParam.certRSA.ID, 0, sizeof(mInterParam.certRSA.ID));
	memcpy(mInterParam.certRSA.ID, pID, idSize);

}
void CRsa::SetID(const char  *strID)
{
	memset(mInterParam.certRSA.ID, 0, sizeof(mInterParam.certRSA.ID));
	::hex2bin(strID, mInterParam.certRSA.ID);


}
#if 0
CERTEXT_RSA * CRsa::getCERTEXT_RSA()
{
	return &this->mInterParam;

}

#endif // 0

string CRsa::viewCETEXT()
{
	string str = "";
	str += "ID:";
	str += BytetoHexStr(mInterParam.certRSA.ID, CERT_IDSIZE);
	str += "\n";
	str += "NChip:";
	str += BytetoHexStr(mInterParam.certRSA.Nchip, CERT_NCHIPSIZE);
	str += "\n";
	str += "Sign:";
	str += BytetoHexStr(mInterParam.certRSA.SIGN, CERT_SIGNSZE_RSA);
	str += "\n";
	str += "SDATA:";
	str += BytetoHexStr(mInterParam.SDATA, CERT_SDATASIZE);
	str += "\n";
	str += "EHK:";
	str += BytetoHexStr(mInterParam.EHKDevice, CERT_EHKSIZE);
	str += "\n";

	return str;
}
bool CRsa::VerifySign(BYTE * E,BYTE * N, BYTE * EM, BYTE * sign)
{
	int num, i;
	int iFlagErr;
	BYTE EM_prime[256];
	RSA *key_chip;


	key_chip = RSA_new();
	key_chip->n = BN_bin2bn(N, CERT_NTSMSIZE, NULL);
	key_chip->e = BN_bin2bn(E, CERT_ETSMSIZE, NULL);



	num = RSA_public_decrypt(256, sign, EM_prime, key_chip, RSA_NO_PADDING);


	RSA_free(key_chip);
	appendLog("    hash_for_sign data2 : ");
	B2Str(EM_prime + 256 - 32, 32);

	appendLog("    sign verified data : ");
	B2Str(EM_prime, 256);

	for (iFlagErr = 1;;)
	{
		if ((EM_prime[0] != 0) || (EM_prime[1] != 1)){
			appendLog("Init err");
			break;
		}
			

		for (i = 2; i < (256 - 32 - 19 - 1); i++)
		if (EM_prime[i] != 0xFF){
			appendLog("FF errr ");
			break;
		}
			

		if (EM_prime[i] != 0){
			appendLog("middle zero error ");
			break;
		}
			

		if (memcmp(EM_prime + i + 1, ID_SHA256, 19) != 0){
			appendLog("ID_SHA256 error ");
			break;
		}

		if (memcmp(EM_prime + i + 20, EM, 32) != 0){
			appendLog("msg|ID Hash error ");
			break;
		}

		iFlagErr = 0;
		break;
	}

	if (iFlagErr != 0)
	{
		errlog = " sign error\n";
		return false;
	}
	return true;
	
}

bool CRsa::SetDIGITSign(BYTE * E_prime, BYTE *C_otp, BYTE *S_prime,  BYTE* pbOTP)
{
	memcpy(mInterParam.Ep, E_prime, DEFUNITSIZE);
	memcpy(mInterParam.Cotp, C_otp, DEFUNITSIZE);
	memcpy(mInterParam.Sp, S_prime, DEFRSASIZE);
	memcpy(mInterParam.OTP, pbOTP, OTPSIZE);
//	char e[2 * 16 + 2];
//	char n[2 * 256 + 2];

//	RSA *key_chip;
//	int num, i;
	BYTE EM[32];
//	BYTE EM_prime[256];
//	int iFlagErr;

	SHA256_CTX c;

	//파싱	cmd(1) ||  E`(16) || C_opt(16) || S`(256)
	appendLog("  input data : ");
	

	appendLog("  RES_SIGN_RSA");


	appendLog("    E' : ");
	B2Str(E_prime, DEFUNITSIZE);

	appendLog("    C(OTP) : ");
	B2Str(C_otp, DEFUNITSIZE);

	appendLog("    S : ");
	B2Str(S_prime, CERT_SIGNSZE_RSA);
#if 0
	//프로세싱
	////1. E_prime check

	key_chip = RSA_new();

	ByteArraytoString(mInterParam.certRSA.Nchip, n, 256);
	BN_hex2bn(&key_chip->n, n);
	ByteArraytoString(E_prime, e, 16);
	BN_hex2bn(&key_chip->e, e);
#endif
	//	if(RSA_check_key(key_chip) ==0)
	//	{
	//		appendLog( "  rsa_key_chip error\n");
	//		return -1;
	//	}

	////2. RSA_Vchip(msg||ID||E`||C_otp) == S` 
#if 0
	SHA256_Init(&c);
	if (mInterParam.g_msgPos)
		SHA256_Update(&c, mInterParam.G_MSG, mInterParam.g_msgPos);
	//	SHA256_Update(&c,E_prime,16);
	SHA256_Update(&c, mInterParam.certRSA.ID, 16);

	//SHA256_Update(&c,C_otp,16);
	SHA256_Final(EM, &c);	//HASH(msg) 완성

#endif // 0

	updateSignHash(mInterParam.ID, CERT_IDSIZE);
	getHashValue4Sign(EM);


	appendLog("    hash_for_sign data1 : ");
	B2Str(EM, 32);

	if (!VerifySign(E_prime, mInterParam.certRSA.Nchip, EM, S_prime)){

		return false;
	}
#if 0
	num = RSA_public_decrypt(256, S_prime, EM_prime, key_chip, RSA_NO_PADDING);

#if RSA_XOR
	RSA_xor_chip(S_prime, mInterParam.certRSA.Nchip, E_prime, EM_prime);
#endif


	RSA_free(key_chip);
	appendLog("    hash_for_sign data2 : ");
	B2Str(EM_prime + 256 - 32, 32);

	appendLog("    sign verified data : ");
	B2Str(EM_prime, 256);

	for (iFlagErr = 1;;)
	{
		if ((EM_prime[0] != 0) || (EM_prime[1] != 1))
			break;

		for (i = 2; i < (256 - 32 - 19 - 1); i++)
		if (EM_prime[i] != 0xFF)
			break;

		if (EM_prime[i] != 0)
			break;

		if (memcmp(EM_prime + i + 1, ID_SHA256, 19) != 0)
			break;

		if (memcmp(EM_prime + i + 20, EM, 32) != 0)
			break;

		iFlagErr = 0;
		break;
	}

	if (iFlagErr != 0)
	{
		errlog = " sign error\n";
		return false;
	}
#endif
	
		////3. D_ECB:k1(C_otp) == GenerateUserOTP()
		BYTE P_otp[16];	// 6byte || zero_padding

		D_ECB_K1(C_otp, P_otp, mInterParam.SEL_AES);
		/*
		AES_KEY AES_key;
		AES_set_decrypt_key(K1, 128, &AES_key);
		AES_decrypt(C_otp, P_otp, &AES_key);
		*/


		if (memcmp(pbOTP, P_otp, 3) != 0)
		{
			errlog = "  OTP compare error\n";
			return false;
		}

		////4. HK`_tsm = hash(K1||K2||IV)
		SHA256_CTX ck;
		SHA256_Init(&ck);
		SHA256_Update(&ck, mInterParam.K1, 16);
		SHA256_Update(&ck, mInterParam.K2, 16);
		SHA256_Update(&ck, mInterParam.IV, 16);
		SHA256_Final(mInterParam.HK_tsm, &ck);

		////5. store(HK`_tsm)
		//4번에서 처리

		//로그
	appendLog("    OTP : ");
	B2Str(P_otp, 3);
	appendLog("    HK`_TSM: ");
	B2Str(mInterParam.HK_tsm, 32);

	//UpdateSign();
	

	return true;



}

void CRsa::UpdateTSM(){
	//Bn2Bin(G_RSAkey_TSM->e, mInterParam.Etsm, CERT_ETSMSIZE);
	//Bn2Bin(G_RSAkey_TSM->n, mInterParam.Ntsm, CERT_NTSMSIZE);

	G_RSAkey_TSM->e = BN_bin2bn(mInterParam.Etsm, sizeof(mInterParam.Etsm), G_RSAkey_TSM->e);
	G_RSAkey_TSM->n = BN_bin2bn(mInterParam.Ntsm, sizeof(mInterParam.Ntsm), G_RSAkey_TSM->n);
	//키값 전역변수에 저장
	//BN_hex2bn(&G_RSAkey_TSM->n,n);
	G_RSAkey_TSM->n->dmax = 0x100;
	//BN_hex2bn(&G_RSAkey_TSM->e,e);
	G_RSAkey_TSM->e->dmax = 0x01;

}

void CRsa::UpdatePRIKEY(){
	G_RSAkey_TSM->d = BN_bin2bn(mInterParam.Drsa, sizeof(mInterParam.Drsa), G_RSAkey_TSM->d);
	//Bn2Bin(&G_RSAkey_TSM->d, mInterParam.Drsa);
	G_RSAkey_TSM->d->dmax = 0x100;

}
bool CRsa::UpdateSign()
{

//	char e[2 * 16 + 2];
//	char n[2 * 256 + 2];

//	RSA *key_chip;
//	int num, i;
	BYTE EM[32];
//	BYTE EM_prime[256];
//	int iFlagErr;

//	SHA256_CTX c;

	//파싱	cmd(1) ||  E`(16) || C_opt(16) || S`(256)
	appendLog("  input data : ");


	appendLog("  RES_SIGN_RSA");


	appendLog("    E' : ");
	B2Str(mInterParam.Ep, DEFUNITSIZE);

	appendLog("    C(OTP) : ");
	B2Str(mInterParam.Cotp, DEFUNITSIZE);

	appendLog("    S : ");
	B2Str(mInterParam.Sp, CERT_SIGNSZE_RSA);
#if 0
	//프로세싱
	////1. E_prime check

	key_chip = RSA_new();

	ByteArraytoString(mInterParam.certRSA.Nchip, n, 256);
	BN_hex2bn(&key_chip->n, n);
	ByteArraytoString(E_prime, e, 16);
	BN_hex2bn(&key_chip->e, e);
#endif
	//	if(RSA_check_key(key_chip) ==0)
	//	{
	//		appendLog( "  rsa_key_chip error\n");
	//		return -1;
	//	}

	////2. RSA_Vchip(msg||ID||E`||C_otp) == S` 
#if 0
	SHA256_Init(&c);
	if (mInterParam.g_msgPos)
		SHA256_Update(&c, mInterParam.G_MSG, mInterParam.g_msgPos);
	//	SHA256_Update(&c,E_prime,16);
	SHA256_Update(&c, mInterParam.certRSA.ID, 16);

	//SHA256_Update(&c,C_otp,16);
	SHA256_Final(EM, &c);	//HASH(msg) 완성
#endif

	updateSignHash(mInterParam.ID, CERT_IDSIZE);
	getHashValue4Sign(EM);

	appendLog("    hash_for_sign data1 : ");
	B2Str(EM, 32);

	if (!VerifySign(mInterParam.Ep, mInterParam.certRSA.Nchip, EM, mInterParam.Sp)){

		return false;
	}
#if 0
	num = RSA_public_decrypt(256, S_prime, EM_prime, key_chip, RSA_NO_PADDING);

#if RSA_XOR
	RSA_xor_chip(S_prime, mInterParam.certRSA.Nchip, E_prime, EM_prime);
#endif


	RSA_free(key_chip);
	appendLog("    hash_for_sign data2 : ");
	B2Str(EM_prime + 256 - 32, 32);

	appendLog("    sign verified data : ");
	B2Str(EM_prime, 256);

	for (iFlagErr = 1;;)
	{
		if ((EM_prime[0] != 0) || (EM_prime[1] != 1))
			break;

		for (i = 2; i < (256 - 32 - 19 - 1); i++)
		if (EM_prime[i] != 0xFF)
			break;

		if (EM_prime[i] != 0)
			break;

		if (memcmp(EM_prime + i + 1, ID_SHA256, 19) != 0)
			break;

		if (memcmp(EM_prime + i + 20, EM, 32) != 0)
			break;

		iFlagErr = 0;
		break;
	}

	if (iFlagErr != 0)
	{
		errlog = " sign error\n";
		return false;
	}
#endif

	////3. D_ECB:k1(C_otp) == GenerateUserOTP()
	BYTE P_otp[16];	// 6byte || zero_padding

	D_ECB_K1(mInterParam.Cotp, P_otp, mInterParam.SEL_AES);
	/*
	AES_KEY AES_key;
	AES_set_decrypt_key(K1, 128, &AES_key);
	AES_decrypt(C_otp, P_otp, &AES_key);
	*/


	if (memcmp(mInterParam.OTP, P_otp, 3) != 0)
	{
		//errlog = "  OTP compare error\n";
		//return false;
	}

	////4. HK`_tsm = hash(K1||K2||IV)
	SHA256_CTX ck;
	SHA256_Init(&ck);
	SHA256_Update(&ck, mInterParam.K1, 16);
	SHA256_Update(&ck, mInterParam.K2, 16);
	SHA256_Update(&ck, mInterParam.IV, 16);
	SHA256_Final(mInterParam.HK_tsm, &ck);

	////5. store(HK`_tsm)
	//4번에서 처리

	//로그
	appendLog("    OTP : ");
	B2Str(P_otp, 3);
	appendLog("    HK`_TSM: ");
	B2Str(mInterParam.HK_tsm, 32);


	return true;



}
