#pragma once

// control cmd
#define     CMD_REQ_RESTART                     0x1E
#define     CMD_RES_RESTART                     0x9E
#define     CMD_REQ_SLEEP                       0x1D
#define     CMD_RES_SLEEP                       0x9D
#define     CMD_REQ_WAKEUP                      0xFB
#define     CMD_RES_WAKEUP                      0xFC
#define     CMD_REQ_FLASH_MODE                  0xF3
#define     CMD_RES_FLASH_MODE                  0xF4
#define     CMD_REQ_SELF_TEST_MODE              0xF5
#define     CMD_RES_SELF_TEST_MODE              0xF6
#define     CMD_REQ_IP_TEST_MODE                0xF7
#define     CMD_RES_IP_TEST_MODE                0xF8
#define     CMD_REQ_PUF_TEST_MODE               0xF9
#define     CMD_RES_PUF_TEST_MODE               0xFA
#define     CMD_RES_DENY                        0xFD

// fab
#define     CMD_REQ_TEST                        0x00
#define     CMD_RES_TEST                        0x80
#define     CMD_ERR_TEST                        0xC0

#define     CMD_REQ_SEL_PUF                     0x01
#define     CMD_RES_SEL_PUF                     0x81
#define     CMD_REQ_CONFIG                      0x02
#define     CMD_RES_CONFIG                      0x82

// issuie
#define     CMD_REQ_SN                          0x03
#define     CMD_RES_SN                          0x83
#define     CMD_ERR_SN                          0xC3

// issue RSA
#define     CMD_REQ_ID_RSA                      0x24
#define     CMD_RES_ID_RSA                      0xA4
#define     CMD_ERR_ID                          0xC4

#define     CMD_REQ_PUK_RSA                     0x25
#define     CMD_RES_PUK_RSA                     0xA5
#define     CMD_ERR_PUK_RSA                     0xE5

#define     CMD_REQ_CERT_RSA                    0x26
#define     CMD_RES_CERT_RSA                    0xA6
#define     CMD_ERR_CERT_RSA                    0xC6

#define     CMD_REQ_REG_CERT_RSA                0x27
#define     CMD_ERR_REG_CERT                    0xC7

#define     CMD_REQ_FINISH                      0x1F

//issue ECC
#define     CMD_REQ_ID_ECC                      0x34
#define     CMD_RES_ID_ECC                      0xB4

#define     CMD_REQ_PUK_ECC                     0x35
#define     CMD_RES_PUK_ECC                     0xB5
#define     CMD_ERR_PUK_ECC                     0xF5

#define     CMD_REQ_CERT_ECC                    0x36
#define     CMD_RES_CERT_ECC                    0xB6
#define     CMD_ERR_CERT_ECC                    0xF6

#define     CMD_REQ_REG_CERT_ECC                0x37


//use
#define     CMD_REQ_RN                          0x28
#define     CMD_RES_RN                          0x88

//use RSA
#define     CMD_REQ_AUTH_RSA                    0x29
#define     CMD_ERR_AUTH                        0xC9

#define     CMD_REQ_SESSION_RSA                 0x2A
#define     CMD_ERR_SESSION_RSA                 0xEA

#define     CMD_REQ_HS1_RSA                     0x2B	//2B ->AB : 20140617
#define     CMD_ERR_HS1_RSA                     0xEB

#define     CMD_REQ_EHK                         0x0D
#define     CMD_RES_EHK                         0x8D

#define     CMD_REQ_SIGN                        0x0E
#define     CMD_RES_SIGN_RSA                    0xAE
#define     CMD_ERR_SIGN                        0xCE

//use ECC
#define     CMD_REQ_AUTH_ECC                    0x39

#define     CMD_REQ_SESSION_ECC                 0x3A
#define     CMD_ERR_SESSION_ECC                 0xFA

#define     CMD_REQ_HS1_ECC                     0x3B
#define     CMD_ERR_HS1_ECC                     0xFB

#define     CMD_REQ_HS2_ECC                     0x3C //++
#define     CMD_ERR_HS2_ECC                     0xFC

#define     CMD_RES_SIGN_ECC                    0xBE


//crypto
#define     CMD_REQ_CTR                         0x10
#define     CMD_REQ_ENC_CCM                     0x11
#define     CMD_REQ_DEC_CCM                     0x12
#define     CMD_REQ_CRYPTO_CONT                 0x13
#define     CMD_REQ_CCM_GENERATE_MAC            0x14
#define     CMD_REQ_CCM_VERIFY_MAC              0x15
#define     CMD_REQ_HMAC                        0x16
#define     CMD_REQ_HMAC_CONT                   0x17
#define     CMD_REQ_GENERATE_HMAC               0x18
#define     CMD_REQ_VERIFY_HMAC                 0x19

#define     CMD_RES_CRYPTO                      0x90
#define     CMD_RES_CCM_GENERATE_MAC            0x94
#define     CMD_RES_CCM_VERIFY_MAC              0x95
#define     CMD_RES_HMAC                        0x96
#define     CMD_RES_GENERATE_HMAC               0x98
#define     CMD_RES_VERIFY_HMAC                 0x99


// Flash mode command
#define     CMD_FLASH_REQ_SYNC                  0x80
#define     CMD_FLASH_RES_SYNC                  0x80
#define     CMD_FLASH_REQ_READ                  0xE1
#define     CMD_FLASH_RES_READ                  0xE2
#define     CMD_FLASH_REQ_WRITE                 0xE3
#define     CMD_FLASH_RES_WRITE                 0xE4
#define     CMD_FLASH_REQ_ERASE                 0xE5
#define     CMD_FLASH_RES_ERASE                 0xE6






//ERR cdoe
#define     EC_NO_ERR                           0x00
#define     EC_ER_DISUSE                        0x01
#define     EC_ER_SN                            0x02
#define     EC_ER_ID                            0x03
#define     EC_ER_INVALID_SN                    0x04
#define     EC_ER_CRC                           0x05
#define     EC_ER_CERT                          0x06
#define     EC_ER_SDATA                         0x07
#define     EC_ER_HK                            0x08
#define     EC_ER_E                             0x09
#define     EC_ER_PNT                           0x0A
#define     EC_ER_RSA_SIGN                      0x0B
#define     EC_ER_ECC_SIGN                      0x0C
#define     EC_ER_OTP                           0x0D
#define     EC_ER_HS                            0x0E
#define     EC_ER_RN                            0x0F
#define     EC_ER_CERT_SIGN                     0x10			//++ dy insert

//crypt mode
#define     MODE_CRYPTO_CTR                     0x00
#define     MODE_CRYPTO_HMAC                    0x01
#define     MODE_CRYPTO_CCM                     0x02


//CRYPTO_ERROR_CMD
#define     CRYPTO_ERROR_NO                     0
#define     CRYPTO_ERROR_CMD                    1
#define     CRYPTO_ERROR_MAC                    2


#define     COMM_REQUEST_TIME                   1000
#define     COMM_IGNORE_TIME                    200
#define     COMM_RSA_KEY_GEN_TIME               40000


//EXCEP
#define     EXCEP_TSM_E                         1
#define     EXCEP_CRC                           2
#define     EXCEP_CERT_ID                       11
#define     EXCEP_CERT_N_CHIP                   12
#define     EXCEP_CERT_SIGN                     13
#define     EXCEP_SDATA                         20
#define     EXCEP_EHK                           21
#define     EXCEP_C1                            30
#define     EXCEP_ERR_SIGN_OTP                  31
#define     EXCEP_ERR_SIGN_E                    32
#define     EXCEP_ERR_SIGN_SIGN                 33
#define     EXCEP_ERR_SIGN                      34

#define     EXCEP_TSM_Q                         100
#define     EXCEP_CERT_Q_CHIP                   101
#define     EXCEP_ERR_PNT                       102


#define     EXCEP_CRYPTO_OTHER                  0x20000
#define     EXCEP_IN_CRYPTO_OTHER               0x2
#define     EXCEP_CRYPTO_MAC                    0x30000
#define     EXCEP_IN_CRYPTO_MAC                 0x3




//#define     BYTE                                unsigned char
//#define     DWORD                               unsigned long


typedef void*               HANDLE;
typedef void*               LPVOID;
typedef int                 BOOL;
typedef BOOL*               LPBOOL;
typedef unsigned char       BYTE;
typedef BYTE                UCHAR;
typedef BYTE*               LPBYTE;
typedef UCHAR*              PUCHAR;
typedef unsigned short      WORD;
typedef WORD*               LPWORD;
typedef unsigned long       DWORD;
typedef DWORD               ULONG;
typedef DWORD*              LPDWORD;
typedef ULONG*              PULONG;
typedef char*               LPSTR;
typedef const char*         LPCSTR;



#undef      GetCurrentDirectory

#if 0

extern BYTE K1[16];
extern BYTE K2[16];
extern BYTE IV[16];
extern BYTE G_MSG[];
extern int g_msgPos;
extern BYTE SEL_AES;
extern BYTE SN[16];
extern BYTE RN[16];






//int RSA_Keyinit_TSM();
//int ECC_Keyinit_TSM();
int ECC_keyinit(int gen);
int RSA_Keyinit(int gen);
int generateSN(BYTE *pbOut, DWORD *pdwLenOut, BYTE *pbLog);
int processSNRSA(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);

int requestIdRSA(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog, int iExcep);
int requestCertRSA(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
int requestRegCertRSA(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog, int iExcep);

int requestIdECC(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog, int iExcep);
int requestCertECC(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
int requestRegCertECC(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog, int iExcep);

int requestRN(BYTE *pbOut, DWORD *pdwLenOut, BYTE *pbLog, int iExcep);

int requestRNECC(BYTE *pbOut, DWORD *pdwLenOut, BYTE *pbLog, int iExcep);

int requestSessionRSA(BYTE* pbIn, int iLenIn, BYTE* pbLog);
int requestHS1RSA(BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog, int iExcep);
int requestSignRSA(BYTE* pbIn, int iLenIn, BYTE *pbOut, DWORD *pdwLenOut, BYTE *pbLog, BYTE* pbOTP, int iExcep);
int requestEHKdevice(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog);

int requestSessionECC(BYTE* pbIn, int iLenIn, BYTE* pbLog);
int requestHS1ECC(BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog, int iExcep);
int requestHS2ECC(BYTE* pbIn, int iLenIn, BYTE* pbLog);
int errorHS2ECC(BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
int storeEHKdeviceECC(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog);
int requestSignECC(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog, BYTE* pbOTP, int iExcep);

//for test old
int requestSessionECCold(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog);

int encryptCTR(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
//int decryptCTR(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
int encryptCCM(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
int decryptCCM(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
int generateHMAC(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);

int data2paddingdata_HMAC(BYTE* data, int len, BYTE* pbLog);
int data2paddingdata_AES(BYTE* data, int len, BYTE* pbLog);



//.
int requestIdECCxor(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut,   BYTE* pbLog);
int requestCertECCxor(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
int requestRNECCxor(BYTE *pbOut, DWORD *pdwLenOut, BYTE *pbLog);
int requestSessionECCxor(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
int requestHS2ECCxor(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog);
int storeEHKdeviceECCxor(BYTE* pbIn, int iLenIn, BYTE * pbOut, DWORD * pdwLenOut, BYTE *pbLog);
int requestConfirmECCxor(BYTE* pbIn, int iLenIn, BYTE* pbOut, DWORD* pdwLenOut, BYTE* pbLog, BYTE* pbOTP);
#endif