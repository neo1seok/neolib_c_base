
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <cassert>
#include <locale>
//#include <Turboc.h>
#include <time.h>
#include <stdarg.h>

#include<map>
//#error asdfsdf
//#include <tchar.h>
#include <chrono>
#include <thread>

#include "neoCoLib.h"
#include "neoDebug.h"


//#include <process.h>

//#include "NeoSystemLib.h"

//  #include <boost/filesystem.hpp>
//  #include <boost/filesystem/fstream.hpp>
//  #include <boost/algorithm/string/predicate.hpp>
// #include <boost/locale.hpp>

#include "utf8.h"
using namespace neocolib;
#if 0

#ifndef _UTF8_CODECVT_H_
#define _UTF8_CODECVT_H_

#define BOOST_UTF8_BEGIN_NAMESPACE namespace boost { namespace mylib { 
#define BOOST_UTF8_END_NAMESPACE }} 
#define BOOST_UTF8_DECL 
#pragma warning(push)
#pragma warning(disable : 4100) 
#include "utf8_codecvt_facet.hpp" 
#pragma warning(pop) 
#undef BOOST_UTF8_DECL 
#undef BOOST_UTF8_END_NAMESPACE 
#undef BOOST_UTF8_BEGIN_NAMESPACE 

typedef boost::mylib::utf8_codecvt_facet utf8_codecvt;

#endif // _UTF8_CODECVT_H_
#endif

#include "strman.hpp"


// int _Strcount = 0;
// neocolib::tchar _staticTStrArray[NUMMAXSTR][MAX_NCL_PATH+1] = {0,};
 //template<class cTy_,class cTySTR = basic_string<cTy_, char_traits<cTy_>, allocator<cTy_> > >
//  cTy_ * CopySTR(cTySTR str){
//  	int memsize = 0;
//  	cTy_*ret =  GetString(str.size(),&memsize);
//  	memcpy(ret,str.c_str(),memsize);
//  	return ret;
//  }

STRMAN<tchar> m_tstrman;
STRMAN<char> m_strman;
STRMAN<wchar> m_wstrman;

p_circular_tstr NeoCoLib::GetStaticTStr(const tstring &str)
{

// 	int index = _Strcount%NUMMAXSTR;
// 	_Strcount++;
	tchar*ret =  m_tstrman.CopySTR(str);
	//CopyPTSTRFromTSTRING(ret,str);
	//STRNCPY(_staticStrArray[index],str.c_str(),MAX_NCL_PATH+1);
	// 	int memlength = GetStrMemSize(str,MAX_NCL_PATH);
	// 	//int memlength = sizeof(TCHAR)*(min(str.c_str()+1,MAX_NCL_PATH+1));
	// 	memcpy(_staticStrArray[index],str.c_str(),memlength);
	return ret;
}

p_circular_wstr NeoCoLib::GetStaticWStr(const wstring &str)
{

	// 	int index = _Strcount%NUMMAXSTR;
	// 	_Strcount++;
#ifdef __USE_CIRCULAR_MEMORY__
#error fdsfs
	wchar*ret =  m_wstrman.CopySTR(str);
	return ret;
#else
	return str;
#endif
	//CopyPTSTRFromTSTRING(ret,str);
	//STRNCPY(_staticStrArray[index],str.c_str(),MAX_NCL_PATH+1);
	// 	int memlength = GetStrMemSize(str,MAX_NCL_PATH);
	// 	//int memlength = sizeof(TCHAR)*(min(str.c_str()+1,MAX_NCL_PATH+1));
	// 	memcpy(_staticStrArray[index],str.c_str(),memlength);
	
}

p_circular_str NeoCoLib::GetStaticStr(const string &str)
{

	// 	int index = _Strcount%NUMMAXSTR;
	// 	_Strcount++;
	#ifdef __USE_CIRCULAR_MEMORY__
	char*ret =  m_strman.CopySTR(str);
	//CopyPTSTRFromTSTRING(ret,str);
	//STRNCPY(_staticStrArray[index],str.c_str(),MAX_NCL_PATH+1);
	// 	int memlength = GetStrMemSize(str,MAX_NCL_PATH);
	// 	//int memlength = sizeof(TCHAR)*(min(str.c_str()+1,MAX_NCL_PATH+1));
	// 	memcpy(_staticStrArray[index],str.c_str(),memlength);
	return ret;
#else
	return str;
#endif
}




//neocolib::char _staticwStrArray[NUMMAXSTR][MAX_NCL_PATH+1] = {0,};


//TCHAR _staticStrArray[NUMMAXSTR][MAX_NCL_PATH+1] = {0,};

std::wstring 
mbs_to_wcs(std::string const& str, std::locale const& loc = std::locale())
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t state = std::mbstate_t();
	std::vector<wchar_t> buf(str.size() + 1);
	char const* in_next = str.c_str();
	wchar_t* out_next = &buf[0];
	std::codecvt_base::result r = codecvt.in(state, 
		str.c_str(), str.c_str() + str.size(), in_next, 
		&buf[0], &buf[0] + buf.size(), out_next);
	if (r == std::codecvt_base::error)
		throw std::runtime_error("can't convert string to wstring");   
	return std::wstring(&buf[0]);
}



std::string 
wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale())
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);

	std::mbstate_t state = std::mbstate_t();
	std::vector<char> buf((str.size() + 1) * codecvt.max_length());
	wchar_t const* in_next = str.c_str();
	char* out_next = &buf[0];
	std::codecvt_base::result r = codecvt.out(state, 
		str.c_str(), str.c_str() + str.size(), in_next, 
		&buf[0], &buf[0] + buf.size(), out_next);
	if (r == std::codecvt_base::error)
		throw std::runtime_error("can't convert wstring to string");   
	return std::string(&buf[0]);
}

std::wstring 
utf8_to_ucs(const std::string & str)
{
	string line = str.c_str();
	string::iterator  end_it = utf8::find_invalid(line.begin(), line.end());
	if (end_it != line.end()) {

	}


	// Get the line length (at least for the valid part)
	int length = (int)utf8::distance(line.begin(), end_it);

	// Convert it to utf-16
	vector<wchar_t> utf16line;
	utf8::utf8to16(line.begin(), end_it, back_inserter(utf16line));
	wstring wss(&utf16line[0],length);
	return wss;
}

std::string 
ucs_to_utf8(std::wstring const& wstr)
{
	string utf8line; 
	utf8::utf16to8(wstr.begin(), wstr.end(), back_inserter(utf8line));
	return utf8line;
}


std::string 
utf8_to_mbs(std::string & line, std::locale const& loc = std::locale())
{
#if 0
	const string::iterator  end_it = utf8::find_invalid(line.begin(), line.end());
	if (end_it != line.end()) {

	}


	// Get the line length (at least for the valid part)
	int length = utf8::distance(line.begin(), end_it);

	// Convert it to utf-16
	vector<wchar_t> utf16line;
	utf8::utf8to16(line.begin(), end_it, back_inserter(utf16line));

#endif	
	wstring wss = utf8_to_ucs(line);

	//wstring wss(&utf16line[0],length);




	return wcs_to_mbs(wss,loc);
}

std::string 
mbs_to_utf8(std::string const& str, std::locale const& loc = std::locale())
{
	wstring strutf16 = mbs_to_wcs(str,loc);
// 	string utf8line; 
// 	utf8::utf16to8(strutf16.begin(), strutf16.end(), back_inserter(utf8line));

	return ucs_to_utf8(strutf16);
}

#if 0
wchar * convwstring2wstr(const wstring& orgstr){
	int memsize = sizeof(wchar)*(orgstr.size()+1);
	CMANMEM<wchar> cmndata = (char *)NCL::MallocMem(memsize);

	memcpy(cmndata.GetLpBuff(),orgstr.c_str(),memsize);
	return cmndata.Return();
	
}

char * convstring2str(const string& orgstr){
	CMANMEM<char> cmndata = (char *)NCL::MallocMem(orgstr.size()+1);
	memcpy(cmndata.GetLpBuff(),orgstr.c_str(),orgstr.size()+1);
	return cmndata.Return();
}


int NeoCoLib::GetStrMemSize(const tstring &str,int maxSize,int unitsize )
{
	return unitsize*(min((int)str.size()+1,maxSize+1));
}

void NeoCoLib::CopyPTSTRFromTSTRING(neocolib::ptstr pstr,const tstring& str,int maxSize)
{
	int memlength = GetStrMemSize(str,maxSize);
	//int memlength = sizeof(TCHAR)*(min(str.c_str()+1,MAX_NCL_PATH+1));
	memcpy(pstr,str.c_str(),memlength);

}

void NeoCoLib::CopyPSTRFromSTRING(char* pstr,const string& str,int maxSize){
	//int maxstrsize = 3*(str.size()+1);
	int memlength = (min((int)str.size()+1,maxSize+1));
	memcpy(pstr,str.c_str(),memlength);
}
void NeoCoLib::CopyPWSTRFromWSTRING(wchar* pstr,const wstring& str,int maxSize){

	int memlength = sizeof(wchar)*(min((int)str.size()+1,maxSize+1));
	memcpy(pstr,str.c_str(),memlength);
}
#endif

//static LP_STATIC_TSTR GetStaticStr(const STRING &str)


bool _isTotalMemCheck;
void MNode_Init();
void MNode_Add(pvoid lpmem,int memSize);
void MNode_Remove(pvoid lpmem);
void MNode_GetMemInfo(int & totalSize,int & nodeNum);
void MNode_NodeState();


#if 0

template<class Ty_>
ARRAYLIST<Ty_> * ConvertFromVector(vector<Ty_> & vecfindlist)
{

	ARRAYLIST<Ty_> * lp = (ARRAYLIST<Ty_> *)NCL::MakeNSFormDwordBase((neocolib::uint)vecfindlist.size(),sizeof(Ty_));

	FORNLOOP(lp->length){
		lp->array[nCount] = vecfindlist[nCount];
	}
	return lp;
}
#endif

LPWSTRARRAY NeoCoLib::ConvertStrArrayFromVECSTRING( VECSTRING &vecstr)
{
	LPWSTRARRAY ret = (LPWSTRARRAY)NCL::MakeNSFormDwordBase((neocolib::uint)vecstr.size(),sizeof(neocolib::ptstr));
	int count = 0;
	FORVECTER(VECSTRING,vecstr){
		ret->array[count] = NCL::CopyNewStr(*piter);
		count++;
	}

	return ret;


}

VECSTRING NeoCoLib::ConvertVECSTRINGFromStrArray( LPWSTRARRAY  lpwstrarray)
{
	VECSTRING vecstr;

	FORNLOOP(lpwstrarray->length){

		vecstr.push_back(lpwstrarray->array[nCount]);

	}
	return vecstr;
}


pvoid NeoCoLib::MakeNSFormDwordBase(neocolib::uint nLength,neocolib::uint nUnitStructure,pvoid reAloococ,neocolib::uint* lpmemSize){
#if 0	
	pvoid lpRetRegValue = NULL;
	neocolib::uint nMemSize = sizeof(neocolib::uint) + (nLength)*nUnitStructure ;
	if(lpmemSize) *lpmemSize = nMemSize;
	lpRetRegValue = CCA::MallocMem(nMemSize,reAloococ);


	if(!reAloococ){
		lpRetRegValue = malloc(nMemSize);
		memset(lpRetRegValue,0x00,nMemSize);
	}
	else
		lpRetRegValue = realloc(reAloococ,nMemSize);
#endif	
	pvoid lpRetRegValue = NeoCoLib::MakeNSFormMInfoBase(nLength,sizeof(neocolib::uint),nUnitStructure, reAloococ, lpmemSize);
	memcpy(lpRetRegValue,&nLength,sizeof(neocolib::uint));
	return lpRetRegValue;
}

pvoid NeoCoLib::MakeNSFormMInfoBase(neocolib::uint nLength,neocolib::uint bfUnitMemSize,neocolib::uint nUnitStructure,pvoid reAloococ,neocolib::uint* lpmemSize){

	pvoid lpRetRegValue = NULL;

	neocolib::uint nMemSize = bfUnitMemSize + (nLength)*nUnitStructure;
	if(lpmemSize) *lpmemSize = nMemSize;

	lpRetRegValue = NeoCoLib::MallocMem(nMemSize+nUnitStructure,reAloococ);
		//neo1seok 2013.09.11 realloc 시에 여유버퍼가 없으면 이상이 생긴다.


#if 0
if(!reAloococ){
		lpRetRegValue = malloc(nMemSize);
		memset(lpRetRegValue,0x00,nMemSize);
	}
	else
		lpRetRegValue = realloc(reAloococ,nMemSize);
#endif

	//memcpy(lpRetRegValue,&nLength,sizeof(neocolib::uint));
	return lpRetRegValue;
}

#if 0

CString & CCA::Make_LOG_Form_Keyboard(LPLOGKEYDATAINFO lpLogKeyDataInfo)
{
	static CString strRet;
	strRet = L"";
	strRet.Format(L"%d\x2%u\x2",lpLogKeyDataInfo->uiProcessID,lpLogKeyDataInfo->hOccurredInWnd);

	TCHAR ** ppCharStart = (TCHAR ** )&lpLogKeyDataInfo->pstrEvent;
	lpLogKeyDataInfo->pEnd = NULL;
	for(TCHAR **ppChar =  ppCharStart ; *ppChar ; ppChar ++){
		strRet += *ppChar;
		strRet += (TCHAR)0x0002;
	}

	return strRet;
}
#endif

#if 1


#if 0
neocolib::ptstr NeoCoLib::GetStrForm(neocolib::pctstr fmt, ...)
{
	
	//static count = 0;
	va_list args;
	;// =NULL;

	//tr_len = SPRINTF(szTemp,_N("%s:"),prjName);
	va_start(args, fmt);
	ptstr ret = GetNewStrFormV(fmt,args);
	va_end(args);
	return ret;
}
#endif

p_circular_tstr NeoCoLib::GetStaticStrForm(neocolib::pctstr fmt, ...)
{
	//static count = 0;
	//MUTEXLOCK(GetStaticStrForm);
// 	static TCHAR szRet[MAX_NCL_PATH];
// 	szRet[0]= NULL;
	va_list args;
	

	//tr_len = SPRINTF(szTemp,_N("%s:"),prjName);
	va_start(args, fmt);
	p_circular_tstr str = GetNewStrFormV(fmt,args);
	va_end(args);
	//tstring str = buffer;

// 	int memlength = GetStrMemSize(str,MAX_NCL_PATH);
// 	memcpy(szRet,str.c_str(),memlength);
// 	CopyLPTSTRFromSTRING();
// 	//lstrcpyn(szRet,buffer,MAX_NCL_PATH-1);
// 
// 	szRet[MAX_NCL_PATH-1] = NULL;

	return str;
}

p_circular_tstr NeoCoLib::GetStaticTimeForm(time_t *pltime,neocolib::pctstr fmt, ...)
{
	//static count = 0;
	//MUTEXLOCK(GetStaticStrForm);
// 	static wchar szRet[MAX_NCL_PATH];
// 	szRet[0]= NULL;
	va_list args;
	;

	//tr_len = SPRINTF(szTemp,_N("%s:"),prjName);
	va_start(args, fmt);
	tstring strbuff = GetNewStrFormV(fmt,args);
	va_end(args);
	//tstring strbuff = cmn.GetLpBuff();
	
	ChangeDate2StrEx(strbuff,pltime);

// 	lstrcpyn(szRet,strbuff.c_str(),MAX_NCL_PATH-1);
// 	szRet[MAX_NCL_PATH-1] = NULL;

	return GetStaticTStr(strbuff);;
}


neocolib::p_circular_tstr NeoCoLib::GetNewStrFormV(neocolib::pctstr fmt, va_list args)
{
	//STRING buffer;
	//static count = 0;
	int tr_len=0;
	;

	tr_len = MAX_NCL_PATH;//VSCPRINTF( fmt, args ) + 1;
	//buffer.resize(tr_len);

	//CMANMEM<tchar> cmn = (tchar *)MallocMem(sizeof(tchar)*tr_len);
	ptstr buffer = (tchar *)MallocMem(tr_len);
	int length = VSPRINTF( buffer, fmt,args );
	buffer = (tchar *)MallocMem( sizeof(tchar)*(length+1),buffer);
	CMANMEM<tchar> cmn = buffer;

	return GetStaticTStr((tstring)buffer);
}

#endif


void NeoCoLib::FreeMem(pvoid lpMem)
{
	//MNode_Remove(lpMem);
	free(lpMem);
}

pvoid NeoCoLib::MallocMem(neocolib::uint memSize,pvoid lpMem)
{
	pvoid ret = NULL;
	
//	MNode_Init();
	
	if(lpMem){
		//MNode_Remove(lpMem);
		ret = realloc(lpMem,memSize);
	}
	else{
		ret = malloc(memSize);
		memset(ret,0x00,memSize);
	}
	//MNode_Add(ret,memSize);

	return ret;
}

void NeoCoLib::GetMemoryInfo(int&totalSize,int &nNode)
{
	MNode_GetMemInfo(totalSize,nNode);
}

void NeoCoLib::MemNodeState()
{
	MNode_NodeState();
}


// [DSNTECH] 신원석(neo1seok) 2011.06.24 : begin
p_circular_tstr NeoCoLib::GetTimeStr(const tstring&  szStr,time_t * pltime){
//	MUTEXLOCK(GetTimeStr);
	tm curtim;
	tm * lt = &curtim;

	
	//localtime_s( lt,pltime );
	
	curtim = *localtime(pltime);

	//time(pltime);

	const tchar      *month[] = { _t("Jan"), _t("Feb"), _t("Mar"),_t("Apr"),_t( "May"),_t( "Jun"),_t( "Jul"),_t("Aug"),_t("Sep"),_t("Oct"),_t("Nov"),_t("Dec")};


	tchar szTemp[32];
	
	if(!szStr.compare(_t("YYYY"))){
		SPRINTF(szTemp,_t("%04d"),lt->tm_year+1900);
	}
	else if(!szStr.compare(_t("MMM"))){
		SPRINTF(szTemp,_t("%s"),month[lt->tm_mon]);
	}
	else if(!szStr.compare(_t("MM"))){
		SPRINTF(szTemp,_t("%02d"),lt->tm_mon+1);
	}
	else if(!szStr.compare(_t("DD"))){
		SPRINTF(szTemp,_t("%02d"),lt->tm_mday);
	}
	else if(!szStr.compare(_t("HH"))){
		SPRINTF(szTemp,_t("%02d"),lt->tm_hour);
	}
	else if(!szStr.compare(_t("mm"))){
		SPRINTF(szTemp,_t("%02d"),lt->tm_min);
	}
	else if(!szStr.compare(_t("ss"))){
		SPRINTF(szTemp,_t("%02d"),lt->tm_sec);
	}
	else if(!szStr.compare(_t("YY"))){
		SPRINTF(szTemp,_t("%02d"),lt->tm_year+1900-2000);
	}
	return GetStaticTStr(szTemp);
}


//[DSNTECH] 신원석(neo1seok) 2011년 6월 24일 금요일 :문자열의 YYYY DDDD 등의 문자열을 날짜 값으로 변환한다.  
//\5 ~ \5 사이의 문자가 있다면 그 사이의 문자가 날짜이다. 
 bool  NeoCoLib::ChangeDate2Str(tstring &str ,time_t *pltime){

//	MUTEXLOCK(ChangeDate2Str);
	//if(!str) return false;
	//tm curtim;
	//tm * lt = &curtim;
	//localtime_s( lt,pltime ); 
	if(!pltime){
		static time_t ltime;
		time(&ltime);
		pltime = &ltime;
	}

	tchar*  szForm[] = { _t("YYYY"),_t("MMM"),_t("MM"),_t("DD"),_t("HH"),_t("mm"),_t("ss"),_t("YY"),NULL};//순서주의 
	tstring orgStr  = str ;
	tchar ** ppChar ;

	for(ppChar = szForm ; *ppChar ; ppChar++){
		tstring ret  ;
		str =  RecplaceStr(str,* ppChar,GetTimeStr(* ppChar,pltime));
//		CMem cMem = ret;
		//if(!ret)  return false;
		//lstrcpy(str,ret);
		//str = cmn.GetLpBuff();
		//free(ret);
	}

	return	true;

}

neocolib::p_circular_tstr NeoCoLib::CopyInterStr(const tstring& pOrgStr,tchar ch){
	tstring outstr;

	size_t st = pOrgStr.find(ch,0);
	size_t ed = pOrgStr.find(ch,st+1);
	if(st==(size_t)-1) return NULL;

	outstr = pOrgStr.substr(st+1, ed-st-1);

	return GetStaticTStr(outstr);
	
}

bool NeoCoLib::ChangeDate2StrEx(tstring & str ,time_t *pltime){
	//if(!str) return false;
	//tm curtim;
	//tm * lt = &curtim;
	//localtime_s( lt,pltime ); 
	//bool isReplaceNull = false;

	tstring ret ;

	//CMANMEM<TCHAR> cmmStrInter =  CopyInterStr(str,L'\5');
	//if(!cmmStrInter.GetLpBuff()) return false;

	size_t st = str.find_first_of(_t('\5'));
	size_t ed = str.find_last_of (_t('\5'));

	if(st == (size_t)-1) {
		ChangeDate2Str(str,pltime);
		return true;
	}
	tstring strmid ;
	strmid = CopyInterStr(str,_t('\5'));
	//strmid = cmn.GetLpBuff();

#if 0

	STRING pSt = NULL,*pEd = NULL,*pForm = NULL;;

	STRING pChar = str;
	while(pChar = StrChr(pChar,L'\5')){
		if(!pSt) pSt = pChar;
		else
			pEd = pChar;
		pChar++;
	}
	if(pEd) {
		*pEd = NULL;
	}

	if(!pSt){
		ChangeDate2Str(str,pltime);
		return true;
	}
#endif

	ChangeDate2Str(strmid,pltime);





	//if(pEd ) *pEd = L'\5';

	str = str.replace(st,ed-st+1,strmid);


	#if 0
CMANMEM<TCHAR> cmmRepStr = RecplaceStr(str,L"\5",L"");
	lstrcpy(str,cmmRepStr);
	return RecplaceStr(str,_t("\5"),_t(""));






	pSt = StrChr(str,L'\5');
	if(!pSt){
		ChangeDate2Str(str,pltime);
		return true;
	}

	pEd = StrChr(pSt+1,L'\5');
	if(!pEd) {
		pEd = str+lstrlen(str)-1;
	}
	long fomrLen = (long)(pEd - pSt) +1 ;
	pForm = (STRING)MallocMem(sizeof(TCHAR)*(fomrLen+1));
	if(!pForm) return false;
	memcpy(pForm,pSt,sizeof(TCHAR)*(fomrLen));
	pForm[fomrLen] = NULL;

	STRING pNew = CopyNewStr(pForm);
	CMem cMemNew = pNew;
	ChangeDate2Str(pNew,pltime);

	pSt = StrChr(pNew,L'\5');
	if(pSt){
		pSt++;
	}
	pEd = StrChr(pSt,L'\5');
	if(pEd) *pEd = NULL;

	STRINGpRet = RecplaceStr(str,pForm,pSt);
	CMem cMemRet = pRet;

	lstrcpy(str,pRet);
	//free(pNew);
	//free(pRet);
#endif
	return	true;
}

p_circular_tstr NeoCoLib::RecplaceStr(const tstring & pOrg,const tstring &  pszOld,const tstring &  pszNew){  

	tstring ret ;

	size_t index = 0;
	ret = pOrg;
	while(true){
		index =pOrg.find(pszOld,index);
		
		if(ISINVALIDATE(index)) break;
		ret = ret.replace(index,pszOld.size(),pszNew);
		index++;
	}
#if 0

	LPCWSTR pChar =pOrg;
	int orgLen = lstrlen(pOrg);
	int nszOldLen = lstrlen(pszNew);
	int nszNewLen = lstrlen(pszOld);
	int nCount = 0;
	while(pChar = StrStr(pChar,pszOld)){
		nCount++;
		pChar += nszNewLen;
	}

	TCHAR * pret = (TCHAR *)MallocMem(sizeof(TCHAR)*(orgLen + 1+ (nszOldLen-nszNewLen)*nCount));

	TCHAR * pTarget = pret;
	pChar = pOrg;
	while(*pChar){
		if(!StrCmpN(pChar ,pszOld,nszNewLen)){
			//StrCpyN(pTarget,pszNew,nszOldLen);
			memcpy(pTarget,pszNew,nszOldLen*sizeof(TCHAR));
			pTarget += nszOldLen;
			pChar += nszNewLen;
			continue;
		}
		*pTarget++ = *pChar++;

	}
	*pTarget = 0;		
#endif

	return GetStaticTStr(ret);
	//return CopyNewStr(ret);
}	

#if 0
neocolib::ptstr NeoCoLib::GetStrTimeForm(time_t *pltime,pctstr fmt, ...)
{
	tstring buffer;
	va_list args;
	//TCHAR* buffer =NULL;
	;

	//tr_len = SPRINTF(szTemp,_N("%s:"),prjName);
	va_start(args, fmt);
	CMANMEM<tchar> cmn = GetNewStrFormV(fmt,args);
	va_end(args);
	buffer = cmn.GetLpBuff();
	ChangeDate2StrEx(buffer,pltime);

	return CopyNewStr(buffer);

	
}
#endif







size_t GetPartPostion(const tstring& szPathName,pctstr szmarks ,bool isFirstFind){
	tstring marks = szmarks;

	size_t index = -1;

	FORVECTER(tstring,marks){
		tchar ch = *piter;
		index = isFirstFind ? szPathName.find_first_of(ch):szPathName.find_last_of(ch);
		if(!ISINVALIDATE(index) ) return index;
	}
	return index;
}

p_circular_tstr NeoCoLib::GetSubPart(const tstring& szPathName,pctstr szmarks,bool isFirstFind,bool isFrontPart)
{
	tstring outstr;

	size_t index = GetPartPostion(szPathName,szmarks,isFirstFind);
	if(ISINVALIDATE(index)) return NULL;

	size_t st = isFrontPart?0:index+1;
	size_t length = isFrontPart?index:szPathName.size()-st;
	
	outstr =  szPathName.substr(st,length);

	return GetStaticTStr(outstr);
	//return CopyNewStr(outstr);
}


p_circular_tstr NeoCoLib::GetFileNameFromPath(const tstring& szPathName)
{
//	MUTEXLOCK(GetFileNameFromPath);
	
	
	return GetSubPart(szPathName,_t("\\/"),false,false);//szPathName.substr(lastindex+1,szPathName.size()- lastindex-1);
}

p_circular_tstr NeoCoLib::GetDirNameFromPath(const tstring& szPathName)
{
//	MUTEXLOCK(GetDirNameFromPath);

	return GetSubPart(szPathName,_t("\\/"),false,true);;

#if 0


	static TCHAR szRet[MAX_NCL_PATH];
	lstrcpy(szRet,szPathName);
	const TCHAR chMark = L'\\';
	const TCHAR chMark2 = L'/';
	STRING pCharMark = NULL;
	for(STRING pChar = szRet;*pChar; pChar++){
		if(*pChar == chMark || *pChar == chMark2){
			pCharMark = pChar;
		}
	} 
	if(!pCharMark) {
		szRet[0] = NULL;
		return szRet;
	}
	*pCharMark = NULL;

	return STRING(szRet);
#endif
}


p_circular_tstr NeoCoLib::GetExtName(const tstring& szPathName)
{
//	MUTEXLOCK(GetExtName);
	



	return GetSubPart(szPathName,_t("."),false,false);;;

}
p_circular_tstr NeoCoLib::GetDriverNameFromPath(const tstring& szPathName)
{
//	MUTEXLOCK(GetExtName);

	return GetSubPart(szPathName,_t(":"),true,true);;;
}


p_circular_str   NeoCoLib::ConvWC2MB(neocolib::uint codePage ,const wstring &szOrgStr)
{
	
	string ret = wcs_to_mbs(szOrgStr);
	return GetStaticStr(ret);
#if 0
	return 
	CMANMEM<char> cmndata = (char *)MallocMem(ret.size()+1);

	memcpy(cmndata.GetLpBuff(),ret.c_str(),ret.size()+1);

	



	int lenth = WideCharToMultiByte(codePage,0,szOrgStr.c_str(),(int)szOrgStr.size()+1,NULL,0,NULL,NULL);
	CMANMEM<char> cmndata = (char *)MallocMem(lenth+1);

	WideCharToMultiByte(codePage,0,szOrgStr.c_str(),(int)szOrgStr.size()+1,cmndata.GetLpBuff(),(int)lenth,NULL,NULL);

	return cmndata.Return();
#endif	

}
p_circular_wstr NeoCoLib::ConvMB2WC(neocolib::uint codePage ,const string &szOrgStr)
{

	wstring ret = mbs_to_wcs(szOrgStr);
#if 0
	int memsize = sizeof(wchar)*(ret.size()+1);
	CMANMEM<wchar> cmndata = (char *)MallocMem(memsize);

	memcpy(cmndata.GetLpBuff(),ret.c_str(),memsize);




	int lenth = (int)MultiByteToWideChar(codePage,0,szOrgStr.c_str(),(int)szOrgStr.size()+1,NULL,0);
	CMANMEM<wchar> cmndata = (wchar*) MallocMem(sizeof(tchar)*(lenth+1));
	MultiByteToWideChar(codePage,0,szOrgStr.c_str(),(int)szOrgStr.size()+1,cmndata.GetLpBuff(),(int)lenth);
#endif
	
	return GetStaticWStr(ret);
}



p_circular_str NeoCoLib::ConvWC2UTF8(const wstring&  szOrgStr){
	std::string ret = "";
	if(!szOrgStr.empty())
		ret = ucs_to_utf8(szOrgStr);

	return GetStaticStr(ret);
}
p_circular_wstr NeoCoLib::ConvUTF82WC(const string&  szOrgStr){
	std::wstring ret = L"";
	if(!szOrgStr.empty())
		ret = utf8_to_ucs(szOrgStr);
	return GetStaticWStr(ret);;
}



int NeoCoLib::CompBin(pbyte lpCmp1,pbyte lpCmp2,neocolib::uint binsize)
{
	int count = 0;

	for(neocolib::uint i = 0 ; i < binsize ; i++,lpCmp1++,lpCmp2++)
	{
		int cmp = *lpCmp1 - *lpCmp2;
		if(cmp) {
			count++;
		}
	}

	return count;
}

#if 0
LPBYTEARRAY CCA::FindDeffByte(LPBYTE lpBaseByte,LPBYTE lpCmp,uint binsize)
{	
	

	LPBYTEARRAY lpRet = (LPBYTEARRAY)MakeNSFormDwordBase(binsize,sizeof(BYTE));

	for(uint i = 0 ; i < binsize ; i++,lpBaseByte++,lpCmp++)
	{
		LPBYTE lpCur = lpRet->pBuff + i;
		if(*lpBaseByte !=  *lpCmp) {
			*lpCur = *lpCmp;//^*lpBaseByte;
		}
	}

	return lpRet;
}
#endif
int NeoCoLib::CompBin(neocolib::uint* lpCmp1,neocolib::uint* lpCmp2,neocolib::uint dwsize)
{
	int count = 0;

	for(neocolib::uint i = 0 ; i < dwsize ; i++,lpCmp1++,lpCmp2++)
	{
		int cmp = *lpCmp1 - *lpCmp2;
		if(cmp) {
			count++;
		}
	}

	return count;
}




pvoid NeoCoLib::LoadMemory(pvoid lpOrgBuff ,pvoid lpBuff,neocolib::uint nSize){
	memcpy(lpBuff,lpOrgBuff,nSize);
	return (pbyte)lpOrgBuff + nSize;
}

pvoid NeoCoLib::SaveMemory(pvoid lpOrgBuff ,pvoid lpBuff,neocolib::uint nSize){
	memcpy(lpOrgBuff,lpBuff,nSize);
	return (pbyte)lpOrgBuff + nSize;
}


void NeoCoLib::FreeStringArray(ptstr * ppList){
	
	FreeLPP((pvoid*)ppList);
}
void NeoCoLib::FreeStrArray(LPWSTRARRAY tstrarray){

	FreeLPArray((LPPVOIDARRAY)tstrarray);
}

//template <typename pvoid>
void NeoCoLib::FreeLPP(pvoid *ppList)
{

	for(pvoid* pptmp = ppList;* pptmp;pptmp++){
		NeoCoLib::FreeMem(*pptmp);
	}
	NeoCoLib::FreeMem(ppList);


}

//template <typename pvoid>
void NeoCoLib::FreeLPArray(LPPVOIDARRAY lparray){

	for(neocolib::uint i = 0 ; i < lparray->length;i++){
		if(lparray->array[i]) NeoCoLib::FreeMem(lparray->array[i]);
	}

	NeoCoLib::FreeMem(lparray);
}

LPWSTRARRAY NeoCoLib::GetSplit2Array(const tstring& strSTr , pctstr szSep)
//LPWSTRARRAY CCA::GetSplit2Array(LPWSTR strSTr , TCHAR chSep)
{
	VECSTRING caList;
	


	//TCHAR seps[2]   ={chSep,0,};
	// Establish string and get the first token:

	CMANMEM<tchar> pTok = CopyNewStr(strSTr);

	ptstr token = STRTOK( pTok.GetLpBuff(), szSep ); // C4996
	// Note: strtok is deprecated; consider using strtok_s instead
	while( token != NULL )
	{
		// While there are tokens in "string"

		caList.push_back(token);
		token = STRTOK( NULL, szSep ); // C4996
	}
	return ConvertStrArrayFromVECSTRING(caList);
}
#if 0
//template <typename pvoid>
pvoid * CCA::ConvertLPWSTRARRAY2LPPWSTR(LPPVOIDARRAY  lpwstrarray)
{
	pvoid * ppRet = (pvoid *)CCA::MallocMem((lpwstrarray->length+1)*sizeof(pvoid));
	int i = 0; 
	for (i =0 ; i < lpwstrarray->length ; i++)
	{
		ppRet[i] = lpwstrarray->array[i];
	}
	ppRet[i] = NULL;

	return ppRet;
}


LPPVOIDARRAY  CCA::ConvertLPPWSTR2LPWSTRARRAY(pvoid * lpparray){
	CARRAY<pvoid> caList;
	pvoid * lpptemp = lpparray;

	for(;*lpptemp;lpptemp++){
		caList.Add(*lpptemp);

	}

	return caList.Return();

}




LPWSTR * CCA::GetSplit(LPWSTR strSTr , TCHAR chSep){


	LPWSTRARRAY lpwstrarray = CCA::GetSplit2Array( strSTr ,  chSep);

	STRING* ppRet = (STRING*)ConvertLPWSTRARRAY2LPPWSTR((LPPVOIDARRAY)lpwstrarray);
	CCA::FreeMem(lpwstrarray);
	return ppRet;

	
#if 0
STRING* ppStrList = (STRING*)MallocMem(sizeof(TCHAR*)*(lpwstrarray->length+2));


	STRING* ppTmp = ppStrList;

	* ppTmp = cmmStr;
	STRING pStStart = cmmStr;
	for(STRING pChar = cmmStr ;*pChar; pChar++){
		if(*pChar == chSep){
			

			*pChar = NULL;
			*ppTmp++ = CopyNewStr(pStStart);
			pStStart = pChar +1;
		}
	}
	*ppTmp++ = CopyNewStr(pStStart);
	*ppTmp++ = NULL;

	return ppStrList;
#endif
}

#endif

neocolib::uint NeoCoLib::GetNote(neocolib::uint size,tchar &ch){
	tchar szTmp[] = _t("\0KMGTPEZY");
	int num = 0;
	neocolib::uint orgsize = size;
	neocolib::uint ret = 0;
	while(orgsize >0){

		ret = orgsize;
		orgsize /= 1024;
		
		if(ret < 100 && ret >10) break;

		if(orgsize < 1) break;
		num++;
	}
	ch = szTmp[num];

	return ret;

}
p_circular_tstr NeoCoLib::GetNote(neocolib::uint size){
/*	tchar szRet[16];*/
	tstring ret = "";
	tchar ch = L'\0';
	size = GetNote(size,ch);
//	SPRINTF(szRet,_t("%d%c"),size,ch);
	ret = GetStaticStrForm(_t("%d%c"),size,ch);
	return ret;
	//return GetStaticStrForm(_t("%d%c"),size,ch);

	//return GetStaticTStr(szRet);
}

std::vector<std::string> split(const std::string& s, char seperator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring( s.substr(prev_pos, pos-prev_pos) );

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word

	return output;
}




int findbywildcard(const char * filter,int stindex ,const char *  realname){
	int count = 0;

	const char* lp2start = realname + stindex;
	const char * pchar = lp2start;

	for (;*pchar;pchar++)
	{
		const char* lp1 = filter;
		const char* lp2 = pchar + stindex;
		char ch1 = 0,ch2 = 0;
		for (;*lp1 && *lp2 ;lp1++,lp2++)
		{
			ch1 = *lp1;
			ch2 = *lp2;
			if(ch1 == '?') continue;
			if(ch1 != ch2) {
				break;
			}

		}
		if (ch1 == ch2) return (int)(pchar - lp2start);

	}
	return -1;
}

bool NeoCoLib::WildCompare(rctstr pattern, rctstr input)
{
	std::vector<std::string> ret = split(pattern,'*');
	size_t  submatch = 0;

	int size = (int)ret.size();
	int count = -1;

	FORVECTER(std::vector<std::string>,ret)
	{
		count++;

		int strlength = (int)piter->size();

		if(!piter->compare("")) continue;
		submatch = (size_t)findbywildcard(piter->c_str(), (int)submatch, input.c_str());
		//submatch = input.find(*piter, submatch);
		if (std::string::npos == submatch) return false;

		if(count == 0){

			if(submatch !=0) return false;


		}


		if(count == size-1){
			if(submatch +strlength != input.size()) return false;

		}
		submatch++;

	}

	return true;


}
void NeoCoLib::ReplaceAll (string& strSrc, const string& strFind, const string& strDest)
{
	size_t j;
	while ((j = strSrc.find(strFind)) != string::npos)
		strSrc.replace(j, strFind.length(), strDest);
}


int NeoCoLib::char2int(char input)
{
	int ret = 0;
	//NEO_DWORD(input);
	if (input >= '0' && input <= '9')
		ret = input - '0';
	if (input >= 'A' && input <= 'F')
		ret = input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		ret = input - 'a' + 10;
	//  throw Exception('Invalid input string');
	//NEO_DWORD(ret);
	return ret;
}

string NeoCoLib::BytetoHexStr(const  void * pbuff, int Blen)
{
	string str = "";
	int i;
	int j = 0;
	pbyte Barray = (pbyte)pbuff;

	for (i = 0; i < Blen; i++){
		str += (Barray[i] >= 0xA0) ? ((Barray[i] >> 4) + ('A' - 10)) : (Barray[i] >> 4) + '0';
		str += ((Barray[i] & 15) >= 10) ? ((Barray[i] & 15) + ('A' - 10)) : (Barray[i] & 15) + '0';
	}
	return str;

}
string NeoCoLib::VecBytetoHexStr(const  VECBYTE &vecbyte)
{
	
	return BytetoHexStr(V2A(vecbyte), vecbyte.size());

}

int  test(char p1, char p2){
	NEO_DEBUG("%x,%x \n", p1, p2);
	return 0;
}
string NeoCoLib::HexStr2AsciiString(const string & str){
	string ret = "";
	VECBYTE  vecbyte = NeoCoLib::HexStr2Byte(str);
	return (char *)V2A(vecbyte);


}

neocolib::VECBYTE  NeoCoLib::HexStr2Byte(const string & str)
{
	neocolib::VECBYTE ret;
	char * src = (char*)str.c_str();

	while (*src && src[1])
	{
		char fr = *src;
		char bk = *(src+1);
		nbyte bfr = char2int(fr);
		nbyte bbk = char2int(bk);

		//neocolib::nbyte aaa = char2int(*src++) * 16 + char2int(*src++);

		//neocolib::nbyte aaa = char2int(bfr) * 16 + char2int(bk);
		neocolib::nbyte aaa = bfr * 16 + bbk;
		src += 2;

		//NEO_DWORD(aaa);
		ret.push_back(aaa);

	}
	return ret;

}
int  NeoCoLib::HexStr2ByteArray(const string & str,void *pbyte)
{
	int retsize = 0;
	VECBYTE vecbyte = HexStr2Byte(str);
	retsize = vecbyte.size();
	if (pbyte){

		if(retsize>0) memcpy(pbyte, V2A(vecbyte), retsize);
	}
	

	return retsize;

}
int  NeoCoLib::ByteToByteArray(const  VECBYTE &vecbyte,  void * Barray)
{
	int retsize = 0;
	
	retsize = vecbyte.size();
	memcpy(Barray, V2A(vecbyte), retsize);
	return retsize;
}
VECBYTE NeoCoLib::ByteArrayToByte(const  void * Barray, int Blen)
{

	VECBYTE ret;

	AppendVector(ret, (pbyte)Barray, Blen);
	return ret;

}

string NeoCoLib::ByteToBitsString(unsigned char byte)
{
	string ret;

	const char * bitstr[] = {
		"0000",
		"0001",
		"0010",
		"0011",
		"0100",
		"0101",
		"0110",
		"0111",
		"1000",
		"1001",
		"1010",
		"1011",
		"1100",
		"1101",
		"1110",
		"1111"
	};


	nbyte lo = byte & 0x0F;
	nbyte hi = (byte & 0xF0)>>4;


	
	ret += bitstr[hi];
	ret += bitstr[lo];


	return ret;

}


short NeoCoLib::InverSHORT(short org)
{


	return MAKE_WORD(HI_BYTE(org), LO_BYTE(org));

}
int NeoCoLib::InverINT(int org){

	short iHI = InverSHORT(HI_WORD(org));
	short iLow = InverSHORT(LO_WORD(org));

	return MAKE_LONG(iHI, iLow);
}


static const unsigned char BitReverseTable256[] =
{
	0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
	0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
	0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
	0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
	0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
	0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
	0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
	0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
	0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
	0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
	0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
	0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
	0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
	0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
	0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
	0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

unsigned char NeoCoLib::InverBitsPerByte(unsigned char byte)
{

	return BitReverseTable256[byte];
}

void NeoCoLib::InverBits(void  *  buff, int length)
{
	VECBYTE retbyte;
	unsigned char *  ptmp = (unsigned char *)buff+length-1;

	for (size_t i = 0; i < length; i++, ptmp--)
	{
		retbyte.push_back(InverBitsPerByte(*ptmp));
	}
	memcpy(buff, V2A(retbyte), length);
	
}

int NeoCoLib::countBits(unsigned char byte)
{
	static const int NIBBLE_LOOKUP[16] =
	{
		0, 1, 1, 2, 1, 2, 2, 3,
		1, 2, 2, 3, 2, 3, 3, 4
	};


	return NIBBLE_LOOKUP[byte & 0x0F] + NIBBLE_LOOKUP[byte >> 4];
}
int  NeoCoLib::countBitsFromBytes(unsigned char * pbyte, int bytecount)
{
	int ret = 0;
	FORNLOOP(bytecount){
		ret += countBits(*pbyte);
		pbyte++;
	}

	return ret;
}

void NeoCoLib::Sleep(int ms){
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

#if 0

// 문자열 우측 공백문자 삭제 함수
LPWSTR CCA::rtrim(LPWSTR s) 
{
	CMANMEM<TCHAR > cmn = CCA::GetStaticStrForm(L"%s",s);
	STRINGend;
	STRING t = cmn.GetLpBuff();

	end = t + wcslen(t) - 1;

	while (end != t && isspace(*end))
		end--;
	*(end + 1) = '\0';
	

	return CCA::GetStaticStrForm(L"%s",t);
}


// 문자열 좌측 공백문자 삭제 함수
LPWSTR CCA::ltrim(LPWSTR s) 
{
	STRING  begin;
	begin = s;

	while (*begin != '\0') {
		if (isspace(*begin))
			begin++;
		else {
			s = begin;
			break;
		}
	}
	return CCA::GetStaticStrForm(L"%s",s);
}


// 문자열 앞뒤 공백 모두 삭제 함수
LPWSTR CCA::trim(LPWSTR s) 
{
	CMANMEM<TCHAR > cmn =ltrim(s);
	return rtrim(cmn);
};
#endif


void NeoCoLib::UnitTest(){

	nbyte orgbyte = 0;
	for (size_t i = 0; i < 256; i++)
	{
		nbyte ret = InverBitsPerByte(orgbyte);
		printf("0x%0.2x,",ret);
		orgbyte++;
	}
	uint16 testshort = 0x035d;
	InverBits(&testshort,2);

	return;

	#if 0
const char* test_file_path = "d:\\test\\test12312.txt";
	// Open the test file (contains UTF-8 encoded text)
	ifstream fs8(test_file_path);
	if (!fs8.is_open()) {
		cout << "Could not open " << test_file_path << endl;
		return ;
	}

	unsigned line_count = 1;
	string line;
	// Play with all the lines in the file
	while (getline(fs8, line)) {
		// check for invalid utf-8 (for a simple yes/no check, there is also utf8::is_valid function)
		string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
		if (end_it != line.end()) {
			cout << "Invalid UTF-8 encoding detected at line " << line_count << "\n";
			cout << "This part is fine: " << string(line.begin(), end_it) << "\n";
		}

		// Get the line length (at least for the valid part)
		int length = utf8::distance(line.begin(), end_it);
		cout << "Length of line " << line_count << " is " << length <<  "\n";

		// Convert it to utf-16
		vector<wchar_t> utf16line;
		utf8::utf8to16(line.begin(), end_it, back_inserter(utf16line));

		wstring wss(&utf16line[0],length);

		// And back to utf-8
		string utf8line; 
		utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(utf8line));

		// Confirm that the conversion went OK:
		if (utf8line != string(line.begin(), end_it))
			cout << "Error in UTF-16 conversion at line: " << line_count << "\n";        

		line_count++;
	}

	return;
	std::locale::global(std::locale(""));

	std::string mbs1 = "abcdef가나다라";
	std::wstring wcs1 = L"abcdef가나다라";
	std::string mbs2 = wcs_to_mbs(wcs1);
	
	std::wstring wcs2 = mbs_to_wcs(mbs1);

	std::string wcs4 = mbs_to_utf8(mbs1);
	std::string mb3 = utf8_to_mbs(wcs4);


	std::string data("Testing, ?");

	std::locale fromLoc = boost::locale::generator().generate("en_US.UTF-8");
	std::locale toLoc   = boost::locale::generator().generate("en_US.UTF-32");

	typedef std::codecvt<wchar_t, char, mbstate_t> cvtType;
	cvtType const* toCvt = &std::use_facet<cvtType>(toLoc);

	//std::wstring wcs3 = mbs_to_wcs(mbs1,toLoc);



	std::locale convLoc = std::locale(fromLoc, toCvt);

	std::cout.imbue(convLoc);
	std::cout << data << std::endl;


	


	wstring str2 =  mbs_to_wcs("test fuccker 씨발");


	STRING strpath = _t("D:/PROJECT/NGLS/SRC/NGLS/LsAgent_WIN/NEOCOLIB/CoreModule/commApi.cpp");

	size_t aa = strpath.find_first_of(L'.');
	size_t aa1 = strpath.find_last_of(L'.');

	 STRING str = NCL::GetStaticTimeForm(NULL,_t("D:\\logs\\LSC.\5YYYYMMDD\5.log"));
	
	STRING name = 	NeoCoLib::GetFileNameFromPath(str);
	STRING dir = GetDirNameFromPath(str);
	STRING ext =  NeoCoLib::GetExtName(str);
	STRING drivername = NeoCoLib::GetDriverNameFromPath(str);
	LPWSTRARRAY vecstr = 	NeoCoLib::GetSplit2Array(str,_t("/\\"));
#endif
	
}
tstring NeoCoLib::UnitTest2(){
	tstring ret ;

	ret = _t("fsdafsadfsadfsadfsadfsafsadfsfsad");
	return ret;
}
int * NeoCoLib::UnitTest3()
{
	//int * pint = new int[3] ;
	int * pint = (int*)malloc(4*3) ;
	*pint = 1;
	*pint = 2;
	*pint = 3;
	
	return pint ;
}


map<string, string> NeoCoLib::getmaparam(int argc, char* argv[])
{
	map<string, string> mapparam;
	int endc = argc;
	for (int i = 1; i <endc; i++)
	{
		string key;
		string mapvalue = "";
		key = argv[i];
		if (key[0] != '-') continue;


		char* pcharnext = argv[i + 1];

		if (pcharnext == NULL) pcharnext = "";

		if (pcharnext[0] != '-') {
			mapvalue = pcharnext;
			mapparam[key] = mapvalue;
			i++;
			continue;
		}
		mapparam[key] = mapvalue;



	}
	return mapparam;


}