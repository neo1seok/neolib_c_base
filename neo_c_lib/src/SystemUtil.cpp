#include "util/SystemUtil.h"

#ifdef WIN32
#include "util/SystemUtil4windows.h"

using namespace stdext;
#else
#include "util/SystemUtil4Linux.h"
using namespace std;
#endif
CSystemUtil *_psystemUtil = NULL;
CSystemUtil::CSystemUtil(void)
{
}

CSystemUtil::~CSystemUtil(void)
{
}
CSystemUtil * CSystemUtil::GetSystemUtilClass(){
	

	if (!_psystemUtil){
#ifdef WIN32
		_psystemUtil = new CSystemUtil4windows();
#else
		_psystemUtil = new CSystemUtil4Linux();
#endif
	}
	return _psystemUtil;
}
