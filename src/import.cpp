

#if (_MSC_VER == 1800)
#ifdef _M_AMD64
#pragma comment(lib,"../lib/x64/libeay32.lib")
#pragma comment(lib,"../lib/x64/ssleay32.lib")
#else
#pragma comment(lib,"../lib/win32/libeay32.lib")
#pragma comment(lib,"../lib/win32/ssleay32.lib")
#endif
#endif