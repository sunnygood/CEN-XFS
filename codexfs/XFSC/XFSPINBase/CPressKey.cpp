/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CPressKey.cpp
* Description: press key class for PIN
*
* Author:      liuguoxun
* Date:        2015-11-09         
*
*****************************************************************************/

#include "CPressKey.h"
#include "IBaseLog.h"


#define   KEYCODE_DEFINED        _T("KeyConfig\\KeyCode\\Defined")
#define   KEYCODE_USERDEFINED    _T("KeyConfig\\KeyCode\\UserDefined")

#define   LAYOUT_DATA            _T("KeyConfig\\TextEntry\\FK") //_T("Layout\\Data")
#define   LAYOUT_PIN             _T("KeyConfig\\PinEntry\\FK")  //_T("Layout\\Pin")
#define   LAYOUT_SECURE          _T("KeyConfig\\KeyEntry\\FK")  //_T("Layout\\Secure")
#define   SECURE_CONFIG          _T("KeyConfig\\KeyEntry")      //_T("Layout\\Secure\\Config")


CPressKey::CPressKey()
{
	m_pSPManage = NULL;
	m_DataFKMask = m_DataFDKMask = 0;
	m_PinFKMask = m_PinFDKMask = 0;
	m_SecureFKMask = m_SecureFKMask = 0;
	memset(m_szServiceName, 0, sizeof(m_szServiceName));

	m_vFKInfo.clear();
	m_vFDKInfo.clear();	
	m_vDataKeys.clear();
	m_vPinKeys.clear();	
	m_vSecureKeys.clear();
}

CPressKey::~CPressKey()
{
}

void CPressKey::Init(LPCTSTR lpServiceName, IXFSSPManage *p)
{
	m_pSPManage = p;
	TCSCPY_S(m_szServiceName, MAX_PATH, lpServiceName);

	InitKeyCodes();
	InitSecureKeys();
	InitEntryKeys(LAYOUT_DATA, m_vDataKeys, m_DataFKMask, m_DataFDKMask);
	InitEntryKeys(LAYOUT_PIN,  m_vPinKeys,  m_PinFKMask,  m_PinFDKMask);
}

inline void CPressKey::PushFK(LPCTSTR lpPath, LPCTSTR lpName, ULONG ulValue, int nDefaultValue)
{
	XFSKEYINFO stInfo;
	TCSCPY_S(stInfo.szName, MAX_NAME_PRESSKEY, lpName);
	stInfo.ulValue = ulValue;
	stInfo.keyCode = m_pSPManage->GetServiceNodeInt(m_szServiceName, lpPath, lpName, nDefaultValue);
	m_vFKInfo.push_back(stInfo);
}

inline void CPressKey::PushFDK(LPCTSTR lpName, ULONG ulValue, int nDefaultValue)
{
	XFSKEYINFO stInfo;
	TCSCPY_S(stInfo.szName, MAX_NAME_PRESSKEY, lpName);
	stInfo.ulValue = ulValue;
	stInfo.keyCode = m_pSPManage->GetServiceNodeInt(m_szServiceName, KEYCODE_USERDEFINED, lpName, nDefaultValue);
	m_vFDKInfo.push_back(stInfo);
}

void CPressKey::InitKeyCodes()
{
	PushFK(KEYCODE_DEFINED, _T("FK_0"), WFS_PIN_FK_0, 0x30);
	PushFK(KEYCODE_DEFINED, _T("FK_1"), WFS_PIN_FK_1, 0x31);
	PushFK(KEYCODE_DEFINED, _T("FK_2"), WFS_PIN_FK_2, 0x32);
	PushFK(KEYCODE_DEFINED, _T("FK_3"), WFS_PIN_FK_3, 0x33);
	PushFK(KEYCODE_DEFINED, _T("FK_4"), WFS_PIN_FK_4, 0x34);
	PushFK(KEYCODE_DEFINED, _T("FK_5"), WFS_PIN_FK_5, 0x35);
	PushFK(KEYCODE_DEFINED, _T("FK_6"), WFS_PIN_FK_6, 0x36);
	PushFK(KEYCODE_DEFINED, _T("FK_7"), WFS_PIN_FK_7, 0x37);
	PushFK(KEYCODE_DEFINED, _T("FK_8"), WFS_PIN_FK_8, 0x38);
	PushFK(KEYCODE_DEFINED, _T("FK_9"), WFS_PIN_FK_9, 0x39);

	PushFK(KEYCODE_DEFINED, _T("FK_ENTER"),     WFS_PIN_FK_ENTER,     0x0D);
	PushFK(KEYCODE_DEFINED, _T("FK_CANCEL"),    WFS_PIN_FK_CANCEL,    0x1B);
	PushFK(KEYCODE_DEFINED, _T("FK_CLEAR"),     WFS_PIN_FK_CLEAR,     0x0C);
	PushFK(KEYCODE_DEFINED, _T("FK_BACKSPACE"), WFS_PIN_FK_BACKSPACE, 0x08);
	PushFK(KEYCODE_DEFINED, _T("FK_HELP"),      WFS_PIN_FK_HELP,      0x2F); 
	PushFK(KEYCODE_DEFINED, _T("FK_DECPOINT"),  WFS_PIN_FK_DECPOINT,  0x2E);
	PushFK(KEYCODE_DEFINED, _T("FK_00"),        WFS_PIN_FK_00,        0x7F); 
	PushFK(KEYCODE_DEFINED, _T("FK_000"),       WFS_PIN_FK_000,       0x7E);

	PushFK(KEYCODE_USERDEFINED, _T("FK_RES1"), WFS_PIN_FK_RES1, 0x41);
	PushFK(KEYCODE_USERDEFINED, _T("FK_RES2"), WFS_PIN_FK_RES2, 0x42);
	PushFK(KEYCODE_USERDEFINED, _T("FK_RES3"), WFS_PIN_FK_RES3, 0x43);
	PushFK(KEYCODE_USERDEFINED, _T("FK_RES4"), WFS_PIN_FK_RES4, 0x44);
	PushFK(KEYCODE_USERDEFINED, _T("FK_RES5"), WFS_PIN_FK_RES5, 0x45);
	PushFK(KEYCODE_USERDEFINED, _T("FK_RES6"), WFS_PIN_FK_RES6, 0x46);
	PushFK(KEYCODE_USERDEFINED, _T("FK_RES7"), WFS_PIN_FK_RES7, 0x10);
	PushFK(KEYCODE_USERDEFINED, _T("FK_RES8"), WFS_PIN_FK_RES8, 0x00);
	PushFK(KEYCODE_USERDEFINED, _T("FK_OEM1"), WFS_PIN_FK_OEM1, 0x20);
	PushFK(KEYCODE_USERDEFINED, _T("FK_OEM2"), WFS_PIN_FK_OEM2, 0x21);
	PushFK(KEYCODE_USERDEFINED, _T("FK_OEM3"), WFS_PIN_FK_OEM3, 0x22);
	PushFK(KEYCODE_USERDEFINED, _T("FK_OEM4"), WFS_PIN_FK_OEM4, 0x23);
	PushFK(KEYCODE_USERDEFINED, _T("FK_OEM5"), WFS_PIN_FK_OEM5, 0x2A);
	PushFK(KEYCODE_USERDEFINED, _T("FK_OEM6"), WFS_PIN_FK_OEM6, 0x2C);

	PushFDK(_T("FK_FDK01"), WFS_PIN_FK_FDK01, 0x70);
	PushFDK(_T("FK_FDK02"), WFS_PIN_FK_FDK02, 0x71);
	PushFDK(_T("FK_FDK03"), WFS_PIN_FK_FDK03, 0x72);
	PushFDK(_T("FK_FDK04"), WFS_PIN_FK_FDK04, 0x73);
	PushFDK(_T("FK_FDK05"), WFS_PIN_FK_FDK05, 0x74);
	PushFDK(_T("FK_FDK06"), WFS_PIN_FK_FDK06, 0x75);
	PushFDK(_T("FK_FDK07"), WFS_PIN_FK_FDK07, 0x76);
	PushFDK(_T("FK_FDK08"), WFS_PIN_FK_FDK08, 0x77);
	PushFDK(_T("FK_FDK09"), WFS_PIN_FK_FDK09, 0x00);
	PushFDK(_T("FK_FDK10"), WFS_PIN_FK_FDK10, 0x00);
	PushFDK(_T("FK_FDK11"), WFS_PIN_FK_FDK11, 0x00);
	PushFDK(_T("FK_FDK12"), WFS_PIN_FK_FDK12, 0x00);
	PushFDK(_T("FK_FDK13"), WFS_PIN_FK_FDK13, 0x00);
	PushFDK(_T("FK_FDK14"), WFS_PIN_FK_FDK14, 0x00);
	PushFDK(_T("FK_FDK15"), WFS_PIN_FK_FDK15, 0x00);
	PushFDK(_T("FK_FDK16"), WFS_PIN_FK_FDK16, 0x00);
	PushFDK(_T("FK_FDK17"), WFS_PIN_FK_FDK17, 0x00);
	PushFDK(_T("FK_FDK18"), WFS_PIN_FK_FDK18, 0x00);
	PushFDK(_T("FK_FDK19"), WFS_PIN_FK_FDK19, 0x00);
	PushFDK(_T("FK_FDK20"), WFS_PIN_FK_FDK20, 0x00);
	PushFDK(_T("FK_FDK21"), WFS_PIN_FK_FDK21, 0x00);
	PushFDK(_T("FK_FDK22"), WFS_PIN_FK_FDK22, 0x00);
	PushFDK(_T("FK_FDK23"), WFS_PIN_FK_FDK23, 0x00);
	PushFDK(_T("FK_FDK24"), WFS_PIN_FK_FDK24, 0x00);
	PushFDK(_T("FK_FDK25"), WFS_PIN_FK_FDK25, 0x00);
	PushFDK(_T("FK_FDK26"), WFS_PIN_FK_FDK26, 0x00);
	PushFDK(_T("FK_FDK27"), WFS_PIN_FK_FDK27, 0x00);
	PushFDK(_T("FK_FDK28"), WFS_PIN_FK_FDK28, 0x00);
	PushFDK(_T("FK_FDK29"), WFS_PIN_FK_FDK29, 0x00);
	PushFDK(_T("FK_FDK30"), WFS_PIN_FK_FDK30, 0x00);
	PushFDK(_T("FK_FDK31"), WFS_PIN_FK_FDK31, 0x00);
	PushFDK(_T("FK_FDK32"), WFS_PIN_FK_FDK32, 0x00);
}

void CPressKey::InitEntryKeys(LPCTSTR SubKeyName, VKEYPOS &vEntryKeys, ULONG &ulFKMask, ULONG &ulFDKMask)
{
	vector<basic_string<TCHAR>> vValues;
	basic_string<TCHAR> value;
	TCHAR szTemp[MAX_PATH] = {0};
	TCHAR szName[64] = {0};
	
	for(int i = 1; i <= 32; i++)
	{		
		sprintf(szName, "%02d", i);
		memset(szTemp, 0, sizeof(szTemp));
		value = m_pSPManage->GetServiceNodeStr(m_szServiceName, SubKeyName, szName, szTemp, MAX_PATH);
		if(value.length() == 0)
		{
			continue;
		}

		LPWFSPINFK pPos = new WFSPINFK();
		if(NULL == pPos)
		{
			RLog2(LOG_LEVEL_ERROR, "Invalid pointer, SubKeyName=%s szName=%s szTemp=%s", SubKeyName, szName, szTemp);
			break;
		}

		pPos->ulFK = pPos->ulShiftFK = WFS_PIN_FK_UNUSED;
		pPos->usXPos = pPos->usYPos = pPos->usXSize = pPos->usYSize = 0;

		vValues.clear();
		SplitString(value, _T(","), vValues);
		if(vValues.size() == 3) //FDK有三项信息
		{
			pPos->usXPos = ATOI(vValues[1].c_str());
			pPos->usYPos = ATOI(vValues[2].c_str());

			pPos->wKeyType = WFS_PIN_FDK;
			pPos->ulFK = GetFDKs(vValues[0].c_str());

			ulFDKMask |= pPos->ulFK;
		}
		else
		{
			pPos->wKeyType = WFS_PIN_FK;
			pPos->ulFK = GetFKs(vValues[0].c_str());

			ulFKMask |= pPos->ulFK;
		}

		vEntryKeys.push_back(pPos);
	}
}

void CPressKey::InitSecureKeys()
{
	vector<basic_string<TCHAR>> vValues;
	basic_string<TCHAR> value;
	TCHAR szTemp[MAX_PATH] = {0};
	TCHAR szName[64] = {0};

	m_wRows        = m_pSPManage->GetServiceNodeInt(m_szServiceName, SECURE_CONFIG, _T("Rows"), 0);
	m_wColumns     = m_pSPManage->GetServiceNodeInt(m_szServiceName, SECURE_CONFIG, _T("Columns"), 0);

	int nShift = m_pSPManage->GetServiceNodeInt(m_szServiceName, SECURE_CONFIG, _T("ShiftMode"), 0);
	if(nShift == 0x0001)
	{
		m_eShiftMode = SHIFT_ONCE;
	}
	else if(nShift == 0x0002)
	{
		m_eShiftMode = SHIFT_LONG;
	}
	else if(nShift == 0x0003)
	{
		m_eShiftMode = SHIFT_HW;
	}
	else
	{
		m_eShiftMode = SHIFT_NONE;
	}

	value = m_pSPManage->GetServiceNodeStr(m_szServiceName, SECURE_CONFIG, _T("EntryMode"), szTemp, MAX_PATH);
	if(value == _T("WFS_PIN_SECUREKEY_REG_SHIFT"))
	{
		m_wEntryMode = WFS_PIN_SECUREKEY_REG_SHIFT;
	}
	else if(value == _T("WFS_PIN_SECUREKEY_IRREG_SHIFT"))
	{
		m_wEntryMode = WFS_PIN_SECUREKEY_IRREG_SHIFT;
	}	
	else if(value == _T("WFS_PIN_SECUREKEY_REG_UNIQUE"))
	{
		m_wEntryMode = WFS_PIN_SECUREKEY_REG_UNIQUE;
	}	
	else if(value == _T("WFS_PIN_SECUREKEY_IRREG_UNIQUE"))
	{
		m_wEntryMode = WFS_PIN_SECUREKEY_IRREG_UNIQUE;
	}	
	else
	{
		m_wEntryMode = WFS_PIN_SECUREKEY_NOTSUPP;
	}

	//等效键
	m_ulCancelFDKs    = GetFDKs(m_pSPManage->GetServiceNodeStr(m_szServiceName, SECURE_CONFIG, _T("CancelFDKs"), szTemp, MAX_PATH));
	m_ulClearFDKs     = GetFDKs(m_pSPManage->GetServiceNodeStr(m_szServiceName, SECURE_CONFIG, _T("ClearFDKs"), szTemp, MAX_PATH));
	m_ulBackspaceFDKs = GetFDKs(m_pSPManage->GetServiceNodeStr(m_szServiceName, SECURE_CONFIG, _T("BackspaceFDKs"), szTemp, MAX_PATH));
	m_ulEnterFDKs     = GetFDKs(m_pSPManage->GetServiceNodeStr(m_szServiceName, SECURE_CONFIG, _T("EnterFDKs"), szTemp, MAX_PATH));
	m_ulSoftShiftFK   = GetFKs (m_pSPManage->GetServiceNodeStr(m_szServiceName, SECURE_CONFIG, _T("SoftShiftFK"), szTemp, MAX_PATH));

	
	for(int i = 1; i <= 32; i++)
	{
		sprintf(szName, "%02d", i);
		memset(szTemp, 0, sizeof(szTemp));
		value = m_pSPManage->GetServiceNodeStr(m_szServiceName, LAYOUT_SECURE, szName, szTemp, MAX_PATH);
		if(value.length() == 0)
		{
			continue;
		}

		vValues.clear();
		SplitString(value, _T(","), vValues);
		if(vValues.size() <= 0)
		{
			continue;
		}

		LPWFSPINFK pPos = new WFSPINFK();
		if(NULL == pPos)
		{
			return;
		}

		pPos->wKeyType = WFS_PIN_FK;
		pPos->ulFK = pPos->ulShiftFK = WFS_PIN_FK_UNUSED;
		pPos->usXPos = pPos->usYPos = pPos->usXSize = pPos->usYSize = 0;

		pPos->ulFK = GetFKs(vValues[0].c_str());
		if(pPos->ulFK == WFS_PIN_FK_UNUSED)
		{
			pPos->wKeyType = WFS_PIN_FDK;
			pPos->ulFK = GetFDKs(vValues[0].c_str());
		}

		if(pPos->wKeyType == WFS_PIN_FDK)
		{
			if(vValues.size() >= 2)
 			{
 				pPos->usXPos = ATOI(vValues[1].c_str());
 			}

			if(vValues.size() >= 3)
 			{
 				pPos->usYPos = ATOI(vValues[2].c_str());
			}

			m_SecureFDKMask |= pPos->ulFK;
		}
		else
		{
			if(vValues.size() >= 2) 
			{
				pPos->ulShiftFK = GetFKs(vValues[1].c_str());
			}

 			if(vValues.size() >= 3) 
 			{	
				pPos->usXPos = ATOI(vValues[2].c_str());
 			}

 			if(vValues.size() >= 4) 
 			{	
 				pPos->usYPos = ATOI(vValues[3].c_str());	
 			}

 			if(vValues.size() >= 5) 
 			{	
				pPos->usXSize = ATOI(vValues[4].c_str());	
 			}

 			if(vValues.size() >= 6)
 			{	
 				pPos->usYSize = ATOI(vValues[5].c_str());	
 			}

			m_SecureFKMask |= pPos->ulFK;
			m_SecureFKMask |= pPos->ulShiftFK;
		}

		m_vSecureKeys.push_back(pPos);
	}
}

void CPressKey::GetKeyValue(BYTE keyCode, ULONG &FK, ULONG &FDK)
{
	FK = FDK = 0;
	for(int i = 0; i < m_vFKInfo.size(); ++i)
	{
		if(keyCode == m_vFKInfo[i].keyCode)
		{
			FK = m_vFKInfo[i].ulValue;
			break;
		}
	}

	for(int i = 0; i < m_vFDKInfo.size(); ++i)
	{
		if(keyCode == m_vFDKInfo[i].keyCode)
		{
			FDK = m_vFDKInfo[i].ulValue;
			break;
		}
	}
}

inline void CPressKey::GetKeyCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, vector<BYTE> &vCodes, VKEYPOS &vKeys)
{
	vCodes.clear();

	BYTE keyCode = 0;
	for(int i = 0; i < vKeys.size(); ++i)
	{
		if(vKeys.at(i)->wKeyType == WFS_PIN_FDK)
		{
			if(vKeys.at(i)->ulFK & ulActiveFDKs)
			{
				keyCode = GetFDKCode(vKeys.at(i)->ulFK);
				vCodes.push_back(keyCode);
			}
			else
			{
				vCodes.push_back(0x00);
			}
		}
		else
		{
			if(vKeys.at(i)->ulFK & ulActiveFKs)
			{
				keyCode = GetFKCode(vKeys.at(i)->ulFK);
				vCodes.push_back(keyCode);
			}
			else
			{
				vCodes.push_back(0x00);
			}
		}
	}
}

void CPressKey::GetPinCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, vector<BYTE> &vCodes)
{
	GetKeyCodes(ulActiveFKs, ulActiveFDKs, vCodes, m_vPinKeys);
}

void CPressKey::GetDataCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, vector<BYTE> &vCodes)
{
	GetKeyCodes(ulActiveFKs, ulActiveFDKs, vCodes, m_vDataKeys);
}

void CPressKey::GetSecureCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, vector<BYTE> &vCodes)
{
	GetKeyCodes(ulActiveFKs, ulActiveFDKs, vCodes, m_vSecureKeys);
}

inline ULONG CPressKey::GetFDKs(LPCTSTR lpName)
{
	ULONG FDKs = 0;
	basic_string<TCHAR> vSrc(lpName);
	vector<basic_string<TCHAR>> vValues;

	SplitString(vSrc, _T("|"), vValues);
	for(vector<basic_string<TCHAR>>::size_type i = 0; i < vValues.size(); ++i)
	{
		for(VKIIT it = m_vFDKInfo.begin(); it != m_vFDKInfo.end(); ++it)
		{
			if(vValues[i] == it->szName)
			{
				FDKs |= it->ulValue;
				break;
			}
		}
	}

	return FDKs;
}

inline ULONG CPressKey::GetFKs(LPCTSTR lpName)
{
	ULONG FKs = WFS_PIN_FK_UNUSED;
	for(VKIIT it = m_vFKInfo.begin(); it != m_vFKInfo.end(); ++it)
	{
		if(TCSCMP(lpName, it->szName) == 0)
		{
			FKs = it->ulValue;
			break;
		}
	}

	return FKs;
}

inline BOOL CPressKey::GetFDKs(LPCTSTR lpName, ULONG &ulValue, BYTE &keyCode)
{
	for(VKIIT it = m_vFDKInfo.begin(); it != m_vFDKInfo.end(); ++it)
	{
		if(TCSCMP(lpName, it->szName) == 0)
		{
			ulValue = it->ulValue;
			keyCode = it->keyCode;
			return TRUE;
		}
	}

	return FALSE;
}

inline BOOL CPressKey::GetFKs(LPCTSTR lpName, ULONG &ulValue, BYTE &keyCode)
{
	for(VKIIT it = m_vFKInfo.begin(); it != m_vFKInfo.end(); ++it)
	{
		if(TCSCMP(lpName, it->szName) == 0)
		{
			ulValue = it->ulValue;
			keyCode = it->keyCode;
			return TRUE;
		}
	}

	return FALSE;
}

inline BYTE CPressKey::GetFDKCode(ULONG ulValue)
{
	for(VKIIT it = m_vFDKInfo.begin(); it != m_vFDKInfo.end(); ++it)
	{
		if(ulValue == it->ulValue)
		{
			return it->keyCode;
		}
	}

	return 0;
}

inline BYTE CPressKey::GetFKCode(ULONG ulValue)
{
	for(VKIIT it = m_vFKInfo.begin(); it != m_vFKInfo.end(); ++it)
	{
		if(ulValue == it->ulValue)
		{
			return it->keyCode;
		}
	}

	return 0;
}

LPTSTR CPressKey::GetFKsName(ULONG ulActiveFKs, LPTSTR lpOutName)
{
	if(NULL == lpOutName)
	{
		return NULL;
	}

	TCHAR *p = lpOutName;

	for(VKIIT it = m_vFKInfo.begin(); it != m_vFKInfo.end(); ++it)
	{
		if(ulActiveFKs & it->ulValue)
		{
			if(TCSLEN(lpOutName) > 0)
			{
				TCSCAT_S(lpOutName, 512, _T("|"));
			}

			TCSCAT_S(lpOutName, 512, it->szName);
		}
	}

	return p;
}

LPTSTR CPressKey::GetFDKsName(ULONG ulActiveFDKs, LPTSTR lpOutName)
{
	if(NULL == lpOutName)
	{
		return NULL;
	}

	TCHAR *p = lpOutName;

	for(VKIIT it = m_vFDKInfo.begin(); it != m_vFDKInfo.end(); ++it)
	{
		if(ulActiveFDKs & it->ulValue)
		{
			if(TCSLEN(lpOutName) > 0)
			{
				TCSCAT_S(lpOutName, 512, _T("|"));
			}

			TCSCAT_S(lpOutName, 512, it->szName);
		}
	}

	return p;
}

void CPressKey::GetUniqueSecureCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, ULONG ulDisableFKs, vector<BYTE> &vCodes, vector<BYTE> &vDisableCodes)
{
	GetKeyCodes(ulActiveFKs, ulActiveFDKs, vCodes, m_vSecureKeys);

	BYTE keyCode = 0;
	vDisableCodes.clear();
	for(int i = 0; i < m_vSecureKeys.size(); ++i)
	{
		if(m_vSecureKeys.at(i)->ulFK & ulDisableFKs)
		{
			keyCode = 0x00;
		}
		else
		{
			keyCode = vCodes[i];
		}

		vDisableCodes.push_back(keyCode);
	}
}

void CPressKey::GetHardShiftSecureCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, ULONG ulDisableFKs, vector<BYTE> &vCodes, vector<BYTE> &vDisableCodes)
{
	GetUniqueSecureCodes(ulActiveFKs, ulActiveFDKs, ulDisableFKs, vCodes, vDisableCodes);
}

void CPressKey::GetSoftShiftSecureCodes(ULONG ulActiveFKs, ULONG ulActiveFDKs, ULONG ulDisableFKs, vector<BYTE> &vCodes, vector<BYTE> &vDisableCodes, vector<BYTE> &vShiftCodes, vector<BYTE> &vDisableShiftCodes)
{
	GetUniqueSecureCodes(ulActiveFKs, ulActiveFDKs, ulDisableFKs, vCodes, vDisableCodes);

	BYTE keyCode = 0;
	vShiftCodes.clear();
	vDisableShiftCodes.clear();
	
	for(int i = 0; i < m_vSecureKeys.size(); ++i)
	{
		if(m_vSecureKeys.at(i)->wKeyType == WFS_PIN_FDK)
		{
			if(m_vSecureKeys.at(i)->ulFK & ulActiveFDKs)
			{
				keyCode = GetFDKCode(m_vSecureKeys.at(i)->ulFK);
				vShiftCodes.push_back(keyCode);
			}
			else
			{
				vShiftCodes.push_back(0x00);
			}
		}
		else
		{
			if(m_vSecureKeys.at(i)->ulShiftFK & ulActiveFKs)
			{
				keyCode = GetFKCode(m_vSecureKeys.at(i)->ulShiftFK);
				vShiftCodes.push_back(keyCode);
			}
			else
			{
				vShiftCodes.push_back(0x00);
			}
		}
	}

	for(int i = 0; i < m_vSecureKeys.size(); ++i)
	{
		if(m_vSecureKeys.at(i)->wKeyType == WFS_PIN_FDK)
		{
			if(m_vSecureKeys.at(i)->ulFK & ulActiveFDKs)
			{
				keyCode = GetFDKCode(m_vSecureKeys.at(i)->ulFK);
				vDisableShiftCodes.push_back(keyCode);
			}
			else
			{
				vDisableShiftCodes.push_back(0x00);
			}
		}
		else
		{
			if(m_vSecureKeys.at(i)->ulShiftFK & ulDisableFKs)
			{
				vDisableShiftCodes.push_back(0x00);
			}
			if(m_vSecureKeys.at(i)->ulShiftFK & ulActiveFKs)
			{
				keyCode = GetFKCode(m_vSecureKeys.at(i)->ulShiftFK);
				vDisableShiftCodes.push_back(keyCode);
			}
			else
			{
				vDisableShiftCodes.push_back(0x00);
			}
		}
	}
}

