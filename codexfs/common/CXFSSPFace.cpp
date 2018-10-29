/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSSPFace.cpp
* Description: the base class for all SP
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#include "CXFSSPFace.h"


CXFSSPFace::CXFSSPFace()
{
	DLog2(LOG_LEVEL_DEBUG, "Enter");

	m_pSPManage = NULL;
	m_pSPKernel = NULL;
	m_dwPortType = 0;
	m_dwCmdType = 0;
	m_dwExtendType = 0;
	m_dwUpdateSatusElapse = 5000;

	m_mapStatusExtra.clear();
	m_mapCapsExtra.clear();

	memset(m_szServiceName, 0, sizeof(m_szServiceName));
	memset(m_szSDKName, 0, sizeof(m_szSDKName));
	memset(m_szPortDesc, 0, sizeof(m_szPortDesc));

	m_pSPManage = NULL;
	m_pSPKernel = NULL;

	m_pDLFactory = InstanceDynLoadFactory();
	if(NULL != m_pDLFactory)
	{
		m_pSPManage = (IXFSSPManage*)m_pDLFactory->Instance(NAME_XFSSPMANAGE, INS_NAME_XFSMANAGE);
		m_pSPKernel = (IXFSSPKernel*)m_pDLFactory->Instance(NAME_XFSSPKERNEL, INS_NAME_XFSKERNEL);
	}
}

CXFSSPFace::~CXFSSPFace()
{
	DLog2(LOG_LEVEL_DEBUG, "Enter");
	ReleaseDynLoadFactory();
	m_pDLFactory = NULL;
}

HRESULT CXFSSPFace::Run(LPCTSTR lpServiceName, IXFSSPFace *pFace)
{
	if(NULL == m_pSPManage || NULL == m_pSPKernel)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid pointer, m_pSPManage=0x%X m_pSPKernel=0x%X", m_pSPManage, m_pSPKernel);
		return WFS_ERR_INTERNAL_ERROR;
	}

	if(NULL != lpServiceName)
	{
		TCSCPY_S(m_szServiceName, MAX_PATH, lpServiceName);
		RLog2(LOG_LEVEL_DEBUG, "ServiceName=%s", m_szServiceName);

		Config();
	}
	else
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid service name");
	}

	if(NULL == pFace)
	{
		return m_pSPKernel->Run(dynamic_cast<IXFSSPFace*>(this), m_pSPManage, lpServiceName);
	}
	else
	{
		return m_pSPKernel->Run(pFace, m_pSPManage, lpServiceName);
	}
}

LPVOID CXFSSPFace::GetInterface(int nIndex)
{
	switch(nIndex)
	{
	case II_MANAGE:
		return dynamic_cast<LPVOID>(m_pSPManage);
	case II_KERNEL:
		return dynamic_cast<LPVOID>(m_pSPKernel);
	case II_FACE:
		return dynamic_cast<LPVOID>(dynamic_cast<IXFSSPFace *>(this));
	default:
		return NULL;
	}
}

HRESULT CXFSSPFace::OnSPStart()
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
	return 0;
}

HRESULT CXFSSPFace::OnSPExit()
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
	return 0;
}

HRESULT CXFSSPFace::OnGetInfo(DWORD dwCategory, LPVOID lpQueryDetail, REQUESTID RequestID)
{
	RLog2(LOG_LEVEL_DEBUG, "dwCategory=%d RequestID=%d", dwCategory, RequestID);
	return 0;
}

HRESULT CXFSSPFace::OnExecute(DWORD dwCommand, LPVOID lpCmdData, REQUESTID RequestID)
{
	RLog2(LOG_LEVEL_DEBUG, "dwCommand=%d RequestID=%d", dwCommand, RequestID);
	return 0;
}

HRESULT CXFSSPFace::OnTimer(UINT nTimerID, LPVOID lpContext)
{
	RLog2(LOG_LEVEL_DEBUG, "nTimerID=%d lpContext=0x%X", nTimerID, lpContext);
	return 0;
}

HRESULT CXFSSPFace::OnCancelRequest(DWORD dwCommand, HRESULT hResult, REQUESTID RequestID)
{
	RLog2(LOG_LEVEL_DEBUG, "dwCommand=%d hResult=%d RequestID=%d", dwCommand, hResult, RequestID);
	return 0;
}

HRESULT CXFSSPFace::OnOpenSession(HSERVICE hService, LPCTSTR szAppID, DWORD dwTraceLevel)
{
	RLog2(LOG_LEVEL_DEBUG, "hService=%d szAppID=%s dwTraceLevel=%d", hService, szAppID, dwTraceLevel);
	return 0;
}

HRESULT CXFSSPFace::OnCloseSession(HSERVICE hService)
{
	RLog2(LOG_LEVEL_DEBUG, "hService=%d", hService);
	return 0;
}

HRESULT CXFSSPFace::OnIdleUpdate()
{
	return 0;
}

HRESULT CXFSSPFace::Config()
{
	if(NULL == m_pSPManage)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid pointer");
		return WFS_ERR_INTERNAL_ERROR;
	}

	m_dwPortType   = m_pSPManage->GetServiceNodeInt(m_szServiceName, _T("PortType"), 0);
	m_dwCmdType    = m_pSPManage->GetServiceNodeInt(m_szServiceName, _T("CmdType"),  0);
	m_dwExtendType = m_pSPManage->GetServiceNodeInt(m_szServiceName, _T("ExtendUsage"), 0);
	m_dwUpdateSatusElapse = m_pSPManage->GetServiceNodeInt(m_szServiceName, _T("UpdateStatusElapse"), 5000);

	m_pSPManage->GetServiceNodeStr(m_szServiceName, _T("SDKName"), m_szSDKName, MAX_PATH);
	m_pSPManage->GetServiceNodeStr(m_szServiceName, _T("PortDesc"), m_szPortDesc, MAX_PATH);

	TCHAR szVendor[MAX_PATH] = {0};
	m_pSPManage->GetServiceNodeStr(m_szServiceName, _T("vendor_name"), szVendor, MAX_PATH);
	m_mapCapsExtra.Update(_T("Vendor"), szVendor);

	TCHAR szXFSVers[MAX_PATH] = {0};
	m_pSPManage->GetServiceNodeStr(m_szServiceName, _T("SrvcVersion"), szXFSVers, MAX_PATH);
	m_mapCapsExtra.Update(_T("Version(XFS)[min|max]"), szXFSVers);

	TCHAR szVers[MAX_PATH] = {0};
	m_pSPManage->GetServiceNodeStr(m_szServiceName, _T("version"), szVers, MAX_PATH);
	m_mapCapsExtra.Update(_T("Version(SP)"), szVers);

	return 0;
}

HRESULT CXFSSPFace::Convert2SPErrorCode(long nRet)
{
	HRESULT iRet = nRet;
	switch(nRet)
	{
		case EC_SUCCESS:
		{
			iRet = WFS_SUCCESS;
			break;
		}
		case EC_POINTER_NULL:
		{
			iRet = WFS_ERR_INVALID_POINTER;
			break;
		}
		case EC_UNSUPPORT:
		{
			iRet = WFS_ERR_UNSUPP_COMMAND;
			break;
		}
		case EC_INVALID_DATA:
		case EC_INVALID_PARA:
		{
			iRet = WFS_ERR_INVALID_DATA;
			break;
		}
		default:
		{
			iRet = WFS_ERR_INTERNAL_ERROR; 
			break;
		}
	}

	return iRet;
}

