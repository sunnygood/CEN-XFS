/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSPINBase.cpp
* Description: base class for XFS PIN
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#include "CXFSPINBase.h"


CXFSPINBase::CXFSPINBase()
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
	m_pPinpad = NULL;
	m_bIsImportKeyExForSm4 = 0;
	m_bIsLongPressedTimeout = 0;
	m_bIsAysnCancelClearPins = 0;
	m_bIsTimeoutClearPins = 0;
	m_bIsRKLNeedRandom = 0;
	m_eKCVMode = KCVZERO;
	m_nKCVLen = 0;	
	m_nDESMacAlg = MAC_X9;
	m_nTDESMacAlg = MAC_CBC;
	m_nSM4MacAlg = SM4MAC_CBC;
	m_nEcho = 0;
	m_cEchoHW = 0;
	m_cTimeOutCode   = 0x80;
	m_cLongPressCode = 0x81;
	m_n00[0] = m_n00[1] = m_n00[2] = 1;
	m_n000[0] = m_n000[1] = m_n000[2] =1;

	m_CCaps.AddEMVImpScheme(WFS_PIN_EMV_IMPORT_PLAIN_CA);
	m_CCaps.AddEMVImpScheme(WFS_PIN_EMV_IMPORT_CHKSUM_CA);
	m_CCaps.AddEMVImpScheme(WFS_PIN_EMV_IMPORT_EPI_CA);
	m_CCaps.AddEMVImpScheme(WFS_PIN_EMV_IMPORT_ISSUER);
	m_CCaps.AddEMVImpScheme(WFS_PIN_EMV_IMPORT_ICC);
	m_CCaps.AddEMVImpScheme(WFS_PIN_EMV_IMPORT_ICC_PIN);
	m_CCaps.AddEMVImpScheme(WFS_PIN_EMV_IMPORT_PKCSV1_5_CA);

	memset(&m_stPinCaps, 0, sizeof(PINCAPS));
}

CXFSPINBase::~CXFSPINBase()
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
}

//===========================================================================//
//                          Base class virtual function                      //
//===========================================================================//
LPVOID CXFSPINBase::GetInterface(int nIndex)
{
	switch(nIndex)
	{
	case II_SUBCLASS:
		return dynamic_cast<LPVOID>(dynamic_cast<IXFSPINBase *>(this));
	case II_SDK:
		return dynamic_cast<LPVOID>(m_pPinpad);
	default:
		return CXFSSPFace::GetInterface(nIndex);
	}
}

HRESULT CXFSPINBase::OnSPStart()
{
	RLog2(LOG_LEVEL_COMMON, "Start:");

	m_PressKey.Init(m_szServiceName, m_pSPManage);
	Config();
	UpdateStatus();

	m_pSPKernel->RegisterParallerExecCmd(WFS_CMD_PIN_CRYPT, WFS_CMD_PIN_GET_DATA);
	m_pSPKernel->RegisterParallerExecCmd(WFS_CMD_PIN_CRYPT, WFS_CMD_PIN_GET_PIN);
	m_pSPKernel->RegisterParallerExecCmd(WFS_CMD_PIN_SYNCHRONIZE_COMMAND, 0);

	m_pSPKernel->SetTimer(TIMEID_UPDATE_STATUS, m_dwUpdateSatusElapse);

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::OnSPExit()
{
	RLog2(LOG_LEVEL_COMMON, "Start:");

	long nRet = WFS_SUCCESS;

	m_pSPKernel->KillTimer(TIMEID_UPDATE_STATUS);

	if(NULL != m_pPinpad)
	{
		if(nRet = m_pPinpad->Close())
		{
			RLog2(LOG_LEVEL_ERROR, "close pinpad fail, nRet=[%d].", nRet);
			return WFS_ERR_INTERNAL_ERROR;
		}
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return 0;
}

HRESULT CXFSPINBase::OnGetInfo(DWORD dwCategory, LPVOID lpQueryDetail, REQUESTID RequestID)
{
	RLog2(LOG_LEVEL_DEBUG, "Start: dwCategory=%d RequestID=%d", dwCategory, RequestID);

	HRESULT hRes = WFS_SUCCESS;
	switch(dwCategory)
	{
	case WFS_INF_PIN_STATUS:
	{
		hRes = Status();
		if(hRes == WFS_SUCCESS)
		{
			m_CStatus.lpszExtra = (LPTSTR)m_mapStatusExtra;
			m_pSPKernel->SetResult((LPWFSPINSTATUS)m_CStatus, RequestID);
		}
		break;
	}
	case WFS_INF_PIN_CAPABILITIES: 
		hRes = Capabilities();
		if(hRes == WFS_SUCCESS)
		{
			m_CCaps.lpszExtra = (LPTSTR)m_mapCapsExtra;
			m_pSPKernel->SetResult((LPWFSPINCAPS)m_CCaps, RequestID);
		}
		break;
	case WFS_INF_PIN_KEY_DETAIL: 
		hRes = KeyDetail(static_cast<LPSTR>(lpQueryDetail));
		if(hRes == WFS_SUCCESS)
		{
			m_pSPKernel->SetResult((LPWFSPINKEYDETAIL*)m_CKeyDetail, RequestID);
		}
		break;
	case WFS_INF_PIN_FUNCKEY_DETAIL: 
		hRes = FuncKeyDetail(static_cast<LPULONG>(lpQueryDetail));
 		if(hRes == WFS_SUCCESS)
		{
 			m_pSPKernel->SetResult((LPWFSPINFUNCKEYDETAIL)m_CFuncKeyDetail, RequestID);
		}
		break;
	case WFS_INF_PIN_HSM_TDATA: 
		hRes = HsmTData();
		if(hRes == WFS_SUCCESS)
		{
			m_pSPKernel->SetResult((LPWFSXDATA)m_CHsmTData, RequestID);
		}
		break;
	case WFS_INF_PIN_KEY_DETAIL_EX: 
		hRes = KeyDetailEx(static_cast<LPSTR>(lpQueryDetail));
		if(hRes == WFS_SUCCESS)
		{
			m_pSPKernel->SetResult((LPWFSPINKEYDETAILEX*)m_CKeyDetailEx, RequestID);
		}
		break;
	case WFS_INF_PIN_SECUREKEY_DETAIL: 
		hRes = SecureKeyDetail();
		if(hRes == WFS_SUCCESS)
		{
			m_pSPKernel->SetResult((LPWFSPINSECUREKEYDETAIL)m_CSecureKeyDetail, RequestID);
		}
		break;
	case WFS_INF_PIN_QUERY_LOGICAL_HSM_DETAIL: 
		hRes = QueryLogicalHsmDetail();
		if(hRes == WFS_SUCCESS)
		{
			m_pSPKernel->SetResult((LPWFSPINHSMDETAIL)m_CQueryLogicalHsmDetail, RequestID);
		}
		break;
	case WFS_INF_PIN_QUERY_PCIPTS_DEVICE_ID: 
		hRes = QueryPciptsDeviceId();
		if(hRes == WFS_SUCCESS)
		{
			m_pSPKernel->SetResult((LPWFSPINPCIPTSDEVICEID)m_CQueryPciptsDeviceId, RequestID);
		}
		break;
	case WFS_INF_PIN_GET_LAYOUT: 
		hRes = GetLayout(static_cast<LPWFSPINGETLAYOUT>(lpQueryDetail));
		if(hRes == WFS_SUCCESS)
		{
			m_pSPKernel->SetResult((LPWFSPINLAYOUT*)m_CLayout, RequestID);
		}
		break;
	default:
		hRes = WFS_ERR_UNSUPP_CATEGORY;
	}

	return hRes;
}

HRESULT CXFSPINBase::OnExecute(DWORD dwCommand, LPVOID lpCmdData, REQUESTID RequestID)
{	
	RLog2(LOG_LEVEL_DEBUG, "Start: dwCommand=%d RequestID=%d", dwCommand, RequestID);
	if(m_CStatus.fwDevice == WFS_PIN_DEVOFFLINE) // || m_CStatus.fwDevice == WFS_PIN_DEVHWERROR
	{
		return WFS_ERR_HARDWARE_ERROR;
	}

	HRESULT hRes = WFS_SUCCESS;
	switch(dwCommand)
	{
	case WFS_CMD_PIN_CRYPT:
	{
		hRes = Crypt(static_cast<LPWFSPINCRYPT>(lpCmdData));
		if(hRes == WFS_SUCCESS)
		{
			m_pSPKernel->SetResult((LPWFSXDATA)m_CryptResult, RequestID);
		}
		break;
	}
	case WFS_CMD_PIN_IMPORT_KEY:
		hRes = ImportKey(static_cast<LPWFSPINIMPORT>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSXDATA)m_XDataResult, RequestID);
		break;
	case WFS_CMD_PIN_DERIVE_KEY:
		hRes = DeriveKey(static_cast<LPWFSPINDERIVE>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_GET_DATA: 
		hRes = GetData(static_cast<LPWFSPINGETDATA>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINDATA)m_GetDataResult, RequestID);
		break;
	case WFS_CMD_PIN_GET_PIN: 
		hRes = GetPin(static_cast<LPWFSPINGETPIN>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINENTRY)m_GetPinResult, RequestID);
		break;
	case WFS_CMD_PIN_GET_PINBLOCK: 
		hRes = GetPinBlock(static_cast<LPWFSPINBLOCK>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSXDATA)m_XDataResult, RequestID);
		break;
	case WFS_CMD_PIN_CREATE_OFFSET: 
		hRes = CreateOffset(static_cast<LPWFSPINCREATEOFFSET>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPSTR)m_CharResult, RequestID);
		break;
	case WFS_CMD_PIN_LOCAL_DES: 
		hRes = LocalDes(static_cast<LPWFSPINLOCALDES>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(&m_BoolResult, RequestID);
		break;
	case WFS_CMD_PIN_LOCAL_EUROCHEQUE: 
		hRes = LocalEurocheque(static_cast<LPWFSPINLOCALEUROCHEQUE>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(&m_BoolResult, RequestID);
		break;
	case WFS_CMD_PIN_LOCAL_VISA: 
		hRes = LocalVisa(static_cast<LPWFSPINLOCALVISA>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(&m_BoolResult, RequestID);
		break;
	case WFS_CMD_PIN_LOCAL_BANKSYS: 
		hRes = LocalBankSys(static_cast<LPWFSPINLOCALBANKSYS>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(&m_BoolResult, RequestID);
		break;
	case WFS_CMD_PIN_BANKSYS_IO: 
		hRes = BankSysIo(static_cast<LPWFSPINBANKSYSIO>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINBANKSYSIO)m_BankSysIoResult, RequestID);
		break;
	case WFS_CMD_PIN_PRESENT_IDC: 
		hRes = PresentIDC(static_cast<LPWFSPINPRESENTIDC>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINPRESENTRESULT)m_PresentIDCResult, RequestID);
		break;
	case WFS_CMD_PIN_INITIALIZATION: 
		hRes = Initialize(static_cast<LPWFSPININIT>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_RESET:
		hRes = Reset();
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_HSM_SET_TDATA: 
		hRes = HsmSetTData(static_cast<LPWFSXDATA>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_SECURE_MSG_SEND: 
		hRes = SecureMsgSend(static_cast<LPWFSPINSECMSG>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINSECMSG)m_SecureMsgSendResult, RequestID);
		break;
	case WFS_CMD_PIN_SECURE_MSG_RECEIVE: 
		hRes = SecureMsgReceive(static_cast<LPWFSPINSECMSG>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_GET_JOURNAL: 
		hRes = GetJournal(static_cast<LPWORD>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSXDATA)m_XDataResult, RequestID);
		break;
	case WFS_CMD_PIN_IMPORT_KEY_EX: 
		hRes = ImportKeyEx(static_cast<LPWFSPINIMPORTKEYEX>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_ENC_IO: 
		hRes = EncIo(static_cast<LPWFSPINENCIO>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINENCIO)m_EncIOResult, RequestID);
		break;
	case WFS_CMD_PIN_HSM_INIT: 
		hRes = HsmInit(static_cast<LPWFSPINHSMINIT>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_SECUREKEY_ENTRY: 
		hRes = SecureKeyEntry(static_cast<LPWFSPINSECUREKEYENTRY>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINSECUREKEYENTRYOUT)m_SecureKeyEntryResult, RequestID);
		break;
	case WFS_CMD_PIN_GENERATE_KCV: 
		hRes = GenerateKCV(static_cast<LPWFSPINGENERATEKCV>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINKCV)m_GenerateKcvResult, RequestID);
		break;
	case WFS_CMD_PIN_SET_GUIDANCE_LIGHT: 
		hRes = SetGuidanceLight(static_cast<LPWFSPINSETGUIDLIGHT>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_MAINTAIN_PIN: 
		hRes = MaintainPin(static_cast<LPWFSPINMAINTAINPIN>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_KEYPRESS_BEEP: 
		hRes = KeyPressBeep(static_cast<LPWORD>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_SET_PINBLOCK_DATA: 
		hRes = SetPinBlockData(static_cast<LPWFSPINBLOCK>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_SET_LOGICAL_HSM: 
		hRes = SetLogicalHsm(static_cast<LPWFSPINHSMIDENTIFIER>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_IMPORT_KEYBLOCK: 
		hRes = ImportKeyBlock(static_cast<LPWFSPINIMPORTKEYBLOCK>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_POWER_SAVE_CONTROL: 
		hRes = PowerSaveControl(static_cast<LPWFSPINPOWERSAVECONTROL>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_START_KEY_EXCHANGE: 
		hRes = StartKeyExchange();
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINSTARTKEYEXCHANGE)m_StartKeyExchangeResult, RequestID);
		break;
	case WFS_CMD_PIN_IMPORT_RSA_PUBLIC_KEY: 
		hRes = ImportRsaPublicKey(static_cast<LPWFSPINIMPORTRSAPUBLICKEY>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINIMPORTRSAPUBLICKEYOUTPUT)m_ImportRsaPublicKeyResult, RequestID);
		break;
	case WFS_CMD_PIN_EXPORT_RSA_ISSUER_SIGNED_ITEM: 
		hRes = ExportRsaIssuerSignedItem(static_cast<LPWFSPINEXPORTRSAISSUERSIGNEDITEM>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINEXPORTRSAISSUERSIGNEDITEMOUTPUT)m_ExportRsaIssureSignedItemResult, RequestID);
		break;
	case WFS_CMD_PIN_IMPORT_RSA_SIGNED_DES_KEY: 
		hRes = ImportRsaSignedDesKey(static_cast<LPWFSPINIMPORTRSASIGNEDDESKEY>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINIMPORTRSASIGNEDDESKEYOUTPUT)m_ImportRsaSignedDesKeyResult, RequestID);
		break;
	case WFS_CMD_PIN_GENERATE_RSA_KEY_PAIR: 
		hRes = GenerateRsaKeyPair(static_cast<LPWFSPINGENERATERSAKEYPAIR>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_EXPORT_RSA_EPP_SIGNED_ITEM: 
		hRes = ExportRsaEppSignedItem(static_cast<LPWFSPINEXPORTRSAEPPSIGNEDITEM>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINEXPORTRSAEPPSIGNEDITEMOUTPUT)m_ExportRsaEppSignedItemResult, RequestID);
		break;
	case WFS_CMD_PIN_LOAD_CERTIFICATE: 
		hRes = LoadCertificate(static_cast<LPWFSPINLOADCERTIFICATE>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINLOADCERTIFICATEOUTPUT)m_LoadCertificateResult, RequestID);
		break;
	case WFS_CMD_PIN_GET_CERTIFICATE: 
		hRes = GetCertificate(static_cast<LPWFSPINGETCERTIFICATE>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINGETCERTIFICATEOUTPUT)m_GetCertificateResult, RequestID);
		break;
	case WFS_CMD_PIN_REPLACE_CERTIFICATE: 
		hRes = ReplaceCertificate(static_cast<LPWFSPINREPLACECERTIFICATE>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINREPLACECERTIFICATEOUTPUT)m_ReplaceCertificateResult, RequestID);
		break;
	case WFS_CMD_PIN_IMPORT_RSA_ENCIPHERED_PKCS7_KEY: 
		hRes = ImportRsaEncipheredPkcs7Key(static_cast<LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEY>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEYOUTPUT)m_ImportRsaEncipheredPkcs7KeyResult, RequestID);
		break;
	case WFS_CMD_PIN_EMV_IMPORT_PUBLIC_KEY: 
		hRes = EmvImportPublicKey(static_cast<LPWFSPINEMVIMPORTPUBLICKEY>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINEMVIMPORTPUBLICKEYOUTPUT)m_EmvImportPublicKeyResult, RequestID);
		break;
	case WFS_CMD_PIN_DIGEST: 
		hRes = Digest(static_cast<LPWFSPINDIGEST>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSPINDIGESTOUTPUT)m_DigestResult, RequestID);
		break;
	case WFS_CMD_PIN_LOAD_CERTIFICATE_EX: 
		hRes = LoadCertificateEx(static_cast<LPWFSPINLOADCERTIFICATEEX>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_IMPORT_RSA_ENCIPHERED_PKCS7_KEY_EX: 
		hRes = ImportRsaEncipheredPkcs7KeyEx(static_cast<LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEYEX>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_DEFINE_LAYOUT: 
		hRes = DefineLayout(static_cast<LPWFSPINLAYOUT*>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_START_AUTHENTICATE: 
		hRes = StartAuthenticate(static_cast<LPWFSPINSTARTAUTHENTICATE>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_AUTHENTICATE: 
		hRes = Authenticate(static_cast<LPWFSPINAUTHENTICATE>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	case WFS_CMD_PIN_GET_PINBLOCK_EX: 
		hRes = GetPinBlockEx(static_cast<LPWFSPINBLOCKEX>(lpCmdData));
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult((LPWFSXDATA)m_XDataResult, RequestID);
		break;
	case WFS_CMD_PIN_SYNCHRONIZE_COMMAND: 
		hRes = SynchronizeCmd(static_cast<LPWFSPINSYNCHRONIZECOMMAND>(lpCmdData), RequestID);
		if(hRes == WFS_SUCCESS)
			m_pSPKernel->SetResult(NULL, RequestID);
		break;
	default:
		hRes = WFS_ERR_UNSUPP_COMMAND;
	}

	return hRes;
}

HRESULT CXFSPINBase::OnTimer(UINT nTimerID, LPVOID lpContext)
{
	DLog2(LOG_LEVEL_DEBUG, "nTimerID=%d lpContext=0x%X", nTimerID, lpContext);

	HRESULT hRes = WFS_ERR_INVALID_TIMER;
	if(nTimerID == TIMEID_UPDATE_STATUS)
	{
		hRes = UpdateStatus();
	}

	return hRes;
}

//===========================================================================//
//                          GetInfo Commands                                 //
//===========================================================================//
HRESULT CXFSPINBase::Status()
{
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::Capabilities()
{
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::KeyDetail(LPSTR lpszKeyName)
{
	if(NULL == m_pPinpad)
	{
		return WFS_ERR_PIN_KEYNOTFOUND;
	}

	long nRet = 0;
	PINKEYDETAIL stDetail;

	m_CKeyDetail.Clear();
	if(NULL != lpszKeyName && strlen(lpszKeyName) > 0)
	{
		nRet = m_pPinpad->XFS_GetKeyDetail(lpszKeyName, &stDetail);
		if(nRet == 0)
		{
			m_CKeyDetail.Add(stDetail.szKeyName, stDetail.dwKeyAttr, stDetail.bLoaded);
		}
		else
		{
			nRet = WFS_ERR_PIN_KEYNOTFOUND;
		}

		return nRet;
	}
	else
	{
		for(WORD i = 0; i < m_stPinCaps.usKeyNum; ++i)
		{
			nRet = m_pPinpad->XFS_GetKeyDetail(i, &stDetail);
			if(nRet == 0)
			{		
				m_CKeyDetail.Add(stDetail.szKeyName, stDetail.dwKeyAttr, stDetail.bLoaded);
				if(strlen(stDetail.szKeyName) == 0)
				{
					RLog2(LOG_LEVEL_DEBUG, "Index=%d wKeyLen=%d dwKeyAttr=%d wKeyId=%d", i, stDetail.wKeyLen, stDetail.dwKeyAttr, stDetail.wKeyId);
				}
			}
		}

		return WFS_SUCCESS;
	}
}

HRESULT CXFSPINBase::FuncKeyDetail(LPULONG lpulFDKMask)
{
	return WFS_ERR_UNSUPP_CATEGORY;
}

HRESULT CXFSPINBase::HsmTData()
{
	return WFS_ERR_UNSUPP_CATEGORY;
}

HRESULT CXFSPINBase::KeyDetailEx(LPSTR lpszKeyName)
{
	if(NULL == m_pPinpad)
	{
		return WFS_ERR_PIN_KEYNOTFOUND;
	}

	long nRet = 0;
	PINKEYDETAIL stDetail;

	m_CKeyDetailEx.Clear();	
	if(NULL != lpszKeyName && strlen(lpszKeyName) > 0)
	{
		nRet = m_pPinpad->XFS_GetKeyDetail(lpszKeyName, &stDetail);
		if(nRet == 0)
		{
			m_CKeyDetailEx.Add(stDetail.szKeyName, stDetail.dwKeyAttr, stDetail.bLoaded, NULL, stDetail.wKeyLen, 
				stDetail.bGeneration, stDetail.bVersion, stDetail.bActivatingDate, stDetail.bExpiryDate);
		}
		else
		{
			nRet = WFS_ERR_PIN_KEYNOTFOUND;
		}

		return nRet;
	}
	else
	{
		for(WORD i = 0; i < m_stPinCaps.usKeyNum; ++i)
		{
			nRet = m_pPinpad->XFS_GetKeyDetail(i, &stDetail);
			if(nRet == 0)
			{
				m_CKeyDetailEx.Add(stDetail.szKeyName, stDetail.dwKeyAttr, stDetail.bLoaded, NULL, stDetail.wKeyLen, 
					stDetail.bGeneration, stDetail.bVersion, stDetail.bActivatingDate, stDetail.bExpiryDate);
			}
		}

		return WFS_SUCCESS;
	}
}

HRESULT CXFSPINBase::SecureKeyDetail()
{
	return WFS_ERR_UNSUPP_CATEGORY;
}

HRESULT CXFSPINBase::QueryLogicalHsmDetail()
{
	return WFS_ERR_UNSUPP_CATEGORY;
}

HRESULT CXFSPINBase::QueryPciptsDeviceId()
{
	return WFS_ERR_UNSUPP_CATEGORY;
}

HRESULT CXFSPINBase::GetLayout(LPWFSPINGETLAYOUT lpLayout)
{
	return WFS_ERR_UNSUPP_CATEGORY;
}

//===========================================================================//
//                          Exectue Commands                                 //
//===========================================================================//
HRESULT CXFSPINBase::Crypt(LPWFSPINCRYPT lpCrypt)
{
	long nRet = 0;
	bool bEncrypt = false;
	HRESULT hRes = 0;
	LPBYTE lpResult = NULL;
	WORD wKeyId = KEY_INVALID, wIVKeyId = KEY_INVALID;	
	BYTE lpEncKey[32] = {0}, bEncKeyLen = 0, *lpIV = NULL;

	if(NULL == lpCrypt)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	WORD wMode = lpCrypt->wMode;
	LPSTR lpKey = lpCrypt->lpsKey;
	LPWFSXDATA lpxEncKey = lpCrypt->lpxKeyEncKey;
	WORD wAlgorithm = lpCrypt->wAlgorithm;
	LPSTR lpIVKey = lpCrypt->lpsStartValueKey;
	LPWFSXDATA lpxIVData = lpCrypt->lpxStartValue;
	BYTE Padding = Padding2Bin(lpCrypt->bPadding);
	BYTE bCompression = lpCrypt->bCompression;
	LPWFSXDATA lpxData = lpCrypt->lpxCryptData;

	RLog2(LOG_LEVEL_COMMON, "Start: wMode=[0x%04X],lpKey=[%s],wAlgorithm=[0x%04X],"
				"lpIVKey=[%s],bPadding=[0x%X],bCompression=[0x%X]"
				, wMode, lpKey, wAlgorithm, lpIVKey, Padding, bCompression);

	FAIL_RET(nRet, this, CheckOpen());
	
	switch(wMode)
	{
		case WFS_PIN_MODEENCRYPT:
			bEncrypt = true;
			break;
		case WFS_PIN_MODEDECRYPT:
			bEncrypt = false;
			break;
		case WFS_PIN_MODERANDOM:
		{
			DWORD dwLen = 8;
			lpResult = m_CryptResult.GetBufferSetLength(512);
			if(NULL == lpResult)
			{
				return WFS_ERR_INVALID_BUFFER;
			}

			if(nRet = m_pPinpad->GetRandom(eRAND_EPP, dwLen, (char*)lpResult))
			{
				RLog2(LOG_LEVEL_ERROR, "get random data fail, nRet=[%d].", nRet);
				return Convert2SPError(nRet);
			}

			m_CryptResult.ReleaseBufferSetLength((USHORT)dwLen);
			RLog2(LOG_LEVEL_COMMON, "End:");
			return WFS_SUCCESS;
		}
		default:
			RLog2(LOG_LEVEL_ERROR, "the mode is not supported.");
			return WFS_ERR_PIN_MODENOTSUPPORTED;
	}

	if(lpKey == NULL || lpKey[0] == '\0')
	{
		char szKeyName[128] = {0};
		m_pSPManage->GetServiceNodeStr(m_szServiceName, "CryptDefaultKeyName", szKeyName, 128);

		lpKey = szKeyName;
		if(strlen(lpKey) <= 0)
		{
			RLog2(LOG_LEVEL_ERROR, "The key name string is empty.");
			return WFS_ERR_PIN_INVALIDDATA;
		}
	}

	if(lpxEncKey && lpxEncKey->lpbData && lpxEncKey->usLength > 0)
	{
		bEncKeyLen = (BYTE)lpxEncKey->usLength;
		if(bEncKeyLen != 8 && bEncKeyLen != 16 && bEncKeyLen != 24)
		{
			RLog2(LOG_LEVEL_ERROR, "twice encrypted key length is wrong, real length=[%d].", bEncKeyLen);
			return WFS_ERR_PIN_INVALIDKEYLENGTH;
		}
		memcpy(lpEncKey, lpxEncKey->lpbData, bEncKeyLen);
	}	

	if(!(m_stPinCaps.fwAlgorithms & wAlgorithm))
	{
		RLog2(LOG_LEVEL_ERROR, "the algorithm is not supported, algorithm=[0x%X].", wAlgorithm);
		return WFS_ERR_PIN_ALGORITHMNOTSUPP;
	}

	if(lpIVKey && lpIVKey[0] != '\0')
	{
		FAIL_RET(nRet, this, FindKey(lpIVKey, wIVKeyId, (WFS_PIN_USESVENCKEY)));
	}

	if(lpxIVData && lpxIVData->lpbData)
	{
		if(wAlgorithm == WFS_PIN_CRYPTSM4 || wAlgorithm == WFS_PIN_CRYPTSM4MAC)
		{
			if(lpxIVData->usLength != 16)
			{
				RLog2(LOG_LEVEL_ERROR, "SM4 IV data's length is wrong, real length=[%d].", 
					lpxIVData->usLength);
				return WFS_ERR_PIN_INVALIDKEYLENGTH;
			}
		}
		else
		{
			if(lpxIVData->usLength != 8)
			{
				RLog2(LOG_LEVEL_ERROR, "IV data's length[%d] is wrong, need length=[8].", lpxIVData->usLength);
				return WFS_ERR_PIN_INVALIDKEYLENGTH;
			}
		}	
		lpIV = lpxIVData->lpbData;
	}

	if(lpxData == NULL || lpxData->lpbData == NULL || lpxData->usLength <= 0)
	{
		RLog2(LOG_LEVEL_ERROR, "data is empty.");
		return WFS_ERR_INVALID_DATA;
	}

	if(lpxData->usLength >= 0xFFF0)
	{
		RLog2(LOG_LEVEL_ERROR, "Data is too long.");
		return WFS_ERR_INVALID_DATA;
	}

	DWORD dwDataLen = lpxData->usLength;
	LPBYTE lpbData = lpxData->lpbData;
	
	switch(wAlgorithm)
	{
		case WFS_PIN_CRYPTDESECB:
		case WFS_PIN_CRYPTDESCBC:
		case WFS_PIN_CRYPTDESCFB:
		case WFS_PIN_CRYPTTRIDESECB:
		case WFS_PIN_CRYPTTRIDESCBC:
		case WFS_PIN_CRYPTTRIDESCFB:
		case WFS_PIN_CRYPTSM4:
		{
			FAIL_RET(nRet, this, FindKey(lpKey, wKeyId, (WFS_PIN_USECRYPT)));

			lpResult = m_CryptResult.GetBufferSetLength(dwDataLen + 16);
			if(NULL == lpResult)
			{
				return WFS_ERR_INVALID_BUFFER;
			}
			
			nRet = m_pPinpad->Crypt(wKeyId, ECRYPT(wAlgorithm), (LPCSTR)lpbData, dwDataLen, (char*)lpResult, bEncrypt, (char*)lpIV, wIVKeyId, Padding);
			if(nRet == 0)
			{
				hRes = 0;
				m_CryptResult.ReleaseBufferSetLength((USHORT)dwDataLen);
			}
			else
			{				
				hRes = Convert2SPError(nRet);
				RLog2(LOG_LEVEL_ERROR, "Crypt failed, nRet=[%d].", nRet);
			}
			
			break;
		}
		case WFS_PIN_CRYPTDESMAC:
		case WFS_PIN_CRYPTTRIDESMAC:
		case WFS_PIN_CRYPTSM4MAC:
		{
			FAIL_RET(nRet, this, FindKey(lpKey, wKeyId, (WFS_PIN_USEMACING)));

			if(wAlgorithm == WFS_PIN_CRYPTSM4MAC)
			{
				wAlgorithm = m_nSM4MacAlg;
			}
			else if(wAlgorithm == WFS_PIN_CRYPTDESMAC)
			{
				wAlgorithm = m_nDESMacAlg;
			}
			else
			{
				wAlgorithm = m_nTDESMacAlg;
			}

			//计算MAC时，如果bCompression不为0x00,就要去掉该值
			if(bCompression != 0)
			{
				lpbData = new BYTE[dwDataLen];
				if(NULL == lpbData)
				{
					RLog2(LOG_LEVEL_ERROR, "Alloc buffer failed.");
					return WFS_ERR_INTERNAL_ERROR;
				}

				dwDataLen = 0;
				for(int k = 0; k < lpxData->usLength; ++k)
				{
					if(lpxData->lpbData[k] == bCompression)
					{
						continue;
					}
					lpbData[dwDataLen++] = lpxData->lpbData[k];
				}
			}

			lpResult = m_CryptResult.GetBufferSetLength(128);
			if(NULL == lpResult)
			{
				return WFS_ERR_INVALID_BUFFER;
			}

			nRet = m_pPinpad->CalcMAC(wKeyId, EMAC(wAlgorithm), (LPCSTR)lpbData, dwDataLen, (char*)lpResult, (char*)lpIV, wIVKeyId);
			if(bCompression != 0)
			{
				delete [] lpbData;
			}

			if(nRet == 0)
			{
				hRes = 0;
				m_CryptResult.ReleaseBufferSetLength((USHORT)dwDataLen);
			}
			else
			{
				hRes = Convert2SPError(nRet);
				RLog2(LOG_LEVEL_ERROR, "Calc MAC failed, nRet=[%d].", nRet);
			}

			break;
		}		
		default:
			RLog2(LOG_LEVEL_ERROR, "algorithm is not supported fail, algorithm=[0x%X].", wAlgorithm);
			hRes = WFS_ERR_PIN_ALGORITHMNOTSUPP;
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return hRes;
}

HRESULT CXFSPINBase::ImportKey(LPWFSPINIMPORT lpImport)
{
	if(NULL == lpImport)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	BYTE szKCV[64] = {0};
	LPSTR lpsKey = lpImport->lpsKey;
	LPSTR lpEncKey = lpImport->lpsEncKey;
	DWORD dwUse = lpImport->fwUse;

	RLog2(LOG_LEVEL_COMMON, "Start: lpsKey=[%s],lpEncKey=[%s],wUse=[0x%04X]", lpsKey, lpEncKey, dwUse);

	if(dwUse == 0)
	{
		return DeleteKey(lpsKey);
	}

	DWORD dwUsage = WFS_PIN_USECRYPT | WFS_PIN_USEFUNCTION | WFS_PIN_USEMACING | WFS_PIN_USEKEYENCKEY 
		      | WFS_PIN_USENODUPLICATE | WFS_PIN_USESVENCKEY | WFS_PIN_USECONSTRUCT | WFS_PIN_USESECURECONSTRUCT;
	if(dwUse & (~dwUsage))
	{
		RLog2(LOG_LEVEL_ERROR, "the usage is not support.");
		return WFS_ERR_PIN_USEVIOLATION;
	}

	HRESULT hRes = DownLoadKey(lpsKey, lpEncKey, lpImport->lpxValue, dwUse, lpImport->lpxIdent, (char*)szKCV, m_eKCVMode);
	if(hRes == 0)
	{
		m_pPinpad->GetCapabilities(&m_stPinCaps);
		m_XDataResult.Set(szKCV, m_stPinCaps.ucKCVL);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return hRes;
}

HRESULT CXFSPINBase::DeriveKey(LPWFSPINDERIVE lpDerive)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::GetData(LPWFSPINGETDATA lpPinGetData)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::GetPin(LPWFSPINGETPIN lpGetPin)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::GetPinBlock(LPWFSPINBLOCK lpPinBlock)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::CreateOffset(LPWFSPINCREATEOFFSET lpPinOffset)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::LocalDes(LPWFSPINLOCALDES lpLocalDes)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::LocalEurocheque(LPWFSPINLOCALEUROCHEQUE lpLocalEurocheque)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::LocalVisa(LPWFSPINLOCALVISA lpLocalVisa)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::LocalBankSys(LPWFSPINLOCALBANKSYS lpLocalBanksys)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::BankSysIo(LPWFSPINBANKSYSIO lpBankSysIo)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::PresentIDC(LPWFSPINPRESENTIDC lpPresentIDC)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::Initialize(LPWFSPININIT lpInit)
{
	if(NULL == lpInit)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	LPWFSXDATA lpxId = lpInit->lpxIdent;
	LPWFSXDATA lpxKey = lpInit->lpxKey;
	
	RLog2(LOG_LEVEL_COMMON, "Start:");
	
	long nRet = 0;
	FAIL_RET(nRet, this, CheckOpen());
	
	if(nRet = m_pPinpad->SetCaps(eCAP_KCVL, m_nKCVLen))
	{
		RLog2(LOG_LEVEL_WARN, "set kcv length fail, nRet=[%d]", nRet);
		//return Convert2SPError(nRet);
	}

	if(nRet = m_pPinpad->Init()) 
	{ 
		RLog2(LOG_LEVEL_ERROR, "initialize fail, nRet=[%d]", nRet);
		return Convert2SPError(nRet);
	}

	FireInitialized(NULL, 0, NULL, 0);
	UpdateStatus();

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::Reset()
{
	RLog2(LOG_LEVEL_COMMON, "Start:");
	
	long nRet = 0;
	FAIL_RET(nRet, this, CheckOpen());

	if(nRet = m_pPinpad->Reset()) 
	{
		RLog2(LOG_LEVEL_ERROR, "reset fail, nRet=[%d]", nRet);
		return Convert2SPError(nRet);
	}

	UpdateStatus();

	RLog2(LOG_LEVEL_COMMON, "End:");
	return  WFS_SUCCESS;
}

HRESULT CXFSPINBase::HsmSetTData(LPWFSXDATA lpxTData)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::SecureMsgSend(LPWFSPINSECMSG lpSecMsgIn)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::SecureMsgReceive(LPWFSPINSECMSG lpSecMsgIn)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::GetJournal(LPWORD lpwProtocol)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::ImportKeyEx(LPWFSPINIMPORTKEYEX lpImportKeyEx)	
{
	if(NULL == lpImportKeyEx)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	BYTE szKCV[64] = {0};
	LPSTR lpsKey = lpImportKeyEx->lpsKey;
	LPSTR lpEncKey = lpImportKeyEx->lpsEncKey;
	DWORD dwUse = lpImportKeyEx->dwUse;
	WORD wKeyCheckMode = lpImportKeyEx->wKeyCheckMode;
	LPWFSXDATA lpxKeyCheckValue = lpImportKeyEx->lpxKeyCheckValue;

	RLog2(LOG_LEVEL_COMMON, "Start: lpsKey=[%s],lpEncKey=[%s],dwUse=[0x%08X]", lpsKey, lpEncKey, dwUse);

	if(dwUse == 0)
	{
		return DeleteKey(lpsKey);
	}

	if(wKeyCheckMode != WFS_PIN_KCVNONE)
	{
		if(lpxKeyCheckValue == NULL || lpxKeyCheckValue->lpbData == NULL || lpxKeyCheckValue->usLength <= 0)
		{
			RLog2(LOG_LEVEL_ERROR, "key check value inputed is empty.");
			return WFS_ERR_PIN_INVALIDDATA;
		}
	}

	m_bIsImportKeyExForSm4 = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsImportKeyExForSm4", 0);
	if(m_bIsImportKeyExForSm4) //处理国密密钥属性
	{
		if(dwUse & ~(WFS_PIN_USECRYPT | WFS_PIN_USEFUNCTION | WFS_PIN_USEMACING | WFS_PIN_USEKEYENCKEY | 
			WFS_PIN_USENODUPLICATE | WFS_PIN_USESVENCKEY | WFS_PIN_USEPINLOCAL))
		{
			RLog2(LOG_LEVEL_ERROR, "the usage is not support.");
			return WFS_ERR_PIN_USEVIOLATION;
		}

		dwUse |= ATTR_SM4;
	}
	else
	{
		if(dwUse & (~(WFS_PIN_USECRYPT | WFS_PIN_USEFUNCTION | WFS_PIN_USEMACING | WFS_PIN_USEKEYENCKEY | 
		         WFS_PIN_USENODUPLICATE | WFS_PIN_USESVENCKEY | WFS_PIN_USEPINLOCAL | 
				 WFS_PIN_USERSAPUBLIC | WFS_PIN_USERSAPRIVATE| WFS_PIN_USECONSTRUCT | WFS_PIN_USESECURECONSTRUCT)))
		{
			RLog2(LOG_LEVEL_ERROR, "the usage is not support.");
			return WFS_ERR_PIN_USEVIOLATION;
		}
	}

	HRESULT hRes = DownLoadKey(lpsKey, lpEncKey, lpImportKeyEx->lpxValue, dwUse, lpImportKeyEx->lpxControlVector, (char*)szKCV, (EKCVMODE)wKeyCheckMode);

	if(hRes == 0 && wKeyCheckMode != WFS_PIN_KCVNONE)
	{
		m_pPinpad->GetCapabilities(&m_stPinCaps);
		long nKCVL = (m_stPinCaps.ucKCVL > lpxKeyCheckValue->usLength) ? lpxKeyCheckValue->usLength : m_stPinCaps.ucKCVL;
		if(memcmp(szKCV, lpxKeyCheckValue->lpbData, nKCVL) != 0)
		{
			RLog2(LOG_LEVEL_ERROR, "kcv check value is not equal.");
			return WFS_ERR_PIN_KEYINVALID;
		}
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return hRes;
}

HRESULT CXFSPINBase::EncIo(LPWFSPINENCIO lpEncIo)
{
	if(NULL == lpEncIo)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	HRESULT hRes = 0;
	WORD wSubProtocol = 0;

	WORD wProtocol = lpEncIo->wProtocol;
	ULONG ulDataLength = lpEncIo->ulDataLength;
	LPVOID lpvData = lpEncIo->lpvData;

	if(NULL == lpvData)
	{
		return WFS_ERR_INVALID_POINTER;
	}
	else
	{
		wSubProtocol = *((LPWORD)lpvData);
	}

	RLog2(LOG_LEVEL_COMMON, "Start: wProtocol=[0x%04x], wSubProtocol=[0x%04x], ulDataLength=[%d].", wProtocol, wSubProtocol, ulDataLength);

	switch(wProtocol)
	{
		case 0x03E7:                //对应Wincore工具传入的garbag
		case WFS_PIN_ENC_PROT_SZZT:
			hRes = ENC_SZZT(wSubProtocol, lpvData, ulDataLength);
			break;
		case WFS_PIN_ENC_PROT_GIECB:
			hRes = ENC_GIECB(wSubProtocol, lpvData, ulDataLength);
			break;
		case WFS_PIN_ENC_PROT_CHN:
			hRes = ENC_China(wSubProtocol, lpvData, ulDataLength);
			break;
		default:
			RLog2(LOG_LEVEL_ERROR, "ENC protocol not supported.");
			hRes = WFS_ERR_PIN_PROTOCOLNOTSUPP;			
	}
	
	RLog2(LOG_LEVEL_COMMON, "End:");
	return hRes;
}

HRESULT CXFSPINBase::HsmInit(LPWFSPINHSMINIT lpHsmInit)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::SecureKeyEntry(LPWFSPINSECUREKEYENTRY lpSecureKeyEntry)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::GenerateKCV(LPWFSPINGENERATEKCV lpGenerateKCV)
{
	DWORD dwAttr = 0;
	WORD wKeyId = KEY_INVALID;
	long nRet = 0;
	char szKcv[256] = {0};

	if(NULL == lpGenerateKCV)
		return WFS_ERR_INVALID_POINTER;

	LPSTR lpsKey = lpGenerateKCV->lpsKey;
	WORD wKeyCheckMode = lpGenerateKCV->wKeyCheckMode;

	RLog2(LOG_LEVEL_COMMON, "Start: lpsKey=[%s],wKeyCheckMode=[0x%04X]", lpsKey, wKeyCheckMode);

	FAIL_RET(nRet, this, CheckOpen());

	FAIL_RET(nRet, this, FindKey(lpsKey, wKeyId));

	if(nRet = m_pPinpad->SetCaps(eCAP_KCVL, m_nKCVLen))
	{
		RLog2(LOG_LEVEL_WARN, "set kcv length fail, nRet=[%d]", nRet);
		//return Convert2SPError(nRet);
	}
	
	if(nRet = m_pPinpad->ReadKeyAttribute(wKeyId, &dwAttr, szKcv, EKCVMODE(wKeyCheckMode)))
	{
		RLog2(LOG_LEVEL_ERROR, "read key attribute fail, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	m_pPinpad->GetCapabilities(&m_stPinCaps);
	m_GenerateKcvResult.Set((LPBYTE)szKcv, m_stPinCaps.ucKCVL);

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SetGuidanceLight(LPWFSPINSETGUIDLIGHT lpSetGuidLight)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::MaintainPin(LPWFSPINMAINTAINPIN lpMaintainPinIn)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::KeyPressBeep(LPWORD lpwMode)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::SetPinBlockData(LPWFSPINBLOCK lpPinSetBlockData)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::SetLogicalHsm(LPWFSPINHSMIDENTIFIER lpSetHsm)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::ImportKeyBlock(LPWFSPINIMPORTKEYBLOCK lpImportKeyBlock)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::PowerSaveControl(LPWFSPINPOWERSAVECONTROL lpPowerSaveControl)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::DefineLayout(LPWFSPINLAYOUT *lppLayout)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::StartAuthenticate(LPWFSPINSTARTAUTHENTICATE lpSA)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::Authenticate(LPWFSPINAUTHENTICATE lpAuth)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::GetPinBlockEx(LPWFSPINBLOCKEX lpPinBlockEx)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::SynchronizeCmd(LPWFSPINSYNCHRONIZECOMMAND lpSynCmd, REQUESTID RequestID)
{
	if(NULL == lpSynCmd)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	if(lpSynCmd->dwCommand == WFS_CMD_PIN_SYNCHRONIZE_COMMAND)
	{
		return WFS_ERR_CFG_INVALID_VALUE;
	}

	return OnExecute(lpSynCmd->dwCommand, lpSynCmd->lpCmdData, RequestID);
}

//===========================================================================//
//                       RKL(RSA and Certificate), EMV                       //
//===========================================================================//
HRESULT CXFSPINBase::StartKeyExchange()
{
	RLog2(LOG_LEVEL_COMMON, "Start:");

	BYTE Random[512] = {0};
	long nRet = 0;
	DWORD dwRandomLen = m_pSPManage->GetServiceNodeInt(m_szServiceName, "RandomLen", 274);

	FAIL_RET(nRet, this, CheckOpen());

	if(nRet = m_pPinpad->GetRandom(eRAND_RSA, dwRandomLen, (char*)Random))
	{
		RLog2(LOG_LEVEL_ERROR, "get rsa random fail, nRet=[%d]", nRet);
		return Convert2SPError(nRet);
	}
	
	m_StartKeyExchangeResult.Set(Random, dwRandomLen);

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::ImportRsaPublicKey(LPWFSPINIMPORTRSAPUBLICKEY lpImportRsaPublicKey)
{
	long nRet = 0;	
	BYTE szKCV[512] = {0};
	DWORD dwKCVMode = 0, dwKCVLen = 0;
	WORD wKeyId = KEY_INVALID, wSigKeyId = KEY_INVALID;

	if(NULL == lpImportRsaPublicKey)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	LPSTR lpsKey = lpImportRsaPublicKey->lpsKey;
	LPWFSXDATA lpxValue = lpImportRsaPublicKey->lpxValue;
	DWORD dwUse = lpImportRsaPublicKey->dwUse;
	LPSTR lpsSigKey = lpImportRsaPublicKey->lpsSigKey;
	DWORD dwRSASignatureAlgorithm = lpImportRsaPublicKey->dwRSASignatureAlgorithm;
	LPWFSXDATA lpxSignature = lpImportRsaPublicKey->lpxSignature;

	RLog2(LOG_LEVEL_COMMON, "Start: lpsKey=[%s],dwUse=[0x%08X],lpsSigKey=[%s],dwRSASignatureAlgorithm=[0x%08X]"
		    , lpsKey, dwUse, lpsSigKey, dwRSASignatureAlgorithm);

	if(dwUse == 0)
	{
		return DeleteKey(lpsKey);
	}

	//检查签名算法
	if(dwRSASignatureAlgorithm != WFS_PIN_SIGN_NA && 
		 dwRSASignatureAlgorithm != WFS_PIN_SIGN_RSASSA_PKCS1_V1_5 &&
		 dwRSASignatureAlgorithm != WFS_PIN_SIGN_RSASSA_PSS)
	{
		RLog2(LOG_LEVEL_ERROR, "signature check algorithm is wrong.");
		return WFS_ERR_PIN_SIG_NOT_SUPP;
	} 

	//判断数据的有效性
	if(lpxValue == NULL || lpxValue->lpbData == NULL || lpxValue->usLength == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "lpxValue=[null]");
		return WFS_ERR_INVALID_DATA;
	}

	FAIL_RET(nRet, this, CheckOpen());

	ALLOT_KEYID(lpsKey, wKeyId);

	//检查签名密钥和签名数据的正确性
	if(dwRSASignatureAlgorithm != WFS_PIN_SIGN_NA)
	{
		if(lpsSigKey == NULL || lpsSigKey[0] == '\0' || strcmp(lpsSigKey, "_SigIssuerVendor") == 0)
		{
			wSigKeyId = _VENDOR_PK;
		}	
		else if(strcmp(lpsSigKey, "_EPPSignKey") == 0)
		{
			wSigKeyId = _EPP_SIG_PK;
		}
		else
		{
			FAIL_RET(nRet, this, FindKey(lpsSigKey, wSigKeyId, (WFS_PIN_USERSAPUBLICVERIFY)));	
		}

		if(lpxSignature == NULL || lpxSignature->lpbData == NULL || lpxSignature->usLength == 0)
		{
			RLog2(LOG_LEVEL_ERROR, "lpxSignature is empty.");
			return WFS_ERR_PIN_SIGNATUREINVALID;
		}
	}

	if(nRet = m_pPinpad->RSA_ImpRSAKey(wKeyId, (char*)lpxValue->lpbData, lpxValue->usLength, (char*)lpxSignature->lpbData, lpxSignature->usLength,
		ESIG_ALGORITHM(dwRSASignatureAlgorithm), wSigKeyId, &dwKCVMode, (char*)szKCV, &dwKCVLen))
	{
		RLog2(LOG_LEVEL_ERROR, "host public key import failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	m_ImportRsaPublicKeyResult.Set(dwKCVMode, szKCV, (USHORT)dwKCVLen);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::ExportRsaIssuerSignedItem(LPWFSPINEXPORTRSAISSUERSIGNEDITEM lpExportRsaIssuerSignedItem)
{
	long nRet = 0;
	ESIG_ALGORITHM SigAlg = SIGN_NA;
	BYTE lpData[2048] = {0}, lpSignature[1024] = {0};
	WORD wResultLen = 0, wDataLen = 0, wSignatureLen = 0;
	WORD wKeyId = KEY_INVALID;
	
	if(NULL == lpExportRsaIssuerSignedItem)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	WORD wExportItemType = lpExportRsaIssuerSignedItem->wExportItemType;
	LPSTR lpsName = lpExportRsaIssuerSignedItem->lpsName;

	RLog2(LOG_LEVEL_COMMON, "Start: wExportItemType=[0x%04X],lpsName=[%s]" ,wExportItemType, lpsName);

	FAIL_RET(nRet, this, CheckOpen());

	if(wExportItemType == WFS_PIN_EXPORT_PUBLIC_KEY)
	{
		if(lpsName == NULL || lpsName[0] == '\0' || strcmp(lpsName, "_EPPCryptKey") == 0) 
		{	
			wKeyId = _EPP_CRY_PK;
		}
		else if(strcmp(lpsName, "_EPPSignKey") == 0) 
		{
			wKeyId = _EPP_SIG_PK;
		}
		else
		{
			FAIL_RET(nRet, this, FindKey(lpsName, wKeyId, (WFS_PIN_USERSAPUBLIC | WFS_PIN_USERSAPUBLICVERIFY)));
		}		
	}
	else if(wExportItemType == WFS_PIN_EXPORT_EPP_ID)
	{
		wKeyId = _RSA_EPP_ID;
	}
	else
	{
		RLog2(LOG_LEVEL_ERROR, "item type not supported.");
		return WFS_ERR_PIN_INVALIDDATA;
	}

	nRet = m_pPinpad->RSA_Export(wKeyId, (char*)lpData, wDataLen, (char*)lpSignature, wSignatureLen, SigAlg);
	if(nRet)
	{
		RLog2(LOG_LEVEL_ERROR, "Export RSA key failed, nRet=[%d]", nRet);
		return Convert2SPError(nRet);
	}

	m_ExportRsaIssureSignedItemResult.Set(SigAlg, lpData, wDataLen, lpSignature, wSignatureLen);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::ImportRsaSignedDesKey(LPWFSPINIMPORTRSASIGNEDDESKEY lpImportRsaSignedDesKey)
{
	WORD wKeyId = KEY_INVALID, wDecryptKeyId = KEY_INVALID, wSigKeyId = 1;	
	BYTE szKCV[256] = {0};
	int nRet = 0, DataLen = 0;

	if(NULL == lpImportRsaSignedDesKey)
		return WFS_ERR_INVALID_POINTER;

	LPSTR lpsKey = lpImportRsaSignedDesKey->lpsKey;
	LPSTR lpsDecryptKey = lpImportRsaSignedDesKey->lpsDecryptKey;
	DWORD dwRSAEncipherAlgorithm = lpImportRsaSignedDesKey->dwRSAEncipherAlgorithm;
	LPWFSXDATA lpxValue = lpImportRsaSignedDesKey->lpxValue;
	DWORD dwUse = lpImportRsaSignedDesKey->dwUse;
	LPSTR lpsSigKey = lpImportRsaSignedDesKey->lpsSigKey;
	DWORD dwRSASignatureAlgorithm = lpImportRsaSignedDesKey->dwRSASignatureAlgorithm;
	LPWFSXDATA lpxSignature = lpImportRsaSignedDesKey->lpxSignature;

	RLog2(LOG_LEVEL_COMMON, 
		    "Start: lpsKey=[%s],lpsDecryptKey=[%s],dwRSAEncipherAlgorithm=[0x%08X],"
				"dwUse=[0x%08X],lpsSigKey=[%s],dwRSASignatureAlgorithm=[0x%08X]"
		    , lpsKey, lpsDecryptKey, dwRSAEncipherAlgorithm, dwUse, lpsSigKey, dwRSASignatureAlgorithm);

	if(dwUse == 0)
	{
		return DeleteKey(lpsKey);
	}

	//检查签名算法
	if(dwRSASignatureAlgorithm != WFS_PIN_SIGN_NA && 
		 dwRSASignatureAlgorithm != WFS_PIN_SIGN_RSASSA_PKCS1_V1_5 &&
		 dwRSASignatureAlgorithm != WFS_PIN_SIGN_RSASSA_PSS)
	{
		RLog2(LOG_LEVEL_ERROR, "signature check algorithm is wrong.");
		return WFS_ERR_PIN_SIG_NOT_SUPP;
	}

	//检查解密算法
	if(dwRSAEncipherAlgorithm != WFS_PIN_CRYPT_RSAES_PKCS1_V1_5 && dwRSAEncipherAlgorithm != WFS_PIN_CRYPT_RSAES_OAEP)
	{
		RLog2(LOG_LEVEL_ERROR, "decrypt algorithm is not supported.");
		return WFS_ERR_PIN_ALGORITHMNOTSUPP;
	} 

	if(lpxValue == NULL || lpxValue->lpbData == NULL || lpxValue->usLength == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "lpxValue is empty.");
		return WFS_ERR_PIN_INVALIDDATA;
	}

	FAIL_RET(nRet, this, CheckOpen());

	ALLOT_KEYID(lpsKey, wKeyId);

	//默认的加解密密钥对
	if(lpsDecryptKey == NULL || lpsDecryptKey[0] == '\0' || strcmp(lpsDecryptKey, "_EPPCryptKey") == 0)
	{
		wDecryptKeyId = _EPP_CRY_SK;
	}
	else
	{
		FAIL_RET(nRet, this, FindKey(lpsDecryptKey, wDecryptKeyId, (WFS_PIN_USERSAPRIVATE)));
	}	
	
	//检查签名数据
	if(dwRSASignatureAlgorithm != WFS_PIN_SIGN_NA)
	{
		FAIL_RET(nRet, this, FindKey(lpsSigKey, wSigKeyId, (WFS_PIN_USERSAPUBLICVERIFY)));
		
		if(lpxSignature == NULL || lpxSignature->lpbData == NULL || lpxSignature->usLength == 0)
		{
			RLog2(LOG_LEVEL_ERROR, "lpxSignature is empty.");
			return WFS_ERR_PIN_SIGNATUREINVALID;
		}
	}

	if(nRet = m_pPinpad->SetCaps(eCAP_KCVL, m_nKCVLen))
	{
		RLog2(LOG_LEVEL_WARN, "set kcv length fail, nRet=[%d]", nRet);
		//return Convert2SPError(nRet);
	}

	if(nRet = m_pPinpad->RSA_ImpDESKey(wKeyId, dwUse, (char*)lpxValue->lpbData, lpxValue->usLength, (char*)lpxSignature->lpbData, lpxSignature->usLength, 
		(m_bIsRKLNeedRandom ? true : false), ESIG_ALGORITHM(dwRSASignatureAlgorithm), wSigKeyId, wDecryptKeyId, m_eKCVMode, (char*)szKCV))
	{
		RLog2(LOG_LEVEL_ERROR, "import des key fail, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	m_pPinpad->GetCapabilities(&m_stPinCaps);
	m_ImportRsaSignedDesKeyResult.Set(WFS_PIN_KEYDOUBLE, m_eKCVMode, szKCV, m_stPinCaps.ucKCVL);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::GenerateRsaKeyPair(LPWFSPINGENERATERSAKEYPAIR lpGenerateRsaKeyPair)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::ExportRsaEppSignedItem(LPWFSPINEXPORTRSAEPPSIGNEDITEM lpExportRsaEppSignedItem)
{
	ESIG_ALGORITHM SigAlg = SIGN_NA;
	BYTE lpData[2048] = {0}, lpSignature[1024] = {0};
	WORD wResultLen = 0, DataLen = 0, SignatureLen = 0;
	DWORD dwUsage = 0;
	WORD wKeyId = KEY_INVALID, wSigKeyId = KEY_INVALID;
	long nRet = 0;

	if(NULL == lpExportRsaEppSignedItem)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	WORD wExportItemType = lpExportRsaEppSignedItem->wExportItemType;
	LPSTR lpsName = lpExportRsaEppSignedItem->lpsName;
	LPSTR lpsSigKey = lpExportRsaEppSignedItem->lpsSigKey;
	DWORD dwSignatureAlgorithm = lpExportRsaEppSignedItem->dwSignatureAlgorithm;

	RLog2(LOG_LEVEL_COMMON, "Start: wExportItemType=[0x%04X],lpsName=[%s],lpsSigKey=[%s],dwSignatureAlgorithm=[0x%08X]",
		 wExportItemType, lpsName, lpsSigKey, dwSignatureAlgorithm);

	if(dwSignatureAlgorithm != WFS_PIN_SIGN_NA && dwSignatureAlgorithm != WFS_PIN_SIGN_RSASSA_PKCS1_V1_5 &&
		 dwSignatureAlgorithm != WFS_PIN_SIGN_RSASSA_PSS)
	{
		RLog2(LOG_LEVEL_ERROR, "signature generate algorithm is wrong.");
		return WFS_ERR_PIN_SIG_NOT_SUPP;
	} 

	FAIL_RET(nRet, this, CheckOpen());

	if(lpsSigKey == NULL || lpsSigKey[0] == '\0')
	{
		wSigKeyId = _EPP_SIG_SK;
	}
	else if(strcmp(lpsSigKey, "_SigIssuerVendor") == 0) 
	{
		wSigKeyId = _VENDOR_PK;
	}
	else if(strcmp(lpsSigKey, "_EPPSignKey") == 0)
	{
		wSigKeyId = _EPP_SIG_SK;
	}	
	else
	{
		FAIL_RET(nRet, this, FindKey(lpsSigKey, wSigKeyId, (WFS_PIN_USERSAPRIVATE)));
	}

	if(wExportItemType == WFS_PIN_EXPORT_PUBLIC_KEY)
	{
		if(lpsName == NULL || lpsName[0] == '\0' || strcmp(lpsName, "_EPPCryptKey") == 0)
		{	
			wKeyId = _EPP_CRY_PK;
		}
		else if(strcmp(lpsName, "_EPPSignKey") == 0)
		{
			wKeyId = _EPP_SIG_PK;
		}
		else
		{
			FAIL_RET(nRet, this, FindKey(lpsName, wKeyId, (WFS_PIN_USERSAPUBLIC | WFS_PIN_USERSAPUBLICVERIFY)));
		}
	}
	else if(wExportItemType == WFS_PIN_EXPORT_EPP_ID)
	{
		wKeyId = _RSA_EPP_ID;
	}
	else
	{
		RLog2(LOG_LEVEL_ERROR, "item type not supported.");
		return WFS_ERR_PIN_INVALIDDATA;
	}

	nRet = m_pPinpad->RSA_Export(wKeyId, (char*)lpData, DataLen, (char*)lpSignature, SignatureLen, SigAlg, wSigKeyId);
	if(nRet)
	{
		RLog2(LOG_LEVEL_ERROR, "export security item fail, nRet=[%d]", nRet);
		return Convert2SPError(nRet);
	}

	if(SigAlg != dwSignatureAlgorithm)
	{
		RLog2(LOG_LEVEL_ERROR, "signature algorithm is not supported");
		return WFS_ERR_PIN_SIG_NOT_SUPP;
	}

	m_ExportRsaEppSignedItemResult.Set(lpData, DataLen, NULL, 0, lpSignature, SignatureLen);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::LoadCertificate(LPWFSPINLOADCERTIFICATE lpLoadCertificate)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::GetCertificate(LPWFSPINGETCERTIFICATE lpGetCertificate)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::ReplaceCertificate(LPWFSPINREPLACECERTIFICATE lpReplaceCertificate)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::ImportRsaEncipheredPkcs7Key(LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEY lpImportRsaEncipheredPkcs7Key)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::LoadCertificateEx(LPWFSPINLOADCERTIFICATEEX lpLCEx)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::ImportRsaEncipheredPkcs7KeyEx(LPWFSPINIMPORTRSAENCIPHEREDPKCS7KEYEX lpIREPKEx)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::EmvImportPublicKey(LPWFSPINEMVIMPORTPUBLICKEY lpEmvImportPublicKey)
{
	WORD wResultLen = 0;
	WORD wKeyId = KEY_INVALID, wSigKeyId = KEY_INVALID;	
	long nRet = 0;
	char szExpiryDate[128] = {0};

	if(NULL == lpEmvImportPublicKey)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	LPSTR lpsKey = lpEmvImportPublicKey->lpsKey;
	DWORD dwUse = lpEmvImportPublicKey->dwUse;
	WORD wImportScheme = lpEmvImportPublicKey->wImportScheme;
	LPWFSXDATA lpxImportData = lpEmvImportPublicKey->lpxImportData;
	LPSTR lpsSigKey = lpEmvImportPublicKey->lpsSigKey;

	RLog2(LOG_LEVEL_COMMON, "Start: lpsKey=[%s],dwUse=[0x%X],wImportScheme=[0x%X],lpsSigKey=[%s]", lpsKey, dwUse, wImportScheme, lpsSigKey);

	if(dwUse == 0)
	{
		return DeleteKey(lpsKey);
	}

	if(lpxImportData == NULL || lpxImportData->lpbData == NULL || lpxImportData->usLength == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "CA data is empty.");
		return WFS_ERR_PIN_INVALIDDATA;
	}

	FAIL_RET(nRet, this, CheckOpen());

	ALLOT_KEYID(lpsKey, wKeyId);

	//下面的证书需要验签
	if(wImportScheme == WFS_PIN_EMV_IMPORT_ISSUER || wImportScheme == WFS_PIN_EMV_IMPORT_ICC ||
		 wImportScheme == WFS_PIN_EMV_IMPORT_ICC_PIN || wImportScheme == WFS_PIN_EMV_IMPORT_PKCSV1_5_CA)
	{
		FAIL_RET(nRet, this, FindKey(lpsSigKey, wSigKeyId, WFS_PIN_USERSAPUBLICVERIFY));
	}

	if(nRet = m_pPinpad->EMV_ImportPK(wKeyId, dwUse, wSigKeyId, (EEMV)wImportScheme, (const char*)(lpxImportData->lpbData), 
		      lpxImportData->usLength, szExpiryDate, &wResultLen))
	{
		RLog2(LOG_LEVEL_ERROR, "import emv ca fail, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	m_EmvImportPublicKeyResult.Set(szExpiryDate);
	RLog2(LOG_LEVEL_COMMON, "_End: ");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::Digest(LPWFSPINDIGEST lpDigest)
{
	RLog2(LOG_LEVEL_COMMON, "_Start");

	long nRet = 0;
	char szDigest[256] = {0};

	if(NULL == lpDigest)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	WORD wHashAlgorithm = lpDigest->wHashAlgorithm;
	LPWFSXDATA lpxDigestInput = lpDigest->lpxDigestInput;
	DWORD dwDigestLen = lpxDigestInput->usLength;

	if(!lpxDigestInput || lpxDigestInput->usLength <= 0)
	{
		RLog2(LOG_LEVEL_ERROR, "data is empty.");
		return WFS_ERR_PIN_INVALIDDATA;
	}

	if(NULL == m_pPinpad)
	{
		RLog2(LOG_LEVEL_ERROR, "m_pPinpad is NULL, make sure instance pinpad success.");
		return WFS_ERR_DEV_NOT_READY;
	}

	if(nRet = m_pPinpad->Soft_Hash((EHASH)wHashAlgorithm, (char*)(lpxDigestInput->lpbData), dwDigestLen, szDigest))
	{
		RLog2(LOG_LEVEL_ERROR, "digest fail, nRet=[%d]", nRet);
		return Convert2SPError(nRet);
	}

	m_DigestResult.Set((LPBYTE)szDigest, dwDigestLen);
	RLog2(LOG_LEVEL_COMMON, "_End");
	return WFS_SUCCESS;
}


//===========================================================================//
//                           China Protocol in ENC IO                        //
//===========================================================================//
HRESULT CXFSPINBase::ENC_China(WORD wProtocol, LPVOID lpData, ULONG ulDataLength)
{
	HRESULT hRes = 0;
	switch(wProtocol)
	{
		case WFS_CMD_ENC_IO_CHN_DIGEST: 
		{
			hRes = SM3_Digest(static_cast<LPPROTCHNDIGESTIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_CHN, NULL, 0);
			}
			break;
		}
		case WFS_CMD_ENC_IO_CHN_SET_SM2_PARAM: 
		{
			hRes = SM2_SetPara(static_cast<LPPROTCHNSM2ALGORITHMPARAMIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_CHN, NULL, 0);
			}
			break;
		}
		case WFS_CMD_ENC_IO_CHN_IMPORT_SM2_PUBLIC_KEY: 
		{
			hRes = SM2_ImportPK(static_cast<LPPROTCHNIMPORTSM2PUBLICKEYIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_CHN, NULL, 0);
			}
			break;
		}
		case WFS_CMD_ENC_IO_CHN_SIGN: 
		{
			hRes = SM2_Sign(static_cast<LPPROTCHNSIGNIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_CHN, NULL, 0);
			}
			break;
		}
		case WFS_CMD_ENC_IO_CHN_VERIFY: 
		{
			hRes = SM2_Verify(static_cast<LPPROTCHNVERIFYIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_CHN, NULL, 0);
			}
			break;
		}
		case WFS_CMD_ENC_IO_CHN_EXPORT_SM2_ISSUER_SIGNED_ITEM: 
		{
			hRes = SM2_ExportKey(static_cast<LPPROTCHNEXPORTSM2EPPSIGNEDITEMIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_CHN, NULL, 0);
			}
			break;
		}
		case WFS_CMD_ENC_IO_CHN_GENERATE_SM2_KEY_PAIR: 
		{
			hRes = SM2_GenerateKeyPair(static_cast<LPPROTCHNGENERATESM2KEYPAIRIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_CHN, NULL, 0);
			}
			break;
		}
		case WFS_CMD_ENC_IO_CHN_EXPORT_SM2_EPP_SIGNED_ITEM: 
		{
			hRes = SM2_ExportKey(static_cast<LPPROTCHNEXPORTSM2EPPSIGNEDITEMIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_CHN, NULL, 0);
			}
			break;
		}
		case WFS_CMD_ENC_IO_CHN_IMPORT_SM2_SIGNED_SM4_KEY: 
		{
			hRes = SM2_ImportSM4Key(static_cast<LPPROTCHNIMPORTSM2SIGNEDSM4KEY>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_CHN, NULL, 0);
			}
			break;
		}		
		default:
		{
			RLog2(LOG_LEVEL_ERROR, "China protocol not supported.");
			hRes = WFS_ERR_PIN_PROTOCOLNOTSUPP;
		}
	}

	return hRes;
}

HRESULT CXFSPINBase::SM3_Digest(LPPROTCHNDIGESTIN lpDigest)
{
	long nRet = 0;
	char szBuf[512] = {0};

	LPWFSXDATA lpXData = (LPWFSXDATA)((char*)lpDigest + sizeof(PROTCHNDIGESTIN));
	char *lpData = (char*)lpXData + sizeof(WFSXDATA);
	DWORD dwDataLen = lpXData->usLength;
	RLog2(LOG_LEVEL_COMMON, "Start: wDataLen=[%d]", dwDataLen);

	FAIL_RET(nRet, this, CheckOpen());

	if(nRet = m_pPinpad->SM3_Algorithm(lpData, dwDataLen, szBuf))
	{
		RLog2(LOG_LEVEL_ERROR, "SM3_Algorithm failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SM2_SetPara(LPPROTCHNSM2ALGORITHMPARAMIN lpSM2)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINBase::SM2_ImportPK(LPPROTCHNIMPORTSM2PUBLICKEYIN lpSM2ImpPK)
{
	long nRet = 0;
	RLog2(LOG_LEVEL_COMMON, "Start: ");

	WORD wKeyId = 0;
	LPCSTR lpKey = NULL;
	WORD wKeyLen = 0;
	WORD wHostPK = 0;
	char szZa[256] = {0};
	WORD wZaLen = 0;
	char szSig[256] = {0};
	WORD wSigLen = 0;
	char *lpKCVRet = 0;
	LPDWORD pKCVL = 0;
	EKCVMODE eKCV = KCVNONE;
	ESIG_ALGORITHM eAlg = SIGN_SM2_GM_T_2012;

	FAIL_RET(nRet, this, CheckOpen());

	if(nRet = m_pPinpad->SM2_ImportKey(wKeyId, lpKey, wKeyLen, szZa, wZaLen, szSig, wSigLen))
	{
		RLog2(LOG_LEVEL_ERROR, "SM2_ImportKey failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SM2_Sign(LPPROTCHNSIGNIN lpSign)
{
	long nRet = 0;
	RLog2(LOG_LEVEL_COMMON, "Start: ");

	FAIL_RET(nRet, this, CheckOpen());

	//if(nRet = m_pPinpad-)
	{
		RLog2(LOG_LEVEL_ERROR, "SM2_Sign failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SM2_Verify(LPPROTCHNVERIFYIN lpVerify)
{
	long nRet = 0;
	RLog2(LOG_LEVEL_COMMON, "Start: ");

	FAIL_RET(nRet, this, CheckOpen());

	//if(nRet = m_pPinpad-())
	{
		RLog2(LOG_LEVEL_ERROR, "SM2_Verify failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SM2_GenerateKeyPair(LPPROTCHNGENERATESM2KEYPAIRIN lpGenKey)
{
	long nRet = 0;
	RLog2(LOG_LEVEL_COMMON, "Start: ");
	WORD wSKID = 0;
	WORD wPKID = 0;

	FAIL_RET(nRet, this, CheckOpen());

	if(nRet = m_pPinpad->SM2_GenKeyPair(wSKID, wPKID))
	{
		RLog2(LOG_LEVEL_ERROR, "SM2_GenKeyPair failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SM2_ExportKey(LPPROTCHNEXPORTSM2EPPSIGNEDITEMIN lpExpKey)
{
	long nRet = 0;
	RLog2(LOG_LEVEL_COMMON, "Start: ");

	FAIL_RET(nRet, this, CheckOpen());

	WORD wPKLen = 0;
	char szPK[1024] = {0};
	ESIG_ALGORITHM eAlg = SIGN_SM2_GM_T_2012;
	char szZa[256] = {0};
	WORD wZaLen = 0;
	char szSig[256] = {0};
	WORD wSigLen = 0;

	if(nRet = m_pPinpad->SM2_ExportKey(_EPP_SM2_SIG_PK, szPK, wPKLen, eAlg, szZa, wZaLen, szSig, wSigLen))
	{
		RLog2(LOG_LEVEL_ERROR, "SM2_ExportKey failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	/*WFSXDATA stData;
	stData.usLength = wPKLen;
	stData.lpbData = (LPBYTE)szPK;

	PROTCHNEXPORTSM2EPPSIGNEDITEMOUT stOut;
	stOut.wCommand = WFS_CMD_ENC_IO_CHN_EXPORT_SM2_EPP_SIGNED_ITEM;
	stOut.wResult = nRet;
	stOut.dwSM2SignatureAlgorithm = eAlg;
	stOut.lpxValue = &stData;*/

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SM2_ImportSM4Key(LPPROTCHNIMPORTSM2SIGNEDSM4KEY lpSM4Key)
{
	long nRet = 0;
	RLog2(LOG_LEVEL_COMMON, "Start: ");

	WORD wKeyId = 0;
	DWORD dwKeyAttr = 0;
	LPCSTR lpEncKey = NULL;
	WORD wEKeyLen = 0;
	WORD wHostPK = 0;
	char szZa[256] = {0};
	WORD wZaLen = 0;
	char szSig[512] = {0};
	WORD wSigLen = 0;
	char *lpKCVRet = 0;
	LPDWORD pKCVL = 0;
	EKCVMODE eKCV = KCVNONE;
	bool bRandom = false;
	ESIG_ALGORITHM eAlg = SIGN_SM2_GM_T_2012;
	WORD wDecryptId = _EPP_SM2_CRYPT_SK;
	WORD *lpSM4KeyLen = 0;

	FAIL_RET(nRet, this, CheckOpen());

	if(nRet = m_pPinpad->SM2_ImportSM4Key(wKeyId, dwKeyAttr, lpEncKey, wEKeyLen, wHostPK, szZa, wZaLen, szSig, wSigLen))
	{
		RLog2(LOG_LEVEL_ERROR, "SM2_ImportSM4Key failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SM2_KeyExchangeB(LPWFSPINSM2KEYEXBIN lpKeyExcB)
{
	if(NULL == lpKeyExcB)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	long nRet = 0;
	WORD wKeyId = KEY_INVALID;
	LPSTR lpsKey = lpKeyExcB->lpsSigKey;
	RLog2(LOG_LEVEL_COMMON, "Start: lpsKey=[%s], wCommand=[0x%04X]", lpsKey, lpKeyExcB->wCommand);

	FAIL_RET(nRet, this, CheckOpen());

	FindKey(lpsKey, wKeyId);
	if(wKeyId == KEY_INVALID)
	{
		wKeyId = _HOST_SM2_SIG_PK;
	}

	WFSPINSM2KEYEXBOUT stOut;
	memset(&stOut, 0, sizeof(WFSPINSM2KEYEXBOUT));
	stOut.wCommand = lpKeyExcB->wCommand;

	if(nRet = m_pPinpad->SM2_KeyExchangeB((LPCSTR)lpKeyExcB->lpZa, lpKeyExcB->wZaLen, (LPCSTR)lpKeyExcB->lpPa, lpKeyExcB->wPaLen, 
		(LPCSTR)lpKeyExcB->lpRa, lpKeyExcB->wRaLen, (LPCSTR)lpKeyExcB->lpASig, lpKeyExcB->wASigLen, (LPSTR)stOut.lpZb, stOut.wZbLen, 
		(LPSTR)stOut.lpRb, stOut.wRbLen, (LPSTR)stOut.lpSb, stOut.wSbLen, (LPSTR)stOut.lpBSig, stOut.wBSigLen, wKeyId))
	{
		RLog2(LOG_LEVEL_ERROR, "SM2_KeyExchangeB failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, &stOut, sizeof(WFSPINSM2KEYEXBOUT));

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SM2_KeyAgreementB(LPWFSPINSM2KEYAGREEMENTBIN lpKeyAgreeB)
{
	if(NULL == lpKeyAgreeB)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	long nRet = 0;
	WORD wKeyId = KEY_INVALID;
	WORD wKCVL = 0;
	BYTE szKCV[64] = {0};
	LPSTR lpsKey = lpKeyAgreeB->lpsKey;
	DWORD dwUse = lpKeyAgreeB->dwUse;
	RLog2(LOG_LEVEL_COMMON, "Start: lpsKey=[%s], wCommand=[0x%04X], dwUse=[0x%X]", lpsKey, lpKeyAgreeB->wCommand, dwUse);

	DWORD dwUsage = WFS_PIN_USECRYPT | WFS_PIN_USEFUNCTION | WFS_PIN_USEMACING | WFS_PIN_USEKEYENCKEY | WFS_PIN_USENODUPLICATE | WFS_PIN_USESVENCKEY;
	if(dwUse & (~dwUsage))
	{
		RLog2(LOG_LEVEL_ERROR, "the usage is not support.");
		return WFS_ERR_PIN_USEVIOLATION;
	}

	FAIL_RET(nRet, this, CheckOpen());

	ALLOT_KEYID(lpsKey, wKeyId);

	if(nRet = m_pPinpad->SM2_KeyAgreementB(wKeyId, dwUse, (LPCSTR)lpKeyAgreeB->lpSa, lpKeyAgreeB->wSaLen, (LPSTR)szKCV, wKCVL, m_eKCVMode))
	{
		RLog2(LOG_LEVEL_ERROR, "SM2_KeyAgreementB failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}


//===========================================================================//
//                            CB协议的实现                                   //
//===========================================================================//
HRESULT CXFSPINBase::ENC_GIECB(WORD wProtocol, LPVOID lpData, ULONG ulDataLength)
{
	LPBYTE p, lpbData;
	BYTE bVersion;
	char szCode[512] = {0x00}, szKeyName[512] = {0x00};
	ULONG ulIndex = 0, ulLen = 0, i = 0, ulLength = 0;

	p = (LPBYTE)lpData;
	while(ulLen < ulDataLength)
	{
		switch(*(p + ulIndex))
		{
			//protocol version
		case 0x40:
			ulIndex++;
			ulLen = *(p + ulIndex++);
			if(ulLen != 0x01)
			{
				RLog2(LOG_LEVEL_ERROR, "protocol version format is wrong.");
				return WFS_ERR_INVALID_DATA;
			}
			bVersion = *(p + ulIndex++);
			break;
			//interchange code
		case 0x41:
			ulIndex++;
			ulLen = *(p + ulIndex++);
			if(ulLen > 0x80)
			{
				i = ulLen - 0x80;
				ulLen = 0;
				while(i-- > 0)
				{
					ulLen = ulLen * 256 + p[ulIndex++];
				}
			}
			memcpy(szCode,  p+ulIndex, ulLen);
			ulIndex += ulLen;
			break;
			//key name
		case 0x43:
			ulIndex++;
			ulLen = *(p + ulIndex++);
			if(ulLen > 0x80)
			{
				i = ulLen - 0x80;
				ulLen = 0;
				while(i-- > 0)
				{
					ulLen = ulLen * 256 + p[ulIndex++];
				}
			}
			memcpy(szKeyName,  p+ulIndex, ulLen);
			ulIndex += ulLen;
			break;
			//data
		case 0x62:
			ulIndex++;
			ulLen = *(p + ulIndex++);
			if(ulLen > 0x80)
			{
				i = ulLen - 0x80;
				ulLen = 0;
				while(i-- > 0)
				{
					ulLen = ulLen * 256 + p[ulIndex++];
				}
			}
			lpbData = p+ulIndex;
			ulLength = ulLen;
			break;
		default:
			RLog2(LOG_LEVEL_ERROR, "data format is wrong.");
			return WFS_ERR_INVALID_DATA;
			break;
		}
	}

	return Interchange(bVersion, szCode, szKeyName, lpbData, ulLength);
}

HRESULT CXFSPINBase::Interchange(BYTE bVersion, char *szCode, char *szKeyName, LPBYTE lpbData, ULONG ulLength)
{
	return WFS_SUCCESS;
}


//===========================================================================//
//                            Events                                         //
//===========================================================================//
void CXFSPINBase::FireKey(WORD wComplete, ULONG ulDigit)
{
	WFSPINKEY key;
	key.wCompletion = wComplete;
	key.ulDigit = ulDigit;
	m_pSPKernel->FireExecEvent(WFS_EXEE_PIN_KEY, WFS_SUCCESS, &key);

	RLog2(LOG_LEVEL_COMMON, "Fire key, complete=[0x%04X], digit=[0x%08X]", wComplete, ulDigit);
}

void CXFSPINBase::FireInitialized(LPBYTE lpbIdentData, USHORT usIdentLength, LPBYTE lpbKeyData, USHORT usKeyLength)
{
	WFSPININIT init;
	WFSXDATA Ident, Key;

	Ident.lpbData = lpbIdentData;
	Ident.usLength = usIdentLength;
	Key.lpbData = lpbKeyData;
	Key.usLength = usKeyLength;
	init.lpxIdent = &Ident;
	init.lpxKey = &Key;
	m_pSPKernel->FireServEvent(WFS_SRVE_PIN_INITIALIZED, WFS_SUCCESS, &init);
}

void CXFSPINBase::FireIllegalKeyAccess(LPSTR szKeyName, LONG lErrorCode)
{
	WFSPINACCESS ia;
	ia.lpsKeyName = szKeyName;
	ia.lErrorCode = lErrorCode;
	m_pSPKernel->FireServEvent(WFS_SRVE_PIN_ILLEGAL_KEY_ACCESS, WFS_SUCCESS, &ia);
}

void CXFSPINBase::FireOptRequired()
{
	m_pSPKernel->FireServEvent(WFS_SRVE_PIN_OPT_REQUIRED);
}

void CXFSPINBase::FireCertificateChange(WORD wCertificateChange)
{
	m_pSPKernel->FireServEvent(WFS_SRVE_PIN_CERTIFICATE_CHANGE, WFS_SUCCESS, &wCertificateChange);
}

void CXFSPINBase::FireHsmTDataChanged(LPBYTE lpbData, USHORT usLength)
{
	WFSXDATA Data;	
	Data.lpbData = lpbData;
	Data.usLength = usLength;
	m_pSPKernel->FireServEvent(WFS_SRVE_PIN_HSM_TDATA_CHANGED, WFS_SUCCESS, &Data);
}

void CXFSPINBase::FireHsmChanged(WORD wHSMSerialNumber)
{
	WFSPINHSMIDENTIFIER Identifier;	
	Identifier.wHSMSerialNumber = wHSMSerialNumber;
	m_pSPKernel->FireServEvent(WFS_SRVE_PIN_HSM_CHANGED, WFS_SUCCESS, &Identifier);
}

void CXFSPINBase::FireEnterData()
{
	m_pSPKernel->FireExecEvent(WFS_EXEE_PIN_ENTERDATA);
}

void CXFSPINBase::FireDevicePosition(WORD wPosition)
{
	WFSPINDEVICEPOSITION Position;	
	Position.wPosition = wPosition;
	m_pSPKernel->FireServEvent(WFS_SRVE_PIN_DEVICEPOSITION, WFS_SUCCESS, &Position);
}

void CXFSPINBase::FirePowerSaveChange(USHORT usPowerSaveRecoveryTime)
{
	WFSPINPOWERSAVECHANGE Power;	
	Power.usPowerSaveRecoveryTime= usPowerSaveRecoveryTime;
	m_pSPKernel->FireServEvent(WFS_SRVE_PIN_POWER_SAVE_CHANGE, WFS_SUCCESS, &Power);
}

void CXFSPINBase::FireLayout(DWORD dwEntryMode, USHORT usNumberOfFrames, LPWFSPINFRAME *lppFrames)
{
	WFSPINLAYOUT Layout;	
	Layout.dwEntryMode = dwEntryMode;
	Layout.usNumberOfFrames = usNumberOfFrames;
	Layout.lppFrames = lppFrames;
	m_pSPKernel->FireExecEvent(WFS_EXEE_PIN_LAYOUT, WFS_SUCCESS, &Layout);
}


//===========================================================================//
//                            SZZT ENC IO                                    //
//===========================================================================//
HRESULT CXFSPINBase::ENC_SZZT(WORD wProtocol, LPVOID lpData, ULONG ulDataLength)
{
	HRESULT hRes = 0;
	switch(wProtocol)
	{
		case WFS_CMD_ENC_IO_GET_PASSWORD: 
		{
			hRes = GetPassword(static_cast<LPWFSPINGETPASSWORD>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, NULL, 0);
			}

			break;
		}
		case WFS_PIN_ENC_IO_RSA_VENDORINIT: 
		{
			hRes = RsaVendorInit(static_cast<LPWFSPINRSAVENDORINIT>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, NULL, 0);
			}

			break;
		}
		case WFS_PIN_ENC_IO_RSA_IMPHOSTVER: 
		{
			hRes = ImportHostVersion(static_cast<LPWFSPINIMPORTHOSTVERSION>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, NULL, 0);
			}

			break;
		}
		case WFS_PIN_ENC_IO_RSA_EXPHOSTVER: 
		{				
			hRes = ExportHostVersion();
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, (LPWFSPINEXPORTHOSTVERSIONOUT)m_ExpHostVer, sizeof(WFSPINEXPORTHOSTVERSIONOUT));
			}

			break;
		}
		case WFS_PIN_ENC_IO_RSA_ENCRYPTRC: 
		{			
			hRes = EncryptRC(static_cast<LPWFSPINENCRYPTRCIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, (LPWFSPINENCRYPTRCOUT)m_EncryptRc, sizeof(WFSPINENCRYPTRCOUT));
			}

			break;
		}
		case WFS_PIN_ENC_IO_AUTO_UPDATEPSW: 
		{
			hRes = AutoUpdatePassword(static_cast<LPWFSPINAUTOUPDATEPASSWORD>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, NULL, 0);
			}

			break;
		}
		case WFS_PIN_ENC_IO_AUTO_VIRTUALBK: 
		{
			hRes = VirtualBankKey(static_cast<LPWFSPINVIRTUALBANKKEYIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, (LPWFSPINVIRTUALBANKKEYOUT)m_VirtualBankKey, sizeof(WFSPINVIRTUALBANKKEYOUT));
			}

			break;
		}
		case WFS_PIN_ENC_IO_AUTO_RI: 
		{
			hRes = AutoRemoveInstall(static_cast<LPWFSPINAUTOREMOVEINSTALL>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, NULL, 0);
			}

			break;
		}
		case WFS_PIN_ENC_IO_GET_FWMAC: 
		{				
			hRes = FirmwareMac();
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, (LPWFSPINFIRMWAREMACOUT)m_FirmwareMac, sizeof(WFSPINFIRMWAREMACOUT));
			}

			break;
		}			
		case WFS_PIN_ENC_IO_CHN_SELFTEST: 
		{
			hRes = SelfTest(static_cast<LPWFSPINSELFTESTIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, NULL, 0);
			}

			break;
		}
		case WFS_PIN_ENC_IO_CHN_KEYEXCHANGE_B: 
		{
			hRes = SM2_KeyExchangeB(static_cast<LPWFSPINSM2KEYEXBIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, NULL, 0);
			}

			break;
		}
		case WFS_PIN_ENC_IO_CHN_KEYAGREEMENT_B: 
		{
			hRes = SM2_KeyAgreementB(static_cast<LPWFSPINSM2KEYAGREEMENTBIN>(lpData));
			if(hRes == WFS_SUCCESS)
			{
				m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, NULL, 0);
			}

			break;
		}
		default:
		{
			RLog2(LOG_LEVEL_ERROR, "szzt protocol not supported.");
			hRes = WFS_ERR_PIN_PROTOCOLNOTSUPP;
		}
	}
	
	RLog2(LOG_LEVEL_COMMON, "End:");
	return hRes;
}

HRESULT CXFSPINBase::GetPassword(LPWFSPINGETPASSWORD lpGetPsw)
{
	vector<BYTE> veCodes, veDisableCodes, veCurrentCodes;
	EPSW PswType;
	ULONG ulFK, ulFDK;
	BYTE szBuf[512] = {0};
	DWORD i, dwLen = 0, dwPswLen = 0;
	WORD wComp = 0;
	long nRet = 0;

	if(NULL == lpGetPsw)
	{
		RLog2(LOG_LEVEL_ERROR, "Pointer is NULL.");
		return WFS_ERR_INVALID_POINTER;
	}

	WORD wOperator = lpGetPsw->wOperator;
	WORD wPswType = lpGetPsw->wPswType; 
	USHORT usMaxLen = lpGetPsw->usMaxLen;
	BOOL bAutoEnd = lpGetPsw->bAutoEnd;
	CHAR cEcho = lpGetPsw->cEcho;
	ULONG ulActiveFDKs = lpGetPsw->ulActiveFDKs; 
	ULONG ulActiveKeys = lpGetPsw->ulActiveKeys; 
	ULONG ulTerminateFDKs = lpGetPsw->ulTerminateFDKs; 
	ULONG ulTerminateKeys = lpGetPsw->ulTerminateKeys; 

	RLog2(LOG_LEVEL_COMMON,
		"Start: wOperator=[0x%04X],usMaxLen=[%d],bAutoEnd=[%d],ulActiveFKs=[0x%X],"
		"ulActiveFDKs=[0x%X],ulTerminateFKs=[0x%X],ulTerminateFDKs=[0x%X]",
		wOperator,usMaxLen, bAutoEnd, ulActiveKeys, ulActiveFDKs, ulTerminateKeys, ulTerminateFDKs);

	DWORD dwEcho = ((m_nEcho != 0xFFFF) ? m_nEcho : cEcho);

	FAIL_RET(nRet, this, CheckOpen());

	if(wPswType == WFS_PIN_PASSWORD_UPDATE)
	{
		if(nRet = m_pPinpad->UpdatePassword(EPSW_ID(wOperator)))
		{
			RLog2(LOG_LEVEL_ERROR, "Update administrator password failed, nRet=%d.", nRet);
			return Convert2SPError(nRet);
		}
		RLog2(LOG_LEVEL_COMMON, "End:");
		return WFS_SUCCESS;
	}

	//检查自动结束(最大位数为零时，自动终止应该无效)
	if(usMaxLen == 0 && bAutoEnd)
	{
		RLog2(LOG_LEVEL_ERROR, "max length is 0 and auto end is true.");
		return WFS_ERR_INVALID_DATA;
	}

	FAIL_RET(nRet, this, CheckTerminateKey(ulTerminateKeys, ulTerminateFDKs, bAutoEnd));
	FAIL_RET(nRet, this, CheckActiveKey(ulActiveKeys, ulActiveFDKs, ulTerminateKeys, ulTerminateFDKs, m_PressKey.m_PinFKMask, m_PressKey.m_PinFDKMask));

	switch(wPswType)
	{
		case WFS_PIN_PASSWORD_OLD:
			PswType = PSW_OLD;
			break;
		case WFS_PIN_PASSWORD_NEW:
			PswType = PSW_NEW;
			break;
		default:
			RLog2(LOG_LEVEL_ERROR, "password type is wrong.");
			return WFS_ERR_CFG_INVALID_VALUE;
	}

	m_PressKey.GetPinCodes(0xFFFFFFFF, 0xFFFFFFFF, veCodes);
	CAutoRecoverKeyCode AutoRecoverKeyCode(m_pPinpad, veCodes, m_nIsDisablePressKey, ENTRY_MODE_PIN);

	//激活按键
	m_PressKey.GetPinCodes(ulActiveKeys, ulActiveFDKs, veCodes);
	m_PressKey.GetPinCodes(~(WFS_PIN_FK_0|WFS_PIN_FK_1|WFS_PIN_FK_2|WFS_PIN_FK_3|WFS_PIN_FK_4|
		                    WFS_PIN_FK_5|WFS_PIN_FK_6|WFS_PIN_FK_7|WFS_PIN_FK_8|WFS_PIN_FK_9) & ulActiveKeys, 
												ulActiveFDKs, veDisableCodes);

 	//写键码	
	if(m_nIsDisablePressKey)
	{
		if(nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size(), ENTRY_MODE_PIN))
		{
			RLog2(LOG_LEVEL_ERROR, "write key codes fail, nRet=%d.", nRet);
			return Convert2SPError(nRet);
		}
		veCurrentCodes = veCodes;
	}

	//打开输入
	if(nRet = m_pPinpad->StartPswInput(ESOUND(m_nPswEntryVoice), EPSW_ID(wOperator), PswType, (BYTE)usMaxLen, bAutoEnd ? true : false))
	{
		RLog2(LOG_LEVEL_ERROR, "open input fail, nRet=%d.", nRet);
		return Convert2SPError(nRet);
	}
	CAutoCloseEntry AutoCloseEntry(m_pPinpad);

	while(TRUE)
	{
		if(m_pSPKernel->IsRequestCancelled())
		{
			RLog2(LOG_LEVEL_WARN, "pin input is cancelled.");
			return WFS_ERR_CANCELED;
		}	
		if(m_pSPKernel->IsRequestTimeout())
		{
			RLog2(LOG_LEVEL_WARN, "pin input is timeout.");	
			return WFS_ERR_TIMEOUT;
		}

		dwLen = 0;
		nRet = m_pPinpad->ReadText((char*)szBuf, dwLen, 100);
		for(i = 0; i < dwLen; ++i)
		{
			if(szBuf[i] == m_cLongPressCode)  //长按键超时
			{
				if(m_bIsLongPressedTimeout)
				{
					RLog2(LOG_LEVEL_WARN, "password input long pressed timeout occupy.");
					return WFS_ERR_TIMEOUT;
				}
				continue;
			}	
			if(szBuf[i] == m_cTimeOutCode)  //不按键超时，因键盘已经关闭输入，只能返回超时
			{
				RLog2(LOG_LEVEL_WARN, "pin input long not press timeout occupy.");
				return WFS_ERR_TIMEOUT;
			}
			
			m_PressKey.GetKeyValue(szBuf[i], ulFK, ulFDK);
			//密码键处理
			if(szBuf[i] == 0x00 || (ulFK == 0 && ulFDK == 0) || szBuf[i] == m_cEchoHW)
			{
				//计数密码位
				if(usMaxLen > 0 && dwPswLen >= usMaxLen)
					continue;
				dwPswLen++;
				FireKey(WFS_PIN_COMPCONTINUE, dwEcho);
				//禁止数字键
				if(m_nIsDisablePressKey && usMaxLen > 0 && dwPswLen >= usMaxLen && veCurrentCodes != veDisableCodes)
				{
					if(nRet = m_pPinpad->SetKeyboardCode((char*)(veDisableCodes.data()), veDisableCodes.size(), ENTRY_MODE_PIN))
					{
						RLog2(LOG_LEVEL_ERROR, "write key codes fail, nRet=%d.", nRet);
						return Convert2SPError(nRet);
					}
					veCurrentCodes = veDisableCodes;
				}
				//自动终止
				if(usMaxLen > 0 && dwPswLen >= usMaxLen && bAutoEnd == TRUE)
				{
					wComp = WFS_PIN_COMPAUTO;
					goto Exit;
				}
				continue;
			}

			//不在激活键范围内
			if(!(ulFK & ulActiveKeys || ulFDK & ulActiveFDKs))
 				continue;

			//功能键处理
			wComp = WFS_PIN_COMPCONTINUE;
			switch(ulFK)
			{
			case 0:
				wComp = WFS_PIN_COMPCONTFDK;
				break;
			case WFS_PIN_FK_ENTER:
				break;
			case WFS_PIN_FK_CANCEL:
				for ( ; dwPswLen > 0; --dwPswLen)
					m_pPinpad->DelEntryBuffer();		
				break;
			case WFS_PIN_FK_CLEAR:
				for ( ; dwPswLen > 0; --dwPswLen)
					m_pPinpad->DelEntryBuffer();		
				break;
			case WFS_PIN_FK_BACKSPACE:
				if(dwPswLen > 0)
				{
					dwPswLen--;
					m_pPinpad->DelEntryBuffer();
				}
				break;
			case WFS_PIN_FK_HELP:
				break;
			default:
				break;
			}
			FireKey(wComp, ulFK + ulFDK);

			//恢复数字键
			if(m_nIsDisablePressKey && dwPswLen < usMaxLen && veCurrentCodes != veCodes)
			{
				if(nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size(), ENTRY_MODE_PIN))
				{
					RLog2(LOG_LEVEL_ERROR, "write key codes fail, nRet=%d.", nRet);
					return Convert2SPError(nRet);
				}
				veCurrentCodes = veCodes;
			}		

			//终止键处理
			if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
			{
				switch(ulFK)
				{
				case 0:
					wComp = WFS_PIN_COMPFDK;
					break;
				case WFS_PIN_FK_ENTER: 
					if(dwPswLen < usMaxLen)
					{
						continue;
					}	
					wComp = WFS_PIN_COMPENTER;
					break;
				case WFS_PIN_FK_CANCEL:
					wComp = WFS_PIN_COMPCANCEL;
					break;
				case WFS_PIN_FK_CLEAR:
					wComp = WFS_PIN_COMPCLEAR;
					break;
				case WFS_PIN_FK_BACKSPACE:
					wComp = WFS_PIN_COMPBACKSPACE;
					break;
				case WFS_PIN_FK_HELP:
					wComp = WFS_PIN_COMPHELP;
					break;
				default:
					wComp = WFS_PIN_COMPFK;
					break;
				}
				goto Exit;
			}
		} //for
	} //while

Exit:

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::RsaVendorInit(LPWFSPINRSAVENDORINIT lpRSAVI)
{
	long nRet = 0;
	LPBYTE pPkData = NULL, pSkData = NULL;

	if(NULL == lpRSAVI)
	{
		RLog2(LOG_LEVEL_ERROR, "Pointer is NULL.");
		return WFS_ERR_INVALID_POINTER;
	}

	BOOL bSoftGen = lpRSAVI->bSoftGen;
	ULONG ulBits = lpRSAVI->ulBits; 
	USHORT usPkLength = lpRSAVI->usPkLength;
	LPBYTE lpPkData = lpRSAVI->lpPkData;
	USHORT usSkLength = lpRSAVI->usSkLength;
	LPBYTE lpSkData = lpRSAVI->lpSkData;

	RLog2(LOG_LEVEL_COMMON, "Start: bSoftGen=[%d],ulBits=[%d]", bSoftGen, ulBits);

	if(ulBits != 1024 && ulBits != 2048)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid Bits.");
		return WFS_ERR_INVALID_DATA;
	}	

	if(usPkLength > 0)
		pPkData = lpPkData;
	if(usSkLength > 0)
		pSkData = lpSkData;

	FAIL_RET(nRet, this, CheckOpen());
	
	if(nRet = m_pPinpad->RSA_VendorInit(bSoftGen ? true : false, ulBits, (char*)pPkData, usPkLength, (char*)pSkData, usSkLength))
	{
		RLog2(LOG_LEVEL_ERROR, "RSA_VendorInit failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}	

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::ImportHostVersion(LPWFSPINIMPORTHOSTVERSION lpImpHostVer)
{
	long nRet = 0;
	WORD wSigKeyId = KEY_INVALID;

	if(NULL == lpImpHostVer)
	{
		RLog2(LOG_LEVEL_ERROR, "Pointer is NULL.");
		return WFS_ERR_INVALID_POINTER;
	}

	LPBYTE lpDer = lpImpHostVer->lpDer;
	USHORT usDerLength = lpImpHostVer->usDerLength; 
	LPBYTE lpSig = lpImpHostVer->lpSig;
	USHORT usSigLength = lpImpHostVer->usSigLength;
	DWORD dwRSASignatureAlgorithm = lpImpHostVer->bRSASignatureAlgorithm;
	LPSTR lpsSigKey = lpImpHostVer->lpsSigKey;

	RLog2(LOG_LEVEL_COMMON,
		"Start: usDerLength=[%d],usSigLength=[%d],dwRSASignatureAlgorithm=[0x%08x],lpsSigKey=[%s]", 
		          usDerLength, usSigLength, dwRSASignatureAlgorithm, lpsSigKey);

	if(lpDer == NULL || usDerLength <= 0)
	{
		RLog2(LOG_LEVEL_ERROR, "lpDer is empty.");
		return WFS_ERR_PIN_INVALIDDATA;
	}

	if(dwRSASignatureAlgorithm != WFS_PIN_SIGN_NA)
	{
		if(lpsSigKey == NULL || lpsSigKey[0] == '\0' || strcmp(lpsSigKey, "_SigIssuerVendor") == 0)
		{
			wSigKeyId = _VENDOR_PK;
		}	
		else if(strcmp(lpsSigKey, "_EPPSignKey") == 0)
		{
			wSigKeyId = _EPP_SIG_PK;
		}
		else
		{
			RLog2(LOG_LEVEL_ERROR, "invalid key.");
			return WFS_ERR_PIN_KEYINVALID;
		}

		if(lpSig == NULL || usSigLength == 0)
		{
			RLog2(LOG_LEVEL_ERROR, "lpSig is empty.");
			return WFS_ERR_PIN_SIGNATUREINVALID;
		}
	}

	FAIL_RET(nRet, this, CheckOpen());
	
	if(nRet = m_pPinpad->RSA_ImpHostVersion((char*)lpDer, usDerLength, (char*)lpSig, usSigLength, 
		                ESIG_ALGORITHM(dwRSASignatureAlgorithm), wSigKeyId))
	{
		RLog2(LOG_LEVEL_ERROR, "import host version fail, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}	

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::ExportHostVersion()
{
	long nRet = 0;
	char lpTag[2048] = {0};
	WORD wTagLen = 0;
	ESIG_ALGORITHM eSig = SIGN_NA;
	
	RLog2(LOG_LEVEL_COMMON, "Start:");

	FAIL_RET(nRet, this, CheckOpen());

	m_ExpHostVer.Clear();

	if(nRet = m_pPinpad->RSA_ExpHostVersion(lpTag, wTagLen, eSig))
	{
		RLog2(LOG_LEVEL_ERROR, "export host version fail, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}
	
	m_ExpHostVer.Set(WFS_PIN_ENC_IO_RSA_EXPHOSTVER, (LPBYTE)lpTag, wTagLen, (DWORD)eSig);

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::EncryptRC(LPWFSPINENCRYPTRCIN lpEncryptRC)
{
	BYTE lpTag[2048] = {0};
	WORD wHostKeyId = 1, wSigKeyId = KEY_INVALID;
	long nRet = 0;

	if(NULL == lpEncryptRC)
	{
		RLog2(LOG_LEVEL_ERROR, "Pointer is NULL.");
		return WFS_ERR_INVALID_POINTER;
	}

	LPBYTE lpRc = lpEncryptRC->lpRc;
	USHORT usRcLength = lpEncryptRC->bRcLength; 
	DWORD dwRSASignatureAlgorithm = lpEncryptRC->bRSASignatureAlgorithm;
	LPSTR lpsHostKey = lpEncryptRC->lpsHostKey;
	LPSTR lpsSigKey  = lpEncryptRC->lpsSigKey;

	RLog2(LOG_LEVEL_COMMON, "Start:");

	FAIL_RET(nRet, this, CheckOpen());

	m_EncryptRc.Clear();

	if(lpsHostKey && lpsHostKey[0] != '\0')
	{
		if(strcmp(lpsHostKey, "_EPPCryptKey") == 0)
		{
			wSigKeyId = _EPP_CRY_PK;
		}	
		else 
		{
			FAIL_RET(nRet, this, FindKey(lpsHostKey, wHostKeyId, (WFS_PIN_USERSAPUBLIC)));
		}
	}

	if(dwRSASignatureAlgorithm != WFS_PIN_SIGN_NA)
	{
		if(lpsSigKey == NULL || lpsSigKey[0] == '\0' || strcmp(lpsSigKey, "_SigIssuerVendor") == 0)
		{
			wSigKeyId = _VENDOR_PK;
		}	
		else if(strcmp(lpsSigKey, "_EPPSignKey") == 0)
		{
			wSigKeyId = _EPP_SIG_PK;
		}
		else
		{
			RLog2(LOG_LEVEL_ERROR, "invalid signature key.");
			return WFS_ERR_PIN_KEYINVALID;
		}
	}

	if(nRet = m_pPinpad->RSA_EncryptRC((LPCSTR)lpRc, usRcLength, (char*)lpTag, ESIG_ALGORITHM(dwRSASignatureAlgorithm), wHostKeyId, wSigKeyId))
	{
		RLog2(LOG_LEVEL_ERROR, "encrypt rc fail, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	m_EncryptRc.Set(WFS_PIN_ENC_IO_RSA_ENCRYPTRC, lpTag, usRcLength);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::AutoUpdatePassword(LPWFSPINAUTOUPDATEPASSWORD lpAutoUpdatePsw)
{
	LPBYTE pKeyTable = NULL;
	long nRet = 0;

	if(NULL == lpAutoUpdatePsw)
	{
		RLog2(LOG_LEVEL_ERROR, "Pointer is NULL.");
		return WFS_ERR_INVALID_POINTER;
	}

	WORD wOperator = lpAutoUpdatePsw->wOperator;
	LPBYTE lpOldPsw = lpAutoUpdatePsw->lpOldPsw; 
	LPBYTE lpNewPsw = lpAutoUpdatePsw->lpNewPsw;

	RLog2(LOG_LEVEL_COMMON, "Start: wOperator=[0x%04x]", wOperator);

	FAIL_RET(nRet, this, CheckOpen());

	vector<BYTE> vCodes;
	m_PressKey.GetPinCodes(0xFFFFFFFF, 0xFFFFFFFF, vCodes);
	if(vCodes.size() > 0)
	{
		pKeyTable = vCodes.data();
	}

	if(nRet = m_pPinpad->AutoUpdatePassword(EPSW_ID(wOperator), (LPCTSTR)lpOldPsw, (LPCTSTR)lpNewPsw, (LPCTSTR)pKeyTable))
	{
		RLog2(LOG_LEVEL_ERROR, "Auto update administrator password failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::VirtualBankKey(LPWFSPINVIRTUALBANKKEYIN lpVirtualKey)
{
	LPBYTE pKeyTable = NULL;
	BYTE lpKcv[128] = {0};
	WORD wKeyId = KEY_INVALID;
	long nRet = 0;

	if(NULL == lpVirtualKey)
	{
		RLog2(LOG_LEVEL_ERROR, "Pointer is NULL.");
		return WFS_ERR_INVALID_POINTER;
	}

	LPSTR lpsKey = lpVirtualKey->lpsKey;
	DWORD dwUse = lpVirtualKey->dwUse; 
	LPBYTE lpKeyValue = lpVirtualKey->lpKeyValue;
	USHORT usKeyLength = lpVirtualKey->usKeyLength;
	LPBYTE lpPsw  = lpVirtualKey->lpPsw;
	
	RLog2(LOG_LEVEL_COMMON, "Start: lpsKey=[%s],dwUse=[0x%08x],usKeyLength=[%d].", lpsKey, dwUse, usKeyLength);

	if(dwUse == 0)
	{
		return DeleteKey(lpsKey);
	}

	//检查用途
	DWORD dwUsage = WFS_PIN_USECRYPT | WFS_PIN_USEFUNCTION | WFS_PIN_USEMACING | WFS_PIN_USEKEYENCKEY 
		      | WFS_PIN_USENODUPLICATE | WFS_PIN_USESVENCKEY;
	if(dwUse & (~dwUsage))
	{
		RLog2(LOG_LEVEL_ERROR, "the usage is not support.");
		return WFS_ERR_PIN_USEVIOLATION;
	}

	//检查密钥值
	if(lpKeyValue == NULL || usKeyLength == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "key value is empty.");
		return WFS_ERR_PIN_KEYNOVALUE;
	}
	if(usKeyLength != 8 && usKeyLength != 16 && usKeyLength != 24)
	{
		RLog2(LOG_LEVEL_ERROR, "key value length is wrong, length=[%d].", usKeyLength);
		return WFS_ERR_PIN_INVALIDKEYLENGTH;
	}
	if(dwUse & WFS_PIN_USESVENCKEY && usKeyLength != 8) //IV密钥长度必须是8字节
	{
		RLog2(LOG_LEVEL_ERROR, "iv key value length must be 8 bytes, length=[%d].", usKeyLength);
		return WFS_ERR_PIN_INVALIDKEYLENGTH;
	}

	FAIL_RET(nRet, this, CheckOpen());

	ALLOT_KEYID(lpsKey, wKeyId);

	if(nRet = m_pPinpad->SetCaps(eCAP_KCVL, m_nKCVLen))
	{
		RLog2(LOG_LEVEL_WARN, "set kcv length fail, nRet=[%d]", nRet);
		//return Convert2SPError(nRet);
	}

	vector<BYTE> vCodes;
	m_PressKey.GetPinCodes(0xFFFFFFFF, 0xFFFFFFFF, vCodes);
	if(vCodes.size() > 0)
	{
		pKeyTable = vCodes.data();
	}

 	if(nRet = m_pPinpad->VirtualBankKey(wKeyId, dwUse, (LPCSTR)lpKeyValue, usKeyLength, (LPCSTR)lpPsw, (LPCSTR)pKeyTable, (char*)lpKcv, m_eKCVMode))
 	{
 		RLog2(LOG_LEVEL_ERROR, "auto update administrator password fail, nRet=[%d].", nRet);
 		return Convert2SPError(nRet);
 	}

	m_pPinpad->GetCapabilities(&m_stPinCaps);
	m_VirtualBankKey.Set(WFS_PIN_ENC_IO_AUTO_VIRTUALBK, lpKcv, m_stPinCaps.ucKCVL);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::AutoRemoveInstall(LPWFSPINAUTOREMOVEINSTALL lpAutoRI)
{
	if(NULL == lpAutoRI)
	{
		RLog2(LOG_LEVEL_ERROR, "Pointer is NULL.");
		return WFS_ERR_INVALID_POINTER;
	}

	WORD wMode = lpAutoRI->wMode;
	LPBYTE lpPsw1 = lpAutoRI->lpPsw1; 
	LPBYTE lpPsw2 = lpAutoRI->lpPsw2;

	RLog2(LOG_LEVEL_COMMON, "Start: wMode=[0x%04x]", wMode);

	LPBYTE pKeyTable = NULL;
	long nRet = 0;
	FAIL_RET(nRet, this, CheckOpen());

	vector<BYTE> vCodes;
	m_PressKey.GetPinCodes(0xFFFFFFFF, 0xFFFFFFFF, vCodes);
	if(vCodes.size() > 0)
	{
		pKeyTable = vCodes.data();
	}

	if(nRet = m_pPinpad->RI_AutoRemoveInstall(EINSTALL_AUTH(wMode), (LPCSTR)lpPsw1, (LPCSTR)lpPsw2, (LPCSTR)pKeyTable))
	{
		RLog2(LOG_LEVEL_ERROR, "Auto remove install failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::FirmwareMac()
{
	RLog2(LOG_LEVEL_COMMON, "Start: ");

	long nRet = 0;
	FAIL_RET(nRet, this, CheckOpen());

	m_FirmwareMac.Clear();

	char szMac[128] = {0};
	long nLen = sizeof(szMac);
	if(nRet = m_pPinpad->Execute(INX_FIRMWARE_MAC, szMac, nLen))
	{
		RLog2(LOG_LEVEL_ERROR, "FirmwareMAC failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	m_FirmwareMac.Set(WFS_PIN_ENC_IO_GET_FWMAC, (LPBYTE)szMac, 8);
	
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SelfTest(LPWFSPINSELFTESTIN lpSelfTest)
{
	if(NULL == lpSelfTest)
	{
		RLog2(LOG_LEVEL_ERROR, "Pointer is NULL.");
		return WFS_ERR_INVALID_POINTER;
	}

	DWORD dwComponent = lpSelfTest->dwComponent;
	RLog2(LOG_LEVEL_COMMON, "Start: dwComponent=0x%X", dwComponent);

	long nRet = 0;
	FAIL_RET(nRet, this, CheckOpen());

	if(nRet = m_pPinpad->SelfTest(dwComponent))
	{
		RLog2(LOG_LEVEL_ERROR, "SelfTest fail, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	m_EncIOResult.Set(WFS_PIN_ENC_PROT_SZZT, NULL, 0);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}


//===========================================================================//
//                            Inner function                                 //
//===========================================================================//
inline HRESULT CXFSPINBase::NewPinpad()
{
	if(NULL == m_pPinpad)
	{
		RLog2(LOG_LEVEL_DEBUG, "szSDKName=%s, dwPortType=0x%X, dwCmdType=0x%X, szPortDesc=%s, dwExtendType=0x%X", m_szSDKName, m_dwPortType, m_dwCmdType, m_szPortDesc, m_dwExtendType);
		m_pPinpad = (IPinpad*)m_pDLFactory->Instance(m_szSDKName, INS_NAME_PINPAD, m_dwPortType, m_dwCmdType, m_szPortDesc, m_dwExtendType);
		if(NULL != m_pPinpad)
		{
			m_pPinpad->GetCapabilities(&m_stPinCaps);
			memcpy(&m_CCaps, &m_stPinCaps, sizeof(WFSPINCAPS));

			TCHAR szTemp[MAX_PATH] = {0};
			m_pPinpad->GetSoftVersion(szTemp);
			m_mapCapsExtra.Update(_T("Version(SDK)"), szTemp);

			/*memset(szTemp, 0, sizeof(szTemp));
			m_pPinpad->GetSoftVersion(szTemp, GetModuleHandle(NULL));
			m_mapCapsExtra.Update(_T("SPVer"), szTemp);*/

			memset(szTemp, 0, sizeof(szTemp));
			m_pSPManage->GetServiceNodeStr(m_szServiceName, "EnlargeFile", szTemp, MAX_PATH);
			if(TCSLEN(szTemp) == 0)
			{
				GetRunPath(szTemp, MAX_PATH, g_hModule);
				TCSCAT_S(szTemp, MAX_PATH, "EnlargeKey.dat");
				m_pSPManage->SetServiceNodeStr(m_szServiceName, "EnlargeFile", szTemp);
			}
			m_pPinpad->SetCaps(eCAP_MAPPINGPATH, 0, szTemp);


			DWORD dwUserKeyOffset = m_pSPManage->GetServiceNodeInt(m_szServiceName, "UserKeyOffset", 0x10);
			if(dwUserKeyOffset < 16)
			{
				dwUserKeyOffset = 16;
			}
			m_pPinpad->SetCaps(eCAP_UKO, dwUserKeyOffset);


			BOOL bEPPCmdSeq = m_pSPManage->GetServiceNodeInt(m_szServiceName, "EPPCmdSeq", 0);
			if(bEPPCmdSeq)
			{
				m_pPinpad->SetCaps(eCAP_CMDSEQ, TRUE);
			}
			
			BOOL bSplitBankSysMAC = m_pSPManage->GetServiceNodeInt(m_szServiceName, "SplitBankSysMAC", 1);
			if(!bSplitBankSysMAC)
			{
				m_pPinpad->SetCaps(eCAP_SPLIT_CBS_MAC, FALSE);
			}
		}
		/*else
		{
			RLog2(LOG_LEVEL_DEBUG, "Instance pinpad failed.");
		}*/
	}

	return 0;
}

HRESULT CXFSPINBase::Config()
{
	m_nEcho   = m_pSPManage->GetServiceNodeInt(m_szServiceName, "cEcho", 0);
	m_cEchoHW = (BYTE)m_pSPManage->GetServiceNodeInt(m_szServiceName, "cEchoHW", 0);
	m_cTimeOutCode   = (BYTE)m_pSPManage->GetServiceNodeInt(m_szServiceName, "cTimeOutCode", 0x80);
	m_cLongPressCode = (BYTE)m_pSPManage->GetServiceNodeInt(m_szServiceName, "cLongPressCode", 0x81);

	m_bIsLongPressedTimeout  = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsLongPressedTimeout", 0);
	m_bIsAysnCancelClearPins = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsAysnCancelClearPins", 0);
	m_bIsTimeoutClearPins    = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsTimeoutClearPins", 0);	
	m_nStartPinInputTimeout  = m_pSPManage->GetServiceNodeInt(m_szServiceName, "StartPinInputTimeout", 120);

	m_nDataEntryVoice = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsDataEntryVoice", 1);
	m_nPinEntryVoice  = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsPinEntryVoice", 1);
	m_nKeyEntryVoice  = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsKeyEntryVoice", 1);
	m_nPswEntryVoice  = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsPswEntryVoice", 1);

	m_nDESMacAlg  = m_pSPManage->GetServiceNodeInt(m_szServiceName, "DESMacAlgorithms", MAC_X9);
	m_nTDESMacAlg = m_pSPManage->GetServiceNodeInt(m_szServiceName, "TDESMacAlgorithms", MAC_CBC);
	m_nSM4MacAlg  = m_pSPManage->GetServiceNodeInt(m_szServiceName, "SM4MacAlgorithms", SM4MAC_CBC);

	m_nCustomerDataMode  = m_pSPManage->GetServiceNodeInt(m_szServiceName, "CustomerDataMode", 0);
	m_nIsCheckEnKeyUsage = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsCheckEnKeyUsage", 0);
	m_nIsDisablePressKey = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsDisablePressKey", 1);
	m_bIsRKLNeedRandom   = m_pSPManage->GetServiceNodeInt(m_szServiceName, "IsRklNeedRandom", 0);
	m_bExistKeyIsReady   = m_pSPManage->GetServiceNodeInt(m_szServiceName, "ExistKeyIsReady", 0);

	//CreateOffset和LocalDes相关的配置参数
	m_nOffsetGenAlgorithm    = m_pSPManage->GetServiceNodeInt(m_szServiceName, "OffsetGenAlgorithm", 0);
	m_nOffsetVerifyAlgorithm = m_pSPManage->GetServiceNodeInt(m_szServiceName, "OffsetVerifyAlgorithm", 0);
	m_nOffsetIndex           = m_pSPManage->GetServiceNodeInt(m_szServiceName, "OffsetIndex", 0);

	//LocalVisa相关的配置参数
	m_nVisaAlgorithm = m_pSPManage->GetServiceNodeInt(m_szServiceName, "VisaVerifyAlgorithm", 0);
	m_nVisaIndex     = m_pSPManage->GetServiceNodeInt(m_szServiceName, "VisaIndex", 0);
	m_nVisaLen       = m_pSPManage->GetServiceNodeInt(m_szServiceName, "VisaLength", 0);

	//KCV算法和长度
	m_nKCVLen    = m_pSPManage->GetServiceNodeInt(m_szServiceName, "KCVLength", 4);
	int nKCVMode = m_pSPManage->GetServiceNodeInt(m_szServiceName, "KCVMode", 2);
	if(nKCVMode == 0)
	{
		m_eKCVMode = KCVNONE;
	}
	else if(nKCVMode == 1)
	{
		m_eKCVMode = KCVSELF;
	}
	else
	{
		m_eKCVMode = KCVZERO;
	}

	TCHAR szbuf[MAX_PATH] = {0};
	m_pSPManage->GetServiceNodeStr(m_szServiceName, "00", szbuf, MAX_PATH);
	if(TCSLEN(szbuf) > 0)
	{
		sscanf(szbuf, "%d,%d,%d", m_n00, m_n00+1, m_n00+2);
	}

	m_pSPManage->GetServiceNodeStr(m_szServiceName, "000", szbuf, MAX_PATH);
	if(TCSLEN(szbuf) > 0)
	{
		sscanf(szbuf, "%d,%d,%d", m_n000, m_n000+1, m_n000+2);
	}

	return 0;
}

HRESULT CXFSPINBase::UpdateStatus()
{
	long nRet = 0;
	WORD fwDevice = WFS_PIN_DEVOFFLINE;
	
	NewPinpad();
	if(NULL == m_pPinpad)
	{
		return WFS_ERR_DEV_NOT_READY;
	}

	nRet = CheckOpen();

	UINT nLen = strlen(m_szVer);
	if(nRet == 0 && nLen == 0)
	{
		m_pPinpad->GetHardwareVersion(m_szVer);
		nLen = strlen(m_szVer);
	}

	if(nLen > 0)
	{
		fwDevice = WFS_PIN_DEVONLINE;
		m_mapStatusExtra.Update("FirmwareVersion", m_szVer);
	}
	else
	{
		fwDevice = WFS_PIN_DEVOFFLINE;
		m_mapStatusExtra.Update("FirmwareVersion", " ");
	}

	m_mapStatusExtra.Update("LastErrorDetail", m_pPinpad->GetErrorInfo(m_pPinpad->GetLastErrorCode()));


	//设备状态
	if(m_CStatus.fwDevice != fwDevice)
	{
		RLog2(LOG_LEVEL_DEBUG, "Status changed, [%d] -> [%d]", m_CStatus.fwDevice, fwDevice);
		m_CStatus.fwDevice = fwDevice;
		m_pSPKernel->FireDevStatus(m_CStatus.fwDevice);
		if (m_CStatus.fwDevice == WFS_PIN_DEVOFFLINE)
		{
			m_pSPKernel->FireSystError(WFS_SYSE_HARDWARE_ERROR, WFS_ERR_ACT_NOACTION);
		}
	}

	//加密模块状态
	if(m_CStatus.fwDevice == WFS_PIN_DEVONLINE)
	{
		if(m_pPinpad->XFS_GetKeyCount() > 0)
		{
			if(m_bExistKeyIsReady)
			{
				m_CStatus.fwEncStat = WFS_PIN_ENCREADY;
			}
			else
			{
				if(m_pPinpad->XFS_HadWorkKey())
				{
					m_CStatus.fwEncStat = WFS_PIN_ENCREADY;
				}
				else
				{
					m_CStatus.fwEncStat = WFS_PIN_ENCINITIALIZED;
				}
			}
		}
		else
		{
			m_CStatus.fwEncStat = WFS_PIN_ENCNOTINITIALIZED;
		}
	}
	else
	{
		m_CStatus.fwEncStat = WFS_PIN_ENCUNDEFINED;	
	}

	return WFS_SUCCESS;
}

inline HRESULT CXFSPINBase::CheckOpen()
{
	if(NULL == m_pPinpad)
	{
		RLog2(LOG_LEVEL_ERROR, "m_pPinpad is NULL, make sure instance pinpad success.");
		return WFS_ERR_DEV_NOT_READY;
	}

	HRESULT nRet = WFS_SUCCESS;
	memset(m_szVer, 0, sizeof(m_szVer));
	if(nRet = m_pPinpad->GetHardwareVersion(m_szVer))
	{
		m_pPinpad->Close();
		nRet = m_pPinpad->Open(m_szPortDesc);
		if(nRet != 0)
		{
			RLog2(LOG_LEVEL_DEBUG, "Failed to communicate with pinpad.");
			nRet = WFS_ERR_DEV_NOT_READY;
		}
	}

	return nRet;
}

HRESULT CXFSPINBase::Convert2SPError(long nErrNo)
{
	HRESULT hRes = WFS_ERR_HARDWARE_ERROR;

	switch(nErrNo)
	{
	case EC_UNSUPPORT:                            hRes = WFS_ERR_UNSUPP_COMMAND; break;
 	case EC_POINTER_NULL:                         hRes = WFS_ERR_INVALID_POINTER; break;
 	case EC_INVALID_DATA:                         hRes = WFS_ERR_INVALID_DATA; break;
 	case EC_INVALID_PARA:                         hRes = WFS_ERR_INVALID_DATA; break;
	
	case PIN_INVALID_COMMAND_PARA:                hRes = WFS_ERR_INVALID_DATA; break;
	case PIN_MAC_XOR_ERROR:                       hRes = WFS_ERR_PIN_MACINVALID; break;
	case PIN_MAC_CRC_ERROR:                       hRes = WFS_ERR_PIN_MACINVALID; break;
	case PIN_PRESS_KEY_TIMEOUT:                   hRes = WFS_ERR_TIMEOUT; break;
	case PIN_NOPSW_OR_ERROR:                      hRes = WFS_ERR_PIN_NOPIN; break;
	case PIN_DATA_TOOLONG:                        hRes = WFS_ERR_INVALID_DATA; break;
 	case PIN_COMMAND_UNSUPPORT:                   hRes = WFS_ERR_UNSUPP_COMMAND; break;
// 	case PIN_RECV_SPECIAL_KEY:                    hRes = ErrNo; break;
// 	case PIN_SERIAL_NUM_ERROR:                    hRes = ErrNo; break;
// 	case PIN_INVALID_RSA_SN:                      hRes = ErrNo; break;
// 	case PIN_GET_PINBLOCK_ERROR:                  hRes = ErrNo; break;
// 	case PIN_KEY_UNRELEASED:                      hRes = ErrNo; break;
	case PIN_INVALID_PIN_LENGTH:                  hRes = WFS_ERR_PIN_MINIMUMLENGTH; break;
	case PIN_EPP_NOT_INITIALIZED:                 hRes = WFS_ERR_DEV_NOT_READY; break;
	case PIN_ALGORITHM_UNSUPPORT:                 hRes = WFS_ERR_PIN_ALGORITHMNOTSUPP; break;
	case PIN_INNER_ERROR:                         hRes = WFS_ERR_INTERNAL_ERROR; break;
	case PIN_MAC_KEYCOMMAND_ERROR:                hRes = WFS_ERR_PIN_MACINVALID; break;
//	case PIN_SELFTEST_ERROR:                      hRes = ErrNo; break;
	case PIN_RANDOM_DATA_ERROR:                   hRes = WFS_ERR_PIN_RANDOMINVALID; break;
	case PIN_KEY_USEVIOLATION:                    hRes = WFS_ERR_PIN_USEVIOLATION; break;
	case PIN_KEY_NOT_LOADED:                      hRes = WFS_ERR_PIN_KEYNOTFOUND; break;
	case PIN_KEY_LOCKED:                          hRes = WFS_ERR_PIN_NOTALLOWED; break;
	case PIN_INVALID_MASTER_KEY:                  hRes = WFS_ERR_PIN_KEYNOTFOUND; break;
	case PIN_IMK_NOT_EXIST:                       hRes = WFS_ERR_PIN_KEYINVALID; break;
	case PIN_TMK_NOT_EXIST:                       hRes = WFS_ERR_PIN_KEYNOTFOUND; break;
	case PIN_KEY_NOT_EXIST:                       hRes = WFS_ERR_PIN_KEYNOTFOUND; break;
	case PIN_SAME_KEY_VALUE:                      hRes = WFS_ERR_PIN_DUPLICATEKEY; break;
	case PIN_INVALID_KEY_VALUE:                   hRes = WFS_ERR_PIN_INVALIDDATA; break;
	case PIN_INVALID_KEY_LENGTH:                  hRes = WFS_ERR_PIN_INVALIDKEYLENGTH; break;
//	case PIN_INVALID_IV_ATTRIBUTES:               hRes = ErrNo; break;
	case PIN_INVALID_KEY_ATTRIBUTES:              hRes = WFS_ERR_PIN_KEYINVALID; break;
	case PIN_INVALID_OFFSET_LENGTH:               hRes = WFS_ERR_PIN_INVALIDDATA; break;
//	case PIN_INVALID_LENGTH_OR_SUM:               hRes = ErrNo; break;
	case PIN_INVALID_DATA:                        hRes = WFS_ERR_PIN_INVALIDDATA; break;
	case PIN_INVALID_WRITE_MODE:                  hRes = WFS_ERR_PIN_MODENOTSUPPORTED; break;
	case PIN_INVALID_ENTRY_MODE:                  hRes = WFS_ERR_PIN_MODENOTSUPPORTED; break;
//	case PIN_PIN_VERIFICATION_FAILED:             hRes = ErrNo; break;
	case PIN_INVALID_USERBLOCK_ADDRESS:           hRes = WFS_ERR_PIN_INVALIDDATA; break;
	case PIN_INVALID_MODULUS_LENGTH:              hRes = WFS_ERR_PIN_INVALID_MOD_LEN; break;
// 	case PIN_INVALID_EXPONENT_LENGTH:             hRes = ErrNo; break;
// 	case PIN_INVALID_PKCS_STRUCTURE:              hRes = ErrNo; break;
// 	case PIN_INVALID_PKCS_PADDING:                hRes = ErrNo; break;
// 	case PIN_INVALID_SIGNATURE_LENGTH:            hRes = ErrNo; break;
	case PIN_SIG_VERIFICATION_FAILED:             hRes = WFS_ERR_PIN_SIGNATUREINVALID; break;
// 	case PIN_ENCRYPT_SUSPENDED:                   hRes = ErrNo; break;
// 	case PIN_AUTHENTICATE_LOCKED_HOURS:           hRes = ErrNo; break;
	case PIN_COMMAND_LOCKED:                      hRes = WFS_ERR_PIN_NOTALLOWED; break;
// 	case PIN_VERIFICATION_FAILED:                 hRes = ErrNo; break;
// 	case PIN_NOT_AUTHENTE:                        hRes = ErrNo; break;
// 	case PIN_INVALID_AUTHENTICATION_MODE:         hRes = ErrNo; break;
// 	case PIN_CERTIFICATE_NOT_EXIST:               hRes = ErrNo; break;
// 	case PIN_KCV_VERIFICATION_FAILED:             hRes = ErrNo; break;
	case PIN_INVALID_CERTIFICATE_FORMAT:          hRes = WFS_ERR_PIN_INVALIDCERTSTATE; break;
	case PIN_INVALID_CERTIFICATE_VERSION:         hRes = WFS_ERR_PIN_INVALIDCERTSTATE; break;
	case PIN_INVALID_CERTIFICATE_ISSUER:          hRes = WFS_ERR_PIN_INVALIDCERTSTATE; break;
	case PIN_INVALID_CERTIFICATE_VALIDITY:        hRes = WFS_ERR_PIN_INVALIDCERTSTATE; break;
	case PIN_INVALID_CERTIFICATE_SUBJECT:         hRes = WFS_ERR_PIN_INVALIDCERTSTATE; break;
	case PIN_INVALID_CERTIFICATE_ALGOR:           hRes = WFS_ERR_PIN_INVALIDCERTSTATE; break;
// 	case PIN_NO_CARD:                             hRes = ErrNo; break;
// 	case PIN_CARD_APDU_ERROR:                     hRes = ErrNo; break;
// 	case PIN_EMV_NOT_INITIALIZED:                 hRes = ErrNo; break;
// 	case PIN_EMV_NOT_READY:                       hRes = ErrNo; break;
// 	case PIN_EMV_NEED_REINITIALIZE:               hRes = ErrNo; break;
// 	case PIN_EMV_TIMEOUT:                         hRes = ErrNo; break;
	case PIN_INVALID_KEYID:                       hRes = WFS_ERR_PIN_KEYINVALID; break;
// 	case PIN_POWER_ERROR:                         hRes = ErrNo; break;
// 	case PIN_CERTIFICATE_ALREADY:                 hRes = ErrNo; break;
// 	case PIN_EPP_ALREADY_INITIALIZED:             hRes = ErrNo; break;
// 	case PIN_EMV_ALREADY_INITIALIZED:             hRes = ErrNo; break;
	default:                                      hRes = WFS_ERR_HARDWARE_ERROR; break;
	}
	
	return hRes;
}

HRESULT CXFSPINBase::FindKey(LPSTR lpsKey, WORD &wKeyId, DWORD dwCheckUse)
{
	wKeyId = KEY_INVALID;
	if(NULL == lpsKey || strlen(lpsKey) <= 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Key name is empty.");
		return WFS_ERR_PIN_INVALIDID;
	}

	DWORD dwAttr = 0;
	PINKEYDETAIL stDetail;
	long nRet = m_pPinpad->XFS_GetKeyDetail(lpsKey, &stDetail);
	if(nRet == 0)
	{
		wKeyId = stDetail.wKeyId;
		dwAttr = stDetail.dwKeyAttr;
	}

	if(wKeyId == KEY_INVALID)
	{
		RLog2(LOG_LEVEL_ERROR, "[%s] key is not found.", lpsKey);
		FireIllegalKeyAccess(lpsKey, WFS_ERR_PIN_KEYNOTFOUND);
		return WFS_ERR_PIN_KEYNOTFOUND;
	}

	if(dwCheckUse > 0 && (!(dwAttr & dwCheckUse)))
	{
		FireIllegalKeyAccess(lpsKey, WFS_ERR_PIN_USEVIOLATION);
		RLog2(LOG_LEVEL_ERROR, "key usage is wrong, [%s] need usage[0x%08X].", lpsKey, dwCheckUse);
		return WFS_ERR_PIN_USEVIOLATION;
	}

	return 0;
}

HRESULT CXFSPINBase::DeleteKey(LPSTR lpsKey)
{
	long nRet = 0;
	WORD wKeyId = KEY_INVALID;

	FAIL_RET(nRet, this, CheckOpen());

	FAIL_RET(nRet, this, FindKey(lpsKey, wKeyId));

	m_pPinpad->DeleteKey(wKeyId);

	if(m_pPinpad->XFS_GetKeyCount() <= 0)
	{
		m_CStatus.fwEncStat = WFS_PIN_ENCNOTINITIALIZED;
	}

	m_XDataResult.Set(NULL, 0);
	RLog2(LOG_LEVEL_COMMON, "_End: Delete key [%s] success.", lpsKey);
	return WFS_SUCCESS;
}

inline HRESULT CXFSPINBase::CheckActiveKey(ULONG ulActiveKeys, ULONG ulActiveFDKs, ULONG ulTerminateKeys, ULONG ulTerminateFDKs, ULONG ulFKMask, ULONG ulFDKMask)
{
	ULONG ulTempV = 0;
	TCHAR szTempName[512] = {0};

	if(ulActiveKeys == 0 && ulActiveFDKs == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "No active keys");
		return WFS_ERR_PIN_NOACTIVEKEYS;
	}

	ulTempV = ulActiveKeys ^ ulFKMask & ulActiveKeys;
 	if(ulTempV > 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Active key[ %s ] not in keyboard.", m_PressKey.GetFKsName(ulTempV, szTempName));
 		return WFS_ERR_PIN_KEYNOTSUPPORTED;
	}

	ulTempV = ulActiveFDKs ^ ulFDKMask & ulActiveFDKs;
 	if(ulTempV > 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Active FDK[ %s ] not in keyboard.", m_PressKey.GetFDKsName(ulTempV, szTempName));
 		return WFS_ERR_PIN_KEYNOTSUPPORTED;
	}

	ulTempV = ulTerminateKeys ^ ulActiveKeys & ulTerminateKeys;
 	if(ulTempV > 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Terminate key[ %s ] not in active keys.", m_PressKey.GetFKsName(ulTempV, szTempName));
 		return WFS_ERR_PIN_KEYNOTSUPPORTED;
	}

	ulTempV = ulTerminateFDKs ^ ulActiveFDKs & ulTerminateFDKs;
 	if(ulTempV > 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Terminate FDK[ %s ] not in active keys.", m_PressKey.GetFDKsName(ulTempV, szTempName));
 		return WFS_ERR_PIN_KEYNOTSUPPORTED;
	}

	return 0;
}

inline HRESULT CXFSPINBase::CheckTerminateKey(ULONG ulTerminateKeys, ULONG ulTerminateFDKs, BOOL bAutoEnd)
{
	if(bAutoEnd == FALSE && ulTerminateKeys == 0 && ulTerminateFDKs == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "No terminate keys.");
		return WFS_ERR_PIN_NOTERMINATEKEYS;
	}

	return 0;
}

HRESULT CXFSPINBase::DownLoadKey(LPSTR lpsKey, LPSTR lpEncKey, LPWFSXDATA lpxValue, DWORD dwUse, LPWFSXDATA lpxAA, char *lpKCV, EKCVMODE eKCVMode)
{
	WORD wKeyId = KEY_INVALID, wEncKeyId = KEY_INVALID;
	HRESULT nRet = 0;
	USHORT usKeyLen = 0;

	dwUse &= ~WFS_PIN_USENODUPLICATE;

	FAIL_RET(nRet, this, CheckOpen());

	ALLOT_KEYID(lpsKey, wKeyId);

	EKEYMODE eMode = GetLoadKeyMode(lpxAA);

	if(nRet = m_pPinpad->SetCaps(eCAP_KCVL, m_nKCVLen))
	{
		RLog2(LOG_LEVEL_WARN, "set kcv length fail, nRet=[%d]", nRet);
	}

	if(dwUse & WFS_PIN_USESECURECONSTRUCT)
	{
		if(dwUse & ~(WFS_PIN_USECRYPT | WFS_PIN_USEFUNCTION | WFS_PIN_USEMACING | WFS_PIN_USEKEYENCKEY 
			| WFS_PIN_USENODUPLICATE | WFS_PIN_USESVENCKEY | WFS_PIN_USESECURECONSTRUCT))
		{
			RLog2(LOG_LEVEL_ERROR, "the usage is not support.");
			return WFS_ERR_PIN_USEVIOLATION;
		}

		usKeyLen = lpxValue->usLength / 2;
		if(usKeyLen != 8 && usKeyLen != 16 && usKeyLen != 24)
		{
			RLog2(LOG_LEVEL_ERROR, "WFS_PIN_USESECURECONSTRUCT key length[%d] is wrong.", usKeyLen);
			return WFS_ERR_PIN_INVALIDKEYLENGTH;
		}

 		if(nRet = m_pPinpad->StoreKey(wKeyId, dwUse & ~WFS_PIN_USESECURECONSTRUCT, eMode, lpxValue->usLength, lpKCV, m_eKCVMode))
		{
			RLog2(LOG_LEVEL_ERROR, "store key failed, nRet=[%d].", nRet);
			return Convert2SPError(nRet);
		}
	}
	else
	{
		if(lpxValue == NULL || lpxValue->lpbData == NULL)
		{
			RLog2(LOG_LEVEL_ERROR, "key value is empty.");
			return WFS_ERR_PIN_KEYNOVALUE;
		}

		USHORT usKeyLen = lpxValue->usLength;
		if(dwUse & WFS_PIN_USECRYPT || dwUse & WFS_PIN_USEFUNCTION || dwUse & WFS_PIN_USEMACING ||
			 dwUse & WFS_PIN_USEKEYENCKEY || dwUse & WFS_PIN_USESVENCKEY || dwUse & WFS_PIN_USEPINLOCAL)
		{
			if(usKeyLen != 8 && usKeyLen != 16 && usKeyLen != 24)
			{
				RLog2(LOG_LEVEL_ERROR, "key length[%d] is wrong.", usKeyLen);
				return WFS_ERR_PIN_INVALIDKEYLENGTH;
			}
		}
		else if(dwUse & WFS_PIN_USERSAPUBLIC || dwUse & WFS_PIN_USERSAPRIVATE)
		{
			if(usKeyLen != 132)
			{
				RLog2(LOG_LEVEL_ERROR, "key length[%d] is wrong.", usKeyLen);
				return WFS_ERR_PIN_INVALIDKEYLENGTH;
			}
		}
		else
		{
			RLog2(LOG_LEVEL_ERROR, "the usage is not support.");
			return WFS_ERR_PIN_USEVIOLATION;
		}

		//检查主密钥
		if(lpEncKey != NULL && lpEncKey[0] != '\0')
		{
			FAIL_RET(nRet, this, FindKey(lpEncKey, wEncKeyId, WFS_PIN_USEKEYENCKEY));
		}

		if(nRet = m_pPinpad->LoadKey(wKeyId, dwUse & ~WFS_PIN_USECONSTRUCT, (char*)(lpxValue->lpbData), usKeyLen, wEncKeyId, lpKCV, eKCVMode, eMode))
		{
			RLog2(LOG_LEVEL_ERROR, "load key failed, nRet=[%d].", nRet);
			return Convert2SPError(nRet);
		}
	}

	if(m_bExistKeyIsReady)
	{
		m_CStatus.fwEncStat = WFS_PIN_ENCREADY;
	}
	else
	{
		if(m_pPinpad->XFS_HadWorkKey())
		{
			m_CStatus.fwEncStat = WFS_PIN_ENCREADY;
		}
		else
		{
			m_CStatus.fwEncStat = WFS_PIN_ENCINITIALIZED;
		}
	}

	return nRet;
}

HRESULT CXFSPINBase::LocalIBM(LPSTR lpsKey, LPWFSXDATA lpxKeyEncKey, USHORT usMaxPIN, BYTE bPadding, LPCSTR lpsDecTable, LPSTR lpsValidationData, USHORT usValDigits, BYTE B_EMode, char *lpInOutPVV)
{
	char szValidationData[32] = {0}, szDecTable[16] = {'0'};
	char lpKeyEncKey[32] = {0};
	BYTE bKeyEncKeyLen = 0;
	WORD wKeyId = KEY_INVALID;
	long nRet = 0;

	if(usMaxPIN > 16)
	{
		RLog2(LOG_LEVEL_ERROR, "the pin length is wrong, usMaxPIN=[%d].", usMaxPIN);
 		return WFS_ERR_PIN_INVALIDDATA;
	}

 	if(usValDigits > 16)
 	{
		RLog2(LOG_LEVEL_ERROR, "Validation data length is wrong, usValDigits=[%d], Max=16.", usValDigits);
 		return WFS_ERR_PIN_INVALIDDATA;
 	}

	if(NULL == lpsValidationData)
	{
		RLog2(LOG_LEVEL_ERROR, "Validation data is empty.");
 		return WFS_ERR_PIN_INVALIDDATA;
	}
	if(strlen(lpsValidationData) < usValDigits)
	{
		RLog2(LOG_LEVEL_ERROR, "Validation data length is less than usValdigits.");
 		return WFS_ERR_PIN_INVALIDDATA;
	}
	memcpy(szValidationData, lpsValidationData, usValDigits);
	bPadding = Padding2Hex(bPadding);
	if(bPadding == 0)
	{
		RLog2(LOG_LEVEL_ERROR, "Invalid Padding 0x%X.", bPadding);
		return WFS_ERR_PIN_INVALIDDATA;
	}
	while(usValDigits < 16) 
	{
		szValidationData[usValDigits++] = bPadding;
	}

	if(lpsDecTable)
	{
		int len = strlen(lpsDecTable);
		if(IsSplitChar(lpsDecTable, len))
		{
			memcpy(szDecTable, lpsDecTable, (len > 16 ? 16 : len));
		}
		else
		{
			RLog2(LOG_LEVEL_ERROR, "lpsDecTable contain the wrong chars.");
			return WFS_ERR_PIN_INVALIDDATA;
		}
	}

	if(lpxKeyEncKey && lpxKeyEncKey->lpbData && lpxKeyEncKey->usLength > 0)
	{
		bKeyEncKeyLen = (BYTE)lpxKeyEncKey->usLength;
		if(bKeyEncKeyLen != 8 && bKeyEncKeyLen != 16 && bKeyEncKeyLen != 24)
		{
			RLog2(LOG_LEVEL_ERROR, "Twice encrypt key length is wrong, length=[%d].", bKeyEncKeyLen);
			return WFS_ERR_PIN_INVALIDKEYLENGTH;
		}
		memcpy(lpKeyEncKey, lpxKeyEncKey->lpbData, bKeyEncKeyLen);
	}
	
	FAIL_RET(nRet, this, CheckOpen());

	FAIL_RET(nRet, this, FindKey(lpsKey, wKeyId, (WFS_PIN_USEFUNCTION | WFS_PIN_USEPINLOCAL)));

	nRet = m_pPinpad->PO_LocalIBM(wKeyId, lpKeyEncKey, bKeyEncKeyLen, szValidationData, B_EMode, lpInOutPVV, szDecTable, usMaxPIN, m_nOffsetIndex);
	
	return nRet;
}

HRESULT CXFSPINBase::SecureKeyEntryForN(USHORT usKeyLen, BOOL bAutoEnd, ULONG ulActiveFDKs, ULONG ulActiveKeys, 
	 ULONG ulTerminateFDKs, ULONG ulTerminateKeys, WORD wVerificationType)
{
	vector<BYTE> veCodes, veDisableCodes;
	ULONG ulFK, ulFDK;
	BYTE szBuf[128];
	DWORD i, dwLen = 0;
	WORD wComp;
	BOOL IsModifyCode = FALSE;
	long nRet;
	
	RLog2(LOG_LEVEL_COMMON, "Start:");

	IsModifyCode = m_nIsDisablePressKey;

	m_PressKey.GetSecureCodes(0xFFFFFFFF, 0xFFFFFFFF, veCodes);
	CAutoRecoverKeyCode AutoRecoverKeyCode(m_pPinpad, veCodes, IsModifyCode, ENTRY_MODE_KEY);

	m_PressKey.GetUniqueSecureCodes(ulActiveKeys, ulActiveFDKs, 
							  WFS_PIN_FK_0|WFS_PIN_FK_1|WFS_PIN_FK_2|WFS_PIN_FK_3|
							  WFS_PIN_FK_4|WFS_PIN_FK_5|WFS_PIN_FK_6|WFS_PIN_FK_7|
							  WFS_PIN_FK_8|WFS_PIN_FK_9|WFS_PIN_FK_A|WFS_PIN_FK_B|
						      WFS_PIN_FK_C|WFS_PIN_FK_D|WFS_PIN_FK_E|WFS_PIN_FK_F, 
							  veCodes, veDisableCodes);

	if(IsModifyCode && (nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size(), ENTRY_MODE_KEY)))
	{
		RLog2(LOG_LEVEL_ERROR, "write key mode initialize key codes fail, nRet=%d.", nRet);
		return Convert2SPError(nRet);
	}
	
	if(nRet = m_pPinpad->StartKeyInput(ESOUND(m_nKeyEntryVoice), (BYTE)usKeyLen, bAutoEnd ? true : false))
	{
		RLog2(LOG_LEVEL_ERROR, "open key input fail, nRet=%d", nRet);
		return Convert2SPError(nRet);
	}	
	CAutoCloseEntry AutoCloseEntry(m_pPinpad);
	
	if(m_pSPKernel->GetXFSVersion() >= XFSVER_310)
	{
		FireEnterData();
	}

	while(TRUE)
	{
		if(m_pSPKernel->IsRequestCancelled())
		{
			RLog2(LOG_LEVEL_WARN, "pin input is cancelled.");
			if(m_bIsAysnCancelClearPins)
			{
				m_usKeyLen = 0;
			}
			return WFS_ERR_CANCELED;
		}	
		if(m_pSPKernel->IsRequestTimeout())
		{
			RLog2(LOG_LEVEL_WARN, "pin input is timeout.");
			if(m_bIsTimeoutClearPins)
			{
				m_usKeyLen = 0;
			}
			return WFS_ERR_TIMEOUT;
		}

		//获取一次按键
		dwLen = 0;
		nRet = m_pPinpad->ReadText((char*)szBuf, dwLen, 100);
		for(i = 0; i < dwLen; ++i)
		{
			if(szBuf[i] == m_cLongPressCode && m_bIsLongPressedTimeout)  //长按键超时
			{
				RLog2(LOG_LEVEL_WARN, "pin input long pressed timeout occupy.");
				if(m_bIsTimeoutClearPins)
					m_usKeyLen = 0;
				return WFS_ERR_TIMEOUT;
			}	
			if(szBuf[i] == m_cTimeOutCode)  //不按键超时，因键盘已经关闭输入，只能返回超时
			{
				RLog2(LOG_LEVEL_WARN, "pin input long not press timeout occupy.");
				if(m_bIsTimeoutClearPins)
					m_usKeyLen = 0;	
				return WFS_ERR_TIMEOUT;
			}

			m_PressKey.GetKeyValue(szBuf[i], ulFK, ulFDK);

			//PIN按下
			if(szBuf[i] == 0x00 || (ulFK == 0 && ulFDK == 0) || szBuf[i] == m_cEchoHW)
			{
				m_usKeyLen++;
				FireKey(WFS_PIN_COMPCONTINUE, m_nEcho);

				if(m_usKeyLen < usKeyLen)
					continue;

				//禁止密码键
				if(IsModifyCode && (nRet = m_pPinpad->SetKeyboardCode((char*)(veDisableCodes.data()), veDisableCodes.size(), ENTRY_MODE_KEY)))
				{
					RLog2(LOG_LEVEL_ERROR, "disable 0~9,A~F keys fail, nRet=[%d].", nRet);
					return Convert2SPError(nRet);
				}
				
				//结束处理
				if(!bAutoEnd)
					continue;
				wComp = WFS_PIN_COMPAUTO;
				goto Exit;
			}

			//功能键处理
			//不在激活键范围内
			if(!(ulFK & ulActiveKeys || ulFDK & ulActiveFDKs))
 				continue;

			wComp = WFS_PIN_COMPCONTINUE;
			switch(ulFK)
			{
			case 0:
				wComp = WFS_PIN_COMPCONTFDK;
				break;
			case WFS_PIN_FK_ENTER:
				break;
				break;
			case WFS_PIN_FK_CANCEL:
				for( ; m_usKeyLen > 0; --m_usKeyLen)
					m_pPinpad->DelEntryBuffer();					
				break;
			case WFS_PIN_FK_CLEAR:
				for( ; m_usKeyLen > 0; --m_usKeyLen)
					m_pPinpad->DelEntryBuffer();					
				break;
			case WFS_PIN_FK_BACKSPACE:
				if(m_usKeyLen > 0)
				{
					m_usKeyLen--;
					m_pPinpad->DelEntryBuffer();
				}
				break;
			case WFS_PIN_FK_HELP:
				break;
			default:
				break;
			}
			FireKey(wComp, ulFK + ulFDK);

			//恢复密码键
			if(IsModifyCode && m_usKeyLen < usKeyLen && (nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size(), ENTRY_MODE_KEY)))
			{
				RLog2(LOG_LEVEL_ERROR, "restore 0~9,A~F keys fail, nRet=[%d].", nRet);
				return Convert2SPError(nRet);
			}			

			//终止键处理
			if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
			{
				switch(ulFK)
				{
				case 0:
					wComp = WFS_PIN_COMPFDK;
					break;
				case WFS_PIN_FK_ENTER: 
					if(m_usKeyLen < usKeyLen)
					{
						continue;
					}	
					wComp = WFS_PIN_COMPENTER;
					break;
				case WFS_PIN_FK_CANCEL:
					wComp = WFS_PIN_COMPCANCEL;
					break;
				case WFS_PIN_FK_CLEAR:
					wComp = WFS_PIN_COMPCLEAR;
					break;
				case WFS_PIN_FK_BACKSPACE:
					wComp = WFS_PIN_COMPBACKSPACE;
					break;
				case WFS_PIN_FK_HELP:
					wComp = WFS_PIN_COMPHELP;
					break;
				default:
					wComp = WFS_PIN_COMPFK;
					break;
				}
				goto Exit;
			}
		} //for
	} //while

Exit:
	m_SecureKeyEntryResult.Set(m_usKeyLen, wComp, NULL, 0);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SecureKeyEntryForH(USHORT usKeyLen, BOOL bAutoEnd, ULONG ulActiveFDKs, ULONG ulActiveKeys, 
	ULONG ulTerminateFDKs, ULONG ulTerminateKeys, WORD wVerificationType)
{
	vector<BYTE> veCodes, veDisableCodes;
	ULONG ulFK, ulFDK;
	BYTE szBuf[128];
	DWORD i, dwLen = 0;
	WORD wComp;
	BOOL IsModifyCode = FALSE;
	long nRet;
	
	RLog2(LOG_LEVEL_COMMON, "Start: ");

	IsModifyCode = m_nIsDisablePressKey;

	m_PressKey.GetSecureCodes(0xFFFFFFFF, 0xFFFFFFFF, veCodes);
	CAutoRecoverKeyCode AutoRecoverKeyCode(m_pPinpad, veCodes, IsModifyCode, ENTRY_MODE_KEY);

	m_PressKey.GetUniqueSecureCodes(ulActiveKeys, 
		                      ulActiveFDKs,
							  WFS_PIN_FK_0|WFS_PIN_FK_1|WFS_PIN_FK_2|WFS_PIN_FK_3|
							  WFS_PIN_FK_4|WFS_PIN_FK_5|WFS_PIN_FK_6|WFS_PIN_FK_7|
							  WFS_PIN_FK_8|WFS_PIN_FK_9|WFS_PIN_FK_A|WFS_PIN_FK_B|
						      WFS_PIN_FK_C|WFS_PIN_FK_D|WFS_PIN_FK_E|WFS_PIN_FK_F, 
							  veCodes, veDisableCodes);

	if(IsModifyCode && (nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size(), ENTRY_MODE_KEY)))
	{
		RLog2(LOG_LEVEL_ERROR, "write key mode initialize key codes fail, nRet=%d.", nRet);
		return Convert2SPError(nRet);
	}
	
	if(nRet = m_pPinpad->StartKeyInput(ESOUND(m_nKeyEntryVoice), (BYTE)usKeyLen, bAutoEnd ? true : false))
	{
		RLog2(LOG_LEVEL_ERROR, "open key input fail, nRet=%d", nRet);
		return Convert2SPError(nRet);
	}	
	CAutoCloseEntry AutoCloseEntry(m_pPinpad);

	if(m_pSPKernel->GetXFSVersion() >= XFSVER_310)
	{
		FireEnterData();
	}

	while(TRUE)
	{
		if(m_pSPKernel->IsRequestCancelled())
		{
			RLog2(LOG_LEVEL_WARN, "pin input is cancelled.");
			if(m_bIsAysnCancelClearPins)
			{
				m_usKeyLen = 0;
			}
			return WFS_ERR_CANCELED;
		}	
		if(m_pSPKernel->IsRequestTimeout())
		{
			RLog2(LOG_LEVEL_WARN, "pin input is timeout.");
			if(m_bIsTimeoutClearPins)
			{
				m_usKeyLen = 0;
			}
			return WFS_ERR_TIMEOUT;
		}

		//获取一次按键
		dwLen = 0;
		nRet = m_pPinpad->ReadText((char*)szBuf, dwLen, 100);
		for(i = 0; i < dwLen; ++i)
		{
			if(szBuf[i] == m_cLongPressCode && m_bIsLongPressedTimeout)  //长按键超时
			{
				RLog2(LOG_LEVEL_WARN, "pin input long pressed timeout occupy.");
				if(m_bIsTimeoutClearPins)
				{
					m_usKeyLen = 0;
				}
				return WFS_ERR_TIMEOUT;
			}	
			if(szBuf[i] == m_cTimeOutCode)  //不按键超时，因键盘已经关闭输入，只能返回超时
			{
				RLog2(LOG_LEVEL_WARN, "pin input long not press timeout occupy.");
				if(m_bIsTimeoutClearPins)
				{
					m_usKeyLen = 0;
				}
				return WFS_ERR_TIMEOUT;
			}

			m_PressKey.GetKeyValue(szBuf[i], ulFK, ulFDK);

			//PIN按下
			if(szBuf[i] == 0x00 || (ulFK == 0 && ulFDK == 0) || szBuf[i] == m_cEchoHW)
			{
				m_usKeyLen++;
				FireKey(WFS_PIN_COMPCONTINUE, m_nEcho);

				if(m_usKeyLen < usKeyLen)
					continue;

				//禁止密码键
				if(IsModifyCode && (nRet = m_pPinpad->SetKeyboardCode((char*)(veDisableCodes.data()), veDisableCodes.size(), ENTRY_MODE_KEY)))
				{
					RLog2(LOG_LEVEL_ERROR, "disable 0~9,A~F keys fail, nRet=[%d].", nRet);
					return Convert2SPError(nRet);
				}
				
				//结束处理
				if(!bAutoEnd)
					continue;
				wComp = WFS_PIN_COMPAUTO;
				goto Exit;
			}
			
			//不在激活键范围内
			if(!(ulFK & ulActiveKeys || ulFDK & ulActiveFDKs))
 				continue;

			//功能键处理
			wComp = WFS_PIN_COMPCONTINUE;
			switch(ulFK)
			{
			case 0:
				wComp = WFS_PIN_COMPCONTFDK;
				break;
			case WFS_PIN_FK_ENTER:
				break;
				break;
			case WFS_PIN_FK_CANCEL:
				for ( ; m_usKeyLen > 0; --m_usKeyLen)
					m_pPinpad->DelEntryBuffer();					
				break;
			case WFS_PIN_FK_CLEAR:
				for ( ; m_usKeyLen > 0; --m_usKeyLen)
					m_pPinpad->DelEntryBuffer();			
				break;
			case WFS_PIN_FK_BACKSPACE:
				if(m_usKeyLen > 0)
				{
					m_usKeyLen--;
					m_pPinpad->DelEntryBuffer();
				}	
				//恢复密码键
				break;
			case WFS_PIN_FK_HELP:
				break;
			default:
				break;
			}
			FireKey(wComp, ulFK + ulFDK);

			//恢复密码键			
			if(IsModifyCode && m_usKeyLen < usKeyLen && (nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size(), ENTRY_MODE_KEY)))
			{
				RLog2(LOG_LEVEL_ERROR, "restore 0~9,A~F keys fail, nRet=[%d].", nRet);
				return Convert2SPError(nRet);
			}			

			//终止键处理
			if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
			{
				switch(ulFK)
				{
				case 0:
					wComp = WFS_PIN_COMPFDK;
					break;
				case WFS_PIN_FK_ENTER: 
					if(m_usKeyLen < usKeyLen)
					{
						continue;
					}	
					wComp = WFS_PIN_COMPENTER;
					break;
				case WFS_PIN_FK_CANCEL:
					wComp = WFS_PIN_COMPCANCEL;
					break;
				case WFS_PIN_FK_CLEAR:
					wComp = WFS_PIN_COMPCLEAR;
					break;
				case WFS_PIN_FK_BACKSPACE:
					wComp = WFS_PIN_COMPBACKSPACE;
					break;
				case WFS_PIN_FK_HELP:
					wComp = WFS_PIN_COMPHELP;
					break;
				default:
					wComp = WFS_PIN_COMPFK;
					break;
				}
				goto Exit;
			}
		} //for
	} //while

Exit:
	m_SecureKeyEntryResult.Set(m_usKeyLen, wComp, NULL, 0);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINBase::SecureKeyEntryForS(USHORT usKeyLen, BOOL bAutoEnd, ULONG ulActiveFDKs, ULONG ulActiveKeys, 
	                                ULONG ulTerminateFDKs, ULONG ulTerminateKeys, WORD wVerificationType)
{
	vector<BYTE> veCodes, veDisableCodes, veShiftCodes, veDisableShiftCodes, veSwitchCodes, veCurrentCodes;
	ULONG ulFK, ulFDK;
	BYTE szBuf[128];
	DWORD i, dwLen = 0;
	WORD wComp;
	BOOL IsModifyCode = FALSE, bShiftState = FALSE;
	long nRet;
	
	RLog2(LOG_LEVEL_COMMON, "Start: ");

	IsModifyCode = m_nIsDisablePressKey;

	m_PressKey.GetSecureCodes(0xFFFFFFFF, 0xFFFFFFFF, veCodes);
	CAutoRecoverKeyCode AutoRecoverKeyCode(m_pPinpad, veCodes, IsModifyCode, ENTRY_MODE_KEY);

	m_PressKey.GetSoftShiftSecureCodes(ulActiveKeys, 
		                         ulActiveFDKs,
								 WFS_PIN_FK_0|WFS_PIN_FK_1|WFS_PIN_FK_2|WFS_PIN_FK_3|
								 WFS_PIN_FK_4|WFS_PIN_FK_5|WFS_PIN_FK_6|WFS_PIN_FK_7|
								 WFS_PIN_FK_8|WFS_PIN_FK_9|WFS_PIN_FK_A|WFS_PIN_FK_B|
								 WFS_PIN_FK_C|WFS_PIN_FK_D|WFS_PIN_FK_E|WFS_PIN_FK_F, 
								 veCodes, veDisableCodes, veShiftCodes, veDisableShiftCodes);
	if(IsModifyCode)
	{
		if(nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size(), ENTRY_MODE_KEY))
		{
			RLog2(LOG_LEVEL_ERROR, "write key mode initialize key codes fail, nRet=%d.", nRet);
			return Convert2SPError(nRet);
		}
		veCurrentCodes = veCodes;
	}
	
	if(nRet = m_pPinpad->StartKeyInput(ESOUND(m_nKeyEntryVoice), (BYTE)usKeyLen, bAutoEnd ? true : false))
	{
		RLog2(LOG_LEVEL_ERROR, "open key input fail, nRet=%d", nRet);
		return Convert2SPError(nRet);
	}	
	CAutoCloseEntry AutoCloseEntry(m_pPinpad);
	
	while(TRUE)
	{
		if(m_pSPKernel->IsRequestCancelled())
		{
			RLog2(LOG_LEVEL_WARN, "pin input is cancelled.");
			if(m_bIsAysnCancelClearPins)
			{
				m_usKeyLen = 0;
			}
			return WFS_ERR_CANCELED;
		}	
		if(m_pSPKernel->IsRequestTimeout())
		{
			RLog2(LOG_LEVEL_WARN, "pin input is timeout.");
			if(m_bIsTimeoutClearPins)
			{
				m_usKeyLen = 0;
			}
			return WFS_ERR_TIMEOUT;
		}

		//获取一次按键
		dwLen = 0;
		nRet = m_pPinpad->ReadText((char*)szBuf, dwLen, 100);
		for(i = 0; i < dwLen; ++i)
		{
			if(szBuf[i] == m_cLongPressCode && m_bIsLongPressedTimeout)  //长按键超时
			{
				RLog2(LOG_LEVEL_WARN, "pin input long pressed timeout occupy.");
				if(m_bIsTimeoutClearPins)
				{
					m_usKeyLen = 0;
				}
				return WFS_ERR_TIMEOUT;
			}	
			if(szBuf[i] == m_cTimeOutCode)  //不按键超时，因键盘已经关闭输入，只能返回超时
			{
				RLog2(LOG_LEVEL_WARN, "pin input long not press timeout occupy.");
				if(m_bIsTimeoutClearPins)
				{
					m_usKeyLen = 0;
				}
				return WFS_ERR_TIMEOUT;
			}

			m_PressKey.GetKeyValue(szBuf[i], ulFK, ulFDK);

			//SHIFT键
			if(ulFK == WFS_PIN_FK_SHIFT || (m_PressKey.m_ulSoftShiftFK != 0 && ulFK == m_PressKey.m_ulSoftShiftFK))
			{
				FireKey(WFS_PIN_COMPCONTINUE, ulFK + ulFDK);
				
				//SHIFT状态
				if(bShiftState && m_PressKey.m_eShiftMode == SHIFT_LONG) 
				{
					bShiftState = FALSE;
					veSwitchCodes = (m_usKeyLen >= usKeyLen ? veDisableCodes : veCodes);	
				}
				else
				{
					bShiftState = TRUE;
					veSwitchCodes = (m_usKeyLen >= usKeyLen ? veDisableShiftCodes : veShiftCodes);	
				}

				if(IsModifyCode && veCurrentCodes != veSwitchCodes)
				{
					if(nRet = m_pPinpad->SetKeyboardCode((char*)(veSwitchCodes.data()), veSwitchCodes.size(), ENTRY_MODE_KEY))
					{
						RLog2(LOG_LEVEL_ERROR, "write key mode key codes fail, nRet=%d.", nRet);
						return Convert2SPError(nRet);
					}	
					veCurrentCodes = veSwitchCodes;
				}		
				continue;
			}

			//非SHIFT键被按下
			if(bShiftState && m_PressKey.m_eShiftMode == SHIFT_LONG) 
			{
				bShiftState = TRUE;
				veSwitchCodes = (m_usKeyLen >= usKeyLen ? veDisableShiftCodes : veShiftCodes);
			}
			else
			{
				bShiftState = FALSE;
				veSwitchCodes = (m_usKeyLen >= usKeyLen ? veDisableCodes : veCodes);		
			}	

			if(IsModifyCode && m_PressKey.m_eShiftMode == SHIFT_LONG && veCurrentCodes != veSwitchCodes)
			{
				if(nRet = m_pPinpad->SetKeyboardCode((char*)(veSwitchCodes.data()), veSwitchCodes.size(), ENTRY_MODE_KEY))
				{
					RLog2(LOG_LEVEL_ERROR, "write key mode key codes fail, nRet=%d.", nRet);
					return Convert2SPError(nRet);
				}
				veCurrentCodes = veSwitchCodes;
			}		

			//PIN按下
			if(szBuf[i] == 0x00 || (ulFK == 0 && ulFDK == 0) || szBuf[i] == m_cEchoHW)
			{
				m_usKeyLen++;
				FireKey(WFS_PIN_COMPCONTINUE, m_nEcho);

				if(m_usKeyLen < usKeyLen)
					continue;

				//禁止密码键
				veSwitchCodes = (bShiftState ? veDisableShiftCodes : veDisableCodes);
				if(IsModifyCode && m_PressKey.m_eShiftMode == SHIFT_LONG && m_usKeyLen >= usKeyLen && veCurrentCodes != veSwitchCodes)
				{
					if(nRet = m_pPinpad->SetKeyboardCode((char*)(veSwitchCodes.data()), veSwitchCodes.size(), ENTRY_MODE_KEY))
					{
						RLog2(LOG_LEVEL_ERROR, "disable 0~9,A~F keys fail, nRet=%d.", nRet);
						return Convert2SPError(nRet);
					}
					veCurrentCodes = veSwitchCodes;
				}	
				
				//结束处理
				if(!bAutoEnd)
					continue;
				wComp = WFS_PIN_COMPAUTO;
				goto Exit;
			}
			
			//不在激活键范围内
			if(!(ulFK & ulActiveKeys || ulFDK & ulActiveFDKs))
 				continue;

			//功能键处理
			wComp = WFS_PIN_COMPCONTINUE;
			switch(ulFK)
			{
			case 0:
				wComp = WFS_PIN_COMPCONTFDK;
				break;
			case WFS_PIN_FK_ENTER:
				break;
				break;
			case WFS_PIN_FK_CANCEL:
				for ( ; m_usKeyLen > 0; --m_usKeyLen)
					m_pPinpad->DelEntryBuffer();					
				break;
			case WFS_PIN_FK_CLEAR:
				for ( ; m_usKeyLen > 0; --m_usKeyLen)
					m_pPinpad->DelEntryBuffer();			
				break;
			case WFS_PIN_FK_BACKSPACE:
				if(m_usKeyLen > 0)
				{
					m_usKeyLen--;
					m_pPinpad->DelEntryBuffer();
				}	
				//恢复密码键
				break;
			case WFS_PIN_FK_HELP:
				break;
			default:
				break;
			}
			FireKey(wComp, ulFK + ulFDK);

			//恢复密码键
			veSwitchCodes = (bShiftState ? veShiftCodes : veCodes);	
			if(IsModifyCode && m_PressKey.m_eShiftMode == SHIFT_LONG && m_usKeyLen < usKeyLen && veCurrentCodes != veSwitchCodes)
			{				
				if(nRet = m_pPinpad->SetKeyboardCode((char*)(veSwitchCodes.data()), veSwitchCodes.size(), ENTRY_MODE_KEY))
				{
					RLog2(LOG_LEVEL_ERROR, "restore 0~9,A~F keys fail, nRet=%d.", nRet);
					return Convert2SPError(nRet);
				}
				veCurrentCodes = veSwitchCodes;
			}				

			//终止键处理
			if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
			{
				switch(ulFK)
				{
				case 0:
					wComp = WFS_PIN_COMPFDK;
					break;
				case WFS_PIN_FK_ENTER: 
					if(m_usKeyLen < usKeyLen)
					{
						continue;
					}	
					wComp = WFS_PIN_COMPENTER;
					break;
				case WFS_PIN_FK_CANCEL:
					wComp = WFS_PIN_COMPCANCEL;
					break;
				case WFS_PIN_FK_CLEAR:
					wComp = WFS_PIN_COMPCLEAR;
					break;
				case WFS_PIN_FK_BACKSPACE:
					wComp = WFS_PIN_COMPBACKSPACE;
					break;
				case WFS_PIN_FK_HELP:
					wComp = WFS_PIN_COMPHELP;
					break;
				default:
					wComp = WFS_PIN_COMPFK;
					break;
				}
				goto Exit;
			}
		} //for
	} //while

Exit:
	m_SecureKeyEntryResult.Set(m_usKeyLen, wComp, NULL, 0);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

inline EKEYMODE CXFSPINBase::GetLoadKeyMode(LPWFSXDATA lpData)
{
	EKEYMODE eMode = KEY_SET;
	if(NULL != lpData && NULL != lpData->lpbData && lpData->usLength == 1)
	{
		BYTE bMode = lpData->lpbData[0];
		if(bMode == 0x31 || bMode == 0x1)
		{
			eMode = KEY_XOR;
		}
		else if(bMode == 0x32 || bMode == 0x2)
		{
			eMode = KEY_XOR2;
		}
		else if(bMode == 0x33 || bMode == 0x3)
		{
			eMode = KEY_XOR3;
		}
		else
		{
			eMode = KEY_SET;
		}
	}

	return eMode;
}

//特殊处理Cxx系列的clear和backspace键
void CXFSPINBase::SetKeyCodeforCxx(ULONG ulActiveKeys)
{
	ULONG ulFK = 0;
	ULONG ulFDK = 0; 

	m_PressKey.GetKeyValue(0x08, ulFK, ulFDK);
	if(ulFK == WFS_PIN_FK_BACKSPACE && ulFK & ulActiveKeys)
	{
		m_pPinpad->SetControlMode(0x21, 0xFFFF); //设置08为backspace键
	}

	m_PressKey.GetKeyValue(0x0C, ulFK, ulFDK);
	if(ulFK == WFS_PIN_FK_CLEAR && ulFK & ulActiveKeys)
	{
		m_pPinpad->SetControlMode(0x20, 0xFFFF); //设置08为clear键

		for(int i = 0; i < m_PressKey.m_vPinKeys.size(); i++)
		{
			if(m_PressKey.m_vPinKeys.at(i)->wKeyType != WFS_PIN_FDK && m_PressKey.m_vPinKeys.at(i)->ulFK == 0x0C)
			{
				m_PressKey.m_vPinKeys.at(i)->ulFK = 0x08;
			}
		}
	}
}
