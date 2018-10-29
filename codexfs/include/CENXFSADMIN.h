/*****************************************************************************
* Copyright(C) 2015,  Corporation.
*
* File Name:   CENXFSADMIN.h
* Description: Administration and Support functions  
*
* Author:      liuguoxun
* Date:        2015-06-18        
*
*****************************************************************************/

#ifndef _CENXFSADMIN_H_20150618
#define _CENXFSADMIN_H_20150618

#ifdef __cplusplus
extern "C" {
#endif

#include <XFSAPI.h>

/*   be aware of alignment   */
#pragma pack(push,1)


/* values of ulFlags used for WFMAllocateBuffer */
#define WFS_MEM_SHARE                        0x00000001
#define WFS_MEM_ZEROINIT                     0x00000002

/****** Support Functions ****************************************************/

HRESULT extern WINAPI WFMAllocateBuffer(ULONG ulSize, ULONG ulFlags, LPVOID * lppvData);

HRESULT extern WINAPI WFMAllocateMore(ULONG ulSize, LPVOID lpvOriginal, LPVOID * lppvData);

HRESULT extern WINAPI WFMFreeBuffer(LPVOID lpvData);

HRESULT extern WINAPI WFMGetTraceLevel(HSERVICE hService, LPDWORD lpdwTraceLevel);

HRESULT extern WINAPI WFMKillTimer(WORD wTimerID);

HRESULT extern WINAPI WFMOutputTraceData(LPSTR lpszData);

HRESULT extern WINAPI WFMReleaseDLL(HPROVIDER hProvider);

HRESULT extern WINAPI WFMSetTimer(HWND hWnd, LPVOID lpContext, DWORD dwTimeVal, LPWORD lpwTimerID);


/*   restore alignment   */
#pragma pack(pop)

#ifdef __cplusplus
}       /*extern "C"*/
#endif

#endif    /* _CENXFSADMIN_H_20150618 */
