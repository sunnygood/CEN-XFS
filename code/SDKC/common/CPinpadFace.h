/*****************************************************************************
* Copyright(C) 1993-2012,XXX Corporation.
*
* File Name:   CPinpadFace.h
* Description: the business class of all the pinpad device, it base on IPinpad.h
*
* Author:      liuguoxun
* Date:        2012-05-29
* Ver:         
*
*****************************************************************************/

#ifndef CPINPADFACE_H_20120529
#define CPINPADFACE_H_20120529

#pragma warning(disable: 4018 4127 4996)


#include "IPinpad.h"
#include "IPinpadEx.h"
#include "IPlatform.h"
#include "CCommandBase.h"


#ifdef  LOG_NAME_LOG
#undef  LOG_NAME_LOG
#define LOG_NAME_LOG     "Pinpad.log"
#undef  LOG_NAME_DEBUG
#define LOG_NAME_DEBUG   "HexPinpad.log"
#endif

/*****************************************************************************/

#pragma pack(push, 1)

typedef struct _key_ael
{
	WORD   wEnlargeId; //密钥的固件ID
	WORD   wVI;
	DWORD  dwKeyAttr;
}AELKEY, *LPAELKEY;

typedef struct _key_use_ael
{
    WORD   wAppId; //花瓶，密钥组ID，也是索引，范围为0 ~ (m_stCap.usKeyNum - 1)
	WORD   wKeyLen;
	DWORD  dwKeyAttr;
	WORD   wEnKeyId;
	WORD   wEnKeyLen;
	AELKEY stKey[MAX_AEL_KEY];
	char   szKeyName[MAX_KEY_NAME + 1];
	char   szKeyValue[24];
}ARTKEY, *LPARTKEY;

typedef struct _file_ael_head
{
	DWORD  dwFileLen;
	BYTE   ucFlag;
	BYTE   ucReserve;
}AELFILE, *LPAELFILE; //写入EPP的用户数据区时，会多写这个文件头

#pragma pack(pop)

	
/*****************************************************************************/


class CPinpadFace : public IPinpad, public CCommandBase
{
public:
	//通讯控制函数
	long Open(LPTSTR lpDesc);
	long ReOpen(DWORD dwAfterTime = 0);
	bool IsOpen(void) const;
	long Close(void);
	long SetBaudrate(DWORD dwBaudrate, BOOL bOnlyPort = 0);
	DWORD GetCurBaudrate(void);
	long SendData(const char *pData, unsigned long dwDataLen, unsigned long dwTimeout);
	long ReadData(char *pData, DWORD &dwInOutLen, DWORD dwTimeout);
	long Cancel(void);
	void *GetPortPointer(void) { return m_pPort; }


	//固件下载
	DWORD OpenFile(char *pOutBuf, LPCTSTR lpFileName, LPCSTR lpDecryptKey = 0, WORD wKeyLen = 0);
	long ChipCommunicate(BYTE Mode, char *pChipInfo = 0);
	long DownProgram(LPCSTR pcFileData, long nFileLen, BYTE Mode, void *hWnd = 0);


	//基本操作
	long Init(long nMode = 0);										  //设备初始化(清除数据)
	long Reset(void);											      //设备复位
	long SelfTest(long iComponent = 0);							      //设备进行自检
	long GetStatus(void);										      //获取设备状态
	long GetCapabilities(void *lpvCap);							      //获取设备能力
	long Execute(long iCmdIndex, char *pcBuffer, long &iBufOrRetLen); //执行命令
	long GetLastErrorCode(void);	    					          //获取错误码及错误码解释
	char *GetErrorInfo(long iCode);				                      //取错误码解释
	long GetHardwareID(char *pcModelId);						      //读硬件ID
	long GetHardwareSN(char *pcSn);						              //读硬件生产序列号	
	long GetHardwareVersion(char *pcVersion);				          //读硬件版本号
	long GetLoaderVersion(char *pcVersion);
	long GetSoftVersion(char *pcSoftVersion, void *hModule = 0);      //读软件版本号

	long SetCaps(ECAPS eCapsSwitch, DWORD dwValue, LPVOID lpReserve = 0);
	long SetControlMode(BYTE byControlMode, WORD ControlCode = 0xFFFF); //设置控制模式
	long GetRandom(ERANDOM eRandMode, DWORD &dwInOutLen, char *pRandom);
	long GetTerminalNO(char *pTNO);
	long SetTerminalNO(LPCSTR pTNO);	
	long SetKeyboardCode(LPCSTR lpKeyboardCodes, long nLen, ENTRYMODE eMode = ENTRY_MODE_TEXT);
	long GetKeyboardCode(char *lpKeyboardCodes, long &nOutLen, ENTRYMODE eMode = ENTRY_MODE_TEXT);
	long SetCustomerInfo(LPCSTR pInfo, DWORD dwLen, int iBlockNo = 0);
	long GetCustomerInfo(char *pInfo, DWORD &dwLen, int iBlockNo = 0);	
	long EPPSetTime(time_t tTime = 0);
	long EPPGetTime(time_t &tTime, char *pFormatTime = 0);
    void LogConfig(int nLevel = LOG_LEVEL_DATA, const char *szLogDir = NULL, int nSaveMode = -2, int nExtend = 0);
	
	
	//Input
	long OpenKeyboardAndSound(ESOUND eSound, ENTRYMODE eMode, DWORD dwActiveKey = 0, DWORD dwActiveFDK = 0);
	long StartPinInput(ESOUND eSound, BYTE MaxLen = 6, BYTE MinLen = 4, bool bAutoEnd = true, BYTE TimeOut = 30);
	long StartKeyInput(ESOUND eSound, BYTE KeyLen, bool bAutoEnd = true, BYTE BeginPos = 0);
	long StartPswInput(ESOUND eSound, EPSW_ID ePswId, EPSW ePsw, BYTE PswLen = 16, bool bAutoEnd = true);
	long UpdatePassword(EPSW_ID ePswId);
	long AutoUpdatePassword(EPSW_ID ePswId, LPCSTR lpOldPSW, LPCSTR lpNewPSW, LPCSTR lpKeyboardCodes = 0);
	WORD ParseKeyTable(LPCSTR lpNeedPress, LPCSTR lpKeyboardCodes, char *lpKeyTable, long nBufSize = 48); //根据键盘码解析虚拟按键位置
    long VirtualPress(LPCSTR lpKeyTable, BYTE cNumbers);
	long ReadText(char *lpText, DWORD &dwOutLen, DWORD dwTimeOut);
	long DelEntryBuffer(BOOL bClearAll = FALSE);

	
	//Download key
	long LoadKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD wKeyLen, WORD wEnKey = KEY_INVALID, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO, EKEYMODE eMode = KEY_SET);//clear/cipher
	long LoadKey_SAM(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD wKeyLen, WORD wEnKey, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO); //use SAM card to download work key
	long StoreKey(WORD wKeyId, DWORD dwKeyAttr, EKEYMODE eMode, WORD wKeyLen = 16, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO);//store key which is pressed on pinpad(16/32/48)	
	long VirtualBankKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpPSW, LPCSTR lpKeyboardCodes = 0, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO);
	long ReadKeyAttribute(WORD wKeyId, LPDWORD lpKeyAttr, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO);
	long DeleteKey(WORD wKeyId);	
	

	//Crypt
	long GetPinBlock(WORD wKeyId, BYTE PinLen, EPINFORMAT ePinFormat, LPCSTR lpCardNo, char *pPinBlock, WORD &wOutLen, BYTE Padding = 0x0F, WORD wKeyId2 = KEY_INVALID);	
	long Crypt(WORD wKeyId, ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, bool bEncrypt = true, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, BYTE Padding = 0);		
	long CalcMAC(WORD wKeyId, EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID); 
    long GetPinBlock_F(char *pPinBlock);
	long StartPinInput_F(WORD wKeyId, BYTE PinLen, EPINFORMAT ePinFormat, LPCSTR lpCardNo, bool bAutoEnd = true, BYTE TimeOut = 30, WORD wKeyId2 = KEY_INVALID);


	//Pin Offset(Local)
	long PO_LocalVISA(  WORD wKeyId, LPCSTR lpKeyEnKey, WORD wEnKeyLen, LPCSTR lpPAN, BYTE B_EMode, char *lpInOutPVV, BYTE MaxPIN = 4, BYTE POLen = 0);
	long PO_LocalIBM(   WORD wKeyId, LPCSTR lpKeyEnKey, WORD wEnKeyLen, LPCSTR lpPVD, BYTE B_EMode, char *lpInOutPVV, LPCSTR lpDecTable, BYTE MaxPIN, BYTE POLen = 0);
	long PO_LocalEurope(WORD wKeyId, LPCSTR lpKeyEnKey, WORD wEnKeyLen, LPCSTR lpEUD, BYTE B_EMode, char *lpInOutPVV, LPCSTR lpDecTable, BYTE MaxPIN, BYTE FirstEncLen = 0, BYTE POFirstEnc = 0, BYTE POLen = 0);


	//Certificate
	long CA_Load(WORD wCAId, DWORD dwKeyAttr, WORD wSigId, LPCSTR lpCA, WORD wCALen, char *lpOutData, LPWORD OutDataLen);
	long CA_Replace(WORD wCAId, DWORD dwKeyAttr, WORD wSigId, LPCSTR lpCA, WORD wCALen, char *lpOutData, LPWORD OutDataLen);
	long CA_Get(WORD wCAId, char *lpOutData, LPWORD OutDataLen);
	long CA_ImportPKCS7Key(WORD wKeyId, DWORD dwKeyAttr, WORD wCAId, LPCSTR lpPKCS7, WORD &wInOutLen, char *lpOutData, LPWORD lpKeyLen);


	//RSA
	long RSA_VendorInit(bool bSoftGenKeyPair = true, long nBits = 2048, LPCSTR SKOtherVendor = 0, WORD wSKLen = 0, LPCSTR PKOtherVendor = 0, WORD wPKLen = 0);
	long RSA_GenKeyPair(char *pSKDer, WORD &wSKOutLen, char *pPKDer, WORD &wPKOutLen, long nBits = 2048);
	long RSA_GenKeyPair(BYTE ucEPPKeyPairType = 0x31);
	long RSA_SigDes(LPCSTR pEnData, WORD &wInOutLen, LPCSTR lpSigKeyDer, WORD wSLen, char *pSig, bool bSigTag = false, LPCSTR lpRandom = 0, WORD wRandomLen = 0);	
	long RSA_Export(WORD wKeyId, LPSTR lpDer, WORD &wDerLen, LPSTR lpSig, WORD &wSigLen, ESIG_ALGORITHM &eSig, WORD wSigId = _VENDOR_PK);
	long RSA_ImpRSAKey(WORD wKeyId, LPCSTR lpDer, WORD wDerLen, LPCSTR lpSig, WORD wSigLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wSigId = _VENDOR_PK, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0);	
	long RSA_ImpDESKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpDer, WORD wDerLen, LPCSTR lpSig, WORD wSigLen, bool bRandom = true, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, 
		     WORD wHostPK = _HOST_PK, WORD wDecryptId = _EPP_CRY_SK, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0, WORD *lpDesKeyLen = 0);
	
	long RSA_ImpHostVersion(LPCSTR lpDer, WORD wDerLen, LPCSTR lpSig, WORD wSigLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wSigId = _VENDOR_PK);
	long RSA_ExpHostVersion(char *lpTag, WORD &wTLen, ESIG_ALGORITHM &eSig);
	long RSA_EncryptRC(LPCSTR lpRC, WORD &wInOutLen, char *lpTag, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wEnHostId = _HOST_PK, WORD wSigId = _EPP_SIG_SK);
	
	
    //China Secure Encryption Algorithm
	long SM_GetInfo(char* pInfo);
	long SM3_Algorithm(LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut);
	long SM2_VendorInit(bool bSoftGenKeyPair = true, LPCSTR lpIDa = 0, WORD wIDLen = 0, LPCSTR SKOtherVendor = 0, WORD wSKLen = 0, LPCSTR PKOtherVendor = 0, WORD wPKLen = 0);
	long SM2_GenKeyPair(WORD wSKID, WORD wPKID);
	long SM2_GenKeyPair(char *lpSK, WORD &wSKLen, char *lpPK, WORD &wPKLen);
	long SM2_CalcZa(WORD wKeyId, LPCSTR lpIDa, WORD &wInOutLen, char *pZa, WORD wIDType = 0);
	long SM2_ExportKey(WORD wKeyId, char *lpPK, WORD &wPKLen, ESIG_ALGORITHM &eSig, char *lpZa, WORD &wZaLen, char *lpSig, WORD &wSigLen, WORD wSigId = _VENDOR_SM2_PK);
	long SM2_ImportKey(WORD wKeyId, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpZa, WORD wZaLen, LPCSTR lpSig, WORD wSigLen,
						ESIG_ALGORITHM eSig = SIGN_SM2_GM_T_2012, WORD wSigId = _VENDOR_SM2_PK, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0);	
	long SM2_ImportSM4Key(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpEncKey, WORD wEKeyLen, WORD wHostPK, LPCSTR lpZa, WORD wZaLen, 
						LPCSTR lpSig, WORD wSigLen, char *lpKCVRet = 0, LPDWORD pKCVL = 0, bool bRandom = false, WORD *lpSM4KeyLen = 0, 
							EKCVMODE eKCV = KCVZERO, WORD wDecryptId = _EPP_SM2_CRYPT_SK, ESIG_ALGORITHM eSig = SIGN_SM2_GM_T_2012);
	
	long SM2_KeyExchangeA(char *lpZa, WORD &wZaLen, char *lpPa, WORD &wPaLen, char *lpRa, WORD &wRaLen, char *lpASig, WORD &wASigLen);
	long SM2_KeyExchangeB(LPCSTR lpZa, WORD wZaLen, LPCSTR lpPa, WORD wPaLen, LPCSTR lpRa, WORD wRaLen, LPCSTR lpASig, WORD wASigLen, 
								char *lpZb, WORD &wZbLen, char *lpRb, WORD &wRbLen, char *lpSb, WORD &wSbLen, char *lpBSig, WORD &wBSigLen, WORD wSigPKa = _HOST_SM2_SIG_PK);
	long SM2_KeyAgreementA(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpZb, WORD wZbLen, LPCSTR lpRb, WORD wRbLen, LPCSTR lpSb, WORD wSbLen, LPCSTR lpBSig, WORD wBSigLen, 
		            char *lpSa, WORD &wSaLen, char *lpKCVRet, WORD &KCVLen, WORD wExcPKb = _HOST_SM2_EXC_PK, WORD wSigPKb = _HOST_SM2_SIG_PK, EKCVMODE eKCV = KCVZERO, WORD wKeyLen = 16);
	long SM2_KeyAgreementB(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpSa, WORD wSaLen, char *lpKCVRet, WORD &wKCVLen, EKCVMODE eKCV = KCVZERO);


	//EMV
	long EMV_ImportPK(WORD wKeyId, DWORD dwKeyAttr, WORD wSigId, EEMV eScheme, LPCSTR lpDataIn, WORD wInLen, char *lpOut, LPWORD lpOutLen);


	//IC
	long IC_PowerOn(char *cpExValue, long &nOutLen);
	long IC_PowerOff(void);
	long IC_Control(LPCSTR lpValue, long &nInOutLen, char *cpExValue);
	long IC_SetParameter(BYTE iIC, WORD wICType);
	long IC_ReadParameter(BYTE iIC, WORD &wICType);


	//RemoveInstall
	long RI_GetStatus(long &nStatus);
	long RI_RemoveInstallAuth(EINSTALL_AUTH eMode);
	long RI_AutoRemoveInstall(EINSTALL_AUTH eMode, LPCSTR lpPSW1, LPCSTR lpPSW2, LPCSTR lpKeyboardCodes = 0);
	long RI_RemoveInstallBySig(EINSTALL_AUTH eMode, LPCSTR lpTag, DWORD dwTagLen, WORD wKeyID = _HOST_SM2_SIG_PK_DLL);


	//LCD
	long LCD_ShowInfo(LPCSTR lpInfo, BYTE Len, BYTE Mode = 0, BYTE Pos = 0);


	//Big Keyboard(PC Keyboard)
	long BKG_GetVersion(char *pcHVersion);


	//ECC
	long ECC_CryptComData(WORD wDataKeyId, WORD wMacKeyId, BYTE bReturnMode, LPCSTR lpIV, WORD wRealSCmdLen, LPCSTR lpSend, WORD wSLen, LPCSTR lpMac,
								   WORD &wRealRCmdLen, char *lpReceive, WORD &wRLen, char *lpRetMac);
	long ECC_CryptComData_S(WORD wDataKeyId, WORD wMacKeyId, BYTE bReturnMode, LPCSTR lpIV, WORD wRealSCmdLen, LPCSTR lpSend, WORD wSLen, LPCSTR lpMac);
	long ECC_CryptComData_R(WORD wTimeout, WORD &wRealRCmdLen, char *lpReceive, WORD &wRLen, char *lpRetMac);
	

	//Key name is string, such as CEN/XFS(WOSA)
	WORD XFS_AllotKeyId(LPCSTR lpKeyName);
	WORD XFS_GetKeyId(LPCSTR lpKeyName) const;
	WORD XFS_GetKeyCount(void) const;
	BOOL XFS_HadWorkKey(void) const;
	long XFS_GetKeyDetail(WORD wKeyId, LPPINKEYDETAIL lpDetail) const;
	long XFS_GetKeyDetail(LPCSTR lpKeyName, LPPINKEYDETAIL lpDetail) const;


	//Raw interface, for PCI, SM or other test
	long RAW_GenKeys(ECRYPT eMode, char *lpSK, WORD &wSKLen, char *lpPK, WORD &wPKLen);

	long RAW_Hash(EHASH eHashAlg, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut);	
	long RAW_Crypt(    ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut, bool bEncrypt = true);
	long RAW_MAC(         EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut);
	long RAW_GenSig(   ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut);
	long RAW_VerifySig(ECRYPT eMode, LPCSTR lpDataIn, DWORD dwInLen,     LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, LPCSTR lpSigIn, WORD wSigLen);


    //software interface
	long Soft_Hash(   EHASH eHashAlg, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut);
	long Soft_Crypt(    ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut, bool bEncrypt = true);
	long Soft_MAC(         EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut);
	long Soft_GenSig(   ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut);
	long Soft_VerifySig(ECRYPT eMode, LPCSTR lpDataIn, DWORD dwInLen,     LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, LPCSTR lpSigIn, WORD wSigLen);

	DWORD Soft_Hex2Bin(char *pBin, DWORD dwBufLen, LPCSTR pHex, DWORD dwLen);
	DWORD Soft_Bin2Hex(char *pHex, DWORD dwBufLen, LPCSTR pBin, DWORD dwLen);

	DWORD Soft_SM3Calc(LPCSTR lpData, DWORD dwDataLen, char *lpHash);
	long  Soft_CalcZa(LPCSTR lpPK, WORD wPKLen, LPCSTR lpIDa, WORD &wInOutLen, char *pZa, WORD wIDType = 0);
	long  Soft_SM2ExchangeA(LPCSTR Za, WORD wZaLen, LPCSTR Zb, WORD wZbLen, LPCSTR PKa, WORD wPKaLen, LPCSTR PKb, WORD wPKbLen, 
		               LPCSTR RandomPKa, WORD wRandomPKaLen, LPCSTR RandomPKb, WORD wRandomPKbLen, LPCSTR SKa, WORD wSKaLen, LPCSTR RandomSKa, WORD wRandomSKaLen, 
					   char *Sa, WORD &wSaLen, char *S1, WORD &wS1Len, char *TargetKey, WORD wKeyLen = 16);
	long  Soft_SM2ExchangeB(LPCSTR Za, WORD wZaLen, LPCSTR Zb, WORD wZbLen, LPCSTR PKa, WORD wPKaLen, LPCSTR PKb, WORD wPKbLen, 
		               LPCSTR RandomPKa, WORD wRandomPKaLen, LPCSTR RandomPKb, WORD wRandomPKbLen, LPCSTR SKb, WORD wSKbLen, LPCSTR RandomSKb, WORD wRandomSKbLen, 
					   char *Sb, WORD &wSbLen, char *S2, WORD &wS2Len, char *TargetKey, WORD wKeyLen = 16);



public:
	explicit CPinpadFace(EPORT ePort, EExchange eType, EPIN_EXTEND eExtend);
	virtual ~CPinpadFace();


	virtual long RegisterMyFeatures(BOOL bAll = TRUE); //注册自己的特性


	void SetPinpadType(EPIN_TYPE eEPPType, WORD wSubType = 0)
	{
		m_stCap.ePinpadType = eEPPType;
		m_stCap.wSubType = wSubType;
	}


protected:
	virtual long Inner_Init(long nMode = 0) = 0;
	virtual long Inner_Reset(void) = 0;
	virtual long Inner_SelfTest(long iComponent = 0) = 0;
	virtual long Inner_GetLoaderVersion(char *pcVersion) = 0;
	virtual long Inner_ChipCommunicate(BYTE Mode, char *pChipInfo) = 0;
	virtual long Inner_DownProgram(LPCSTR pcFileData, long nFileLen, BYTE Mode, void *hWnd) = 0;
	virtual long Inner_SetKeyboardCode(LPCSTR lpKeyboardCodes, long nLen, ENTRYMODE eMode = ENTRY_MODE_TEXT) = 0;
	virtual long Inner_GetKeyboardCode(char *lpKeyboardCodes, long &nOutLen, ENTRYMODE eMode = ENTRY_MODE_TEXT) = 0;
	virtual long Inner_SM2VendorInit(bool bSoftGenKeyPair, LPCSTR lpIDa, WORD wIDLen, LPCSTR SKOtherVendor, WORD wSKLen, LPCSTR PKOtherVendor, WORD wPKLen) = 0;
	virtual long Inner_RSAVendorInit(bool bSoftGenKeyPair = true, long nBits = 2048, LPCSTR SKOtherVendor = 0, WORD wSKLen = 0, LPCSTR PKOtherVendor = 0, WORD wPKLen = 0) = 0;
	virtual long Inner_ImpRSAKey(WORD wKeyId, LPCSTR lpDer, WORD wDerLen, LPCSTR lpSig, WORD wSigLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wSigId = _VENDOR_PK, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0) = 0;	
	virtual long Inner_RSAImpDESKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpTag, WORD wTagLen, bool bRandom = true, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wHostPK = 1, WORD wDecryptId = _EPP_CRY_SK, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0, WORD *lpDesKeyLen = 0) = 0;

	virtual long Inner_LoadKeySAM(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD wKeyLen, WORD wEnKey, char *lpKCVRet, EKCVMODE eKCV) = 0;
	virtual long Inner_DeleteKey(WORD wKeyId) = 0;
	virtual long Inner_ReadKeyAttr(WORD wKeyId, LPDWORD lpKeyAttr, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0, WORD wVI = 0) = 0;
	virtual long Inner_StoreKey(WORD wKeyId, DWORD dwKeyAttr, EKEYMODE eMode, long iKeyLen = 16, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0) = 0;
	virtual long Inner_VirtualBankKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long nLen, LPCSTR lpPSW, LPCSTR lpKeyboardCodes, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0) = 0;

	virtual long Inner_LoadKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, WORD wEnKey = KEY_INVALID, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0) = 0;
	virtual long Inner_GetPinBlock(WORD wKeyId, BYTE PinLen, EPINFORMAT ePinFormat, LPCSTR lpCardNo, char *pPinBlock, WORD &wOutLen, BYTE Padding = 0x0F, WORD wKeyId2 = KEY_INVALID, WORD wVI = 0, WORD wVI2 = 0) = 0;
	virtual long Inner_Crypt(WORD wKeyId, ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, bool bEncrypt = true, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, WORD wVI = 0) = 0;		
	virtual long Inner_MAC(WORD wKeyId, EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, WORD wVI = 0) = 0; 
	virtual long SM4_LoadKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD iKeyLen, WORD wEnKey = KEY_INVALID, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0) = 0;
	virtual long SM4_GetPinBlock(WORD wKeyId, BYTE PinLen, EPINFORMAT ePinFormat, LPCSTR lpCardNo, char *pPinBlock, WORD &wOutLen, BYTE Padding = 0x0F, WORD wKeyId2 = KEY_INVALID, WORD wVI = 0, WORD wVI2 = 0) = 0;
	virtual long SM4_Crypt(WORD wKeyId, ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, bool bEncrypt = true, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, WORD wVI = 0) = 0;
	virtual long SM4_MAC(WORD wKeyId, EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, WORD wVI = 0) = 0;
	virtual long SM4_StoreKey(WORD wKeyId, DWORD dwKeyAttr, EKEYMODE eMode, long iKeyLen, EKCVMODE eKCV, char *lpKCVRet) = 0;

	virtual	long RSA_ExpEppSN(char *lpSNTag, WORD &wSNTLen, ESIG_ALGORITHM &eSig, WORD wSigId = _VENDOR_PK) = 0;
	virtual long RSA_ExpEppPK(char *lpPKTag, WORD &wPKTLen, ESIG_ALGORITHM &eSig, WORD wKeyId = _EPP_CRY_PK, WORD wSigId = _VENDOR_PK) = 0;

	virtual long RandomData(DWORD &dwInOutLen, char *pRandom) = 0;
	virtual long RI_RandomData(DWORD &dwInOutLen, char *pRandom) = 0;
	virtual long RSA_RandomData(DWORD &dwInOutLen, char *pRandom) = 0;

	virtual long AllotEnlargeKeyId(WORD wAppId, DWORD dwKeyAttr, WORD &wAelId, WORD &wIndex) = 0;
	virtual long ExecuteEx(long iCmdIndex, char *pcBuffer, long &iBufOrRetLen) = 0;
	virtual long Authentication(WORD wKeyId, EAUTHMODE eMode = USE_KEY) = 0;
	virtual DWORD ConverKeyAttr(DWORD dwKeyAttr) = 0;
	virtual BYTE ConvertFormat(EPINFORMAT ePinFormat) = 0;
	virtual	long ExchangeKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, WORD wEnKey, WORD wVI = 0, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0) = 0;
	virtual long SetEntryMode(ENTRYMODE eMode, DWORD dwActiveKey = 0, DWORD dwActiveFDK = 0) = 0;	
	virtual long Cmd_RemoveInstallBySig(EINSTALL_AUTH eMode, LPCSTR lpTag, DWORD dwTagLen, WORD wKeyID = _HOST_SM2_SIG_PK_DLL) = 0;
	virtual long Inner_RemoveInstallBySig(EINSTALL_AUTH eMode, LPCSTR lpKey, WORD wKeyLen, WORD wKeyID = _HOST_SM2_SIG_PK_DLL) = 0;


	virtual long RSA_Encrypt(WORD wKeyId, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut) = 0;
	virtual long SM2_Encrypt(WORD wKeyId, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut) = 0;


	virtual long RAW_Crypt_DES(ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpIV, WORD wIVLen, char *lpOut, bool bEncrypt = true) = 0;
	virtual long RAW_MAC_DES(EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpIV, WORD wIVLen, char *lpOut) = 0;
	virtual long RAW_Crypt_RSA(LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpModulus, WORD wModulusLen, LPCSTR lpExponent, WORD wExponentLen, char *lpOut) = 0;
	virtual long RAW_GenSig_RSA(LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpModulus, WORD wModulusLen, LPCSTR lpExponent, WORD wExponentLen, char *lpOut) = 0;
	virtual long RAW_VerifySig_RSA(LPCSTR lpDataIn, DWORD dwInLen, LPCSTR lpModulus, WORD wModulusLen, LPCSTR lpExponent, WORD wExponentLen, LPCSTR lpSigIn, WORD wSigLen) = 0;
	virtual long RAW_Crypt_SM2(LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpIV, WORD wIVLen, char *lpOut, bool bEncrypt = true) = 0;
	virtual long RAW_GenSig_SM2(LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut) = 0;
	virtual long RAW_VerifySig_SM2(LPCSTR lpDataIn, DWORD dwInLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, LPCSTR lpSigIn, WORD wSigLen) = 0;
	virtual long RAW_Crypt_SM4(ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpIV, WORD wIVLen, char *lpOut, bool bEncrypt = true) = 0;
	virtual long RAW_SM2_GenKeys(char *lpSK, WORD &wSKLen, char *lpPK, WORD &wPKLen) = 0;



protected:
	long ReadEnlargeKeyFromEPP(void);
	long SaveEnlargeKey2EPP(BOOL bInit = FALSE);
	long ReadEnlargeKeyFromFile(void);
	long SaveEnlargeKey2File(void);
	long ReadEnlargeKey(void);
	long SaveEnlargeKey(BOOL bInit = FALSE);
	long GetEnlargeKeyInfo(WORD wAppId, DWORD dwNeedAttr, AELKEY &stAelKey, LPARTKEY pArtKey = NULL);
	long GetEnKeyInfo(WORD wEnKeyAppId, ARTKEY &stArtKey);
	long GetEnKeyDownKeyCount(WORD wEnKeyId);
	long GetMyVI(WORD wKeyId);

	long Soft_Random(DWORD dwNeedLen, char *lpDataOut);
	long DPASPA_Enable(BOOL bEnable);
	long GetDeviceInfo(char *pInfo, long &nInOutLen);

	long DES_Crypt(LPCSTR lpDataIn, DWORD dwInLen, LPCSTR lpKey, WORD wKeyLen, char *lpOut, bool bEncrypt = true);
	long Soft_CalcMAC(LPCSTR lpDataIn, DWORD dwInLen, LPCSTR lpKey, WORD wKeyLen, EMAC eMac, LPCSTR lpIV, char *lpOut);

	inline ESIG_ALGORITHM ConverSigAlg(BYTE SigAlg)
	{
		ESIG_ALGORITHM eSig = SIGN_RSASSA_V15;
		if(SigAlg == 0x0 || SigAlg == 0x30)
		{
			eSig = SIGN_NA;
		}
		else if(SigAlg == 0x1 || SigAlg == 0x31)
		{
			eSig = SIGN_RSASSA_V15;
		}
		else
		{
			eSig = SIGN_RSASSA_PSS;
		}

		return eSig;
	}
	

protected:
	EPIN_EXTEND  m_eExtend;   //扩展属性
	PINCAPS      m_stCap;     //键盘能力

	long   m_nChipSubType;    //芯片子类型标志(用于固件下载)
	WORD   m_wInsIndex;       //实例化对象的索引号 
	DWORD  m_dwActiveKey;
	DWORD  m_dwActiveFDK;

	ARTKEY m_stArtKey[MAX_MAP_KEY];
	WORD   m_wRandVI[32];
	WORD   m_wEMVImportSchemes[MAX_EMV_SCHEMES + 1];

	TCHAR  m_szEnlargeFile[MAX_PATH];
	char   m_szKeyCode[MAX_KEY_TABLE];
	char   m_szKeyNameBuf[MAX_MAP_KEY][MAX_KEY_NAME + 1]; //密钥映射字符串名的缓冲区
};


#endif //CPINPADFACE_H_20120529

