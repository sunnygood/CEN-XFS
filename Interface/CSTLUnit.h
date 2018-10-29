/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CSTLUnit.h
* Description: the realize class for STL  
*
* Author:      liuguoxun
* Date:        2015-11-19
*
*****************************************************************************/

#ifndef  _CSTLUNIT_H_20151119
#define  _CSTLUNIT_H_20151119

#include <vector>
#include <map>
#include <string>
#include <tchar.h>
#include <IBaseSystem.h>
#include <IBaseLog.h>


namespace std
{

template<typename _Elem>
class CDataBuffer
{
public:
	CDataBuffer()
	{
		m_dwSize = 0;
		m_pData  = NULL;
	}
	
	~CDataBuffer()
	{		
		Clear();
	}
	
	void Clear()
	{
		m_dwSize = 0;
		if(NULL != m_pData)
		{
			delete [] m_pData;
			m_pData  = NULL;
		}
	}

	DWORD GetSize()
	{
		return m_dwSize;
	}

	_Elem * GetBufferSetLength(DWORD dwLength)
	{
		Clear();

		m_pData = new _Elem[dwLength + 1];
		if(NULL != m_pData)
		{
			m_dwSize = dwLength;
			return m_pData;
		}

		return NULL;
	}
	
	void Set(_Elem *lpData, DWORD dwLength)
	{
		if(NULL != lpData && dwLength > 0)
		{
			Clear();

			m_pData = new _Elem[dwLength + 1];
			if(NULL != m_pData)
			{
				m_dwSize = dwLength;
				memcpy(m_pData, lpData, dwLength * sizeof(_Elem));
				*(m_pData + dwLength) = 0;
			}
		}
	}
	
	operator _Elem*()
	{
		return m_pData;
	}
	
protected:
	DWORD  m_dwSize;
	_Elem *m_pData;
};

template<typename _Ty>
class CVectorValue : public vector<_Ty>
{
	typedef _Ty _mytype;

public:
	CVectorValue()
	{
	}

	~CVectorValue()
	{
	}

	operator _mytype*()
	{
		if(empty())
		{
			return NULL;
		}
		else
		{
			return (_mytype*)(data());
		}
	}
};

template<typename _Ty>
class CVectorPtr : public vector<_Ty>
{
	typedef _Ty _mytype;
	typedef typename CVectorPtr<_mytype>::iterator VPIT;

public:
	CVectorPtr()
	{
	}

	~CVectorPtr()
	{
		Clear();
	}

	void Clear()
	{
		for(VPIT it = begin(); it != end(); it++)
		{
			delete (*it);
		}

		clear();
	}

	operator _mytype()
	{
		if(empty())
		{
			return NULL;
		}
		else
		{
			return (_mytype)(front());
		}
	}

	operator _mytype*()
	{
		if(empty())
		{
			return NULL;
		}
		else
		{
			push_back(NULL);
			return (_mytype*)(data());
		}
	}
};

template<class _Elem>
class CStringZ : public basic_string<_Elem>
{
public:
	CStringZ() : basic_string(){}

	CStringZ(const _Myt& _Right) : basic_string(_Right){}

	explicit CStringZ(const _Alloc& _Al) : basic_string(_Al){}

	CStringZ(const _Myt& _Right, size_type _Roff, size_type _Count = npos) : basic_string(_Right, _Roff, _Count){}

	CStringZ(const _Myt& _Right, size_type _Roff, size_type _Count,	const _Alloc& _Al) : basic_string(_Right, _Roff, _Count, _Al){}

	CStringZ(const _Elem *_Ptr, size_type _Count) : basic_string(_Ptr, _Count){}

	CStringZ(const _Elem *_Ptr, size_type _Count, const _Alloc& _Al) : basic_string(_Ptr, _Count, _Al){}

	CStringZ(const _Elem *_Ptr)	: basic_string(_Ptr){}

	CStringZ(const _Elem *_Ptr, const _Alloc& _Al) : basic_string(_Ptr, _Al){}

	CStringZ(size_type _Count, _Elem _Ch) : basic_string(_Count, _Ch){}

	CStringZ(size_type _Count, _Elem _Ch, const _Alloc& _Al) : basic_string(_Count, _Ch, _Al){}

	void Clear()
	{
		clear();
	}

	void Set(const _Elem *lpData)
	{
		clear();
		append(lpData);
	}

	void EndZZ()
	{
		if(sizeof(_Elem) == 1)
		{
			append("\0\0", 2);
		}
		else
		{
			append(L"\0\0", 2);
		}
	}

	void EndZZZ()
	{
		if(sizeof(_Elem) == 1)
		{
			append("\0\0\0", 3);
		}
		else
		{
			append(L"\0\0\0", 3);
		}
	}

	operator LPTSTR()
	{
		return (LPTSTR)(data());
	}
};

template<typename _KT, typename _Ty>
class CMap : public map<_KT, _Ty>
{
	typedef _KT key_type;
	typedef _Ty map_type;
	typedef typename CMap<_KT, _Ty>::iterator MIT;
	typedef typename CMap<_KT, _Ty>::const_iterator MCIT;

public:
	CMap()
	{
	}

	~CMap()
	{
	}

	long Update(key_type s, map_type d)
	{
		for(MIT it = begin(); it != end(); it++)
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

	long FindSecond(key_type s, map_type *pD = 0) const
	{
		MCIT it = find(s);
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
		for(MCIT it = begin(); it != end(); it++)
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

	operator LPTSTR()
	{
		if(empty())
		{
			return NULL;
		}

		m_ts.erase();

		#ifdef _UNICODE
			for(MIT it = begin(); it != end(); ++it)
			{
				m_ts += it->first + L'=' + it->second + L'\0';
			}
			m_ts += L'\0';
		#else
			for(MIT it = begin(); it != end(); ++it)
			{
				m_ts += it->first + '=' + it->second + '\0';
			}
			m_ts += '\0';
		#endif

		return (LPTSTR)(m_ts.c_str());
	}


protected:
	basic_string<TCHAR> m_ts;
};


template<typename _KT, typename _Ty>
class CMultiMap : public multimap<_KT, _Ty>
{
	typedef _KT key_type;
	typedef _Ty map_type;
	typedef typename CMultiMap<_KT, _Ty>::iterator MIT;
	typedef typename CMultiMap<_KT, _Ty>::const_iterator MCIT;

public:
	CMultiMap()
	{
	}

	~CMultiMap()
	{
	}

	long Insert(key_type s, map_type d)
	{
		pair<key_type, map_type> value(s, d);
		insert(value);
		return 0;
	}

	MIT Remove(key_type s, map_type *pD = 0)
	{
		MIT it = find(s);
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

	long FindSecond(key_type s, map_type *pD = 0) const
	{
		MCIT it = find(s);
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
		for(MCIT it = begin(); it != end(); it++)
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
};


static void SplitString(std::basic_string<TCHAR> str, std::basic_string<TCHAR> pattern, std::vector<std::basic_string<TCHAR>> &vOut)
{
	std::basic_string<TCHAR>::size_type pos = 0, index = 0;

	do
	{
		pos = str.find(pattern, index);
		if(pos == std::basic_string<TCHAR>::npos)
		{
			if(index < str.size())
			{
				vOut.push_back(str.substr(index));
				DLog2(LOG_LEVEL_DEBUG, _T("Sub1 ServiceName=%s"), str.substr(index).c_str());
			}
			break;
		}
		else
		{
			DLog2(LOG_LEVEL_DEBUG, _T("Sub2 ServiceName=%s"), str.substr(index, pos - index).c_str());
			vOut.push_back(str.substr(index, pos - index));
			index = pos + 1;
		}
	}while(1);
}

}

#endif //_CSTLUNIT_H_20151119

