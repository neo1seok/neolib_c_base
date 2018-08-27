#include <stdarg.h>
#include <vector>
#include"Util.h"
#include "neoCoLib.h"
#ifndef __NOTUSEOPENSSL__
//for(it_type iterator = m.begin(); iterator != m.end(); iterator++) {	}
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

#include <openssl/x509.h>
#endif //ifndef __NOTUSEOPENSSL__


MAPSTRKEVLAUE getMAPfromARGS(int argc,char *argv[])
{

	std::map<string,string> mapcmd;

	for(int i = 0 ; i < argc ; i++){
		string pcmd = argv[i];
		char * pvalue = argv[i+1];
		string value = pvalue ?pvalue:"";
		
		if(pcmd[0] == '-')
		{
			mapcmd[pcmd] = (value[0] != '-')? value :"";
		}
	}

	return mapcmd;
}

string findValueFormMAP(string key,std::map<string,string> & mapcmd,bool * pbool)
{
	ITER_MAPSTRKEVLAUE fi = mapcmd.find(key);
	if(pbool) *pbool = false;
	if(fi == mapcmd.end()){
		return "";;
		

	}
	if(pbool) *pbool = true;;
	return fi->second;

}

int char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
//  throw Exception('Invalid input string');
  return 0;
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large

void PrintViewLine(const char * fmt, ...)
{
	va_list args;
	


	va_start(args, fmt);
	p_circular_tstr str = NCL::GetNewStrFormV(fmt,args);
	va_end(args);

	PrintView("%s\n",str.c_str());
	

	//return str;
}

void PrintView(const char * fmt, ...)
{

#ifdef NEODEBUG
	va_list args;
	


	va_start(args, fmt);
	p_circular_tstr str = NCL::GetNewStrFormV(fmt,args);
	va_end(args);

	printf(str.c_str());

	FILE * fp = fopen("out.txt","ab");
	fprintf(fp,str.c_str());
	fclose(fp);
#endif
	//return str;
}

int hex2bin(const char* src, unsigned char* target)
{
	unsigned char * pst = target;
	while (*src && src[1])
	{
		*(target++) = char2int(*src) * 16 + char2int(src[1]);
		src += 2;
	}

	return (int)(target - pst);
}

string BytetoHexStr(const  unsigned char * Barray, int Blen)
{
	string str = "";
	int i;
	int j = 0;
	for (i = 0; i < Blen; i++){
		str += (Barray[i] >= 0xA0) ? ((Barray[i] >> 4) + ('A' - 10)) : (Barray[i] >> 4) + '0';
		str += ((Barray[i] & 15) >= 10) ? ((Barray[i] & 15) + ('A' - 10)) : (Barray[i] & 15) + '0';
	}
	return str;

}

neocolib::VECBYTE  HexStr2Byte(const string & str)
{
	neocolib::VECBYTE ret;
	char * src = (char*)str.c_str();

	while (*src && src[1])
	{
		neocolib::nbyte aaa = char2int(*src++) * 16 + char2int(*src++);
		ret.push_back(aaa);

	}
	return ret;

}

int StringtoByteArray(const  char* str, unsigned char* Barray, int Blen)
{	
	//str length = BArray length * 2
	int i;
    int j;
    unsigned long dwRet = 0;
    for (i = 0, j = 0; i < Blen; i++)
    {
        if ((unsigned long)(str[i] - '0') <= ('9' - '0'))
            dwRet += (unsigned long)(str[i] - '0');
        else if ((unsigned long)(str[i] - 'A') <= ('F' - 'A'))
            dwRet += (unsigned long)(str[i] + 10 - 'A');
        else if ((unsigned long)(str[i] - 'a') <= ('f' - 'a'))
            dwRet += (unsigned long)(str[i] + 10 - 'a');
        else
            continue;

        if (j & 1)
        { //odd
            Barray[j / 2] = (unsigned char)dwRet;
            dwRet = 0;
        }

        j++;

        dwRet <<= 4;
    }

    return 0;
}

int ByteArraytoString(unsigned char* Barray, char* str, int Blen)
{
	int i;
	int j=0;
	for (i = 0; i < Blen; i++){
        str[j++] = (Barray[i] >= 0xA0) ? ((Barray[i] >> 4) + ('A' - 10)) : (Barray[i] >> 4) + '0';
		str[j++] = ((Barray[i] & 15) >= 10) ? ((Barray[i] & 15) + ('A' - 10)) : (Barray[i] & 15) + '0';
	}
	str[j] = NULL;
	return 0;
}


unsigned char* B2Str(unsigned char* Barray,  int Blen, unsigned char* str)
{
	int i;
	int j=0;
	for (i = 0; i < Blen; i++){
        str[j++] = (Barray[i] >= 0xA0) ? ((Barray[i] >> 4) + ('A' - 10)) : ((Barray[i] >> 4) + '0');
		str[j++] = ((Barray[i] & 15) >= 10) ? ((Barray[i] & 15) + ('A' - 10)) : ((Barray[i] & 15) + '0');
	}
	str[j] = 0x0A;
	str[j+1] = 0;
	return str + j + 1;
}



int CheckSNisValid(unsigned char* SN)
{	
	//SN의 유효성을 검사
	//나중에 구현
	return 1;
}

#ifndef __NOTUSEOPENSSL__

int Gen_SDATA(unsigned char* SDATA)
{
	// SDATA	32byte

	char *msg = "SDATA";
	//SHA256((const unsigned char*)msg,20,SDATA);
	
#if PRINTF
	printf("\nGEN_SDATA\n");
	for(int i=0 ; i<32 ; i++)	printf("%02x", SDATA[i]);	
	printf("\n");
#endif
	return 0;
}
int Gen_SDATA(unsigned char *SDATA, unsigned char *ID,unsigned char *PIN, unsigned char *INFORM)
{
	// SDATA	32byte
	// ID		16byte
	// PIN		16byte
	//INFORM	16byte (Temp)
	
	SHA256_CTX c;
	SHA256_Init(&c);
	SHA256_Update(&c,ID,16);
	SHA256_Update(&c,PIN,16);
	SHA256_Update(&c,INFORM,16);
	SHA256_Final(SDATA,&c);

#if 0
	printf("\nGEN_SDATA\n");
	for(int i=0 ; i<32 ; i++)	printf("%02x", SDATA[i]);	
	printf("\n");
#endif
	return 0;
}

int Gen_SDATA(unsigned char *SDATA, unsigned char *ID, unsigned char *PIN, int pinSize,unsigned char *INFORM,int informSize)
{
	// SDATA	32byte
	// ID		16byte
	// PIN		16byte
	//INFORM	16byte (Temp)

	SHA256_CTX c;
	SHA256_Init(&c);
	SHA256_Update(&c, ID, 16);
	SHA256_Update(&c, PIN, pinSize);
	SHA256_Update(&c, INFORM, informSize);
	SHA256_Final(SDATA, &c);

#if 0
	printf("\nGEN_SDATA\n");
	for (int i = 0; i<32; i++)	printf("%02x", SDATA[i]);
	printf("\n");
#endif
	return 0;
}


string GetSHA256Hexstr(const void *org, int orgSize)
{
	// SDATA	32byte
	// ID		16byte
	// PIN		16byte
	//INFORM	16byte (Temp)
	unsigned char SDATA[SHA256_DIGEST_LENGTH];

	SHA256_CTX c;
	SHA256_Init(&c);
	SHA256_Update(&c, org, orgSize);
	SHA256_Final(SDATA, &c);

	return NCL::BytetoHexStr(SDATA, SHA256_DIGEST_LENGTH);

}
string GetSHA256(const char * hexMsg)
{
	VECBYTE vecMsg = NCL::HexStr2Byte(hexMsg);

	if (vecMsg.size() == 0) return "";

	unsigned char SDATA[SHA256_DIGEST_LENGTH];

	SHA256_CTX c;
	SHA256_Init(&c);
	SHA256_Update(&c, V2A(vecMsg), vecMsg.size());
	SHA256_Final(SDATA, &c);


	return NCL::BytetoHexStr(SDATA, SHA256_DIGEST_LENGTH);


}

string GetHMAC(const string & hexMsg, const string & hexKey)
{
	VECBYTE vecMsg = NCL::HexStr2Byte(hexMsg);
	VECBYTE vecKey = NCL::HexStr2Byte(hexKey);

	if (vecMsg.size() == 0 || vecKey.size() == 0) return "";
	
	
	unsigned int sizeMD = 0;
	unsigned char * hmac = HMAC(EVP_sha256(), V2A(vecKey), vecKey.size(), V2A(vecMsg), vecMsg.size(), NULL, &sizeMD);
	
	return NCL::BytetoHexStr(hmac, sizeMD);
	

}




int Bn2Bin(const BIGNUM * pbignum,unsigned char *pbuff,int bytesize)
{
	int oldbytsize = BN_num_bytes(pbignum);

	if(oldbytsize > bytesize ) return 0;
	memset(pbuff,0x00,bytesize);

	int startpos = bytesize - oldbytsize;
	int sizee = BN_bn2bin(pbignum,pbuff+startpos);
	return bytesize;
}
string BN2string(BIGNUM *bn){

	string ret;

	char * tmp = BN_bn2hex(bn);
	if (tmp){
		ret = tmp;
		OPENSSL_free(tmp);
	}

	return ret;

}

int CalcRSAFactorsBINNUM(BIGNUM *bnP, BIGNUM *bnQ, const BIGNUM *bnN, const BIGNUM *bnE, const BIGNUM *bnD)
{
	int ret = 0;
	BIGNUM *bnR, *bnTwo, *bnTemp;
	BIGNUM *bnG, *bnX, *bnY, *bnN_1;
	BIGNUM *bnY_1;
	int t = 0;
	bool skip;
	bool find = false;

	BN_CTX *ctx = BN_CTX_new();
	BN_CTX_start(ctx);
	


	BIGNUM *bnK = BN_CTX_get(ctx);
	BIGNUM *bnOne = BN_CTX_get(ctx);

	BN_one(bnOne);

	BN_mul(bnK, bnE, bnD, ctx);
	BN_sub(bnK, bnK, bnOne); //k=de-1

	if (BN_is_odd(bnK) == 1) //홀수 일 경우 prime factor를 찾을 수 없음 
	{
		ret = -1;
		goto END;
		//return -1;
	}


	//k = 2^t*r 형태로 분해
	
	bnR = BN_CTX_get(ctx);
	bnTwo = BN_CTX_get(ctx);
	bnTemp = BN_CTX_get(ctx);
	BN_set_word(bnTwo, 2);

	BN_copy(bnR, bnK);
	while (BN_is_odd(bnR) == 0)
	{
		BN_div(bnR, bnTemp, bnR, bnTwo, ctx);
		t++;
	}


	
	bnG = BN_CTX_get(ctx);
	bnX = BN_CTX_get(ctx);
	bnY = BN_CTX_get(ctx);
	bnN_1 = BN_CTX_get(ctx); //N-1저장


	//BIGNUM *bnP = BN_CTX_get(ctx);
	//BIGNUM *bnQ = BN_CTX_get(ctx);

	BN_sub(bnN_1, bnN, bnOne);

	for (int i = 0; i < 100; i++)
	{
		skip = false;
		BN_rand_range(bnG, bnN); //0~N-1인 정수 생성
		BN_mod_exp(bnY, bnG, bnR, bnN, ctx);// y = g^r mod n
		if ((BN_cmp(bnY, bnOne) == 0) || (BN_cmp(bnY, bnN_1) == 0))
			continue;

		for (int j = 0; j < t - 1; j++)
		{
			BN_mod_exp(bnX, bnY, bnTwo, bnN, ctx); // x = y^2 mod N
			if (BN_cmp(bnX, bnOne) == 0)
			{
				find = true;
				skip = true;
				i = 100; //i에 대한 loop도 빠져 나가기 위함
				break;
			}
			if (BN_cmp(bnX, bnN_1) == 0)
			{
				skip = true;
				break;
			}
			BN_copy(bnY, bnX);
		}

		if (!skip)
		{
			BN_mod_exp(bnX, bnY, bnTwo, bnN, ctx); //x = y^2 mod N
			if (BN_cmp(bnX, bnOne) == 0)
			{
				find = true;
				break;
			}
		}
	}

	if (!find){
		ret = -1;
		goto END;

	}

	bnY_1 = BN_new(); //Y-1저장

	BN_sub(bnY_1, bnY, bnOne);
	BN_gcd(bnP, bnY_1, bnN, ctx);
	BN_div(bnQ, bnTemp, bnN, bnP, ctx);

	BN_clear_free(bnY_1);

	

	if (BN_cmp(bnP, bnQ) < 0)
	{
		BN_swap(bnP, bnQ);
	}


END:
	BN_CTX_end(ctx);
	BN_CTX_free(ctx);

	//RSA_free(key);

	return ret;


}



int CalcRSACRTParameterwithPQDPIGNUM(BIGNUM *bnDp, BIGNUM *bnDq, BIGNUM *bnQInv, const BIGNUM *bnP, const BIGNUM *bnQ, const BIGNUM *bnD)
{
	int result = 0;

	BN_CTX *ctx = BN_CTX_new();
	BN_CTX_start(ctx);

	BIGNUM *newbnP = BN_dup(bnP);
	BIGNUM *newbnQ = BN_dup(bnQ);

	BN_mod_inverse(bnQInv, bnQ, bnP, ctx);
	if (bnQInv != NULL)
	{
		BN_sub_word(newbnP, 1);
		BN_mod(bnDp, bnD, newbnP, ctx);

		BN_sub_word(newbnQ, 1);
		BN_mod(bnDq, bnD, newbnQ, ctx);


		//result = 1;
	}
	else {
		result = -1;
	}

	BN_CTX_end(ctx);
	BN_CTX_free(ctx);
	BN_free(newbnP);
	BN_free(newbnQ);

	return result;
}


int CalcRSAFactors( RSA_HEXSTR & rsaHexStr)
{
	int result;
	RSA *key;
	BN_CTX *ctx = BN_CTX_new();
	BN_CTX_start(ctx);

	//입력 String-> Hexa Stream->BIGNUM 으로 변환
	BIGNUM *bnN = BN_CTX_get(ctx);
	BIGNUM *bnE = BN_CTX_get(ctx);
	BIGNUM *bnD = BN_CTX_get(ctx);

	BIGNUM *bnP = BN_CTX_get(ctx);
	BIGNUM *bnQ = BN_CTX_get(ctx);
	BIGNUM *bnDp = BN_CTX_get(ctx);
	BIGNUM *bnDq = BN_CTX_get(ctx);
	BIGNUM *bnQInv = BN_CTX_get(ctx);

	BN_hex2bn(&bnN, rsaHexStr.strN.c_str());
	BN_hex2bn(&bnE, rsaHexStr.strE.c_str());
	BN_hex2bn(&bnD, rsaHexStr.strD.c_str());



	result = CalcRSAFactorsBINNUM(bnP, bnQ, bnN, bnE, bnD);
	if (result == -1) {
		goto END;
	}

	result = CalcRSACRTParameterwithPQDPIGNUM(bnDp, bnDq, bnQInv, bnP, bnQ, bnD);
	if (result == -1) {
		goto END;
	}



	//goto END;

#if 0
	//NIST SP 800-56B: Recommendation for Pair-Wise Key Establishment Schemes
	//       Using Integer Factorization Cryptography
	//       August 2009
	// Appendix C: Prime Factor Recovery
	//k = ed - 1
	BIGNUM *bnK = BN_CTX_get(ctx);
	BIGNUM *bnOne = BN_CTX_get(ctx);

	BN_one(bnOne);

	BN_mul(bnK, bnE, bnD, ctx);
	BN_sub(bnK, bnK, bnOne); //k=de-1

	if (BN_is_odd(bnK) == 1) //홀수 일 경우 prime factor를 찾을 수 없음 
	{
		BN_CTX_end(ctx);
		BN_CTX_free(ctx);

		return -1;
	}


	//k = 2^t*r 형태로 분해
	int t = 0;
	BIGNUM *bnR = BN_CTX_get(ctx);
	BIGNUM *bnTwo = BN_CTX_get(ctx);
	BIGNUM *bnTemp = BN_CTX_get(ctx);
	BN_set_word(bnTwo, 2);

	BN_copy(bnR, bnK);
	while (BN_is_odd(bnR) == 0)
	{
		BN_div(bnR, bnTemp, bnR, bnTwo, ctx);
		t++;
	}


	bool skip;
	bool find = false;
	BIGNUM *bnG = BN_CTX_get(ctx);
	BIGNUM *bnX = BN_CTX_get(ctx);
	BIGNUM *bnY = BN_CTX_get(ctx);
	BIGNUM *bnN_1 = BN_CTX_get(ctx); //N-1저장


	BIGNUM *bnP = BN_CTX_get(ctx);
	BIGNUM *bnQ = BN_CTX_get(ctx);

	BN_sub(bnN_1, bnN, bnOne);

	for (int i = 0; i < 100; i++)
	{
		skip = false;
		BN_rand_range(bnG, bnN); //0~N-1인 정수 생성
		BN_mod_exp(bnY, bnG, bnR, bnN, ctx);// y = g^r mod n
		if ((BN_cmp(bnY, bnOne) == 0) || (BN_cmp(bnY, bnN_1) == 0))
			continue;

		for (int j = 0; j < t - 1; j++)
		{
			BN_mod_exp(bnX, bnY, bnTwo, bnN, ctx); // x = y^2 mod N
			if (BN_cmp(bnX, bnOne) == 0)
			{
				find = true;
				skip = true;
				i = 100; //i에 대한 loop도 빠져 나가기 위함
				break;
			}
			if (BN_cmp(bnX, bnN_1) == 0)
			{
				skip = true;
				break;
			}
			BN_copy(bnY, bnX);
		}

		if (!skip)
		{
			BN_mod_exp(bnX, bnY, bnTwo, bnN, ctx); //x = y^2 mod N
			if (BN_cmp(bnX, bnOne) == 0)
			{
				find = true;
				break;
			}
		}
	}

	if (find)
	{
		BIGNUM *bnY_1 = BN_new(); //Y-1저장

		BN_sub(bnY_1, bnY, bnOne);
		BN_gcd(bnP, bnY_1, bnN, ctx);
		BN_div(bnQ, bnTemp, bnN, bnP, ctx);

		BN_clear_free(bnY_1);

	}
	else
	{
		BN_CTX_end(ctx);
		BN_CTX_free(ctx);
		return -1;
	}
#endif
	//key validity 검사 //////////////////
	key = RSA_new();
	key->n = BN_new();
	key->e = BN_new();
	key->d = BN_new();
	key->p = BN_new();
	key->q = BN_new();
	BN_copy(key->n, bnN);
	BN_copy(key->e, bnE);
	BN_copy(key->d, bnD);
	BN_copy(key->p, bnP);
	BN_copy(key->q, bnQ);


	if (RSA_check_key(key) != 1)
	{
		
		result = -1;
		RSA_free(key);
		goto END;
	}

	rsaHexStr.strP = BN2string(bnP);
	rsaHexStr.strQ = BN2string(bnQ);
	rsaHexStr.strDp = BN2string(bnDp);
	rsaHexStr.strDq = BN2string(bnDq);
	rsaHexStr.strQInv = BN2string(bnQInv);


	result = 1;


	RSA_free(key);

END:
	BN_CTX_end(ctx);
	BN_CTX_free(ctx);



	return result;
}

void Extended_BinaryGcd(BIGNUM* bnG, BIGNUM* bnX, BIGNUM* bnY, const BIGNUM* bnA, const BIGNUM* bnB)
{
	BN_CTX *ctx = BN_CTX_new();
	BN_CTX_start(ctx);

	BIGNUM *bnAA = BN_CTX_get(ctx);
	BIGNUM *bnBB = BN_CTX_get(ctx);
	BIGNUM *bnU = BN_CTX_get(ctx);
	BIGNUM *bnV = BN_CTX_get(ctx);
	BIGNUM *bnX1 = BN_CTX_get(ctx);
	BIGNUM *bnX2 = BN_CTX_get(ctx);
	BIGNUM *bnY1 = BN_CTX_get(ctx);
	BIGNUM *bnY2 = BN_CTX_get(ctx);
	BIGNUM *bnE = BN_CTX_get(ctx); //공약수 중 짝수 인자 저장

	BN_one(bnE);
	BN_copy(bnAA, bnA);
	BN_copy(bnBB, bnB);

	while (!BN_is_odd(bnAA) && !BN_is_odd(bnBB))
	{
		BN_rshift1(bnAA, bnAA);
		BN_rshift1(bnBB, bnBB);
		BN_lshift1(bnE, bnE);
	}

	BN_copy(bnU, bnAA);
	BN_copy(bnV, bnBB);
	BN_one(bnX1);
	BN_zero(bnY1);
	BN_zero(bnX2);
	BN_one(bnY2);

	while (1)
	{
		while (!BN_is_odd(bnU))
		{
			BN_rshift1(bnU, bnU);
			if (!BN_is_odd(bnX1) && !BN_is_odd(bnY1))
			{
				BN_rshift1(bnX1, bnX1);
				BN_rshift1(bnY1, bnY1);
			}
			else
			{
				BN_add(bnX1, bnX1, bnBB);
				BN_rshift1(bnX1, bnX1);

				BN_sub(bnY1, bnY1, bnAA);
				BN_rshift1(bnY1, bnY1);
			}
		}

		while (!BN_is_odd(bnV))
		{
			BN_rshift1(bnV, bnV);
			if (!BN_is_odd(bnX2) && !BN_is_odd(bnY2))
			{
				BN_rshift1(bnX2, bnX2);
				BN_rshift1(bnY2, bnY2);
			}
			else
			{
				BN_add(bnX2, bnX2, bnBB);
				BN_rshift1(bnX2, bnX2);

				BN_sub(bnY2, bnY2, bnAA);
				BN_rshift1(bnY2, bnY2);
			}
		}

		if (BN_cmp(bnU, bnV) >= 0)
		{
			BN_sub(bnU, bnU, bnV);
			BN_sub(bnX1, bnX1, bnX2);
			BN_sub(bnY1, bnY1, bnY2);
		}
		else
		{
			BN_sub(bnV, bnV, bnU);
			BN_sub(bnX2, bnX2, bnX1);
			BN_sub(bnY2, bnY2, bnY1);
		}

		if (BN_is_zero(bnU))
		{
			BN_copy(bnX, bnX2);
			BN_copy(bnY, bnY2);
			BN_mul(bnG, bnE, bnV, ctx);
			break;
		}
	}

	BN_CTX_end(ctx);
	BN_CTX_free(ctx);
}

int RSACRTKeyToNomalKey(BIGNUM* bnN, BIGNUM* bnE, BIGNUM* bnD,
	const BIGNUM* bnP, const BIGNUM* bnQ, const BIGNUM* bnDp, const BIGNUM* bnDq)
{
	int result = -1;
	BN_CTX *ctx = BN_CTX_new();
	BN_CTX_start(ctx);

	BIGNUM *bn_P_1 = BN_CTX_get(ctx);
	BIGNUM *bn_Q_1 = BN_CTX_get(ctx);
	BIGNUM *bn_Dp_Dq = BN_CTX_get(ctx);
	BIGNUM *bn_Dp = BN_CTX_get(ctx);
	BIGNUM *bn_Dq = BN_CTX_get(ctx);
	//BIGNUM *bn_Temp = BN_CTX_get(ctx);
	BIGNUM *bn_X = BN_CTX_get(ctx);
	BIGNUM *bn_Y = BN_CTX_get(ctx);
	BIGNUM *bn_Phi = BN_CTX_get(ctx);
	BIGNUM *bn_U = BN_CTX_get(ctx);
	BIGNUM *bn_V = BN_CTX_get(ctx);
	BIGNUM *bn_G = BN_CTX_get(ctx);
	BIGNUM *bn_N = BN_CTX_get(ctx);
	BIGNUM *bn_E = BN_CTX_get(ctx);
	BIGNUM *bn_D = BN_CTX_get(ctx);
	BIGNUM *bn_One = BN_CTX_get(ctx);

	BN_one(bn_One);

	if (BN_cmp(bnDp, bnDq) >= 0)
	{
		BN_copy(bn_Dp, bnDp);
		BN_copy(bn_Dq, bnDq);
		BN_sub(bn_P_1, bnP, bn_One);
		BN_sub(bn_Q_1, bnQ, bn_One);
	}
	else
	{
		BN_copy(bn_Dp, bnDq);
		BN_copy(bn_Dq, bnDp);
		BN_sub(bn_P_1, bnQ, bn_One);
		BN_sub(bn_Q_1, bnP, bn_One);
	}



	Extended_BinaryGcd(bn_G, bn_U, bn_V, bn_P_1, bn_Q_1);
	BN_sub(bn_Dp_Dq, bn_Dp, bn_Dq);
	BN_div(bn_X, NULL, bn_Dp_Dq, bn_G, ctx);

	BN_mul(bn_X, bn_X, bn_U, ctx);
	BN_mul(bn_X, bn_X, bn_P_1, ctx);
	BN_sub(bn_X, bn_Dp, bn_X);

	BN_mul(bn_Phi, bn_P_1, bn_Q_1, ctx);
	BN_div(bn_Y, NULL, bn_Phi, bn_G, ctx);

	BN_mod(bn_X, bn_X, bn_Y, ctx);
	if (!BN_is_zero(bn_X) && BN_is_negative(bn_X))
		BN_add(bn_X, bn_X, bn_Y);

	BN_mod_inverse(bn_E, bn_X, bn_Y, ctx);
	BN_mod_inverse(bn_D, bn_E, bn_Phi, ctx);

	BN_mul(bn_N, bnP, bnQ, ctx);

	if (!BN_is_zero(bn_D) && !BN_is_negative(bnD))
	{
		BN_mod(bn_X, bn_D, bn_P_1, ctx);
		if (BN_cmp(bn_X, bn_Dp) == 0)
		{
			BN_copy(bnN, bn_N);
			BN_copy(bnE, bn_E);
			BN_copy(bnD, bn_D);
			result = 1;
			/*if (CheckRsaSfmKey(bn_N, bn_E, bn_D))
			{
			result = 1;

			}*/

		}
	}

	BN_CTX_end(ctx);
	BN_CTX_free(ctx);
	return result;
}




int CalcRSAParametersType3(string& strN, string& strE, string& strD,	const string & strP, const string & strQ, const string & strDp, const string & strDq, const string & strQInv)
{
	BN_CTX	*ctx = BN_CTX_new();
	BN_CTX_start(ctx);

	BIGNUM *bnN = BN_CTX_get(ctx);
	BIGNUM *bnP = BN_CTX_get(ctx);
	BIGNUM *bnQ = BN_CTX_get(ctx);
	BIGNUM *bnE = BN_CTX_get(ctx);
	BIGNUM *bnD = BN_CTX_get(ctx);
	BIGNUM *bnDp = BN_CTX_get(ctx);
	BIGNUM *bnDq = BN_CTX_get(ctx);
	BIGNUM *bnQInv = BN_CTX_get(ctx);


	BN_hex2bn(&bnP, strP.c_str());;
	BN_hex2bn(&bnQ, strQ.c_str());;

	BN_hex2bn(&bnDp, strDp.c_str());
	BN_hex2bn(&bnDq, strDq.c_str());
	BN_hex2bn(&bnQInv, strQInv.c_str());

	

	BIGNUM *bnOne = BN_CTX_get(ctx);
	BIGNUM *bnX = BN_CTX_get(ctx);
	BIGNUM *bnP_1 = BN_CTX_get(ctx); //p-1
	BIGNUM *bnQ_1 = BN_CTX_get(ctx); //q-1
	BIGNUM *bnDp_Dq = BN_CTX_get(ctx); //dp-dq
	BIGNUM *bnInv = BN_CTX_get(ctx);
	BIGNUM *bnGcd = BN_CTX_get(ctx); //gcd(q-1, p-1)

	BIGNUM *bnU = BN_CTX_get(ctx);
	BIGNUM *bnV = BN_CTX_get(ctx);
	BIGNUM *bnR = BN_CTX_get(ctx);
	BIGNUM *bnK = BN_CTX_get(ctx);
	BIGNUM *bnL = BN_CTX_get(ctx);
	BN_one(bnOne);


	RSA *key = RSA_new();
	key->p = BN_new();
	key->q = BN_new();
	key->n = BN_new();
	key->e = BN_new();
	key->d = BN_new();
	key->dmp1 = BN_new();
	key->dmq1 = BN_new();
	key->iqmp = BN_new();

	BN_copy(key->p, bnP);
	BN_copy(key->q, bnQ);
	BN_copy(key->dmp1, bnDp);
	BN_copy(key->dmq1, bnDq);
	BN_copy(key->iqmp, bnQInv);

	int result = -1;

	int temp = RSACRTKeyToNomalKey(bnN, bnE, bnD, bnP, bnQ, bnDp, bnDq);
	if (temp == 1)
	{
		BN_mul(bnN, bnP, bnQ, ctx);
		BN_copy(key->n, bnN);
		BN_copy(key->e, bnE);
		BN_copy(key->d, bnD);
		if (RSA_check_key(key) == 1)
		{
			result = 1;
			unsigned char  bfE[4];
			strE = BN2string(bnE);

			//Bn2Bin(bnE, bfE,4);
			//strE = NCL::BytetoHexStr(bnE,8);

			strN = BN2string(bnN);
			
			strD = BN2string(bnD);

		}
	}

	BN_CTX_end(ctx);
	BN_CTX_free(ctx);
	RSA_free(key);

	return result;

}


int CalcRSAFactorsType3(RSA_HEXSTR & rsaHexStr)
{

	
	return CalcRSAParametersType3(rsaHexStr.strN, rsaHexStr.strE, rsaHexStr.strD, rsaHexStr.strP, rsaHexStr.strQ, rsaHexStr.strDp, rsaHexStr.strDq, rsaHexStr.strQInv);

}

int CalcRSACRTParameterwithPQD(string & strDp, string & strDq, string & strQInv,	const string & strP, const string & strQ, const string & strD)
{
	int result = 0;

	BN_CTX *ctx = BN_CTX_new();
	BN_CTX_start(ctx);

	BIGNUM *bnP = BN_CTX_get(ctx);
	BIGNUM *bnQ = BN_CTX_get(ctx);
	BIGNUM *bnD = BN_CTX_get(ctx);
	BIGNUM *bnDp = BN_CTX_get(ctx);
	BIGNUM *bnDq = BN_CTX_get(ctx);
	BIGNUM *bnQInv = BN_CTX_get(ctx);


	BN_hex2bn(&bnP, strP.c_str());;
	BN_hex2bn(&bnQ, strQ.c_str());;
	BN_hex2bn(&bnD, strD.c_str());;


	CalcRSACRTParameterwithPQDPIGNUM(bnDp, bnDq,bnQInv,bnP, bnQ, bnD );

	//StringToBignum(bnP, strP, strP->Length);
	//StringToBignum(bnQ, strQ, strQ->Length);
	//StringToBignum(bnD, strD, strD->Length);

	strDp = BN2string(bnDp);
	strDq = BN2string(bnDq);
	strQInv = BN2string(bnQInv);


#if 0


	BN_mod_inverse(bnQInv, bnQ, bnP, ctx);
	if (bnQInv != NULL)
	{
		BN_sub_word(bnP, 1);
		BN_mod(bnDp, bnD, bnP, ctx);

		BN_sub_word(bnQ, 1);
		BN_mod(bnDq, bnD, bnQ, ctx);

		strDp = BN2string(bnDp);
		strDq = BN2string(bnDq);
		strQInv = BN2string(bnQInv);

		//BignumToStringN(strDp	, strP->Length, bnDp	);
		//BignumToStringN(strDq	, strQ->Length, bnDq	);
		//BignumToStringN(strQInv	,strP->Length,bnQInv	);

		result = 1;
	}

#endif // 0

	BN_CTX_end(ctx);
	BN_CTX_free(ctx);

	return result;
}




#if 0
int CalcRSACRTParameterwithPQDORG(string & strDp, string & strDq, string & strQInv,
	const string & strP, const string & strQ, const string & strD)
{
	int result = 0;

	BN_CTX *ctx = BN_CTX_new();
	BN_CTX_start(ctx);

	BIGNUM *bnP = BN_CTX_get(ctx);
	BIGNUM *bnQ = BN_CTX_get(ctx);
	BIGNUM *bnD = BN_CTX_get(ctx);
	BIGNUM *bnDp = BN_CTX_get(ctx);
	BIGNUM *bnDq = BN_CTX_get(ctx);
	BIGNUM *bnQInv = BN_CTX_get(ctx);


	BN_hex2bn(&bnP, strP.c_str());;
	BN_hex2bn(&bnQ, strQ.c_str());;
	BN_hex2bn(&bnD, strD.c_str());;





	BN_mod_inverse(bnQInv, bnQ, bnP, ctx);
	if (bnQInv != NULL)
	{
		BN_sub_word(bnP, 1);
		BN_mod(bnDp, bnD, bnP, ctx);

		BN_sub_word(bnQ, 1);
		BN_mod(bnDq, bnD, bnQ, ctx);

		strDp = BN2string(bnDp);
		strDq = BN2string(bnDq);
		strQInv = BN2string(bnQInv);

		//BignumToStringN(strDp	, strP->Length, bnDp	);
		//BignumToStringN(strDq	, strQ->Length, bnDq	);
		//BignumToStringN(strQInv	,strP->Length,bnQInv	);

		result = 1;
	}



	BN_CTX_end(ctx);
	BN_CTX_free(ctx);

	return result;
}
#endif

int UpdateFillRsaCrt(RSA *rsa)
{


	rsa->p = BN_new();
	rsa->q = BN_new();
	rsa->dmp1 = BN_new();
	rsa->dmq1 = BN_new();
	rsa->iqmp = BN_new();

	int result = -1;
	int count = 1;
	for (int i = 0; i < 3 && result != 0 ; i++){
		result = CalcRSAFactorsBINNUM(rsa->p, rsa->q, rsa->n, rsa->e, rsa->d);

	} 
	
	if (result != 0) return result;

	result = CalcRSACRTParameterwithPQDPIGNUM(rsa->dmp1, rsa->dmq1, rsa->iqmp, rsa->p, rsa->q, rsa->d);
	if (result != 0) return result;

	return result;
}

RSA * GenerateRSA(int bytes,const unsigned char  *E, int elen)
{
	RSA *rsa = RSA_new();
	rsa->e = BN_bin2bn(E, elen, NULL);
	RSA_generate_key_ex(rsa, bytes*8, rsa->e, NULL);
	return rsa;

}
RSA * GenerateRSAHexStr(int bytes, const char  *E){
	VECBYTE vecbyte = NCL::HexStr2Byte(E);
	return GenerateRSA(bytes, V2A(vecbyte), vecbyte.size());
}
void ConvertRsaToHexStr(RSA * rsa,RSA_HEXSTR & rsaHexStr)
{
	rsaHexStr.strN = BN2string(rsa->n);
	rsaHexStr.strE = BN2string(rsa->e);
	rsaHexStr.strD = BN2string(rsa->d);
	rsaHexStr.strP = BN2string(rsa->p);
	rsaHexStr.strQ = BN2string(rsa->q);
	rsaHexStr.strDp = BN2string(rsa->dmp1);
	rsaHexStr.strDq = BN2string(rsa->dmq1);
	rsaHexStr.strQInv = BN2string(rsa->iqmp);


}
RSA * CreateRSA(const unsigned char  *N, int nlen, const unsigned char  *E, int elen, const unsigned char  *D, int dlen)
{

	RSA *rsa = RSA_new();
	rsa->e = BN_bin2bn(E, elen, NULL);
	rsa->n = BN_bin2bn(N, nlen, NULL);
	if (D) {
		rsa->d = BN_bin2bn(D, dlen, NULL);

		//RSA_augment_key(rsa);
	

	}

	//RSA * rsanew = 
	
	//RSA_generate_key_ex(rsa, 8*nlen, rsa->e, NULL);

	return rsa;
}
RSA * CreateRSAByHexStr(const char * N, const char * E, const char * D)
{
	unsigned char * byted = NULL;
	VECBYTE bytesN = NCL::HexStr2Byte(N);
	VECBYTE bytesE = NCL::HexStr2Byte(E);
	VECBYTE bytesD;
	if (D) {
		bytesD = NCL::HexStr2Byte(D);
		byted = V2A(bytesD);
	}

	return CreateRSA(V2A(bytesN), bytesN.size(),
		V2A(bytesE), bytesE.size(),
		byted, bytesD.size()
		);


}

typedef int (*PFi2dFUNCTION)(RSA *a, unsigned char **pp);

int i2fPubkey(RSA *a, unsigned char **pp){
	return i2d_RSAPublicKey(a,pp);
}

int i2fSubjectPubkey(RSA *a, unsigned char **pp){
	return i2d_RSA_PUBKEY(a, pp);
}
int i2fPrivkey(RSA *a, unsigned char **pp){
	return i2d_RSAPrivateKey(a, pp);
}


VECBYTE DER_Encode_DEF(	const char * N, const char * E, const char * D, PFi2dFUNCTION pfi2dfunction) 
{

	VECBYTE  retbyte;

	RSA *rsa = CreateRSAByHexStr(N, E, D);
//	rsa->e = BN_bin2bn(E, elen, NULL);
//	rsa->n = BN_bin2bn(N, nlen, NULL);
	//키값 전역변수에 저장
	//BN_hex2bn(&G_RSAkey_TSM->n,n);
	//rsa->n->dmax = 0x100;
	//BN_hex2bn(&G_RSAkey_TSM->e,e);
	//rsa->e->dmax = 0x04;
	if (D){
		UpdateFillRsaCrt(rsa);
		//rsa->p = BN_new();
		//rsa->q = BN_new();
		//rsa->dmp1 = BN_new();
		//rsa->dmq1 = BN_new();
		//rsa->iqmp = BN_new();
	}

	

	unsigned char *buf, *next;
	int size = 0;
	size = pfi2dfunction(rsa, 0);

	next = buf = (unsigned char *)malloc(size);
	if (!buf) return retbyte;
	
	//if (!(buf = next = (unsigned char *)malloc(size))) return 0;


	pfi2dfunction(rsa, &next); /* If we use buf here, return buf; becomes wrong */

	RSA_free(rsa);


	retbyte.resize(size);
	memcpy(&retbyte[0], buf, size);

	free(buf);


	return retbyte;
}
#if 0
void DEREncodeDEF(const VECBYTE &N, const VECBYTE &E, const VECBYTE &D, PFi2dFUNCTION pfi2dfunction, VECBYTE & retbyte) {
	int size = 0;

	const char * strN = NCL::VecBytetoHexStr(N).c_str();
	const char * strE = NCL::VecBytetoHexStr(E).c_str();
	const char * strD = NCL::VecBytetoHexStr(D).c_str();


	unsigned char * pbuff = DER_Encode_DEF(strN, strE, strD,
		&size,pfi2dfunction);

	retbyte.resize(size);
	memcpy(&retbyte[0], pbuff, size);

	free(pbuff);

}



unsigned char *DER_encode_RSA_Public(const unsigned char  *N, int nlen, const unsigned char  *E, int elen, int *outlength){
	return DER_Encode_DEF(N, nlen, E, elen, outlength, i2fPubkey);
}
unsigned char *DER_encode_RSA_SubjectPublic(const unsigned char  *N, int nlen, const unsigned char  *E, int elen, int *outlength) 
{
	//RSA *rsa = CreateRSA(N, nlen, E, elen);
	////RSA *rsa = RSA_new();
	////rsa->e = BN_bin2bn(E, elen, NULL);
	////rsa->n = BN_bin2bn(N, nlen, NULL);
	//////키값 전역변수에 저장
	//////BN_hex2bn(&G_RSAkey_TSM->n,n);
	////rsa->n->dmax = 0x100;
	//////BN_hex2bn(&G_RSAkey_TSM->e,e);
	////rsa->e->dmax = 0x04;

	////RSA_free(rsa);

	//unsigned char *buf, *next;

	//*outlength = i2d_RSA_PUBKEY(rsa, 0);
	//if (!(buf = next = (unsigned char *)malloc(*outlength))) return 0;
	//i2d_RSA_PUBKEY(rsa, &next); /* If we use buf here, return buf; becomes wrong */

	//RSA_free(rsa);

	return DER_Encode_DEF(N, nlen, E, elen, outlength, i2fSubjectPubkey);;
}
#endif

VECBYTE DEREncodeRSApublic(const VECBYTE &N, const VECBYTE &E)
{
	return DER_Encode_DEF(NCL::VecBytetoHexStr(N).c_str(), NCL::VecBytetoHexStr(E).c_str(), NULL, i2fPubkey);;
}

VECBYTE DEREncodeRSAprivate(const VECBYTE &N, const VECBYTE &E, const VECBYTE &D)
{
	return DER_Encode_DEF(NCL::VecBytetoHexStr(N).c_str(), NCL::VecBytetoHexStr(E).c_str(), NCL::VecBytetoHexStr(D).c_str(), i2fPrivkey);;
}

VECBYTE DEREncodeSubjectRSApublic(const VECBYTE &N, const VECBYTE &E){
	return DER_Encode_DEF(NCL::VecBytetoHexStr(N).c_str(), NCL::VecBytetoHexStr(E).c_str(), NULL, i2fSubjectPubkey);;


}

int checkRSAKey(const string & strN, const string & strE, const string & strD )
{
	VECBYTE N = NCL::HexStr2Byte(strN);
	VECBYTE E = NCL::HexStr2Byte(strE);
	VECBYTE D = NCL::HexStr2Byte(strD);
	RSA *rsa = CreateRSA(&N[0], N.size(), &E[0], E.size(), &D[0], D.size());

	return RSA_check_key(rsa);
}

void derencodingtest()
{
	//Get the X509 object.
	VECBYTE Ntsm = NCL::HexStr2Byte("94471B1FEDABCA256DAD465908A69B5880048853CA337BED6C6BD50652EABEBCB524873C86A959389DBE897521901EE9B76FFA0D86D4211BE1658AA97A132E8415B711DAD8032D5767D905183A0A5D8BE8AC6724F7039C67E5C6A86462DED539990DA23434BA0261A1368E424230310D7651EEAE8182AD2A066ED69EFA82616A3991C1783795AB1B6A0ADB56C0B94860B02B73C3E4739CB00A48DCC82AD4930D1525516F988E7603F2AC4C4E7EEE0802B260EAE7221A8FFB7C5917DC4265A7F7C695888BC5C297910AC010415D96131D4771A08B6B3ABF3417A4C9AABF284B9118B98EB4520EE604999DFE147C0D40A6752ABBBDE5B8AB67611225E93DA13807");
	VECBYTE Etsm = NCL::HexStr2Byte("00010001");
	VECBYTE retbyte = DEREncodeSubjectRSApublic(Ntsm, Etsm);

	int len = 0;
	//unsigned char * pbyte = DER_encode_RSA_public(&Ntsm[0], Ntsm.size(), &Etsm[0], Etsm.size(), &len);

	FILE * fp = fopen("public.bin","wb");
	fwrite(&retbyte[0], 1, retbyte.size(), fp);
	fclose(fp);

	

	
}
#endif

static const unsigned short CRC16TAB[256] = 
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,  
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441, 
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,  
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041, 
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 
};
unsigned long calcCRCByTable(unsigned char* data, int iLength, const unsigned short *crctable)
{
	unsigned long crc = 0;
	unsigned short index =0;
	int i;

	for (i = 0; i < iLength; i++){
		index = (unsigned char)(crc ^ data[i]);
		crc = (crc >> 8) ^ crctable[index];
	}

	//	crc = crc << 8 | crc >> 8 ; //++ swap because HW uart crc module

	return (unsigned long)(unsigned short)crc;
}

unsigned long calcCRC(unsigned char* data, int iLength)
{
	return calcCRCByTable(data, iLength, CRC16TAB);
    unsigned long crc = 0;
	int i;
     
    for (i = 0; i < iLength; i++)
         crc = (crc >> 8) ^ CRC16TAB[(unsigned char)(crc ^ data[i])];

//	crc = crc << 8 | crc >> 8 ; //++ swap because HW uart crc module

    return (unsigned long)(unsigned short)crc;
}

string refhex = "0123456789ABCDEF";
string refascii = "0123456789ABCDEFGHJKLMNOPQRSTUVWXYZ!@#$%^&*()";

string  getRandNum(const string & ref, int length){
	string ret = ""; 

	srand((int)time(NULL));
	FORNLOOP(length){
		int index = rand() % refhex.length();
		char ch = ref[index];
		ret += ch;
	}

	return ret;
}
string  getRandHexStr(int length)
{
	return getRandNum(refhex, length*2);
}

string  getRandAscii(int length)
{
	return getRandNum(refascii, length);
}

void getRandBYTE(unsigned char * byte,int length)
{
	CMANMEM<BYTEARRAY> cman = getHexStr(getRandHexStr(length));
	memcpy(byte,cman.GetLpBuff()->array,length);

}

LPBYTEARRAY getHexStr(const string & str)
{
	LPBYTEARRAY cmnBuffer = (LPBYTEARRAY)NCL::MakeNSFormDwordBase((neocolib::uint)str.length() / 2, sizeof(nbyte));
	hex2bin(str.c_str(), cmnBuffer->array);
	return cmnBuffer;
}



short InverSHORT(short org)
{
	

	return MAKE_WORD(HI_BYTE(org), LO_BYTE(org));

}



unsigned char * getDecryptValuePtr(const unsigned char * input, int  inputSize, int *outsize)
{
	unsigned char * pchar = (unsigned char *)input;
	unsigned char * pcharstart = NULL;

	for (int i = 0; i < inputSize; i++, pchar++){

		unsigned char value = *pchar;
		if (i == 0 && value != 0x00) return NULL;

		if (i == 1 && value != 0x02) return NULL;

		if (i > 1 && value == 0x00) {
			pcharstart = pchar + 1;
			break;
		}


	}
	if (outsize){
		*outsize = inputSize - (pcharstart - (unsigned char *)input);
	}
	return pcharstart;


}
bool getDecryptValue(VECBYTE & output, const VECBYTE & input)
{
	int size;
	const unsigned char * pcharstart = getDecryptValuePtr(V2A(input), input.size(), &size);
	if (!pcharstart) return false;



	NCL::AppendVector(output, pcharstart, size);
	//output.insert(output.begin(), output.end())

	return true;

}
