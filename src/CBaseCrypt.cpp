//#include "main.h"
#include <string.h>

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
#include <math.h>
#include "Util.h"
#include "IRSD1410_DEFINE.h" 
#include"commfunc.h"
#include "neoCoLib.h"
#include "CBaseCrypt.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))  
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))  

//BYTE mInterParam.K1[16] = "12312";
//BYTE mInterParam.IV[16] = "123456";
//BYTE mInterParam.K2[16] = "123";
/**********************************************
5월 12일 이후 : 수정사항 
CTR의 mInterParam.IV 카운터를 64bit로 수정
1. encryptCTR
	BYTE mInterParam.IV[16]={0x00,};
	memcpy(mInterParam.IV,mInterParam.IV,8);

2. decryptCTR
	BYTE mInterParam.IV[16]={0x00,};
	memcpy(mInterParam.IV,mInterParam.IV,8);

3. int data2paddingdata_HMAC 함수 추가

*********************************************/

//BYTE HexCharToByte(char hexChar);
//char ByteToHexChar(BYTE byte);
//int StringtoByteArray(const char* str, BYTE* Barray, int Blen);
int ByteArraytoString(BYTE* Barray, char* str, int Blen);
BYTE* B2Str(BYTE* Barray,  int Blen, BYTE* str);

CBaseCrypt::CBaseCrypt(void)
{
	mInterParam.SEL_AES = 1;
	mInterParam.g_pinSize = DEFUNITSIZE;
	mInterParam.g_pinformSize = DEFUNITSIZE;

	mInterParam.G_PIN = (BYTE*)malloc(mInterParam.g_pinSize);
	mInterParam.G_INFORM = (BYTE*)malloc(mInterParam.g_pinformSize);
	mstrlog = "";

	memset(mInterParam.G_PIN, 0x00, 16);
	memset(mInterParam.G_INFORM, 0x00, 16);
	strcpy((char*)mInterParam.G_PIN, "456789");
	strcpy((char*)mInterParam.G_INFORM, "ABCDEF");

}

CBaseCrypt::~CBaseCrypt(void)
{
}

void CBaseCrypt::startLog()
{
	mstrlog = "";
}

void CBaseCrypt::appendLog(const char * fmt, ...)
{
	va_list args;



	va_start(args, fmt);
	string str = NCL::GetNewStrFormV(fmt, args);
	va_end(args);
	mstrlog += str;

}
string CBaseCrypt::flushLog()
{
	string ret = mstrlog;
	mstrlog = "";
	return ret;
}
string CBaseCrypt::getLog()
{
	return mstrlog;
}
void  CBaseCrypt:: B2Str(BYTE* Barray, int Blen){

	mstrlog += BytetoHexStr(Barray, Blen);
	mstrlog += "\r\n";
}

int CBaseCrypt::encryptSEEDCTR(BYTE* pbIn, int iLenIn, BYTE* pbOut, BYTE* pbCTR, unsigned int* dwNumCTR)
{
	//선언
	BYTE ctr[16]={0x00,};
	BYTE B_IV[16]={0x00,};
	memcpy(B_IV,pbCTR,8);		//++
    unsigned int dwNum = *dwNumCTR;
    int i, j;

	int denlen;
	EVP_CIPHER_CTX CTXd;
	EVP_CIPHER_CTX_init(&CTXd);
	EVP_CipherInit(&CTXd, EVP_seed_ecb(), mInterParam.K1, NULL, 1);
    for (i = 0; i < iLenIn; i += 16)
    {
        B_IV[12] = (BYTE)(dwNum >> 24);
        B_IV[13] = (BYTE)(dwNum >> 16);
        B_IV[14] = (BYTE)(dwNum >> 8);
        B_IV[15] = (BYTE)(dwNum);
		EVP_CipherUpdate(&CTXd, ctr, &denlen, B_IV, 16);

        for (j = i + 15; j >= i; j--)
            pbOut[j] = ctr[j & 15] ^ pbIn[j];
        dwNum++;
    }

	EVP_cleanup();
	EVP_CIPHER_CTX_cleanup(&CTXd);

    *dwNumCTR = dwNum;
	  
	return 0;
}
int CBaseCrypt::requestEHKdevice(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut)
{

	//REQ_TITLE(requestEHKdevice);
	//파싱
	appendLog("  input data : ");
	B2Str(pbIn, iLenIn);

	if (pbIn[0] != CMD_RES_EHK)
	{
		appendLog("  not RES_EHK : %02X\n", pbIn[0]);
		return -1;
	}
	if (iLenIn != 1 + 32)
	{
		appendLog("  length error\n");
		return -1;
	}
	//저장

	memcpy(getHKDevice(), pbIn + 1, CERT_EHKSIZE);
	//로그
	appendLog("    EHK`(device) : ");
	B2Str(getHKDevice(), 32);

	return 1;
}

void CBaseCrypt::resetSignHash(){
	SHA256_Init(&ctx4Sign);
}
void CBaseCrypt::updateSignHash(BYTE * msg,int size)
{
	SHA256_Update(&ctx4Sign, msg, size);
}
void CBaseCrypt::ViewMsgHash(BYTE * msg, int size)
{
	BYTE tmpem[32];
	SHA256_CTX c;
	SHA256_Init(&c);
	SHA256_Update(&c, msg, size);
	SHA256_Update(&c, mInterParam.ID, 16);
	SHA256_Final(tmpem, &c);
	appendLog("ViewMsgHash:");
	B2Str(tmpem, 32);

}

void CBaseCrypt::getHashValue4Sign(BYTE *EM)
{
	SHA256_Final(EM, &ctx4Sign);	//HASH(msg) 완성



}

int CBaseCrypt::encryptCTR(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut)
{
	//선언
	BYTE ctr[16]={0x00,};
	BYTE B_IV[16]={0x00,};
	memcpy(B_IV,mInterParam.IV,8);		//++
    unsigned int dwNum = 0;

    appendLog("  CTR in data : ");
	B2Str(pbIn	,iLenIn	);

	if (mInterParam.SEL_AES == 0)
    { //seed
        encryptSEEDCTR(pbIn, iLenIn, pbOut, mInterParam.IV, &dwNum);
    }
    else
    { //aes
	    //프로세싱
	    AES_KEY aes_key;
	    AES_set_encrypt_key(mInterParam.K1,128,&aes_key);

	    //ex_AesCTR
	    //AES_ctr128_encrypt(text, enc_text, 16, &pKey, ctrIV, ctr, &num);
	    AES_ctr128_encrypt(pbIn, pbOut, iLenIn, &aes_key, B_IV, ctr, &dwNum);
    }

    *pdwLenOut = iLenIn;
	  
	//memcpy(G_MSG + g_msgPos, pbIn, iLenIn);
    //g_msgPos += iLenIn;

	//로그
    appendLog("  CTR out data : ");
	B2Str(pbOut, iLenIn	);


	return 0;
}

int CBaseCrypt::encryptCCM(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut)
{
	//선언
	int i, j;

	BYTE ctr[16]= {0x00,};	
	//memcpy(ctr,0,16);
	unsigned int num = 0;
		
	BYTE CTR0[16]= {0x00,};	
	BYTE B0[16]= {0x00,};			

	BYTE YxorP[16];

	BYTE Y[16];	//CBC의 결과물
	appendLog("encryptCCM\r\n");
	appendLog("  CCM in data (LEN:%d):\r\n", iLenIn);
	B2Str(pbIn	,iLenIn	);

    //프로세싱
    ////step 1: B0
    B0[0] = 0x3F;
//	memcpy(B0+1, mInterParam.IV+1+8, 7);		//nonce
	memcpy(B0 + 1, mInterParam.IV + 1, 7);		//nonce
    B0[14] = (BYTE)(iLenIn >> 8);
    B0[15] = (BYTE)(iLenIn);


	if (mInterParam.SEL_AES == 0)
    { //seed
		int denlen;
		EVP_CIPHER_CTX CTXd;

		EVP_CIPHER_CTX_init(&CTXd);
		EVP_CipherInit(&CTXd, EVP_seed_ecb(), mInterParam.K1, NULL, 1);

	    ////step 2: Y0
        EVP_CipherUpdate(&CTXd, Y, &denlen, B0, 16);

	    ////step 3: Yn 	//CBC를 사용해 MAC생성 --> Y
	    //Msg block
    	
	    for(i=0; i < iLenIn/16; i++){
		    for(j=0; j<16; j++)	
                YxorP[j] = pbIn[i*16 + j]^Y[j];
            EVP_CipherUpdate(&CTXd, Y, &denlen, YxorP, 16);
	    }
    	
	    //last Msg block : zero padding
	    if(iLenIn & 15)
        {
		    for(j=0;j<16; j++)
            {
                if ((i*16 + j) < iLenIn)
                    YxorP[j] = pbIn[i*16 + j]^Y[j];
                else
                    YxorP[j] = Y[j];
            }
    		
            EVP_CipherUpdate(&CTXd, Y, &denlen, YxorP, 16);
	    }

	    ////step 5: CTR0	//CTR의 mInterParam.IV생성
	    CTR0[0]= 0x07;				
    //	memcpy(CTR0+1, mInterParam.IV+1+8, 7); //nonce
		memcpy(CTR0 + 1, mInterParam.IV + 1, 7); //nonce

	    ////step 6.7.8. :CTRn
	    //Tag 
        encryptSEEDCTR(Y, 16, pbOut+iLenIn, CTR0, &num);
	    //ctext
        encryptSEEDCTR(pbIn, iLenIn, pbOut, CTR0, &num);

		EVP_cleanup();
		EVP_CIPHER_CTX_cleanup(&CTXd);
    }
    else
    { //ase
	    AES_KEY aes_key;

	    AES_set_encrypt_key(mInterParam.K1,128,&aes_key);

	    ////step 2: Y0
	    AES_encrypt(B0, Y, &aes_key);	//CBC의 mInterParam.IV생성
    	
	    ////step 3: Yn 	//CBC를 사용해 MAC생성 --> Y
	    //Msg block
    	
	    for(i=0; i < iLenIn/16; i++){
		    for(j=0; j<16; j++)	
                YxorP[j] = pbIn[i*16 + j]^Y[j];
		    AES_encrypt(YxorP, Y, &aes_key);
	    }
    	
	    //last Msg block : zero padding
	    if(iLenIn & 15)
        {
		    for(j=0;j<16; j++)
            {
                if ((i*16 + j) < iLenIn)
                    YxorP[j] = pbIn[i*16 + j]^Y[j];
                else
                    YxorP[j] = Y[j];
            }
    		
		    AES_encrypt(YxorP, Y, &aes_key);
	    }

	    ////step 5: CTR0	//CTR의 mInterParam.IV생성
	    CTR0[0]= 0x07;				
    //	memcpy(CTR0+1, mInterParam.IV+1+8, 7); //nonce
	    memcpy(CTR0+1, mInterParam.IV+1, 7); //nonce

	    ////step 6.7.8. :CTRn
	    //Tag 
	    AES_ctr128_encrypt(Y, pbOut+iLenIn, 16, &aes_key, CTR0, ctr, &num); 
	    //ctext
	    AES_ctr128_encrypt(pbIn, pbOut, iLenIn,  &aes_key, CTR0, ctr, &num); 
    }

    *pdwLenOut = iLenIn + 16;

	//memcpy(G_MSG + g_msgPos, pbOut, *pdwLenOut);
   // g_msgPos += *pdwLenOut;
		
	appendLog("  CCM out data (LEN:%d):\r\n", *pdwLenOut);
	B2Str(pbOut, *pdwLenOut	);


	return 1;
}


int CBaseCrypt::decryptCCM(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut)
{
	//선언
	int i, j;
	int m_length = iLenIn  - 16;

	BYTE ctr[16]= {0x00,};	
	//memcpy(ctr,0,16);
	unsigned int num = 0;
		
	BYTE CTR0[16]= {0x00,};	
	BYTE B0[16]= {0x00,};	

	BYTE YxorP[16];

	BYTE T[16];
	BYTE Y[16];
	
	appendLog("decryptCCM\r\n");
	appendLog("CCM in data (LEN:%d):\r\n", iLenIn);
	B2Str(pbIn	,iLenIn	);

	//프로세싱
	////step 5: CTR0
	CTR0[0] = 0x07;				
	//memcpy(CTR0+1, mInterParam.IV+1+8, 7); //nonce
	memcpy(CTR0+1, mInterParam.IV+1, 7); //nonce

	if (mInterParam.SEL_AES == 0)
    { //seed
		int denlen;
		EVP_CIPHER_CTX CTXd;

		EVP_CIPHER_CTX_init(&CTXd);
		EVP_CipherInit(&CTXd, EVP_seed_ecb(), mInterParam.K1, NULL, 1);

	    ////step 6.7.8. :CTRn
	    //Tag 
        encryptSEEDCTR(pbIn + m_length, 16, T, CTR0, &num);
	    //ctext
        encryptSEEDCTR(pbIn, m_length, pbOut, CTR0, &num);

	    ////step 1: B0
	    B0[0] = 0x3F;
	    memcpy(B0+1, mInterParam.IV+1, 7);
	    //memcpy(B0+1, mInterParam.IV+1+8, 7);
	    B0[14] = (BYTE)(m_length >> 8);
	    B0[15] = (BYTE)(m_length);
    	
	    ////step 2: Y0
        EVP_CipherUpdate(&CTXd, Y, &denlen, B0, 16);
    	
	    ////step 3: Yn
		    //Msg block
    	
	    for(i=0; i < m_length/16; i++){
		    for(j=0; j<16; j++)	
                YxorP[j] = pbOut[i*16 + j]^Y[j];
            EVP_CipherUpdate(&CTXd, Y, &denlen, YxorP, 16);
	    }
    	
	    //last Msg block : zero padding
	    if(m_length & 15)
        {
		    for(j=0;j<16; j++)
            {
                if ((i*16 + j) < m_length)
                    YxorP[j] = pbOut[i*16 + j]^Y[j];
                else
                    YxorP[j] = Y[j];
            }
    		
            EVP_CipherUpdate(&CTXd, Y, &denlen, YxorP, 16);
	    }

	    ////step 6.7.8. :CTRn
	    //Tag 
        num = 0;
        memset(CTR0 + 8, 0, 8);
        encryptSEEDCTR(Y, 16, T, CTR0, &num);
    }
    else
    {//aes
	    AES_KEY aes_key;
	    AES_set_encrypt_key(mInterParam.K1, 128, &aes_key);

	    ////step 6.7.8. :CTRn
	    //Tag 
	    AES_ctr128_encrypt(pbIn + m_length, T, 16, &aes_key, CTR0, ctr, &num); 
	    //ctext
	    AES_ctr128_encrypt(pbIn, pbOut, m_length,  &aes_key, CTR0, ctr, &num); 

	    ////step 1: B0
	    B0[0] = 0x3F;
	    memcpy(B0+1, mInterParam.IV+1, 7);
	    //memcpy(B0+1, mInterParam.IV+1+8, 7);
	    B0[14] = (BYTE)(m_length >> 8);
	    B0[15] = (BYTE)(m_length);
    	
	    ////step 2: Y0
	    AES_encrypt(B0, Y, &aes_key);
    	
	    ////step 3: Yn
		    //Msg block
    	
	    for(i=0; i < m_length/16; i++){
		    for(j=0; j<16; j++)	
                YxorP[j] = pbOut[i*16 + j]^Y[j];
		    AES_encrypt(YxorP, Y, &aes_key);
	    }
    	
	    //last Msg block : zero padding
	    if(m_length & 15)
        {
		    for(j=0;j<16; j++)
            {
                if ((i*16 + j) < m_length)
                    YxorP[j] = pbOut[i*16 + j]^Y[j];
                else
                    YxorP[j] = Y[j];
            }
    		
		    AES_encrypt(YxorP, Y, &aes_key);
	    }

	    ////step 6.7.8. :CTRn
	    //Tag 
        num = 0;
        memset(CTR0 + 8, 0, 8);
	    AES_ctr128_encrypt(Y, T, 16, &aes_key, CTR0, ctr, &num); 
    }

	*pdwLenOut = m_length;

    if (memcmp(T, pbIn + m_length, 16) != 0)
    {
        appendLog("  CCM redundancy ERROR\n  calculated MAC : ");
	    B2Str(T, 16	);

        return -1;
    }
	
  // memcpy(G_MSG + g_msgPos, pbOut, *pdwLenOut);
  // g_msgPos += *pdwLenOut;
	appendLog("  CCM out data (LEN:%d):\r\n", *pdwLenOut);
	B2Str(pbOut, *pdwLenOut	);


	return 1;
}

int CBaseCrypt::cryptoCBC(BYTE *symkey, int lengthsymkey, BYTE* pbIn, int iLenIn, BYTE* pbOut, int* pdwLenOut, bool isEnc)
{
	BYTE ctr[16] = { 0x00, };
	BYTE B_IV[16] = { 0x00, };
	unsigned int dwNum = 0;

	//appendLog("  CBC in data : ");
	//B2Str(pbIn, iLenIn);
	AES_KEY aes_key;
	int enc = AES_ENCRYPT;

	if (isEnc)	{ //seed
		AES_set_encrypt_key(symkey, 128, &aes_key);
		enc = AES_ENCRYPT;
	}
	else{ //aes
		AES_set_decrypt_key(symkey, 128, &aes_key);
		enc = AES_DECRYPT;
	}
	AES_cbc_encrypt(pbIn, pbOut, iLenIn, &aes_key, B_IV, enc);
	*pdwLenOut = iLenIn;

	//memcpy(G_MSG + g_msgPos, pbIn, iLenIn);
	//g_msgPos += iLenIn;

	//로그
//	appendLog("  CBC out data : ");
//	B2Str(pbOut, iLenIn);


	return 1;
}

int CBaseCrypt::generateHMAC(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut)
{
	unsigned int hmac_length=32;
	HMAC(EVP_sha256(), mInterParam.K2, 16, (const unsigned char*)pbIn, iLenIn , pbOut, &hmac_length);
	*pdwLenOut = 32;

    appendLog("  HMAC in data : ");
	B2Str(pbIn	,iLenIn	);



	//memcpy(G_MSG + g_msgPos, pbOut, 32);
    //g_msgPos += 32;




    appendLog("  HMAC out data : ");
	B2Str(pbOut, *pdwLenOut	);
	

	return 1;
}
void CBaseCrypt::PARAM_Clear()
{
	memset(mInterParam.EHKDevice, 0x46, CERT_EHKSIZE / 2);
	memset(mInterParam.EHKDevice + 16, 0x00, CERT_EHKSIZE / 2);

}
int CBaseCrypt::data2paddingdata_AES(BYTE* data, int len)
{
	//hw로 ccm에 평문을 전송하기 전에 zero padding을 해주기 위한 함수
	
	//data 입력 데이터 포인터
	//len  데이터 길이(byte)

	//data에 패딩 데이터 추가 
	//return 패딩된 데이터 길이( 16*n byte )
	int left_len;
	left_len = 16 - len % 16;

	for(int i=0; i<left_len; i++)
	{
		*(data+len+i) = 0x00;
	}	
	return len + left_len;
}

int CBaseCrypt::data2paddingdata_HMAC(BYTE* data, int len)
{
	//hw로 hmac할 데이터를 전송하기 전에 패딩을 해주기 위한 함수
	
	//data 입력 데이터 포인터
	//len  데이터 길이(byte)

	//data에 패딩 데이터 추가 
	//return 패딩된 데이터 길이( 64*n byte )

	int left_len;
	int hmac_len;
	int i;
	
	hmac_len = len + 64;
	left_len = 64 - len%64;

	appendLog("  input data : ");
	B2Str(data	,len		);

	if(len%64 > 56){
        *(data+ len) = 0x80;
		for(i=1; i< left_len + 64; i++)
		{
			*(data+ len+i) = 0x00;
		}
		*(data + len + left_len +64 -4) = hmac_len >> 21;
		*(data + len + left_len +64 -3) = hmac_len >> 13;
		*(data + len + left_len +64 -2) = hmac_len >> 5;
		*(data + len + left_len +64 -1) = hmac_len << 3;
		
		appendLog("  padding data : ");
		B2Str(data	,len + left_len +64		);

		
		return len + left_len +64;
	}
	else{
        *(data+ len) = 0x80;
		for(i=1; i< left_len; i++)
		{
			*(data+ len+i) = 0x00;
		}
		*(data + len + left_len -4) = hmac_len >> 21;
		*(data + len + left_len -3) = hmac_len >> 13;
		*(data + len + left_len -2) = hmac_len >> 5;
		*(data + len + left_len -1) = hmac_len << 3;

		appendLog("  padding data : ");
		B2Str(data	,len + left_len		);


		return len + left_len;
	}
	
}

///// ECB 암호화(AES, SEED)

int CBaseCrypt::SelAesSeed(int sel_aes){
	if (sel_aes) mInterParam.SEL_AES = 0x01;
	else		mInterParam.SEL_AES = 0x00;
	return mInterParam.SEL_AES;
}

int CBaseCrypt::E_ECB_K1(BYTE* in, BYTE* out, BYTE sel_aes)
{
	if(sel_aes==0x01)
	{		
		AES_KEY AES_key;
		AES_set_encrypt_key(mInterParam.K1,128,&AES_key);
		AES_encrypt(in,out,&AES_key);
	}
	//SEED로 암호화
	else
	{
		EVP_CIPHER_CTX CTXe;
		EVP_CIPHER_CTX_init(&CTXe);
		int enclen;
		EVP_CipherInit(&CTXe, EVP_seed_ecb(), mInterParam.K1, NULL, 1);
		EVP_CipherUpdate(&CTXe, out, &enclen, in,16);
		EVP_cleanup();
		EVP_CIPHER_CTX_cleanup(&CTXe);
	}
	return 1;
}


int CBaseCrypt::D_ECB_K1(BYTE* in, BYTE* out, BYTE sel_aes)
{
	if(sel_aes==0x01)
	{		
		AES_KEY AES_key;
		AES_set_decrypt_key(mInterParam.K1,128,&AES_key);
		AES_decrypt(in,out,&AES_key);
	}
	//SEED로 암호화
	else
	{
		EVP_CIPHER_CTX CTXd;
		EVP_CIPHER_CTX_init(&CTXd);
		int denlen;
		EVP_CipherInit(&CTXd, EVP_seed_ecb(), mInterParam.K1, NULL, 0);
		EVP_CipherUpdate(&CTXd, out, &denlen, in,16);
		EVP_cleanup();
		EVP_CIPHER_CTX_cleanup(&CTXd);
	}
	return 1;
}
#if 0

void CBaseCrypt::ISSUEfromString(const char * hsId, const char * hsqChip)
{



	neocolib::VECBYTE vecbyteid = HexStr2Byte(hsId);
	neocolib::VECBYTE vecbyte = HexStr2Byte(hsqChip);
	ISSUE(&vecbyteid[0], vecbyteid.size(), &vecbyte[0], vecbyte.size());

}

#endif // 0

void CBaseCrypt::bytecpy(BYTE * pdst, int dstSize, BYTE *psrc, int srcSize){
	memset(pdst, 0x00, dstSize);
	memcpy(pdst, psrc, MIN(dstSize, srcSize));

}
#if 1

void CBaseCrypt::SetPINnINFORM(BYTE * pPin, int pinSize, BYTE *pinform, int informSize)
{
	if (mInterParam.G_PIN) free(mInterParam.G_PIN);
	if (mInterParam.G_INFORM) free(mInterParam.G_INFORM);

	mInterParam.G_PIN = (BYTE*)malloc(pinSize);
	mInterParam.G_INFORM = (BYTE*)malloc(informSize);

	mInterParam.g_pinSize = pinSize;
	mInterParam.g_pinformSize = informSize;

	bytecpy(mInterParam.G_PIN, mInterParam.g_pinSize, pPin, pinSize);
	bytecpy(mInterParam.G_INFORM, mInterParam.g_pinformSize, pinform, informSize);

}


#endif // 0


INTER_PARAM * CBaseCrypt::GetInterParam(){
	return &mInterParam;
}
string CBaseCrypt::viewCETEXT(){
	string str = "";

	str += "SDATA:";
	str += BytetoHexStr(mInterParam.SDATA, CERT_SDATASIZE);
	str += "\n";
	str += "EHK:";
	str += BytetoHexStr(mInterParam.EHKDevice, CERT_EHKSIZE);
	str += "\n";

	return str;

}

void CBaseCrypt::UpdateSDATA()
{
	//requestIdRSA


	BYTE *SDATA = mInterParam.SDATA;

	Gen_SDATA(SDATA, mInterParam.ID, mInterParam.G_PIN, mInterParam.G_INFORM);

}
/*

int genHMAC(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut)
{
	//선언
	BYTE *len;
	int length;
	BYTE *msg;
	unsigned int hmac_length=32;
	
	//파싱
	if(pbIn[0] != CMD_REQ_GENERATE_MAC) return -1;
	len = pbIn+1;
	msg = pbIn+1+2;
	length = *len;
	length = length <<8;
	length += *(len+1);
	
	//프로세싱
	//ex hmac
	//HMAC(EVP_sha256(), key, 16, (const unsigned char*)msg, 3, md, &len);
	HMAC(EVP_sha256(),mInterParam.K2,16, (const unsigned char*)msg, length, pbOut+1,&hmac_length);

	//머징
	pbOut[0] = CMD_RES_GEN_MAC;
	*pdwLenOut = 33;
	
	//로그
	B2Str(msg		,length		);
	B2Str(pbOut+1	,32			);
	*(pbLog) = NULL;

	return 0;
}

int verifyHMAC(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut)
{
	//선언
	BYTE *len;
	int length;
	BYTE *msg;
	BYTE *hmac;
	BYTE hmac_prime[32];
	unsigned int hmac_length=32;
	BYTE result=0x00;
	
	//파싱
	if(pbIn[0] != CMD_REQ_VERIFY_MAC) return -1;
	len = pbIn+1;
	msg = pbIn+1+2;
	length = *len;
	length = length <<8;
	length += *(len+1);

	hmac= pbIn+1+2+length;
	
	//프로세싱
	//ex hmac
	//HMAC(EVP_sha256(), key, 16, (const unsigned char*)msg, 3, md, &len);
	HMAC(EVP_sha256(),mInterParam.K2,16, (const unsigned char*)msg, length, hmac_prime,&hmac_length);
	
	for(int i =0; i<32; i++)
	{
		if(hmac_prime[i] != hmac[i]) result = 0x01;
	}

	//머징
	pbOut[0] = CMD_RES_VERIFY_MAC;
	pbOut[1] = result;
	*pdwLenOut = 2;

	//로그
	return 0;
}
*/
