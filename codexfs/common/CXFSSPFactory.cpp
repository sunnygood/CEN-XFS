/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSSPFactory.cpp
* Description: the factory class for all SP       
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#include "CXFSSPFactory.h"


CXFSSPFactory::CXFSSPFactory()
{
	m_pSPManage = NULL;
	m_vServiceName.clear();
	m_pDLFactory = InstanceDynLoadFactory();
}

CXFSSPFactory::~CXFSSPFactory()
{
	ReleaseDynLoadFactory();
	m_pDLFactory = NULL;
}

long CXFSSPFactory::SPRun(LPCTSTR lpCmdLine)
{
	DLog2(LOG_LEVEL_DEBUG, "Enter");

	long nRet = ParseCmdLine(lpCmdLine);
	if(nRet < 0)
	{
		MessageBox(NULL, _T("Run at mistake way!"), _T("CXFSSPFactory"), MB_OK | MB_ICONSTOP);
		return nRet;
	}

	int nSize = m_vServiceName.size();
	if(nSize <= 0)
	{
		return -1;
	}
	else if(nSize > 1)
	{
		nSize--;
		for(int i = 0; i < nSize; ++i)
		{
			ThreadSPRun(i);
		}
	}

	return StartSP(m_vServiceName.back().c_str());
}

long CXFSSPFactory::ParseCmdLine(LPCTSTR lpCmdLine) //lpCmdLine = -s ServiceName -c CombineSrvcName
{
	if(NULL == lpCmdLine || NULL == m_pDLFactory)
	{
		return -1;
	}

	std::basic_string<TCHAR> sCmdLine(lpCmdLine);
	std::basic_string<TCHAR> sSrvcName;
	std::basic_string<TCHAR> sCombineSrvcName;

	int nPos = sCmdLine.find(_T("-s")); 
	if(nPos == std::basic_string<TCHAR>::npos)
	{
		return -2;
	}

	int nPosC = sCmdLine.find(_T("-c")); 
	if(nPosC == std::basic_string<TCHAR>::npos)
	{
		sSrvcName = sCmdLine.substr(nPos + 3);
		m_vServiceName.push_back(sSrvcName);
		RLog2(LOG_LEVEL_DEBUG, "ServiceName=%s lpCmdLine=%s", sSrvcName.c_str(), lpCmdLine);
	}
	else
	{
		sSrvcName = sCmdLine.substr(nPos + 3, nPosC - nPos - 4);
		sCombineSrvcName = sCmdLine.substr(nPosC + 3);

		SplitString(sCombineSrvcName, _T("|"), m_vServiceName);
		m_vServiceName.push_back(sSrvcName);
		RLog2(LOG_LEVEL_DEBUG, "ServiceName=%s sCombineSrvcName=%s lpCmdLine=[%s] size=%d", sSrvcName.c_str(), sCombineSrvcName.c_str(), lpCmdLine, m_vServiceName.size());
	}

	TCHAR szRunPath[MAX_PATH] = {0};
	SetCurrentDirectory(GetRunPath(szRunPath, MAX_PATH));
	RLog2(LOG_LEVEL_DEBUG, "RunPath=%s", szRunPath);

	m_pSPManage = (IXFSSPManage*)m_pDLFactory->Instance(NAME_XFSSPMANAGE, INS_NAME_XFSMANAGE);
	if(NULL == m_pSPManage)
	{
		MessageBox(NULL, _T("Load DLL[ XFSSPManage ] failed."), _T("CXFSSPFactory"), MB_OK | MB_ICONSTOP);
		return EC_INSTANCE_FAILED;
	}

	return 0;
}

long CXFSSPFactory::StartSP(LPCTSTR lpServiceName)
{
	DLog2(LOG_LEVEL_DEBUG, "Enter");
	if(NULL == lpServiceName)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid parameter");
		return EC_INVALID_PARA;
	}

	TCHAR szModule[MAX_PATH] = {0};
	m_pSPManage->GetServiceNodeStr(lpServiceName, _T("XFSModule"), szModule, MAX_PATH);
	if(TCSLEN(szModule) == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid parameter XFSModule, ServiceName=[%s]", lpServiceName);
		return EC_INVALID_PARA;
	}

	TCHAR szClass[MAX_PATH] = {0};
	m_pSPManage->GetServiceNodeStr(lpServiceName, _T("Class"), szClass, MAX_PATH);
	if(TCSLEN(szClass) == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid parameter Class, ServiceName=[%s]", lpServiceName);
		return EC_INVALID_PARA;
	}

	TCHAR szInsName[MAX_PATH] = {0};
	TCSCPY_S(szInsName, MAX_PATH, _T("Instance"));
	TCSCAT_S(szInsName, MAX_PATH, szClass);

	TCHAR szType[MAX_PATH] = {0};
	m_pSPManage->GetServiceNodeStr(lpServiceName, _T("Type"), szType, MAX_PATH);

	DWORD dwType = m_pSPManage->ConvertType(szClass, szType);

	IXFSSPFace *pSPFace = (IXFSSPFace*)m_pDLFactory->Instance(szModule, szInsName, dwType, (I32)0, lpServiceName);
	if(NULL == pSPFace)
	{
		RLog2(LOG_LEVEL_ERROR, "Load Face DLL[%s] failed, ServiceName=[%s]", szModule, lpServiceName);
		return EC_INSTANCE_FAILED;
	}

	return pSPFace->Run(lpServiceName);
}

long CXFSSPFactory::ThreadSPRun(DWORD dwIndex)
{
	DLog2(LOG_LEVEL_DEBUG, "Enter");

	LPTHREADPARASIN pTPS = new THREADPARASIN();
	if(NULL == pTPS)
	{
		RLog2(LOG_LEVEL_ERROR, "Alloc memory failed.");
		return EC_POINTER_NULL;
	}

	pTPS->pThis = this;
	pTPS->dwValue = dwIndex;
	pTPS->pPointer = NULL;

	DWORD dwThreadID = 0;
	HANDLE hSubThread = OS_CreateThread(&SubThreadEntry, pTPS, &dwThreadID);
	if(NULL == hSubThread)
	{
		RLog2(LOG_LEVEL_ERROR, "Create thread failed");
		return EC_INVALID_THREAD;
	}

	return 0;
}

DWORD CALLBACK CXFSSPFactory::SubThreadEntry(LPVOID pParam)
{
	LPTHREADPARASIN pTPS = (LPTHREADPARASIN)pParam;
	if(NULL == pTPS)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid pointer.");
		return EC_POINTER_NULL;
	}

	DWORD dwIndex = pTPS->dwValue;
	CXFSSPFactory *pThis = (CXFSSPFactory *)pTPS->pThis;
	delete pTPS;

	if(NULL == pThis || dwIndex >= pThis->m_vServiceName.size())
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid parameter.");
		return EC_INVALID_PARA;
	}

	RLog2(LOG_LEVEL_DEBUG, "dwIndex=0x%X ServiceName=%s", dwIndex, pThis->m_vServiceName[dwIndex].c_str());
	return pThis->StartSP(pThis->m_vServiceName[dwIndex].c_str());
}

/*************************************************************************/

static IXFSSPFactory *g_pXFSSPFactory = NULL;

IXFSSPFactory * InstanceSPFactory()
{
	if(NULL == g_pXFSSPFactory)
	{
		g_pXFSSPFactory = dynamic_cast<IXFSSPFactory*>(new CXFSSPFactory());
	}

	return g_pXFSSPFactory;
}

void ReleaseSPFactory()
{
	if(NULL != g_pXFSSPFactory)
	{
		delete dynamic_cast<CXFSSPFactory *>(g_pXFSSPFactory);
	}
}

