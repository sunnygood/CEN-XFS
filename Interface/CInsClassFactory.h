/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CInsClassFactory.h
* Description: the instance class factory
*
* Author:      liuguoxun
* Date:        2015-11-07
*
*****************************************************************************/

#ifndef _CINSCLASSFACTORY_H_20151107
#define _CINSCLASSFACTORY_H_20151107

#include "IFactoryInsClass.h"
#include <CSTLMutexUnit.h>

//first do below at your project factory
//typedef  IPinpad        InterfacePtr;
//typedef  CPinpadBase    ObjectBase;

typedef enum {
	  INS_BY_NEW   = (0x0L) /* 非计数模式，每次都new一个对象(default) */
	, INS_BY_ID    = (0x1L) /* 计数模式，由nUniqueID区分是否同一个对象 */
	, INS_BY_NAME  = (0x2L) /* 计数模式，由szUniqueName区分是否同一个对象 */
	, INS_BY_ALL   = (0x3L) /* 计数模式，由nUniqueID和szUniqueName一起区分是否同一个对象 */
	, INS_ONLY_ONE = (0x4L) /* 计数模式，一个进程就只能实例化一个对象 */
}INSTYPE;

#define  MAX_UNIQUE_NAME    128

#pragma pack(push, 1)

typedef struct _ins_class_factory
{
	LPVOID   instance;
	DWORD    dwCount;
	I32      nUniqueID;
	TCHAR    szUniqueName[MAX_UNIQUE_NAME];
}INS_CF, *LPINS_CF;

#pragma pack(pop)

class CInsClassFactory : public IFactoryInsClass
{
public:
	virtual InterfacePtr * Instance()
	{
		return NULL;
	}

	virtual InterfacePtr * Instance(I32 PA, LPVOID PB, LPCTSTR PC)
	{
		return NULL;
	}

	virtual InterfacePtr * Instance(I32 PA, I32 PB, LPCTSTR PC)
	{
		return NULL;
	}

	virtual InterfacePtr * Instance(I32 PA, I32 PB, LPCTSTR PC, I32 PD)
	{
		return NULL;
	}

	void Release(LPVOID p)
	{
		if(NULL == p)
		{
			return;
		}

		if(m_eType == INS_BY_NEW)
		{
			InnerDelete(p);
		}
		else
		{
			m_mvObject.Lock();
			for(MVPCFIT it = m_mvObject.begin(); it != m_mvObject.end(); )
			{
				if(p == (*it)->instance)
				{
					if((*it)->dwCount > 1)
					{
						(*it)->dwCount--;
					}
					else
					{
						delete (ObjectBase*)p;
						it = m_mvObject.erase(it);
					}

					RLog2(LOG_LEVEL_DEBUG, _T("Remove one object[0x%08X]."), p);
					break;
				}
				else
				{
					it++;
				}
			}
			m_mvObject.UnLock();
		}
	}

	CInsClassFactory()
	{
		m_eType = INS_BY_NEW;
		m_mvObject.InitMutex();
	}

	virtual ~CInsClassFactory()
	{
		InnerDelete(NULL);
	}

protected:
	virtual I32 AutoDetectType(I32 PA, LPCTSTR PC)
	{
		return 0;
	}

	virtual I32 AutoDetectType(I32 PA, LPCTSTR PC, LPWORD lpSubType = 0)
	{
		return 0;
	}

	void SetInsType(INSTYPE eType)
	{
		m_eType = eType;
	}

	BOOL AddObject(LPVOID instance, I32 nUniqueID = 0, LPCTSTR pUniqueName = 0)
	{
		LPINS_CF pInsCF = new INS_CF();
		if(NULL == pInsCF)
		{
			return FALSE;
		}

		pInsCF->instance = instance;
		pInsCF->dwCount = 1;
		pInsCF->nUniqueID = nUniqueID;

		if(NULL != pUniqueName)
		{
			TCSCPY_S(pInsCF->szUniqueName, MAX_UNIQUE_NAME, pUniqueName);
		}
		else
		{
			memset(pInsCF->szUniqueName, 0, MAX_UNIQUE_NAME * sizeof(TCHAR));
		}

		m_mvObject.MutexAdd(pInsCF);
		return TRUE;
	}

	LPVOID AddCount(I32 nUniqueID = 0, LPCTSTR pUniqueName = 0)
	{
		LPVOID _instance = NULL;

		m_mvObject.Lock();
		for(MVPCFIT it = m_mvObject.begin(); it != m_mvObject.end(); it++)
		{
			if(m_eType == INS_BY_ID)
			{
				if(nUniqueID == (*it)->nUniqueID)
				{
					_instance = (*it)->instance;
					(*it)->dwCount++;
					break;
				}
			}
			else if(m_eType == INS_BY_NAME)
			{
				if(TCSCMP((*it)->szUniqueName, pUniqueName) == 0)
				{
					_instance = (*it)->instance;
					(*it)->dwCount++;
					break;
				}
			}
			else if(m_eType == INS_BY_ALL)
			{
				if(nUniqueID == (*it)->nUniqueID && TCSCMP((*it)->szUniqueName, pUniqueName) == 0)
				{
					_instance = (*it)->instance;
					(*it)->dwCount++;
					break;
				}
			}
			else if(m_eType == INS_ONLY_ONE)
			{
				_instance = (*it)->instance;
				(*it)->dwCount++;
				break;
			}
			else
			{
				; //INS_BY_NEW don't need call this function
			}
		}
		m_mvObject.UnLock();

		return _instance;
	}


private:
	void InnerDelete(LPVOID p)
	{
		if(m_mvObject.size() == 0)
		{
			return;
		}

		BOOL bDelAll = FALSE;
		LPVOID pInstance = NULL;
		if(NULL == p)
		{
			bDelAll = TRUE;
			RLog2(LOG_LEVEL_DEBUG, _T("Auto to delete all object."));
		}
		else
		{
			pInstance = p;
			RLog2(LOG_LEVEL_DEBUG, _T("Delete object[0x%08X]."), p);
		}

		m_mvObject.Lock();
		for(MVPCFIT it = m_mvObject.begin(); it != m_mvObject.end(); )
		{
			if(bDelAll)
			{
				if(NULL != (*it)->instance)
				{
					delete (ObjectBase*)((*it)->instance);
				}

				it = m_mvObject.erase(it);
			}
			else
			{
				if(pInstance == (*it)->instance)
				{
					delete (ObjectBase*)((*it)->instance);
					it = m_mvObject.erase(it);
					break;
				}
				else
				{
					it++;
				}
			}
		}
		m_mvObject.UnLock();
	}

private:
	INSTYPE m_eType;
	CMutexVector<LPINS_CF> m_mvObject;
	typedef CMutexVector<LPINS_CF>::iterator MVPCFIT;
};


/****************************************************************************/
// 此处仅是当使用动态库时，可以智能释放类对象；如果是静态库，则要求使用者必须自己释放
// 下面代码里不能添加其它功能的代码
#ifdef __ms_windows__
extern HMODULE g_hModule;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			g_hModule = hModule;
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		{
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
	}

    return TRUE;
}
#endif //__ms_windows__


#endif	//_CINSCLASSFACTORY_H_20151107
