/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSPINEx.cpp
* Description: extend class for XFS PIN
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#include "CXFSPINEx.h"


CXFSPINEx::CXFSPINEx(IXFSSPManage *pManage, IXFSSPFace *pFace, IXFSPINBase *pBase)
	: CXFSSPFaceEx(pManage, pFace)
	, m_pSPBase(pBase)
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");

	m_pPinpad = NULL;
	if(NULL != pFace)
	{
		m_pPinpad = (IPinpad *)pFace->GetInterface(II_SDK);
	}
}

CXFSPINEx::~CXFSPINEx()
{
	RLog2(LOG_LEVEL_COMMON, "Enter");
}

//===========================================================================//
//                          GetInfo Commands                                 //
//===========================================================================//
HRESULT CXFSPINEx::Status()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, Status());
}

HRESULT CXFSPINEx::Capabilities()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, Capabilities());
}

HRESULT CXFSPINEx::KeyDetail(LPSTR lpszKeyName)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, KeyDetail(lpszKeyName));
}

HRESULT CXFSPINEx::FuncKeyDetail(LPULONG lpulFDKMask)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, FuncKeyDetail(lpulFDKMask));
}

HRESULT CXFSPINEx::HsmTData()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, HsmTData());
}

HRESULT CXFSPINEx::KeyDetailEx(LPSTR lpszKeyName)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, KeyDetailEx(lpszKeyName));
}

HRESULT CXFSPINEx::SecureKeyDetail()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SecureKeyDetail());
}

HRESULT CXFSPINEx::QueryLogicalHsmDetail()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, QueryLogicalHsmDetail());
}

HRESULT CXFSPINEx::QueryPciptsDeviceId()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, QueryPciptsDeviceId());
}

HRESULT CXFSPINEx::GetLayout(LPWFSPINGETLAYOUT lpLayout)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, GetLayout(lpLayout));
}

//===========================================================================//
//                          Exectue Commands                                 //
//===========================================================================//
HRESULT CXFSPINEx::Crypt(LPWFSPINCRYPT lpCrypt)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, Crypt(lpCrypt));
}

HRESULT CXFSPINEx::ImportKey(LPWFSPINIMPORT lpImport)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ImportKey(lpImport));
}

HRESULT CXFSPINEx::DeriveKey(LPWFSPINDERIVE lpDerive)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, DeriveKey(lpDerive));
}

HRESULT CXFSPINEx::GetData(LPWFSPINGETDATA lpPinGetData)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, GetData(lpPinGetData));
}

HRESULT CXFSPINEx::GetPin(LPWFSPINGETPIN lpGetPin)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, GetPin(lpGetPin));
}

HRESULT CXFSPINEx::GetPinBlock(LPWFSPINBLOCK lpPinBlock)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, GetPinBlock(lpPinBlock));
}

HRESULT CXFSPINEx::CreateOffset(LPWFSPINCREATEOFFSET lpPinOffset)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, CreateOffset(lpPinOffset));
}

HRESULT CXFSPINEx::LocalDes(LPWFSPINLOCALDES lpLocalDes)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, LocalDes(lpLocalDes));
}

HRESULT CXFSPINEx::LocalEurocheque(LPWFSPINLOCALEUROCHEQUE lpLocalEurocheque)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, LocalEurocheque(lpLocalEurocheque));
}

HRESULT CXFSPINEx::LocalVisa(LPWFSPINLOCALVISA lpLocalVisa)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, LocalVisa(lpLocalVisa));
}

HRESULT CXFSPINEx::LocalBankSys(LPWFSPINLOCALBANKSYS lpLocalBanksys)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, LocalBankSys(lpLocalBanksys));
}

HRESULT CXFSPINEx::BankSysIo(LPWFSPINBANKSYSIO lpBankSysIo)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, BankSysIo(lpBankSysIo));
}

HRESULT CXFSPINEx::PresentIDC(LPWFSPINPRESENTIDC lpPresentIDC)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, PresentIDC(lpPresentIDC));
}

HRESULT CXFSPINEx::Initialize(LPWFSPININIT lpInit)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, Initialize(lpInit));
}

HRESULT CXFSPINEx::Reset()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, Reset());
}

HRESULT CXFSPINEx::HsmSetTData(LPWFSXDATA lpxTData)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, HsmSetTData(lpxTData));
}

HRESULT CXFSPINEx::SecureMsgSend(LPWFSPINSECMSG lpSecMsgIn)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SecureMsgSend(lpSecMsgIn));
}

HRESULT CXFSPINEx::SecureMsgReceive(LPWFSPINSECMSG lpSecMsgIn)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SecureMsgReceive(lpSecMsgIn));
}

HRESULT CXFSPINEx::GetJournal(LPWORD lpwProtocol)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, GetJournal(lpwProtocol));
}

HRESULT CXFSPINEx::ImportKeyEx(LPWFSPINIMPORTKEYEX lpImportKeyEx)	
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ImportKeyEx(lpImportKeyEx));
}

HRESULT CXFSPINEx::EncIo(LPWFSPINENCIO lpEncIo)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, EncIo(lpEncIo));
}

HRESULT CXFSPINEx::HsmInit(LPWFSPINHSMINIT lpHsmInit)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, HsmInit(lpHsmInit));
}

HRESULT CXFSPINEx::SecureKeyEntry(LPWFSPINSECUREKEYENTRY lpSecureKeyEntry)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SecureKeyEntry(lpSecureKeyEntry));
}

HRESULT CXFSPINEx::GenerateKCV(LPWFSPINGENERATEKCV lpGenerateKCV)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, GenerateKCV(lpGenerateKCV));
}

HRESULT CXFSPINEx::SetGuidanceLight(LPWFSPINSETGUIDLIGHT lpSetGuidLight)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SetGuidanceLight(lpSetGuidLight));
}

HRESULT CXFSPINEx::MaintainPin(LPWFSPINMAINTAINPIN lpMaintainPinIn)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, MaintainPin(lpMaintainPinIn));
}

HRESULT CXFSPINEx::KeyPressBeep(LPWORD lpwMode)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, KeyPressBeep(lpwMode));
}

HRESULT CXFSPINEx::SetPinBlockData(LPWFSPINBLOCK lpPinSetBlockData)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SetPinBlockData(lpPinSetBlockData));
}

HRESULT CXFSPINEx::SetLogicalHsm(LPWFSPINHSMIDENTIFIER lpSetHsm)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SetLogicalHsm(lpSetHsm));
}

HRESULT CXFSPINEx::ImportKeyBlock(LPWFSPINIMPORTKEYBLOCK lpImportKeyBlock)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ImportKeyBlock(lpImportKeyBlock));
}

HRESULT CXFSPINEx::PowerSaveControl(LPWFSPINPOWERSAVECONTROL lpPowerSaveControl)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, PowerSaveControl(lpPowerSaveControl));
}

HRESULT CXFSPINEx::DefineLayout(LPWFSPINLAYOUT *lppLayout)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, DefineLayout(lppLayout));
}

HRESULT CXFSPINEx::StartAuthenticate(LPWFSPINSTARTAUTHENTICATE lpSA)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, StartAuthenticate(lpSA));
}

HRESULT CXFSPINEx::Authenticate(LPWFSPINAUTHENTICATE lpAuth)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, Authenticate(lpAuth));
}

HRESULT CXFSPINEx::GetPinBlockEx(LPWFSPINBLOCKEX lpPinBlockEx)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, GetPinBlockEx(lpPinBlockEx));
}

HRESULT CXFSPINEx::SynchronizeCmd(LPWFSPINSYNCHRONIZECOMMAND lpSynCmd, REQUESTID RequestID)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SynchronizeCmd(lpSynCmd, RequestID));
}

//===========================================================================//
//                       RKL(RSA and Certificate), EMV                       //
//===========================================================================//
HRESULT CXFSPINEx::StartKeyExchange()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, StartKeyExchange());
}

HRESULT CXFSPINEx::ImportRsaPublicKey(LPWFSPINIMPORTRSAPUBLICKEY lpImportRsaPublicKey)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ImportRsaPublicKey(lpImportRsaPublicKey));
}

HRESULT CXFSPINEx::ExportRsaIssuerSignedItem(LPWFSPINEXPORTRSAISSUERSIGNEDITEM lpExportRsaIssuerSignedItem)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ExportRsaIssuerSignedItem(lpExportRsaIssuerSignedItem));
}

HRESULT CXFSPINEx::ImportRsaSignedDesKey(LPWFSPINIMPORTRSASIGNEDDESKEY lpImportRsaSignedDesKey)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ImportRsaSignedDesKey(lpImportRsaSignedDesKey));
}

HRESULT CXFSPINEx::GenerateRsaKeyPair(LPWFSPINGENERATERSAKEYPAIR lpGenerateRsaKeyPair)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, GenerateRsaKeyPair(lpGenerateRsaKeyPair));
}

HRESULT CXFSPINEx::ExportRsaEppSignedItem(LPWFSPINEXPORTRSAEPPSIGNEDITEM lpExportRsaEppSignedItem)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ExportRsaEppSignedItem(lpExportRsaEppSignedItem));
}

HRESULT CXFSPINEx::LoadCertificate(LPWFSPINLOADCERTIFICATE lpLoadCertificate)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, LoadCertificate(lpLoadCertificate));
}

HRESULT CXFSPINEx::GetCertificate(LPWFSPINGETCERTIFICATE lpGetCertificate)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, GetCertificate(lpGetCertificate));
}

HRESULT CXFSPINEx::ReplaceCertificate(LPWFSPINREPLACECERTIFICATE lpReplaceCertificate)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ReplaceCertificate(lpReplaceCertificate));
}

HRESULT CXFSPINEx::ImportRsaEncipheredPkcs7Key(LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEY lpImportRsaEncipheredPkcs7Key)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ImportRsaEncipheredPkcs7Key(lpImportRsaEncipheredPkcs7Key));
}

HRESULT CXFSPINEx::LoadCertificateEx(LPWFSPINLOADCERTIFICATEEX lpLCEx)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, LoadCertificateEx(lpLCEx));
}

HRESULT CXFSPINEx::ImportRsaEncipheredPkcs7KeyEx(LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEYEX lpIREPKEx)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ImportRsaEncipheredPkcs7KeyEx(lpIREPKEx));
}

HRESULT CXFSPINEx::EmvImportPublicKey(LPWFSPINEMVIMPORTPUBLICKEY lpEmvImportPublicKey)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, EmvImportPublicKey(lpEmvImportPublicKey));
}

HRESULT CXFSPINEx::Digest(LPWFSPINDIGEST lpDigest)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, Digest(lpDigest));
}


//===========================================================================//
//                           China Protocol in ENC IO                        //
//===========================================================================//
HRESULT CXFSPINEx::ENC_China(WORD wProtocol, LPVOID lpData, ULONG ulDataLength)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, ENC_China(wProtocol, lpData, ulDataLength));
}

HRESULT CXFSPINEx::SM3_Digest(LPPROTCHNDIGESTIN lpDigest)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM3_Digest(lpDigest));
}

HRESULT CXFSPINEx::SM2_SetPara(LPPROTCHNSM2ALGORITHMPARAMIN lpSM2)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM2_SetPara(lpSM2));
}

HRESULT CXFSPINEx::SM2_ImportPK(LPPROTCHNIMPORTSM2PUBLICKEYIN lpSM2ImpPK)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM2_ImportPK(lpSM2ImpPK));
}

HRESULT CXFSPINEx::SM2_Sign(LPPROTCHNSIGNIN lpSign)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM2_Sign(lpSign));
}

HRESULT CXFSPINEx::SM2_Verify(LPPROTCHNVERIFYIN lpVerify)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM2_Verify(lpVerify));
}

HRESULT CXFSPINEx::SM2_GenerateKeyPair(LPPROTCHNGENERATESM2KEYPAIRIN lpGenKey)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM2_GenerateKeyPair(lpGenKey));
}

HRESULT CXFSPINEx::SM2_ExportKey(LPPROTCHNEXPORTSM2EPPSIGNEDITEMIN lpExpKey)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM2_ExportKey(lpExpKey));
}

HRESULT CXFSPINEx::SM2_ImportSM4Key(LPPROTCHNIMPORTSM2SIGNEDSM4KEY lpSM4Key)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM2_ImportSM4Key(lpSM4Key));
}

HRESULT CXFSPINEx::SM2_KeyExchangeB(LPWFSPINSM2KEYEXBIN lpKeyExcB)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM2_KeyExchangeB(lpKeyExcB));
}

HRESULT CXFSPINEx::SM2_KeyAgreementB(LPWFSPINSM2KEYAGREEMENTBIN lpKeyAgreeB)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_CALL(m_pSPBase, SM2_KeyAgreementB(lpKeyAgreeB));
}


//===========================================================================//
//                            Events                                         //
//===========================================================================//
void CXFSPINEx::FireKey(WORD wComplete, ULONG ulDigit)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireKey(wComplete, ulDigit));
}

void CXFSPINEx::FireInitialized(LPBYTE lpbIdentData, USHORT usIdentLength, LPBYTE lpbKeyData, USHORT usKeyLength)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireInitialized(lpbIdentData, usIdentLength, lpbKeyData, usKeyLength));
}

void CXFSPINEx::FireIllegalKeyAccess(LPSTR szKeyName, LONG lErrorCode)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireIllegalKeyAccess(szKeyName, lErrorCode));
}

void CXFSPINEx::FireOptRequired()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireOptRequired());
}

void CXFSPINEx::FireCertificateChange(WORD wCertificateChange)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireCertificateChange(wCertificateChange));
}

void CXFSPINEx::FireHsmTDataChanged(LPBYTE lpbData, USHORT usLength)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireHsmTDataChanged(lpbData, usLength));
}

void CXFSPINEx::FireHsmChanged(WORD wHSMSerialNumber)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireHsmChanged(wHSMSerialNumber));
}

void CXFSPINEx::FireEnterData()
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireEnterData());
}

void CXFSPINEx::FireDevicePosition(WORD wPosition)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireDevicePosition(wPosition));
}

void CXFSPINEx::FirePowerSaveChange(USHORT usPowerSaveRecoveryTime)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FirePowerSaveChange(usPowerSaveRecoveryTime));
}

void CXFSPINEx::FireLayout(DWORD dwEntryMode, USHORT usNumberOfFrames, LPWFSPINFRAME *lppFrames)
{
	RLog2(LOG_LEVEL_COMMON, "Enter:");
	SAFE_EXEC(m_pSPBase, FireLayout(dwEntryMode, usNumberOfFrames, lppFrames));
}

