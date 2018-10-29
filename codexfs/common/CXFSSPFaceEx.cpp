/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSSPFaceEx.cpp
* Description: extend class for XFS SP interface
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#include "CXFSSPFaceEx.h"


CXFSSPFaceEx::CXFSSPFaceEx(IXFSSPManage *pManage, IXFSSPFace *pFace)
	: m_pSPFace(pFace)
	, m_pSPManage(pManage)
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");

	m_pSPKernel = NULL;
	if(NULL != pFace)
	{
		m_pSPKernel = (IXFSSPKernel *)pFace->GetInterface(II_KERNEL);
	}
}

CXFSSPFaceEx::~CXFSSPFaceEx()
{
	RLog2(LOG_LEVEL_COMMON, "Enter");
}

//===========================================================================//
//                          Base class virtual function                      //
//===========================================================================//
HRESULT CXFSSPFaceEx::Run(LPCTSTR lpServiceName, IXFSSPFace *pFace)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");

	SAFE_CALL(m_pSPFace, Run(lpServiceName, dynamic_cast<IXFSSPFace*>(this)));
}

LPVOID CXFSSPFaceEx::GetInterface(int nIndex)
{
	switch(nIndex)
	{
	case II_FACE:
		return dynamic_cast<LPVOID>(dynamic_cast<IXFSSPFace *>(this));
	default:
		return NULL;
	}
}

HRESULT CXFSSPFaceEx::OnSPStart()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");

	SAFE_CALL(m_pSPFace, OnSPStart());
}

HRESULT CXFSSPFaceEx::OnSPExit()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");

	SAFE_CALL(m_pSPFace, OnSPExit());
}

HRESULT CXFSSPFaceEx::OnGetInfo(DWORD dwCategory, LPVOID lpQueryDetail, REQUESTID RequestID)
{
	RLog2(LOG_LEVEL_DEBUG, "Start: dwCategory=%d RequestID=%d", dwCategory, RequestID);

	SAFE_CALL(m_pSPFace, OnGetInfo(dwCategory, lpQueryDetail, RequestID));
}

HRESULT CXFSSPFaceEx::OnExecute(DWORD dwCommand, LPVOID lpCmdData, REQUESTID RequestID)
{	
	RLog2(LOG_LEVEL_DEBUG, "Start: dwCommand=%d RequestID=%d", dwCommand, RequestID);
	
	SAFE_CALL(m_pSPFace, OnExecute(dwCommand, lpCmdData, RequestID));
}

HRESULT CXFSSPFaceEx::OnTimer(UINT nTimerID, LPVOID lpContext)
{
	RLog2(LOG_LEVEL_DEBUG, "nTimerID=%d lpContext=0x%X", nTimerID, lpContext);

	SAFE_CALL(m_pSPFace, OnTimer(nTimerID, lpContext));
}

HRESULT CXFSSPFaceEx::OnOpenSession(HSERVICE hService, LPCTSTR szAppID, DWORD dwTraceLevel)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");

	SAFE_CALL(m_pSPFace, OnOpenSession(hService, szAppID, dwTraceLevel));
}

HRESULT CXFSSPFaceEx::OnCloseSession(HSERVICE hService)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");

	SAFE_CALL(m_pSPFace, OnCloseSession(hService));
}

HRESULT CXFSSPFaceEx::OnIdleUpdate()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");

	SAFE_CALL(m_pSPFace, OnIdleUpdate());
}

HRESULT CXFSSPFaceEx::OnCancelRequest(DWORD dwCommand, HRESULT hResult, REQUESTID RequestID)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");

	SAFE_CALL(m_pSPFace, OnCancelRequest(dwCommand, hResult, RequestID));
}

