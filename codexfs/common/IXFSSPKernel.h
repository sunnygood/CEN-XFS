/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IXFSSPKernel.h
* Description: the kernel interface       
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/


#ifndef  _IXFSSPKERNEL_H_20151109
#define  _IXFSSPKERNEL_H_20151109


#include "IXFSSPFace.h"
#include "IXFSSPManage.h"


//[0x00000000, 0x0000FF00) Deferred timer, used for communicate with hardware
//[0x0000FF00, 0x0000FFFF] Immediately timer
//(0x0000FFFF, 0xFFFFFFFF] Cann't use, used by kernel module
#define  IMMEDIATELY_TIMER_BASE     0x0000FF00


INTERFACE_I IXFSSPKernel
{
public:
	virtual HRESULT Run(IXFSSPFace *pIXFSSPFace, IXFSSPManage *pIXFSSPManage, LPCTSTR lpServiceName) = 0;


public:
	virtual REQUESTID GetRequestID() const = 0;
	virtual LPCTSTR   GetLogicalName(REQUESTID RequestID = 0) const = 0;
	virtual HSERVICE  GetSessionID(REQUESTID RequestID = 0) const = 0;	
	virtual DWORD     GetTotalTimeout(REQUESTID RequestID = 0) const = 0;
	virtual BOOL      IsRequestTimeout(REQUESTID RequestID = 0) const = 0;
	virtual BOOL      IsRequestCancelled(REQUESTID RequestID = 0) const = 0;
	virtual DWORD     GetXFSVersion(REQUESTID RequestID = 0) const = 0;
	virtual BOOL      SetResult(LPVOID pBuffer, REQUESTID RequestID = 0) = 0;

	virtual HRESULT RegisterParallerExecCmd(DWORD dwPCmd, DWORD dwCurCmd = 0) = 0;
	virtual HRESULT RegisterDeferredInfoCmd(DWORD dwCategory) = 0;
	virtual HRESULT SetTimer(UINT nTimerID, UINT nElapse, LPVOID lpContext = NULL) = 0;
	virtual HRESULT KillTimer(UINT nTimerID) = 0;
	virtual HRESULT SIUGetInfo(DWORD dwCategory, LPVOID lpQueryDetail) = 0;
	virtual HRESULT SIUControl(DWORD dwType, DWORD dwIndex, DWORD dwCommand) = 0;
	virtual HRESULT WFMAlloc2Buffer(DWORD dwSizeFirst, DWORD dwSizeSecond, DWORD dwFlag, LPVOID *ppFirst, LPVOID *ppSecond) = 0;

	virtual HRESULT FireExecEvent(DWORD dwEventID, HRESULT hResult = WFS_SUCCESS, LPVOID pData = NULL, REQUESTID RequestID = 0, SYSTEMTIME *pTimestamp = NULL) = 0;
	virtual HRESULT FireServEvent(DWORD dwEventID, HRESULT hResult = WFS_SUCCESS, LPVOID pData = NULL, REQUESTID RequestID = 0, SYSTEMTIME *pTimestamp = NULL) = 0;
	virtual HRESULT FireUserEvent(DWORD dwEventID, HRESULT hResult = WFS_SUCCESS, LPVOID pData = NULL, REQUESTID RequestID = 0, SYSTEMTIME *pTimestamp = NULL) = 0;
	virtual HRESULT FireSystEvent(DWORD dwEventID, HRESULT hResult = WFS_SUCCESS, LPVOID pData = NULL, REQUESTID RequestID = 0, SYSTEMTIME *pTimestamp = NULL) = 0;

	virtual HRESULT FireDevStatus(DWORD dwState) = 0;
	virtual HRESULT FireSystError(DWORD dwEventID, DWORD dwAction, HRESULT hResult = 0, REQUESTID RequestID = 0, DWORD dwSize = 0, LPBYTE lpDesc = NULL) = 0;
};


#endif  //_IXFSSPKERNEL_H_20151109

