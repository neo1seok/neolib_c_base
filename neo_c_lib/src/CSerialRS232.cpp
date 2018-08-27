#include <iostream>
#include <map>
#include<string>
#include<stdio.h>

#include "CSerialRS232.h"
#include "rs232.h"
//#include "neoDebug.h"

CSerialRS232::CSerialRS232(int portindex, int bdrate, const char * mode)
{
	setparam(portindex, bdrate, mode);

}
CSerialRS232::CSerialRS232(const char * file, int bdrate, const char * mode)
{
	setparam(file, bdrate, mode);

}
CSerialRS232::~CSerialRS232(){

}

int CSerialRS232::Write(const unsigned char * pbuff, int length){
	
	return RS232_SendBuf(m_portindex, (unsigned char *)pbuff, length);

}
int CSerialRS232::Read(unsigned char * pbuff, int length){
	//NEO_MARK(CSerialRS232::Read);
	//NEO_INT(m_portindex);
	return RS232_PollComport(m_portindex, pbuff, length);
}

void CSerialRS232::close()
{
	RS232_CloseComport(m_portindex);

 }
bool CSerialRS232::isopen()
{
	if (m_portindex < 0) return false;
	return RS232_isOpen(m_portindex);

}

const char *  CSerialRS232::getportnmae()
{
	static char szRet[1024];
	szRet[0] = 0;
	if (m_portindex < 0) return szRet;

	strcpy(szRet, RS232_getportname(m_portindex));

	return szRet; 

} 
void CSerialRS232::setparam(int portindex, int bdrate,const char * mode)
{

//	char mode[] = { '8', 'N', '1', 0 };
	RS232_Init();
	m_portindex = portindex;
	m_bdrate = bdrate;
	strcpy((char*)m_mode, mode);


	
}
void CSerialRS232::setparam(const char * file, int bdrate, const char * mode)
{
	setparam(RS232_buildPortIndex(file),bdrate,mode);

}
bool CSerialRS232::open()
{

	//char *mode[] = { '8', 'N', '1', 0 };
	//m_portindex = portindex;
//	m_bdrate = bdrate;
	//PrintView(" index:%d ptname:%s bdrate:%d mode:%s \r\n", m_portindex, getportnmae(), m_bdrate, m_mode);
	 
	if (RS232_OpenComport(m_portindex, m_bdrate, m_mode))
	{

		return false;
	}

	RS232_enableRTS(m_portindex);

	RS232_disableRTS(m_portindex);

	return true;
}
void CSerialRS232::rtstrigger()
{
	RS232_enableRTS(m_portindex);
	RS232_disableRTS(m_portindex);
}