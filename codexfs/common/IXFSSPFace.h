/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IXFSSPFace.h
* Description: the standard interface for all sub class, such as PIN, PTR, IDC       
*
* Author:      liuguoxun
* Date:        2015-11-09         
*
*****************************************************************************/


#ifndef  _IXFSSPFACE_H_20151109
#define  _IXFSSPFACE_H_20151109


#include <IBaseSystem.h>

//Interface Index, GetInterface function's parameter
#define  II_MANAGE        0 //IXFSSPManage*
#define  II_KERNEL        1 //IXFSSPKernel*
#define  II_FACE          2 //IXFSSPFace*
#define  II_SUBCLASS      3 //IXFSPINBase* (PIN, PTR, IDC...)

#define  II_SDK           9 //IPinpad*


/********************************************************************************/

INTERFACE_I  IXFSSPFace
{
public:
	virtual HRESULT Run(LPCTSTR lpServiceName, IXFSSPFace *pFace = 0) = 0;
	virtual LPVOID  GetInterface(int nIndex = II_FACE) = 0; //This is use at extend mode


public:
	virtual HRESULT OnSPStart() = 0;
	virtual HRESULT OnSPExit() = 0;
	virtual HRESULT OnGetInfo(DWORD dwCategory, LPVOID lpQueryDetail, U32 RequestID) = 0;
	virtual HRESULT OnExecute(DWORD dwCommand, LPVOID lpCmdData, U32 RequestID) = 0;

	virtual HRESULT OnTimer(UINT nTimerID, LPVOID lpContext = NULL) = 0;
	virtual HRESULT OnOpenSession(U16 hService, LPCTSTR szAppID, DWORD dwTraceLevel) = 0;
	virtual HRESULT OnCloseSession(U16 hService) = 0;
	virtual HRESULT OnIdleUpdate() = 0;
	virtual HRESULT OnCancelRequest(DWORD dwCommand = 0, HRESULT hResult = -4, U32 RequestID = 0) = 0;
};


#endif  //_IXFSSPFACE_H_20151109

