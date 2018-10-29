/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   PINFactory.h
* Description: PIN base module factory
*
* Author:      liuguoxun
* Date:        2015-11-11
*
*****************************************************************************/

#include "CXFSPINBase.h"
#include "CXFSPINHSM.h"
#include "CXFSPINETS.h"


typedef  IXFSSPFace      InterfacePtr;
typedef  CXFSPINBase     ObjectBase;
#include "CInsClassFactory.h"


class CMyFactory : public CInsClassFactory
{
public:
	virtual InterfacePtr * Instance(I32 PA, I32 PB, LPCTSTR PC)
	{
		ObjectBase *_instance = NULL;
		DWORD dwType = static_cast<DWORD>(PA);

		switch(dwType)
		{
			case WFS_PIN_TYPEEDM:
			{
				_instance = new ObjectBase();
				break;
			}
			case WFS_PIN_TYPEETS:
			{
				_instance = new CXFSPINETS();
				break;
			}
			default:
			{
				_instance = new CXFSPINHSM();
				break;
			}
		}
		
		if(NULL != _instance)
		{
			AddObject(_instance);
		}

		return (dynamic_cast<InterfacePtr*>(_instance));
	}
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

