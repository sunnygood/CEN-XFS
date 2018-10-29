/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IXFSSPManage.h
* Description: the manage interface of XFS SP       
*
* Author:      liuguoxun
* Date:        2015-08-15         
*
*****************************************************************************/


#ifndef  _IXFSSPMANAGE_H_20150815
#define  _IXFSSPMANAGE_H_20150815

#include <XFSAPI.h>
#include <CENXFSADMIN.h>
#include <IBaseSystem.h>
#include <CSTLUnit.h>


#define  PLATFORM_SPVER           0x0603  //V3.6

extern  HMODULE g_hModule;


#define  WFS_VERSION_300     (0x0003) /* Version 3.00 */
#define  WFS_VERSION_310     (0x0A03) /* Version 3.10 */
#define  WFS_VERSION_320     (0x1403) /* Version 3.20 */
#define  WFS_VERSION_330     (0x1E03) /* Version 3.30 */

typedef enum {
	  XFSVER_FREE    =    0UL /* nonstandard, define by user */

	, XFSVER_300     =  300UL /* Version 3.00 */
	, XFSVER_310     =  310UL /* Version 3.10 */
	, XFSVER_320     =  320UL /* Version 3.20 */
	, XFSVER_330     =  330UL /* Version 3.30 */
}XFSVERS;

#pragma pack(push, 1)

typedef struct _thread_parameters_in
{
	DWORD	  dwValue;
	LPVOID	  pThis;
	LPVOID	  pPointer;
}THREADPARASIN, *LPTHREADPARASIN;

#pragma pack(pop)

/********************************************************************************/

class CAutoFreeXFSBuffer
{
public:
	CAutoFreeXFSBuffer(void *p)
	{
		m_p = p;
	}

	~CAutoFreeXFSBuffer()
	{
		if(NULL != m_p)
		{
			WFMFreeBuffer(m_p);
		}
		m_p = NULL;
	}

protected:
	void *m_p;
};

/********************************************************************************/

INTERFACE_I  IXFSSPManage
{
public:
	virtual BOOL    IsRegistrationCodeOK() = 0;	
	virtual int     CompareVersion(WORD wV1, WORD wV2) = 0;
	virtual DWORD   ConvertXFSVer(WORD wSrvcVers) = 0;
	virtual DWORD   ConvertType(LPCTSTR lpClass, LPCTSTR lpType) = 0;
	virtual HRESULT IsVerSupport(DWORD dwSPIVerRequired, LPWFSVERSION lpSPIVer, DWORD dwSrvcVerRequired, LPWFSVERSION lpSrvcVer, LPCTSTR lpServiceName) = 0;
	virtual LPCTSTR MsgId2Desc(UINT uMsg) const = 0;
	virtual LPCTSTR CmdId2ClassName(int nID) const = 0;
	virtual IXFSSPManage * Clone() const = 0;


	virtual LPCTSTR GetLogicalNodeStr(LPCTSTR lpLogicalName, LPCTSTR lpValueName, LPTSTR lpBuf, long nBufSize) = 0;
	virtual LPCTSTR GetService(LPCTSTR lpLogicalName, LPTSTR lpBuf, long nBufSize) = 0;
	virtual LPCTSTR GetServiceNodeStr(LPCTSTR lpServiceName, LPCTSTR lpSubKeyName, LPCTSTR lpValueName, LPTSTR lpBuf, long nBufSize) = 0;
	virtual LPCTSTR GetServiceNodeStr(LPCTSTR lpServiceName, LPCTSTR lpValueName, LPTSTR lpBuf, long nBufSize) = 0;
	virtual int     GetServiceNodeInt(LPCTSTR lpServiceName, LPCTSTR lpSubKeyName, LPCTSTR lpValueName, int nDefault) = 0;
	virtual int     GetServiceNodeInt(LPCTSTR lpServiceName, LPCTSTR lpValueName, int nDefault) = 0;

	virtual HRESULT SetServiceNodeStr(LPCTSTR lpServiceName, LPCTSTR lpSubKeyName, LPCTSTR lpValueName, LPTSTR lpValue) = 0;
	virtual HRESULT SetServiceNodeStr(LPCTSTR lpServiceName, LPCTSTR lpValueName, LPTSTR lpValue) = 0;


	virtual HRESULT CreateInteractiveObject(WNDPROC lpfnProc, LPCTSTR lpName, LPVOID lpOwnerObject, LPVOID *lppObject, int nType = 0) = 0;
	virtual HRESULT DestroyInteractiveObject(LPVOID lpObject, int nType = 0, LPCTSTR lpName = 0, LPVOID lpOwnerObject = 0) = 0;
	virtual BOOL    IsInteractiveObjectExist(LPVOID lpObject, int nType = 0) const = 0;
	virtual LPVOID  FindInteractiveObject(LPCTSTR lpName, int nType = 0) const = 0;


	virtual HRESULT LoadSPStruct(LPCTSTR lpClassName) = 0;
	virtual HRESULT RegisterEmbedCmd(int nCommand, int nProtocol = 0) = 0;//such as WFS_CMD_PIN_ENC_IO
	virtual HRESULT SendData(LPVOID lpPack, LPVOID lpObject, DWORD dwXFSVer, int nFlag) = 0;
	virtual HRESULT UnPack(LPVOID lpSrc, DWORD dwXFSVer, int nFlag, int nID, int nParentID = 0) = 0;
	virtual HRESULT SPIRecvProc(LPCSTR lpData, UINT nDataLen, DWORD dwXFSVer) = 0;
};



//Instance an object
//[IN] 
extern "C" IXFSSPManage * WINAPI InstanceSPManage();


//delete an object
extern "C" void WINAPI ReleaseSPManage(IXFSSPManage *p);


#endif  //_IXFSSPMANAGE_H_20150815

