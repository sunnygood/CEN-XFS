/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IBaseSystem.h
* Description: the system interface of all OS,such as windows,linux and so on       
*
* Author:      liuguoxun
* Date:        2012-05-20
*
*****************************************************************************/


#ifndef  _IBASESYSTEM_H_20120520
#define  _IBASESYSTEM_H_20120520

#ifndef __cplusplus
#error ERROR: This platform must define __cplusplus.
#endif

#define  PLATFORM_VER              0x0603  //V3.6


#if ((defined _WINDOWS) || (defined WIN32) || (defined WIN64))
#define  __ms_windows__  
#elif (defined _ANDROID)
#define  __gg_android__  
#else
#define  __os_linux__ 
#endif

#ifdef __ms_windows__
#include <tchar.h>
#include <windows.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <process.h>
#include "errorcode.h"


//communication port type
typedef enum { 
      eCOM            = 0x0L  // Serial(such as "COM1:9600,N,8,1", "ttyS1:9600,N,8,1")
    , eUSB_FTDI       = 0x1L  // only for FTDI chip with baudrate, such as EPP("FT232R USB UART:9600,N,8,1")

    , eUSB            = 0x2L  // Windows(such as "VID_23AB&PID_0002","VID_23AB&PID_0002&REV_0900","USB\\VID_23AB&PID_0002") 
                             // Linux  (such as "usbxxx", "usb/xxx", lp0")
								  
    , eHID            = 0x3L  // Windows(such as "VID_23AB&PID_1003","VID_23AB&PID_1003&REV_0100","HID\\VID_23AB&PID_1003")
                             // Linux  (such as "hidxxx", "hid/xxx", "hiddev0")

    , ePC_SC          = 0x4L  // only for windows PC/SC()
    , eLPT            = 0x5L  // LPT(such as "LPT1")
    , eTCPIP          = 0x6L  // TCP(such as "127.0.0.1:36860")

    , eCOMBINE        = 0x9L  // It is combined with master device, and must behind master device instantiated
} EPORT;

typedef enum { 
	  CRYPT_DESECB     = (0x0001)    //DES ECB
	, CRYPT_DESCBC     = (0x0002)    //DES CBC
	, CRYPT_DESCFB     = (0x0004)    //DES CFB
	, CRYPT_RSA        = (0x0008)    //RSA
	, CRYPT_ECMA       = (0x0010)    //ECMA
	, CRYPT_DESMAC     = (0x0020)    //DES MAC
	, CRYPT_TRIDESECB  = (0x0040)    //TDES ECB
	, CRYPT_TRIDESCBC  = (0x0080)    //TDES CBC
	, CRYPT_TRIDESCFB  = (0x0100)    //TDES CFB
	, CRYPT_TRIDESMAC  = (0x0200)    //TDES MAC
	, CRYPT_MAAMAC     = (0x0400)    //MAA MAC
	, CRYPT_SM4ECB 	   = (0x1000)    //SM4 ECB (WFS_PIN_CRYPTSM4)
	, CRYPT_SM4CBC	   = (0x2000)    //SM4 CBC (WFS_PIN_CRYPTSM4MAC)

	, CRYPT_SM2        = (0x80000)   //SM2
	, CRYPT_AESECB     = (0x100000)  //AES ECB
	, CRYPT_AESCBC     = (0x200000)  //AES CBC
	, CRYPT_AESOFB     = (0x400000)  //AES OFB
	, CRYPT_AESCFB     = (0x800000)  //AES CFB
	, CRYPT_AESPCBC    = (0x1000000) //AES PCBC
	, CRYPT_AESCTR     = (0x2000000) //AES CTR
	, CRYPT_SM4CFB	   = (0x10000000)//SM4 CFB
	, CRYPT_SM4OFB	   = (0x20000000)//SM4 OFB
	, CRYPT_DESOFB	   = (0x40000000)//DES OFB
	, CRYPT_TRIDESOFB  = (0x80000000)//TDES OFB
} ECRYPT; //Crypt mode

typedef enum { 
	  MAC_X9         = 0x00  //X9.9  (ANSI X9.9-1986)
	, MAC_X919       = 0x01  //X9.19 (ISO9797-1,Alg3)
	, MAC_PSAM       = 0x02  //PSAM
	, MAC_PBOC       = 0x03  //PBOC
	, MAC_CBC        = 0x04  //CBC   (ISO16609 = ISO9797-1,Alg1)
	, MAC_BANKSYS    = 0x05  //China POS UnionPay
	, AES_CMAC       = 0x06  //AES-CMAC-PRF-128
	, AES_XCBC       = 0x07  //AES-XCBC-PRF-128
	, SM4MAC_PBOC    = 0x08  //PBOC
	, SM4MAC_CBC     = 0x09  //SM4 CBC
	, SM4MAC_BANKSYS = 0x0A  //SM4 China POS UnionPay
} EMAC;

typedef enum { 
	  HASH_NONE            =  0x0000 //no KCV

	, HASH_SHA1            =  0x0001 //SHA1,   Hash Algorithm or KCV
	, HASH_SHA256          =  0x0002 //SHA256, Hash Algorithm or KCV
	, HASH_SHA512          =  0x0003 //SHA512, Hash Algorithm or KCV

	, HASH_SHA224          =  0x0008 //SHA224, Hash Algorithm or KCV 
	, HASH_SHA384          =  0x0009 //SHA384, Hash Algorithm or KCV 
} EHASH;

enum eEncode 
{
	ENC_ASCII      = 0,
	ENC_NATIVE_OEM = 1, //windows
	ENC_NATIVE_MAC = 2, //Mac computer
	ENC_THREAD_ASC = 3,
	ENC_UTF7       = 65000,
	ENC_UTF8       = 65001
};

/********************************************************************************/

#if ((defined _MSC_VER) && (_MSC_VER > 1200)) 
#define  INTERFACE_I   __interface
#else
#define  INTERFACE_I   class   // vs6.0 or earlier or other builder
#endif


#if ((defined _MSC_VER) && (_MSC_VER >= 800)) 
#ifndef CALLBACK
#define CALLBACK    __stdcall
#define WINAPI      __stdcall
#define WINAPIV     __cdecl
#define APIENTRY    WINAPI
#define APIPRIVATE  __stdcall
#define PASCAL      __stdcall
#endif
#else 
#ifndef CALLBACK
#define CALLBACK
#define WINAPI
#define WINAPIV
#define APIENTRY    WINAPI
#define APIPRIVATE
#define PASCAL      pascal
#endif //CALLBACK
#endif //((defined _MSC_VER) && (_MSC_VER >= 800))


#ifndef _MAC
typedef wchar_t WCHAR;    // wide char, 16-bit UNICODE character
#else
// some Macintosh compilers don't define wchar_t in a convenient location, or define it as a char
typedef unsigned short WCHAR;
#endif //_MAC


#ifdef  UNICODE
	#ifndef _TCHAR_DEFINED
	typedef WCHAR TCHAR, *PTCHAR;
	typedef WCHAR TBYTE, *PTBYTE;
	#define _TCHAR_DEFINED
	#endif /* !_TCHAR_DEFINED */

	typedef wchar_t BYTE_T, *PBYTE_T;

	#ifndef _T
	#define _T(x)      L##x
	#define __TEXT(x)  L##x
	#endif //_T
#else
	#ifndef _TCHAR_DEFINED
	typedef char TCHAR, *PTCHAR;
	typedef unsigned char TBYTE, *PTBYTE;
	#define _TCHAR_DEFINED
	#endif /* !_TCHAR_DEFINED */

	typedef unsigned char BYTE_T, *PBYTE_T;

	#ifndef _T
	#define _T(x)      x
	#define __TEXT(x)  x
	#endif //_T
#endif //UNICODE


/****************************************************************************/

//if you don't need define below, you can define _NAME_TYPEDEFINE before include IBaseSystem.h
#ifndef _NAME_TYPEDEFINE
typedef char                I8;
typedef short               I16;
typedef long                I32;
typedef long long           I64;
typedef int                 INT;
typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned long       U32;
typedef unsigned long long  U64;
typedef unsigned int        UINT;

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef void               *HANDLE;
typedef void               *LPVOID;
typedef char               *LPSTR;
typedef const char         *LPCSTR;
typedef wchar_t            *LPWSTR;
typedef const wchar_t      *LPCWSTR;
typedef TCHAR              *LPTSTR;
typedef const TCHAR        *LPCTSTR;
typedef BYTE               *LPBYTE;
typedef WORD               *LPWORD;
typedef DWORD              *LPDWORD;
typedef UINT               *LPUINT;

#define _NAME_TYPEDEFINE
#endif //_NAME_TYPEDEFINE


#ifdef __ms_windows__
typedef DWORD              THREADRET;
#else
typedef void *             THREADRET;
#endif


#ifdef DECLARE_HANDLE
typedef HINSTANCE           HMODULE;
typedef HKEY                *PHKEY;
typedef HWND                *PHWND;
#else 
typedef LPVOID              HMODULE;
typedef LPVOID              HKEY;
typedef LPVOID              HWND;
#endif //DECLARE_HANDLE


/****************************************************************************/


#ifndef MAKEWORD
#if defined(_WIN64)
    typedef unsigned __int64 DWORD_PTR, *PDWORD_PTR;
#else
    typedef unsigned long DWORD_PTR, *PDWORD_PTR;
#endif

#define MAKEBYTE(l, h)      ((BYTE)((((BYTE)(l)) & 0x0F) | (((BYTE)(h)) << 4)))
#define MAKEWORD(l, h)      ((WORD)(((BYTE)(((DWORD_PTR)(l)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(h)) & 0xff))) << 8))
#define MAKELONG(l, h)      ((LONG)(((WORD)(((DWORD_PTR)(l)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(h)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#define LOBITS(b)           ((BYTE)(b) & 0x0F)
#define HIBITS(b)           ((BYTE)(((BYTE)(b)>>4) & 0x0F))
#endif //MAKEWORD


#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


#ifndef MAX_PATH
#define MAX_PATH            260
#endif


#define LINKWITHNAME(s, n)  (s##n)


/****************************************************************************/

typedef  THREADRET (WINAPI *PTHREAD_FUNC)(LPVOID lpThreadParameter);


//if you don't need define below, you can define _SAFE_FUNC_DEFINE before include IBaseSystem.h
#ifndef _SAFE_FUNC_DEFINE
#ifdef __ms_windows__
#define  LOAD_LIB(lpModuleName)                  LoadLibrary(lpModuleName)
#define  FREE_LIB(hLib)                          FreeLibrary(hLib)
#define  FUNC_GET(hLib, lpFuncName)              GetProcAddress(hLib, lpFuncName)

#define  MEMCPY_S(dst, dstSize, src, len)        memcpy_s(dst, dstSize, src, len)
#define  STRCAT_S(dst, dstSize, src)             strcat_s(dst, dstSize, src)
#define  STRCPY_S(dst, dstSize, src)             strcpy_s(dst, dstSize, src)

#define  STRICMP(dst, src)                       stricmp(dst, src)

#define  TCSCAT_S(dst, dstSize, src)             _tcscat_s(dst, dstSize, src)
#define  TCSCPY_S(dst, dstSize, src)             _tcscpy_s(dst, dstSize, src)
#define  TCSCPY(dst, src)                        _tcscpy(dst, src)
#define  TCSLEN(src)                             _tcslen(src)
#define  TCSCMP(dst, src)                        _tcscmp(dst, src)
#define  TCSICMP(dst, src)                       _tcsicmp(dst, src)
#define  TCSTOK(dst, delim)                      _tcstok(dst, delim)
#define  TCSTOK_S(dst, delim, p)                 _tcstok_s(dst, delim, p)
#define  TCSSTR(s, sub)                          _tcsstr(s, sub)
#define  TCSCHR(src, chr)                        _tcschr(src, chr)
#define  TCSRCHR(src, chr)                       _tcsrchr(src, chr)
#define  TCSNCPY(dst, src, count)                _tcsncpy(dst, src, count)
#define  TCSNCPY_S(dst, dstSize, src, count)     _tcsncpy_s(dst, dstSize, src, count)
#define  TCSNCMP(dst, src, count)                _tcsncmp(dst, src, count)
#define  TCSCANF(dst, format, ...)               _tcscanf(dst, format, ##__VA_ARGS__)

#define  FOPEN(fn, mode)                         _tfopen(fn, mode)
#define  FPUTS(d, fn)                            _fputts(d, fn)
#define  FPRINTF(dst, format, ...)               _ftprintf(dst, format, ##__VA_ARGS__)
#define  ACCESS(fn, mode)                        _taccess(fn, mode)
#define  STAT(fn, p)                             _tstat(fn, (struct _stat64i32 *)p)
#define  RENAME(ofn, nfn)                        _trename(ofn, nfn)
#define  SSCANF(src, format, ...)                _stscanf(src, format, ##__VA_ARGS__)


#define  SPRINTF(dst, format, ...)               _stprintf(dst, format, ##__VA_ARGS__)
#define  SPRINTF_S(dst, dstSize, format, ...)    _stprintf_s(dst, dstSize, format, ##__VA_ARGS__)
#define  VSPRINTF_S(dst, dstSize, format, ...)   _vstprintf_s(dst, dstSize, format, ##__VA_ARGS__)
#define  VFPRINTF(dst, format, ...)              _vftprintf(dst, format, ##__VA_ARGS__)
#define  ATOI(s)                                 _ttoi(s)
#define  SLEEP(t)                                Sleep(t)
#define  POSTMESSAGE(hWnd, Msg, wParam, lParam)  PostMessage(hWnd, Msg, wParam, lParam)
#define  SENDMESSAGE(hWnd, Msg, wParam, lParam)  SendMessage(hWnd, Msg, wParam, lParam)

#define  TIMEPOINT(t);                           t = GetTickCount();
#define  CHECKTIMEOUT(p, t)                      if((GetTickCount() - p > t))
#else
#define  LOAD_LIB(lpModuleName)                  dlopen(lpModuleName, RTLD_LAZY)
#define  FREE_LIB(hLib)                          !dlclose(hLib)
#define  FUNC_GET(hLib, lpFuncName)              dlsym(hLib, lpFuncName)

#define  MEMCPY_S(dst, dstSize, src, len)        memcpy(dst, src, len)
#define  STRCAT_S(dst, dstSize, src)             strcat(dst, src)
#define  STRCPY_S(dst, dstSize, src)             strcpy(dst, src)

#define  STRICMP(dst, src)                       strcasecmp(dst, src)

#define  TCSCAT_S(dst, dstSize, src)             strcat(dst, src)
#define  TCSCPY_S(dst, dstSize, src)             strcpy(dst, src)
#define  TCSCPY(dst, src)                        strcpy(dst, src)
#define  TCSLEN(src)                             strlen(src)
#define  TCSCMP(dst, src)                        strcmp(dst, src)
#define  TCSICMP(dst, src)                       strcasecmp(dst, src)
#define  TCSTOK(dst, delim)                      strtok(dst, delim)
#define  TCSTOK_S(dst, delim, p)                 strtok(dst, delim)
#define  TCSSTR(s, sub)                          strstr(s, sub)
#define  TCSCHR(src, chr)                        strchr(src, chr)
#define  TCSRCHR(src, chr)                       strrchr(src, chr)
#define  TCSNCPY(dst, src, count)                strncpy(dst, src, count)
#define  TCSNCPY_S(dst, dstSize, src, count)     strncpy(dst, src, count)
#define  TCSNCMP(dst, src, count)                strncmp(dst, src, count)
#define  TCSCANF(dst, format, ...)               sscanf(dst, format, ##__VA_ARGS__)

#define  FOPEN(fn, mode)                         fopen(fn, mode)
#define  FPUTS(d, fn)                            fputs(d, fn)
#define  FPRINTF(dst, format, ...)               fprintf(dst, format, ##__VA_ARGS__)
#define  ACCESS(fn, mode)                        access(fn, mode)
#define  STAT(fn, p)                             stat(fn, p)
#define  RENAME(ofn, nfn)                        rename(ofn, nfn)
#define  SSCANF(src, format, ...)                sscanf(src, format, ##__VA_ARGS__)

#define  SPRINTF(dst, format, ...)               sprintf(dst, format, ##__VA_ARGS__)
#define  SPRINTF_S(dst, dstSize, format, ...)    sprintf(dst, format, ##__VA_ARGS__)
#define  VSPRINTF_S(dst, dstSize, format, ...)   vsprintf(dst, format, ##__VA_ARGS__)
#define  VFPRINTF(dst, format, ...)              vfprintf(dst, format, ##__VA_ARGS__)
#define  ATOI(s)                                 atoi(s)
#define  SLEEP(t)                                usleep(t * 1000)
#define  POSTMESSAGE(hWnd, Msg, wParam, lParam)  
#define  SENDMESSAGE(hWnd, Msg, wParam, lParam)  

#define  TIMEPOINT(t);                           struct timeval stTimeP; \
                                                 gettimeofday(&stTimeP, NULL); \
                                                 t = stTimeP.tv_sec * 1000 + stTimeP.tv_usec / 1000;
#define  CHECKTIMEOUT(p, t)                      struct timeval stTimeC; \
                                                 gettimeofday(&stTimeC, NULL); \
                                                 if(stTimeC.tv_sec * 1000 + stTimeC.tv_usec / 1000 - p > t)
#endif

#define _SAFE_FUNC_DEFINE
#endif //_SAFE_FUNC_DEFINE


//if you don't need define below, you can define _QUICK_FUNC_DEFINE before include IBaseSystem.h
#ifndef _QUICK_FUNC_DEFINE
#define  DELETE_P(p);  if(NULL != p) \
                       {             \
                           delete p; \
                           p = NULL; \
                       }

#define  SAFE_EXEC(p, f);  if(NULL != p) \
                           {             \
                               p->f;     \
                           }

#define  SAFE_CALL(p, f);  if(NULL != p) \
							{             \
								return p->f; \
							}\
							else \
							{ \
								return EC_POINTER_NULL; \
							} \

#define  FAIL_RET2(r, f); \
                           r = f;     \
                           if(r != 0)    \
                           { \
                               return r; \
                           } \

#define  FAIL_RET(r, p, f); \
                           if(p != 0) \
                           { \
                               r = p->f;     \
                               if(r != 0)    \
                               { \
                                   return r; \
                               } \
                           } \
                           else \
                           { \
                               return EC_POINTER_NULL; \
                           } \

#define  SUCC_RET(r, p, f); \
                           if(p != 0)        \
                           { \
                               r = p->f;     \
                               if(r == 0)    \
                               { \
                                   return r; \
                               } \
                           } \
                           else \
                           { \
                               return EC_POINTER_NULL; \
                           } \

#define _QUICK_FUNC_DEFINE
#endif //_QUICK_FUNC_DEFINE


/********************************************************************************/


#ifdef __cplusplus
extern "C" 
{
#endif //__cplusplus


//OS, process and thread, directory, file and so on
#ifndef  __os_module__1
HMODULE WINAPI Load_Lib(LPCTSTR lpModuleName);
BOOL    WINAPI Free_Lib(HMODULE lpLib);
void *  WINAPI Func_Get(HMODULE lpLib, LPCSTR lpFuncName);

UINT    WINAPI OS_CreateProcess(LPSTR lpCommandLine, UINT uCmdShow = SW_NORMAL);
HANDLE  WINAPI OS_CreateThread(PTHREAD_FUNC lpThreadProc, LPVOID lpParameter, LPDWORD lpThreadId);

LPTSTR  WINAPI GetRunPath(LPTSTR lpPath, DWORD dwBufLen, LPVOID hModule = 0);
void    WINAPI AutoCreateDirectory(LPCTSTR strPath);
void    WINAPI DeleteAllDirectory(LPCTSTR pcDirectory, BOOL bDirectoryDelete);

long    WINAPI SaveFileC(LPCTSTR SavePath, LPCSTR pData, DWORD dwLen);
long    WINAPI ReadFileC(LPCTSTR FilePath, char *pBuf, DWORD &dwLen);
long    WINAPI GetFileVersion(LPCTSTR pFileName, LPTSTR lpVersion);
DWORD   WINAPI OS_GetFileSize(LPCTSTR pFileName);
long    WINAPI OS_CreateGUID(LPTSTR lpGUIDBuffer);
long    WINAPI OS_GetLocalTime(struct tm *pTM, LPWORD pMillisecond = 0);
BOOL    WINAPI Is64BitSystem(void);

#define  __os_module__1
#endif //__os_module__1


/********************************************************************************/

//Common module
#ifndef  __common_module__3
DWORD WINAPI Hex2Bin(char *pBin, DWORD dwBufLen, LPCSTR pHex, DWORD dwLen); //2 visible hex char to 1 binary char
DWORD WINAPI Bin2Hex(char *pHex, DWORD dwBufLen, LPCSTR pBin, DWORD dwLen); //1 binary char to 2 visible hex char

DWORD WINAPI THex2Bin(LPTSTR pBin, DWORD dwBufLen, LPCTSTR pHex, DWORD dwLen); //2 visible hex char to 1 binary char
DWORD WINAPI TBin2Hex(LPTSTR pHex, DWORD dwBufLen, LPCTSTR pBin, DWORD dwLen); //1 binary char to 2 visible hex char

DWORD WINAPI HexFile2Bin(LPCSTR pFileBuffer, DWORD nFileLen, char *pOut); //line begin with 0x3A

TCHAR * WINAPI TCSUPR(TCHAR *str);
TCHAR * WINAPI TCSLWR(TCHAR *str);

DWORD WINAPI CharAnd_RightSide(LPCSTR str1, DWORD dwLen1, LPCSTR str2, DWORD dwLen2, char *pOut);
DWORD WINAPI CharXor_RightSide(LPCSTR str1, DWORD dwLen1, LPCSTR str2, DWORD dwLen2, char *pOut);
DWORD WINAPI CharAnd(LPCSTR str1, char *str2, int nLen); //LeftSide, str1 And str2(And nLen char), save at str2 
DWORD WINAPI CharXor(LPCSTR str1, char *str2, int nLen); //LeftSide, str1 XOR str2(XOR nLen char), save at str2 
BYTE  WINAPI XORSum(LPCSTR lpBuf, int nLen);
BYTE  WINAPI AddSum(LPCSTR lpBuf, int nLen);
BOOL  WINAPI IsSplitChar(LPCSTR str, int nLen);
WORD  WINAPI CRC16(LPBYTE lpData, int nLen);
WORD  WINAPI MD5(LPBYTE lpData, int nLen, char *pOut);
BYTE  WINAPI Padding2Bin(BYTE bPadding);
BYTE  WINAPI Padding2Hex(BYTE bPadding);

int  WINAPI Native2WChar(LPCSTR lpNative, eEncode eSrc, LPWSTR lpWChar, int nBufLen);
int  WINAPI WChar2Native(LPCWSTR lpWChar, eEncode eDest, LPSTR lpNative, int nBufLen);
int  WINAPI Native2UTF8(LPCSTR lpNative, eEncode eSrc, LPSTR lpUTF8, int nBufLen);
int  WINAPI UTF82Native(LPCSTR lpUTF8, eEncode eDest, LPSTR lpNative, int nBufLen);

void WINAPI ConvertSpaceToZero(char *lpData, int nLen);// ' ' -> '0'
void WINAPI FillZeroInLeft(char *lpData, int nLen);
void WINAPI FillSpaceInLeft(char *lpData, int nLen);
void WINAPI FillSpaceInRight(char *lpData, int nLen);
void WINAPI DelSpaceInLeft(char *lpData, int nLen);
void WINAPI DelSpaceInRight(char *lpData, int nLen);

int  WINAPI WeekOfYearToday(long iYear, long iMonth, long iDay, long iDayOfWeek); //week of today


long WINAPI DER_Pack(LPCSTR lpSig, WORD wSigLen, LPCSTR lpDer, WORD &wInOutLen, char *pTag);
long WINAPI DER_UnPack(LPCSTR lpTag, WORD wTagLen, LPSTR lpDer, WORD &wDerLen, LPSTR lpSig, WORD &wSigLen);

#define __common_module__3
#endif //__common_module__3


/********************************************************************************/

//Config module
#ifndef  __config_module__7
long  WINAPI INI_WriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);
long  WINAPI INI_ReadString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
long  WINAPI INI_ReadInteger(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault, LPCTSTR lpFileName);


long  WINAPI JSON_ReadString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
long  WINAPI JSON_ReadInteger(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault, LPCTSTR lpFileName);

BOOL  WINAPI DB_InitParaTable(LPCTSTR lpSQLFile, LPCTSTR lpDBName = 0);
long  WINAPI DB_WriteString(LPCTSTR lpTableName, LPCTSTR lpKeyName, LPCTSTR lpValue, LPCTSTR lpDBName = 0);
long  WINAPI DB_ReadString(LPCTSTR lpTableName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpDBName = 0);
long  WINAPI DB_ReadInteger(LPCTSTR lpTableName, LPCTSTR lpKeyName, int nDefault, LPCTSTR lpDBName = 0);


#ifdef __ms_windows__
long  WINAPI REG_ReadString(HKEY keyBase, LPCTSTR keyPath, LPCTSTR keyName, LPTSTR pBuf, long nBufSize, LPCTSTR lpDefault);
long  WINAPI REG_ReadInteger(HKEY keyBase, LPCTSTR keyPath, LPCTSTR keyName, long nDefault);
long  WINAPI REG_WriteInteger(HKEY keyBase, LPCTSTR keyPath, LPCTSTR keyName, long nValue);
long  WINAPI REG_WriteString(HKEY keyBase, LPCTSTR keyPath, LPCTSTR keyName, LPCTSTR lpValue);
#endif //__ms_windows__

#define  __config_module__7
#endif //__config_module__7



#ifdef __cplusplus
}
#endif //__cplusplus


#endif //_IBASESYSTEM_H_20120520

