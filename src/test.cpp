class CCtrlBASE{
	public:
	void genDefCMD(int cmd, BYTE * pbuff,int & size,std::string &strlog,void* param=NULL);
}
#define FUNCDEF(genpost_,class_,cmd_) void tmp_##genpost_##cmd_(class_ *pThis,int cmd, BYTE * pbuff, int & size, std::string &strlog, void* param){	 pThis->##genpost_##cmd_(cmd, pbuff, size, strlog, param);};
	
	

FUNCDEF(gen, CCtrlBASE, DefCMD);