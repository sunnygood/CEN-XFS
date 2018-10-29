/*****************************************************************************
* Copyright(C) 1993-2012,ZT Corporation.
*
* File Name:   CPinpadBase.h
* Description: the base class of all the pinpad device, the templet is ePIN_PCI
*
* Author:      liuguoxun
* Date:        2012-05-28
* History 1: 
*
*****************************************************************************/

#ifndef CPINPADBASE_H_20120528
#define CPINPADBASE_H_20120528

#include "CPinpadFace.h"


/*****************************************************************************/

#define  ONCE_LEN           2016L
#define  ADD_MAC_LEN           4L //打包时需要增加MAC的长度



//当用户直接指定键盘类型，需要检测子类型
#define  MYSELF_SUBTYPE() \
			if(m_stCap.wSubType == SUBVER_NONE && m_pPort->IsOpen()) \
			{\
				RLog2(LOG_LEVEL_DEBUG, "m_stCap.wSubType == SUBVER_NONE && m_pPort->IsOpen()");\
				WORD wSubType = 0;\
				EPIN_TYPE ePinType = IsMyself(wSubType, m_pPort);\
				if(ePinType != ePIN_UNKNOWN)\
				{\
					m_stCap.wSubType = wSubType;\
				}\
			}

	
/*****************************************************************************/

class CPinpadBase : public CPinpadFace
{
public:
	explicit CPinpadBase(EPORT ePort, EExchange eType, EPIN_EXTEND eExtend);
	virtual ~CPinpadBase();

	virtual long RegisterMyFeatures(BOOL bAll = TRUE); //注册自己的特性


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
	virtual long Inner_LoadKeySAM(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD wKeyLen, WORD wEnKey, char *lpKCVRet, EKCVMODE eKCV);

	virtual long PackData(PSTCOMMAND pstCmd, LPCSTR pcData, WORD wDataLen, char *pcPackData);//打包数据(HEX格式、非拆分字符串格式传入)
	virtual long UnPackData(PSTCOMMAND pstCmd, LPCSTR pcData, WORD wDataLen, char *pcRetData, long &iBufOrRetLen); //数据解包(HEX格式、非拆分字符串格式输出)

	virtual long ExecuteEx(long iCmdIndex, char *pcBuffer, long &iBufOrRetLen); //执行特殊命令

	virtual long OpenPortEx(BYTE CheckMode, EPORT ePort, LPCTSTR lpDesc); //自动检测波特率后打开串口(下载模式、正常模式)
	virtual long Authentication(WORD wKeyId, EAUTHMODE eMode = USE_KEY);
	virtual long SetEntryMode(ENTRYMODE eMode, DWORD dwActiveKey = 0, DWORD dwActiveFDK = 0);	

	virtual long ConverErrorCode(BYTE cRet); //转换硬件的错误码
	virtual DWORD ConverKeyAttr(DWORD dwKeyAttr); //转换Key的属性
	virtual BYTE ConvertFormat(EPINFORMAT ePinFormat);//把Pin格式码转换成硬件对应的数值

	virtual long ECHO(void);
	virtual long SetHardwareSN(LPCSTR pcSn);            //设置硬件生产序列号	
	virtual long FWRandomKey(BYTE RandomMode, char *lpRandomData); //固件下载时获取随机密钥
	virtual long FWUpdateBlock(BYTE Mode, long nTotalBlock, HWND hWnd = 0, char* lpExData=NULL); //固件下载时更新Block
	virtual long SetHaredwareVer(LPCSTR pcHWVer);
	virtual long GetFirmwareLog(EFWLOG eMode, char *pOut, long &nOutLen);
	virtual long GetPower(long *pPower);
	virtual long FirmwareMAC(WORD wKeyId, WORD wIVId, char *lpOutMAC);

	virtual long LoadTextKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);//明文下载密钥
	virtual	long ExchangeKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, WORD wEnKey, WORD wVI = 0, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);//密文下载密钥
	virtual long Inner_StoreKey(WORD wKeyId, DWORD dwKeyAttr, EKEYMODE eMode, long iKeyLen = 16, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long Inner_VirtualBankKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long nLen, LPCSTR lpPSW, LPCSTR lpKeyboardCodes, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long VirtualBankKeyGo(WORD wKeyId, DWORD dwKeyAttr, LPCSTR cKeyTable, long nLen, LPCSTR cPSWTable, long nPSWLen, LPCSTR cZeros, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long VILoadKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, WORD wEnKey = KEY_INVALID, WORD wEnVI = 0, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long AllotEnlargeKeyId(WORD wAppId, DWORD dwKeyAttr, WORD &wAelId, WORD &wIndex);

	virtual long SM4_LoadTextKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);//明文下载SM4密钥
	virtual	long SM4_ExchangeKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, long iKeyLen, WORD wEnKey, WORD wVI = 0, EKEYMODE eMode = KEY_SET, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);//密文下载SM4密钥
	virtual long SM4_StoreKey(WORD wKeyId, DWORD dwKeyAttr, EKEYMODE eMode, long iKeyLen = 16, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);

	virtual long Inner_DeleteKey(WORD wKeyId);
	virtual long Inner_ReadKeyAttr(WORD wKeyId, LPDWORD lpKeyAttr, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0, WORD wVI = 0);
	virtual long Cmd_ReadKeyAttr(WORD wKeyId, LPDWORD lpKeyAttr, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	virtual long Cmd_ReadKeyAttr(WORD wKeyId, WORD wVI, LPDWORD lpKeyAttr, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0);
	
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

	virtual long Cmd_GetCOMKey(char *lpKeyValue, WORD &wOutLen);


protected:
	long   DownCharacteristic(LPCSTR pcFileName);
	DWORD  DSDealData2LB(char *pOut, LPCSTR pSrc, DWORD dwLen);
	long   DetectChipCmd(char *pChipType = NULL);
	long   DSLockChip(void);
	long   MAXQGetStatus(void);
	long   InitFlash(void); //如果是Flash则需要初始化
	long   InitMAXQ(void);
	long   RecoveryProgram(void);
	long   TR31ReadIAPVer(char *cpVer);
	long   TR31StartDownloading();
	long   A980Start(void);
	long   A980End(void);

	long  A980Download(LPCSTR pFileBuffer, DWORD dwFileLen, BYTE Mode, HWND hWnd=0);
	long  TR31Download(LPCSTR pFileBuffer, DWORD dwFileLen, BYTE Mode, HWND hWnd=0);
	long  SuperDownLoad(LPCSTR pFileBuffer, DWORD dwFileLen, BYTE Mode, HWND hWnd = 0); //用指令下载到STM再更新到EPP
	long  MAXQDownLoad(LPCSTR pFileBuffer, DWORD dwFileLen, HWND hWnd = 0);
	long  DSDownLoad(LPCSTR pFileBuffer, DWORD dwFileLen, BOOL bLBMode, BOOL bFlash, HWND hWnd = 0);
	long  DSHexDownLoad(LPCSTR pFileBuffer, DWORD dwFileLen, HWND hWnd = 0); //下载一行L(DS5002)
	long  DSBinDownLoad(LPCSTR pFileBuffer, DWORD dwFileLen, HWND hWnd = 0); //下载一行LB(DS5250)
	
	long  SSTDownLoad(LPCSTR pFileBuffer, DWORD dwFileLen, HWND hWnd = 0);
	long  STCDownLoad(LPCSTR pFileBuffer, DWORD dwFileLen, HWND hWnd = 0);
	long  MPCDownLoad(LPCSTR pFileBuffer, DWORD dwFileLen, HWND hWnd = 0);

	long   DetectSST(void);
	long   DetectSTC(void);
	long   DetectMPC(void);

	long   SSTEcho(void);
	long   STCEcho(void);
	long   MPCEcho(void);


	void   ConverPAN(LPCSTR lpCardNo, char *pOutPAN);
	long   PinFillFormat(EPINFORMAT ePinFormat, BYTE PinLen, BYTE Padding, LPCSTR lpCardNo, BYTE &PinOffset, BYTE &PinPostion, char *lpInit, char *lpCustomerData);


	DWORD  EncryptFireware(char *pOutBuf, LPCTSTR lpFileName, LPCSTR lpKey, WORD wKeyLen);

	
	long RSA_SigSN(char *pSNDer, WORD &wSNOutLen, char *pSig, WORD &wSigOutLen, bool bSigTag = false, LPCSTR SKOtherVendor = 0, WORD wSKLen = 0);
	long RSA_SigKey(LPCSTR pDer, WORD &wInOutLen, char *pSig, bool bRandom = false, LPCSTR SKOtherVendor = 0, WORD wSKLen = 0);
	long RSA_RandomData(DWORD &dwInOutLen, char *pRandom);
	long RSA_ExpEppSN(char *lpSNTag, WORD &wSNTLen, ESIG_ALGORITHM &eSig, WORD wSigId = _VENDOR_PK);
	long RSA_ExpEppPK(char *lpPKTag, WORD &wPKTLen, ESIG_ALGORITHM &eSig, WORD wKeyId = _EPP_CRY_PK, WORD wSigId = _VENDOR_PK);
	long RSA_ImpVndPK(LPCSTR lpTag, WORD wTagLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0); //import PKvnd
	long RSA_ImpEppSN(LPCSTR lpSNTag, WORD wSNTLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0); //import SN
	long RSA_ImpEppPK(LPCSTR lpTag, WORD wTagLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wPKId = _EPP_CRY_PK, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0);//import PK(cepp、sepp)
	long RSA_ImpEppSK(LPCSTR lpTag, WORD wTagLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wSKId = _EPP_CRY_SK, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0);//import SK(cepp、sepp)	
	long RSA_ImpHostPK(LPCSTR lpTag, WORD wTagLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wHostPK = 1, WORD wSigId = _VENDOR_PK, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0);//import HostPK
	long RSA_Encrypt(WORD wKeyId, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut);


protected:
	long  m_iUAKLen;
	long  m_iKBPKLen;
	char  m_UAK[32];
	char  m_KBPK[32];
};


#endif //CPINPADBASE_H_20120528

