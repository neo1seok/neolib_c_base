#pragma once


/****************************************************************************
 * File Name			:
 * Usage				:
 * Author				: neo1seok
 * Creation				: 2013.12.02
 * Last Modification	: 2013.12.12
 * Contents				:
 해당 모듈은 neo1seok이 c++ 기반의 코딩작업시 필요한 모듈을 정리한 것으로
 기존에 자체 고안된 Array이 방식에서 STL을 적요한 버젼으로 제작되었다.

 이곳에서 사용하는 p_circular_tstr,p_circular_str,p_circular_wstr 은
각각 tchar*,char*,wchar* 으로서 각각 n개의 개수를 가지고
 순환하는 구조이다. 즉 n개 이상의 요청이 있을때 기존에 첫번째 메모리가
 해제되고 다시 생성된다.

 그렇기 때문에 20개 이상의 스트링 구조를 갖기 위해서는 반드시
 CopyNewStr 로 새로운 스트링을 가지고 사용해야 한다.





						DSNTECH Co., Ltd.
3F,Cheongwon Bldg, 828-5 Yeoksam-dong, Gangnam-gu,  Seoul 135-935, Korea
****************************************************************************/

/***************************************************************************
 * Includes
 ***************************************************************************/
//#include <windows.h>
//#include <tchar.h>
//#include <afx.h>
#include <stdio.h>		// CODE ADD 20140212 sodazim : error: ‘va_list’ has not been declared
#include <string.h>		// CODE ADD 20140213 sodazim : memset(), memcpy()
#include <string>
#include <stdlib.h>
#include<map>

#include "comdef.h"

using namespace neocolib;

/***************************************************************************
 * Local Definitions
 ***************************************************************************/
#define	 CND CNeoDebug
#define NCL NeoCoLib
#define CMNTCHAR CMANMEM<tchar>

/***************************************************************************
 * Local Type Definitions
 ***************************************************************************/

/***************************************************************************
 * External Variables
 ***************************************************************************/

/***************************************************************************
 * External Functions
 ***************************************************************************/

/***************************************************************************
 * Internal Variables
 ***************************************************************************/

/***************************************************************************
 * Internal Functions
 ***************************************************************************/

/***************************************************************************
 * Functions Descriptions
 ***************************************************************************/

#ifdef __VALIST
#define va_list __VALIST
#endif
#if (_MSC_VER == 1800)
 #ifndef max
 #define max(a,b)    (((a) > (b)) ? (a) : (b))
 #endif
 #ifndef min
 #define min(a,b)    (((a) < (b)) ? (a) : (b))
 #endif
#endif
namespace neocolib {

//class EXTERN_API NeoCoLib{
class NeoCoLib {
public:
	static pvoid MakeNSFormDwordBase(neocolib::uint nLength,neocolib::uint nUnitStructure,pvoid reAloococ = NULL,neocolib::uint* lpmemSize = NULL);
	static pvoid MakeNSFormMInfoBase(neocolib::uint nLength,neocolib::uint bfUnitMemSize,neocolib::uint nUnitStructure,pvoid reAloococ= NULL,neocolib::uint* lpmemSize = NULL);

	static p_circular_tstr GetSubPart(const tstring& szPathName,pctstr szmarks,bool isFirstFind,bool isFrontPart);

	static p_circular_tstr GetNewStrFormV(pctstr fmt, va_list args);

	static bool ChangeDate2Str(OUT tstring& str ,time_t *pltime = NULL);
	static bool ChangeDate2StrEx(OUT tstring&  str ,time_t *pltime =NULL);

	static p_circular_tstr RecplaceStr(const tstring& pOrg,const tstring& pszOld,const tstring& pszNew);
	static p_circular_tstr GetTimeStr(const tstring& szStr,time_t * pltime);
	static p_circular_tstr CopyInterStr(const tstring& pOrgStr,tchar ch);

	static void FreeMem(pvoid lpMem);
	static pvoid MallocMem(neocolib::uint memSize,pvoid lpMem = NULL);
	static void GetMemoryInfo(int&totalSize,int &nNode);
	static void MemNodeState();

	static LPWSTRARRAY ConvertStrArrayFromVECSTRING( VECSTRING &vecstr);
	static VECSTRING ConvertVECSTRINGFromStrArray( LPWSTRARRAY  lpwstrarray);

	static p_circular_tstr GetDirNameFromPath(const tstring& szPathName);
	static p_circular_tstr GetFileNameFromPath(const tstring& szPathName);
	static p_circular_tstr GetExtName(const tstring& szPathName);
	static p_circular_tstr GetDriverNameFromPath(const tstring& szPathName);

	static p_circular_str ConvWC2UTF8(const wstring&  szOrgStr);
	static p_circular_wstr ConvUTF82WC(const string&  szOrgStr);

	static p_circular_wstr ConvMB2WC(neocolib::uint codePage ,const string&  szOrgStr);
	static p_circular_str  ConvWC2MB(neocolib::uint codePage ,const wstring& szOrgStr);

	static int CompBin(pbyte lpCmp1,pbyte lpCmp2,neocolib::uint binsize);

	static int CompBin(neocolib::uint* lpCmp1,neocolib::uint* lpCmp2,neocolib::uint dwsize);
	static bool WildCompare(rctstr pattern, rctstr input);

	static p_circular_tstr GetStaticStrForm(pctstr fmt, ...);
	static p_circular_tstr GetStaticTimeForm(time_t *pltime,pctstr fmt, ...);

	static void MakeException(const string &msg) throw( char*);

#if 0 //시스템 관련 함수 들이므로 systemlib.h 으로 이동
	static LPBYTEARRAY  GetLpDBFromFP(HANDLE hFile ,neocolib::uint stOffset = 0,neocolib::uint edOffset = -1);
	static HANDLE CreateFileSimple(const tstring &lpfname,MODECREATE mode);
	static neocolib::uint GetLastOffset(HANDLE hFile);
	static neocolib::uint WriteStrFile(HANDLE hFile,pctstr fmt,...);
	static neocolib::uint WriteFile(HANDLE hFile,pvoid lpbuff, neocolib::uint nBuffSize);
	static neocolib::uint ReadFile(HANDLE hFile,pvoid lpbuff, neocolib::uint nBuffSize);
#endif

	static pvoid LoadMemory(pvoid lpOrgBuff ,pvoid lpBuff,neocolib::uint nSize);
	static pvoid SaveMemory(pvoid lpOrgBuff ,pvoid lpBuff,neocolib::uint nSize);

	static void FreeStringArray(ptstr * ppList);
	static void FreeStrArray(LPWSTRARRAY lpstrarray);
	static void FreeLPP(pvoid *lparray);
	static void FreeLPArray(LPPVOIDARRAY lparray);

	static LPWSTRARRAY GetSplit2Array(const tstring& strSTr , pctstr szSep);
	static neocolib::uint GetNote(neocolib::uint size,tchar &ch);
	static p_circular_tstr GetNote(neocolib::uint size);
	static tstring UnitTest2();
	static int * UnitTest3();

	static p_circular_tstr GetStaticTStr(const tstring &str);
	static p_circular_str GetStaticStr(const string &str);
	static p_circular_wstr GetStaticWStr(const wstring &str);

	static void ReplaceAll ( tstring & strSrc, rctstr strFind, rctstr strDest);
	static map<string, string> getmaparam(int argc, char* argv[]);
	static int char2int(char input);



	static string BytetoHexStr(const  void * Barray, int Blen);
	static string VecBytetoHexStr(const  VECBYTE &vecbyte);
	static VECBYTE  HexStr2Byte(const string & str);
	static string HexStr2AsciiString(const string & str);


	static int  HexStr2ByteArray(const string & str, void *pbyte);

	static int ByteToByteArray(const  VECBYTE &vecbyte,  void * Barray);
	static VECBYTE ByteArrayToByte(const  void * Barray, int Blen);
	static string ByteToBitsString(unsigned char byte);
	static short InverSHORT(short org);
	static int InverINT(int org);
	static unsigned char InverBitsPerByte(unsigned char byte);
	static void InverBits( void *  buff, int length);
	
	static int countBits(unsigned char byte);
	static int countBitsFromBytes(unsigned char * pbyte, int bytecount);

	static void Sleep(int ms);
	//template<class cTy_ >
	//static cTy_* V2A(vector<cTy_>& dstvector,int start = 0){
	//	//if (dstvector.size() <= start) return 0;

	//	return &dstvector[start];
	//}


	template<class cTy_ >
	static void AppendVector(vector<cTy_>& dstvector, const cTy_* lpbuff, int length){

		dstvector.insert(dstvector.begin() + dstvector.size(), (cTy_*)lpbuff, (cTy_*)lpbuff + length);

	}

	template<class cTy_ >
	static void AppendVector(vector<cTy_>& dstvector, const vector<cTy_> & vecbuff){

		dstvector.insert(dstvector.begin() + dstvector.size(), vecbuff.begin(), vecbuff.end());

	}
	template<class cTy_ >
	static cTy_ * MallocMemTPL(neocolib::uint memSize, pvoid lpMem = NULL){

		return (cTy_ *)MallocMem(memSize, lpMem);
	
	}
	
	template<class cTy_ >
	static ARRAYLIST<cTy_> *MakeNSFormDwordBaseTPL(neocolib::uint nLength, pvoid reAloococ = NULL, neocolib::uint* lpmemSize = NULL){
		return (ARRAYLIST<cTy_> *)MakeNSFormDwordBase(nLength, sizeof(cTy_), reAloococ, lpmemSize);
	}
	

	template<class cTy_ >
	static cTy_* CopyPSTRFromSTRING(cTy_* pstr,
					const basic_string<cTy_> & str,
					int maxSize =0x7fffffff,
					int * pmemsize = NULL)
	{

		int memlength = sizeof(cTy_)*(min((int)(str.size()+1),maxSize));
		if(!pstr){
			pstr = (cTy_*)MallocMem(memlength+sizeof(cTy_));
		}

		if(pmemsize) *pmemsize = memlength;
		memset(pstr,0x00,memlength);
		memcpy(pstr,str.c_str(),memlength);
		return pstr;
	}

	template<class cTy_ >
	static cTy_* CopyNewStr(const basic_string<cTy_>& szOrgStr, int * pmemsize = NULL)
	{
		cTy_* pBuff =CopyPSTRFromSTRING<cTy_>(NULL,szOrgStr,0x7fffffff,pmemsize);

		return pBuff;
	}

	template<class Ty_>
	static ARRAYLIST<Ty_> * ConvertFromVector(vector<Ty_> & vecfindlist)
	{
		ARRAYLIST<Ty_> * lp = (ARRAYLIST<Ty_> *)NCL::MakeNSFormDwordBase((neocolib::uint)vecfindlist.size(),sizeof(Ty_));

		FORNLOOP(lp->length){
			lp->array[nCount] = vecfindlist[nCount];
		}
		return lp;
	}

	template<class Ty_>
	static vector<Ty_>  ConvertVectorFromArray(ARRAYLIST<Ty_>* lp)
	{
		vector<Ty_> ret ;


		FORNLOOP(lp->length){
			ret.push_back(lp->array[nCount]);
		}
		return ret;
	}

	static void UnitTest();
};


typedef void (* LPFNFREEF )(pvoid) ;

//class EXTERN_API CMem{
class CMem{
protected:
	pvoid m_lpBuff;
	LPFNFREEF m_pfnFree;
public:
	CMem();
	CMem(pvoid lpIn);
	~CMem();
	pvoid GetLpBuff();
	pvoid Return();
	void Free();
	void SetFreeFn(LPFNFREEF pfnFree);
	CMem & operator =(pvoid lpin) ;       // operator+()

};

template<class T>
class  CMANMEM : public CMem
{
private:
	T *m_lpTBuff;
public:
	CMANMEM(){
		m_lpTBuff = (T*)CMem::GetLpBuff();
	}

	CMANMEM(pvoid lpIn):CMem(lpIn){
		m_lpTBuff = (T*)CMem::GetLpBuff();
	}

	~CMANMEM(){
	}
	T *GetLpBuff(){
		return (T*)CMem::GetLpBuff();

	}
	T *Return(){
		return (T*)CMem::Return();

	}

	operator T*()  throw()
	{
		return( (T*)CMem::GetLpBuff() );
	}

	CMANMEM<T> & operator =(pvoid lpin) {
		m_lpBuff = lpin;
		m_lpTBuff = (T*)CMem::GetLpBuff();
		return *this;
	};
};

template<class T>
class  CARRAY :CMem
{
public:

	CARRAY(int length = 0, bool isHaveToFreePointArray = false):CMem()
	{
		m_lpBuff = Malloc(length,NULL);
		m_isHaveToFreePointArray = isHaveToFreePointArray;
	}

	CARRAY(pvoid lpBuff)
	{
	}

	CARRAY(pvoid lpBuff, bool isHaveToFreePointArray = false):CMem(lpBuff)
	{
		m_isHaveToFreePointArray = isHaveToFreePointArray;
	}

	~CARRAY()
	{
		if (m_isHaveToFreePointArray) {
			if (m_lpBuff) {
				ARRAYLIST<T> * lpArrayList = (ARRAYLIST<T> *)CMem::GetLpBuff();
				NCL::FreeLPArray((LPPVOIDARRAY)lpArrayList);
				m_lpBuff = NULL;
			}
		}
	}

	static ARRAYLIST<T> * Malloc(int length, pvoid reaaloc = NULL)
	{
		return (ARRAYLIST<T> *)NCL::MakeNSFormDwordBase(length,sizeof(T),reaaloc);
	}

	void ReAlloc(int length)
	{
		m_lpBuff = Malloc(length,CMem::GetLpBuff());
		SetLength(length);
	}

	void Add( T memb)
	{
		neocolib::uint length = GetLength();

		ReAlloc(length+1);
		T* last = GetArray();
		last[length] = memb;
	}


	ARRAYLIST<T> *GetArrayList()
	{
		return (ARRAYLIST<T> *)CMem::GetLpBuff();
	}

	T *GetArray()
	{
		ARRAYLIST<T> * lpArrayList = (ARRAYLIST<T> *)CMem::GetLpBuff();

		if (!lpArrayList)
			return NULL;
		else
			return lpArrayList->array;
	}

	neocolib::uint GetLength()
	{
		ARRAYLIST<T> * lpArrayList = (ARRAYLIST<T> *)CMem::GetLpBuff();

		if (!lpArrayList)
			return -1;
		else
			return lpArrayList->length;
	}

	bool SetLength(neocolib::uint length)
	{
		ARRAYLIST<T> * lpArrayList = (ARRAYLIST<T> *)CMem::GetLpBuff();

		if (!lpArrayList)
			return false;

		lpArrayList->length = length;
		return true;
	}

	ARRAYLIST<T> *Return()
	{
		return (ARRAYLIST<T> *)CMem::Return();
	}

	operator ARRAYLIST<T>*()  throw()
	{
		return GetArrayList();
	}

private:
	bool m_isHaveToFreePointArray;

};

}


