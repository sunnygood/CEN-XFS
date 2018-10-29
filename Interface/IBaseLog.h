/*****************************************************************************
* Copyright(C) 1993-2012, Corporation.
*
* File Name:   IBaseLog.h
* Description: the log interface       
*
* Author:      liuguoxun
* Date:        2012-05-20
* History 1: 
*
*****************************************************************************/


#ifndef  _ILOG_H_20120520
#define  _ILOG_H_20120520

#include "IBaseSystem.h"


#define  LOG_MODULE_VER              PLATFORM_VER


/* log level constant */
#define LOG_LEVEL_DEBUG               (0x1)
#define LOG_LEVEL_COMMON              (0x2)
#define LOG_LEVEL_WARN                (0x3)
#define LOG_LEVEL_DATA                (0x4)
#define LOG_LEVEL_ERROR               (0x5)


/* current function macro defined (ANSI only) */
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
    #define CURRENT_FUNC     __FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
    #define CURRENT_FUNC     __FUNCTION__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
    #define CURRENT_FUNC     __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
    #define CURRENT_FUNC     __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
    #define CURRENT_FUNC     __func__
#elif defined(__func__)
    #define CURRENT_FUNC     __func__
#elif defined(__FUNCTION__)
    #define CURRENT_FUNC     __FUNCTION__
#elif (__linux__ || ((defined _MSC_VER) && (_MSC_VER > 1200))) 
    #define CURRENT_FUNC     __FUNCTION__
#else
    #define CURRENT_FUNC     "Unknown"
#endif


/* for easy use, all log can become :
 * RLog2(int nLevel, const char *szFormat, ...);
 * RHex2(int nLevel, const char *szData, unsigned int uiDataLength);
 * RDebug2(const char *szHead, const char *szData, unsigned int uiDataLength);
 *
 * for example in pinpad module:
 * in the pinpadbase.h, we can do this once:
 #undef  LOG_NAME_LOG
 #define LOG_NAME_LOG     "Pinpad.log"
 #undef  LOG_NAME_DEBUG
 #define LOG_NAME_DEBUG   "DPinpad.log"
 */
 
// default name of log
#define  LOG_NAME_LOG       _T("Log.log")
#define  LOG_NAME_DEBUG     _T("Debug.log")


// Debug function, only active at Debug lib
#ifdef _DEBUG
    #define DLog2(nLevel, ft, ...)  \
			LLog(LOG_NAME_LOG, __FILE__, __LINE__, CURRENT_FUNC, (nLevel), (ft), ##__VA_ARGS__)

	#define DHex2(nLevel, pData, nLen)  \
		    LHex(LOG_NAME_LOG, __FILE__, __LINE__, CURRENT_FUNC, (nLevel), (pData), (nLen))

	#define DMix2(nLevel, pHead, pData, nLen)  \
		    LMix(LOG_NAME_DEBUG, __FILE__, __LINE__, CURRENT_FUNC, (nLevel), (pHead), (pData), (nLen))
#else
    #define DLog2(nLevel, ft, ...) 

	#define DHex2(nLevel, pData, nLen) 

	#define DMix2(nLevel, pHead, pData, nLen) 
#endif


// useful function
#if (__linux__ || ((defined _MSC_VER) && (_MSC_VER > 1200))) 
    #define RLog2(nLevel, ft, ...)  \
			LLog(LOG_NAME_LOG, __FILE__, __LINE__, CURRENT_FUNC, (nLevel), (ft), ##__VA_ARGS__)

	#define RLog5(filename, nLevel, ft, ...)  \
			LLog(filename, __FILE__, __LINE__, CURRENT_FUNC, (nLevel), (ft), ##__VA_ARGS__)
#else
    #define RLog2(nLevel, ft, ...) 

	#define RLog5(filename, nLevel, ft, ...)
#endif

#define RHex2(nLevel, pData, nLen)  \
		    LHex(LOG_NAME_LOG, __FILE__, __LINE__, CURRENT_FUNC, (nLevel), (pData), (nLen))

#define RMix2(nLevel, pHead, pData, nLen)  \
		    LMix(LOG_NAME_DEBUG, __FILE__, __LINE__, CURRENT_FUNC, (nLevel), (pHead), (pData), (nLen))

#define RHex5(filename, nLevel, pData, nLen)  \
		    LHex(filename, __FILE__, __LINE__, CURRENT_FUNC, (nLevel), (pData), (nLen))


/********************************************************************************/


#ifdef __cplusplus
extern "C" 
{
#endif //__cplusplus


/********************************************************************************/

//Log module
#ifndef  __log_module__5
void WINAPI LLog(LPCTSTR lpLogFile, LPCSTR lpCppFile, int nLine, LPCSTR lpModule, int nLevel, LPCTSTR lpFormat, ...);
void WINAPI LHex(LPCTSTR lpLogFile, LPCSTR lpCppFile, int nLine, LPCSTR lpModule, int nLevel, LPCTSTR lpData, UINT uiDataLength);
void WINAPI LMix(LPCTSTR lpLogFile, LPCSTR lpCppFile, int nLine, LPCSTR lpModule, int nLevel, LPCTSTR lpHead = 0, LPCTSTR lpData = 0, UINT uiDataLength = 0);
void WINAPI LogConfig(int nLevel = LOG_LEVEL_DATA, LPCTSTR lpLogDir = NULL, int nSaveMode = -2, int nExtend = 0);
void WINAPI LogDelete(LPCTSTR lpLogDir, int nExpireDays = 180);


void WINAPI Log5(LPCTSTR lpFileName, int nLevel, LPCTSTR lpFormat, ...);

#define  __log_module__5
#endif //__log_module__5


#ifdef __cplusplus
}
#endif //__cplusplus


#endif //_ILOG_H_20120520

