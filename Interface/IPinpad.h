/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IPinpad.h
* Description: the Pinpad standard interface       
*
* Author:      liuguoxun
* Date:        2012-05-29
* Ver:         
*
*****************************************************************************/


#ifndef  _IPINPAD_H_20120529
#define  _IPINPAD_H_20120529


#define PIN_MODULE_VER      PLATFORM_VER


#include "IDriver.h"


#define   KEY_INVALID        0xFFFF    //invalid or no used(key id)
#define   KEY_DELETE_DES       9000    //delete all DES
#define   KEY_DELETE_RSA        600    //delete all RSA
#define   KEY_DELETE_ALL        999    //delete all
#define	  KEY_DELETE_KEYPSW     900    //delete admin1,admin2,admin3's password
#define	  KEY_DELETE_SM4        800    //delete all SM4 
#define	  KEY_DELETE_SM2        620    //delete all SM2
#define   KEY_DELETE_SM2_SM4   1600    //delete SM2 and SM4

#define   GUIDLIGHTS_SIZE        32    //pinpad guid light's size
#define   MAX_KEY_TABLE          32    //pinpad max key number(index from 0x0 to 0x1F)
#define   MAX_EMV_SCHEMES         7    //max EMVImportSchemes


//RSA
#define   _VENDOR_PK            600    //PKvnd                        _SigIssuerVendor(XFS)
#define   _RSA_EPP_ID           601    //EPP ID with signature
#define   _EPP_CRY_SK           602    //SKcepp(crypt)                _EPPCryptKey(XFS)
#define   _EPP_CRY_PK           603    //PKcepp                       _EPPCryptKey(XFS)
#define   _EPP_SIG_SK           604    //SKsepp(signature)            _EPPSignKey(XFS)
#define   _EPP_SIG_PK           605    //PKsepp                       _EPPSignKey(XFS)
#define   _HOST_PK              606    //Host PK(maybe from bank)

//SM2
#define   _VENDOR_SM2_PK        620    //PKvnd                        _SM2Vendor(XFS)
#define   _EPP_SM2_SIG_SK       621    //EPP SK for Signature         _SM2SignKey(XFS)
#define	  _EPP_SM2_SIG_PK       622    //EPP PK for verfying sig      _SM2SignKey(XFS)
#define   _EPP_SM2_EXC_SK       623    //EPP SK for key agreement     _SM2ExcKey(XFS)
#define	  _EPP_SM2_EXC_PK       624    //EPP PK for key agreement     _SM2ExcKey(XFS)
#define   _EPP_SM2_CRYPT_SK     625    //EPP SK for decryption        _SM2CryptKey(XFS)
#define	  _EPP_SM2_CRYPT_PK     626    //EPP PK for encryption        _SM2CryptKey(XFS)
#define   _HOST_SM2_EXC_PK      627    //Host PK for key agreement
#define   _HOST_SM2_SIG_PK      628    //Host PK for verfying sig
#define   _HOST_SM2_SIG_PK_DLL  629    //The default public key used for this DLL, can be used for auto-RemoveInstall
#define   _HOST_SM2_PK          630    //SM2 Host PK(maybe from bank)



//Download FW's rate message,WPARAM is rate(100%); when LPARAM==0 is download message, LPARAM!=0 is update block message
#define   WM_RATE_DOWNFW       (WM_USER + 0x398) 


/********************************************************************************/



//Command type
typedef enum {
	  ePIN_UNKNOWN   =      0x0L   //unknown type(DLL will try to detect pinpad's type,when finish do that you cann't change other command pinpad)
	, ePIN_SIMULATE  =    0x100L   //Simulate Pinpad

	, ePIN_EPP       =  0x10000L   //EPP command type(Bxx Cxx Exx Vxx)
	, ePIN_VISA      =  0x20000L   //VISA command type(Dxx)
	, ePIN_PCI       =  0x40000L   //PCI command type(Hxx)
	, ePIN_WOSA      =  0x80000L   //WOSA command type(Fxx)

	, ePIN_PCI_3X    = ePIN_PCI  + 0x1  //PCI_3X/4X
	, ePIN_VISA_3X   = ePIN_VISA + 0x2  //VISA_3X
	, ePIN_EPP_BR    = ePIN_EPP  + 0x4  //EPP_3X
	, ePIN_WOSA_3X   = ePIN_WOSA + 0x8  //WOSA_3X

	, eECC_VISA      = ePIN_VISA + 0x80  //ECC-ZT128A
	, eECC_PCI       = ePIN_PCI  + 0x80  //ECC-ZT128B/ECC-ZT130
} EPIN_TYPE;


//Pinpad extend
typedef enum {
	  eEX_NONE             =  0x00L  //No extend
	, eEX_STRING_NAME      =  0x01L  //Key name is string, set this to use XFS_XXX function
	, eEX_ENLARGE_KEY      =  0x02L  //Auto enlarge key usage or key attribute	
	, eEX_PC_KB            =  0x04L  //use as PC keyboard

	, eEX_SAVE2EPP         =  0x08L  //mapped table save to EPP(it can't use singleness and some EPP unsupport)

	, eEX_1_8              =  eEX_STRING_NAME | eEX_SAVE2EPP
	, eEX_2_8              =  eEX_ENLARGE_KEY | eEX_SAVE2EPP
	, eEX_4_8              =  eEX_PC_KB       | eEX_SAVE2EPP

	, eEX_1_2              =  eEX_STRING_NAME | eEX_ENLARGE_KEY
	, eEX_1_4              =  eEX_STRING_NAME | eEX_PC_KB	
	, eEX_1_2_8            =  eEX_STRING_NAME | eEX_ENLARGE_KEY | eEX_SAVE2EPP
	, eEX_1_4_8            =  eEX_STRING_NAME | eEX_PC_KB | eEX_SAVE2EPP
} EPIN_EXTEND;


typedef enum {
	  FORMAT_FREE        = (0x0000)      //free arithmetic defined by user
	, FORMAT_IBM3624     = (0x0001)      //IBM3624
	, FORMAT_ANSI        = (0x0002)      //ANSI X9.8
	, FORMAT_ISO0        = (0x0004)      //ISO9564 0
	, FORMAT_ISO1        = (0x0008)      //ISO9564 1
	, FORMAT_ECI2        = (0x0010)      //ECI2
	, FORMAT_ECI3        = (0x0020)      //ECI3
	, FORMAT_VISA        = (0x0040)      //VISA
	, FORMAT_DIEBOLD     = (0x0080)      //DIEBOLD
	, FORMAT_DIEBOLDCO   = (0x0100)      //DIEBOLDCO
	, FORMAT_VISA3       = (0x0200)      //VISA3
	, FORMAT_BANKSYS     = (0x0400)      //Bank system
	, FORMAT_EMV         = (0x0800)      //EMV
	, FORMAT_ISO3        = (0x2000)      //ISO9564 3	
	, FORMAT_AP          = (0x4000)      //Authentication Parameter PIN block
} EPINFORMAT;


typedef enum { 
	  VA_DES             = (0x0001)      //DES Validation Algorithms
	, VA_EUROCHEQUE      = (0x0002)      //EUROCHEQUE Validation Algorithms
	, VA_VISA            = (0x0004)      //VISA	Validation Algorithms
	, VA_DES_OFFSET      = (0x0008)      //DES OFFSET Validation Algorithms
	, VA_BANKSYS         = (0x0010)      //BANKSYS Validation Algorithms
} EVALIDATION;


typedef enum { 
	  SIGN_NA              =  (0x0000)  //Not Signature
	, SIGN_RSASSA_V15      =  (0x0001)  //RSASSA_PKCS1_V1_5
	, SIGN_RSASSA_PSS      =  (0x0002)  //RSASSA_PSS
	, SIGN_SM2_GM_T_2012   =  (0x0004)  //SM2
} ESIG_ALGORITHM;


typedef enum { 
	  ATTR_SPECIAL     =     (0x0000)  //special key(UID | UAK | KBPK | IMK)

	, ATTR_DK          =     (0x0001)  //DATA KEY(WFS_PIN_USECRYPT)
	, ATTR_PK          =     (0x0002)  //PIN KEY(WFS_PIN_USEFUNCTION)
	, ATTR_AK          =     (0x0004)  //MAC KEY(WFS_PIN_USEMACING)
	, ATTR_MK          =     (0x0020)  //MASTER KEY / MK only for MK(WFS_PIN_USEKEYENCKEY)
	, ATTR_IV          =     (0x0080)  //IV KEY(WFS_PIN_USESVENCKEY)

    , ATTR_NODUPLICATE =     (0x0040)  //WFS_PIN_NODUPLICATE
	, ATTR_ANSTR31     =     (0x0400)  //ANSTR31 MASTER KEY(WFS_PIN_USEANSTR31MASTER)
	, ATTR_RESTRICTED  =     (0x0800)  //Restricted MASTER KEY(WFS_PIN_USERESTRICTEDKEYENCKEY)
	, ATTR_SM4         =     (0x8000)  //China Secure Encryption Algorithm(SM4)

	, ATTR_PINLOCAL    =    (0x10000)  //PIN local offset(WFS_PIN_USEPINLOCAL)
	, ATTR_RSAPUBLIC   =    (0x20000)  //RSA public(WFS_PIN_USERSAPUBLIC)
	, ATTR_RSAPRIVATE  =    (0x40000)  //RSA private(WFS_PIN_USERSAPRIVATE)
	, ATTR_CHIPINFO    =   (0x100000)  //WFS_PIN_USECHIPINFO
	, ATTR_CHIPPIN     =   (0x200000)  //WFS_PIN_USECHIPPIN
	, ATTR_CHIPPS      =   (0x400000)  //WFS_PIN_USECHIPPS
	, ATTR_CHIPMAC     =   (0x800000)  //WFS_PIN_USECHIPMAC
	, ATTR_CHIPLT      =  (0x1000000)  //WFS_PIN_USECHIPLT
	, ATTR_CHIPMACLZ   =  (0x2000000)  //WFS_PIN_USECHIPMACLZ
	, ATTR_CHIPMACAZ   =  (0x4000000)  //WFS_PIN_USECHIPMACAZ
	, ATTR_RSA_VERIFY  =  (0x8000000)  //RSA public verify(WFS_PIN_USERSAPUBLICVERIFY)
	, ATTR_RSA_SIGN    = (0x10000000)  //RSA private sign(WFS_PIN_USERSAPRIVATESIGN)
	, ATTR_PINREMOTE   = (0x20000000)  //PIN remote(WFS_PIN_USEPINREMOTE)
	, ATTR_SM2         = (0x80000000)  //China Secure Encryption Algorithm(SM2)

	, ATTR_MPK         = (ATTR_MK | ATTR_PK)  //MASTER KEY only for PIN KEY
	, ATTR_MDK         = (ATTR_MK | ATTR_DK)  //MASTER KEY only for DATA KEY
	, ATTR_MAK         = (ATTR_MK | ATTR_AK)  //MASTER KEY only for MAC KEY
	, ATTR_MIV         = (ATTR_MK | ATTR_IV)  //MASTER KEY only for IV KEY

	, ATTR_WK          = (ATTR_PK | ATTR_DK | ATTR_AK)
	, ATTR_MWK         = (ATTR_MK | ATTR_PK | ATTR_DK | ATTR_AK)
} EKEYATTR;


typedef enum { 
	  KEY_SET              =  0x30  // It's equivalent to "combine" at some pinpad
	, KEY_XOR              =  0x31
	, KEY_XOR2             =  0x32
	, KEY_XOR3             =  0x33
} EKEYMODE;


typedef enum { 
	  KCVNONE              =  0x0000 //no KCV
	, KCVSELF              =  0x0001 //key encrypt itself(first 8 char)
	, KCVZERO              =  0x0002 //key encrypt 00000000
} EKCVMODE;


typedef enum { 
	  ENTRY_MODE_CLOSE     =  0x0
	, ENTRY_MODE_TEXT      =  0x1 //GetData
	, ENTRY_MODE_PIN       =  0x2 //GetPIN
	, ENTRY_MODE_KEY       =  0x3 //Secure key
} ENTRYMODE;


typedef enum { 
	  SOUND_CLOSE          =  0x0
	, SOUND_OPEN           =  0x1
	, SOUND_KEEP           =  0x2
} ESOUND;


typedef enum { 
	  AUTH_FORBID          =  0x30      //forbid(disable) RemoveInstallAuth, don't use this RemoveInstallAuth
	, AUTH_REMOVE          =  0x31      //remove RemoveInstallAuth 
	, AUTH_INSTALL         =  0x32      //install RemoveInstallAuth
} EINSTALL_AUTH;


typedef enum { 
	  PSW_ID1              =  0x1       //Admin 1 password(for Secure key)
	, PSW_ID2              =  0x2       //Admin 2 password
	, PSW_ID3              =  0x3       //Admin 3 password

	, PSW_ID4              =  0x4       //Admin 4 password(for RemoveInstallAuth)
	, PSW_ID5              =  0x5       //Admin 5 password(for RemoveInstallAuth)
} EPSW_ID;


typedef enum { 
	  PSW_OLD              =  0x30      // use the PIN buffer
	, PSW_NEW              =  0x31      // use the KEY buffer
} EPSW;


typedef enum {
	  EMV_PLAIN_CA         =  0x0001
	, EMV_CHKSUM_CA        =  0x0002
	, EMV_EPI_CA           =  0x0003
	, EMV_ISSUER           =  0x0004
	, EMV_ICC              =  0x0005
	, EMV_ICC_PIN          =  0x0006
	, EMV_PKCSV1_5_CA      =  0x0007
} EEMV;


typedef enum { 
	  eRAND_SOFT           =  0x30 //software
	, eRAND_EPP            =  0x31 //EPP or ECC hardware random
	, eRAND_RSA            =  0x32 //RSA
	, eRAND_SM             =  0x33 //China Secure Encryption Algorithm
	, eRAND_RI             =  0x34 //use by RI_RemoveInstallBySig
} ERANDOM;


typedef enum { 
	  eCAP_RSA_HASHALG     =  0x0001 //_pin_caps.dwRSAHashAlg, see the EHASH
	, eCAP_KCVL            =  0x0002 //_pin_caps.ucKCVL
	, eCAP_UKO             =  0x0003 //_pin_caps.wUserKeyOffset, don't change it unless you need to compatibility some project
	, eCAP_SPLIT_CBS_MAC   =  0x0004 //_pin_caps.bSplitBankSysMAC, default is TRUE
	, eCAP_RSA_SIG_ALG     =  0x0005 //_pin_caps.dwRSASignatureAlgorithm, see the ESIG_ALGORITHM

	, eCAP_DPASPA          =  0x1000 //Control DPA/SPA, 0 is Disable, else is enable
	, eCAP_CMDSEQ          =  0x2000 //Command sequence add one every time, 0 is FALSE, else is TRUE, default is FALSE
	, eCAP_MAPPINGPATH     =  0x3000 //The path of enlarge key mapping table, maybe only path or include file name(*.dat) 
} ECAPS;


typedef enum { 
	  eTEST_ALL            =  0x0000 //Selftest all

	, eSM_RANDOM           =  0x0001 //Selftest SM random
	, eSM_SM4              =  0x0002 //Selftest SM4
	, eSM_SM3              =  0x0004 //Selftest SM3
	, eSM_SM2_CRYPT        =  0x0008 //Selftest SM2 crypt
	, eSM_SM2_SIGN         =  0x0010 //Selftest SM2 sign(generate and verify)
} ESMTEST;  // for long SelfTest(long iComponent);


/********************************************************************************/

#define  MAX_KEY_NAME         23L //max length of string key name


#pragma pack(push, 1)

typedef struct _pin_caps
{
    WORD                   wClass;
    WORD                   fwType;
    BOOL                   bCompound;
    WORD                   usKeyNum;
    WORD                   fwAlgorithms;
    WORD                   fwPinFormats;
    WORD                   fwDerivationAlgorithms;
    WORD                   fwPresentationAlgorithms;
    WORD                   fwDisplay;
    BOOL                   bIDConnect;
    WORD                   fwIDKey;
    WORD                   fwValidationAlgorithms;
    WORD                   fwKeyCheckModes;
    LPSTR                  lpszExtra;
    DWORD                  dwGuidLights[GUIDLIGHTS_SIZE];
    BOOL                   bPINCanPersistAfterUse;
    WORD                   fwAutoBeep;
    LPSTR                  lpsHSMVendor;
    BOOL                   bHSMJournaling;
    DWORD                  dwRSAAuthenticationScheme;
    DWORD                  dwRSASignatureAlgorithm;
    DWORD                  dwRSACryptAlgorithm;
    DWORD                  dwRSAKeyCheckMode;
    DWORD                  dwSignatureScheme;
    LPWORD                 lpwEMVImportSchemes;
    WORD                   fwEMVHashAlgorithm;
    BOOL                   bKeyImportThroughParts;
    WORD                   fwENCIOProtocols;
    BOOL                   bTypeCombined;
    BOOL                   bSetPinblockDataRequired;
    WORD                   fwKeyBlockImportFormats;
    BOOL                   bPowerSaveControl;
    BOOL                   bAntiFraudModule;
    WORD                   wDESKeyLength;
    WORD                   wCertificateTypes;
    void                   *lppLoadCertOptions;
    DWORD                  dwCRKLLoadOptions;
    void                   *lpETSCaps;
    LPDWORD                lpdwSynchronizableCommands;
    void                   *lppRestrictedKeyEncKeySupport;


    DWORD                  dwCapsSize;         //size of struct PINCAPS
    DWORD                  dwAlgorithms;       //match with ECRYPT, low 16 bits is fwAlgorithms	
    DWORD                  dwRSAHashAlg;       //RSA Hash Algorithm, same with dwRSAKeyCheckMode
    BYTE                   ucKCVL;             //Key Check Value Length
    EPIN_TYPE              ePinpadType;        //Pinpad command type
    WORD                   wSubType;           //Pinpad subtype
    WORD                   wUserKeyOffset;     //Base index of user enlarge key
    BOOL                   bSplitBankSysMAC;   //Split MAC of china Bank System(the last step), default is TRUE;
    DWORD                  dwRIMode;           //RI's mode, password or signature	
    DWORD                  dwReserve1;         //Reserve for later
    DWORD                  dwReserve2;         //Reserve for later
} PINCAPS, *LPPINCAPS;


typedef struct _pin_key_detail
{
    WORD   wKeyId;
    WORD   wKeyLen;
    DWORD  dwKeyAttr;
    BYTE   bGeneration;
    BYTE   bVersion;
    BYTE   bActivatingDate[4];
    BYTE   bExpiryDate[4];
    BOOL   bLoaded;
    char   szKeyBlockHeader[34];
    char   szKeyName[MAX_KEY_NAME + 1];	
}PINKEYDETAIL, *LPPINKEYDETAIL;

#pragma pack(pop)


/********************************************************************************/

//   interface in/out parameter£º
//   1¡¢bin character(0x00 ~ 0xFF)
//   2¡¢viewable character string(isn't split character, such as ID,SN,version)

// the base interface for pinpad 
INTERFACE_I  IPinpad : public IDriver
{
public:
	//EPP base operate
	virtual long SetCaps(ECAPS eCapsSwitch, DWORD dwValue, LPVOID lpReserve = 0) = 0;
	virtual long SetControlMode(BYTE byControlMode, WORD ControlCode = 0xFFFF) = 0;
	virtual long GetRandom(ERANDOM eRandMode, DWORD &dwInOutLen, char *pRandom) = 0;
	virtual long GetTerminalNO(char *pTNO) = 0;
	virtual long SetTerminalNO(LPCSTR pTNO) = 0;
	virtual long SetKeyboardCode(LPCSTR lpKeyboardCodes, long nLen, ENTRYMODE eMode = ENTRY_MODE_TEXT) = 0;
	virtual long GetKeyboardCode(char *lpKeyboardCodes, long &nOutLen, ENTRYMODE eMode = ENTRY_MODE_TEXT) = 0;
	virtual long SetCustomerInfo(LPCSTR pInfo, DWORD dwLen, int iBlockNo = 0) = 0;
	virtual long GetCustomerInfo(char *pInfo, DWORD &dwLen, int iBlockNo = 0) = 0;
	virtual long EPPSetTime(time_t tTime = 0) = 0;
	virtual long EPPGetTime(time_t &tTime, char *pFormatTime = 0) = 0;
	virtual void LogConfig(int nLevel = 0x40, const char *szLogDir = NULL, int nSaveMode = -2, int nExtend = 0) = 0;
	
	
	//Input
	virtual long OpenKeyboardAndSound(ESOUND eSound, ENTRYMODE eMode, DWORD dwActiveKey = 0, DWORD dwActiveFDK = 0) = 0;
	virtual long StartPinInput(ESOUND eSound, BYTE MaxLen = 6, BYTE MinLen = 4, bool bAutoEnd = true, BYTE TimeOut = 30) = 0;
	virtual long StartKeyInput(ESOUND eSound, BYTE KeyLen, bool bAutoEnd = true, BYTE BeginPos = 0) = 0; //(KeyLen: 16/32/48)
	virtual long StartPswInput(ESOUND eSound, EPSW_ID ePswId, EPSW ePsw, BYTE PswLen = 16, bool bAutoEnd = true) = 0;
	virtual long UpdatePassword(EPSW_ID ePswId) = 0;
	virtual long AutoUpdatePassword(EPSW_ID ePswId, LPCSTR lpOldPSW, LPCSTR lpNewPSW, LPCSTR lpKeyboardCodes = 0) = 0;
	virtual WORD ParseKeyTable(LPCSTR lpNeedPress, LPCSTR lpKeyboardCodes, char *lpKeyTable, long nBufSize = 48) = 0;
	virtual long VirtualPress(LPCSTR lpKeyTable, BYTE cNumbers) = 0;	
	virtual long ReadText(char *lpText, DWORD &dwOutLen, DWORD dwTimeOut) = 0;
	virtual long DelEntryBuffer(BOOL bClearAll = FALSE) = 0;
	
	
	//Download key
	virtual	long LoadKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD wKeyLen, WORD wEnKey = KEY_INVALID, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO, EKEYMODE eMode = KEY_SET) = 0;//clear/cipher
	virtual long LoadKey_SAM(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD wKeyLen, WORD wEnKey, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO) = 0; //use PSAM card to download work key
	virtual long StoreKey(WORD wKeyId, DWORD dwKeyAttr, EKEYMODE eMode, WORD wKeyLen = 16, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO) = 0;//store key which is pressed on pinpad(16/32/48)	
	virtual long VirtualBankKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpPSW, LPCSTR lpKeyboardCodes = 0, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO) = 0;
	virtual long ReadKeyAttribute(WORD wKeyId, LPDWORD lpKeyAttr, char *lpKCVRet = 0, EKCVMODE eKCV = KCVZERO) = 0;
	virtual long DeleteKey(WORD wKeyId) = 0;
	

	//Crypt
	virtual long GetPinBlock(WORD wKeyId, BYTE PinLen, EPINFORMAT ePinFormat, LPCSTR lpCardNo, char *pPinBlock, WORD &wOutLen, BYTE Padding = 0x0F, WORD wKeyId2 = KEY_INVALID) = 0;
	virtual long Crypt(WORD wKeyId, ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, bool bEncrypt = true, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID, BYTE Padding = 0) = 0; 
	virtual long CalcMAC(WORD wKeyId, EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut, LPCSTR lpIVdata = 0, WORD wIVid = KEY_INVALID) = 0;
	virtual long GetPinBlock_F(char *pPinBlock) = 0; //old interface, don't use it
	virtual long StartPinInput_F(WORD wKeyId, BYTE PinLen, EPINFORMAT ePinFormat, LPCSTR lpCardNo, bool bAutoEnd = true, BYTE TimeOut = 30, WORD wKeyId2 = KEY_INVALID) = 0;//old interface, don't use it


	//Pin Offset(Local)
	virtual long PO_LocalVISA(  WORD wKeyId, LPCSTR lpKeyEnKey, WORD wEnKeyLen, LPCSTR lpPAN, BYTE B_EMode, char *lpInOutPVV, BYTE MaxPIN = 4, BYTE POLen = 0) = 0;
	virtual long PO_LocalIBM(   WORD wKeyId, LPCSTR lpKeyEnKey, WORD wEnKeyLen, LPCSTR lpPVD, BYTE B_EMode, char *lpInOutPVV, LPCSTR lpDecTable, BYTE MaxPIN, BYTE POLen = 0) = 0;
	virtual long PO_LocalEurope(WORD wKeyId, LPCSTR lpKeyEnKey, WORD wEnKeyLen, LPCSTR lpEUD, BYTE B_EMode, char *lpInOutPVV, LPCSTR lpDecTable, BYTE MaxPIN, BYTE FirstEncLen = 0, BYTE POFirstEnc = 0, BYTE POLen = 0) = 0;


	//Certificate
	virtual long CA_Load(WORD wCAId, DWORD dwKeyAttr, WORD wSigId, LPCSTR lpCA, WORD wCALen, char *lpOutData, LPWORD OutDataLen) = 0;
	virtual long CA_Replace(WORD wCAId, DWORD dwKeyAttr, WORD wSigId, LPCSTR lpCA, WORD wCALen, char *lpOutData, LPWORD OutDataLen) = 0;
	virtual long CA_Get(WORD wCAId, char *lpOutData, LPWORD OutDataLen) = 0;
	virtual long CA_ImportPKCS7Key(WORD wKeyId, DWORD dwKeyAttr, WORD wCAId, LPCSTR lpPKCS7, WORD &wInOutLen, char *lpOutData, LPWORD lpKeyLen) = 0;


	//RSA
	virtual long RSA_VendorInit(bool bSoftGenKeyPair = true, long nBits = 2048, LPCSTR SKOtherVendor = 0, WORD wSKLen = 0, LPCSTR PKOtherVendor = 0, WORD wPKLen = 0) = 0;
	virtual long RSA_GenKeyPair(char *pSKDer, WORD &wSKOutLen, char *pPKDer, WORD &wPKOutLen, long nBits = 2048) = 0; //software generate
	virtual long RSA_GenKeyPair(BYTE ucEPPKeyPairType = 0x31) = 0; //EPP generate, only some EPP support
	virtual long RSA_SigDes(LPCSTR pEnData, WORD &wInOutLen, LPCSTR lpSigKeyDer, WORD wSLen, char *pSig, bool bSigTag = false, LPCSTR lpRandom = 0, WORD wRandomLen = 0) = 0;	
	virtual long RSA_Export(WORD wKeyId, LPSTR lpDer, WORD &wDerLen, LPSTR lpSig, WORD &wSigLen, ESIG_ALGORITHM &eSig, WORD wSigId = _VENDOR_PK) = 0;	
	virtual long RSA_ImpRSAKey(WORD wKeyId, LPCSTR lpDer, WORD wDerLen, LPCSTR lpSig, WORD wSigLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wSigId = _VENDOR_PK, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0) = 0;	
	virtual long RSA_ImpDESKey(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpDer, WORD wDerLen, LPCSTR lpSig, WORD wSigLen, bool bRandom = true, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, 
		              WORD wHostPK = _HOST_PK, WORD wDecryptId = _EPP_CRY_SK, EKCVMODE eKCV = KCVZERO, char *lpKCVRet = 0, WORD *lpDesKeyLen = 0) = 0;

	virtual long RSA_ImpHostVersion(LPCSTR lpDer, WORD wDerLen, LPCSTR lpSig, WORD wSigLen, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wSigId = _VENDOR_PK) = 0;
	virtual long RSA_ExpHostVersion(char *lpTag, WORD &wTLen, ESIG_ALGORITHM &eSig) = 0;
	virtual long RSA_EncryptRC(LPCSTR lpRC, WORD &wInOutLen, char *lpTag, ESIG_ALGORITHM eSig = SIGN_RSASSA_V15, WORD wEnHostId = _HOST_PK, WORD wSigId = _EPP_SIG_SK) = 0;


	//China Secure Encryption Algorithm
	virtual long SM_GetInfo(char *pInfo) = 0;
	virtual long SM3_Algorithm(LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut) = 0;
	virtual long SM2_VendorInit(bool bSoftGenKeyPair = true, LPCSTR lpIDa = 0, WORD wIDLen = 0, LPCSTR SKOtherVendor = 0, WORD wSKLen = 0, LPCSTR PKOtherVendor = 0, WORD wPKLen = 0) = 0;
	virtual long SM2_GenKeyPair(WORD wSKID, WORD wPKID) = 0;
	virtual long SM2_GenKeyPair(char *lpSK, WORD &wSKLen, char *lpPK, WORD &wPKLen) = 0;
	virtual long SM2_CalcZa(WORD wKeyId, LPCSTR lpIDa, WORD &wInOutLen, char *pZa, WORD wIDType = 0) = 0;
	virtual long SM2_ExportKey(WORD wKeyId, char *lpPK, WORD &wPKLen, ESIG_ALGORITHM &eSig, char *lpZa, WORD &wZaLen, char *lpSig, WORD &wSigLen, WORD wSigId = _VENDOR_SM2_PK) = 0;
	virtual long SM2_ImportKey(WORD wKeyId, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpZa, WORD wZaLen, LPCSTR lpSig, WORD wSigLen,
						ESIG_ALGORITHM eSig = SIGN_SM2_GM_T_2012, WORD wSigId = _VENDOR_SM2_PK, LPDWORD pKCVMode = 0, char *lpKCVRet = 0, LPDWORD pKCVL = 0) = 0;
	virtual long SM2_ImportSM4Key(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpEncKey, WORD wEKeyLen, WORD wHostPK, LPCSTR lpZa, WORD wZaLen, 
						LPCSTR lpSig, WORD wSigLen, char *lpKCVRet = 0, LPDWORD pKCVL = 0, bool bRandom = false, WORD *lpSM4KeyLen = 0, 
							EKCVMODE eKCV = KCVZERO, WORD wDecryptId = _EPP_SM2_CRYPT_SK, ESIG_ALGORITHM eSig = SIGN_SM2_GM_T_2012) = 0;

	virtual long SM2_KeyExchangeA(char *lpZa, WORD &wZaLen, char *lpPa, WORD &wPaLen, char *lpRa, WORD &wRaLen, char *lpASig, WORD &wASigLen) = 0;
	virtual long SM2_KeyExchangeB(LPCSTR lpZa, WORD wZaLen, LPCSTR lpPa, WORD wPaLen, LPCSTR lpRa, WORD wRaLen, LPCSTR lpASig, WORD wASigLen, 
								char *lpZb, WORD &wZbLen, char *lpRb, WORD &wRbLen, char *lpSb, WORD &wSbLen, char *lpBSig, WORD &wBSigLen, WORD wSigPKa = _HOST_SM2_SIG_PK) = 0;
	virtual long SM2_KeyAgreementA(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpZb, WORD wZbLen, LPCSTR lpRb, WORD wRbLen, LPCSTR lpSb, WORD wSbLen, LPCSTR lpBSig, WORD wBSigLen, 
		            char *lpSa, WORD &wSaLen, char *lpKCVRet, WORD &KCVLen, WORD wExcPKb = _HOST_SM2_EXC_PK, WORD wSigPKb = _HOST_SM2_SIG_PK, EKCVMODE eKCV = KCVZERO, WORD wKeyLen = 16) = 0;
	virtual long SM2_KeyAgreementB(WORD wKeyId, DWORD dwKeyAttr, LPCSTR lpSa, WORD wSaLen, char *lpKCVRet, WORD &wKCVLen, EKCVMODE eKCV = KCVZERO) = 0;


	//EMV
	virtual long EMV_ImportPK(WORD wKeyId, DWORD dwKeyAttr, WORD wSigId, EEMV eScheme, LPCSTR lpDataIn, WORD wInLen, char *lpOut, LPWORD lpOutLen) = 0;


	//IC
	virtual long IC_PowerOn(char *cpExValue, long &nOutLen) = 0;
	virtual long IC_PowerOff(void) = 0;
	virtual long IC_Control(LPCSTR lpValue, long &nInOutLen, char *cpExValue) = 0;
	virtual long IC_SetParameter(BYTE iIC, WORD wICType) = 0;
	virtual long IC_ReadParameter(BYTE iIC, WORD &wICType) = 0;


	//RemoveInstall
	virtual long RI_GetStatus(long &nStatus) = 0;
	virtual long RI_RemoveInstallAuth(EINSTALL_AUTH eMode) = 0;
	virtual long RI_AutoRemoveInstall(EINSTALL_AUTH eMode, LPCSTR lpPSW1, LPCSTR lpPSW2, LPCSTR lpKeyboardCodes = 0) = 0;
	virtual long RI_RemoveInstallBySig(EINSTALL_AUTH eMode, LPCSTR lpTag, DWORD dwTagLen, WORD wKeyID = _HOST_SM2_SIG_PK_DLL) = 0;


	//LCD
	virtual long LCD_ShowInfo(LPCSTR lpInfo, BYTE Len, BYTE Mode = 0, BYTE Pos = 0) = 0;


	//Big Keyboard(PC Keyboard)
	virtual long BKG_GetVersion(char *pcHVersion)  = 0;


	//ECC
	virtual long ECC_CryptComData(WORD wDataKeyId, WORD wMacKeyId, BYTE bReturnMode, LPCSTR lpIV, WORD wRealSCmdLen, LPCSTR lpSend, WORD wSLen, LPCSTR lpMac,
								   WORD &wRealRCmdLen, char *lpReceive, WORD &wRLen, char *lpRetMac) = 0;
	virtual long ECC_CryptComData_S(WORD wDataKeyId, WORD wMacKeyId, BYTE bReturnMode, LPCSTR lpIV, WORD wRealSCmdLen, LPCSTR lpSend, WORD wSLen, LPCSTR lpMac) = 0;
	virtual long ECC_CryptComData_R(WORD wTimeout, WORD &wRealRCmdLen, char *lpReceive, WORD &wRLen, char *lpRetMac) = 0;


	//Key name is string, such as CEN/XFS(WOSA)
	virtual WORD XFS_AllotKeyId(LPCSTR lpKeyName) = 0;
	virtual WORD XFS_GetKeyId(LPCSTR lpKeyName) const = 0;
	virtual WORD XFS_GetKeyCount(void) const = 0;
	virtual BOOL XFS_HadWorkKey(void) const = 0;
	virtual long XFS_GetKeyDetail(WORD wKeyId, LPPINKEYDETAIL lpDetail) const = 0;
	virtual long XFS_GetKeyDetail(LPCSTR lpKeyName, LPPINKEYDETAIL lpDetail) const = 0;


	//Raw interface, for PCI, SM or other test
	virtual long RAW_GenKeys(ECRYPT eMode, char *lpSK, WORD &wSKLen, char *lpPK, WORD &wPKLen) = 0;	

	virtual long RAW_Hash(   EHASH eHashAlg, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut) = 0;
	virtual long RAW_Crypt(    ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut, bool bEncrypt = true) = 0;
	virtual long RAW_MAC(         EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut) = 0;
	virtual long RAW_GenSig(   ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut) = 0;
	virtual long RAW_VerifySig(ECRYPT eMode, LPCSTR lpDataIn, DWORD dwInLen,     LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, LPCSTR lpSigIn, WORD wSigLen) = 0;


	//software interface
	virtual long Soft_Hash(   EHASH eHashAlg, LPCSTR lpDataIn, DWORD &dwInOutLen, char *lpOut) = 0;
	virtual long Soft_Crypt(    ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut, bool bEncrypt = true) = 0;
	virtual long Soft_MAC(         EMAC eMac, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut) = 0;
	virtual long Soft_GenSig(   ECRYPT eMode, LPCSTR lpDataIn, DWORD &dwInOutLen, LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, char *lpOut) = 0;
	virtual long Soft_VerifySig(ECRYPT eMode, LPCSTR lpDataIn, DWORD dwInLen,     LPCSTR lpKey, WORD wKeyLen, LPCSTR lpEx, WORD wExLen, LPCSTR lpSigIn, WORD wSigLen) = 0;

	virtual DWORD Soft_Hex2Bin(char *pBin, DWORD dwBufLen, LPCSTR pHex, DWORD dwLen) = 0; //2 visible hex char to 1 binary char
	virtual DWORD Soft_Bin2Hex(char *pHex, DWORD dwBufLen, LPCSTR pBin, DWORD dwLen) = 0; //1 binary char to 2 visible hex char

	virtual DWORD Soft_SM3Calc(LPCSTR lpData, DWORD dwDataLen, char *lpHash) = 0;
	virtual long  Soft_CalcZa(LPCSTR lpPK, WORD wPKLen, LPCSTR lpIDa, WORD &wInOutLen, char *pZa, WORD wIDType = 0) = 0;
	virtual long  Soft_SM2ExchangeA(LPCSTR Za, WORD wZaLen, LPCSTR Zb, WORD wZbLen, LPCSTR PKa, WORD wPKaLen, LPCSTR PKb, WORD wPKbLen, 
		               LPCSTR RandomPKa, WORD wRandomPKaLen, LPCSTR RandomPKb, WORD wRandomPKbLen, LPCSTR SKa, WORD wSKaLen, LPCSTR RandomSKa, WORD wRandomSKaLen, 
					   char *Sa, WORD &wSaLen, char *S1, WORD &wS1Len, char *TargetKey, WORD wKeyLen = 16) = 0;
	virtual long  Soft_SM2ExchangeB(LPCSTR Za, WORD wZaLen, LPCSTR Zb, WORD wZbLen, LPCSTR PKa, WORD wPKaLen, LPCSTR PKb, WORD wPKbLen, 
		               LPCSTR RandomPKa, WORD wRandomPKaLen, LPCSTR RandomPKb, WORD wRandomPKbLen, LPCSTR SKb, WORD wSKbLen, LPCSTR RandomSKb, WORD wRandomSKbLen, 
					   char *Sb, WORD &wSbLen, char *S2, WORD &wS2Len, char *TargetKey, WORD wKeyLen = 16) = 0;
};



//Instance a pinpad object
//[IN] ePort: see enum EPORT
//[IN] eType: when is ePIN_UNKNOWN, you must input right port description(for auto detect EPP);otherwise ignore lpDesc
//[IN] lpDesc: see EPORT remark, such as "COM1:9600,N,8,1" "VID_23AB&PID_1003"
//[IN] eExtend: see enum EPIN_EXTEND
extern "C" IPinpad * WINAPI InstancePinpad(EPORT ePort, EPIN_TYPE eType, LPCTSTR lpDesc = 0, EPIN_EXTEND eExtend = eEX_NONE);


//delete a pinpad instance
extern "C" void WINAPI ReleasePinpad(IPinpad *p);


typedef IPinpad *(WINAPI * PINPAD_INSFUNC)(EPORT, EPIN_TYPE, LPCTSTR, EPIN_EXTEND);
typedef void (WINAPI * PINPAD_DELFUNC)(IPinpad *);


#endif  //_IPINPAD_H_20120529

