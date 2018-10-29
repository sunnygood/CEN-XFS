/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSPINBase.h
* Description: base class for XFS PIN
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#ifndef _CXFSPINBASE_H_20151109
#define _CXFSPINBASE_H_20151109

#include <IXFSPINBase.h>
#include <CXFSSPFace.h>
#include <IPinpad.h>
#include <IPinpadEx.h>
#include "CPressKey.h"
#include "CPINStruct.h"


#undef  LOG_NAME_LOG
#define LOG_NAME_LOG     LOG_XFSPIN


#define TIMEID_UPDATE_STATUS	(WFS_SERVICE_CLASS_PIN)


#define ALLOT_KEYID(lpKeyName, wKeyId)  \
		if(lpKeyName == NULL || lpKeyName[0] == 0)\
		{\
			RLog2(LOG_LEVEL_ERROR, "Key name is empty.");\
			return WFS_ERR_PIN_INVALIDID;\
		}\
		if(strlen(lpKeyName) > MAX_KEY_NAME)\
		{\
			RLog2(LOG_LEVEL_ERROR, "Key name is too long, max length is %d.", MAX_KEY_NAME);\
			return WFS_ERR_PIN_INVALIDID;\
		}\
		if((wKeyId = m_pPinpad->XFS_AllotKeyId(lpKeyName)) == KEY_INVALID) \
		{\
			RLog2(LOG_LEVEL_ERROR, "No space to store key."); \
			return WFS_ERR_PIN_NOKEYRAM; \
		}

/********************************************************************************/

class CXFSPINBase : virtual public CXFSSPFace, public IXFSPINBase
{
public:
	virtual LPVOID  GetInterface(int nIndex = II_FACE);
	virtual HRESULT OnSPStart();
	virtual HRESULT OnSPExit();
	virtual HRESULT OnGetInfo(DWORD dwCategory, LPVOID lpQueryDetail, REQUESTID RequestID);
	virtual HRESULT OnExecute(DWORD dwCommand, LPVOID lpCmdData, REQUESTID RequestID);

	virtual HRESULT OnTimer(UINT nTimerID, LPVOID lpContext = NULL);


public: 
	//Info Commands	
	virtual HRESULT Status();
	virtual HRESULT Capabilities();
	virtual HRESULT KeyDetail(LPSTR lpszKeyName);
	virtual HRESULT FuncKeyDetail(LPULONG lpulFDKMask);
	virtual HRESULT HsmTData();
	virtual HRESULT KeyDetailEx(LPSTR lpszKeyName);
	virtual HRESULT SecureKeyDetail();
	//3.10
	virtual HRESULT QueryLogicalHsmDetail();
	//3.20
	virtual HRESULT QueryPciptsDeviceId();
	//3.30
	virtual HRESULT GetLayout(LPWFSPINGETLAYOUT lpLayout);


public: 
	//Execute Commands
	virtual HRESULT Crypt(LPWFSPINCRYPT lpCrypt);
	virtual HRESULT ImportKey(LPWFSPINIMPORT lpImport);
	virtual HRESULT DeriveKey(LPWFSPINDERIVE lpDerive);
	virtual HRESULT GetPin(LPWFSPINGETPIN lpGetPin);
	virtual HRESULT LocalDes(LPWFSPINLOCALDES lpLocalDes);
	virtual HRESULT CreateOffset(LPWFSPINCREATEOFFSET lpPinOffset);
	virtual HRESULT LocalEurocheque(LPWFSPINLOCALEUROCHEQUE lpLocalEurocheque);
	virtual HRESULT LocalVisa(LPWFSPINLOCALVISA lpLocalVisa);	
	virtual HRESULT PresentIDC(LPWFSPINPRESENTIDC lpPresentIDC);
	virtual HRESULT GetPinBlock(LPWFSPINBLOCK lpPinBlock);
	virtual HRESULT GetData(LPWFSPINGETDATA lpPinGetData);
	virtual HRESULT Initialize(LPWFSPININIT lpInit);
	virtual HRESULT LocalBankSys(LPWFSPINLOCALBANKSYS lpLocalBanksys);
	virtual HRESULT BankSysIo(LPWFSPINBANKSYSIO lpBankSysIo);
	virtual HRESULT Reset();
	virtual HRESULT HsmSetTData(LPWFSXDATA lpxTData);
	virtual HRESULT SecureMsgSend(LPWFSPINSECMSG lpSecMsgIn);
	virtual HRESULT SecureMsgReceive(LPWFSPINSECMSG lpSecMsgIn);
	virtual HRESULT GetJournal(LPWORD lpwProtocol);
	virtual HRESULT ImportKeyEx(LPWFSPINIMPORTKEYEX lpImportKeyEx);	
	virtual HRESULT EncIo(LPWFSPINENCIO lpEncIo);
	virtual HRESULT HsmInit(LPWFSPINHSMINIT lpHsmInit);
	virtual HRESULT SecureKeyEntry(LPWFSPINSECUREKEYENTRY lpSecureKeyEntry);
	virtual HRESULT GenerateKCV(LPWFSPINGENERATEKCV lpGenerateKcv);
	//3.10
	virtual HRESULT SetGuidanceLight(LPWFSPINSETGUIDLIGHT lpSetGuidLight);
	virtual HRESULT MaintainPin(LPWFSPINMAINTAINPIN lpMaintainPinIn);
	virtual HRESULT KeyPressBeep(LPWORD lpwMode);
	virtual HRESULT SetPinBlockData(LPWFSPINBLOCK lpPinSetBlockData);
	virtual HRESULT SetLogicalHsm(LPWFSPINHSMIDENTIFIER lpSetHsm);
	virtual HRESULT ImportKeyBlock(LPWFSPINIMPORTKEYBLOCK lpImportKeyBlock);
	virtual HRESULT PowerSaveControl(LPWFSPINPOWERSAVECONTROL lpPowerSaveControl);
	//3.30
	virtual HRESULT DefineLayout(LPWFSPINLAYOUT *lppLayout);
	virtual HRESULT StartAuthenticate(LPWFSPINSTARTAUTHENTICATE lpSA);
	virtual HRESULT Authenticate(LPWFSPINAUTHENTICATE lpAuth);
	virtual HRESULT GetPinBlockEx(LPWFSPINBLOCKEX lpPinBlockEx);
	virtual HRESULT SynchronizeCmd(LPWFSPINSYNCHRONIZECOMMAND lpSynCmd, REQUESTID RequestID);

	//Remote Key Loading Using Signatures(3.00)
	virtual HRESULT StartKeyExchange();
	virtual HRESULT ImportRsaPublicKey(LPWFSPINIMPORTRSAPUBLICKEY lpImportRsaPublicKey);
	virtual HRESULT ExportRsaIssuerSignedItem(LPWFSPINEXPORTRSAISSUERSIGNEDITEM lpExportRsaIssuerSignedItem);
	virtual HRESULT ImportRsaSignedDesKey(LPWFSPINIMPORTRSASIGNEDDESKEY lpImportRsaSignedDesKey);
	virtual HRESULT GenerateRsaKeyPair(LPWFSPINGENERATERSAKEYPAIR lpGenerateRsaKeyPair);
	virtual HRESULT ExportRsaEppSignedItem(LPWFSPINEXPORTRSAEPPSIGNEDITEM lpExportRsaEppSignedItem);

	//Remote Key Loading Using Certificates(3.00)
	virtual HRESULT LoadCertificate(LPWFSPINLOADCERTIFICATE lpLoadCertificate);
	virtual HRESULT GetCertificate(LPWFSPINGETCERTIFICATE lpGetCertificate);
	virtual HRESULT ReplaceCertificate(LPWFSPINREPLACECERTIFICATE lpReplaceCertificate);
	virtual HRESULT ImportRsaEncipheredPkcs7Key(LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEY lpImportRsaEncipheredPkcs7Key);
	//3.30
	virtual HRESULT LoadCertificateEx(LPWFSPINLOADCERTIFICATEEX lpLCEx);
	virtual HRESULT ImportRsaEncipheredPkcs7KeyEx(LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEYEX lpIREPKEx);

	//EMV(3.00)
	virtual HRESULT EmvImportPublicKey(LPWFSPINEMVIMPORTPUBLICKEY lpEmvImportPublicKey);
	virtual HRESULT Digest(LPWFSPINDIGEST lpDigest);

	
public:
	//China Protocol
	virtual HRESULT ENC_China(WORD wProtocol, LPVOID lpData, ULONG ulDataLength);	
	virtual HRESULT SM3_Digest(LPPROTCHNDIGESTIN lpDigest);
	virtual HRESULT SM2_SetPara(LPPROTCHNSM2ALGORITHMPARAMIN lpSM2);
	virtual HRESULT SM2_ImportPK(LPPROTCHNIMPORTSM2PUBLICKEYIN lpSM2ImpPK);
	virtual HRESULT SM2_Sign(LPPROTCHNSIGNIN lpSign);
	virtual HRESULT SM2_Verify(LPPROTCHNVERIFYIN lpVerify);
	virtual HRESULT SM2_GenerateKeyPair(LPPROTCHNGENERATESM2KEYPAIRIN lpGenKey);
	virtual HRESULT SM2_ExportKey(LPPROTCHNEXPORTSM2EPPSIGNEDITEMIN lpExpKey);
	virtual HRESULT SM2_ImportSM4Key(LPPROTCHNIMPORTSM2SIGNEDSM4KEY lpSM4Key);

	virtual HRESULT SM2_KeyExchangeB(LPWFSPINSM2KEYEXBIN lpKeyExcB);
	virtual HRESULT SM2_KeyAgreementB(LPWFSPINSM2KEYAGREEMENTBIN lpKeyAgreeB);

	
public: 
	//France Groupement des Cartes Bancaires
	virtual HRESULT ENC_GIECB(WORD wProtocol, LPVOID lpData, ULONG ulDataLength);
	virtual HRESULT Interchange(BYTE bVersion, char *szCode, char *szKeyName, LPBYTE lpbData, ULONG ulLength);


public:
	//Events
	virtual void FireKey(WORD wComplete, ULONG ulDigit);
	virtual void FireInitialized(LPBYTE lpbIdentData, USHORT usIdentLength, LPBYTE lpbKeyData, USHORT usKeyLength);
	virtual void FireIllegalKeyAccess(LPSTR szKeyName, LONG lErrorCode);
	virtual void FireOptRequired();
	virtual void FireCertificateChange(WORD wCertificateChange);
	virtual void FireHsmTDataChanged(LPBYTE lpData, USHORT usLength);
	//3.10
	virtual void FireHsmChanged(WORD wHSMSerialNumber);
	virtual void FireEnterData();
	virtual void FireDevicePosition(WORD wPosition);
	virtual void FirePowerSaveChange(USHORT usPowerSaveRecoveryTime);
	//3.30
	virtual void FireLayout(DWORD dwEntryMode, USHORT usNumberOfFrames, LPWFSPINFRAME *lppFrames);


public:
	explicit CXFSPINBase();
	virtual ~CXFSPINBase();


protected:
	//SZZT
	HRESULT ENC_SZZT(WORD wProtocol, LPVOID lpData, ULONG ulDataLength);
	HRESULT GetPassword(LPWFSPINGETPASSWORD lpGetPsw);
	HRESULT RsaVendorInit(LPWFSPINRSAVENDORINIT lpRSAVI);
	HRESULT ImportHostVersion(LPWFSPINIMPORTHOSTVERSION lpImpHostVer);
	HRESULT ExportHostVersion();
	HRESULT EncryptRC(LPWFSPINENCRYPTRCIN lpEncryptRC);
	HRESULT AutoUpdatePassword(LPWFSPINAUTOUPDATEPASSWORD lpAutoUpdatePsw);
	HRESULT VirtualBankKey(LPWFSPINVIRTUALBANKKEYIN lpVirtualKey);	
	HRESULT AutoRemoveInstall(LPWFSPINAUTOREMOVEINSTALL lpAutoRI);
	HRESULT FirmwareMac();
	HRESULT SelfTest(LPWFSPINSELFTESTIN lpSelfTest);


protected:
	//Inner function
	HRESULT NewPinpad();
	HRESULT Config();
	HRESULT UpdateStatus();
	HRESULT CheckOpen();
	HRESULT Convert2SPError(long nErrNo);

	HRESULT FindKey(LPSTR lpsKey, WORD &wKeyId, DWORD dwCheckUse = 0);
	HRESULT DeleteKey(LPSTR lpsKey);
	HRESULT CheckActiveKey(ULONG ulActiveKeys, ULONG ulActiveFDKs, ULONG ulTerminateKeys, ULONG ulTerminateFDKs, ULONG ulFKMask, ULONG ulFDKMask);
	HRESULT CheckTerminateKey(ULONG ulTerminateKeys, ULONG ulTerminateFDKs, BOOL bAutoEnd);
	HRESULT DownLoadKey(LPSTR lpsKey, LPSTR lpEncKey, LPWFSXDATA lpxValue, DWORD dwUse, LPWFSXDATA lpxAA, char *lpKCV, EKCVMODE eKCVMode = KCVZERO);
	HRESULT LocalIBM(LPSTR lpsKey, LPWFSXDATA lpxKeyEncKey, USHORT usMaxPIN, BYTE bPadding, LPCSTR lpsDecTable, LPSTR lpsValidationData, USHORT usValDigits, BYTE B_EMode, char *lpInOutPVV);
	HRESULT SecureKeyEntryForN(USHORT usKeyLen, BOOL bAutoEnd, ULONG ulActiveFDKs, ULONG ulActiveKeys, ULONG ulTerminateFDKs, ULONG ulTerminateKeys, WORD wVerificationType);
	HRESULT SecureKeyEntryForH(USHORT usKeyLen, BOOL bAutoEnd, ULONG ulActiveFDKs, ULONG ulActiveKeys, ULONG ulTerminateFDKs, ULONG ulTerminateKeys, WORD wVerificationType);
	HRESULT SecureKeyEntryForS(USHORT usKeyLen, BOOL bAutoEnd, ULONG ulActiveFDKs, ULONG ulActiveKeys, ULONG ulTerminateFDKs, ULONG ulTerminateKeys, WORD wVerificationType);
	
	EKEYMODE GetLoadKeyMode(LPWFSXDATA lpData);
	void SetKeyCodeforCxx(ULONG ulActiveKeys);
	

protected:
	class CAutoRecoverKeyCode
	{
	public:
		CAutoRecoverKeyCode(IPinpad *Pinpad, vector<BYTE> veCodes, BOOL IsRecover, ENTRYMODE eMode = ENTRY_MODE_TEXT)
			: m_Pinpad(Pinpad), m_veCodes(veCodes), m_IsRecover(IsRecover), m_Mode(eMode)
		{
		}	
		virtual ~CAutoRecoverKeyCode()
		{
			if(!m_IsRecover)
				return;
			m_Pinpad->SetKeyboardCode((char*)(m_veCodes.data()), m_veCodes.size(), m_Mode);	
		}	

	private:
		BOOL m_IsRecover;
		ENTRYMODE m_Mode;
		vector<BYTE> m_veCodes;
		IPinpad *m_Pinpad;
	};

	class CAutoCloseEntry
	{
	public:
		CAutoCloseEntry(IPinpad *Pinpad)
			: m_Pinpad(Pinpad)
		{
		}	
		virtual ~CAutoCloseEntry()
		{
			long nRet;
			if(nRet = m_Pinpad->OpenKeyboardAndSound(SOUND_CLOSE, ENTRY_MODE_CLOSE))
			{
				RLog2(LOG_LEVEL_ERROR, "close input fail, nRet=%d", nRet);
			}	
		}	

	private:
		IPinpad *m_Pinpad;
	};


protected:
	CPressKey      m_PressKey;
	USHORT         m_usPinLen, m_usKeyLen;
	IPinpad       *m_pPinpad;
	PINCAPS        m_stPinCaps;
	ULONG          m_nEcho;
	BYTE           m_cEchoHW;
	BYTE           m_cTimeOutCode;
    BYTE           m_cLongPressCode;
	EKCVMODE       m_eKCVMode;

	int m_nKCVLen;
	int m_nDESMacAlg, m_nTDESMacAlg, m_nSM4MacAlg;
	int m_nOffsetGenAlgorithm, m_nOffsetVerifyAlgorithm, m_nOffsetIndex;
	int m_nVisaAlgorithm, m_nVisaIndex, m_nVisaLen;
	int m_n00[3], m_n000[3]; //GetData,GetPin,SecureKey
	int m_bIsLongPressedTimeout, m_bIsAysnCancelClearPins, m_bIsTimeoutClearPins;
	int m_bIsRKLNeedRandom;
	int m_nDataEntryVoice, m_nPinEntryVoice, m_nKeyEntryVoice, m_nPswEntryVoice;
	int m_nIsDisablePressKey;
	int m_nCustomerDataMode;
	int m_nStartPinInputTimeout;
	int m_bExistKeyIsReady;
	int m_bIsImportKeyExForSm4;
	int m_nIsCheckEnKeyUsage;
	
	char m_szVer[MAX_PATH];
	

protected:
	CWFSPINSTATUS							 m_CStatus;
	CWFSPINCAPS                              m_CCaps;
	CWFSPINKEYDETAILWRAPPER		  		     m_CKeyDetail;
	CWFSPINFUNCKEYDETAIL					 m_CFuncKeyDetail;
	CWFSXDATA								 m_CHsmTData;
	CWFSPINKEYDETAILEXWRAPPER				 m_CKeyDetailEx;
	CWFSPINSECUREKEYDETAIL					 m_CSecureKeyDetail;
	CWFSPINHSMDETAIL                         m_CQueryLogicalHsmDetail;
	CWFSPINPCIPTSDEVICEID                    m_CQueryPciptsDeviceId;
	CWFSPINLAYOUTWRAPPER                     m_CLayout;

	CWFSXDATA								 m_CryptResult;
	CWFSXDATA								 m_XDataResult;    //ImportKey,GetPinBlock,Initialize,GetJournal
	CWFSPINDATA								 m_GetDataResult;
	CWFSPINENTRY							 m_GetPinResult;
	CStringZ<char>							 m_CharResult;           //CreateOffset
	BOOL									 m_BoolResult;           //LocalDes,LocalEurocheque,LocalVisa,LocalBankSys
	CWFSPINBANKSYSIO						 m_BankSysIoResult;
	CWFSPINPRESENTRESULT				     m_PresentIDCResult;
	CWFSPINSECMSG							 m_SecureMsgSendResult;               
	CWFSPINENCIO							 m_EncIOResult;
	CWFSPINSECUREKEYENTRYOUT				 m_SecureKeyEntryResult;
	CWFSPINKCV								 m_GenerateKcvResult;

	CWFSPINSTARTKEYEXCHANGE					 m_StartKeyExchangeResult;
	CWFSPINIMPORTRSAPUBLICKEYOUTPUT			 m_ImportRsaPublicKeyResult;
	CWFSPINEXPORTRSAISSUERSIGNEDITEMOUTPUT	 m_ExportRsaIssureSignedItemResult;
	CWFSPINIMPORTRSASIGNEDDESKEYOUTPUT		 m_ImportRsaSignedDesKeyResult;
	CWFSPINEXPORTRSAEPPSIGNEDITEMOUTPUT		 m_ExportRsaEppSignedItemResult;

	CWFSPINLOADCERTIFICATEOUTPUT			 m_LoadCertificateResult;
	CWFSPINGETCERTIFICATEOUTPUT				 m_GetCertificateResult;
	CWFSPINREPLACECERTIFICATEOUTPUT			 m_ReplaceCertificateResult;
	CWFSPINIMPORTRSAENCIPHEREDPKCS7KEYOUTPUT m_ImportRsaEncipheredPkcs7KeyResult;

	CWFSPINEMVIMPORTPUBLICKEYOUTPUT			 m_EmvImportPublicKeyResult;
	CWFSPINDIGESTOUTPUT						 m_DigestResult;

	//ENC_IO
	CWFSPINEXPORTHOSTVERSIONOUT m_ExpHostVer;
	CWFSPINENCRYPTRCOUT         m_EncryptRc;
	CWFSPINVIRTUALBANKKEYOUT    m_VirtualBankKey;
	CWFSPINFIRMWAREMACOUT       m_FirmwareMac;
};

#endif	//_CXFSPINBASE_H_20151109
