/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSSPKernel.cpp
* Description: kernel class for all SP
*
* Author:      liuguoxun
* Date:        2015-10-13
*
*****************************************************************************/


#include "CXFSSPKernel.h"


#define FLAG_EXITSP   0xEA17CC08

#define SAME_OWNER(pSessionA, pSessionB)  (pSessionA->hApp == pSessionB->hApp)

extern CMutexMap<LPVOID, CXFSSPKernel*> g_mapThis;


CXFSSPKernel::CXFSSPKernel()
{ 
	RLog2(LOG_LEVEL_DEBUG, "Enter");
	m_pSPManage = NULL;
	m_pSPFace   = NULL;
	m_hSPObject = NULL;
	m_hSPMutex       = NULL;
	m_hExitEvent     = NULL;
	m_hExitEventDisp = NULL;
	m_hExitEventExec = NULL;
	m_hExitEventIdle = NULL;
	m_hBusyEventDisp = NULL;
	m_hBusyEventExec = NULL;
	m_hBusyEventIdle = NULL;
	m_eSPLockState = LS_UNLOCKED;
	m_nKernelIdleElapse = 8000;
	memset(m_szServiceName, 0, sizeof(m_szServiceName));
	memset(m_szWorkStationName, 0, sizeof(m_szWorkStationName));

	m_hMutexSPLockState = ::CreateMutex(NULL, FALSE, NULL);

	m_hExitEvent     = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hExitEventDisp = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hExitEventExec = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hExitEventIdle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hBusyEventDisp = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hBusyEventExec = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hBusyEventIdle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	
	m_mapLockState.InitMutex();
	m_mapTimer.InitMutex();
	m_mapTimerDefer.InitMutex();
	m_mapParallerExecCmd.InitMutex();
	m_vectorDeferredInfoCmd.InitMutex();
	m_vectorRegEvt.InitMutex();
	m_vectorSession.InitMutex();
	m_listExec.InitMutex();
	m_listDeferredReq.InitMutex();
	m_listLock.InitMutex();	
	m_listReq.InitMutex();
	m_listCancel.InitMutex();
}

CXFSSPKernel::~CXFSSPKernel()
{
	if(NULL != m_hExitEvent)
	{
		CloseHandle(m_hExitEvent);
		m_hExitEvent = NULL;
	}

	if(NULL != m_hExitEventDisp)
	{
		CloseHandle(m_hExitEventDisp);
		m_hExitEventDisp = NULL;
	}

	if(NULL != m_hExitEventExec)
	{
		CloseHandle(m_hExitEventExec);
		m_hExitEventExec = NULL;
	}

	if(NULL != m_hExitEventIdle)
	{
		CloseHandle(m_hExitEventIdle);
		m_hExitEventIdle = NULL;
	}

	if(NULL != m_hBusyEventDisp)
	{
		CloseHandle(m_hBusyEventDisp);
		m_hBusyEventDisp = NULL;
	}

	if(NULL != m_hBusyEventExec)
	{
		CloseHandle(m_hBusyEventExec);
		m_hBusyEventExec = NULL;
	}

	if(NULL != m_hBusyEventIdle)
	{
		CloseHandle(m_hBusyEventIdle);
		m_hBusyEventIdle = NULL;
	}

	if(NULL != m_hMutexSPLockState)
	{
		ReleaseMutex(m_hMutexSPLockState);
		CloseHandle(m_hMutexSPLockState);
		m_hMutexSPLockState = NULL;
	}

	if(NULL != m_hSPMutex)
	{
		ReleaseMutex(m_hSPMutex);
		CloseHandle(m_hSPMutex);
		m_hSPMutex = NULL;
	}
}

//===========================================================================//
//                          Base virtual function                            //
//===========================================================================//
HRESULT CXFSSPKernel::Run(IXFSSPFace *pSPFace, IXFSSPManage *pSPManage, LPCTSTR lpServiceName)
{
	SetProcessShutdownParameters(0x270, SHUTDOWN_NORETRY);

	HRESULT hRes = 0;
	m_pSPFace   = pSPFace;
	m_pSPManage = pSPManage;
	if(NULL != lpServiceName)
	{
		TCSCPY_S(m_szServiceName, MAX_SP_NAME, lpServiceName);
		RLog2(LOG_LEVEL_DEBUG, "ServiceName=%s pSPFace=0x%X pSPManage=0x%X", m_szServiceName, pSPFace, pSPManage);
	}

	if(NULL == m_pSPManage || NULL == m_pSPFace || TCSLEN(m_szServiceName) == 0)
	{
		MessageBox(NULL, _T("Run at mistake way!"), _T("CXFSSPKernel"), MB_OK | MB_ICONSTOP);
		return WFS_ERR_INTERNAL_ERROR;
	}

	if(!m_pSPManage->IsRegistrationCodeOK())
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid registration code.");
		return WFS_ERR_INTERNAL_ERROR;
	}

	m_pSPManage->CreateInteractiveObject(&FaceRecvProc, m_szServiceName, g_hModule, &m_hSPObject);
	if(NULL == m_hSPObject)
	{
		RLog2(LOG_LEVEL_ERROR, "Create object failed.");
		return WFS_ERR_INTERNAL_ERROR;
	}

	TCHAR szWindowClass[MAX_PATH] = {0};
	SPRINTF_S(szWindowClass, MAX_PATH, XFSSPWNDCLASS, m_szServiceName);
	CAutoReleaseWnd _autoreleasewnd(szWindowClass, (HWND)m_hSPObject, g_hModule);
	g_mapThis.MutexUpdate(m_hSPObject, this);

	TCHAR szMutexName[MAX_PATH] = {0};
	SPRINTF_S(szMutexName, MAX_PATH, XFSSPPROCESSMUTEX, m_szServiceName);
	m_hSPMutex = CreateMutex(NULL, FALSE, szMutexName);
	if(WaitForSingleObject(m_hSPMutex, 50) != WAIT_OBJECT_0)
	{
		MessageBox(NULL, _T("SP was already running!"), _T("CXFSSPKernel"), MB_OK | MB_ICONSTOP);
		return WFS_ERR_INTERNAL_ERROR;
	}

	m_pSPManage->GetServiceNodeStr(m_szServiceName, _T("WorkStationName"), m_szWorkStationName, MAX_PATH);
	m_nKernelIdleElapse = m_pSPManage->GetServiceNodeInt(m_szServiceName, _T("KernelIdleElapse"), 8000);

	TCHAR szClass[MAX_PATH] = {0};
	m_pSPManage->GetServiceNodeStr(m_szServiceName, _T("Class"), szClass, MAX_PATH);
	if(TCSLEN(szClass) == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid parameter Class, ServiceName=[%s]", m_szServiceName);
		return EC_INVALID_PARA;
	}
	m_pSPManage->LoadSPStruct(szClass);


	HANDLE hDispThread = CreateEventThread(TF_DISP);
	HANDLE hExecThread = CreateEventThread(TF_EXEC);
	HANDLE hIdleThread = CreateEventThread(TF_IDLE);

	HANDLE hHandles[3] = {NULL, NULL, NULL};
	hHandles[0] = hDispThread;
	hHandles[1] = hExecThread;
	hHandles[2] = hIdleThread;
	DWORD dwRet = WaitForMultipleObjects(3, hHandles, FALSE, 50);
	if(dwRet == WAIT_OBJECT_0 || dwRet == WAIT_OBJECT_0 + 1 || dwRet == WAIT_OBJECT_0 + 2)
	{
		SetEvent(m_hExitEvent);
		Sleep(50);

		CloseHandle(hDispThread);
		CloseHandle(hExecThread);
		CloseHandle(hIdleThread);
		RLog2(LOG_LEVEL_ERROR, "Create thread failed, GetLastError()=%d", GetLastError());
		return WFS_ERR_INTERNAL_ERROR;
	}

	::SetThreadPriority(hExecThread, THREAD_PRIORITY_BELOW_NORMAL);
	::SetThreadPriority(hIdleThread, THREAD_PRIORITY_BELOW_NORMAL);

	hRes = m_pSPFace->OnSPStart();
	if(hRes != 0)
	{
		RLog2(LOG_LEVEL_ERROR, "OnSPStart return failed, hRes=%d", hRes);
	}

	MSG msg;
	BOOL bRet = FALSE;
	while(1)
	{
		bRet = ::GetMessage(&msg, NULL, 0, 0);
		if(bRet)
		{
			if(bRet == -1)
			{
				continue;
			}
			else
			{
				//RLog2(LOG_LEVEL_DEBUG, "message=0x%X wParam=0x%X", msg.message, msg.wParam);
				if(msg.message == WM_TIMER && msg.hwnd == (HWND)m_hSPObject)
				{
					FaceOnTimer(msg.wParam, (LPVOID)msg.lParam);
					continue;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if(msg.message == WM_QUIT && msg.wParam == FLAG_EXITSP)
			{
				RLog2(LOG_LEVEL_DEBUG, "Quit SP......");
				break;
			}
			else
			{
				continue;
			}
		}
	}

	SetEvent(m_hExitEvent);

	hHandles[0] = m_hExitEventDisp;
	hHandles[1] = m_hExitEventExec;
	hHandles[2] = m_hExitEventIdle;
	if(WaitForMultipleObjects(3, hHandles, TRUE, INFINITE) == WAIT_OBJECT_0)
	{
		RLog2(LOG_LEVEL_DEBUG, "All sub thread was exit");
		CloseHandle(hDispThread);
		CloseHandle(hExecThread);
		CloseHandle(hIdleThread);
	}

	hRes = m_pSPFace->OnSPExit();
	if(hRes != 0)
	{
		RLog2(LOG_LEVEL_ERROR, "OnSPExit return failed, hRes=%d", hRes);
	}

	RemoveAllTimer();
	RemoveRegEvent();
	RemoveSession();
	return 0;
}

LPCTSTR CXFSSPKernel::GetLogicalName(REQUESTID RequestID) const
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");

	LPSESSION_DATA pSession = NULL;
	if(m_listExec.size() > 0)
	{
		HSERVICE hService = GetSessionID(RequestID);
		if(NULL != hService)
		{
			pSession = FindSession(hService);
		}
	}

	if(NULL != pSession)
	{
		return pSession->szLogicalName;
	}
	else if(m_vectorSession.size() > 0)
	{
		return m_vectorSession.at(0)->szLogicalName;
	}
	else
	{
		return NULL;
	}
}

inline REQUESTID CXFSSPKernel::GetRequestID() const
{
	//RLog2(LOG_LEVEL_DEBUG, "Enter");
	return const_cast<CXFSSPKernel*>(this)->OPExecContext(0, OP_GET_REQUESTID); 
}

inline HSERVICE CXFSSPKernel::GetSessionID(REQUESTID RequestID) const
{
	//RLog2(LOG_LEVEL_DEBUG, "Enter");
	return (HSERVICE)const_cast<CXFSSPKernel*>(this)->OPExecContext(RequestID, OP_GET_SESSION);
}

DWORD CXFSSPKernel::GetTotalTimeout(REQUESTID RequestID) const
{
	RLog2(LOG_LEVEL_DEBUG, "RequestID=%d", RequestID);

	DWORD dwTimeout = const_cast<CXFSSPKernel*>(this)->OPExecContext(RequestID, OP_GET_TOTALTIME);
	if(dwTimeout == 0)
	{
		return INFINITE;
	}
	else
	{
		return dwTimeout;
	}
}

BOOL CXFSSPKernel::IsRequestTimeout(REQUESTID RequestID) const
{
	//RLog2(LOG_LEVEL_DEBUG, "RequestID=%d", RequestID);
	return (BOOL)const_cast<CXFSSPKernel*>(this)->OPExecContext(RequestID, OP_GET_TIMEOUT);
}

BOOL CXFSSPKernel::IsRequestCancelled(REQUESTID RequestID) const
{
	//RLog2(LOG_LEVEL_DEBUG, "RequestID=%d", RequestID);
	return (BOOL)const_cast<CXFSSPKernel*>(this)->OPExecContext(RequestID, OP_GET_CANCEL);
}

DWORD CXFSSPKernel::GetXFSVersion(REQUESTID RequestID) const
{
	RLog2(LOG_LEVEL_DEBUG, "RequestID=%d", RequestID);

	LPSESSION_DATA pSession = NULL;		
	HSERVICE hService = GetSessionID(RequestID);
	if(0 != hService)
	{
		pSession = FindSession(hService);
	}

	if(NULL != pSession)
	{
		return pSession->dwSrvcVerRequired;
	}
	else if(m_vectorSession.size() > 0)
	{
		return m_vectorSession.at(0)->dwSrvcVerRequired;
	}
	else
	{
		return 0;
	}
}

BOOL CXFSSPKernel::SetResult(LPVOID pBuffer, REQUESTID RequestID)
{
	RLog2(LOG_LEVEL_DEBUG, "RequestID=%d pBuffer=0x%X", RequestID, pBuffer);
	return (BOOL)OPExecContext(RequestID, OP_SET_BUFFER, pBuffer);
}

HRESULT CXFSSPKernel::RegisterParallerExecCmd(DWORD dwPCmd, DWORD dwCurCmd)
{
	RLog2(LOG_LEVEL_DEBUG, "dwPCmd=%d dwCurCmd=%d m_mapParallerExecCmd.size()=%d", dwPCmd, dwCurCmd, m_mapParallerExecCmd.size());

	return m_mapParallerExecCmd.MutexInsert(dwPCmd, dwCurCmd);
}

HRESULT CXFSSPKernel::RegisterDeferredInfoCmd(DWORD dwCategory)
{
	RLog2(LOG_LEVEL_DEBUG, "dwCategory=%d m_vectorDeferredInfoCmd.size()=%d", dwCategory, m_vectorDeferredInfoCmd.size());

	return m_vectorDeferredInfoCmd.MutexAdd(dwCategory);
}

HRESULT CXFSSPKernel::SetTimer(UINT nTimerID, UINT nElapse, LPVOID lpContext)
{
	if(nTimerID > (0xFFFFFFFF - BASE_TIMER_ID))
	{
		return WFS_ERR_INVALID_TIMER;
	}

	return FaceSetTimer(nTimerID + BASE_TIMER_ID, nElapse, lpContext);
}

HRESULT CXFSSPKernel::KillTimer(UINT nTimerID)
{
	if(nTimerID > (0xFFFFFFFF - BASE_TIMER_ID))
	{
		return WFS_ERR_INVALID_TIMER;
	}
	
	return FaceKillTimer(nTimerID + BASE_TIMER_ID);
}

HRESULT CXFSSPKernel::SIUGetInfo(DWORD dwCategory, LPVOID lpQueryDetail)
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
	return 0;
}

HRESULT CXFSSPKernel::SIUControl(DWORD dwType, DWORD dwIndex, DWORD dwCommand)
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
	return 0;
}

HRESULT CXFSSPKernel::WFMAlloc2Buffer(DWORD dwSizeFirst, DWORD dwSizeSecond, DWORD dwFlag, LPVOID *ppFirst, LPVOID *ppSecond)
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
	if(NULL == ppFirst)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	HRESULT hRes = 0;
	if(dwSizeFirst > 0)
	{
		hRes = WFMAllocateBuffer(dwSizeFirst + 1, dwFlag, ppFirst);
		if(hRes != 0)
		{
			return hRes;
		}
	}

	if(dwSizeSecond > 0)
	{
		hRes = WFMAllocateMore(dwSizeSecond + 1, *ppFirst, ppSecond);
		if(hRes != 0 && dwSizeFirst > 0)
		{
			WFMFreeBuffer(*ppFirst);
		}
	}

	return hRes;
}

HRESULT CXFSSPKernel::FireExecEvent(DWORD dwEventID, HRESULT hResult, LPVOID pData, REQUESTID RequestID, SYSTEMTIME *pTimestamp)
{
	return FireEvent(XFS_EXECUTE_EVENT, dwEventID, hResult, pData, RequestID, pTimestamp);
}

HRESULT CXFSSPKernel::FireServEvent(DWORD dwEventID, HRESULT hResult, LPVOID pData, REQUESTID RequestID, SYSTEMTIME *pTimestamp)
{
	return FireEvent(XFS_SERVICE_EVENT, dwEventID, hResult, pData, RequestID, pTimestamp);
}

HRESULT CXFSSPKernel::FireUserEvent(DWORD dwEventID, HRESULT hResult, LPVOID pData, REQUESTID RequestID, SYSTEMTIME *pTimestamp)
{
	return FireEvent(XFS_USER_EVENT, dwEventID, hResult, pData, RequestID, pTimestamp);
}

HRESULT CXFSSPKernel::FireSystEvent(DWORD dwEventID, HRESULT hResult, LPVOID pData, REQUESTID RequestID, SYSTEMTIME *pTimestamp)
{
	return FireEvent(XFS_SYSTEM_EVENT, dwEventID, hResult, pData, RequestID, pTimestamp);
}

HRESULT CXFSSPKernel::FireDevStatus(DWORD dwState)
{
	WFSDEVSTATUS stStatus;
	stStatus.dwState = dwState;
	stStatus.lpszPhysicalName = m_szServiceName;
	stStatus.lpszWorkstationName = m_szWorkStationName;

	return FireSystEvent(WFS_SYSE_DEVICE_STATUS, WFS_SUCCESS, &stStatus);
}

HRESULT CXFSSPKernel::FireSystError(DWORD dwEventID, DWORD dwAction, HRESULT hResult, REQUESTID RequestID, DWORD dwSize, LPBYTE lpDesc)
{
	//RLog2(LOG_LEVEL_DEBUG, "Enter");

	LPSESSION_DATA pSession = NULL;
	HSERVICE hService = GetSessionID(RequestID);
	if(hService == 0)
	{
		if(m_vectorSession.size() > 0)
		{
			pSession = m_vectorSession.at(0);
		}
	}
	else
	{
		pSession = FindSession(hService);
	}

	if(NULL == pSession)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	WFSHWERROR stHWError;
	stHWError.lpszLogicalName		= pSession->szLogicalName;
	stHWError.lpszPhysicalName		= m_szServiceName;
	stHWError.lpszWorkstationName	= m_szWorkStationName;
	stHWError.lpszAppID			    = pSession->szAppID;
	stHWError.dwAction				= dwAction;
	stHWError.dwSize				= dwSize;
	stHWError.lpbDescription		= lpDesc;

	return FireSystEvent(dwEventID, hResult, &stHWError, RequestID);
}

//===========================================================================//
//                            Inner Function                                 //
//===========================================================================//
HRESULT CXFSSPKernel::FireEvent(DWORD dwEventType, DWORD dwEventID, HRESULT hResult, LPVOID pData, REQUESTID RequestID, SYSTEMTIME *pTimestamp)
{
	RLog2(LOG_LEVEL_DEBUG, "dwEventType=0x%X dwEventID=%d hResult=%d RequestID=%d", dwEventType, dwEventID, hResult, RequestID);

	if(m_vectorRegEvt.size() == 0)
	{
		RLog2(LOG_LEVEL_DEBUG, "Event list is empty now, please register it.");
		return 0;
	}

	HRESULT hRes = 0;
	std::list<SPPACK> FireList;
	FireList.clear();

	DWORD dwEventClass = Msg2EventClass(dwEventType);

	SYSTEMTIME stTime;
	if(NULL == pTimestamp)
	{
		::GetLocalTime(&stTime);
	}
	else
	{
		stTime = *pTimestamp;
	}

	m_vectorRegEvt.Lock();
	for(MVREIT it = m_vectorRegEvt.begin(); it != m_vectorRegEvt.end(); it++)
	{
		if(dwEventClass & (*it)->dwEventClass)
		{
			HSERVICE hRegService = (*it)->hService;
			if(dwEventType == WFS_EXECUTE_EVENT && hRegService != GetSessionID(RequestID))
			{
				continue;
			}

			SPPACK stPack;
			stPack.uMsg = dwEventType;
			stPack.hWnd = (*it)->hRegObject;
			stPack.dwTimeout = 0;
			stPack.u.dwEventID = dwEventID;
			stPack.ups.hResult  = hResult;
			stPack.lpBuffer = pData;
			stPack.hService = hRegService;
			stPack.tsTimestamp = stTime;

			if(RequestID == 0 && dwEventType == WFS_EXECUTE_EVENT)
			{
				stPack.RequestID = GetRequestID();
			}
			else
			{
				stPack.RequestID = RequestID;
			}

			FireList.push_back(stPack);
		}
	}
	m_vectorRegEvt.UnLock();

	for(std::list<SPPACK>::iterator it = FireList.begin(); it != FireList.end(); it++)
	{
		SPPACK stPack = *it;
		hRes = Face2SPI(&stPack);
	}

	return 0;
}

HRESULT CXFSSPKernel::FireUndeliverable(LPTSTR lpLogicalName, LPTSTR lpAppID, DWORD dwSize, LPBYTE lpDesc, DWORD dwMsg, LPWFSRESULT lpWFSResult)
{
	WFSUNDEVMSG stUnDevMsg;
	stUnDevMsg.lpszLogicalName	    = lpLogicalName;
	stUnDevMsg.lpszWorkstationName  = m_szWorkStationName;
	stUnDevMsg.lpszAppID		    = lpAppID;
	stUnDevMsg.dwSize				= dwSize;
	stUnDevMsg.lpbDescription		= lpDesc;
	stUnDevMsg.dwMsg                = dwMsg;
	stUnDevMsg.lpWFSResult          = lpWFSResult;

	return FireSystEvent(WFS_SYSE_UNDELIVERABLE_MSG, WFS_SUCCESS, &stUnDevMsg);
}

HRESULT CXFSSPKernel::FireDisconnect(LPTSTR lpLogicalName, LPTSTR lpAppID)
{
	WFSAPPDISC stAppdisc;
	stAppdisc.lpszLogicalName	  = lpLogicalName;
	stAppdisc.lpszWorkstationName = m_szWorkStationName;
	stAppdisc.lpszAppID			  = lpAppID;

	return FireSystEvent(WFS_SYSE_APP_DISCONNECT, WFS_SUCCESS, &stAppdisc);
}

HRESULT CXFSSPKernel::FireLockReqested()
{
	//RLog2(LOG_LEVEL_DEBUG, "Enter");

	SPPACK stPack;
	HRESULT hRes = WFS_SUCCESS;
	HSERVICE hLockService = 0;
	hRes = m_mapLockState.FindFirst(LS_LOCKED, &hLockService);
	if(hRes != WFS_SUCCESS)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	DWORD dwEventClass = Msg2EventClass(WFS_SYSTEM_EVENT);

	SYSTEMTIME stTime;
	::GetLocalTime(&stTime);
	
	m_vectorRegEvt.Lock();
	for(MVREIT it = m_vectorRegEvt.begin(); it != m_vectorRegEvt.end(); it++)
	{
		if(hLockService == (*it)->hService && (dwEventClass & (*it)->dwEventClass))
		{
			stPack.uMsg = WFS_SYSTEM_EVENT;
			stPack.hWnd = (*it)->hRegObject;
			stPack.dwTimeout = 0;
			stPack.hService = hLockService;
			stPack.u.dwEventID = WFS_SYSE_LOCK_REQUESTED;
			stPack.ups.hResult  = 0;
			stPack.lpBuffer = NULL;
			stPack.RequestID = 0;
			stPack.tsTimestamp = stTime;
			break;
		}
	}
	m_vectorRegEvt.UnLock();

	hRes = Face2SPI(&stPack);
	if(hRes != WFS_SUCCESS)
	{
		RLog2(LOG_LEVEL_ERROR, "Fire WFS_SYSE_LOCK_REQUESTED failed, hRes=[%d]", hRes);
	}

	return hRes;
}

HRESULT CXFSSPKernel::FireVerError(DWORD dwSupportVer, HRESULT hResult, LPTSTR lpLogicalName, LPTSTR lpAppID, DWORD dwSize, LPBYTE lpDesc)
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");

	WFSVERSION stVer;
	memset(&stVer, 0, sizeof(WFSVERSION));
	stVer.wLowVersion  = HIWORD(dwSupportVer);
	stVer.wHighVersion = LOWORD(dwSupportVer);

	WFSVRSNERROR stVerError;
	stVerError.lpszLogicalName		= lpLogicalName;
	stVerError.lpszWorkstationName	= m_szWorkStationName;
	stVerError.lpszAppID		    = lpAppID;
	stVerError.dwSize				= dwSize;
	stVerError.lpbDescription		= lpDesc;
	stVerError.lpWFSVersion         = &stVer;

	return FireSystEvent(WFS_SYSE_VERSION_ERROR, hResult, &stVerError);
}

inline HRESULT CXFSSPKernel::InnerSetTimer(UINT nTimerID, UINT nElapse, LPVOID lpContext)
{
	UINT nTimer = nTimerID;
	if(nTimer >= BASE_TIMER_ID)
	{
		nTimer = nTimer - BASE_TIMER_ID + 1;
	}

	return FaceSetTimer(nTimer, nElapse, lpContext);
}

inline HRESULT CXFSSPKernel::InnerKillTimer(UINT nTimerID)
{
	UINT nTimer = nTimerID;
	if(nTimer >= BASE_TIMER_ID)
	{
		nTimer = nTimer - BASE_TIMER_ID + 1;
	}

	return FaceKillTimer(nTimer);
}

inline HRESULT CXFSSPKernel::FaceSetTimer(UINT nTimerID, UINT nElapse, LPVOID lpContext)
{
	RLog2(LOG_LEVEL_DEBUG, "nTimerID=0x%X nElapse=0x%X lpContext=0x%X", nTimerID, nElapse, lpContext);

	if(NULL == m_hSPObject)
	{
		return WFS_ERR_NOT_STARTED;
	}

	if(nTimerID == ::SetTimer((HWND)m_hSPObject, nTimerID, nElapse, NULL))
	{
		m_mapTimer.MutexUpdate(nTimerID, lpContext);
		return 0;
	}
	else
	{
		return WFS_ERR_NO_TIMER;
	}
}

inline HRESULT CXFSSPKernel::FaceKillTimer(UINT nTimerID)
{
	RLog2(LOG_LEVEL_DEBUG, "nTimerID=0x%X", nTimerID);
	m_mapTimer.MutexRemove(nTimerID);
	::KillTimer((HWND)m_hSPObject, nTimerID);
	return 0;
}

LRESULT CALLBACK CXFSSPKernel::FaceRecvProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RLog2(LOG_LEVEL_DEBUG, "hwnd=0x%X uMsg=0x%X wParam=0x%X lParam=0x%X", hwnd, uMsg, wParam, lParam);

	LRESULT nRet = 0;
	switch(uMsg)
	{
		case WM_COPYDATA:
		{
			CXFSSPKernel *pThis = NULL;
			nRet = g_mapThis.FindSecond((LPVOID)hwnd, &pThis);

			PCOPYDATASTRUCT pCopyData = (PCOPYDATASTRUCT)lParam;
			if(NULL != pCopyData && nRet == 0)
			{
				pThis->DealPack((LPCTSTR)pCopyData->lpData, pCopyData->cbData);
			}
			break;
		}
		case WM_ENDSESSION:
		{
			CXFSSPKernel *pThis = NULL;
			nRet = g_mapThis.FindSecond((LPVOID)hwnd, &pThis);
			if(nRet == 0)
			{
				pThis->ExitSP();
			}
			break;
		}
		default:
		{
			nRet = ::DefWindowProc(hwnd, uMsg, wParam, lParam);
			break;
		}
	}

	return nRet;
}

DWORD CALLBACK CXFSSPKernel::SubThreadEntry(LPVOID pParam)
{
	LPTHREADPARASIN pTPS = (LPTHREADPARASIN)pParam;
	if(NULL == pTPS)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid pParam pointer.");
		return EC_POINTER_NULL;
	}

	DWORD nRet = 0;
	DWORD dwFlag = pTPS->dwValue;
	CXFSSPKernel *pThis = (CXFSSPKernel *)pTPS->pThis;
	delete pTPS;

	if(NULL == pThis)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid pThis pointer.");
		return EC_POINTER_NULL;
	}

	switch(dwFlag)
	{
		case TF_DISP:
		{
			nRet = pThis->DispatchThreadProc();
			break;
		}
		case TF_EXEC:
		{
			nRet = pThis->ExecThreadProc();
			break;
		}
		case TF_IDLE:
		{
			nRet = pThis->IdleThreadProc();
			break;
		}
	}

	return nRet;
}

UINT CXFSSPKernel::DispatchThreadProc()
{
	DWORD dwWaitRet = 0;
	HANDLE hHandles[2] = {NULL, NULL};
	hHandles[0] = m_hExitEvent;
	hHandles[1] = m_hBusyEventDisp;

	while(1)
	{
		dwWaitRet = WaitForMultipleObjects(2, hHandles, FALSE, INFINITE);
		if(dwWaitRet == WAIT_OBJECT_0 + 1)
		{
			//RLog2(LOG_LEVEL_DEBUG, "DispatchThreadProc Enter");
			switch(m_eSPLockState)
			{
				case LS_UNLOCKED:
				{
					DoAtUnLock();
					break;
				}
				case LS_LOCKED:
				{
					DoAtLocked();
					break;
				}
				case LS_PENDING:
				{
					DoAtPending();
					break;
				}
			}

			if(m_listDeferredReq.size() > 0)
			{
				SetEvent(m_hBusyEventExec);
			}

			if(m_listReq.size() == 0 && m_listLock.size() == 0)
			{
				ResetEvent(m_hBusyEventDisp);
			}
		}
		else if(dwWaitRet == WAIT_OBJECT_0)
		{
			break;
		}
		else
		{
			RLog2(LOG_LEVEL_WARN, "Unexpected result[0x%X]", dwWaitRet);
		}
	}

	SetEvent(m_hExitEventDisp);
	return 0;
}

UINT CXFSSPKernel::ExecThreadProc()
{
	DWORD dwWaitRet = 0;
	HANDLE hHandles[2] = {NULL, NULL};
	hHandles[0] = m_hExitEvent;
	hHandles[1] = m_hBusyEventExec;

	while(1)
	{
		dwWaitRet = WaitForMultipleObjects(2, hHandles, FALSE, INFINITE);
		if(dwWaitRet == WAIT_OBJECT_0 + 1)
		{
			//RLog2(LOG_LEVEL_DEBUG, "ExecThreadProc Enter");
			if(m_listDeferredReq.size() > 0)
			{
				LPSPPACK pPack = m_listDeferredReq.MutexPop();
				ExecListProc(pPack);
			}
			else
			{
				DeferTimerProc();
			}

			if(m_listDeferredReq.size() == 0 && m_mapTimerDefer.size() == 0)
			{
				ResetEvent(m_hBusyEventExec);
			}
		}
		else if(dwWaitRet == WAIT_OBJECT_0)
		{
			break;
		}
		else
		{
			RLog2(LOG_LEVEL_WARN, "Unexpected result[0x%X]", dwWaitRet);
		}
	}

	SetEvent(m_hExitEventExec);
	return 0;
}

UINT CXFSSPKernel::IdleThreadProc()
{
	DWORD dwWaitRet = 0;
	HANDLE hHandles[2] = {NULL, NULL};
	hHandles[0] = m_hExitEvent;
	hHandles[1] = m_hBusyEventIdle;	

	while(1)
	{
		dwWaitRet = WaitForMultipleObjects(2, hHandles, FALSE, m_nKernelIdleElapse);
		if(dwWaitRet == WAIT_OBJECT_0 + 1)
		{
			//RLog2(LOG_LEVEL_DEBUG, "IdleThreadProc event");
			CancelListProc();
			ResetEvent(m_hBusyEventIdle);
		}
		else if(dwWaitRet == WAIT_OBJECT_0)
		{
			break;
		}
		else
		{
			//RLog2(LOG_LEVEL_DEBUG, "IdleThreadProc timeout");
			m_pSPFace->OnIdleUpdate();
			CheckSessionConnect();
		}
	}

	SetEvent(m_hExitEventIdle);
	return 0;
}

inline HRESULT CXFSSPKernel::ExecListProc(LPSPPACK pPack)
{
	LPEXECONTEXT pExecContext = new EXECONTEXT();
	if(NULL == pExecContext)
	{
		RLog2(LOG_LEVEL_ERROR, "Alloc memory failed.");
		return WFS_ERR_INTERNAL_ERROR;
	}

	CAutoDeleteBuffer _autodeletebuffer(pExecContext);
	pExecContext->bCancel  = FALSE;
	pExecContext->bTimeout = FALSE;
	pExecContext->pSPPack = pPack;

	m_listExec.MutexPush(pExecContext);
	HRESULT hRes = DoAsyncRequest(pPack);
	m_listExec.MutexRemove(pExecContext);

	return hRes;
}

inline HRESULT CXFSSPKernel::CancelListProc()
{
	if(m_listCancel.size() > 0)
	{
		m_listCancel.Lock();
		for(MLSPPIT it = m_listCancel.begin(); it != m_listCancel.end(); it++)
		{
			Face2SPI(*it);
			delete (*it);
		}
		m_listCancel.clear();
		m_listCancel.UnLock();
	}

	return 0;
}

inline HRESULT CXFSSPKernel::DeferTimerProc()
{
	if(m_mapTimerDefer.size() == 0)
	{
		return 0;
	}

	CMutexMap<UINT, LPVOID> mapDeferTimer;
	m_mapTimerDefer.Lock();
	mapDeferTimer.insert(m_mapTimerDefer.begin(), m_mapTimerDefer.end());
	m_mapTimerDefer.UnLock();

	for(MMTIIT it = mapDeferTimer.begin(); it != mapDeferTimer.end(); it++)
	{
		m_pSPFace->OnTimer(it->first, it->second);
		m_mapTimerDefer.MutexRemove(it->first);
	}

	return 0;
}

HANDLE CXFSSPKernel::CreateEventThread(DWORD dwFlag)
{
	//RLog2(LOG_LEVEL_DEBUG, "Enter");

	LPTHREADPARASIN pTPS = new THREADPARASIN();
	if(NULL == pTPS)
	{
		RLog2(LOG_LEVEL_ERROR, "Alloc memory failed.");
		return NULL;
	}

	pTPS->pThis = this;
	pTPS->dwValue = dwFlag;
	pTPS->pPointer = NULL;

	DWORD dwThreadID = 0;
	HANDLE hSubThread = OS_CreateThread(&SubThreadEntry, pTPS, &dwThreadID);
	if(NULL == hSubThread)
	{
		return NULL;
	}

	//RLog2(LOG_LEVEL_DEBUG, "End");
	return hSubThread;
}

HRESULT CXFSSPKernel::DoAsyncRequest(LPSPPACK pPack)
{
	HRESULT hRes = 0;
	LPSESSION_DATA pSession = NULL;

	if(IsBadReadPtr(pPack, sizeof(SPPACK)))
	{
		RLog2(LOG_LEVEL_ERROR, "pPack is bad pointer[0x%X]", pPack);
		return WFS_ERR_INTERNAL_ERROR;
	}

	RLog2(LOG_LEVEL_DATA, "SPLockState=%d Msg=0x%X hService=%d Command=%d RequestID=%d", m_eSPLockState, pPack->uMsg, pPack->hService, pPack->u.dwCommandCode, pPack->RequestID);
	CAutoDeleteBuffer _autodeletebuffer(pPack);

	if(pPack->uMsg == XFS_OPEN)
	{
		pSession = (LPSESSION_DATA)(pPack + 1);
	}
	else
	{
		pSession = FindSession(pPack->hService);
		if(NULL == pSession)
		{
			RLog2(LOG_LEVEL_ERROR, "Invalid hService[%d], Msg=0x%X.", pPack->hService, pPack->uMsg);
			hRes = WFS_ERR_INVALID_HSERVICE;
			pPack->lpBuffer = NULL;
			goto Complete2SPI;
		}
	}

	if(NULL != pPack->lpBuffer && (pPack->uMsg == XFS_GETINFO || pPack->uMsg == XFS_EXECUTE))
	{
		CMD_FLAG eCmdType = CMDF_NONE;
		if(pPack->uMsg == XFS_GETINFO)
		{
			eCmdType = CMDF_GI;
		}
		else if(pPack->uMsg == XFS_EXECUTE)
		{
			eCmdType = CMDF_EX;
		}

		pPack->lpBuffer = pPack + 1;
		hRes = m_pSPManage->UnPack(pPack->lpBuffer, pSession->dwSrvcVerRequired, eCmdType, pPack->u.dwCommandCode);
		if(hRes != 0)
		{
			RLog2(LOG_LEVEL_ERROR, "UnPack failed.");
			hRes = WFS_ERR_INTERNAL_ERROR;
			pPack->lpBuffer = NULL;
			goto Complete2SPI;
		}
	}

	switch(pPack->uMsg)
	{
		case XFS_GETINFO:
		{
			hRes = FaceOnGetInfo(pPack);
			if(pPack->dwTimeout > 0)
			{
				InnerKillTimer(pPack->RequestID);
			}

			if(hRes != WFS_SUCCESS)
			{
				pPack->lpBuffer = NULL;
			}
			break;
		}
		case XFS_EXECUTE:
		{
			hRes = FaceOnExecute(pPack);
			if(pPack->dwTimeout > 0)
			{
				InnerKillTimer(pPack->RequestID);
			}

			if(hRes != WFS_SUCCESS)
			{
				pPack->lpBuffer = NULL;
			}
			break;
		}
		case XFS_OPEN:
		{
			hRes = FaceOnOpen(pPack, pSession);
			pPack->lpBuffer = NULL;
			if(pPack->dwTimeout > 0)
			{
				InnerKillTimer(pPack->RequestID);
			}
			break;
		}
		case XFS_CLOSE:
		{
			hRes = FaceOnClose(pPack);
			pPack->lpBuffer = NULL;
			break;
		}
		case XFS_LOCK:
		{
			hRes = FaceOnLock(pPack);
			pPack->lpBuffer = NULL;
			if(pPack->dwTimeout > 0)
			{
				InnerKillTimer(pPack->RequestID);
			}
			break;
		}
		case XFS_UNLOCK:
		{
			hRes = FaceOnUnlock(pPack);
			pPack->lpBuffer = NULL;
			break;
		}
		case XFS_REGISTER:
		{
			hRes = FaceOnRegister(pPack);
			pPack->lpBuffer = NULL;
			break;
		}
		case XFS_DEREGISTER:
		{
			hRes = FaceOnDeregister(pPack);
			pPack->lpBuffer = NULL;
			break;
		}
		default:
		{
			hRes = WFS_ERR_INTERNAL_ERROR;
			pPack->lpBuffer = NULL;
			break;
		}
	}


Complete2SPI:
	pPack->ups.hResult = hRes;
	return Face2SPI(pPack, pSession);
}

HRESULT CXFSSPKernel::DoImmediatelyRequest(LPSPPACK pPack)
{
	HRESULT hRes = 0;
	if(IsBadReadPtr(pPack, sizeof(SPPACK)))
	{
		RLog2(LOG_LEVEL_ERROR, "pPack is bad pointer[0x%X]", pPack);
		return WFS_ERR_INTERNAL_ERROR;
	}

	RLog2(LOG_LEVEL_DEBUG, "Msg=0x%X RequestID=%d", pPack->uMsg, pPack->RequestID);
	CAutoDeleteBuffer _autodeletebuffer(pPack);

	switch(pPack->uMsg)
	{
		case XFS_LOGLEVEL:
		{
			RLog2(LOG_LEVEL_DEBUG, "XFS_LOGLEVEL=%d", pPack->ups.hParameter);
			break;
		}
		case XFS_VERSIONERROR:
		{
			LPSESSION_DATA pSession = (LPSESSION_DATA)(pPack + 1);
			if(IsBadReadPtr(pSession, sizeof(SESSION_DATA)))
			{
				RLog2(LOG_LEVEL_ERROR, "Invalid session pointer when send version error.");
				hRes = WFS_ERR_INTERNAL_ERROR;
			}
			else
			{
				hRes = FireVerError(pSession->dwSrvcVerRequired, pPack->ups.hResult, pSession->szLogicalName, pSession->szAppID, 0, NULL);
			}
			break;
		}
		case XFS_CANCEL:
		{
			hRes = CancelRequests(pPack->hService, pPack->RequestID, WFS_ERR_CANCELED, TRUE);
			break;
		}
		default:
		{
			hRes = WFS_ERR_INTERNAL_ERROR;
			RLog2(LOG_LEVEL_ERROR, "Invalid message id[0x%X]", pPack->uMsg);
		}
	}

	return hRes;
}

HRESULT CXFSSPKernel::FaceOnTimer(UINT nTimerID, LPVOID lpV)
{
	LPVOID lpContext = NULL;
	long nRet = m_mapTimer.FindSecond(nTimerID, &lpContext);
	if(nRet != 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid timer ID[0x%X]", nTimerID);
		return 0;
	}

	RLog2(LOG_LEVEL_DEBUG, "nTimerID=0x%X lpContext=0x%X", nTimerID, lpContext);

	if(nTimerID >= BASE_TIMER_ID)
	{
		UINT nRealTimerID = nTimerID - BASE_TIMER_ID;
		if(nRealTimerID >= IMMEDIATELY_TIMER_BASE)
		{
			return m_pSPFace->OnTimer(nRealTimerID, lpContext);
		}
		else
		{
			m_mapTimerDefer.MutexUpdate(nRealTimerID, lpContext);
			SetEvent(m_hBusyEventExec);
			return 0;
		}
	}
	else
	{
		FaceKillTimer(nTimerID);
		return CancelTimeoutRequests((LPSPPACK)lpContext);
	}
}

HRESULT CXFSSPKernel::FaceOnOpen(LPSPPACK pPack, LPSESSION_DATA pSession)
{
	if(IsBadReadPtr(pSession, sizeof(SESSION_DATA)))
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid session pointer when open.");
		return WFS_ERR_INTERNAL_ERROR;
	}

	RLog2(LOG_LEVEL_DATA, "hService=%d szLogicalName=%s szAppID=%s dwSrvcVerRequired=%d hApp=0x%X dwProcessID=%d dwThreadID=%d", 
		pSession->hService, pSession->szLogicalName, pSession->szAppID, pSession->dwSrvcVerRequired, pSession->hApp, pSession->dwProcessID, pSession->dwThreadID);

	if(NULL != FindSession(pPack->hService))
	{
		RLog2(LOG_LEVEL_ERROR, _T("The Service[%d] was exist."), pPack->hService);
		return WFS_ERR_INVALID_HSERVICE;
	}

	HRESULT hRes = m_pSPFace->OnOpenSession(pSession->hService, pSession->szAppID, pSession->dwTraceLevel);
	if(hRes != WFS_SUCCESS)
	{
		RLog2(LOG_LEVEL_ERROR, "OnOpenSession() return failed, hRes=%d", hRes);
		return hRes;
	}

	LPSESSION_DATA pNewSession = new SESSION_DATA();
	if(NULL == pNewSession)
	{
		RLog2(LOG_LEVEL_ERROR, "Alloc memory failed.");
		return WFS_ERR_INTERNAL_ERROR;
	}

	*pNewSession = *pSession;

	LOCK_STATE eLockState = LS_UNLOCKED;
	LOCK_STATE eLSFind = LS_UNLOCKED;

	m_vectorSession.Lock();
	for(MVSIT it = m_vectorSession.begin(); it != m_vectorSession.end(); it++)
	{
		if(SAME_OWNER((*it), pNewSession))
		{
			hRes = m_mapLockState.FindSecond((*it)->hService, &eLSFind);
			if(hRes == 0 && eLSFind == LS_LOCKED)
			{
				eLockState = LS_RESERVED;
				break;
			}
		}
	}
	m_vectorSession.UnLock();

	m_vectorSession.MutexAdd(pNewSession);
	m_mapLockState.MutexUpdate(pNewSession->hService, eLockState);
	RLog2(LOG_LEVEL_DEBUG, "The Service[%d] was started success, LockState=%d", pNewSession->hService, eLockState);
	return 0;
}

HRESULT CXFSSPKernel::FaceOnClose(LPSPPACK pPack)
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");

	HRESULT hRes = WFS_SUCCESS;	
	HSERVICE hService = pPack->hService;

	hRes = m_pSPFace->OnCloseSession(hService);
	if(hRes != WFS_SUCCESS)
	{
		RLog2(LOG_LEVEL_ERROR, "OnCloseSession() return failed, hRes=%d", hRes);
	}

	EndSession(hService);
	return hRes;
}

HRESULT CXFSSPKernel::FaceOnDeregister(LPSPPACK pPack)
{
	LPREGEVENT pRegEvent = (LPREGEVENT)(pPack + 1);
	if(IsBadReadPtr(pRegEvent, sizeof(REGEVENT)))
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid pointer.");
		return WFS_ERR_INTERNAL_ERROR;
	}

	RLog2(LOG_LEVEL_DEBUG, "hRegObject=0x%X dwEventClass=%d", pRegEvent->hRegObject, pRegEvent->dwEventClass);

	if(!m_pSPManage->IsInteractiveObjectExist(pRegEvent->hRegObject))
	{
		return WFS_ERR_INVALID_HWNDREG;
	}

	BOOL bFind = FALSE;

	m_vectorRegEvt.Lock();
	for(MVREIT it = m_vectorRegEvt.begin(); it != m_vectorRegEvt.end(); it++)
	{
		if((*it)->hService == pPack->hService)
		{
			if(NULL == pRegEvent->hRegObject || (*it)->hRegObject == pRegEvent->hRegObject)
			{
				bFind = TRUE;
				if(pRegEvent->dwEventClass == 0)
				{
					(*it)->dwEventClass = 0;
				}
				else
				{
					(*it)->dwEventClass &= ~pRegEvent->dwEventClass;
				}
			}
		}
	}
	m_vectorRegEvt.UnLock();

	if(bFind)
	{
		RemoveRegEvent(pRegEvent->hService, pRegEvent->hRegObject);
		return WFS_SUCCESS;
	}
	else
	{
		return WFS_ERR_NOT_REGISTERED;
	}
}

HRESULT CXFSSPKernel::FaceOnRegister(LPSPPACK pPack)
{
	LPREGEVENT pRegEvent = (LPREGEVENT)(pPack + 1);
	if(IsBadReadPtr(pRegEvent, sizeof(REGEVENT)))
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid pointer.");
		return WFS_ERR_INTERNAL_ERROR;
	}

	if(pRegEvent->dwEventClass == 0)
	{
		return WFS_ERR_INVALID_EVENT_CLASS;
	}

	if(NULL == pRegEvent->hRegObject || !m_pSPManage->IsInteractiveObjectExist(pRegEvent->hRegObject))
	{
		return WFS_ERR_INVALID_HWNDREG;
	}

	RLog2(LOG_LEVEL_DEBUG, "hService=%d hRegObject=0x%X dwEventClass=%d", pRegEvent->hService, pRegEvent->hRegObject, pRegEvent->dwEventClass);

	m_vectorRegEvt.Lock();
	for(MVREIT it = m_vectorRegEvt.begin(); it != m_vectorRegEvt.end(); it++)
	{
		if((*it)->hService == pPack->hService && (*it)->hRegObject == pRegEvent->hRegObject)
		{
			(*it)->dwEventClass = pRegEvent->dwEventClass;
			m_vectorRegEvt.UnLock();
			return WFS_SUCCESS;
		}
	}
	m_vectorRegEvt.UnLock();

	LPREGEVENT pNewRegEvent = new REGEVENT();
	if(NULL != pNewRegEvent)
	{
		*pNewRegEvent = *pRegEvent;
		m_vectorRegEvt.MutexAdd(pNewRegEvent);
		return WFS_SUCCESS;
	}
	else
	{
		return WFS_ERR_INTERNAL_ERROR;
	}
}

HRESULT CXFSSPKernel::FaceOnExecute(LPSPPACK pPack)
{
	RLog2(LOG_LEVEL_DEBUG, "Command=%d RequestID=%d", pPack->u.dwCommandCode, pPack->RequestID);

	HRESULT hRes = WFS_SUCCESS;
	LOCK_STATE eLSFind = LS_UNLOCKED;
	hRes = m_mapLockState.FindSecond(pPack->hService, &eLSFind);
	if(hRes != WFS_SUCCESS)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid service[%d] or internal error.", pPack->hService);
		return WFS_ERR_INTERNAL_ERROR;
	}

	if(m_eSPLockState == LS_LOCKED && eLSFind == LS_UNLOCKED)
	{
		RLog2(LOG_LEVEL_ERROR, "Cann't execute request, Locked by other, hService=%d, RequestID=%d", pPack->hService, pPack->RequestID);
		return WFS_ERR_LOCKED;
	}

	hRes = m_pSPFace->OnExecute(pPack->u.dwCommandCode, pPack->lpBuffer, pPack->RequestID);
	if(hRes != WFS_SUCCESS)
	{
		RLog2(LOG_LEVEL_ERROR, "OnExecute return failed, hRes=%d", hRes);
	}

	return hRes;
}

HRESULT CXFSSPKernel::FaceOnGetInfo(LPSPPACK pPack)
{
	RLog2(LOG_LEVEL_DEBUG, "Command=%d RequestID=%d", pPack->u.dwCommandCode, pPack->RequestID);

	HRESULT hRes = WFS_SUCCESS;
	hRes = m_pSPFace->OnGetInfo(pPack->u.dwCommandCode, pPack->lpBuffer, pPack->RequestID);
	if(hRes != WFS_SUCCESS)
	{
		RLog2(LOG_LEVEL_ERROR, "OnGetInfo return failed, hRes=%d", hRes);
	}

	return hRes;
}

HRESULT CXFSSPKernel::FaceOnLock(LPSPPACK pPack)
{
	HRESULT hRes = WFS_SUCCESS;
	LOCK_STATE eLSFind = LS_UNLOCKED;
	hRes = m_mapLockState.FindSecond(pPack->hService, &eLSFind);
	if(hRes != WFS_SUCCESS)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid service[%d] or internal error.", pPack->hService);
		return WFS_ERR_INTERNAL_ERROR;
	}

	RLog2(LOG_LEVEL_DEBUG, "SPLockState=%d hService=%d LockState=%d", m_eSPLockState, pPack->hService, eLSFind);

	if(m_eSPLockState == LS_PENDING)
	{
		UpdateSessionLockState(pPack->hService, LS_LOCKED, LS_RESERVED);
		UpdateSPLockState(LS_LOCKED);
		hRes = WFS_SUCCESS;
	}
	else if(m_eSPLockState == LS_LOCKED && (eLSFind == LS_LOCKED || eLSFind == LS_RESERVED))
	{
		hRes = WFS_SUCCESS;
	}
	else
	{
		RLog2(LOG_LEVEL_ERROR, "Unexpected SP lock status at this moment.");
		hRes = WFS_ERR_INTERNAL_ERROR;
	}

	return hRes;
}

HRESULT CXFSSPKernel::FaceOnUnlock(LPSPPACK pPack)
{
	//RLog2(LOG_LEVEL_DEBUG, "Enter");
	
	return UnLockSession(pPack->hService);
}

inline HRESULT CXFSSPKernel::CancelRequestsInExecList(HSERVICE hService, REQUESTID RequestID, HRESULT hResult)
{
	RLog2(LOG_LEVEL_DEBUG, "hService=%d, RequestID=%d", hService, RequestID);

	DWORD dwCommand = 0;
	LPSPPACK pPack = NULL;
	HRESULT hRes = WFS_ERR_INVALID_REQ_ID;

	m_listExec.Lock();
	for(MLEIT it = m_listExec.begin(); it != m_listExec.end(); it++)
	{
		pPack = (*it)->pSPPack;
		if(pPack->hService == hService && (RequestID == 0 || pPack->RequestID == RequestID))
		{
			hRes = WFS_SUCCESS;
			if(hResult == WFS_ERR_CANCELED)
			{
				if(!(*it)->bCancel)
				{
					(*it)->bCancel = TRUE;
					if((pPack->dwTimeout > 0) && (pPack->uMsg == XFS_GETINFO || pPack->uMsg == XFS_EXECUTE))
					{
						InnerKillTimer(pPack->RequestID);
					}

					RLog2(LOG_LEVEL_DEBUG, "Requests cancel, hService=%d, RequestID=%d", hService, RequestID);	
				}
			}
			else if(hResult == WFS_ERR_TIMEOUT)
			{
				if(!(*it)->bTimeout)
				{
					(*it)->bTimeout = TRUE;
					RLog2(LOG_LEVEL_DEBUG, "Requests timeout, hService=%d, RequestID=%d", hService, RequestID);
				}
			}

			if(RequestID != 0)
			{
				dwCommand = pPack->u.dwCommandCode;
				break;
			}
		}
	}
	m_listExec.UnLock();

	if(hRes == WFS_SUCCESS)
	{
		HRESULT hRes2 = m_pSPFace->OnCancelRequest(dwCommand, hResult, RequestID);
		if(hRes2 != WFS_SUCCESS)
		{
			RLog2(LOG_LEVEL_DEBUG, "OnCancelRequest()=%d", hRes2);
		}
	}

	if(RequestID == 0)
	{
		return WFS_SUCCESS;
	}
	else
	{
		return hRes;
	}
}

inline HRESULT CXFSSPKernel::CancelRequestsInLockList(HSERVICE hService, REQUESTID RequestID, HRESULT hResult, BOOL bSend2SPI)
{
	RLog2(LOG_LEVEL_DEBUG, "hService=%d, RequestID=%d, hResult=%d", hService, RequestID, hResult);

	LPSPPACK pPack = NULL;
	HRESULT hRes = WFS_ERR_INVALID_REQ_ID;

	m_listLock.Lock();
	for(MLSPPIT it = m_listLock.begin(); it != m_listLock.end(); )
	{
		pPack = (*it);
		if(pPack->hService == hService && (RequestID == 0 || pPack->RequestID == RequestID))
		{
			hRes = WFS_SUCCESS;
			if((pPack->dwTimeout > 0) && hResult != WFS_ERR_TIMEOUT)
			{
				InnerKillTimer(pPack->RequestID);
			}

			if(m_eSPLockState == LS_PENDING)
			{
				UpdateSPLockState(LS_UNLOCKED);
			}

			if(bSend2SPI)
			{
				pPack->ups.hResult = hResult;
				m_listCancel.MutexPush(pPack);
			}
			else
			{
				delete (pPack);
			}

			it = m_listLock.erase(it);

			if(RequestID != 0)
			{
				break;
			}
		}
		else
		{
			it++;
		}
	}
	m_listLock.UnLock();

	if(RequestID == 0)
	{
		return WFS_SUCCESS;
	}
	else
	{
		return hRes;
	}
}

inline HRESULT CXFSSPKernel::CancelRequestsInOtherList(HSERVICE hService, REQUESTID RequestID, HRESULT hResult, BOOL bSend2SPI)
{
	RLog2(LOG_LEVEL_DEBUG, "hService=%d, RequestID=%d, hResult=%d", hService, RequestID, hResult);

	DWORD dwIndex = 0;
	LPSPPACK pPack = NULL;
	HRESULT hRes = WFS_ERR_INVALID_REQ_ID;
	CMutexList<LPSPPACK> listFind;

	DWORD dwReqSize = m_listReq.size();
	if(dwReqSize > 0)
	{
		m_listReq.Lock();
		listFind.insert(listFind.end(), m_listReq.begin(), m_listReq.end());
		m_listReq.UnLock();
	}

	DWORD dwDeferredSize = m_listDeferredReq.size();
	if(dwDeferredSize > 0)
	{
		m_listDeferredReq.Lock();
		listFind.insert(listFind.end(), m_listDeferredReq.begin(), m_listDeferredReq.end());
		m_listDeferredReq.UnLock();
	}
	
	for(MLSPPIT it = listFind.begin(); it != listFind.end(); it++, dwIndex++)
	{
		if((*it)->hService == hService && (RequestID == 0 || (*it)->RequestID == RequestID))
		{
			pPack = (*it);
			if((hResult != WFS_ERR_TIMEOUT) && (pPack->dwTimeout > 0) && 
				(pPack->uMsg == XFS_GETINFO || pPack->uMsg == XFS_EXECUTE || pPack->uMsg == XFS_LOCK || pPack->uMsg == XFS_OPEN))
			{
				InnerKillTimer(pPack->RequestID);
			}

			if(dwReqSize > 0 && dwIndex < dwReqSize)
			{
				hRes = m_listReq.MutexRemove2(pPack);
				if(hRes == WFS_SUCCESS)
				{
					if(bSend2SPI)
					{
						pPack->ups.hResult = hResult;
						pPack->lpBuffer = NULL;
						m_listCancel.MutexPush(pPack);
					}
					else
					{
						delete pPack;
					}

					if(RequestID == 0)
					{
						continue;
					}
					else
					{
						break;
					}
				}
			}

			hRes = m_listDeferredReq.MutexRemove2(pPack);
			if(hRes == WFS_SUCCESS)
			{
				if(bSend2SPI)
				{
					pPack->ups.hResult = hResult;
					pPack->lpBuffer = NULL;
					m_listCancel.MutexPush(pPack);
				}
				else
				{
					delete pPack;
				}

				if(RequestID == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}

			if(RequestID != 0)
			{
				break;
			}
		}
	}

	return hRes;
}

inline HRESULT CXFSSPKernel::CancelRequests(HSERVICE hService, REQUESTID RequestID, HRESULT hResult, BOOL bSend2SPI)
{
	HRESULT hRes = CancelRequestsInOtherList(hService, RequestID, hResult, bSend2SPI);

	if(RequestID == 0 || hRes != 0)
	{
		hRes = CancelRequestsInLockList(hService, RequestID, hResult, bSend2SPI);
	}

	if(RequestID == 0 || hRes != 0)
	{
		hRes = CancelRequestsInExecList(hService, RequestID, hResult);
	}

	if(bSend2SPI && m_listCancel.size() > 0)
	{
		SetEvent(m_hBusyEventIdle);
	}

	return hRes;
}

inline HRESULT CXFSSPKernel::CancelTimeoutRequests(LPSPPACK pPack)
{
	HSERVICE hService   = 0;
	REQUESTID RequestID = 0;

	if(IsBadReadPtr(pPack, sizeof(SPPACK)))
	{
		return WFS_SUCCESS;
	}

	try
	{
		hService  = pPack->hService;
		RequestID = pPack->RequestID;
		if(hService == 0 || RequestID == 0)
		{
			throw;
		}
	}
	catch(...)
	{
		RLog2(LOG_LEVEL_ERROR, "Catch abnormal.");
		return WFS_SUCCESS;
	}

	RLog2(LOG_LEVEL_DEBUG, "hService=%d, RequestID=%d, pPack=0x%X", hService, RequestID, pPack);
	return CancelRequests(hService, RequestID, WFS_ERR_TIMEOUT, TRUE);
}

void CXFSSPKernel::CheckSessionConnect()
{
	BOOL bFind = FALSE;
	TCHAR szLogicalName[MAX_SP_NAME] = {0};
	TCHAR szAppID[MAX_SP_NAME] = {0};

	m_vectorSession.Lock();
	for(MVSIT it = m_vectorSession.begin(); it != m_vectorSession.end(); )
	{
		if(!m_pSPManage->IsInteractiveObjectExist((*it)->hSPIObject))
		{
			RLog2(LOG_LEVEL_WARN, "Caller had exited(LogicalName=%s, hService=%d)", (*it)->szLogicalName, (*it)->hService);
			TCSCPY_S(szLogicalName, MAX_SP_NAME, (*it)->szLogicalName);
			TCSCPY_S(szAppID, MAX_SP_NAME, (*it)->szAppID);

			EndSession((*it)->hService);
			delete (*it);
			it = m_vectorSession.erase(it);

			FireDisconnect(szLogicalName, szAppID);
			bFind = TRUE;
		}
		else
		{
			it++;
		}
	}
	m_vectorSession.UnLock();

	if(bFind && m_vectorSession.size() == 0)
	{
		ExitSP();
	}
}

void CXFSSPKernel::EndSession(HSERVICE hService)
{
	//RLog2(LOG_LEVEL_DEBUG, "Enter");
	CancelRequests(hService, 0);
	UnLockSession(hService);
	RemoveRegEvent(hService);
}

DWORD CXFSSPKernel::OPExecContext(REQUESTID RequestID, EXECONTEXT_OP eOP, LPVOID p)
{
	DWORD dwRet = 0;
	if(m_listExec.size() > 0)
	{
		m_listExec.Lock();
		for(MLECIT it = m_listExec.begin(); it != m_listExec.end(); it++)
		{
			if((*it)->pSPPack->RequestID == RequestID || (RequestID == 0 && (m_listExec.size() == 1 || (*it)->pSPPack->uMsg == XFS_EXECUTE)))
			{
				switch(eOP)
				{
					case OP_SET_BUFFER:
					{
						(*it)->pSPPack->lpBuffer = p;
						dwRet = TRUE;
						break;
					}
					case OP_GET_CANCEL:
					{
						dwRet = (DWORD)(*it)->bCancel;
						break;
					}
					case OP_GET_TIMEOUT:
					{
						dwRet = (DWORD)(*it)->bTimeout;
						break;
					}
					case OP_GET_TOTALTIME:
					{
						dwRet = (*it)->pSPPack->dwTimeout;
						break;
					}
					case OP_GET_REQUESTID:
					{
						dwRet = (*it)->pSPPack->RequestID;
						break;
					}
					case OP_GET_SESSION:
					{
						dwRet = (*it)->pSPPack->hService;
						break;
					}
				}

				break;
			}
		}
		m_listExec.UnLock();
	}

	return dwRet;
}

inline void CXFSSPKernel::RemoveRegEvent(HSERVICE hService, LPVOID hRegObject)
{
	m_vectorRegEvt.Lock();
	for(MVREIT it = m_vectorRegEvt.begin(); it != m_vectorRegEvt.end(); )
	{
		if((*it)->dwEventClass == 0 || (NULL == hRegObject && (hService == 0 || (*it)->hService == hService)) 
			|| ((*it)->hRegObject == hRegObject && (*it)->hService == hService))
		{
			delete (*it);
			it = m_vectorRegEvt.erase(it);
		}
		else
		{
			it++;
		}
	}
	m_vectorRegEvt.UnLock();
}

inline void CXFSSPKernel::RemoveSession(HSERVICE hService)
{
	if(m_vectorSession.size() == 0)
	{
		return;
	}

	m_vectorSession.Lock();
	for(MVSIT it = m_vectorSession.begin(); it != m_vectorSession.end(); )
	{
		if(hService == 0 || (*it)->hService == hService)
		{
			if(hService == 0)
			{
				EndSession((*it)->hService);
			}

			m_mapLockState.MutexRemove((*it)->hService);

			delete (*it);
			it = m_vectorSession.erase(it);
		}
		else
		{
			it++;
		}
	}
	m_vectorSession.UnLock();
}

inline void CXFSSPKernel::RemoveAllTimer()
{
	if(m_mapTimer.size() == 0)
	{
		return;
	}

	for(MMTIIT it = m_mapTimer.begin(); it != m_mapTimer.end(); it++)
	{
		FaceKillTimer(it->first);
	}
}

inline void CXFSSPKernel::ExitSP()
{
	RLog2(LOG_LEVEL_DATA, "All session was closed or OS is closing, SP is exiting now");
	::PostMessage((HWND)m_hSPObject, WM_QUIT, FLAG_EXITSP, 0);
}

inline BOOL CXFSSPKernel::IsMyLocker(HSERVICE hService) const
{
	//RLog2(LOG_LEVEL_DEBUG, "SPLockState=%d hService=%d", m_eSPLockState, hService);

	HRESULT hRes = WFS_SUCCESS;
	LOCK_STATE eLSFind = LS_UNLOCKED;
	hRes = m_mapLockState.FindSecond(hService, &eLSFind);
	if(hRes == WFS_SUCCESS && (eLSFind == LS_LOCKED || eLSFind == LS_RESERVED))
	{
		return TRUE;
	}

	return FALSE;
}

inline BOOL CXFSSPKernel::IsParallerExecCmd(DWORD dwCommand)
{
	BOOL bRet = FALSE;
	DWORD dwCurCmd = 0xFFFF;
	if(m_mapParallerExecCmd.size() == 0 || m_listExec.size() == 0)
	{
		return FALSE;
	}

	m_listExec.Lock();
	for(MLECIT it = m_listExec.begin(); it != m_listExec.end(); it++)
	{
		if((*it)->pSPPack->uMsg == XFS_EXECUTE)
		{
			dwCurCmd = (*it)->pSPPack->u.dwCommandCode;
			break;
		}
	}
	m_listExec.UnLock();

	if(dwCurCmd == 0xFFFF || dwCurCmd == dwCommand)
	{
		return FALSE;
	}

	m_mapParallerExecCmd.Lock();
	for(CMutexMultiMap<DWORD, DWORD>::const_iterator it = m_mapParallerExecCmd.begin(); it != m_mapParallerExecCmd.end(); it++)
	{
		if((*it).first == dwCommand && ((*it).second == 0 || (*it).second == dwCurCmd))
		{
			bRet = TRUE;
			break;
		}
	}
	m_mapParallerExecCmd.UnLock();

	return bRet;
}

inline BOOL CXFSSPKernel::IsDeferredInfoCmd(DWORD dwCategory)
{
	BOOL bRet = FALSE;
	if(m_vectorDeferredInfoCmd.size() == 0)
	{
		return FALSE;
	}

	m_vectorDeferredInfoCmd.Lock();
	for(CMutexVector<DWORD>::const_iterator it = m_vectorDeferredInfoCmd.begin(); it != m_vectorDeferredInfoCmd.end(); it++)
	{
		if((*it) == dwCategory)
		{
			bRet = TRUE;
			break;
		}
	}
	m_vectorDeferredInfoCmd.UnLock();

	return bRet;
}

inline LPSESSION_DATA CXFSSPKernel::FindSession(HSERVICE hService) const
{
	LPSESSION_DATA pSession = NULL;

	const_cast<CXFSSPKernel*>(this)->m_vectorSession.Lock();
	for(MVSCIT it = m_vectorSession.begin(); it != m_vectorSession.end(); it++)
	{
		if((*it)->hService == hService)
		{
			pSession = (*it);
			break;
		}
	}
	const_cast<CXFSSPKernel*>(this)->m_vectorSession.UnLock();

	return pSession;
}

inline DWORD CXFSSPKernel::Msg2EventClass(DWORD dwMsg) const
{
	switch(dwMsg)
	{
		case XFS_EXECUTE_EVENT: 
			return EXECUTE_EVENTS;
		case XFS_SERVICE_EVENT: 
			return SERVICE_EVENTS;		
		case XFS_USER_EVENT: 
			return USER_EVENTS;
		case XFS_SYSTEM_EVENT: 
			return SYSTEM_EVENTS;
		default:
			return 0;
	}
}

inline HRESULT CXFSSPKernel::UnLockSession(HSERVICE hService)
{
	HRESULT hRes = WFS_SUCCESS;
	LOCK_STATE eLSFind = LS_UNLOCKED;
	hRes = m_mapLockState.FindSecond(hService, &eLSFind);
	if(hRes != WFS_SUCCESS)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid service[%d] or internal error.", hService);
		return WFS_ERR_INTERNAL_ERROR;
	}

	RLog2(LOG_LEVEL_DEBUG, "SPLockState=%d hService=%d LockState=%d", m_eSPLockState, hService, eLSFind);

	if(m_eSPLockState != LS_LOCKED || eLSFind == LS_UNLOCKED)
	{
		RLog2(LOG_LEVEL_DEBUG, "Session[%d] wasn't locked, SPLockState=%d", hService, m_eSPLockState);
		return WFS_ERR_NOT_LOCKED;
	}

	if(eLSFind == LS_LOCKED)
	{
		UpdateSessionLockState(hService, LS_UNLOCKED, LS_UNLOCKED);
		if(m_listLock.size() > 0)
		{
			UpdateSPLockState(LS_PENDING);
		}
		else
		{
			UpdateSPLockState(LS_UNLOCKED);
		}
	}
	
	return WFS_SUCCESS;
}

inline HRESULT CXFSSPKernel::UpdateSPLockState(LOCK_STATE eLockState)
{
	CAutoLock autolock(m_hMutexSPLockState);
	m_eSPLockState = eLockState;
	return 0;
}

inline HRESULT CXFSSPKernel::UpdateSessionLockState(HSERVICE hService, LOCK_STATE eLockState, LOCK_STATE eOtherSession)
{
	LPSESSION_DATA pSession = FindSession(hService);
	if(NULL == pSession)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid hService[%d]", hService);
		return WFS_ERR_INVALID_HSERVICE;
	}

	m_mapLockState.MutexUpdate(hService, eLockState);

	m_vectorSession.Lock();
	for(MVSIT it = m_vectorSession.begin(); it != m_vectorSession.end(); it++)
	{
		if((*it)->hService != hService && SAME_OWNER((*it), pSession))
		{
			m_mapLockState.MutexUpdate((*it)->hService, eOtherSession);
			RLog2(LOG_LEVEL_DEBUG, "hService=%d LockState=%d", (*it)->hService, eOtherSession);
		}
	}
	m_vectorSession.UnLock();

	return 0;
}

HRESULT CXFSSPKernel::DealPack(LPCTSTR lpData, UINT nDataLen)
{
	if(nDataLen < sizeof(SPPACK) || IsBadReadPtr(lpData, nDataLen))
	{
		RLog2(LOG_LEVEL_ERROR, "Receive bad data, nDataLen=%d, data=%s", nDataLen, lpData);
		return WFS_ERR_INTERNAL_ERROR;
	}

	RHex2(LOG_LEVEL_DEBUG, lpData, nDataLen);

	HRESULT hRes = 0;
	char *pPackAll = new char[nDataLen + 1];
	if(NULL == pPackAll)
	{
		RLog2(LOG_LEVEL_ERROR, "Alloc memory failed.");
		return WFS_ERR_INTERNAL_ERROR;
	}

	memset(pPackAll, 0, nDataLen + 1);
	memcpy(pPackAll, lpData, nDataLen);
	LPSPPACK pPack = (LPSPPACK)pPackAll;

	switch(pPack->uMsg)
	{
		case XFS_CANCEL:
		case XFS_LOGLEVEL:
		case XFS_VERSIONERROR:
		{
			hRes = DoImmediatelyRequest(pPack);
			break;
		}
		case XFS_CLOSE:
		case XFS_UNLOCK:
		case XFS_REGISTER:
		case XFS_DEREGISTER:
		{
			m_listReq.MutexPush(pPack);
			SetEvent(m_hBusyEventDisp);
			break;
		}
		case XFS_OPEN:
		case XFS_LOCK:
		case XFS_GETINFO:
		case XFS_EXECUTE:
		{
			m_listReq.MutexPush(pPack);
			if(pPack->dwTimeout > 0)
			{
				InnerSetTimer(pPack->RequestID, pPack->dwTimeout, pPack);
			}
			SetEvent(m_hBusyEventDisp);
			break;
		}
		default:
		{
			RLog2(LOG_LEVEL_ERROR, "Invalid packet.");
			hRes = WFS_ERR_INTERNAL_ERROR;
			break;
		}
	}

	/*
	SPPACK stAckPack;
	stAckPack = *pPack;
	stAckPack.uMsg = XFS_ACK;
	stAckPack.lpBuffer = NULL;
	stAckPack.ups.hResult = hRes;
	RHex2(LOG_LEVEL_DEBUG, (LPCTSTR)&stAckPack, sizeof(SPPACK));
	m_CSPPack.SendData(stAckPack.hService, (BYTE *)&stAckPack, sizeof(SPPACK));*/

	DLog2(LOG_LEVEL_DEBUG, "End: hRes=%d", hRes);
	return hRes;
}

HRESULT CXFSSPKernel::Face2SPI(LPSPPACK pPack, LPSESSION_DATA pSession)
{
	HRESULT hRes = 0;
	UINT nDataLen = sizeof(SPPACK);
	if(IsBadReadPtr(pPack, nDataLen))
	{
		RLog2(LOG_LEVEL_ERROR, "Bad Read pointer.");
		return WFS_ERR_INTERNAL_ERROR;
	}

	if(NULL == pSession)
	{
		pSession = FindSession(pPack->hService);
		if(NULL == pSession)
		{
			RLog2(LOG_LEVEL_ERROR, "Invalid hService[%d], Msg=0x%X.", pPack->hService, pPack->uMsg);
			return WFS_ERR_INVALID_HSERVICE;
		}
	}

	RLog2(LOG_LEVEL_DEBUG, "uMsg=0x%X hService=%d lpBuffer=0x%X", pPack->uMsg, pPack->hService, pPack->lpBuffer);

	CMD_FLAG eCmdType = CMDF_NONE;
	if(NULL != pPack->lpBuffer)
	{
		if(pPack->uMsg == XFS_GETINFO)
		{
			eCmdType = CMDF_GC;
		}
		else if(pPack->uMsg == XFS_EXECUTE)
		{
			eCmdType = CMDF_EC;
		}
		else if(pPack->uMsg == XFS_EXECUTE_EVENT || pPack->uMsg == XFS_SERVICE_EVENT || pPack->uMsg == XFS_USER_EVENT || pPack->uMsg == XFS_SYSTEM_EVENT)
		{
			eCmdType = CMDF_EV;
		}
	}	

	if(!m_pSPManage->IsInteractiveObjectExist(pSession->hSPIObject))
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid object[0x%X], Msg=0x%X.", pSession->hSPIObject, pPack->uMsg);
		return WFS_ERR_CONNECTION_LOST;
	}

	if(pPack->tsTimestamp.wYear == 0)
	{
		SYSTEMTIME stTime;
		::GetLocalTime(&stTime);
		pPack->tsTimestamp = stTime;
	}

	hRes = m_pSPManage->SendData(pPack, pSession->hSPIObject, pSession->dwSrvcVerRequired, eCmdType);
	if(hRes != 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Send to SPI failed, hRes=%d, hService=%d, uMsg=0x%X dwCommandCode=%d.", hRes, pPack->hService, pPack->uMsg, pPack->u.dwCommandCode);
	}

	if(pPack->uMsg == XFS_CLOSE)
	{
		RemoveSession(pPack->hService);
		if(m_vectorSession.size() == 0)
		{
			ExitSP();
		}
	}
	return 0;
}

HRESULT CXFSSPKernel::DoAtUnLock()
{
	if(m_listReq.size() == 0)
	{
		return 0;
	}

	HRESULT hRes = 0;
	LPSPPACK pPack = m_listReq.MutexPop();
	if(NULL == pPack)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	if(pPack->uMsg == XFS_GETINFO)
	{
		if(IsDeferredInfoCmd(pPack->u.dwCommandCode))
		{
			m_listDeferredReq.MutexPush(pPack);
		}
		else
		{
			hRes = ExecListProc(pPack);
		}
	}
	else if(pPack->uMsg == XFS_EXECUTE)
	{
		if(IsParallerExecCmd(pPack->u.dwCommandCode))
		{
			hRes = ExecListProc(pPack);
		}
		else
		{
			m_listDeferredReq.MutexPush(pPack);
		}
	} 
	else if(pPack->uMsg == XFS_LOCK)
	{
		m_listLock.MutexPush(pPack);
		UpdateSPLockState(LS_PENDING);
		RLog2(LOG_LEVEL_DEBUG, "XFS_LOCK come, enter pending...");
	}
	else
	{
		RLog2(LOG_LEVEL_DEBUG, "Other Async Request, Msg=0x%X", pPack->uMsg);
		hRes = DoAsyncRequest(pPack);
	}

	return hRes;
}

HRESULT CXFSSPKernel::DoAtLocked()
{
	if(m_listReq.size() == 0)
	{
		return 0;
	}

	HRESULT hRes = 0;
	LPSPPACK pPack = m_listReq.MutexPop();
	if(NULL == pPack)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}

	BOOL bMyLocker = IsMyLocker(pPack->hService);
	if(pPack->uMsg == XFS_GETINFO)
	{
		if(IsDeferredInfoCmd(pPack->u.dwCommandCode))
		{
			m_listDeferredReq.MutexPush(pPack);
		}
		else
		{
			hRes = ExecListProc(pPack);
		}
	}
	else if(bMyLocker && (pPack->uMsg == XFS_EXECUTE))
	{
		if(IsParallerExecCmd(pPack->u.dwCommandCode))
		{
			hRes = ExecListProc(pPack);
		}
		else
		{
			m_listDeferredReq.MutexPush(pPack);
		}
	}
	else if(bMyLocker && (pPack->uMsg == XFS_UNLOCK || pPack->uMsg == XFS_CLOSE))
	{
		m_listDeferredReq.MutexPush(pPack);
	}	
	else if(!bMyLocker && (pPack->uMsg == XFS_LOCK))
	{
		m_listLock.MutexPush(pPack);
		FireLockReqested();
		RLog2(LOG_LEVEL_DEBUG, "Other service request Lock, RequestID=%d", pPack->RequestID);
	}
	else
	{
		hRes = DoAsyncRequest(pPack);
	}

	return hRes;
}

HRESULT CXFSSPKernel::DoAtPending()
{
	HRESULT hRes = 0;
	LPSPPACK pPack = NULL;

	if(m_listDeferredReq.size() == 0)
	{
		pPack = m_listLock.MutexPop();
		if(NULL == pPack)
		{
			RLog2(LOG_LEVEL_ERROR, "SP's lock state or queue is invalid.");
			UpdateSPLockState(LS_UNLOCKED);
			return WFS_ERR_INTERNAL_ERROR;
		}

		HSERVICE hService = pPack->hService;
		hRes = DoAsyncRequest(pPack);
		if(hRes == 0)
		{
			if(m_listLock.size() > 0)
			{
				CancelRequestsInLockList(hService, 0, WFS_SUCCESS, TRUE);
			}

			if(m_listLock.size() > 0)
			{
				LPSESSION_DATA pSession = FindSession(hService);
				if(NULL == pSession)
				{
					RLog2(LOG_LEVEL_ERROR, "Invalid hService[%d]", hService);
					return WFS_ERR_INVALID_HSERVICE;
				}

				m_listLock.Lock();
				for(MLSPPIT it = m_listLock.begin(); it != m_listLock.end(); it++)
				{
					LPSESSION_DATA pSessionOther = FindSession((*it)->hService);
					if(SAME_OWNER(pSessionOther, pSession))
					{
						CancelRequestsInLockList((*it)->hService, 0, WFS_SUCCESS, TRUE);
					}
				}
				m_listLock.UnLock();
			}

			if(m_listCancel.size() > 0)
			{
				SetEvent(m_hBusyEventIdle);
			}
		}
		else
		{
			RLog2(LOG_LEVEL_ERROR, "Lock service failed, hRes=%d", hRes);
			if(m_listLock.size() == 0)
			{
				UpdateSPLockState(LS_UNLOCKED);
			}
		}
	}
	else
	{
		if(m_listReq.size() == 0)
		{
			return 0;
		}

		BOOL bLock = TRUE;
		m_listReq.Lock();
		for(MLSPPIT it = m_listReq.begin(); it != m_listReq.end(); )
		{
			pPack = *it;
			if(pPack->uMsg == XFS_GETINFO)
			{
				if(IsDeferredInfoCmd(pPack->u.dwCommandCode))
				{
					it++;
				}
				else
				{
					it = m_listReq.erase(it);

					bLock = FALSE;
					m_listReq.UnLock();
					hRes = ExecListProc(pPack);
					break;
				}
			}
			else if(pPack->uMsg == XFS_OPEN || pPack->uMsg == XFS_REGISTER || pPack->uMsg == XFS_DEREGISTER)
			{
				it = m_listReq.erase(it);

				bLock = FALSE;
				m_listReq.UnLock();
				hRes = DoAsyncRequest(pPack);
				break;
			}
			else if(pPack->uMsg == XFS_LOCK)
			{
				it = m_listReq.erase(it);
				m_listLock.MutexPush(pPack);
				break;
			}
			else
			{
				it++;
			}
		}

		if(bLock)
		{
			m_listReq.UnLock();
		}
	}

	return hRes;
}

