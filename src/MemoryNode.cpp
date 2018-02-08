//#include <Windows.h>
//#include <atlbase.h>
#include <stdlib.h>
#include <stdio.h>

#include "neoCoLib.h"
#include "neoDebug.h"
using namespace neocolib;

typedef struct _tagMEMORYNODE{
	int index;
	pvoid lpmem;//메모리 포인터
	int length;//메모리 크기 

	//_tagMEMORYNODE* lpPrev;//다음 노드 
	_tagMEMORYNODE* lpNext;//다음 노드 
} MEMORYNODE,*LPMEMORYNODE;


void MNode_Init();
void MNode_Add(pvoid lpmem,int memSize);
void MNode_Remove(pvoid lpmem);
void MNode_GetMemInfo(int & totalSize,int & nodeNum);

//MEMORYNODE _endNode = {0,};
MEMORYNODE _startNode = {0,}; // 최초 노드 실제 사용 되지 않음 

LPMEMORYNODE _lpprevNode = NULL;//마지막 노드 실제 데이터 값이 입력됨

int _index = 0;
void MNode_NodeState(){
	//return;

	int index = 0;
	LPMEMORYNODE lptree = &_startNode;
	
	NEO_DEBUG(_t("\r\nprevnode c:%d n:%d") ,_lpprevNode->index,_lpprevNode->lpNext->index);
	NEO_DEBUG(_t("Node State:") );
	NEO_DEBUG(_t("c:%d n:%d") ,lptree->index,lptree->lpNext->index);

	int totalsize = 0;

	while(lptree != lptree->lpNext){

		LPMEMORYNODE lpcurmem = lptree->lpNext;// neo1seok 2013.09.25 : 최초 노드는 비어 있는 노드 이므로 
		
		totalsize += lpcurmem->length;
// 		WCHAR sz[MAX_NCL_PATH+1];
// 
// 
// 
// 		wcscpy(sz,CCA::GetNote(totalsize));
// 		
		tstring aaa;
		tstring bbb;
// 		NeoCoLib::GetNote(aaa,lpcurmem->length);
// 		NeoCoLib::GetNote(totalsize)
		tstring s2 = NeoCoLib::GetNote(totalsize);

		NEO_DEBUG(_t("%d->c:%d n:%d length:%s tot:%s"), index, lpcurmem->index, lpcurmem->lpNext->index, NeoCoLib::GetNote(lpcurmem->length).c_str(), s2.c_str());


		lptree = lptree->lpNext;
		index++;
	}
}
void DebugState(){
	return;
	MNode_NodeState();
}
void MNode_Init()
{
	//return;

	if(!_lpprevNode){
		//_startNode.lpPrev = &_startNode;
		_startNode.index = -1;
		_startNode.lpNext = &_startNode;
		//_endNode.lpPrev = &_startNode;
		//_endNode.lpNext = &_endNode;
		_lpprevNode = &_startNode;
		DebugState();

	}

	

	

}
void MNode_Add(pvoid lpmem,int memSize)
{
	//return;
	MNode_Init();
	LPMEMORYNODE lpCurNode = _lpprevNode->lpNext;
	LPMEMORYNODE lpmemlist = (LPMEMORYNODE)malloc(sizeof(MEMORYNODE));
	lpmemlist->length = memSize;
	lpmemlist->lpmem = lpmem;
	lpmemlist->lpNext =lpmemlist;// neo1seok 2013.09.25 : 엔드 노드로 세팅
	lpmemlist->index = _index;

	_lpprevNode->lpNext = lpmemlist;

	//_lpprevNode = lpmemlist;

	


	_lpprevNode = lpmemlist;// neo1seok 2013.09.25 :마지막 노드를 현재노드로 세팅해 준다.
	//_lpprevNode->lpNext = &_endNode; // neo1seok 2013.09.25 : 엔드 노드로 세팅

	//NEO_DEBUG(L"\r\nAdd %d",lpmemlist->index);

	DebugState();


	_index++;
	

}
LPMEMORYNODE FindNode(pvoid lpmem,LPMEMORYNODE * lppprevNode) // neo1seok 2013.09.25 : 찾고자 하는 노드의 바로 전 노드를 찾아 준다. 
{

	LPMEMORYNODE lptree = &_startNode;
	while(lptree != lptree->lpNext){

		LPMEMORYNODE lpcurmem = lptree->lpNext;// neo1seok 2013.09.25 : 최초 노드는 비어 있는 노드 이므로 

		if(lpcurmem->lpmem == lpmem){
			if(lppprevNode){
				*lppprevNode = lptree;
			}
			return lpcurmem;
		}
		lptree = lptree->lpNext;
	}
	return NULL;

}


void MNode_Remove(pvoid lpmem)
{
	//return;
	MNode_Init();
	LPMEMORYNODE lpprev = NULL;
	LPMEMORYNODE lpcur = FindNode(lpmem,&lpprev);
	
	lpprev->lpNext = lpcur->lpNext;

	if(lpcur->lpNext == lpcur){
		lpprev->lpNext = lpprev;
		_lpprevNode = lpprev;
	}
	if(_lpprevNode == lpcur){

	}
	//NEO_DEBUG(L"\r\n Remove %d",lpcur->index);

	

	free(lpcur);

	DebugState();
}
void MNode_GetMemInfo(int & totalSize,int & nodeNum)
{
	//return;
	MNode_Init();
	totalSize =0;
	nodeNum = 0;
	LPMEMORYNODE lptree = &_startNode;

	while(lptree != lptree->lpNext){

		LPMEMORYNODE lpcurmem = lptree->lpNext;// neo1seok 2013.09.25 : 최초 노드는 비어 있는 노드 이므로 

		totalSize += (lpcurmem->length + sizeof(MEMORYNODE));
		nodeNum++;
		lptree = lptree->lpNext;
	}
	//return totalSize;
}
void MNode_Test(){

}