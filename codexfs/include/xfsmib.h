/************************************************************************
*                                                                       *
* xfsmib.h      XFS - MIB functions, types, and definitions             *
*                                                                       *
*               Version 3.20  --  Mar 28, 2014                          *
*                                                                       *
************************************************************************/

#ifndef __inc_xfsmib__h
#define __inc_xfsmib__h

#ifdef __cplusplus
extern "C" {
#endif

/*   be aware of alignment   */
#pragma pack(push,1)

/****** Common *********************************************************/

#include <windows.h>
#include <xfsapi.h>

/* XFS MIB Command codes */
#define     WFS_MIB_OFFSET                          (60000)
#define     WFS_INF_MIB_GET_RESPONSE_COUNTS         (WFS_MIB_OFFSET+1)
#define     WFS_INF_MIB_SET_RESPONSE_COUNT          (WFS_MIB_OFFSET+2)
#define     WFS_INF_MIB_RESET_RESPONSE_COUNTS       (WFS_MIB_OFFSET+3)

/* XFS MIB Count structures common across all devices */
typedef struct _wfs_mib_response_count
{
    DWORD    dwCommandCode;
    LONG     lResponseCode;
    DWORD    dwResponseCount;
} WFSMIBRESPONSECOUNT, * LPWFSMIBRESPONSECOUNT;
typedef struct _wfs_mib_reset_response_counts
{
    SYSTEMTIME    tsTimestamp;
} WFSMIBRESETRESPONSECOUNTS, * LPWFSMIBRESETRESPONSECOUNTS;



/*   restore alignment   */
#pragma pack(pop)

#ifdef __cplusplus
}       /*extern "C"*/
#endif

#endif  /* __inc_xfsmib__h */
