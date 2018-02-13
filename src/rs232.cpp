/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015 Teunis van Beelen
*
* Email: teuniz@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/

/* Last revision: January 10, 2015 */

/* For more info and how to use this library, visit: http://www.teuniz.net/RS-232/ */

#include <memory.h>
#include <stdlib.h>
#include "rs232.h"
#include "neoDebug.h"

#define MAXPORTSIZE 100
#define NUMPORTNAME 100




#if defined(__linux__) || defined(__FreeBSD__)   /* Linux & FreeBSD */
#include <errno.h>

static int Cport[MAXPORTSIZE],   error;//-->static 을 붙여 주어야 함.neo1seok

static struct termios new_port_settings,
	old_port_settings[MAXPORTSIZE];//-->static 을 붙여 주어야 함.neo1seok

#ifndef __PPC__
char * comports[MAXPORTSIZE]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4","/dev/ttyS5",
                       "/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9","/dev/ttyS10","/dev/ttyS11",
                       "/dev/ttyS12","/dev/ttyS13","/dev/ttyS14","/dev/ttyS15","/dev/ttyUSB0",
                       "/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4","/dev/ttyUSB5",
                       "/dev/ttyAMA0","/dev/ttyAMA1","/dev/ttyACM0","/dev/ttyACM1",
                       "/dev/rfcomm0","/dev/rfcomm1","/dev/ircomm0","/dev/ircomm1",
                       "/dev/cuau0","/dev/cuau1","/dev/cuau2","/dev/cuau3",
                       "/dev/cuaU0","/dev/cuaU1","/dev/cuaU2","/dev/cuaU3",NULL};
#else
char * comports[MAXPORTSIZE]={"/dev/ttyPSC0","/dev/ttyPSC1","/dev/ttyPSC2","/dev/ttyPSC3","/dev/ttyPSC4","/dev/ttyPSC5","/dev/ttyPSC6","/dev/ttyPSC7","/dev/ttyPSC8","/dev/ttyPSC9","ttyPSC5",NULL};

#endif












int RS232_OpenComport(int comport_number, int baudrate, const char *mode)
{
	NEO_TITLE(RS232_OpenComport);
  int baudr,
      status;
  NEO_INT(Cport[0]);
  NEO_INT(Cport[1]);
  int maxportnum = RS232_getportMaxNum();
  
  if ((comport_number>=maxportnum) || (comport_number<0))
  {
    NEO_DEBUG("illegal comport number\n");
    return(1);
  }
  
  ////NEO_MARK(RS232_OpenComport 0);
  switch(baudrate)
  {
    case      50 : baudr = B50;
                   break;
    case      75 : baudr = B75;
                   break;
    case     110 : baudr = B110;
                   break;
    case     134 : baudr = B134;
                   break;
    case     150 : baudr = B150;
                   break;
    case     200 : baudr = B200;
                   break;
    case     300 : baudr = B300;
                   break;
    case     600 : baudr = B600;
                   break;
    case    1200 : baudr = B1200;
                   break;
    case    1800 : baudr = B1800;
                   break;
    case    2400 : baudr = B2400;
                   break;
    case    4800 : baudr = B4800;
                   break;
    case    9600 : baudr = B9600;
                   break;
    case   19200 : baudr = B19200;
                   break;
    case   38400 : baudr = B38400;
                   break;
    case   57600 : baudr = B57600;
                   break;
    case  115200 : baudr = B115200;
                   break;
    case  230400 : baudr = B230400;
                   break;
    case  460800 : baudr = B460800;
                   break;
    case  500000 : baudr = B500000;
                   break;
    case  576000 : baudr = B576000;
                   break;
    case  921600 : baudr = B921600;
                   break;
    case 1000000 : baudr = B1000000;
                   break;
    case 1152000 : baudr = B1152000;
                   break;
    case 1500000 : baudr = B1500000;
                   break;
    case 2000000 : baudr = B2000000;
                   break;
    case 2500000 : baudr = B2500000;
                   break;
    case 3000000 : baudr = B3000000;
                   break;
    case 3500000 : baudr = B3500000;
                   break;
    case 4000000 : baudr = B4000000;
                   break;
    default      : NEO_DEBUG("invalid baudrate\n");
                   return(1);
                   break;
  }
  //////NEO_MARK(RS232_OpenComport 1);
  int cbits=CS8,
      cpar=0,
      ipar=IGNPAR,
      bstop=0;

  if(strlen(mode) != 3)
  {
    NEO_DEBUG("invalid mode \"%s\"\n", mode);
    return(1);
  }
  ////NEO_MARK(RS232_OpenComport 2);
  switch(mode[0])
  {
    case '8': cbits = CS8;
              break;
    case '7': cbits = CS7;
              break;
    case '6': cbits = CS6;
              break;
    case '5': cbits = CS5;
              break;
    default : NEO_DEBUG("invalid number of data-bits '%c'\n", mode[0]);
              return(1);
              break;
  }
  ////NEO_MARK(RS232_OpenComport 3);
  switch(mode[1])
  {
    case 'N':
    case 'n': cpar = 0;
              ipar = IGNPAR;
              break;
    case 'E':
    case 'e': cpar = PARENB;
              ipar = INPCK;
              break;
    case 'O':
    case 'o': cpar = (PARENB | PARODD);
              ipar = INPCK;
              break;
    default : NEO_DEBUG("invalid parity '%c'\n", mode[1]);
              return(1);
              break;
  }
  ////NEO_MARK(RS232_OpenComport 4);
  //NEO_STR(mode);
  //NEO_INT(mode[2]);

  switch(mode[2])
  {
    case '1': bstop = 0;
              break;
    case '2': bstop = CSTOPB;
              break;
    default : NEO_DEBUG("invalid number of stop bits '%c'\n", mode[2]);
              return(1);
              break;
  }
  ////NEO_MARK(RS232_OpenComport 5);
/*
http://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html

http://man7.org/linux/man-pages/man3/termios.3.html
*/

  NEO_DEBUG("open comport %s ",comports[comport_number]);
  Cport[comport_number] = open(comports[comport_number], O_RDWR | O_NOCTTY | O_NDELAY);
  if(Cport[comport_number]==-1)
  {
    perror("unable to open comport ");
    return(1);
  }

  NEO_STR(comports[comport_number]);
  NEO_DWORD(Cport[comport_number]);
  NEO_DWORD(errno);
  

  error = tcgetattr(Cport[comport_number], old_port_settings + comport_number);
  
  if(error==-1)
  {
	  close(Cport[comport_number]);
	  Cport[comport_number] = -1;
    perror("unable to read portsettings ");
    return(1);
  }
  
  memset(&new_port_settings, 0, sizeof(new_port_settings));  /* clear the new struct */

  new_port_settings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
  new_port_settings.c_iflag = ipar;
  new_port_settings.c_oflag = 0;
  new_port_settings.c_lflag = 0;
  new_port_settings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
  new_port_settings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */

  cfsetispeed(&new_port_settings, baudr);
  cfsetospeed(&new_port_settings, baudr);
  
  error = tcsetattr(Cport[comport_number], TCSANOW, &new_port_settings);
  if(error==-1)
  {
	 
    perror("unable to adjust portsettings ");
	close(Cport[comport_number]);
	Cport[comport_number] = -1;
    return(1);
  }
  
  if(ioctl(Cport[comport_number], TIOCMGET, &status) == -1)
  {
	  
    perror("unable to get portstatus");
	close(Cport[comport_number]);
	Cport[comport_number] = -1;
	
    return(1);
  }

  status |= TIOCM_DTR;    /* turn on DTR */
  status |= TIOCM_RTS;    /* turn on RTS */
  
  if(ioctl(Cport[comport_number], TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
	close(Cport[comport_number]);
	Cport[comport_number] = -1;

    return(1);
  }

  return(0);
}


int RS232_PollComport(int comport_number, unsigned char *buf, int size)
{
  int n;

  n = read(Cport[comport_number], buf, size);

  return(n);
}


int RS232_SendByte(int comport_number, unsigned char byte)
{
  int n;

  n = write(Cport[comport_number], &byte, 1);
  if(n<0)  return(1);

  return(0);
}


int RS232_SendBuf(int comport_number, unsigned char *buf, int size)
{
  return(write(Cport[comport_number], buf, size));
}


void RS232_CloseComport(int comport_number)
{
  int status;

  if(ioctl(Cport[comport_number], TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status &= ~TIOCM_DTR;    /* turn off DTR */
  status &= ~TIOCM_RTS;    /* turn off RTS */

  if(ioctl(Cport[comport_number], TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }

  tcsetattr(Cport[comport_number], TCSANOW, old_port_settings + comport_number);
  close(Cport[comport_number]);
  Cport[comport_number] = -1;
  
}

/*
Constant  Description
TIOCM_LE        DSR (data set ready/line enable)
TIOCM_DTR       DTR (data terminal ready)
TIOCM_RTS       RTS (request to send)
TIOCM_ST        Secondary TXD (transmit)
TIOCM_SR        Secondary RXD (receive)
TIOCM_CTS       CTS (clear to send)
TIOCM_CAR       DCD (data carrier detect)
TIOCM_CD        see TIOCM_CAR
TIOCM_RNG       RNG (ring)
TIOCM_RI        see TIOCM_RNG
TIOCM_DSR       DSR (data set ready)

http://man7.org/linux/man-pages/man4/tty_ioctl.4.html
*/

int RS232_IsDCDEnabled(int comport_number)
{
  int status;

  ioctl(Cport[comport_number], TIOCMGET, &status);

  if(status&TIOCM_CAR) return(1);
  else return(0);
}

int RS232_IsCTSEnabled(int comport_number)
{
  int status;

  ioctl(Cport[comport_number], TIOCMGET, &status);

  if(status&TIOCM_CTS) return(1);
  else return(0);
}

int RS232_IsDSREnabled(int comport_number)
{
  int status;

  ioctl(Cport[comport_number], TIOCMGET, &status);

  if(status&TIOCM_DSR) return(1);
  else return(0);
}

void RS232_enableDTR(int comport_number)
{
  int status;

  if(ioctl(Cport[comport_number], TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status |= TIOCM_DTR;    /* turn on DTR */

  if(ioctl(Cport[comport_number], TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }
}

void RS232_disableDTR(int comport_number)
{
  int status;

  if(ioctl(Cport[comport_number], TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status &= ~TIOCM_DTR;    /* turn off DTR */

  if(ioctl(Cport[comport_number], TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }
}

void RS232_enableRTS(int comport_number)
{
  int status;

  if(ioctl(Cport[comport_number], TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status |= TIOCM_RTS;    /* turn on RTS */

  if(ioctl(Cport[comport_number], TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }
}

void RS232_disableRTS(int comport_number)
{
  int status;

  if(ioctl(Cport[comport_number], TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status &= ~TIOCM_RTS;    /* turn off RTS */

  if(ioctl(Cport[comport_number], TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }
}

bool RS232_isOpen(int comport_number)
{
	NEO_DWORD(Cport[comport_number]);
	NEO_DWORD(comport_number);
	
	//if (Cport[comport_number] ==0 || Cport[comport_number] == -1) return false;

	if (Cport[comport_number] == -1) return false;

	return true;
}



#else         /* windows */

HANDLE Cport[MAXPORTSIZE];
char *comports[MAXPORTSIZE] = { "\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4",
                       "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                       "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                       "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16",NULL};

char mode_str[128];




int RS232_OpenComport(int comport_number, int baudrate, const char *mode)
{
	
	int maxportnum = RS232_getportMaxNum();
	if ((comport_number>=maxportnum) || (comport_number<0))
  {
    NEO_DEBUG("illegal comport number\n");
    return(1);
  }

  switch(baudrate)
  {
    case     110 : strcpy(mode_str, "baud=110");
                   break;
    case     300 : strcpy(mode_str, "baud=300");
                   break;
    case     600 : strcpy(mode_str, "baud=600");
                   break;
    case    1200 : strcpy(mode_str, "baud=1200");
                   break;
    case    2400 : strcpy(mode_str, "baud=2400");
                   break;
    case    4800 : strcpy(mode_str, "baud=4800");
                   break;
    case    9600 : strcpy(mode_str, "baud=9600");
                   break;
    case   19200 : strcpy(mode_str, "baud=19200");
                   break;
    case   38400 : strcpy(mode_str, "baud=38400");
                   break;
    case   57600 : strcpy(mode_str, "baud=57600");
                   break;
    case  115200 : strcpy(mode_str, "baud=115200");
                   break;
    case  128000 : strcpy(mode_str, "baud=128000");
                   break;
    case  256000 : strcpy(mode_str, "baud=256000");
                   break;
    case  500000 : strcpy(mode_str, "baud=500000");
                   break;
    case 1000000 : strcpy(mode_str, "baud=1000000");
                   break;
    default      : NEO_DEBUG("invalid baudrate\n");
                   return(1);
                   break;
  }

  if(strlen(mode) != 3)
  {
    NEO_DEBUG("invalid mode \"%s\"\n", mode);
    return(1);
  }

  switch(mode[0])
  {
    case '8': strcat(mode_str, " data=8");
              break;
    case '7': strcat(mode_str, " data=7");
              break;
    case '6': strcat(mode_str, " data=6");
              break;
    case '5': strcat(mode_str, " data=5");
              break;
    default : NEO_DEBUG("invalid number of data-bits '%c'\n", mode[0]);
              return(1);
              break;
  }

  switch(mode[1])
  {
    case 'N':
    case 'n': strcat(mode_str, " parity=n");
              break;
    case 'E':
    case 'e': strcat(mode_str, " parity=e");
              break;
    case 'O':
    case 'o': strcat(mode_str, " parity=o");
              break;
    default : NEO_DEBUG("invalid parity '%c'\n", mode[1]);
              return(1);
              break;
  }

  switch(mode[2])
  {
    case '1': strcat(mode_str, " stop=1");
              break;
    case '2': strcat(mode_str, " stop=2");
              break;
    default : NEO_DEBUG("invalid number of stop bits '%c'\n", mode[2]);
              return(1);
              break;
  }

  strcat(mode_str, " dtr=on rts=on");

/*
http://msdn.microsoft.com/en-us/library/windows/desktop/aa363145%28v=vs.85%29.aspx

http://technet.microsoft.com/en-us/library/cc732236.aspx
*/
  
  Cport[comport_number] = CreateFileA(comports[comport_number],
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          /* no share  */
                      NULL,                       /* no security */
                      OPEN_EXISTING,
                      0,                          /* no threads */
                      NULL);                      /* no templates */

  //NEO_DWORD(Cport[comport_number]);

  if(Cport[comport_number]==INVALID_HANDLE_VALUE)
  {
    NEO_DEBUG("unable to open comport\n");
    return(1);
  }

  DCB port_settings;
  memset(&port_settings, 0, sizeof(port_settings));  /* clear the new struct  */
  port_settings.DCBlength = sizeof(port_settings);

  if(!BuildCommDCBA(mode_str, &port_settings))
  {
    NEO_DEBUG("unable to set comport dcb settings\n");
	RS232_CloseComport(comport_number);
    return(1);
  }

  if(!SetCommState(Cport[comport_number], &port_settings))
  {
    NEO_DEBUG("unable to set comport cfg settings\n");
	RS232_CloseComport(comport_number);
    return(1);
  }

  COMMTIMEOUTS Cptimeouts;

  Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 0;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(Cport[comport_number], &Cptimeouts))
  {
    NEO_DEBUG("unable to set comport time-out settings\n");
	RS232_CloseComport(comport_number);
    return(1);
  }

  return(0);
}



int RS232_PollComport(int comport_number, unsigned char *buf, int size)
{
  int n;

/* added the void pointer cast, otherwise gcc will complain about */
/* "warning: dereferencing type-punned pointer will break strict aliasing rules" */

  BOOL ret = ReadFile(Cport[comport_number], buf, size, (LPDWORD)((void *)&n), NULL);
  if (!ret) return 0;

  return(n);
}


int RS232_SendByte(int comport_number, unsigned char byte)
{
  int n;

  WriteFile(Cport[comport_number], &byte, 1, (LPDWORD)((void *)&n), NULL);

  if(n<0)  return(1);

  return(0);
}


int RS232_SendBuf(int comport_number, unsigned char *buf, int size)
{
  int n;
  //NEO_MARK(CSerialRS232::Write);
  //NEO_DWORD(Cport[comport_number]);

  if(WriteFile(Cport[comport_number], buf, size, (LPDWORD)((void *)&n), NULL))
  {
    return(n);
  }

  return(-1);
}


void RS232_CloseComport(int comport_number)
{
  CloseHandle(Cport[comport_number]);
  Cport[comport_number] = INVALID_HANDLE_VALUE;
}

/*
http://msdn.microsoft.com/en-us/library/windows/desktop/aa363258%28v=vs.85%29.aspx
*/

int RS232_IsDCDEnabled(int comport_number)
{
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_RLSD_ON) return(1);
  else return(0);
}


int RS232_IsCTSEnabled(int comport_number)
{
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_CTS_ON) return(1);
  else return(0);
}


int RS232_IsDSREnabled(int comport_number)
{
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_DSR_ON) return(1);
  else return(0);
}


void RS232_enableDTR(int comport_number)
{
  EscapeCommFunction(Cport[comport_number], SETDTR);
}


void RS232_disableDTR(int comport_number)
{
  EscapeCommFunction(Cport[comport_number], CLRDTR);
}


void RS232_enableRTS(int comport_number)
{
  EscapeCommFunction(Cport[comport_number], SETRTS);
}


void RS232_disableRTS(int comport_number)
{
  EscapeCommFunction(Cport[comport_number], CLRRTS);
}

bool RS232_isOpen(int comport_number){
	if (Cport[comport_number] ==0 || Cport[comport_number] == INVALID_HANDLE_VALUE) return false;
	return true;
}

#endif

void RS232_Init()
{
	NEO_TITLE(RS232_Init);
	memset(Cport, 0xff, 4 * MAXPORTSIZE);
//	NEO_INT(Cport[0]);
//	NEO_INT(Cport[1]);
//	NEO_INT(Cport[MAXPORTSIZE - 1]);

}
int RS232_buildPortIndex(const char * portname)
{

	int numport = RS232_getportMaxNum();

	int portindex = RS232_findportindex(portname);
	if (portindex < 0){

		char *pchar = (char*)malloc(strlen(portname) + 1);
		strcpy(pchar, portname);
		comports[numport] = pchar;
		portindex = numport;
	}

	return  portindex;

}
void RS232_cputs(int comport_number, const char *text)  /* sends a string to serial port */
{
  while(*text != 0)   RS232_SendByte(comport_number, *(text++));
}

int RS232_findportindex(const char * pname)
{
	char**ppchar = NULL;
	for (ppchar = comports; *ppchar; ppchar++){
		if (!strcmp(*ppchar, pname)) return (int)(ppchar - (char**)comports);

	}
	return -1;
}
const char * RS232_getportname(int portindex)
{
	return comports[portindex];
}

int RS232_getportMaxNum()
{
	char**ppchar = NULL;
	for (ppchar = comports; *ppchar; ppchar++){
		//break;
	}
	return (int)(ppchar - (char**)comports);
}

int RS232_nummaxportname()
{
	int count =0;
	for(char **pchar = comports; *pchar != NULL ; pchar++){
		count++;
	}
	return count;
}
