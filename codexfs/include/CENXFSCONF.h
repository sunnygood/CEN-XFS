/*****************************************************************************
* Copyright(C) 2015,  Corporation.
*
* File Name:   CENXFSCONF.h
* Description: definitions for the Configuration functions      
*
* Author:      liuguoxun
* Date:        2015-06-18        
*
*****************************************************************************/
 
#ifndef _CENXFSCONF_H_20150618
#define _CENXFSCONF_H_20150618

#ifdef __cplusplus
extern "C" {
#endif

	/******* Common **************************************************************/

#include <XFSAPI.h>

	/*   be aware of alignment   */
#pragma pack(push,1)

// following HKEY and PHKEY are already defined in WINREG.H
// so definition has been removed
// typedef HANDLE  HKEY;
// typedef HANDLE * PHKEY;

/******* Value of hKey *******************************************************/
#define     WFS_CFG_HKEY_XFS_ROOT               ((HKEY)1)

// JMK 1/9/00 need to define root keys for 3.0 registry
#define		WFS_CFG_USER_DEFAULT_XFS_ROOT       ((HKEY)2)
#define		WFS_CFG_MACHINE_XFS_ROOT            ((HKEY)3)
// ZDC 3/29/01 define another root key for 3.0 registry - HKEY_CURRENT_USER
#define		WFS_CFG_CURRENT_USER_XFS_ROOT		((HKEY)4)

/* 3.10/3.20 */
#define     WFS_CFG_HKEY_MACHINE_XFS_ROOT       ((HKEY)2)
#define     WFS_CFG_HKEY_USER_DEFAULT_XFS_ROOT  ((HKEY)3)

/******* Values of lpdwDisposition *******************************************/
#define     WFS_CFG_CREATED_NEW_KEY             (0)
#define     WFS_CFG_OPENED_EXISTING_KEY         (1)

/******* Configuration Functions *********************************************/
HRESULT extern  WINAPI  WFMCloseKey(HKEY hKey);

HRESULT extern  WINAPI  WFMCreateKey(HKEY hKey, LPSTR lpszSubKey, PHKEY phkResult, LPDWORD lpdwDisposition);

HRESULT extern  WINAPI  WFMDeleteKey(HKEY hKey, LPSTR lpszSubKey);

HRESULT extern  WINAPI  WFMDeleteValue(HKEY hKey, LPSTR lpszValue );

HRESULT extern  WINAPI  WFMEnumKey(HKEY hKey, DWORD iSubKey, LPSTR lpszName, LPDWORD lpcchName, PFILETIME lpftLastWrite);

HRESULT extern  WINAPI  WFMEnumValue(HKEY hKey, DWORD iValue, LPSTR lpszValue, LPDWORD lpcchValue, LPSTR lpszData, LPDWORD lpcchData);

HRESULT extern  WINAPI  WFMOpenKey(HKEY hKey, LPSTR lpszSubKey, PHKEY phkResult);

HRESULT extern  WINAPI  WFMQueryValue(HKEY hKey, LPSTR lpszValueName, LPSTR lpszData, LPDWORD lpcchData);

HRESULT extern  WINAPI  WFMSetValue(HKEY hKey, LPSTR lpszValueName, LPSTR lpszData, DWORD cchData);


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif  /* _CENXFSCONF_H_20150618 */
