#pragma once

#ifdef __cplusplus
#include <vector>
#include <string>


using namespace std;
#endif


#define  OUT 


#ifdef _DEFNEODEF_
#define _USE_UTF8_
#define NEOUSEMBCS
#define NEO_STATIC
#define LSA_EXPORTS_NOUSE
#endif


#ifdef NEO_STATIC
#define EXTERN_API
#else 
#ifdef NEO_EXPORTS
#define EXTERN_API __declspec(dllexport)
#else
#define EXTERN_API __declspec(dllimport)
#endif

#endif
#define  FORNLOOP(n_) for(int nCount = 0 ; nCount < (int)(n_) ; nCount++)
#define  FORSZPLOOP(lpdec_,lpn_) for(lpdec_ * pTemp = (lpn_) ; *pTemp  ; pTemp++)
#define  FORNLOOPEXT(count_,n_) for(int count_ = 0 ; count_ < (int)(n_) ; count_++)

#define  FORVECTEREXT(pinter_,vecter_,comp_) for(vecter_::iterator pinter_ = comp_.begin();pinter_ < comp_.end();++pinter_)
#define  FORVECTER(vecter_,comp_) for(vecter_::iterator piter = comp_.begin();piter != comp_.end();++piter)//FORVECTEREXT(vecter_,str_,comp_)//for(vector<str_>::iterator piter = comp_.begin();piter <= comp_.end();++piter)
#define  FORCONVECTER(vecter_,comp_) for(vecter_::const_iterator piter = comp_.begin();piter != comp_.end();++piter)//FORVECTEREXT(vecter_,str_,comp_)//for(vector<str_>::iterator piter = comp_.begin();piter <= comp_.end();++piter)
#define  FORCONVECTEREXT(pinter_,vecter_,comp_) for(vecter_::const_iterator pinter_ = comp_.begin();pinter_ != comp_.end();++pinter_)
#define  MAP_ISCONTAIN(map_,key_) (map_.find(key_) != map_.end())
#define MUTEXLOCK(a_)	CMUTEX cMutext(L#a_)

#define ISINVALIDATE(a_) ((a_)==((size_t)-1))

#define MAX_VALUE_NAME 16383
#define MAX_KEY_LENGTH 255

#define MAKELONGLONG(a, b)      ((neocolib::int64)(((int)((a) & 0xffffffff)) | ((neocolib::int64)((int)((b) & 0xffffffff))) << 32))

#define MAKE_WORD(a, b)      ((neocolib::uint16)(((neocolib::nbyte)(((long)(a)) & 0xff)) | ((neocolib::uint16)((neocolib::nbyte)(((long)(b)) & 0xff))) << 8))
#define MAKE_LONG(a, b)      ((long)(((neocolib::uint16)(((long)(a)) & 0xffff)) | ((long)((neocolib::uint16)(((long)(b)) & 0xffff))) << 16))
#define LO_WORD(l)           ((neocolib::uint16)(((long)(l)) & 0xffff))
#define HI_WORD(l)           ((neocolib::uint16)((((long)(l)) >> 16) & 0xffff))
#define LO_BYTE(w)           ((neocolib::nbyte)(((long)(w)) & 0xff))
#define HI_BYTE(w)           ((neocolib::nbyte)((((long)(w)) >> 8) & 0xff))

#define V2A(a_) &(a_)[0]

#define STRDEF_ST(a_) typedef struct _tag##a_{
#define STRDEF_ED(a_)  }a_, * a_##_PTR;

#ifdef _USE_UTF8_
#define MAX_NCL_PATH         2*4096
#else 
#define MAX_NCL_PATH          260
#endif

#ifndef _USE_UTF8_
#define VSPRINTF  vswprintf
#define SPRINTF swprintf
#define VSCPRINTF _vscwprintf
#define STRTOK wcstok
#define STRCPY wcscpy

#define _t(x)      L ## x

#else
#define VSPRINTF vsprintf 
#define SPRINTF sprintf
#define VSCPRINTF _vscprintf

#define STRTOK strtok
#define STRNCPY strncpy
#define STRCPY strcpy
#define _t(x)       x
#endif


//typedef unsigned int uint;	// CODE ADD 20140212 sodazim
#ifdef __cplusplus
namespace neocolib {

typedef wchar_t wchar;

#ifndef _USE_UTF8_
typedef wchar tchar;
typedef wstring tstring;
#else
typedef char tchar;
typedef string tstring;
#endif

typedef long long int64;
typedef unsigned int uint;
typedef unsigned long long uint64;
typedef unsigned char nbyte,*pbyte;
typedef unsigned short uint16, *puint16;
typedef tchar* ptstr;
typedef const tchar* pctstr;
typedef void* pvoid;
typedef const tstring & rctstr;
//#error fasdfs
template <typename T>
struct ARRAYLIST {
	unsigned long length;
	T array[1];
};


#ifdef __USE_CIRCULAR_MEMORY__
typedef  ptstr p_circular_tstr;
typedef  char* p_circular_str;
typedef  wchar* p_circular_wstr;

#else
typedef  tstring p_circular_tstr;
typedef  string p_circular_str;
typedef  wstring p_circular_wstr;
#endif// __USE_CIRCULAR_MEMORY__

typedef ARRAYLIST<nbyte> BYTEARRAY,* LPBYTEARRAY;
typedef ARRAYLIST<uint> DWORDARRAY,* LPDWORDARRAY;
typedef ARRAYLIST<tchar> TCHARSTR,* LPTCHARSTR;
typedef ARRAYLIST<ptstr> WSTRARRAY,*LPWSTRARRAY;
typedef ARRAYLIST<pvoid> PVOIDARRAY,*LPPVOIDARRAY;

typedef vector<tstring> VECSTRING;
typedef vector<nbyte> VECBYTE;

}
#else
	
typedef long long int64;
typedef unsigned int uint;
typedef unsigned long long uint64;
typedef unsigned char nbyte,*pbyte;
typedef unsigned short uint16, *puint16;
//typedef tchar* ptstr;
//typedef const tchar* pctstr;
typedef void* pvoid;


#endif
