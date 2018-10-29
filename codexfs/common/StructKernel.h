/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   StructKernel.h
* Description: struct define for kernel       
*
* Author:      liuguoxun
* Date:        2015-10-24
*
*****************************************************************************/

#ifndef  _STRUCTKERNEL_H_20151024
#define  _STRUCTKERNEL_H_20151024

#include <IXFSSPManage.h>


#define XFSSPWNDCLASS           _T("XFSSPWNDCLASS_%s")   //XFSSPWNDCLASS_ServiceName(SP) XFSSPWNDCLASS_ProcessID(SPI)
#define XFSSPWNDNAME            _T("XFSSPWNDNAME_%s")
#define XFSSPPROCESSMUTEX       _T("XFSSPPROCESSMUTEX_%s")

/********************************************************************************/

/* Message-No = (WM_USER + No), MSG ID, it's same with XFS */
#define XFS_USER                                0x0400

#define XFS_OPEN                                (XFS_USER + 1) //0x0401
#define XFS_CLOSE                               (XFS_USER + 2)
#define XFS_LOCK                                (XFS_USER + 3)
#define XFS_UNLOCK                              (XFS_USER + 4)
#define XFS_REGISTER                            (XFS_USER + 5)
#define XFS_DEREGISTER                          (XFS_USER + 6)
#define XFS_GETINFO                             (XFS_USER + 7)
#define XFS_EXECUTE                             (XFS_USER + 8)

#define XFS_EXECUTE_EVENT                       (XFS_USER + 20) //0x0414
#define XFS_SERVICE_EVENT                       (XFS_USER + 21) //0x0415
#define XFS_USER_EVENT                          (XFS_USER + 22) //0x0416
#define XFS_SYSTEM_EVENT                        (XFS_USER + 23) //0x0417

#define XFS_CANCEL                              (XFS_USER + 32) //0x0420
#define XFS_LOGLEVEL                            (XFS_USER + 33) //0x0421
#define XFS_VERSIONERROR                        (XFS_USER + 34) //0x0422
#define XFS_ACK                                 (XFS_USER + 35) //0x0423

/********************************************************************************/

#define  MAX_SP_NAME    63L

typedef enum
{
	  CMDF_NONE = 0L
	, CMDF_GI
	, CMDF_GC
	, CMDF_EX
	, CMDF_EC
	, CMDF_EV
}CMD_FLAG;

#pragma pack(push, 1)

typedef struct _reg_event
{
	HSERVICE  hService;
	DWORD	  dwEventClass;
	LPVOID	  hRegObject;
}REGEVENT, *LPREGEVENT;

typedef struct _session_data
{
	HSERVICE    hService;                      //服务句柄(会话ID)
	HPROVIDER   hProvider;                     //注册表参数dllname指定DLL的句柄
	HANDLE      hApp;                          //应用句柄
	DWORD       dwProcessID;                   //应用进程ID
	DWORD       dwThreadID;                    //应用线程ID
	DWORD       dwTraceLevel;                  //跟踪级别
	DWORD       dwSrvcVerRequired;             //SP的版本
	LPVOID	    hSPIObject;                    //接收对象(SPI的进程通讯对象)
	TCHAR       szLogicalName[MAX_SP_NAME + 1];//逻辑名
	TCHAR	    szAppID[MAX_SP_NAME + 1];	   //应用标识ID
}SESSION_DATA, *LPSESSION_DATA;

typedef struct _sp_pack
{
	UINT		uMsg;		//消息标识ID
	LPVOID		hWnd;		//接收对象(完成消息对象或事件对象)
	DWORD		dwTimeout;	//超时时间

	REQUESTID   RequestID;  //从该字段开始的以下字段必须和WFSRESULT结构保持一致
	HSERVICE    hService;
	SYSTEMTIME  tsTimestamp;
	union 
	{
		HRESULT hParameter; //SPI->SP
		HRESULT hResult;    //SP->SPI
	} ups;
	union 
	{
		DWORD   dwCommandCode;
		DWORD   dwEventID;
	} u;
	LPVOID      lpBuffer;
}SPPACK, *LPSPPACK;


#pragma pack(pop)

/********************************************************************************/

#endif  //_STRUCTKERNEL_H_20151024

