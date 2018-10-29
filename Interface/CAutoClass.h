/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CAutoClass.h
* Description: the auto class
*
* Author:      liuguoxun
* Date:        2015-10-19         
*
*****************************************************************************/

#ifndef _CAUTOCLASS_H_20151019
#define _CAUTOCLASS_H_20151019


#define AUTO_LOCK(a)			CAutoLock _autolock##a(a)
#define AUTO_LOCK2(a, b)		CAutoLock _autolock##a(b)
#define AUTO_UNLOCK(a)			_autolock##a.UnLock()

#define MUTEX_TIMEOUT   0
#define MUTEX_SUCCESS   1
#define MUTEX_FAILED    2
#define MUTEX_CANCEL    4

class CAutoMutex
{
public:
	CAutoMutex(HANDLE hMutex, HANDLE hCancel = NULL, DWORD dwTimeout = INFINITE)
	{
		m_hMutex = NULL;
		m_nStatus = MUTEX_FAILED;
		DWORD nCount = 0;
		HANDLE hWaitHandles[2] = {0};

		if(NULL != hMutex)
		{
			m_hMutex = hMutex;
			hWaitHandles[nCount++] = hMutex;
		}

		if(NULL != hCancel)
		{
			hWaitHandles[nCount++] = hCancel;
		}

		if(nCount > 0)
		{
			DWORD dwWait = WaitForMultipleObjects(nCount, hWaitHandles, FALSE, dwTimeout);
			if(dwWait == WAIT_TIMEOUT)
			{
				m_nStatus = MUTEX_TIMEOUT;
			}
			else if(NULL != hMutex && dwWait == WAIT_OBJECT_0)
			{
				m_nStatus = MUTEX_SUCCESS;
			}
			else if(NULL != hCancel && ((nCount == 1 && dwWait == WAIT_OBJECT_0) || (nCount == 2 && dwWait == WAIT_OBJECT_0 + 1)))
			{
				m_nStatus = MUTEX_CANCEL;
			}
			else
			{
				m_nStatus = MUTEX_FAILED;
			}
		}
	}

	~CAutoMutex()
	{
		if(m_nStatus == MUTEX_SUCCESS)
		{
			ReleaseMutex(m_hMutex);
		}
	}

	int MutexStatus()	
	{ 
		return m_nStatus;
	}


protected:
	HANDLE m_hMutex;
	int    m_nStatus;
};

class CAutoLock
{
public:
	CAutoLock(HANDLE hMutex)
	{
		m_hMutex = hMutex;
		m_bLock = false;
		if(NULL != hMutex)
		{
			DWORD dwRet = WaitForSingleObject(hMutex, INFINITE);
			if(dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED)
			{
				m_bLock = true;
			}
		}
	}

	void UnLock()
	{
		if(m_bLock && NULL != m_hMutex)
		{
			ReleaseMutex(m_hMutex);
		}

		m_bLock = false;
	}

	~CAutoLock()
	{
		UnLock();
	}

protected:
	HANDLE m_hMutex;
	bool   m_bLock;
};

class CAutoReleaseWnd
{
public:
	CAutoReleaseWnd(LPCTSTR pName, HWND hWnd, HINSTANCE hInst)
	{
		m_pName = pName;
		m_hInst = hInst;
		m_hWnd  = hWnd;
	}

	~CAutoReleaseWnd()
	{
		if(IsWindow(m_hWnd))
		{
			DestroyWindow(m_hWnd);
		}

		if(NULL != m_pName)
		{
			UnregisterClass(m_pName, m_hInst);
		}

		m_hWnd = NULL;
		m_pName = NULL;
		m_hInst = NULL;
	}

protected:
	LPCTSTR   m_pName;
	HWND      m_hWnd;
	HINSTANCE m_hInst;
};

class CAutoFreeBuffer
{
public:
	CAutoFreeBuffer(void *p)
	{
		m_p = p;
	}

	~CAutoFreeBuffer()
	{
		if(NULL != m_p)
		{
			free(m_p);
		}
		m_p = NULL;
	}

protected:
	void *m_p;
};

class CAutoDeleteBuffer
{
public:
	CAutoDeleteBuffer(void *p)
	{
		m_p = p;
	}

	~CAutoDeleteBuffer()
	{
		if(NULL != m_p)
		{
			delete [] m_p;
		}
		m_p = NULL;
	}

protected:
	void *m_p;
};

class CAutoSetPointer
{
public:
	CAutoSetPointer(LPVOID *pp, LPVOID pValue)
	{
		m_pp = pp;
		m_pValue = pValue;
	}

	~CAutoSetPointer()
	{
		if(m_pp && *m_pp)
		{
			*m_pp = m_pValue;
		}
	}

protected:
	LPVOID *m_pp;
	LPVOID m_pValue;
};


class mutex
{
public:
	mutex(void) {  InitializeCriticalSection(&m_cs); }
	~mutex(void) { DeleteCriticalSection(&m_cs); }

	inline void lock(void) { EnterCriticalSection(&m_cs); }	
	inline void unlock(void) { LeaveCriticalSection(&m_cs); }	
	inline void acquire(void) { lock(); }	
	inline void release(void) { unlock(); }	

private:	
    mutex(const mutex&);
    mutex& operator=(const mutex&);

private:
	CRITICAL_SECTION m_cs;
};

class  mutex_lock
{
public:
	mutex_lock(mutex &m) : m_mutex(m) { m_mutex.lock();}
	~mutex_lock() {	m_mutex.unlock(); }
private:
    mutex_lock(const mutex_lock&);
    mutex_lock& operator=(const mutex_lock&);
private:
	mutex& m_mutex;
};

#endif	//_CAUTOCLASS_H_20151019

