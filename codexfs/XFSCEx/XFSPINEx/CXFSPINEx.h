/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSPINEx.h
* Description: extend class for XFS PIN
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#ifndef _CXFSPINEX_H_20151109
#define _CXFSPINEX_H_20151109

#include <IPinpad.h>
#include <IPinpadEx.h>
#include <IXFSPINBase.h>
#include "CXFSSPFaceEx.h"
#include "CPINStruct.h"


#undef  LOG_NAME_LOG
#define LOG_NAME_LOG     LOG_XFSPIN


/********************************************************************************/

class CXFSPINEx : public CXFSSPFaceEx, public IXFSPINBase
{
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
	explicit CXFSPINEx(IXFSSPManage *pManage, IXFSSPFace *pFace, IXFSPINBase *pBase);
	virtual ~CXFSPINEx();


protected:
	IXFSPINBase     *m_pSPBase;
	IPinpad         *m_pPinpad;
};

#endif	//_CXFSPINEX_H_20151109
