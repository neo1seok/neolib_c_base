#ifndef __CRSBASE__
#define __CRSBASE__

class CSerialBASE
{
public:
	virtual void close(){
	};
	virtual bool isopen(){
		return false;
	};
	virtual const char *  getportnmae(){
		return "";
	};
	virtual int Write(const unsigned char * pbuff, int length){
		return 0;
	};
	virtual int Read(unsigned char * pbuff, int length){
		return 0;
	};
	virtual bool open(){
		return true;
	};
	virtual void SetWaitTime(int msec)
	{

	};
	virtual void rtstrigger(){

	}

	virtual int CmdEtc(int cmd, void * param1 = NULL, void * param2 = NULL)
	{
		return 0;
	};
	virtual int CmdEtc(const char *  pcmd, void * param1 = NULL, void * param2 = NULL){
		return 0;
	}
	virtual ~CSerialBASE(){};

};

#endif