/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CSTLMutexUnit.h
* Description: the mutex realize class for STL
*
* Author:      liuguoxun
* Date:        2015-10-18 
*
*****************************************************************************/

#ifndef  _CSTLMUTEXUNIT_H_20151018
#define  _CSTLMUTEXUNIT_H_20151018

#include <map>
#include <vector>
#include <list>
#include <tchar.h>
#include <IBaseSystem.h>
#include <IBaseLog.h>
#include "CAutoClass.h"

/****************************************************************************/

class CSTLMutex
{
public:
	CSTLMutex()
	{
		m_hMutex = NULL;
	}

	~CSTLMutex()
	{
		if(NULL != m_hMutex)
		{
			ReleaseMutex(m_hMutex);
			CloseHandle(m_hMutex);
		}
	}

	long InitMutex(LPCTSTR lpMutexName = NULL)
	{
		TCHAR szMutexName[MAX_PATH] = {0};
		if(NULL != lpMutexName)
		{
			TCSCPY_S(szMutexName, MAX_PATH, lpMutexName);
		}
		else
		{
			OS_CreateGUID(szMutexName);
		}

		m_hMutex = CreateMutex(NULL, FALSE, szMutexName);
		if(NULL == m_hMutex)
		{
			return EC_POINTER_NULL;
		}
		else
		{
			return 0;
		}
	}

	long Lock(DWORD dwTimeout = INFINITE)
	{
		if(NULL == m_hMutex)
		{
			return 0;
		}

		return ::WaitForSingleObject(m_hMutex, dwTimeout);
	}

	long UnLock()
	{
		if(NULL != m_hMutex)
		{
			ReleaseMutex(m_hMutex);
		}

		return 0;
	}

	HANDLE GetMutexHandle()
	{
		return m_hMutex;
	}

protected:
	HANDLE m_hMutex;
};

/****************************************************************************/

using namespace std;

template<typename _KT, typename _Ty>
class CMutexMap : public map<_KT, _Ty>, public CSTLMutex
{
	typedef _KT key_type;
	typedef _Ty map_type;
	typedef CMutexMap<_KT, _Ty>  _Myt;
	typedef typename CMutexMap<_KT, _Ty>::iterator MMIT;
	typedef typename CMutexMap<_KT, _Ty>::const_iterator MMCIT;

public:
	CMutexMap()
	{
	}

	~CMutexMap()
	{
		clear();
	}

	long MutexUpdate(key_type s, map_type d)
	{
		CAutoMutex mutex(m_hMutex);
		for(MMIT it = begin(); it != end(); it++)
		{
			if(it->first == s)
			{
				it->second = d;
				return 0;
			}
		}

		pair<key_type, map_type> value(s, d);		
		insert(value);
		return 0;
	}

	MMIT MutexRemove(key_type s, map_type *pD = 0)
	{
		CAutoMutex mutex(m_hMutex);
		MMIT it = find(s);
		if(it != end())
		{
			if(NULL != pD)
			{
				*pD = it->second;
			}
			
			return erase(it);
		}
		else
		{
			return it;
		}
	}

	long MutexClear()
	{
		CAutoMutex mutex(m_hMutex);
		clear();
		return 0;
	}

	long FindSecond(key_type s, map_type *pD = 0) const
	{
		CAutoMutex mutex(m_hMutex);
		MMCIT it = find(s);
		if(it != end())
		{
			if(NULL != pD)
			{
				*pD = it->second;
			}

			return 0;
		}
		else
		{
			return EC_NOT_FOUNDED;
		}
	}

	long FindFirst(map_type d, key_type *pS = 0) const
	{
		CAutoMutex mutex(m_hMutex);
		for(MMCIT it = begin(); it != end(); it++)
		{
			if(it->second == d)
			{
				if(NULL != pS)
				{
					*pS = it->first;
				}

				return 0;
			}
		}

		return EC_NOT_FOUNDED;
	}

	_Myt& operator=(_Myt&& _Right)
	{
		if(this == &_Right)
		{
			;
		}
		else
		{
			*this = _Right;
			this->m_hMutex = NULL;
		}

		return (*this);
	}
};

template<typename _KT, typename _Ty>
class CMutexMultiMap : public multimap<_KT, _Ty>, public CSTLMutex
{
	typedef _KT key_type;
	typedef _Ty map_type;
	typedef CMutexMultiMap<_KT, _Ty>  _Myt;
	typedef typename CMutexMultiMap<_KT, _Ty>::iterator MMIT;
	typedef typename CMutexMultiMap<_KT, _Ty>::const_iterator MMCIT;

public:
	CMutexMultiMap()
	{
	}

	~CMutexMultiMap()
	{
		clear();
	}

	long MutexInsert(key_type s, map_type d)
	{
		CAutoMutex mutex(m_hMutex);
		pair<key_type, map_type> value(s, d);
		insert(value);
		return 0;
	}

	MMIT MutexRemove(key_type s, map_type *pD = 0)
	{
		CAutoMutex mutex(m_hMutex);
		MMIT it = find(s);
		if(it != end())
		{
			if(NULL != pD)
			{
				*pD = it->second;
			}
			
			return erase(it);
		}
		else
		{
			return it;
		}
	}

	long MutexClear()
	{
		CAutoMutex mutex(m_hMutex);
		clear();
		return 0;
	}

	long FindSecond(key_type s, map_type *pD = 0) const
	{
		CAutoMutex mutex(m_hMutex);
		MMCIT it = find(s);
		if(it != end())
		{
			if(NULL != pD)
			{
				*pD = it->second;
			}

			return 0;
		}
		else
		{
			return EC_NOT_FOUNDED;
		}
	}

	long FindFirst(map_type d, key_type *pS = 0) const
	{
		CAutoMutex mutex(m_hMutex);
		for(MMCIT it = begin(); it != end(); it++)
		{
			if(it->second == d)
			{
				if(NULL != pS)
				{
					*pS = it->first;
				}

				return 0;
			}
		}

		return EC_NOT_FOUNDED;
	}

	_Myt& operator=(_Myt&& _Right)
	{
		if(this == &_Right)
		{
			;
		}
		else
		{
			*this = _Right;
			this->m_hMutex = NULL;
		}

		return (*this);
	}
};


template<typename _Ty>
class CMutexVector : public vector<_Ty>, public CSTLMutex
{
	typedef _Ty _mytype;
	typedef CMutexVector<_mytype>  _Myt;
	typedef typename CMutexVector<_mytype>::iterator MVIT;

public:
	CMutexVector()
	{
	}

	~CMutexVector()
	{
		clear();
	}

	long MutexAdd(_mytype d)
	{
		CAutoMutex mutex(m_hMutex);
		push_back(d);
		return 0;
	}

	MVIT MutexRemove(_mytype d)
	{
		CAutoMutex mutex(m_hMutex);
		for(MVIT it = begin(); it != end(); it++)
		{
			if(*it == d)
			{
				return erase(it);
			}
		}

		return end();
	}

	long MutexClear()
	{
		CAutoMutex mutex(m_hMutex);
		clear();
		return 0;
	}

	_Myt& operator=(_Myt&& _Right)
	{
		if(this == &_Right)
		{
			;
		}
		else
		{
			*this = _Right;
			this->m_hMutex = NULL;
		}

		return (*this);
	}
};


template<typename _Ty>
class CMutexList : public list<_Ty>, public CSTLMutex
{
	typedef _Ty _mytype;
	typedef CMutexList<_mytype>  _Myt;
	typedef typename CMutexList<_mytype>::iterator MLIT;

public:
	CMutexList()
	{
	}

	~CMutexList()
	{
		clear();
	}

	long MutexPush(_mytype d)
	{
		CAutoMutex mutex(m_hMutex);
		push_back(d);
		return 0;
	}

	_mytype MutexPop()
	{
		CAutoMutex mutex(m_hMutex);

		_mytype tyRet = 0;
		if(size() > 0)
		{
			tyRet = front();
			pop_front();
		}

		return tyRet;
	}

	MLIT MutexRemove(_mytype d)
	{
		CAutoMutex mutex(m_hMutex);
		for(MLIT it = begin(); it != end(); it++)
		{
			if(*it == d)
			{
				return erase(it);
			}
		}

		return end();
	}

	long MutexRemove2(_mytype d)
	{
		CAutoMutex mutex(m_hMutex);
		for(MLIT it = begin(); it != end(); it++)
		{
			if(*it == d)
			{
				erase(it);
				return 0;
			}
		}

		return EC_NOT_FOUNDED;
	}

	long MutexClear()
	{
		CAutoMutex mutex(m_hMutex);
		clear();
		return 0;
	}

	_Myt& operator=(_Myt&& _Right)
	{
		if(this == &_Right)
		{
			;
		}
		else
		{
			*this = _Right;
			this->m_hMutex = NULL;
		}

		return (*this);
	}
};

#endif //_CSTLMUTEXUNIT_H_20151018
