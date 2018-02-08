#ifndef _BLUM_RENAME_H_
#define _BLUM_RENAME_H_

#define LSAC CLSACoMod
#define	CND CNeoDebug
#define NCL NeoCoLib

#ifndef WIN32

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE 	0
#endif

typedef unsigned int	uint;

typedef int				SOCKET;
typedef bool			BOOL;
typedef unsigned char 	BYTE;
typedef unsigned int	UINT;
typedef unsigned long 	DWORD;
typedef unsigned short	WORD;
typedef void 			*LPVOID;
typedef DWORD 			*LPDWORD;

#define closesocket (__sock) close(__sock)

#include <unistd.h>
#define Sleep(__x) usleep((__x)*1000)

#endif	// not define WIN32


#endif

