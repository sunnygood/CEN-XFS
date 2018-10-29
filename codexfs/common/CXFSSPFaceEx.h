/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSSPFaceEx.h
* Description: extend class for XFS SP interface
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#ifndef _CXFSSPFACEEX_H_20151109
#define _CXFSSPFACEEX_H_20151109

#include <IXFSSPFace.h>
#include <IXFSSPKernel.h>
#include "IPlatform.h"
#include "CSTLUnit.h"
#include "CDynLoadFactory.h"


/********************************************************************************/

class CXFSSPFaceEx : public IXFSSPFace
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


public:
	explicit CXFSSPFaceEx(IXFSSPManage *pManage, IXFSSPFace *pFace);
	virtual ~CXFSSPFaceEx();


protected:
	IXFSSPManage    *m_pSPManage;
	IXFSSPKernel    *m_pSPKernel;
	IXFSSPFace      *m_pSPFace;	
};

#endif	//_CXFSSPFACEEX_H_20151109
