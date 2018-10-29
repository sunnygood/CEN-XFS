/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSEvent.h
* Description: the base class of XFS event,
*              you can overwrite this class if you need.
* Author:      liuguoxun
* Date:        2015-12-18
*
*****************************************************************************/

#ifndef  _CXFSEVENT_H_20151218
#define  _CXFSEVENT_H_20151218

#include "IXFSEvent.h"


//mode one(use class which write by user themself)

class CXFSEvent : public IXFSEvent
{
public:
	virtual HRESULT OnComplete(LPWFSRESULT lpResult, UINT nMsgID)
	{
		switch(nMsgID)
		{
			case WFS_OPEN_COMPLETE:
			case WFS_CLOSE_COMPLETE:
			case WFS_LOCK_COMPLETE:
			case WFS_UNLOCK_COMPLETE:
			case WFS_REGISTER_COMPLETE:
			case WFS_DEREGISTER_COMPLETE:
			case WFS_GETINFO_COMPLETE:
			case WFS_EXECUTE_COMPLETE:
			{
				// do you need here
				DumpResult(lpResult, nMsgID);
				break;
			}
			default:
			{
				break;
			}
		}
		
		return 0;
	}

	virtual HRESULT OnEvent(LPWFSRESULT lpResult, UINT nMsgID)
	{
		switch(nMsgID)
		{
			case WFS_EXECUTE_EVENT:
			case WFS_SERVICE_EVENT:
			case WFS_USER_EVENT:
			case WFS_SYSTEM_EVENT:
			{
				// do you need here
				DumpResult(lpResult, nMsgID);
				break;
			}
			default:
			{
				break;
			}
		}
		
		return 0;
	}

	virtual BOOL FormatOut(long nColorType, LPCTSTR lpFormat)
	{
		::MessageBox(NULL, lpFormat, _T("FormatOut"), MB_OK);
		return TRUE;
	}

public:
	CXFSEvent()
	{
	}

	virtual ~CXFSEvent()
	{
	}
};



/*
//mode two(aggregate class)
#include "Class.h"
typedef  Class *     AGGREGATECLASSPtr;

class CXFSEvent : public IXFSEvent
{
public:
	virtual HRESULT OnComplete(LPWFSRESULT lpResult, UINT nMsgID)
	{
		if(NULL != m_pCallbackClass)
		{
			m_pCallbackClass->OnComplete(lpResult, nMsgID);
		}

		return 0;
	}

	virtual HRESULT OnEvent(LPWFSRESULT lpResult, UINT nMsgID)
	{
		if(NULL != m_pCallbackClass)
		{
			m_pCallbackClass->OnEvent(lpResult, nMsgID);
		}

		return 0;
	}

	virtual BOOL FormatOut(long nColorType, LPCTSTR lpFormat)
	{
		if(NULL != m_pCallbackClass)
		{
			return m_pCallbackClass->OnFormatOut(nColorType, lpFormat);
		}

		return FALSE;
	}


public:
	void SetCallbackClass(LPVOID p)
	{
		m_pCallbackClass = (AGGREGATECLASSPtr)p;
	}

	CXFSEvent()
	{
		m_pCallbackClass = NULL;
	}

	virtual ~CXFSEvent()
	{
	}


private:
	AGGREGATECLASSPtr m_pCallbackClass;
};
*/



/*
//mode three(use call back function)

//callback function pointer which is called at event fire
typedef HRESULT (CALLBACK *XFSEVENTPROC)(LPWFSRESULT, DWORD);

class CXFSEvent : public IXFSEvent
{
public:
	virtual HRESULT OnComplete(LPWFSRESULT lpResult, UINT nMsgID)
	{
		if(NULL != m_pCallbackComplete)
		{
			m_pCallbackComplete(lpResult, nMsgID);
		}

		return 0;
	}

	virtual HRESULT OnEvent(LPWFSRESULT lpResult, UINT nMsgID)
	{
		if(NULL != m_pCallbackEvent)
		{
			m_pCallbackEvent(lpResult, nMsgID);
		}

		return 0;
	}

	virtual BOOL FormatOut(long nColorType, LPCTSTR lpFormat)
	{
		::MessageBox(NULL, lpFormat, _T("FormatOut"), MB_OK);
		return TRUE;
	}


public:
	void SetCallbackComplete(XFSEVENTPROC p)
	{
		m_pCallbackComplete = p;
	}

	void SetCallbackEvent(XFSEVENTPROC p)
	{
		m_pCallbackEvent = p;
	}

	CXFSEvent()
	{
		m_pCallbackComplete = NULL;
		m_pCallbackEvent = NULL;
	}

	virtual ~CXFSEvent()
	{
	}


private:
	XFSEVENTPROC m_pCallbackComplete;
	XFSEVENTPROC m_pCallbackEvent;
};
*/

#endif //_CXFSEVENT_H_20151218
