/*****************************************************************************
* Copyright(C) 1993-2012,SZZT Corporation.
*
* File Name:   IPinpadEx.h
* Description: the extend interface for IPinpad.h
*
* Author:      liuguoxun
* Date:        2012-05-28
* History 1: 
*
*****************************************************************************/

#ifndef IPINPADEX_H_20120528
#define IPINPADEX_H_20120528


//Hardware type(combination flag)
typedef enum {
	  eHW_EPP       =  0x0001 //Electronic PIN Pad(WFS_PIN_TYPEEPP), it is PC keyboard
	, eHW_EDM       =  0x0002 //Encryption Decryption Module(WFS_PIN_TYPEEDM)
	, eHW_HSM       =  0x0004 //Hardware Security Module(WFS_PIN_TYPEHSM)  eHW_EPP and eHW_EDM within the same physical unit 
	, eHW_ETS       =  0x0008 //Encrypting Touch Screen(WFS_PIN_TYPEETS)
} EHWTYPE;


//chip type
typedef enum {
	  eSTM       =  0x30
	, eSTM2MAXQ  =  0x31
	, eRECOVERY  =  0x32

	, eSST       =  0x56
	, eSTC       =  0x57
	, eMPC       =  0x58
	, eMAXQ1103  =  0x59
	, eDS500x    =  0x60 //Dallas download hex firmware, L
	, eDS5250    =  0x61 //Dallas download bin firmware, LB
	, eDS500x_F  =  0x62 //Dallas download hex firmware, L  + Flash(Stop Use)
	, eDS5250_F  =  0x63 //Dallas download bin firmware, LB + Flash(Stop Use)

	, eA980      =  0x69
	, eMAX3255x  =  0x70	
} ECHIP;


//Authenticate EPP mode
typedef enum { 
	  PASSWORD             =  0x0       //password
	, LOAD_KEY             =  0x31      //Authenticate for download key
	, USE_KEY              =  0x32      //Authenticate for use key(unlock)
} EAUTHMODE;


//RI mode
typedef enum { 
	  RIM_NONE             =  0x0      //None
	, RIM_PSW              =  0x1      //RI use password
	, RIM_SIGN             =  0x2      //RI use signature
} ERIMODE;


//RI state
typedef enum { 
	  RIS_INSTALLED        =  0x1      //Logic Installed
	, RIS_REMOVED          =  0x2      //Logic Removed
	, RIS_ILLEGAL_RM       =  0x4      //Illegal Removed

	, RIS_PHYSICAL_INS     =  0x10     //Physical Installed	
	, RIS_PHYSICAL_RM      =  0x20     //Physical Removed	
} ERISTATE;


typedef enum {
	  eFW_SelfDestruction_Clear  =  0x2A
	, eFW_SelfDestruction        =  0x30 //Self destruction

	, eFW_Battery                =  0x32  

	, eFW_Crystal                =  0x35 //Crystal
	, eFW_Crystal_Clear          =  0x36 //Clear Crystal
} EFWLOG;


/*****************************************************************************/

#define    INX_EXTEND_BASE                       300


/*****************************************************************************/

//Sub version
#define  SUBVER_NONE             0x0000
#define  SUBVER_EPP_WITHOUT_03   0x0001
#define  SUBVER_EPP_WITH_03      0x0002
#define  SUBVER_WOSA_596E        0x0010
#define  SUBVER_WOSA_588D599K    0x0020
#define  SUBVER_WOSA_598J        0x0040
#define  SUBVER_WOSA_588F        0x0080
#define  SUBVER_HXX              0x1000 //PCI1.x PCI2.x
#define  SUBVER_HXX_3X           0x2000 //PCI3.x
#define  SUBVER_HXX_598N         0x8000 //OKI ZT598N


//Customer data index define
#define  INDEX_BUYER             0L //buyer info, such as OKI, NCR
#define  INDEX_TERMINALNO        1L //terminal NO.
#define  INDEX_SAVE2EPP_BEGIN    2L //mapped table save to EPP begin index


#define  KEYID_TEMP_M       0x249 //inner temp key
#define  KEYID_TEMP         0x250 //inner temp key, for LocalIBM and so on
#define  KEYID_IV           0x251 //inner temp IV Key

#define  VENDOR_PK_LEN       270L //RSA
#define  VENDOR_SK_LEN      1192L

#define  MIN_SM4_KEY        1000L
#define  MAX_SM4_KEY        1599L

#define  SM2_PK_LEN           64L
#define  SM2_SK_LEN           32L
#define  SM2_SIGN_LEN         64L



#define  IS_SM4(wKeyId, dwAttr)  (dwAttr & ATTR_SM4 || (wKeyId >= MIN_SM4_KEY && wKeyId <= MAX_SM4_KEY))


// id = 0 ~ 15 or 606 ~ 619
#define  CHECK_HOST_PK(id)  \
			if((id >= 16 && id < _HOST_PK) || id > 619) \
			{ \
				return PIN_INVALID_KEYID; \
			}

// id = 0 ~ 19 or 627 ~ 647
#define  CHECK_HOST_PK_SM2(id)  \
			if((id >= 20 && id < _HOST_SM2_EXC_PK) || id > 647) \
			{ \
				return PIN_INVALID_KEYID; \
			}

#define  OFFSET_BASE_LEFT(id, base)  \
			if(id != KEY_INVALID && id >= base) \
			{ \
				id -= base; \
			} 

#define  OFFSET_BASE_RIGHT(id, base)  \
			if(id != KEY_INVALID && id < base) \
			{ \
				id = id + base; \
			}


/*****************************************************************************/


#endif //IPINPADEX_H_20120528

