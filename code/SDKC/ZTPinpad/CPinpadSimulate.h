/*****************************************************************************
* Copyright(C) 1993-2012,ZT Corporation.
*
* File Name:   CPinpadSimulate.h
* Description: the base class of simulate pinpad device
*
* Author:      liuguoxun
* Date:        2012-05-28
* History 1: 
*
*****************************************************************************/

#ifndef CPINPADSIMULATE_H_20120528
#define CPINPADSIMULATE_H_20120528

#include "CPinpadBase.h"

	
/*****************************************************************************/

class CPinpadSimulate : public CPinpadBase
{
public:
	long GetHardwareVersion(char *pcVersion);
	long OpenKeyboardAndSound(ESOUND eSound, ENTRYMODE eMode, DWORD dwActiveKey = 0, DWORD dwActiveFDK = 0);


	explicit CPinpadSimulate(EPORT ePort, EExchange eType, EPIN_EXTEND eExtend);
	virtual ~CPinpadSimulate();

	virtual long RegisterMyFeatures(BOOL bAll = TRUE);
	

	static EPIN_TYPE IsMyself(WORD &wSubType, IPort *pPort);

	static long CALLBACK ParseRetLen(LPCSTR lpcHead)
	{
		if(NULL == lpcHead)
		{			
			return 0;
		}

		LPCSTR p = strstr(lpcHead, "\x02");
		if(NULL == p)
		{
			return 0;
		}
		else if(p == lpcHead)
		{
			return (((BYTE)*(lpcHead + 4)) << 8) + ((BYTE)*(lpcHead + 3)) + 8; //8 is const length	
		}
		else
		{
			return (lpcHead - p);
		}
	}

	
protected:
	virtual long Inner_Init(long nMode = 0);
	virtual long Inner_Reset(void);
	virtual long Inner_SelfTest(long iComponent = 0);
	virtual long Inner_GetLoaderVersion(char *pcVersion);
	virtual long Inner_ChipCommunicate(BYTE Mode, char *pChipInfo);
	virtual long Inner_DownProgram(LPCSTR pcFileData, long nFileLen, BYTE Mode, void *hWnd);
	virtual long Inner_SetKeyboardCode(LPCSTR lpKeyboardCodes, long nLen, ENTRYMODE eMode = ENTRY_MODE_TEXT);
	virtual long Inner_GetKeyboardCode(char *lpKeyboardCodes, long &nOutLen, ENTRYMODE eMode = ENTRY_MODE_TEXT);
	virtual long Inner_SM2VendorInit(bool bSoftGenKeyPair, LPCSTR lpIDa, WORD wIDLen, LPCSTR SKOtherVendor, WORD wSKLen, LPCSTR PKOtherVendor, WORD wPKLen);
	virtual long Inner_RSAVendorInit(bool bSoftGenKeyPair = true, long nBits = 2048, LPCSTR SKOtherVendor = 0, WORD wSKLen = 0, LPCSTR PKOtherVendor = 0, WORD wPKLen = 0);
	virtual long Inner_ImpRSAKey(WORD wKeyId, LPCSTR lpDer, WORD wDerLen, LPCSTR lpSig, WORD wSigLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wSigId = _VENDOR_PK, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0);	
	virtual long Inner_RSAImpDESKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpTag, WORD wTagLen, bool bRandom = true, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wHostPK = 1, WORD wDecryptId = _EPP_CRY_SK, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0, WORD *lpDesKeyLen = 0);

	virtual long SetEntryMode(ENTRYMODE eMode, DWORD dwActiveKey = 0, DWORD dwActiveFDK = 0);	

	virtual long ECHO(void);

	virtual long Inner_StoreKey(WORD wKeyId, DWORD dwKeyAttr, EKEYMODE eMode, long iKeyLen = 16, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long Inner_VirtualBankKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long nLen, LPCSTR lpPSW, LPCSTR lpKeyboardCodes, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long VirtualBankKeyGo(WORD wKeyId, DWORD dwKeyAttr, LPCSTR cKeyTable, long nLen, LPCSTR cPSWTable, long nPSWLen, LPCSTR cZeros, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);

	virtual long SM4_LoadTextKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual	long SM4_ExchangeKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, WORD wEnKey, WORD wVI = 0, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long SM4_StoreKey(WORD wKeyId, DWORD dwKeyAttr, EKEYMODE eMode, long iKeyLen = 16, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);

	virtual long Inner_DeleteKey(WORD wKeyId);
	virtual long Inner_ReadKeyAttr(WORD wKeyId, LPDWORD lpKeyAttr, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0, WORD wVI = 0);
	
	virtual long Inner_LoadKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, WORD wEnKey = KEY_INVALID, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long Inner_GetPinBlock(WORD wKeyId, BYTE PinLen, EPINFORMAT ePinFormat, LPCSTR lpCardNo, char *pPinBlock, WORD &wOutLen, BYTE Padding = 0x0F, WORD wKeyId2 = KEY_INVALID, WORD wVI = 0, WORD wVI2 = 0);
	virtual long Inner_Crypt(WORD wKeyId, ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, bool bEncrypt = true, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, WORD wVI = 0);		
	virtual long Inner_MAC(WORD wKeyId, EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, WORD wVI = 0); 
	virtual long SM4_LoadKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD iKeyLen, WORD wEnKey = KEY_INVALID, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long SM4_Crypt(WORD wKeyId, ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, bool bEncrypt = true, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, WORD wVI = 0);
	virtual long SM4_MAC(WORD wKeyId, EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, WORD wVI = 0);
	virtual long SM4_GetPinBlock(WORD wKeyId, BYTE PinLen, EPINFORMAT ePinFormat, LPCSTR lpCardNo, char *pPinBlock, WORD &wOutLen, BYTE Padding = 0x0F, WORD wKeyId2 = KEY_INVALID, WORD wVI = 0, WORD wVI2 = 0);

	virtual long SM2_Encrypt(WORD wKeyId, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut);

	virtual long RAW_Crypt_DES(ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpIV, WORD wIVLen, char *lpOut, bool bEncrypt = true);
	virtual long RAW_MAC_DES(EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpIV, WORD wIVLen, char *lpOut);
	virtual long RAW_Crypt_RSA(LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpModulus, WORD wModulusLen, LPCSTR lpExponent, WORD wExponentLen, char *lpOut);
	virtual long RAW_GenSig_RSA(LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpModulus, WORD wModulusLen, LPCSTR lpExponent, WORD wExponentLen, char *lpOut);
	virtual long RAW_VerifySig_RSA(LPCSTR lpDataIn, DWORD dwInLen, LPCSTR lpModulus, WORD wModulusLen, LPCSTR lpExponent, WORD wExponentLen, LPCSTR lpSigIn, WORD wSigLen);
	virtual long RAW_Crypt_SM2(LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpIV, WORD wIVLen, char *lpOut, bool bEncrypt = true);
	virtual long RAW_GenSig_SM2(LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut);
	virtual long RAW_VerifySig_SM2(LPCSTR lpDataIn, DWORD dwInLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, LPCSTR lpSigIn, WORD wSigLen);
	virtual long RAW_Crypt_SM4(ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpIV, WORD wIVLen, char *lpOut, bool bEncrypt = true);

	virtual long RAW_SM2_GenKeys(char *lpSK, WORD &wSKLen, char *lpPK, WORD &wPKLen);
	
	virtual long RandomData(DWORD &dwInOutLen, char *pRandom);
	virtual long RI_RandomData(DWORD &dwInOutLen, char *pRandom);
	virtual long Cmd_RemoveInstallBySig(EINSTALL_AUTH eMode, LPCSTR lpTag, DWORD dwTagLen, WORD wKeyID = _HOST_SM2_SIG_PK_DLL);
	virtual long Inner_RemoveInstallBySig(EINSTALL_AUTH eMode, LPCSTR lpKey, WORD wKeyLen, WORD wKeyID = _HOST_SM2_SIG_PK_DLL);
};


#endif //CPINPADSIMULATE_H_20120528

