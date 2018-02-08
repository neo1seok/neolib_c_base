#define NUMMAXSTR 20
#define NUMBIGSTR 5
#define MAX_BIGSTR_PATH 3*MAX_NCL_PATH

template<class cTy_>
class STRMAN{
	typedef basic_string<cTy_, char_traits<cTy_>, allocator<cTy_> > cTySTR;
	int m_index;
	int m_indexBig;
	cTy_ *m_StrArray[NUMMAXSTR];
	cTy_ *m_StrArrayBIG[NUMBIGSTR];
public:
	STRMAN(){
		Clear();
	}

	~STRMAN(){
		Clear();
	}
	cTy_ * GetString(int length,int*pmemSize= NULL){
		cTy_ **ppt = NULL;
		int assinglnegth = 0;
		if(length < MAX_NCL_PATH){
			ppt = &m_StrArray[m_index%NUMMAXSTR];
			assinglnegth = MAX_NCL_PATH;
			m_index++;
		}
		else if(length < MAX_BIGSTR_PATH){
			ppt = &m_StrArrayBIG[m_indexBig%NUMBIGSTR];
			assinglnegth = MAX_BIGSTR_PATH;
			m_indexBig++;
		}

		int memsize = sizeof(cTy_)*(assinglnegth);;
		if(*ppt == NULL){
			// 			NCL::FreeMem(*ppt);
			*ppt = (cTy_ *)NCL::MallocMem(memsize+sizeof(cTy_));
		};

		//memsize *= 3;

		if(pmemSize)  *pmemSize = memsize;

		
		return *ppt;
	}

	cTy_ * CopySTR(cTySTR str,int*pmemSize= NULL){
		//int memsize = 0;
		cTy_*ret = GetString((int)str.size(), pmemSize);
		NCL::CopyPSTRFromSTRING<cTy_>(ret,str);
		
		//if(pmemSize)  *pmemSize = memsize;
//		memcpy(ret,str.c_str(),memsize);
		
		return ret;

	}

	void Clear()
	{
		FORNLOOP(NUMMAXSTR){
			cTy_ **ppt = &m_StrArray[nCount];
			if(*ppt) NCL::FreeMem(*ppt);
			*ppt  = NULL;
		}
		FORNLOOP(NUMBIGSTR){
			cTy_ **ppt = &m_StrArrayBIG[nCount];
			if(*ppt) NCL::FreeMem(*ppt);
			*ppt  = NULL;
		}
		m_index =0;
		m_indexBig = 0;

	}
};
