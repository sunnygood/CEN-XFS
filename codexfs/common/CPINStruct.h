/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CPINStruct.h
* Description: struct for PIN
*
* Author:      liuguoxun
* Date:        2015-11-09         
*
*****************************************************************************/

#ifndef _CPINSTRUCT_H_20151109
#define _CPINSTRUCT_H_20151109

//#pragma warning(disable : 4786 4251)

#include <CSTLUnit.h>
#include <CENXFSPIN.h>
#include <CENXFSPINEx.h>

/********************************************************************************/
class CWFSXDATA : public WFSXDATA
{
public:
	CWFSXDATA() 
	{ 
		Clear();
	}

	~CWFSXDATA() 
	{ 
		Clear(); 
	}
	
	void Clear()
	{
		usLength = 0;
		lpbData  = NULL;
		m_pBuffer.Clear();
	}

	LPBYTE GetBufferSetLength(USHORT usLen)
	{
		usLength = 0;
		lpbData  = NULL;

		return m_pBuffer.GetBufferSetLength(usLen);
	}

	void ReleaseBufferSetLength(USHORT usLen)
	{
		if(usLen > 0)
		{
			lpbData = (LPBYTE)m_pBuffer;
			usLength = usLen;
		}
		else
		{
			usLength = 0;
			lpbData  = NULL;
		}
	}
	
	void Set(LPBYTE lpData, USHORT usLen)
	{
		if(NULL != lpData && usLen > 0)
		{
			m_pBuffer.Set(lpData, usLen);

			usLength = usLen;
			lpbData = (LPBYTE)m_pBuffer;
		}
		else
		{
			usLength = 0;
			lpbData  = NULL;
		}
	}

	operator LPWFSXDATA()
	{
		return this;
	}

protected:
	CDataBuffer<BYTE> m_pBuffer;
};

/********************************************************************************/

class CWFSPINSTATUS : public WFSPINSTATUS
{
public:
	CWFSPINSTATUS()
	{
		Clear();
	}

	~CWFSPINSTATUS()
	{
		Clear();
	}

	void Clear()
	{
 		fwDevice = WFS_PIN_DEVOFFLINE;
		fwEncStat = WFS_PIN_ENCUNDEFINED;
		lpszExtra = NULL;
		memset(dwGuidLights, WFS_PIN_GUIDANCE_NOT_AVAILABLE, sizeof(dwGuidLights));
		fwAutoBeepMode = WFS_PIN_BEEP_ON_INACTIVE;
		dwCertificateState = WFS_PIN_CERT_UNKNOWN;
		wDevicePosition = WFS_PIN_DEVICEPOSNOTSUPP;
		usPowerSaveRecoveryTime = 0;
		wAntiFraudModule = WFS_PIN_AFMNOTSUPP;
		//m_Extra.Clear();
	}

	/*void AddExtra(std::string name, std::string value)
	{
		m_Extra.Add(name, value);
	}*/

	operator LPWFSPINSTATUS()
	{ 
		//lpszExtra = (LPSTR)m_Extra;
		return this;
	}

//protected:
//	CMap<CStringZ<TCHAR>, CStringZ<TCHAR>> m_Extra;
};

class CWFSPINCAPS : public WFSPINCAPS
{
public:
	CWFSPINCAPS()
	{
		Clear();
	}

	~CWFSPINCAPS()
	{
		Clear();
	}

	void Clear()
	{
		wClass = WFS_SERVICE_CLASS_PIN;
		fwType = 0;
		bCompound = FALSE;
		usKeyNum = 0;
		fwAlgorithms = 0;
		fwPinFormats = 0;
		fwDerivationAlgorithms = 0;
		fwPresentationAlgorithms = 0;
		fwDisplay = 0;
		bIDConnect = FALSE;
		fwIDKey = 0;
		fwValidationAlgorithms = 0;
		fwKeyCheckModes = 0;
		lpszExtra = NULL;
		memset(dwGuidLights, 0x00, sizeof(dwGuidLights));
		bPINCanPersistAfterUse = FALSE;
		fwAutoBeep = 0;
		lpsHSMVendor = NULL;
		bHSMJournaling = FALSE;
		dwRSAAuthenticationScheme = 0;
		dwRSASignatureAlgorithm = 0;
		dwRSACryptAlgorithm = 0;
		dwRSAKeyCheckMode = 0;
		dwSignatureScheme = 0;
		lpwEMVImportSchemes = NULL;
		fwEMVHashAlgorithm = 0;
		bKeyImportThroughParts = FALSE;
		fwENCIOProtocols = 0;
		bTypeCombined = FALSE;
		bSetPinblockDataRequired = FALSE;
		fwKeyBlockImportFormats = 0;
		bPowerSaveControl = FALSE;
		bAntiFraudModule = FALSE;

		wDESKeyLength = WFS_PIN_KEYSINGLE | WFS_PIN_KEYDOUBLE | WFS_PIN_KEYTRIPLE;
		wCertificateTypes = WFS_PIN_PUBLICENCKEY | WFS_PIN_PUBLICVERIFICATIONKEY | WFS_PIN_PUBLICHOSTKEY;
		lppLoadCertOptions = NULL;
		dwCRKLLoadOptions = WFS_PIN_CRKLLOAD_NORANDOM;
		lpETSCaps = NULL;
		lpdwSynchronizableCommands = NULL;
		lppRestrictedKeyEncKeySupport = NULL;
		
 		//m_Extra.Clear();
		m_vEMVImpScheme.clear();
	}	

	void AddEMVImpScheme(WORD wScheme)
	{
		m_vEMVImpScheme.push_back(wScheme);
	}

	void AddSyncCmd(DWORD dwCmd)
	{
		m_vSyncCmds.push_back(dwCmd);
	}

	/*void AddExtra(std::string name, std::string value)
	{
		m_Extra.Add(name, value);
	}*/

	operator LPWFSPINCAPS()
	{
		if(m_vEMVImpScheme.empty())
		{
			lpwEMVImportSchemes = NULL;
		}
		else
		{
			m_vEMVImpScheme.push_back(0);
			lpwEMVImportSchemes = (LPWORD)m_vEMVImpScheme;
		}

		if(m_vSyncCmds.empty())
		{
			lpdwSynchronizableCommands = NULL;
		}
		else
		{
			m_vSyncCmds.push_back(0);
			lpdwSynchronizableCommands = (LPDWORD)m_vSyncCmds;
		}

		//lpszExtra = (LPSTR)m_Extra;
		return this;
	}


protected:
	//CMap<std::string, std::string> m_Extra;
	CVectorValue<WORD> m_vEMVImpScheme;
	CVectorValue<DWORD> m_vSyncCmds;
};

//KeyDetail
class CWFSPINKEYDETAIL : public WFSPINKEYDETAIL
{
public:
	CWFSPINKEYDETAIL() 
	{ 
		Clear(); 
	}

	~CWFSPINKEYDETAIL() 
	{ 
		Clear(); 
	}

	void Clear()
	{
		lpsKeyName = NULL;
		fwUse = 0;
		bLoaded = FALSE;
		lpxKeyBlockHeader = NULL;
		m_KeyName.Clear(); 
		m_KeyBlockHeader.Clear();
	}

	void Set(LPCSTR szKeyName, WORD wUse, BOOL bLoaded, LPBYTE lpbData = NULL, USHORT usLength = 0)
	{
		m_KeyName.Set(szKeyName); 
		lpsKeyName = (LPSTR)m_KeyName;
		fwUse = wUse;
		this->bLoaded = bLoaded;
		m_KeyBlockHeader.Set(lpbData, usLength);
		lpxKeyBlockHeader = (LPWFSXDATA)m_KeyBlockHeader;
	}

	operator LPWFSPINKEYDETAIL()
	{
		return this;
	}

protected:
	CStringZ<char> m_KeyName;
	CWFSXDATA   m_KeyBlockHeader;
};

class CWFSPINKEYDETAILWRAPPER
{
public:
	CWFSPINKEYDETAILWRAPPER()
	{
		Clear();
	}
	
	~CWFSPINKEYDETAILWRAPPER()
	{
		Clear();
	}

	void Clear()
	{
		for(std::vector<CWFSPINKEYDETAIL*>::iterator it = v1.begin(); it != v1.end(); ++it)
		{
			delete (*it);
		}
		v1.clear();
	}

	void Add(LPCSTR szKeyName, WORD wUse, BOOL bLoaded, LPBYTE lpbData = NULL, USHORT usLength = 0)
	{
		CWFSPINKEYDETAIL *p = new CWFSPINKEYDETAIL();
		if(NULL != p)
		{
			p->Set(szKeyName, wUse, bLoaded, lpbData, usLength);
			v1.push_back(p);
		}
	}

	operator LPWFSPINKEYDETAIL*()
	{
		if(v1.size() <= 0)
		{
			return NULL;
		}
		else
		{
			v1.push_back(NULL);
			return (LPWFSPINKEYDETAIL*)(v1.data());
		}
	}
protected:
	std::vector<CWFSPINKEYDETAIL*> v1;
};

//FuncKeyDetail
class CWFSPINFUNCKEYDETAIL : public WFSPINFUNCKEYDETAIL
{
public:
	CWFSPINFUNCKEYDETAIL()
	{
		Clear();
	}

	void Clear()
	{
		ulFuncMask = 0;
		usNumberFDKs = 0;
		lppFDKs = NULL;
		v1.Clear();
	}

	void AddFDK(ULONG ulFDK, USHORT usXPosition, USHORT usYPosition)
	{
 		LPWFSPINFDK p = new WFSPINFDK();
		if(NULL != p)
		{
			p->ulFDK = ulFDK;
			p->usXPosition = usXPosition;
			p->usYPosition = usYPosition;
			v1.push_back(p);
		}
	}

	void Set(ULONG ulFuncMask)
	{
		this->ulFuncMask = ulFuncMask;
	}

	operator LPWFSPINFUNCKEYDETAIL()
	{
		usNumberFDKs = v1.size();
		if(usNumberFDKs <= 0)
			lppFDKs = NULL;
		else
			lppFDKs = (LPWFSPINFDK*)(v1.data());

		return this;
	}

protected:
	CVectorPtr<LPWFSPINFDK> v1;
};

//GetKeyDetailEx
class CWFSPINKEYDETAILEX : public WFSPINKEYDETAILEX
{
public:
	CWFSPINKEYDETAILEX()
	{
		Clear();
	}

	~CWFSPINKEYDETAILEX()
	{
		Clear();
	}

	void Clear()
	{
		lpsKeyName = NULL;
		dwUse = 0;
		bGeneration = 0;
		bVersion = 0;
		memset(bActivatingDate, 0x00, sizeof(bActivatingDate));
		memset(bExpiryDate, 0x00, sizeof(bExpiryDate));
		bLoaded = FALSE;
		lpxKeyBlockHeader = NULL;
		m_KeyName.Clear();
		m_KeyBlockHeader.Clear();
	}

	void Set(LPCSTR szKeyName, DWORD dwUse, BOOL bLoaded = TRUE, LPBYTE lpbData = NULL, USHORT usLength = 0, 
		     BYTE bGeneration = 0xFF, BYTE bVersion = 0xFF, LPBYTE bActivatingDate = NULL, LPBYTE bExpiryDate = NULL)
	{
		this->dwUse = dwUse;
		this->bGeneration = bGeneration;
		this->bVersion = bVersion;
		if (bActivatingDate)
			memcpy(this->bActivatingDate, bActivatingDate, sizeof(this->bActivatingDate));
		if (bExpiryDate)
			memcpy(this->bExpiryDate, bExpiryDate, sizeof(this->bExpiryDate));
		this->bLoaded = bLoaded;
		m_KeyName.Set(szKeyName); 
		lpsKeyName = (LPSTR)m_KeyName;
		m_KeyBlockHeader.Set(lpbData, usLength);
		lpxKeyBlockHeader = (LPWFSXDATA)m_KeyBlockHeader;
	}

	operator LPWFSPINKEYDETAILEX()
	{
		return this;
	}
	
protected:
	CStringZ<char> m_KeyName;
	CWFSXDATA   m_KeyBlockHeader;
};

class CWFSPINKEYDETAILEXWRAPPER
{
public:
	CWFSPINKEYDETAILEXWRAPPER()
	{
		Clear();
	}
	
	~CWFSPINKEYDETAILEXWRAPPER()
	{
		Clear();
	}

	void Clear()
	{
		for(std::vector<CWFSPINKEYDETAILEX*>::iterator it = v1.begin(); it != v1.end(); it++)
		{
			delete (*it);
		}
		v1.clear();
	}

	void Add(LPCSTR szKeyName, DWORD dwUse, BOOL bLoaded = TRUE, LPBYTE lpbData = NULL, USHORT usLength = 0, 
		     BYTE bGeneration = 0xFF, BYTE bVersion = 0xFF, LPBYTE bActivatingDate = NULL, LPBYTE bExpiryDate = NULL)
	{
		CWFSPINKEYDETAILEX *p = new CWFSPINKEYDETAILEX;
		p->Set(szKeyName, dwUse, bLoaded, lpbData, usLength, bGeneration, bVersion, bActivatingDate, bExpiryDate);
		v1.push_back(p);
	}

	operator LPWFSPINKEYDETAILEX*()
	{
		if(v1.size() <= 0)
			return NULL;
		else
		{
			v1.push_back(NULL);
			return (LPWFSPINKEYDETAILEX*)(v1.data());
		}
	}
protected:
	std::vector<CWFSPINKEYDETAILEX*> v1;
};

class CWFSPINSECUREKEYDETAIL : public WFSPINSECUREKEYDETAIL
{
public:
	CWFSPINSECUREKEYDETAIL()
	{
		Clear();
	}	

	~CWFSPINSECUREKEYDETAIL()
	{
		Clear();
	}	

	void Clear()
	{
		fwKeyEntryMode = 0;	
		lpFuncKeyDetail = NULL;
		ulClearFDK = 0;
		ulCancelFDK = 0;
		ulBackspaceFDK = 0;
		ulEnterFDK = 0;
		wColumns = 0;
		wRows = 0;
		lppHexKeys = NULL;
		m_FunckKeyDetail.Clear();
		for(std::vector<LPWFSPINHEXKEYS>::iterator it = v1.begin(); it != v1.end(); ++it)
 		{
 			delete(*it);
 		}
		v1.clear();
	}

	void AddFDK(ULONG ulFDK, USHORT usXPosition, USHORT usYPosition)
	{
		m_FunckKeyDetail.AddFDK(ulFDK, usXPosition, usYPosition);
	}	

	void AddHexKey(USHORT usXPos, USHORT usYPos, USHORT usXSize, USHORT usYSize, ULONG ulFK, ULONG ulShiftFK)
	{
		LPWFSPINHEXKEYS p = new WFSPINHEXKEYS();
		if(NULL != p)
		{
			p->usXPos = usXPos;
			p->usYPos = usYPos;
			p->usXSize = usXSize;
			p->usYSize = usYSize;
			p->ulFK = ulFK;
			p->ulShiftFK = ulShiftFK;
			v1.push_back(p);
		}		
	}

	void Set(WORD wKeyEntryMode, ULONG ulFuncMask, ULONG ulClearFDK, ULONG ulCancelFDK, ULONG ulBackspaceFDK, ULONG ulEnterFDK, WORD wColumns, WORD wRows)
	{
		this->fwKeyEntryMode = wKeyEntryMode;
		this->ulClearFDK = ulClearFDK;
		this->ulCancelFDK = ulCancelFDK;
		this->ulBackspaceFDK = ulBackspaceFDK;
		this->ulEnterFDK = ulEnterFDK;
		this->wColumns = wColumns;
		this->wRows = wRows;

		m_FunckKeyDetail.Set(ulFuncMask);
	}

	operator LPWFSPINSECUREKEYDETAIL()
	{
		lpFuncKeyDetail = (LPWFSPINFUNCKEYDETAIL)m_FunckKeyDetail;
		
		if(v1.empty())
		{
			lppHexKeys = NULL;
		}
		else
		{
			v1.push_back(NULL);
			lppHexKeys = (LPWFSPINHEXKEYS*)(v1.data());
		}	
		return this;
	}

protected:
	CWFSPINFUNCKEYDETAIL	m_FunckKeyDetail;
	std::vector<LPWFSPINHEXKEYS> v1;
};

class CWFSPINHSMINFO : public WFSPINHSMINFO
{
public:
	CWFSPINHSMINFO()
	{
		Clear();
	}	
	~CWFSPINHSMINFO()
	{
		Clear();
	}

	void Clear()
	{
		wHSMSerialNumber = 0;
		lpsZKAID = NULL;
		m_Buffer.Clear();
	}	
	
	void Set(WORD wHSMSerialNumber, LPSTR lpsZKAID)
	{
		this->wHSMSerialNumber = wHSMSerialNumber;
		m_Buffer.Set(lpsZKAID);
		this->lpsZKAID = (LPSTR)m_Buffer;
	}	

protected:
	CStringZ<char> m_Buffer;
};

class CWFSPINHSMDETAIL : public WFSPINHSMDETAIL
{
public:
	CWFSPINHSMDETAIL()
	{	
		Clear();
	}	

	~CWFSPINHSMDETAIL()
	{
		Clear();
	}	

	void Clear()
	{
		wActiveLogicalHSM = 0;
		lppHSMInfo = NULL;
		for(std::vector<CWFSPINHSMINFO*>::iterator it = v1.begin(); it != v1.end(); ++it)
		{
			delete(*it);
		}
		v1.clear();
	}

	void AddHsmInfo(WORD wHSMSerialNumber, LPSTR lpsZKAID)
	{
		CWFSPINHSMINFO *p = new CWFSPINHSMINFO;
		p->Set(wHSMSerialNumber, lpsZKAID);
		v1.push_back(p);
	}	

	void Set(WORD wActiveLogicalHSM)
	{
		this->wActiveLogicalHSM = wActiveLogicalHSM;
	}

	operator LPWFSPINHSMDETAIL()
	{
		if(v1.size() <= 0)
			lppHSMInfo = NULL;
		else
		{
			v1.push_back(NULL);
			lppHSMInfo = (LPWFSPINHSMINFO*)(v1.data());
		}	
		return this;
	}

protected:
	std::vector<CWFSPINHSMINFO*> v1;
};

class CWFSPINPCIPTSDEVICEID : public WFSPINPCIPTSDEVICEID
{
public:
	CWFSPINPCIPTSDEVICEID()
	{
		Clear();
	}

	~CWFSPINPCIPTSDEVICEID()
	{
		Clear();
	}

	void Clear()
	{
		lpszManufacturerIdentifier = NULL;
		lpszModelIdentifier = NULL;
		lpszHardwareIdentifier = NULL;
		lpszFirmwareIdentifier = NULL;
		lpszApplicationIdentifier = NULL;
		m_Buffer1.Clear();
		m_Buffer2.Clear();
		m_Buffer3.Clear();
		m_Buffer4.Clear();
		m_Buffer5.Clear();
	}
	
	void Set(LPSTR lpszManufacturerIdentifier, LPSTR lpszModelIdentifier, LPSTR lpszHardwareIdentifier, LPSTR lpszFirmwareIdentifier, LPSTR lpszApplicationIdentifier)
	{
		m_Buffer1.Set(lpszManufacturerIdentifier);
		m_Buffer2.Set(lpszModelIdentifier);
		m_Buffer3.Set(lpszHardwareIdentifier);
		m_Buffer4.Set(lpszFirmwareIdentifier);
		m_Buffer5.Set(lpszApplicationIdentifier);
	}	

	operator LPWFSPINPCIPTSDEVICEID()
	{
		lpszManufacturerIdentifier = (LPSTR)m_Buffer1;
		lpszModelIdentifier = (LPSTR)m_Buffer2;
		lpszHardwareIdentifier = (LPSTR)m_Buffer3;
		lpszFirmwareIdentifier = (LPSTR)m_Buffer4;
		lpszApplicationIdentifier = (LPSTR)m_Buffer5;
		return this;
	}	

protected:
	CStringZ<char> m_Buffer1, m_Buffer2, m_Buffer3, m_Buffer4, m_Buffer5;
};

class CWFSPINFRAME : public WFSPINFRAME
{
public:
	CWFSPINFRAME()
	{
		Clear();
	}
	
	~CWFSPINFRAME()
	{
		Clear();
	}

	void Clear()
	{
		usFrameXPos = 0;
		usFrameYPos = 0;
		usFrameXSize = 0;
		usFrameYSize = 0;
		wFloatAction = 0;
		lppFKs = NULL;
	}

	void Set(USHORT usFrameXPos, USHORT usFrameYPos, USHORT usFrameXSize, USHORT usFrameYSize, WORD wFloatAction, LPWFSPINFK *lppFKs)
	{
		this->usFrameXPos = usFrameXPos;
		this->usFrameYPos = usFrameYPos;
		this->usFrameXSize = usFrameXSize;
		this->usFrameYSize = usFrameYSize;
		this->wFloatAction = wFloatAction;
		this->lppFKs = lppFKs;
	}

	operator LPWFSPINFRAME()
	{
		return this;
	}
};

class CWFSPINLAYOUT : public WFSPINLAYOUT
{
public:
	CWFSPINLAYOUT()
	{
		Clear();
	}
	
	~CWFSPINLAYOUT()
	{
		Clear();
	}

	void Clear()
	{
		dwEntryMode = 0;
		usNumberOfFrames = 0;
		lppFrames = NULL;
		vFrame.Clear();
	}

	void Add(USHORT usFrameXPos, USHORT usFrameYPos, USHORT usFrameXSize, USHORT usFrameYSize, WORD wFloatAction, LPWFSPINFK *lppFKs)
	{
		CWFSPINFRAME *p = new CWFSPINFRAME();
		if(NULL != p)
		{
			usNumberOfFrames++;
			p->Set(usFrameXPos, usFrameYPos, usFrameXSize, usFrameYSize, wFloatAction, lppFKs);
			vFrame.push_back(p);
		}
	}

	void Set(DWORD dwEntryMode)
	{
		this->dwEntryMode = dwEntryMode;
	}

	operator LPWFSPINLAYOUT()
	{
		if(vFrame.empty())
		{
			lppFrames = NULL;
		}
		else
		{
			vFrame.push_back(NULL);
			lppFrames = (LPWFSPINFRAME*)vFrame.data();
		}

		return this;
	}

protected:
	CVectorPtr<CWFSPINFRAME*> vFrame;
};

class CWFSPINLAYOUTWRAPPER
{
public:
	CWFSPINLAYOUTWRAPPER()
	{
		Clear();
	}
	
	~CWFSPINLAYOUTWRAPPER()
	{
		Clear();
	}

	void Clear()
	{
		vLayout.Clear();
	}

	void Add(DWORD dwEntryMode, USHORT usNumberOfFrames, LPWFSPINFRAME *lpFrames)
	{
		CWFSPINLAYOUT  *p = new CWFSPINLAYOUT();
		if(NULL != p)
		{			
			for(USHORT i = 0; i < usNumberOfFrames; i++)
			{
				p->Add(lpFrames[i]->usFrameXPos, lpFrames[i]->usFrameYPos, lpFrames[i]->usFrameXSize, lpFrames[i]->usFrameYSize, lpFrames[i]->wFloatAction, lpFrames[i]->lppFKs);
			}

			p->Set(dwEntryMode);
			vLayout.push_back(p);
		}
	}

	void Add(DWORD dwEntryMode, USHORT usFrameXPos, USHORT usFrameYPos, USHORT usFrameXSize, USHORT usFrameYSize, WORD wFloatAction, LPWFSPINFK *lppFKs)
	{
		CWFSPINLAYOUT  *p = new CWFSPINLAYOUT();
		if(NULL != p)
		{			
			p->Add(usFrameXPos, usFrameYPos, usFrameXSize, usFrameYSize, wFloatAction, lppFKs);
			p->Set(dwEntryMode);
			vLayout.push_back(p);
		}
	}

	operator LPWFSPINLAYOUT*()
	{
		if(vLayout.empty())
		{
			return NULL;
		}
		else
		{
			vLayout.push_back(NULL);
			return (LPWFSPINLAYOUT*)vLayout.data();
		}
	}

protected:
	CVectorPtr<CWFSPINLAYOUT*> vLayout;
};

/********************************************************************************/

class CWFSPINENCIO : public WFSPINENCIO
{
public:
	CWFSPINENCIO()
	{
		Clear();
	}
	
	~CWFSPINENCIO()
	{
		Clear();
	}
	
	void Clear()
	{
		wProtocol	 = 0;
		ulDataLength = 0;
		lpvData		 = NULL;
		m_pBuffer.Clear();
	}
	
	void Set(WORD wProtocol, LPVOID lpvData, ULONG ulDataLength)
	{
		m_pBuffer.Set((LPBYTE)lpvData, ulDataLength);
		
		this->wProtocol	= wProtocol;
		this->lpvData	= (LPVOID)(LPBYTE)m_pBuffer;
		this->ulDataLength	= ulDataLength;
	}
	
	operator LPWFSPINENCIO()
	{
		return this;
	}

protected:
	CDataBuffer<BYTE> m_pBuffer;
};

class CWFSPINDATA : public WFSPINDATA
{
public:
	CWFSPINDATA()
	{
		Clear();
	}

	~CWFSPINDATA()
	{
		Clear();
	}
	
	void Clear()
	{
		usKeys = 0;
		lpPinKeys = NULL;
		wCompletion = 0;
		for(std::vector<LPWFSPINKEY>::iterator it = v1.begin(); it != v1.end(); it++)
		{
			delete (*it);
		}
		v1.clear();	
	}

	void DeleteEnd()
	{
		ULONG nLen;
		nLen = v1.size();
		if (nLen <= 0)
			return;
		delete v1[nLen - 1];
		v1.pop_back();
	}

	void Add(WORD wCompletion, ULONG ulDigit)
	{
		LPWFSPINKEY p = new WFSPINKEY;
		p->ulDigit = ulDigit;
		p->wCompletion = wCompletion;
		v1.push_back(p);
	}

	ULONG GetCount()
	{
		return v1.size();
	}
	
	void Set(WORD wCompletion)
	{
		this->wCompletion = wCompletion;
	}	
	
	operator LPWFSPINDATA()
	{
		lpPinKeys = NULL;	
		usKeys = v1.size();
		if(usKeys > 0)
			lpPinKeys = (LPWFSPINKEY*)(v1.data());
		return this;
	}

private:
	std::vector<LPWFSPINKEY> v1;
};

class CWFSPINENTRY : public WFSPINENTRY
{
public:
	CWFSPINENTRY()
	{
		usDigits = 0;
		wCompletion = 0;
	}

	~CWFSPINENTRY()
	{
	}
	
	void Set(USHORT usDigits, WORD wCompletion)
	{
		this->usDigits = usDigits;
		this->wCompletion = wCompletion;
	}	
	
	operator LPWFSPINENTRY()
	{
		return this;
	}
};

class CWFSPINBANKSYSIO : public WFSPINBANKSYSIO
{
public:
	CWFSPINBANKSYSIO()
	{
		Clear();
	}

	~CWFSPINBANKSYSIO()
	{
		Clear();
	}
	
	void Clear()
	{
		ulLength = 0;
		lpbData  = NULL;
		m_pBuffer.Clear();
	}
	
	void Set(LPBYTE lpbData, ULONG usLength)
	{
		if(lpbData != NULL && usLength > 0)
		{
			m_pBuffer.Set(lpbData, usLength);
			this->lpbData = (LPBYTE)m_pBuffer;
			this->ulLength = usLength;
		}
	}
	
	operator LPWFSPINBANKSYSIO()
	{
		return this;
	}
protected:
	CDataBuffer<BYTE> m_pBuffer;
};

class CWFSPINPRESENTRESULT : public WFSPINPRESENTRESULT
{
public:
	CWFSPINPRESENTRESULT()
	{
		Clear();
	}	
	~CWFSPINPRESENTRESULT()
	{
		Clear();
	}
	
	void Clear()
	{
		wChipProtocol = 0;
		ulChipDataLength = 0;
		lpbChipData = NULL;
		m_Buffer.Clear();
	}	
	
	void Set(WORD wChipProtocol, LPBYTE lpbChipData, ULONG ulChipDataLength)
	{
		this->wChipProtocol = wChipProtocol;
		m_Buffer.Set(lpbChipData, ulChipDataLength);
		this->lpbChipData = (LPBYTE)m_Buffer;
		this->ulChipDataLength = ulChipDataLength;
	}
	
	operator LPWFSPINPRESENTRESULT()
	{
		return this;
	}
	
protected:
	CDataBuffer<BYTE> m_Buffer;
};

class CWFSPINSECMSG : public WFSPINSECMSG
{
public:
	CWFSPINSECMSG()
	{
		Clear();
	}

	~CWFSPINSECMSG()
	{
		Clear();
	}

	void Clear()
	{
		wProtocol	= 0;
		ulLength	= 0;
		lpbMsg		= NULL;
		m_pBuffer.Clear();
	}

	void Set(WORD wProtocol, LPBYTE lpbMsg, ULONG ulLength)
	{
		m_pBuffer.Set(lpbMsg, ulLength);
		this->wProtocol		= wProtocol;
		this->lpbMsg			= (LPBYTE)m_pBuffer;
		this->ulLength		= ulLength;
	}
	operator LPWFSPINSECMSG()
	{
		return this;
	}

protected:
	CDataBuffer<BYTE> m_pBuffer;
};


//GetData
class CWFSPINKEY : public WFSPINKEY
{
public:
	CWFSPINKEY()
	{
		wCompletion = 0;
		ulDigit = 0;
	}

	~CWFSPINKEY()
	{
	}

	void Set(WORD Completion, ULONG Digit)
	{
		wCompletion = Completion;
		ulDigit = Digit;
	}

	operator LPWFSPINKEY()
	{
		return this;
	}	
};

class CWFSPINSECUREKEYENTRYOUT : public WFSPINSECUREKEYENTRYOUT
{
public:
	CWFSPINSECUREKEYENTRYOUT()
	{
		Clear();
	}
	
	~CWFSPINSECUREKEYENTRYOUT()
	{
		Clear();
	}

	void Clear()
	{
		usDigits	= 0;
		wCompletion = 0;
		lpxKCV = NULL;
		m_xData.Clear();
	}

	void Set(USHORT usDigits, WORD wCompletion, LPBYTE lpbData, USHORT usLength)
	{
		this->usDigits = usDigits;
		this->wCompletion = wCompletion;
		m_xData.Set(lpbData, usLength);
		lpxKCV = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINSECUREKEYENTRYOUT()
	{
		return this;
	}

protected:
	CWFSXDATA  m_xData;
};

class CWFSPINKCV : public WFSPINKCV
{
public:
	CWFSPINKCV()
	{
		Clear();
	}
	
	~CWFSPINKCV()
	{
		Clear();
	}

	void Clear()
	{
		lpxKCV = NULL;
		m_xData.Clear();
	}

	void Set(LPBYTE lpbData, USHORT usLength)
	{
		m_xData.Set(lpbData, usLength);
		lpxKCV = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINKCV()
	{
		return this;
	}

protected:
	CWFSXDATA  m_xData;
};

class CWFSPINSTARTKEYEXCHANGE : public WFSPINSTARTKEYEXCHANGE
{
public:
	CWFSPINSTARTKEYEXCHANGE()
	{
		Clear();
	}
	
	~CWFSPINSTARTKEYEXCHANGE()
	{
		Clear();
	}

	void Clear()
	{
		lpxRandomItem = NULL;
		m_xData.Clear();
	}

	void Set(LPBYTE lpbData, USHORT usLength)
	{
		m_xData.Set(lpbData, usLength);
		lpxRandomItem = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINSTARTKEYEXCHANGE()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xData;
};

class CWFSPINIMPORTRSAPUBLICKEYOUTPUT : public WFSPINIMPORTRSAPUBLICKEYOUTPUT
{
public:
	CWFSPINIMPORTRSAPUBLICKEYOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINIMPORTRSAPUBLICKEYOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		dwRSAKeyCheckMode = 0;
		lpxKeyCheckValue = NULL;
		m_xData.Clear();
	}

	void Set(DWORD dwRSAKeyCheckMode, LPBYTE lpbData, USHORT usLength)
	{
		this->dwRSAKeyCheckMode = dwRSAKeyCheckMode;
		m_xData.Set(lpbData, usLength);
		lpxKeyCheckValue = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINIMPORTRSAPUBLICKEYOUTPUT()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xData;
};

class CWFSPINEXPORTRSAISSUERSIGNEDITEMOUTPUT : public WFSPINEXPORTRSAISSUERSIGNEDITEMOUTPUT
{
public:
	CWFSPINEXPORTRSAISSUERSIGNEDITEMOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINEXPORTRSAISSUERSIGNEDITEMOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		lpxValue = NULL;
		dwRSASignatureAlgorithm = 0;
		lpxSignature = NULL;
		m_xValue.Clear();
		m_xSignature.Clear();
	}

	void Set(DWORD dwRSASignatureAlgorithm, LPBYTE lpbValue, USHORT usValueLength, LPBYTE lpbSignature, 
		          USHORT usSignatureLength)
	{
		this->dwRSASignatureAlgorithm = dwRSASignatureAlgorithm;
		m_xValue.Set(lpbValue, usValueLength);
		lpxValue = (LPWFSXDATA)m_xValue;
		m_xSignature.Set(lpbSignature, usSignatureLength);
		lpxSignature = (LPWFSXDATA)m_xSignature;
	}
	
	operator LPWFSPINEXPORTRSAISSUERSIGNEDITEMOUTPUT()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xValue;
	CWFSXDATA  m_xSignature;
};

class CWFSPINIMPORTRSASIGNEDDESKEYOUTPUT : public WFSPINIMPORTRSASIGNEDDESKEYOUTPUT
{
public:
	CWFSPINIMPORTRSASIGNEDDESKEYOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINIMPORTRSASIGNEDDESKEYOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		wKeyLength = 0;
		wKeyCheckMode = 0;
		lpxKeyCheckValue = NULL;
		m_xData.Clear();
	}

	void Set(WORD wKeyLength, WORD wKeyCheckMode, LPBYTE lpbData, USHORT usLength)
	{
		this->wKeyLength = wKeyLength;
		this->wKeyCheckMode = wKeyCheckMode;
		m_xData.Set(lpbData, usLength);
		lpxKeyCheckValue = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINIMPORTRSASIGNEDDESKEYOUTPUT()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xData;
};

class CWFSPINEXPORTRSAEPPSIGNEDITEMOUTPUT : public WFSPINEXPORTRSAEPPSIGNEDITEMOUTPUT
{
public:
	CWFSPINEXPORTRSAEPPSIGNEDITEMOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINEXPORTRSAEPPSIGNEDITEMOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		lpxValue = NULL;
		lpxSelfSignature = NULL;
		lpxSignature = NULL;
		m_xValue.Clear();
		m_xSelfSignature.Clear();
		m_xSignature.Clear();
	}

	void Set(LPBYTE lpbValue, USHORT usValueLength, LPBYTE lpbSelfSignature, USHORT usSelfSignatureLength, 
		     LPBYTE lpbSignature, USHORT usSignatureLength)
	{
		m_xValue.Set(lpbValue, usValueLength);
		lpxValue = (LPWFSXDATA)m_xValue;
		m_xSelfSignature.Set(lpbSelfSignature, usSelfSignatureLength);
		lpxSelfSignature = (LPWFSXDATA)m_xSelfSignature;
		m_xSignature.Set(lpbSignature, usSignatureLength);
		lpxSignature = (LPWFSXDATA)m_xSignature;
	}
	
	operator LPWFSPINEXPORTRSAEPPSIGNEDITEMOUTPUT()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xValue;
	CWFSXDATA  m_xSelfSignature;
	CWFSXDATA  m_xSignature;
};

class CWFSPINLOADCERTIFICATEOUTPUT : public WFSPINLOADCERTIFICATEOUTPUT
{
public:
	CWFSPINLOADCERTIFICATEOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINLOADCERTIFICATEOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		lpxCertificateData = NULL;
		m_xData.Clear();
	}

	void Set(LPBYTE lpbData, USHORT usLength)
	{
		m_xData.Set(lpbData, usLength);
		lpxCertificateData = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINLOADCERTIFICATEOUTPUT()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xData;
};

class CWFSPINGETCERTIFICATEOUTPUT : public WFSPINGETCERTIFICATEOUTPUT
{
public:
	CWFSPINGETCERTIFICATEOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINGETCERTIFICATEOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		lpxCertificate = NULL;
		m_xData.Clear();
	}

	void Set(LPBYTE lpbData, USHORT usLength)
	{
		m_xData.Set(lpbData, usLength);
		lpxCertificate = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINGETCERTIFICATEOUTPUT()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xData;
};

class CWFSPINREPLACECERTIFICATEOUTPUT : public WFSPINREPLACECERTIFICATEOUTPUT
{
public:
	CWFSPINREPLACECERTIFICATEOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINREPLACECERTIFICATEOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		lpxNewCertificateData = NULL;
		m_xData.Clear();
	}

	void Set(LPBYTE lpbData, USHORT usLength)
	{
		m_xData.Set(lpbData, usLength);
		lpxNewCertificateData = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINREPLACECERTIFICATEOUTPUT()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xData;
};


class CWFSPINIMPORTRSAENCIPHEREDPKCS7KEYOUTPUT : public WFSPINIMPORTRSAENCIPHEREDPKCS7KEYOUTPUT
{
public:
	CWFSPINIMPORTRSAENCIPHEREDPKCS7KEYOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINIMPORTRSAENCIPHEREDPKCS7KEYOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		wKeyLength = 0;
		lpxRSAData = NULL;
		m_xData.Clear();
	}

	void Set(WORD wKeyLength, LPBYTE lpbData, USHORT usLength)
	{
		this->wKeyLength = wKeyLength;
		m_xData.Set(lpbData, usLength);
		lpxRSAData = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEYOUTPUT()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xData;
};


class CWFSPINEMVIMPORTPUBLICKEYOUTPUT : public WFSPINEMVIMPORTPUBLICKEYOUTPUT
{
public:
	CWFSPINEMVIMPORTPUBLICKEYOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINEMVIMPORTPUBLICKEYOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		lpsExpiryDate = NULL;
		m_CharBuffer.Clear();
	}

	void Set(LPSTR szExpiryDate)
	{
		if(NULL != szExpiryDate)
		{
			m_CharBuffer.Set(szExpiryDate);
			lpsExpiryDate = (LPSTR)m_CharBuffer;
		}
	}
	
	operator LPWFSPINEMVIMPORTPUBLICKEYOUTPUT()
	{
		return this;
	}

protected:
	CStringZ<char> m_CharBuffer;
};

class CWFSPINDIGESTOUTPUT : public WFSPINDIGESTOUTPUT
{
public:
	CWFSPINDIGESTOUTPUT()
	{
		Clear();
	}
	
	~CWFSPINDIGESTOUTPUT()
	{
		Clear();
	}

	void Clear()
	{
		lpxDigestOutput = NULL;
		m_xData.Clear();
	}

	void Set(LPBYTE lpbData, USHORT usLength)
	{
		m_xData.Set(lpbData, usLength);
		lpxDigestOutput = (LPWFSXDATA)m_xData;
	}
	
	operator LPWFSPINDIGESTOUTPUT()
	{
		return this;
	}
protected:
	CWFSXDATA  m_xData;
};

class CWFSPINEXPORTHOSTVERSIONOUT : public WFSPINEXPORTHOSTVERSIONOUT
{
public:
	CWFSPINEXPORTHOSTVERSIONOUT()
	{
		Clear();
	}
	~CWFSPINEXPORTHOSTVERSIONOUT()
	{
		Clear();
	}

	void Clear()
	{
		wCommand = 0;
    memset(lpTag, 0, sizeof(lpTag));
    usTagLength = 0;
    bRSASignatureAlgorithm = 0;
	}	

	void Set(WORD Command, LPBYTE Tag, USHORT TagLength, DWORD RSASignatureAlgorithm)
	{
		wCommand = Command;
		usTagLength = TagLength;
		if(usTagLength > 0)
			memcpy(lpTag, Tag, usTagLength);
		bRSASignatureAlgorithm = (BYTE)RSASignatureAlgorithm;
	}
	
	operator LPWFSPINEXPORTHOSTVERSIONOUT()
	{
		return this;
	}
};

class CWFSPINENCRYPTRCOUT : public WFSPINENCRYPTRCOUT
{
public:
	CWFSPINENCRYPTRCOUT()
	{
		Clear();
	}
	~CWFSPINENCRYPTRCOUT()
	{
		Clear();
	}

	void Clear()
	{
    memset(lpTag, 0, sizeof(lpTag));
    usTagLength = 0;
	}	

	void Set(WORD Command, LPBYTE Tag, USHORT TagLength)
	{
		wCommand = Command;
		usTagLength = TagLength;
		if(usTagLength > 0)
			memcpy(lpTag, Tag, usTagLength);
	}
	
	operator LPWFSPINENCRYPTRCOUT()
	{
		return this;
	}
};

class CWFSPINVIRTUALBANKKEYOUT : public WFSPINVIRTUALBANKKEYOUT
{
public:
	CWFSPINVIRTUALBANKKEYOUT()
	{
		Clear();
	}
	~CWFSPINVIRTUALBANKKEYOUT()
	{
		Clear();
	}

	void Clear()
	{
		wCommand = 0;
		usKcvLength = 0;
		memset(lpKcv, 0, sizeof(lpKcv));
	}	

	void Set(WORD Command, LPBYTE Kcv, USHORT KcvLength)
	{
		wCommand = Command;
		usKcvLength = KcvLength;
		if(usKcvLength > 0)
			memcpy(lpKcv, Kcv, usKcvLength);
	}
	
	operator LPWFSPINVIRTUALBANKKEYOUT()
	{
		return this;
	}
};

class CWFSPINFIRMWAREMACOUT : public WFSPINFIRMWAREMACOUT
{
public:
	CWFSPINFIRMWAREMACOUT()
	{
		Clear();
	}
	~CWFSPINFIRMWAREMACOUT()
	{
		Clear();
	}

	void Clear()
	{
		wCommand = 0;
		memset(lpMac, 0, sizeof(lpMac));
		usMacLength = 0;
	}	

	void Set(WORD Command, LPBYTE Mac, USHORT MacLength)
	{
		wCommand = Command;
		usMacLength = MacLength;
		if(usMacLength > 0)
			memcpy(lpMac, Mac, usMacLength);
	}
	
	operator LPWFSPINFIRMWAREMACOUT()
	{
		return this;
	}
};

#endif // _CPINSTRUCT_H_20151109
