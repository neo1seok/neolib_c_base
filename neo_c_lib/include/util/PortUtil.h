#ifndef _LSA_PORT_UTIL_H_
#define _LSA_PORT_UTIL_H_

unsigned int GetTickCount(void);	// using clock_gettime()
unsigned int GetTickCount2(void);	// using gettimeofday()

string SetDirSeperator(string path);
string GetFileName(string fullpath);
string GetDirName(string fullpath);

#endif

