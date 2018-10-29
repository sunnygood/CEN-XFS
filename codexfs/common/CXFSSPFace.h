/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSSPFace.h
* Description: the base class for all SP        
*
* Author:      liuguoxun
* Date:        2015-11-09    
*
*****************************************************************************/


#ifndef  _CXFSSPFACE_H_20151109
#define  _CXFSSPFACE_H_20151109

#include <IXFSSPFace.h>
#include <IXFSSPKernel.h>
#include "IPlatform.h"
#include "CSTLUnit.h"
#include "CDynLoadFactory.h"


/********************************************************************************/

class CXFSSPFace : public IXFSSPFace
{
public:
	virtual HRESULT Run(LPCTSTR lpServiceName, IXFSSPFace *pFace = 0);
	virtual LPVOID  GetInterface(int nIndex = II_FACE);


public:	
	virtual HRESULT OnSPStart();
	virtual HRESULT OnSPExit();
	virtual HRESULT OnGetInfo(DWORD dwCategory, LPVOID lpQueryDetail, REQUESTID RequestID);
	virtual HRESULT OnExecute(DWORD dwCommand, LPVOID lpCmdData, REQUESTID RequestID);

	virtual HRESULT OnTimer(UINT nTimerID, LPVOID lpContext = NULL);
	virtual HRESULT OnOpenSession(HSERVICE hService, LPCTSTR szAppID, DWORD dwTraceLevel);
	virtual HRESULT OnCloseSession(HSERVICE hService);
	virtual HRESULT OnIdleUpdate();
	virtual HRESULT OnCancelRequest(DWORD dwCommand = 0, HRESULT hResult = WFS_ERR_CANCELED, REQUESTID RequestID = 0);


protected:
	HRESULT Config();
	HRESULT Convert2SPErrorCode(long nRet);


	explicit CXFSSPFace();
	virtual  ~CXFSSPFace();


protected:
	IDynLoadFactory *m_pDLFactory;
	IXFSSPManage    *m_pSPManage;
	IXFSSPKernel    *m_pSPKernel;

	DWORD m_dwPortType;
	DWORD m_dwCmdType;
	DWORD m_dwExtendType;
	DWORD m_dwUpdateSatusElapse;
	
	CMap<CStringZ<TCHAR>, CStringZ<TCHAR>> m_mapStatusExtra;
	CMap<CStringZ<TCHAR>, CStringZ<TCHAR>> m_mapCapsExtra;

	TCHAR m_szServiceName[MAX_PATH];
	TCHAR m_szSDKName[MAX_PATH];
	TCHAR m_szPortDesc[MAX_PATH];	
};


#endif  //_CXFSSPFACE_H_20151109

