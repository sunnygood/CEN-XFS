/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CPressKey.h
* Description: press key class for PIN
*
* Author:      liuguoxun
* Date:        2015-11-09         
*
*****************************************************************************/

#ifndef _CPRESSKEY_H_20151109
#define _CPRESSKEY_H_20151109


#include <CENXFSPIN.h>
#include <IXFSSPManage.h>
#include <CSTLUnit.h>


using namespace std;

#define MAX_NAME_PRESSKEY   30


typedef enum { 
	  SHIFT_NONE      =  (0x0000) //no shift
	, SHIFT_ONCE      =  (0x0001) //Soft shift only once(same as PC keyboard)
	, SHIFT_LONG      =  (0x0002) //Soft shift active long time, as far as you press it again to disable
	, SHIFT_HW        =  (0x0003) //Hardware shift
} ESHIFT;


#pragma pack(push, 1)

typedef struct _xfs_key_info
{
	ULONG                  ulValue;
	BYTE                   keyCode;
	TCHAR                  szName[MAX_NAME_PRESSKEY + 1];
}XFSKEYINFO, *LPXFSKEYINFO;

#pragma pack(pop)


/***************************************************************************/

class CPressKey  
{
	typedef vector<XFSKEYINFO>::iterator  VKIIT;
	typedef CVectorPtr<LPWFSPINFK>  VKEYPOS;

public:
	CPressKey();
	~CPressKey();

	void Init(LPCTSTR lpServiceName, IXFSSPManage *p);	
	
	void GetKeyValue(BYTE keyCode, ULONG &FK, ULONG &FDK);
	void GetPinCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, vector<BYTE> &vCodes);
	void GetDataCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, vector<BYTE> &vCodes);
	void GetSecureCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, vector<BYTE> &vCodes);
	void GetUniqueSecureCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, ULONG ulDisableFKs, vector<BYTE> &vCodes, vector<BYTE> &vDisableCodes);
	void GetHardShiftSecureCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, ULONG ulDisableFKs, vector<BYTE> &vCodes, vector<BYTE> &vDisableCodes);
	void GetSoftShiftSecureCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, ULONG ulDisableFKs, vector<BYTE> &vCodes, vector<BYTE> &vDisableCodes, vector<BYTE> &vShiftCodes, vector<BYTE> &vDisableShiftCodes);

	BYTE   GetFDKCode(ULONG ulValue);
	BYTE   GetFKCode(ULONG ulValue);
	LPTSTR GetFKsName(ULONG ulActiveFKs, LPTSTR lpOutName);
	LPTSTR GetFDKsName(ULONG ulActiveFDKs, LPTSTR lpOutName);


protected:
	void PushFK(LPCTSTR lpPath, LPCTSTR lpName, ULONG ulValue, int nDefaultValue);
	void PushFDK(LPCTSTR lpName, ULONG ulValue, int nDefaultValue);

	void InitKeyCodes();
	void InitSecureKeys();
	void InitEntryKeys(LPCTSTR SubKeyName, VKEYPOS &vEntryKeys, ULONG &ulFKMask, ULONG &ulFDKMask);

	void GetKeyCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, vector<BYTE> &vCodes, VKEYPOS &vKeys);

	ULONG  GetFDKs(LPCTSTR lpName);
	ULONG  GetFKs(LPCTSTR lpName);
	BOOL   GetFDKs(LPCTSTR lpName, ULONG &ulValue, BYTE &keyCode);
	BOOL   GetFKs(LPCTSTR lpName, ULONG &ulValue, BYTE &keyCode);


public:
	ULONG m_ulClearFDKs, m_ulCancelFDKs, m_ulBackspaceFDKs, m_ulEnterFDKs, m_ulSoftShiftFK;
	WORD m_wEntryMode, m_wColumns, m_wRows;
	ESHIFT m_eShiftMode;

	ULONG m_DataFKMask, m_DataFDKMask;
	ULONG m_PinFKMask, m_PinFDKMask;
	ULONG m_SecureFKMask, m_SecureFDKMask;

	vector<XFSKEYINFO> m_vFKInfo, m_vFDKInfo;
	CVectorPtr<LPWFSPINFK> m_vDataKeys, m_vPinKeys, m_vSecureKeys;


protected:
	IXFSSPManage *m_pSPManage;
	TCHAR m_szServiceName[MAX_PATH];
};


#endif //_CPRESSKEY_H_20151109
