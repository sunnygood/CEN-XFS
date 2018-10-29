/*****************************************************************************
* Copyright(C) 2015,  Corporation.
*
* File Name:   CENXFSPINEx.h
* Description: the XFS PIN extend description
*
* Author:      Liuguoxun
* Date:        2015-06-18        
*
*****************************************************************************/

#ifndef _CENXFSPINEX_H_20150618
#define _CENXFSPINEX_H_20150618

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)


/* values of WFSPINENCIO.wProtocol and WFSPINCAPS.fwENCIOProtocols */
//#define WFS_PIN_ENC_PROT_CH                     (0x0001) //define in XFSPIN.h
//#define WFS_PIN_ENC_PROT_GIECB                  (0x0002)
//#define WFS_PIN_ENC_PROT_LUX                    (0x0004)
//#define WFS_PIN_ENC_PROT_CHN                    (0x0008)
#define WFS_PIN_ENC_PROT_SZZT                     (0x8000) //0x03E7”√”⁄≤‚ ‘


/************************************************************************
 * Personal Identification Number Keypad (PIN) China Protocol definitions
 * CEN/XFS 3.30
 ************************************************************************/

/* values of PROTCHNIN.wCommand */

#define WFS_CMD_ENC_IO_CHN_DIGEST                               (0x0001)
#define WFS_CMD_ENC_IO_CHN_SET_SM2_PARAM                        (0x0002)
#define WFS_CMD_ENC_IO_CHN_IMPORT_SM2_PUBLIC_KEY                (0x0003)
#define WFS_CMD_ENC_IO_CHN_SIGN                                 (0x0004)
#define WFS_CMD_ENC_IO_CHN_VERIFY                               (0x0005)
#define WFS_CMD_ENC_IO_CHN_EXPORT_SM2_ISSUER_SIGNED_ITEM        (0x0006)
#define WFS_CMD_ENC_IO_CHN_GENERATE_SM2_KEY_PAIR                (0x0007)
#define WFS_CMD_ENC_IO_CHN_EXPORT_SM2_EPP_SIGNED_ITEM           (0x0008)
#define WFS_CMD_ENC_IO_CHN_IMPORT_SM2_SIGNED_SM4_KEY            (0x0009)

#define PROT_CHN_RESULT_OFFSET (0)

/* values of PROTCHNOUT.wResult */

#define PROT_CHN_SUCCESS                            (0)
#define PROT_CHN_ERR_INVALID_CMD                    (-(PROT_CHN_RESULT_OFFSET + 1))
#define PROT_CHN_ERR_INVALID_DATA                   (-(PROT_CHN_RESULT_OFFSET + 2))
#define PROT_CHN_ERR_INVALID_KEY                    (-(PROT_CHN_RESULT_OFFSET + 3))

/* va lues of  PROTCHNDIGESTOUTPUT.wResult, PROTCHNIMPORTSM2 PUBLICKEYOUT.wResult, 
PROTCHNEXPORTSM2ISSUERSIGNEDITEMOUT . wResult, PROTCHNEXPORTSM2EPPSIGNEDITEMOUT.wResult  
and PROTCHNIMPORTSM2SIGNEDSM4KEYOUTPUT.wResult  */ 
 
#define PROT_CHN_ERR_PIN_ACCESSDENIED               (-(PROT_CHN_RESULT_OFFSET + 4))

/* values of  PROTCHNIMPORT SM2 PUBLICKEYOUT.wResult, PROTCHNDIGESTOUT.wResult,  
PROTCHNEXPORTSM2ISSUERSIGNEDITEMOUT . wResult, PROTCHNEXPORTSM2EPPSIGNEDITEMOUT.wResult 
and PROTCHNIMPORTSM2SIGNEDSM4KEYOUTPUT.wResult  */ 
 
#define PROT_CHN_ERR_PIN_KEYNOTFOUND                (-(PROT_CHN_RESULT_OFFSET + 5))

/* values of  PROTCHNIMPORT SM2 PUBLICKEYOUT.wResult, PROTCHNDIGESTOUT.wResult and 
PROTCHNIMPORTSM2SIGNEDSM4KEYOUTPUT.wResult */ 
 
#define PROT_CHN_ERR_PIN_USEVIOLATION               (-(PROT_CHN_RESULT_OFFSET + 6))
#define PROT_CHN_ERR_PIN_INVALIDKEYLENGTH           (-(PROT_CHN_RESULT_OFFSET + 7))

/* additional values of PROTCHNIMPORTSM2 PUBLICKEYOUT.wResult  and 
PROTCHNIMPORTSM2SIGNEDSM4KEYOUTPUT.wResult */ 
 
#define PROT_CHN_ERR_PIN_DUPLICATEKEY               (-(PROT_CHN_RESULT_OFFSET + 8))
#define PROT_CHN_ERR_PIN_SIG_NOT_SUPP               (-(PROT_CHN_RESULT_OFFSET + 9))
#define PROT_CHN_ERR_PIN_SIGNATUREINVALID           (-(PROT_CHN_RESULT_OFFSET + 10))

/* additional values of  PROTCHNSIGNOUT.wResult and 
PROTCHNIMPORTSM2SIGNEDSM4KEYOUTPUT.wResult  */ 
 
#define PROT_CHN_ERR_PIN_MODENOTSUPPORTED           (-(PROT_CHN_RESULT_OFFSET + 11))
#define PROT_CHN_ERR_PIN_KEYNOVALUE                 (-(PROT_CHN_RESULT_OFFSET + 12))
#define PROT_CHN_ERR_PIN_NOCHIPTRANSACTIVE          (-(PROT_CHN_RESULT_OFFSET + 13))
#define PROT_CHN_ERR_PIN_ALGORITHMNOTSUPP           (-(PROT_CHN_RESULT_OFFSET + 14))

/* values of PROTCHNVERIFYOUT.wResult */

#define PROT_CHN_ERR_PIN_SIGNATUREERROR             (-(PROT_CHN_RESULT_OFFSET + 15))

/* values of PROTCHNEXPORTSM2ISSUERSIGNEDITEMOUT.wResult */

#define PROT_CHN_ERR_PIN_NOPRIVATEKEY               (-(PROT_CHN_RESULT_OFFSET + 16))

/* values of PROTCHNGENERATESM2KEYOUT.wResult */

#define PROT_CHN_ERR_PIN_INVALID_MOD_LEN            (-(PROT_CHN_RESULT_OFFSET + 17))
#define PROT_CHN_ERR_PIN_KEY_GENERATION_ERROR       (-(PROT_CHN_RESULT_OFFSET + 18))

/* values of PROTCHNEXPORTSM2EPPSIGNEDITEMOUT.wResult */

#define PROT_CHN_ERR_PIN_NOSM2KEYPAIR               (-(PROT_CHN_RESULT_OFFSET + 19))

/* values of PROTCHNIMPORTSM2SIGNEDSM4KEYOUTPUT.wResult */

#define PROT_CHN_ERR_PIN_NOKEYRAM                   (-(PROT_CHN_RESULT_OFFSET + 20))
#define PROT_CHN_ERR_PIN_RANDOMINVALID              (-(PROT_CHN_RESULT_OFFSET + 21))

/* values of PROTCHNDIGESTIN.wHashAlgorithm */

#define PROT_CHN_HASH_SM3_DIGEST                    (0x0001)

/* values for PROTCHNIMPORTSM2PUBLICKEYIN.dwUse */

#define PROT_CHN_USESM2PUBLIC                       (0x00000001)
#define PROT_CHN_USESM2PUBLICVERIFY                 (0x00000002)

/* values of  PROTCHNIMPORT SM2 PUBLICKEYIN. dwSM2SignatureAlgorithm , 
PROTCHNEXPORTSM2ISSUERSIGNEDITEMOUT . dwSM2SignatureAlgorithm ,  
PROTCHNEXPORTSM2EPPSIGNEDITEMIN.dwSignatureAlgorithm and 
PROTCHNIMPORTSM2SIGNEDSM4 KEY.dwSM2SignatureAlgorithm  */ 
 
#define PROT_CHN_PIN_SIGN_NA                        (0)
#define PROT_CHN_SIGN_SM2_GM_T_2012                 (0x00000001)

/* values for PROTCHNIMPORTSM2PUBLICKEYOUT.dwSM2KeyCheckMode */
#define PROT_CHN_SM2_KCV_NONE                       (0x00000001)
#define PROT_CHN_SM2_KCV_SM3                        (0x00000002)

/* values for PROTCHNEXPORTSM2ISSUERSIGNEDITEMIN.wExportItemType, 
PROTCHNEXPORTSM2EPPSIGNEDITEMIN.wExportItemType */ 
 
#define PROT_CHN_PIN_EXPORT_EPP_ID                  (0x0001)
#define PROT_CHN_PIN_EXPORT_PUBLIC_KEY              (0x0002)

/* values for PROTCHNGENERATESM2KEYOUT.dwUse */
#define PROT_CHN_USESM2PRIVATE                      (0x00000001)
#define PROT_CHN_USESM2PRIVATESIGN                  (0x00000002)

// Used to type-cast specific command to access common fields
typedef struct _prot_chn_in
{
    WORD                   wCommand;
} PROTCHNIN, *LPPROTCHNIN;

// Used to type-cast specific response to access common fields
typedef struct _prot_chn_out
{
    WORD                   wCommand;
    WORD                   wResult;
} PROTCHNOUT, *LPPROTCHNOUT;

typedef struct _prot_chn_digest_in
{
    WORD                   wCommand;
    WORD                   wHashAlgorithm;
    LPWFSXDATA             lpxDigestInput;
} PROTCHNDIGESTIN, *LPPROTCHNDIGESTIN;

typedef struct _prot_chn_digest_out
{
    WORD                   wCommand;
    WORD                   wResult;
    LPWFSXDATA             lpxDigestOutput;
} PROTCHNDIGESTOUT, *LPPROTCHNDIGESTOUT;

typedef struct _prot_chn_sm2_algorithm_param_in
{
    WORD                   wCommand;
    LPWFSXDATA             lpxP;
    LPWFSXDATA             lpxA;
    LPWFSXDATA             lpxB;
    LPWFSXDATA             lpxN;
    LPWFSXDATA             lpxXg;
    LPWFSXDATA             lpxYg;
} PROTCHNSM2ALGORITHMPARAMIN, *LPPROTCHNSM2ALGORITHMPARAMIN;

typedef struct _prot_chn_sm2_algorithm_param_out
{
    WORD                   wCommand;
    WORD                   wResult;
} PROTCHNSM2ALGORITHMPARAMOUT, *LPPROTCHNSM2ALGORITHMPARAMOUT;

typedef struct _prot_chn_import_sm2_public_key_in
{
    WORD                   wCommand;
    LPSTR                  lpsKey;
    LPWFSXDATA             lpxValue;
    DWORD                  dwUse;
    LPSTR                  lpsSigKey;
    DWORD                  dwSM2SignatureAlgorithm;
    LPWFSXDATA             lpxSignature;
} PROTCHNIMPORTSM2PUBLICKEYIN, *LPPROTCHNIMPORTSM2PUBLICKEYIN;

typedef struct _prot_chn_import_sm2_public_key_out
{
    WORD                   wCommand;
    WORD                   wResult;
    DWORD                  dwSM2KeyCheckMode;
    LPWFSXDATA             lpxKeyCheckValue;
} PROTCHNIMPORTSM2PUBLICKEYOUT, *LPPROTCHNIMPORTSM2PUBLICKEYOUT;

typedef struct _prot_chn_sign_in
{
    WORD                   wCommand;
    LPSTR                  lpsKey;
    LPSTR                  lpSignerID;
    LPWFSXDATA             lpxPlaintextData;
} PROTCHNSIGNIN, *LPPROTCHNSIGNIN;

typedef struct _prot_chn_sign_out
{
    WORD                   wCommand;
    WORD                   wResult;
    LPWFSXDATA             lpxSignData;
} PROTCHNSIGNOUT, *LPPROTCHNSIGNOUT;

typedef struct _prot_chn_verify_in
{
    WORD                   wCommand;
    LPSTR                  lpsKey;
    LPWFSXDATA             lpxPlaintextData;
    LPWFSXDATA             lpxSignData;
} PROTCHNVERIFYIN, *LPPROTCHNVERIFYIN;

typedef struct _prot_chn_verify_out
{
    WORD                   wCommand;
    WORD                   wResult;
 } PROTCHNVERIFYOUT, *LPPROTCHNVERIFYOUT;

typedef struct _prot_chn_export_sm2_issuer_signed_item_in
{
    WORD                   wCommand;
    WORD                   wExportItemType;
    LPSTR                  lpsName;
} PROTCHNEXPORTSM2ISSUERSIGNEDITEMIN, *LPPROTCHNEXPORTSM2ISSUERSIGNEDITEMIN;

typedef struct _prot_chn_export_sm2_issuer_signed_item_out
{
    WORD                   wCommand;
    WORD                   wResult;
    LPWFSXDATA             lpxValue;
    WORD                   dwSM2SignatureAlgorithm;
    LPWFSXDATA             lpxSignature;
 } PROTCHNEXPORTSM2ISSUERSIGNEDITEMOUT, *LPPROTCHNEXPORTSM2ISSUERSIGNEDITEMOUT;

typedef struct _prot_chn_generate_sm2_keypair_in
{
    WORD                   wCommand;
    LPSTR                  lpsKey;
    DWORD                  dwUse;
} PROTCHNGENERATESM2KEYPAIRIN, *LPPROTCHNGENERATESM2KEYPAIRIN;

typedef struct _prot_chn_generate_sm2_keypair_out
{
    WORD                   wCommand;
    WORD                   wResult;
} PROTCHNGENERATESM2KEYPAIROUT, *LPPROTCHNGENERATESM2KEYPAIROUT;

typedef struct _prot_chn_export_sm2_epp_signed_item_in
{
    WORD                   wCommand;
    WORD                   wExportItemType;
    LPSTR                  lpsName;
	LPSTR                  lpsSigKey;
    DWORD                  dwSignatureAlgorithm;
} PROTCHNEXPORTSM2EPPSIGNEDITEMIN, *LPPROTCHNEXPORTSM2EPPSIGNEDITEMIN;

typedef struct _prot_chn_export_sm2_epp_signed_item_out
{
    WORD                   wCommand;
    WORD                   wResult;
    LPWFSXDATA             lpxValue;
    WORD                   dwSM2SignatureAlgorithm;
    LPWFSXDATA             lpxSignature;
 } PROTCHNEXPORTSM2EPPSIGNEDITEMOUT, *LPPROTCHNEXPORTSM2EPPSIGNEDITEMOUT;

typedef struct _prot_chn_import_sm2_signed_sm4_key
{
	WORD                   wCommand;
    LPSTR                  lpsKey;
    LPSTR                  lpsDecryptKey;
    DWORD                  dwSM2EncipherAlgorithm;
    LPWFSXDATA             lpxValue;
    DWORD                  dwUse;
    LPSTR                  lpsSigKey;
    DWORD                  dwSM2SignatureAlgorithm;
    LPWFSXDATA             lpxSignature;
} PROTCHNIMPORTSM2SIGNEDSM4KEY, *LPPROTCHNIMPORTSM2SIGNEDSM4KEY;

typedef struct _prot_chn_import_sm2_signed_sm4_key_output
{
    WORD                    wCommand;
    WORD                    wResult;
    WORD                    wKeyCheckMode;
    LPWFSXDATA              lpxKeyCheckValue;
} PROTCHNIMPORTSM2SIGNEDSM4KEYOUTPUT, *LPPROTCHNIMPORTSM2SIGNEDSM4KEYOUTPUT;



/************************************************************************
 * extern interface which was defined by SZZT
 * 
 ************************************************************************/

/* values of WFS_PIN_ENC_PROT_SZZT's child command */
#define WFS_CMD_ENC_IO_GET_PASSWORD               (1)
#define WFS_PIN_ENC_IO_RSA_VENDORINIT             (2) //RSA_VendorInit
#define WFS_PIN_ENC_IO_RSA_IMPHOSTVER             (3) //RSA_ImpHostVersion
#define WFS_PIN_ENC_IO_RSA_EXPHOSTVER             (4) //RSA_ExpHostVersion
#define WFS_PIN_ENC_IO_RSA_ENCRYPTRC              (5) //RSA_EncryptRC
#define WFS_PIN_ENC_IO_AUTO_UPDATEPSW             (6) //AutoUpdatePassword
#define WFS_PIN_ENC_IO_AUTO_VIRTUALBK             (7) //VirtualBankKey
#define WFS_PIN_ENC_IO_AUTO_RI                    (8) //RI_AutoRemoveInstall
#define WFS_PIN_ENC_IO_GET_FWMAC                  (9) //FirmwareMAC

#define WFS_PIN_ENC_IO_CHN_SELFTEST              (10) //Selftest
#define WFS_PIN_ENC_IO_CHN_KEYEXCHANGE_B         (11) //SM2_KeyExchangeB
#define WFS_PIN_ENC_IO_CHN_KEYAGREEMENT_B        (12) //SM2_KeyAgreementB


/* Administrator */
#define WFS_PIN_OPERATOR_1                        (1)
#define WFS_PIN_OPERATOR_2                        (2)
#define WFS_PIN_OPERATOR_3                        (3)
#define WFS_PIN_OPERATOR_4                        (4)
#define WFS_PIN_OPERATOR_5                        (5)

#define WFS_PIN_PASSWORD_OLD                      (0)
#define WFS_PIN_PASSWORD_NEW                      (1)
#define WFS_PIN_PASSWORD_UPDATE                   (2)

#define WFS_PIN_AUTH_FORBID                       (0x30)
#define WFS_PIN_AUTH_REMOVE                       (0x31)
#define WFS_PIN_AUTH_INSTALL                      (0x32)


#define WFS_PIN_KEY_TABLE_SIZE                    (32)    //pinpad max key number(index from 0x0 to 0x1F)
#define WFS_PIN_KEY_BUFFER_SIZE                   (23 + 1)//key name buffer size


#define WFS_PIN_TEST_ALL                         (0x0000)
#define WFS_PIN_TEST_RANDOM                      (0x0001)
#define WFS_PIN_TEST_SM4                         (0x0002)
#define WFS_PIN_TEST_SM3                         (0x0004)
#define WFS_PIN_TEST_SM2_CRYPT                   (0x0008)
#define WFS_PIN_TEST_SM2_SIGN                    (0x0010)


/* administrator password entry in parameter struct*/
typedef struct _wfs_pin_get_password
{
    WORD                wCommand;
    WORD                wOperator;
    WORD                wPswType;
    USHORT              usMaxLen;
    BOOL                bAutoEnd;
    CHAR                cEcho;
    ULONG               ulActiveFDKs;
    ULONG               ulActiveKeys;
    ULONG               ulTerminateFDKs;
    ULONG               ulTerminateKeys;
} WFSPINGETPASSWORD, * LPWFSPINGETPASSWORD;

/* vendor rsa key pair initlize in parameter struct*/
typedef struct _wfs_pin_rsa_vendor_init
{
    WORD                wCommand;
    BOOL                bSoftGen;
    ULONG               ulBits;
    USHORT              usPkLength;
    USHORT              usSkLength;
    BYTE                lpPkData[512];
    BYTE                lpSkData[2048];
} WFSPINRSAVENDORINIT, * LPWFSPINRSAVENDORINIT;

/* rsa import host version in parameter struct*/
typedef struct _wfs_pin_import_host_version
{
    WORD                wCommand;
    USHORT              usDerLength;
    BYTE                lpDer[1024];
    USHORT              usSigLength;
    BYTE                lpSig[1024];
    BYTE                bRSASignatureAlgorithm;
    CHAR                lpsSigKey[WFS_PIN_KEY_BUFFER_SIZE];
} WFSPINIMPORTHOSTVERSION, * LPWFSPINIMPORTHOSTVERSION;

/* rsa export host version in parameter struct*/
typedef struct _wfs_pin_exp_host_version_in
{
    WORD                wCommand;
} WFSPINEXPORTHOSTVERSIONIN, * LPWFSPINEXPORTHOSTVERSIONIN;

/* rsa export host version out parameter struct*/
typedef struct _wfs_pin_exp_host_version_out
{
    WORD                wCommand;
    BYTE                bRSASignatureAlgorithm;
    USHORT              usTagLength;
    BYTE                lpTag[2048];    
} WFSPINEXPORTHOSTVERSIONOUT, * LPWFSPINEXPORTHOSTVERSIONOUT;

/* rc encrypt in parameter struct*/
typedef struct _wfs_pin_encrypt_rc_in
{
    WORD                wCommand;
    BYTE                bRSASignatureAlgorithm;
    BYTE                bRcLength;
    BYTE                lpRc[32];
    CHAR                lpsHostKey[WFS_PIN_KEY_BUFFER_SIZE];
    CHAR                lpsSigKey[WFS_PIN_KEY_BUFFER_SIZE];
} WFSPINENCRYPTRCIN, * LPWFSPINENCRYPTRCIN;

/* rc encrypt in parameter struct*/
typedef struct _wfs_pin_encrypt_rc_out
{
    WORD                wCommand;
    USHORT              usTagLength;
    BYTE                lpTag[2048];
} WFSPINENCRYPTRCOUT, * LPWFSPINENCRYPTRCOUT;

/* auto update administrator password in parameter struct*/
typedef struct _wfs_pin_auto_update_password
{
    WORD                wCommand;
    WORD                wOperator;
    BYTE                lpOldPsw[16+1]; //end by 0x00 for tailed byte
    BYTE                lpNewPsw[16+1]; //end by 0x00 for tailed byte
} WFSPINAUTOUPDATEPASSWORD, * LPWFSPINAUTOUPDATEPASSWORD;

/* virtual bank key in parameter struct*/
typedef struct _wfs_pin_virtual_bank_key_in
{
    WORD                wCommand;
    CHAR                lpsKey[WFS_PIN_KEY_BUFFER_SIZE];
    DWORD               dwUse;
    USHORT              usKeyLength;
    BYTE                lpKeyValue[24];
    BYTE                lpPsw[48+1]; // maybe 16 or (16 * 2) or (16 * 3), end by 0x00 for tailed byte
} WFSPINVIRTUALBANKKEYIN, * LPWFSPINVIRTUALBANKKEYIN;

/* virtual bank key out parameter struct*/
typedef struct _wfs_pin_virtual_bank_key_out
{
    WORD                wCommand;
    USHORT              usKcvLength;
    BYTE                lpKcv[32];
} WFSPINVIRTUALBANKKEYOUT, * LPWFSPINVIRTUALBANKKEYOUT;

/* auto remove install in parameter struct*/
typedef struct _wfs_pin_auto_remove_install
{
    WORD                wCommand;
    WORD                wMode;
    BYTE                lpPsw1[16+1]; //end by 0x00 for tailed byte
    BYTE                lpPsw2[16+1]; //end by 0x00 for tailed byte
} WFSPINAUTOREMOVEINSTALL, * LPWFSPINAUTOREMOVEINSTALL;

/* get firmware mac in parameter struct*/
typedef struct _wfs_pin_firmware_mac_in
{
    WORD                wCommand;
} WFSPINFIRMWAREMACIN, * LPWFSPINFIRMWAREMACIN;

/* get firmware mac out parameter struct*/
typedef struct _wfs_pin_firmware_mac_out
{
    WORD                wCommand;
    USHORT              usMacLength;
    BYTE                lpMac[64];
} WFSPINFIRMWAREMACOUT, * LPWFSPINFIRMWAREMACOUT;


/* SelfTest in parameter struct*/
typedef struct _wfs_pin_selftest_in
{
    WORD                wCommand;
    DWORD               dwComponent;
} WFSPINSELFTESTIN, * LPWFSPINSELFTESTIN;


/* SM2_KeyExchangeB in parameter struct*/
typedef struct _wfs_pin_sm2_keyexchange_b_in
{
    WORD                wCommand;
	WORD                wZaLen;
    BYTE                lpZa[32];
    WORD                wPaLen;
    BYTE                lpPa[64];
    WORD                wRaLen;
    BYTE                lpRa[64];
	WORD                wASigLen;
    BYTE                lpASig[64];
	CHAR                lpsSigKey[WFS_PIN_KEY_BUFFER_SIZE];
} WFSPINSM2KEYEXBIN, *LPWFSPINSM2KEYEXBIN;

/* SM2_KeyExchangeB out parameter struct*/
typedef struct _wfs_pin_sm2_keyexchange_b_out
{
    WORD                wCommand;
	WORD                wZbLen;
    BYTE                lpZb[32];
    WORD                wRbLen;
    BYTE                lpRb[64];
    WORD                wSbLen;
    BYTE                lpSb[32];
	WORD                wBSigLen;
    BYTE                lpBSig[64];
} WFSPINSM2KEYEXBOUT, *LPWFSPINSM2KEYEXBOUT;

/* SM2_KeyAgreementB in parameter struct*/
typedef struct _wfs_pin_sm2_keyagreement_b_in
{
    WORD                wCommand;
    CHAR                lpsKey[WFS_PIN_KEY_BUFFER_SIZE];
	DWORD               dwUse;
	WORD                wSaLen;
    BYTE                lpSa[32];
} WFSPINSM2KEYAGREEMENTBIN, *LPWFSPINSM2KEYAGREEMENTBIN;


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif //_CENXFSPINEX_H_20150618
