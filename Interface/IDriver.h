/*****************************************************************************
* Copyright(C) 1993-2012, Corporation.
*
* File Name:   IDriver.h
* Description: the base interface of all the hardware device       
*
* Author:      liuguoxun
* Date:        2012-04-20
* History 1: 
*
*****************************************************************************/


#ifndef  _IDRIVER_H_20120420
#define  _IDRIVER_H_20120420


#include <IBaseSystem.h>


/* values of nMode in function long Init(long nMode = 0)*/
#define  INIT_MODE_ALL         0x00L
#define  INIT_MODE_HW          0x01L //only hardware/firmware init
#define  INIT_MODE_SOFT        0x02L //only software init


/********************************************************************************/

//   interface in/out parameter£º
//   1¡¢bin character(0x00 ~ 0xFF)
//   2¡¢viewable character string(isn't split character, such as ID,SN,version)

// the base interface for all the device  
INTERFACE_I  IDriver
{
public:
	//communication function
	virtual long Open(LPTSTR lpDesc) = 0; //see EPORT remark to get lpDesc
	virtual long ReOpen(DWORD dwAfterTime = 0) = 0;
	virtual bool IsOpen(void) const = 0;
	virtual long Close(void)  = 0;
	virtual long SetBaudrate(DWORD dwBaudrate, int bOnlyPort = 0) = 0; //if bOnlyPort is FALSE, that is set hardware and port together
	virtual DWORD GetCurBaudrate(void) = 0;
	virtual long SendData(const char *pData, DWORD dwDataLen, DWORD dwTimeout) = 0;
	virtual long ReadData(char *pData, DWORD &dwInOutLen, DWORD dwTimeout) = 0;
	virtual long Cancel(void) = 0;
	virtual void *GetPortPointer(void) = 0;


	//Download FW
	virtual DWORD OpenFile(char *pOutBuf, LPCTSTR lpFileName, LPCSTR lpDecryptKey = 0, WORD wKeyLen = 0) = 0;
	virtual long ChipCommunicate(unsigned char Mode, char *pChipInfo = 0) = 0;
	virtual long DownProgram(const char *pcFileData, long nFileLen, unsigned char Mode, void *hWnd = 0) = 0;


	//base operate
	virtual long Init(long nMode = 0)  = 0;	//reset, and clear other(such as parameter, key, password)
	virtual long Reset(void) = 0;           //reset, just like power on
	virtual long SelfTest(long iComponent = 0) = 0;
	virtual long GetStatus(void) = 0;
	virtual long GetCapabilities(void *lpvCap) = 0;
	virtual long Execute(long iCmdIndex, char *pcBuffer, long &iBufOrRetLen) = 0;
	virtual long GetLastErrorCode(void) = 0;
	virtual char *GetErrorInfo(long iCode) = 0;
	virtual long GetHardwareID(char *pcModelId)  = 0;
	virtual long GetHardwareSN(char *pcSn)  = 0;
	virtual long GetHardwareVersion(char *pcVersion)  = 0;
	virtual long GetLoaderVersion(char *pcVersion)  = 0;
	virtual long GetSoftVersion(char *pcSoftVersion, void *hModule = 0) = 0;
}; 


#endif //_IDRIVER_H_20120420

