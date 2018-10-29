/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSSPKernel.h
* Description: kernel class for all SP
*
* Author:      liuguoxun
* Date:        2015-10-13
*
*****************************************************************************/


#ifndef  _CXFSSPKERNEL_H_20151013
#define  _CXFSSPKERNEL_H_20151013

#include <IXFSSPKernel.h>
#include <CSTLMutexUnit.h>
#include <StructKernel.h>
#include <IPlatform.h>


#undef  LOG_NAME_LOG
#define LOG_NAME_LOG      LOG_XFSSPKERNEL


#define  BASE_TIMER_ID    0xFFFF0000UL


typedef enum 
{
	  LS_UNLOCKED    = 0
	, LS_LOCKED      = 1
	, LS_PENDING     = 2
	, LS_RESERVED    = 4  //for compound devices
}LOCK_STATE;

typedef enum 
{
	  TF_RECV        = 1L //Recvice
	, TF_EXEC        = 2L //Execute
	, TF_DISP        = 3L //Dispatch
	, TF_IDLE        = 4L //Idle
}THREAD_FLAG;

typedef enum 
{
	  OP_SET_BUFFER    = 1L
	, OP_GET_CANCEL    = 3L
	, OP_GET_TIMEOUT   = 4L
	, OP_GET_TOTALTIME = 5L
	, OP_GET_REQUESTID = 6L
	, OP_GET_SESSION   = 7L
}EXECONTEXT_OP;

/********************************************************************************/

#pragma pack(push, 1)

typedef struct _sp_exec_context
{
	BOOL            bCancel;        //是否取消
	BOOL            bTimeout;       //是否超时
	LPSPPACK		pSPPack;		//请求信息指针
}EXECONTEXT, *LPEXECONTEXT;

#pragma pack(pop)

/********************************************************************************/


class CXFSSPKernel : public IXFSSPKernel
{
public:
	virtual HRESULT   Run(IXFSSPFace *pSPFace, IXFSSPManage *pSPManage, LPCTSTR lpServiceName);


public:
	virtual REQUESTID GetRequestID() const;
	virtual LPCTSTR   GetLogicalName(REQUESTID RequestID = 0) const;
	virtual HSERVICE  GetSessionID(REQUESTID RequestID = 0) const;
	virtual DWORD     GetTotalTimeout(REQUESTID RequestID = 0) const;
	virtual BOOL      IsRequestTimeout(REQUESTID RequestID = 0) const;
	virtual BOOL      IsRequestCancelled(REQUESTID RequestID = 0) const;
	virtual DWORD     GetXFSVersion(REQUESTID RequestID = 0) const;
	virtual BOOL      SetResult(LPVOID pBuffer, REQUESTID RequestID = 0);

	virtual HRESULT RegisterParallerExecCmd(DWORD dwPCmd, DWORD dwCurCmd = 0);
	virtual HRESULT RegisterDeferredInfoCmd(DWORD dwCategory);
	virtual HRESULT SetTimer(UINT nTimerID, UINT nElapse, LPVOID lpContext = NULL);
	virtual HRESULT KillTimer(UINT nTimerID);
	virtual HRESULT SIUGetInfo(DWORD dwCategory, LPVOID lpQueryDetail);
	virtual HRESULT SIUControl(DWORD dwType, DWORD dwIndex, DWORD dwCommand);
	virtual HRESULT WFMAlloc2Buffer(DWORD dwSizeFirst, DWORD dwSizeSecond, DWORD dwFlag, LPVOID *ppFirst, LPVOID *ppSecond);

	virtual HRESULT FireExecEvent(DWORD dwEventID, HRESULT hResult = WFS_SUCCESS, LPVOID pData = NULL, REQUESTID RequestID = 0, SYSTEMTIME *pTimestamp = NULL);
	virtual HRESULT FireServEvent(DWORD dwEventID, HRESULT hResult = WFS_SUCCESS, LPVOID pData = NULL, REQUESTID RequestID = 0, SYSTEMTIME *pTimestamp = NULL);
	virtual HRESULT FireUserEvent(DWORD dwEventID, HRESULT hResult = WFS_SUCCESS, LPVOID pData = NULL, REQUESTID RequestID = 0, SYSTEMTIME *pTimestamp = NULL);
	virtual HRESULT FireSystEvent(DWORD dwEventID, HRESULT hResult = WFS_SUCCESS, LPVOID pData = NULL, REQUESTID RequestID = 0, SYSTEMTIME *pTimestamp = NULL);

	virtual HRESULT FireDevStatus(DWORD dwState);
	virtual HRESULT FireSystError(DWORD dwEventID, DWORD dwAction, HRESULT hResult = 0, REQUESTID RequestID = 0, DWORD dwSize = 0, LPBYTE lpDesc = NULL);


public:
	explicit CXFSSPKernel();
	virtual  ~CXFSSPKernel();


protected:
	HRESULT FireEvent(DWORD dwEventType, DWORD dwEventID, HRESULT hResult = WFS_SUCCESS, LPVOID pData = NULL, REQUESTID RequestID = 0, SYSTEMTIME *pTimestamp = NULL);
	HRESULT FireUndeliverable(LPTSTR lpLogicalName, LPTSTR lpAppID, DWORD dwSize, LPBYTE lpDesc, DWORD dwMsg, LPWFSRESULT lpWFSResult);
	HRESULT FireDisconnect(LPTSTR lpLogicalName, LPTSTR lpAppID);
	HRESULT FireLockReqested();
	HRESULT FireVerError(DWORD dwSupportVer, HRESULT hResult, LPTSTR lpLogicalName, LPTSTR lpAppID, DWORD dwSize, LPBYTE lpDesc);

	static  DWORD   CALLBACK SubThreadEntry(LPVOID pParam);
	static  LRESULT CALLBACK FaceRecvProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	
	UINT    DispatchThreadProc();
	UINT    ExecThreadProc();
	UINT    IdleThreadProc();
	HRESULT ExecListProc(LPSPPACK pPack);
	HRESULT CancelListProc();
	HRESULT DeferTimerProc();
	HANDLE  CreateEventThread(DWORD dwFlag);
	HWND    CreateClassWnd(LPCTSTR lpClassName, LPCTSTR lpWindowName, HINSTANCE hInstance, WNDPROC pProc);
	HRESULT DoAsyncRequest(LPSPPACK pPack);
	HRESULT DoImmediatelyRequest(LPSPPACK pPack);
	HRESULT CancelRequestsInExecList(HSERVICE hService, REQUESTID RequestID, HRESULT hResult = WFS_ERR_CANCELED);
	HRESULT CancelRequestsInLockList(HSERVICE hService, REQUESTID RequestID, HRESULT hResult = WFS_ERR_CANCELED, BOOL bSend2SPI = FALSE);
	HRESULT CancelRequestsInOtherList(HSERVICE hService, REQUESTID RequestID, HRESULT hResult = WFS_ERR_CANCELED, BOOL bSend2SPI = FALSE);
	HRESULT CancelRequests(HSERVICE hService, REQUESTID RequestID, HRESULT hResult = WFS_ERR_CANCELED, BOOL bSend2SPI = FALSE);
	HRESULT CancelTimeoutRequests(LPSPPACK pPack);
	void    CheckSessionConnect();
	void    EndSession(HSERVICE hService);
	DWORD   OPExecContext(REQUESTID RequestID, EXECONTEXT_OP eOP, LPVOID p = NULL);
	void    RemoveRegEvent(HSERVICE hService = 0, LPVOID hRegObject = NULL);
	void    RemoveSession(HSERVICE hService = 0);
	void    RemoveAllTimer();
	void    ExitSP();
	BOOL    IsMyLocker(HSERVICE hService) const;
	BOOL    IsParallerExecCmd(DWORD dwCommand);
	BOOL    IsDeferredInfoCmd(DWORD dwCategory);

	LPSESSION_DATA FindSession(HSERVICE hService) const;
	DWORD   Msg2EventClass(DWORD dwMsg) const;
	HRESULT InnerSetTimer(UINT nTimerID, UINT nElapse, LPVOID lpContext = NULL);
	HRESULT InnerKillTimer(UINT nTimerID);
	HRESULT FaceSetTimer(UINT nTimerID, UINT nElapse, LPVOID lpContext = NULL);
	HRESULT FaceKillTimer(UINT nTimerID);
	HRESULT UnLockSession(HSERVICE hService);
	HRESULT UpdateSPLockState(LOCK_STATE eLockState);
	HRESULT UpdateSessionLockState(HSERVICE hService, LOCK_STATE eLockState, LOCK_STATE eOtherSession);

	HRESULT FaceOnTimer(UINT nTimerID, LPVOID lpV = NULL);
	HRESULT FaceOnOpen(LPSPPACK pPack, LPSESSION_DATA pSession);
	HRESULT FaceOnClose(LPSPPACK pPack);
	HRESULT FaceOnDeregister(LPSPPACK pPack);
	HRESULT FaceOnRegister(LPSPPACK pPack);
	HRESULT FaceOnExecute(LPSPPACK pPack);
	HRESULT FaceOnGetInfo(LPSPPACK pPack);
	HRESULT FaceOnLock(LPSPPACK pPack);
	HRESULT FaceOnUnlock(LPSPPACK pPack);

	HRESULT DealPack(LPCTSTR lpData, UINT nDataLen);
	HRESULT Face2SPI(LPSPPACK pPack, LPSESSION_DATA pSession = NULL);

	HRESULT DoAtUnLock();
	HRESULT DoAtLocked();
	HRESULT DoAtPending();


protected:
	CMutexMap<HSERVICE, LOCK_STATE> m_mapLockState;         //锁状态映射表
	CMutexMap<UINT, LPVOID>         m_mapTimer;             //定时器映射表
	CMutexMap<UINT, LPVOID>         m_mapTimerDefer;        //定时器延迟执行表
	CMutexMultiMap<DWORD, DWORD>    m_mapParallerExecCmd;   //并行执行命令列表
	CMutexVector<DWORD>             m_vectorDeferredInfoCmd;//延迟信息命令列表
	CMutexVector<LPREGEVENT>        m_vectorRegEvt;         //事件注册列表
	CMutexVector<LPSESSION_DATA>    m_vectorSession;        //会话列表
	CMutexList<LPEXECONTEXT>        m_listExec;             //执行队列
	CMutexList<LPSPPACK>            m_listDeferredReq;      //延迟队列
	CMutexList<LPSPPACK>            m_listLock;             //锁定队列
	CMutexList<LPSPPACK>            m_listReq;              //请求队列
	CMutexList<LPSPPACK>            m_listCancel;           //取消队列

	typedef CMutexMap<UINT, LPVOID>::iterator        MMTIIT;

	typedef CMutexVector<LPREGEVENT>::iterator       MVREIT;
	typedef CMutexVector<LPREGEVENT>::const_iterator MVRECIT;

	typedef CMutexVector<LPSESSION_DATA>::iterator       MVSIT;
	typedef CMutexVector<LPSESSION_DATA>::const_iterator MVSCIT;

	typedef CMutexList<LPEXECONTEXT>::iterator       MLEIT;
	typedef CMutexList<LPEXECONTEXT>::const_iterator MLECIT;

	typedef CMutexList<LPSPPACK>::iterator       MLSPPIT;
	typedef CMutexList<LPSPPACK>::const_iterator MLSPPCIT;


private:
	LPVOID          m_hSPObject;
	HANDLE          m_hSPMutex;
	HANDLE          m_hMutexSPLockState;
	HANDLE          m_hExitEvent;
	HANDLE          m_hExitEventDisp;
	HANDLE          m_hExitEventExec;
	HANDLE          m_hExitEventIdle;
	HANDLE          m_hBusyEventDisp;
	HANDLE          m_hBusyEventExec;
	HANDLE          m_hBusyEventIdle;
	LOCK_STATE      m_eSPLockState;
    IXFSSPManage   *m_pSPManage;
	IXFSSPFace     *m_pSPFace;
	int             m_nKernelIdleElapse;
	TCHAR           m_szServiceName[MAX_SP_NAME + 1];
	TCHAR           m_szWorkStationName[MAX_PATH];
};


#endif  //_CXFSSPKERNEL_H_20151013

