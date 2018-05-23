#ifndef __CRYPTODEFINE__
#define __CRYPTODEFINE__

#define DEFUNITSIZE		16
#define DEFRSASIZE 256
#define DEFECCSIZE 33
#define DEFECCSIZE_SIGN 32

#define CERT_IDSIZE 16
#define DEVICE_SIZE 32
#define TSM_SIZE	32
#define ACHIP_SIZE	33

#define CERT_QCHIPSIZE 33
#define CERT_SIGNSIZE_ECC 32

#define CERT_NCHIPSIZE 256
#define CERT_SIGNSZE_RSA 256

#define CERT_SDATASIZE 32
#define CERT_EHKSIZE	32
#define CERT_ETSMSIZE	16
#define CERT_NTSMSIZE	256

#define CERT_C1SIZE 16

#define OTPSIZE 6
#define G_MSG_SIZE 65600

#pragma pack(push, 1)   
struct CERT_ECC	//size 113 byte
{
	unsigned char ID[CERT_IDSIZE];
	unsigned char Qchip[CERT_QCHIPSIZE];
	unsigned char SIGN_R[CERT_SIGNSIZE_ECC];
	unsigned char SIGN_S[CERT_SIGNSIZE_ECC];
	//BYTE option_data[256];
};


struct CERTEXT_ECC
{
	CERT_ECC cert;


};

struct CERT_RSA	//size 113 byte
{
	unsigned char ID[CERT_IDSIZE];
	unsigned char Nchip[CERT_NCHIPSIZE];
	unsigned char SIGN[CERT_SIGNSZE_RSA];
	//BYTE option_data[256];
};


struct CERTEXT_RSA
{
	CERT_RSA cert;


};

#pragma pack(pop)  



#endif