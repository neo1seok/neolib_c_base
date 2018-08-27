#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#include "ShareMem.h"

#include "neoCoLib.h"
//#include "NeoSystemLib.h"
#include "neoDebug.h"

#define WM_WINAUDIT	(WM_USER + 0x00003013)
#define WA_SERVICENAME L"winaudit"

//CSHAREMEM * m_lpShareMem =NULL;
#if 0
void CNeoDebug::SendLog2WAService(TCHAR * strLog)
{
#if 1
	if(!m_lpShareMem) m_lpShareMem = new CSHAREMEM(L"winaudit");

	HWND hWndDst = FindWindow(NULL,WA_SERVICENAME);
	if(hWndDst){
		m_lpShareMem->SendShareMemory(hWndDst,WM_WINAUDIT,strLog);
	}

	m_lpShareMem->ReleaseSharedMem();
#endif



}

void CNeoDebug::printOut(WCHAR *szView)
{

	FILE * fp;
	fp = _wfopen(m_szLogoutName,L"rb");
	if(!fp){
		fp = _wfopen(m_szLogoutName,L"wb");
		fwprintf(fp,L"\xfeff");
		fclose(fp);

	}
	else {
		fclose(fp);
	}

	fp = _wfopen(m_szLogoutName,L"ab");
	if(!fp) return ;
	fwprintf(fp,L"%s\r\n",szView);
	fclose(fp);

}


WCHAR CNeoDebug::m_szLogoutName[] = {0,};

void CNeoDebug::debugStart()
{
	time_t ctime;
	time(&ctime);
	NEO_SETFN(CNeoDebug::SendLog2WAService);
	wsprintf(m_szLogoutName,L"c:/out/%s.YYYYMMDD.log",NeoCoLib::GetModuleName2(GetCurrentProcessId()));
	NeoCoLib::ChangeDate2Str(m_szLogoutName,&ctime);

}
#endif

//#ifdef WIN32

//#endif

CMem::CMem()
	:m_lpBuff(NULL)
{
	m_pfnFree = NeoCoLib::FreeMem;

}

CMem::CMem(neocolib::pvoid lpIn)
{
	m_lpBuff = lpIn;
	m_pfnFree = NeoCoLib::FreeMem;
}

CMem::~CMem(){
	//if(m_lpBuff) CCA::FreeMem(m_lpBuff);
	//m_lpBuff = NULL;
	Free();


}

neocolib::pvoid CMem::GetLpBuff(){
	return m_lpBuff;

}

neocolib::pvoid CMem::Return(){

	neocolib::pvoid pRet = m_lpBuff;
	m_lpBuff = NULL;
	return pRet;

}
void CMem::Free(){
	if(m_lpBuff && m_pfnFree) m_pfnFree(m_lpBuff);
	m_lpBuff = NULL;

}
void CMem::SetFreeFn(LPFNFREEF pfnFree){
	m_pfnFree = pfnFree;
}
CMem& CMem::operator =(neocolib::pvoid lpin) {
	m_lpBuff = lpin;
	return *this;
} 


// 
// 
// void FreeArrayMemPPTCHAR(neocolib::pvoid lpVoid)
// {
// 	NeoCoLib::FreeStringArray((WCHAR **) lpVoid);
// }




#if 0
CARRAY<class T>::CARRAY(int length):CMem(){
	m_lpBuff = Malloc(length,NULL);
}
CARRAY<class T>::CARRAY( ){
	m_lpBuff = Malloc(0,NULL);
}

CARRAY<class T>::CARRAY(neocolib::pvoid lpBuff ){
}
CARRAY<class T>::~CARRAY(){
}

// template<class T>
// ARRAYLIST< T> * CARRAY<T>::Malloc(int length,neocolib::pvoid reaaloc){
// 	int unitsize = sizeof(T);
// 	return (ARRAYLIST<T> *)CCA::MakeNSFormDwordBase(length,unitsize,reaaloc);
// }

template<class T>
void CARRAY< T>::ReAlloc(int length){
	m_lpBuff = Malloc(length,CMem::GetLpBuff());
}
template<class T>
void CARRAY< T>::Add( T memb){
	uint length = GetLength();

	ReAlloc(length+1);
	SetLength(length+1);
	T* last = GetArray();
	last[length] = memb;



}

template<class T>
ARRAYLIST<T> *CARRAY< T>::GetArrayList(){
	return (ARRAYLIST<T> *)CMem::GetLpBuff();
}
template<class T>
T *CARRAY< T>::GetArray(){
	ARRAYLIST<T> * lpArrayList = (ARRAYLIST<T> *)CMem::GetLpBuff();
	if(!lpArrayList) return NULL;
	return lpArrayList->array;
}

template<class T>
uint CARRAY< T>::GetLength(){
	ARRAYLIST<T> * lpArrayList = (ARRAYLIST<T> *)CMem::GetLpBuff();
	if(!lpArrayList) return -1;
	return lpArrayList->length;
}
template<class T>
bool CARRAY< T>::SetLength(uint length){
	ARRAYLIST<T> * lpArrayList = (ARRAYLIST<T> *)CMem::GetLpBuff();
	if(!lpArrayList) return FALSE;

	lpArrayList->length = length;
	return TRUE;
}

template<class T>
ARRAYLIST<T> *CARRAY< T>::Return(){
	return (ARRAYLIST<T> *)CMem::Return();
}
#endif
#if 0

template<class T>
CManMem<T>::CManMem()
	:m_lpBuff(NULL)
{

}

template<class T>
CManMem<T>::CManMem(neocolib::pvoid lpIn)
{
	m_lpBuff = (T )lpIn;
}

template<class T>
CManMem<T>::~CManMem(){
	if(m_lpBuff) CCA::FreeMem(m_lpBuff);


}
template<class T>
T CManMem<T>::GetLpBuff(){
	return m_lpBuff;

}

template<class T>
T CManMem<T>::Return(){
	T pRet = m_lpBuff;
	return pRet;

}

template<class T>
T CManMem<T>::operator =(neocolib::pvoid lpin) {
	m_lpBuff =(T)lpin;

	return m_lpBuff;

}
#endif

