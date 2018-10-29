/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CDynLoadFactory.h
* Description: the dynamic load dll/so factory class
*
* Author:      liuguoxun
* Date:        2015-10-28       
*
*****************************************************************************/

#ifndef _CDYNLOADFACTORY_H_20151028
#define _CDYNLOADFACTORY_H_20151028

#include "IFactoryDynLoad.h"
#include <CSTLMutexUnit.h>


#pragma pack(push, 1)

typedef struct _dynload_factory
{
	HMODULE  hLib;
//	LPVOID   lpFuncIns;
	FUNC_REL lpFuncRel;
	TCHAR    szLibName[128];
}DYN_LF, *LPDYN_LF;

#pragma pack(pop)


class CDynLoadFactory : public IDynLoadFactory
{
public:
	LPVOID Instance(LPCTSTR lpLibName, LPCSTR lpInsName)
	{
		DLog2(LOG_LEVEL_DATA, _T("lpLibName=%s lpInsName=%s"), lpLibName, lpInsName);

		FUNC_INS lpFuncIns = (FUNC_INS)GetFuncAddr(lpLibName, lpInsName);
		if(NULL != lpFuncIns)
		{
			return lpFuncIns();
		}

		return NULL;
	}

	LPVOID Instance(LPCTSTR lpLibName, LPCSTR lpInsName, I32 PA, LPVOID PB, LPCTSTR PC)
	{
		DLog2(LOG_LEVEL_DATA, _T("lpLibName=%s lpInsName=%s PA=%d PB=0x%X"), lpLibName, lpInsName, PA, PB);

		FUNC_INS2 lpFuncIns = (FUNC_INS2)GetFuncAddr(lpLibName, lpInsName);
		if(NULL != lpFuncIns)
		{
			return lpFuncIns(PA, PB, PC);
		}

		return NULL;
	}

	LPVOID Instance(LPCTSTR lpLibName, LPCSTR lpInsName, I32 PA, I32 PB, LPCTSTR PC)
	{
		DLog2(LOG_LEVEL_DATA, _T("lpLibName=%s lpInsName=%s PA=%d PB=%d"), lpLibName, lpInsName, PA, PB);

		FUNC_INS3 lpFuncIns = (FUNC_INS3)GetFuncAddr(lpLibName, lpInsName);
		if(NULL != lpFuncIns)
		{
			return lpFuncIns(PA, PB, PC);
		}

		return NULL;
	}

	LPVOID Instance(LPCTSTR lpLibName, LPCSTR lpInsName, I32 PA, I32 PB, LPCTSTR PC, I32 PD)
	{
		DLog2(LOG_LEVEL_DATA, _T("lpLibName=%s lpInsName=%s PA=%d PB=%d PD=%d"), lpLibName, lpInsName, PA, PB, PD);

		FUNC_INS4 lpFuncIns = (FUNC_INS4)GetFuncAddr(lpLibName, lpInsName);
		if(NULL != lpFuncIns)
		{
			return lpFuncIns(PA, PB, PC, PD);
		}

		return NULL;
	}

	LPVOID GetFuncAddr(LPCTSTR lpLibName, LPCSTR lpFuncName)
	{
		if(NULL == lpLibName || NULL == lpFuncName)
		{
			return NULL;
		}

		HMODULE  hLib = NULL;

		m_vDFLib.Lock();
		for(MVDLFIT it = m_vDFLib.begin(); it != m_vDFLib.end(); ++it)
		{
			if(TCSCMP(lpLibName, it->szLibName) == 0)
			{
				hLib = it->hLib;
				break;
			}
		}

		if(NULL == hLib)
		{
			hLib = LOAD_LIB(lpLibName);
			if(NULL != hLib)
			{
				DYN_LF stDynLoadFac;
				stDynLoadFac.hLib = hLib;
				stDynLoadFac.lpFuncRel = NULL;
				TCSCPY_S(stDynLoadFac.szLibName, 128, lpLibName);
				m_vDFLib.push_back(stDynLoadFac);
			}
		}
		m_vDFLib.UnLock();

		if(NULL != hLib)
		{
			return (LPVOID)FUNC_GET(hLib, lpFuncName);
		}

		return NULL;
	}

	void Release(LPCTSTR lpLibName, LPCSTR lpRelName, LPVOID p)
	{
		HMODULE  hLib = NULL;
		MVDLFIT it;

		m_vDFLib.Lock();
		for(it = m_vDFLib.begin(); it != m_vDFLib.end(); ++it)
		{
			if(TCSCMP(lpLibName, it->szLibName) == 0)
			{
				hLib = it->hLib;
				break;
			}
		}
		m_vDFLib.UnLock();

		if(NULL == hLib || NULL == lpRelName)
		{
			return;
		}

		if(NULL == it->lpFuncRel)
		{
			it->lpFuncRel = (FUNC_REL)FUNC_GET(hLib, lpRelName);
		}

		if(NULL != it->lpFuncRel)
		{
			it->lpFuncRel(p);
		}
	}

	CDynLoadFactory()
	{
		m_vDFLib.InitMutex();
	}

	virtual ~CDynLoadFactory()
	{
		FreeLib();
	}

protected:
	void FreeLib()
	{
		m_vDFLib.Lock();
		for(MVDLFIT it = m_vDFLib.begin(); it != m_vDFLib.end(); ++it)
		{
			FREE_LIB(it->hLib);
		}

		m_vDFLib.clear();
		m_vDFLib.UnLock();
	}

protected:
	CMutexVector<DYN_LF> m_vDFLib;
	typedef CMutexVector<DYN_LF>::iterator MVDLFIT;
};

/*************************************************************************/

static IDynLoadFactory *g_pDynLoadFactory = NULL;
static int g_nDLFCount = 0;

static IDynLoadFactory * WINAPI InstanceDynLoadFactory()
{
	g_nDLFCount++;
	if(NULL == g_pDynLoadFactory)
	{
		g_pDynLoadFactory = dynamic_cast<IDynLoadFactory*>(new CDynLoadFactory());
	}

	return g_pDynLoadFactory;
}

static void WINAPI ReleaseDynLoadFactory()
{
	if(g_nDLFCount > 0)
	{
		g_nDLFCount--;
		if(NULL != g_pDynLoadFactory && g_nDLFCount <= 0)
		{
			delete (dynamic_cast<CDynLoadFactory *>(g_pDynLoadFactory));
			g_pDynLoadFactory = NULL;
		}
	}
}

#endif	//_CDYNLOADFACTORY_H_20151028
