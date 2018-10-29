/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   KernelFactory.h
* Description: Kernel module factory
*
* Author:      liuguoxun
* Date:        2015-10-13
*
*****************************************************************************/


#include "CXFSSPKernel.h"


typedef  IXFSSPKernel    InterfacePtr;
typedef  CXFSSPKernel    ObjectBase;
#include "CInsClassFactory.h"

CMutexMap<LPVOID, CXFSSPKernel*> g_mapThis;


class CMyFactory : public CInsClassFactory
{
public:
	virtual InterfacePtr * Instance()
	{
		ObjectBase *_instance = new ObjectBase();
		if(NULL != _instance)
		{
			AddObject(_instance);
		}

		return (dynamic_cast<InterfacePtr*>(_instance));
	}

	CMyFactory()
	{
		g_mapThis.InitMutex();
	}
};

HMODULE g_hModule = NULL;
CMyFactory cMyFactory;


InterfacePtr * WINAPI InstanceSPKernel()
{
	RLog2(LOG_LEVEL_DEBUG, _T("Enter"));
	return cMyFactory.Instance();
}

void WINAPI ReleaseSPKernel(InterfacePtr *p)
{
	cMyFactory.Release(p);
}

