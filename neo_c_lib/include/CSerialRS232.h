#ifndef __CRS232__
#define __CRS232__
#include "CSerialBASE.h"
class CSerialRS232 : public CSerialBASE
{
	private:
	int m_portindex;
	int m_bdrate;
	char m_mode[10];
public:
	CSerialRS232(int portindex, int bdrate = 115200, const char * mode = "8N1");
	CSerialRS232(const char * file, int bdrate = 115200, const char * mode = "8N1");

	~CSerialRS232(void);

	void close();
	bool isopen();
	const char *  getportnmae();
	int Write(const unsigned char * pbuff, int length);
	int Read(unsigned char * pbuff, int length);
	void setparam(int portindex, int bdrate = 115200 , const char * mode = "8N1");
	void setparam(const char * file, int bdrate = 115200, const char * mode = "8N1");
	bool open();
	void rtstrigger();

};

#endif