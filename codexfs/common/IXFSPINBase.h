/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IXFSPINBase.h
* Description: base interface for XFS PIN
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#ifndef _IXFSPINBASE_H_20151109
#define _IXFSPINBASE_H_20151109

#include <IBaseSystem.h>
#include <CENXFSPIN.h>
#include <CENXFSPINEx.h>


INTERFACE_I  IXFSPINBase
{
public:
	//Info Commands
	virtual HRESULT Status() = 0;
	virtual HRESULT Capabilities() = 0;
	virtual HRESULT KeyDetail(LPSTR lpszKeyName) = 0;
	virtual HRESULT FuncKeyDetail(LPULONG lpulFDKMask) = 0;
	virtual HRESULT HsmTData() = 0;
	virtual HRESULT KeyDetailEx(LPSTR lpszKeyName) = 0;
	virtual HRESULT SecureKeyDetail() = 0;
	//3.10
	virtual HRESULT QueryLogicalHsmDetail() = 0;
	//3.20
	virtual HRESULT QueryPciptsDeviceId() = 0;
	//3.30
	virtual HRESULT GetLayout(LPWFSPINGETLAYOUT lpLayout) = 0;


public: 
	//Execute Commands
	virtual HRESULT Crypt(LPWFSPINCRYPT lpCrypt) = 0;
	virtual HRESULT ImportKey(LPWFSPINIMPORT lpImport) = 0;
	virtual HRESULT DeriveKey(LPWFSPINDERIVE lpDerive) = 0;
	virtual HRESULT GetPin(LPWFSPINGETPIN lpGetPin) = 0;
	virtual HRESULT LocalDes(LPWFSPINLOCALDES lpLocalDes) = 0;
	virtual HRESULT CreateOffset(LPWFSPINCREATEOFFSET lpPinOffset) = 0;
	virtual HRESULT LocalEurocheque(LPWFSPINLOCALEUROCHEQUE lpLocalEurocheque) = 0;
	virtual HRESULT LocalVisa(LPWFSPINLOCALVISA lpLocalVisa) = 0;	
	virtual HRESULT PresentIDC(LPWFSPINPRESENTIDC lpPresentIDC) = 0;
	virtual HRESULT GetPinBlock(LPWFSPINBLOCK lpPinBlock) = 0;
	virtual HRESULT GetData(LPWFSPINGETDATA lpPinGetData) = 0;
	virtual HRESULT Initialize(LPWFSPININIT lpInit) = 0;
	virtual HRESULT LocalBankSys(LPWFSPINLOCALBANKSYS lpLocalBanksys) = 0;
	virtual HRESULT BankSysIo(LPWFSPINBANKSYSIO lpBankSysIo) = 0;
	virtual HRESULT Reset() = 0;
	virtual HRESULT HsmSetTData(LPWFSXDATA lpxTData) = 0;
	virtual HRESULT SecureMsgSend(LPWFSPINSECMSG lpSecMsgIn) = 0;
	virtual HRESULT SecureMsgReceive(LPWFSPINSECMSG lpSecMsgIn) = 0;
	virtual HRESULT GetJournal(LPWORD lpwProtocol) = 0;
	virtual HRESULT ImportKeyEx(LPWFSPINIMPORTKEYEX lpImportKeyEx) = 0;	
	virtual HRESULT EncIo(LPWFSPINENCIO lpEncIo) = 0;
	virtual HRESULT HsmInit(LPWFSPINHSMINIT lpHsmInit) = 0;
	virtual HRESULT SecureKeyEntry(LPWFSPINSECUREKEYENTRY lpSecureKeyEntry) = 0;
	virtual HRESULT GenerateKCV(LPWFSPINGENERATEKCV lpGenerateKcv) = 0;
	//3.10
	virtual HRESULT SetGuidanceLight(LPWFSPINSETGUIDLIGHT lpSetGuidLight) = 0;
	virtual HRESULT MaintainPin(LPWFSPINMAINTAINPIN lpMaintainPinIn) = 0;
	virtual HRESULT KeyPressBeep(LPWORD lpwMode) = 0;
	virtual HRESULT SetPinBlockData(LPWFSPINBLOCK lpPinSetBlockData) = 0;
	virtual HRESULT SetLogicalHsm(LPWFSPINHSMIDENTIFIER lpSetHsm) = 0;
	virtual HRESULT ImportKeyBlock(LPWFSPINIMPORTKEYBLOCK lpImportKeyBlock) = 0;
	virtual HRESULT PowerSaveControl(LPWFSPINPOWERSAVECONTROL lpPowerSaveControl) = 0;
	//3.30
	virtual HRESULT DefineLayout(LPWFSPINLAYOUT *lppLayout) = 0;
	virtual HRESULT StartAuthenticate(LPWFSPINSTARTAUTHENTICATE lpSA) = 0;
	virtual HRESULT Authenticate(LPWFSPINAUTHENTICATE lpAuth) = 0;
	virtual HRESULT GetPinBlockEx(LPWFSPINBLOCKEX lpPinBlockEx) = 0;
	virtual HRESULT SynchronizeCmd(LPWFSPINSYNCHRONIZECOMMAND lpSynCmd, REQUESTID RequestID) = 0;

	//Remote Key Loading Using Signatures(3.00)
	virtual HRESULT StartKeyExchange() = 0;
	virtual HRESULT ImportRsaPublicKey(LPWFSPINIMPORTRSAPUBLICKEY lpImportRsaPublicKey) = 0;
	virtual HRESULT ExportRsaIssuerSignedItem(LPWFSPINEXPORTRSAISSUERSIGNEDITEM lpExportRsaIssuerSignedItem) = 0;
	virtual HRESULT ImportRsaSignedDesKey(LPWFSPINIMPORTRSASIGNEDDESKEY lpImportRsaSignedDesKey) = 0;
	virtual HRESULT GenerateRsaKeyPair(LPWFSPINGENERATERSAKEYPAIR lpGenerateRsaKeyPair) = 0;
	virtual HRESULT ExportRsaEppSignedItem(LPWFSPINEXPORTRSAEPPSIGNEDITEM lpExportRsaEppSignedItem) = 0;

	//Remote Key Loading Using Certificates(3.00)
	virtual HRESULT LoadCertificate(LPWFSPINLOADCERTIFICATE lpLoadCertificate) = 0;
	virtual HRESULT GetCertificate(LPWFSPINGETCERTIFICATE lpGetCertificate) = 0;
	virtual HRESULT ReplaceCertificate(LPWFSPINREPLACECERTIFICATE lpReplaceCertificate) = 0;
	virtual HRESULT ImportRsaEncipheredPkcs7Key(LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEY lpImportRsaEncipheredPkcs7Key) = 0;
	//3.30
	virtual HRESULT LoadCertificateEx(LPWFSPINLOADCERTIFICATEEX lpLCEx) = 0;
	virtual HRESULT ImportRsaEncipheredPkcs7KeyEx(LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEYEX lpIREPKEx) = 0;

	//EMV(3.00)
	virtual HRESULT EmvImportPublicKey(LPWFSPINEMVIMPORTPUBLICKEY lpEmvImportPublicKey) = 0;
	virtual HRESULT Digest(LPWFSPINDIGEST lpDigest) = 0;

	
public:
	//China Protocol
	virtual HRESULT ENC_China(WORD wProtocol, LPVOID lpData, ULONG ulDataLength) = 0;	
	virtual HRESULT SM3_Digest(LPPROTCHNDIGESTIN lpDigest) = 0;
	virtual HRESULT SM2_SetPara(LPPROTCHNSM2ALGORITHMPARAMIN lpSM2) = 0;
	virtual HRESULT SM2_ImportPK(LPPROTCHNIMPORTSM2PUBLICKEYIN lpSM2ImpPK) = 0;
	virtual HRESULT SM2_Sign(LPPROTCHNSIGNIN lpSign) = 0;
	virtual HRESULT SM2_Verify(LPPROTCHNVERIFYIN lpVerify) = 0;
	virtual HRESULT SM2_GenerateKeyPair(LPPROTCHNGENERATESM2KEYPAIRIN lpGenKey) = 0;
	virtual HRESULT SM2_ExportKey(LPPROTCHNEXPORTSM2EPPSIGNEDITEMIN lpExpKey) = 0;
	virtual HRESULT SM2_ImportSM4Key(LPPROTCHNIMPORTSM2SIGNEDSM4KEY lpSM4Key) = 0;

	virtual HRESULT SM2_KeyExchangeB(LPWFSPINSM2KEYEXBIN lpKeyExcB) = 0;
	virtual HRESULT SM2_KeyAgreementB(LPWFSPINSM2KEYAGREEMENTBIN lpKeyAgreeB) = 0;


public:
	//Events
	virtual void FireKey(WORD wComplete, ULONG ulDigit) = 0;
	virtual void FireInitialized(LPBYTE lpbIdentData, USHORT usIdentLength, LPBYTE lpbKeyData, USHORT usKeyLength) = 0;
	virtual void FireIllegalKeyAccess(LPSTR szKeyName, LONG lErrorCode) = 0;
	virtual void FireOptRequired() = 0;
	virtual void FireCertificateChange(WORD wCertificateChange) = 0;
	virtual void FireHsmTDataChanged(LPBYTE lpData, USHORT usLength) = 0;
	//3.10
	virtual void FireHsmChanged(WORD wHSMSerialNumber) = 0;
	virtual void FireEnterData() = 0;
	virtual void FireDevicePosition(WORD wPosition) = 0;
	virtual void FirePowerSaveChange(USHORT usPowerSaveRecoveryTime) = 0;
	//3.30
	virtual void FireLayout(DWORD dwEntryMode, USHORT usNumberOfFrames, LPWFSPINFRAME *lppFrames) = 0;
};

#endif	//_IXFSPINBASE_H_20151109
