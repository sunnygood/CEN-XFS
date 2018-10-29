/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   PINExFactory.h
* Description: PIN extend module factory
*
* Author:      liuguoxun
* Date:        2015-11-11
*
*****************************************************************************/

#include "CDynLoadFactory.h"
#include "CXFSPINEx.h"
#include "CCCB.h"

typedef  IXFSSPFace      InterfacePtr;
typedef  CXFSPINEx       ObjectBase;
#include "CInsClassFactory.h"


class CMyFactory : public CInsClassFactory
{
public:
	virtual InterfacePtr * Instance(I32 PA, I32 PB, LPCTSTR PC)
	{
		ObjectBase   *_instance = NULL;
		IXFSSPFace   *pFace = NULL;
		IXFSPINBase  *pBase = NULL;
		IXFSSPManage *pManage = NULL;

		m_pDLFactory = InstanceDynLoadFactory();
		if(NULL != m_pDLFactory)
		{
			pFace = (IXFSSPFace*)m_pDLFactory->Instance(NAME_XFSPINBASE, INS_NAME_XFSPIN, PA, PB, PC);
			if(NULL != pFace)
			{
				pManage = (IXFSSPManage*)pFace->GetInterface(II_MANAGE);
				pBase = (IXFSPINBase *)pFace->GetInterface(II_SUBCLASS);				
			}
		}

		TCHAR szTypeEx[MAX_PATH] = {0};
		if(NULL != pManage)
		{
			pManage->GetServiceNodeStr(PC, _T("TypeEx"), szTypeEx, MAX_PATH);
		}

		RLog2(LOG_LEVEL_DEBUG, _T("szTypeEx=%s pManage=0x%X pFace=0x%X pBase=0x%X"), szTypeEx, pManage, pFace, pBase);
		if(TCSCMP(szTypeEx, _T("CCB")) == 0)
		{
			_instance = new CCCB(pManage, pFace, pBase);
		}
		else if(TCSCMP(szTypeEx, _T("ABC")) == 0)
		{
			_instance = new CXFSPINEx(pManage, pFace, pBase);
		}
		else
		{
			_instance = new CXFSPINEx(pManage, pFace, pBase);
		}
		
		if(NULL != _instance)
		{
			AddObject(_instance);
		}

		return (dynamic_cast<InterfacePtr*>(_instance));
	}

	virtual ~CMyFactory()
	{
		ReleaseDynLoadFactory();
		m_pDLFactory = NULL;
	}

protected:
	IDynLoadFactory *m_pDLFactory;
};


HMODULE g_hModule = NULL;
CMyFactory cMyFactory;

InterfacePtr * WINAPI InstancePIN(I32 PA, I32 PB, LPCTSTR PC)
{
	RLog2(LOG_LEVEL_DEBUG, _T("dwType=0x%X PB=0x%X ServiceName=%s"), PA, PB, PC);
	return cMyFactory.Instance(PA, PB, PC);
}

void WINAPI ReleasePIN(InterfacePtr *p)
{
	cMyFactory.Release(p);
}

