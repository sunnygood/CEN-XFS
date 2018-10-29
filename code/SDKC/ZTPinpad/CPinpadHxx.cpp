/*****************************************************************************
* Copyright(C) 1993-2015,XXX Corporation.
*
* File Name:   CPinpadHxx.cpp
* Description: the class of Hxx, for PCI3.x and PCI4.x
*
* Author:      liuguoxun
* Date:        2015-09-22
* History 1: 
*
*****************************************************************************/


#include "CPinpadHxx.h"


#define   KEY_UAK                 0    //UAK
#define   KEY_KBPK				888    //KBPK



/*****************************************************************************
 * 函数说明: 
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
CPinpadHxx::CPinpadHxx(EPORT ePort, EExchange eType, EPIN_EXTEND eExtend)
	: CPinpadBase(ePort, eType, eExtend)
{
	m_stCap.dwRSAKeyCheckMode = HASH_SHA256;
	m_stCap.dwRSAHashAlg = HASH_SHA256;
	m_stCap.fwEMVHashAlgorithm = HASH_SHA256;
	m_stCap.ucKCVL = 0x03;
}


/*****************************************************************************
 * 函数说明: 
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
CPinpadHxx::~CPinpadHxx()
{
}


/*****************************************************************************
 * 函数说明: 注册自己的特性
 * 参数定义: 
 * 返 回 值: 
 *****************************************************************************/
long CPinpadHxx::RegisterMyFeatures(BOOL bAll)
{
	if(NULL == m_pPort)
	{
		return (EC_POINTER_NULL);
	}

	long iRet = 0;
	if(bAll)
	{
		m_iKBPKLen = 24;
		m_iUAKLen = 24;	
		strcpy(m_KBPK, KEYVALUE_KBPK);
		strcpy(m_UAK,  KEYVALUE_UAK);	
	
	
		// 索引 | 命令 | 参数长度 | 超时 | 最大执行次数 | 报文格式  | 命令描述 
		iRet = RegisterCommand(INX_ECHO_REPLY,                  0x16,      0,   200,  1,  F_NORMAL, "Replace ECHO");
		iRet = RegisterCommand(INX_INIT,                        0x15,      2,  3500,  1,  F_NORMAL, "INIT");
		iRet = RegisterCommand(INX_RESET,                       0x15,      2,  3500,  1,  F_NORMAL, "RESET");
		iRet = RegisterCommand(INX_GET_DEVICE_INFO,             0x16,      0,   500,  1,  F_NORMAL, "GET_DEVICE_INFO");
		iRet = RegisterCommand(INX_VIRTUAL_KEY_PRESS,           0x12, 0xFFFF,   500,  1,  F_NORMAL, "VIRTUAL_KEY_PRESS");
		iRet = RegisterCommand(INX_RANDOM_DATA,                 0x24,      1,  1000,  1,  F_NORMAL, "RANDOM_DATA");
		iRet = RegisterCommand(INX_GET_POWER,			        0xE1,      0,   500,  1,  F_NORMAL, "GET_POWER New Hxx");
		iRet = RegisterCommand(INX_SET_HARDWARE_SN,             0x00,     10,  1000,  1,  F_NORMAL, "SET_HARDWARE_SN");
		iRet = RegisterCommand(INX_LOADER_VERSION,              0x4C,      0,  1000,  1,  F_NORMAL, "READ_LOADER_VER");

		iRet = RegisterCommand(INX_SET_BAUDRATE,                0x15,      2,   500,  1,  F_NORMAL, "SET_BAUDRATE");
		iRet = RegisterCommand(INX_SET_CONTROL_MODE,            0x15,      2,   500,  1,  F_NORMAL, "SET_CONTROL_MODE");
		iRet = RegisterCommand(INX_SET_ENTRY_MODE,              0x10,      5,   500,  1,  F_NORMAL, "SET_ENTRY_MODE");
		iRet = RegisterCommand(INX_SET_KEYBOARD_CODES,          0x14,     65,  2000,  1,  F_NORMAL, "SET_KEYBOARD_CODES");
		iRet = RegisterCommand(INX_GET_KEYBOARD_CODES,          0xE4,      1,  2000,  1,  F_NORMAL, "GET_KEYBOARD_CODES");	
		iRet = RegisterCommand(INX_WRITE_CUSTOMER_INFO,         0x17,    257,  3000,  2,  F_NORMAL, "WRITE_CUSTOMER_INFO");
		iRet = RegisterCommand(INX_READ_CUSTOMER_INFO,          0x18,      1,  2000,  2,  F_NORMAL, "READ_CUSTOMER_INFO");	
		iRet = RegisterCommand(INX_READ_TEXT,                   0x11,      0,  1000,  1,  F_NORMAL, "READ_ENTRY_TEXT");
		iRet = RegisterCommand(INX_READ_PIN_BLOCK,              0x41,     36,  1500,  1,  F_NORMAL, "READ_PIN_BLOCK");
		iRet = RegisterCommand(INX_DEL_ENTRY_BUFFER,            0x13, 0xFFFF,   500,  1,  F_NORMAL, "DEL_ENTRY_BUFFER");

		iRet = RegisterCommand(INX_STORE_KEY,                   0x21, 0xFFFF,  2000,  1,  FS_ADD_MAC_KBPK, "STORE_KEY");
		iRet = RegisterCommand(INX_LOAD_KEY_TEXT,               0x22, 0xFFFF,  2000,  1,  FS_ADD_MAC_KBPK, "LOAD_KEY_TEXT");	 
		iRet = RegisterCommand(INX_LOAD_KEY_CRYPT,              0x23, 0xFFFF,  2000,  1,  FS_ADD_MAC_KBPK, "EXCHANGE_KEY"); 
		iRet = RegisterCommand(INX_DELETE_KEY,                  0x27,      2,  1500,  1,  F_NORMAL, "DELETE_KEY");
		iRet = RegisterCommand(INX_READ_KEY_ATTRIBUTES,         0x28, 0xFFFF,   500,  1,  F_NORMAL, "READ_KEY_ATTRIBUTES");


		iRet = RegisterCommand(INX_DES_ENCRYPT,                 0x46, 0xFFFF,  5000,  1,  F_NORMAL, "CRYPT"); //PCI加解密为同一条命令
		iRet = RegisterCommand(INX_MAC_CALC,                    0x48, 0xFFFF,  5000,  1,  F_NORMAL, "MAC");
		iRet = RegisterCommand(INX_DES_RAW_ENCRYPT,             0x44, 0xFFFF,  5000,  1,  F_NORMAL, "DES_RAW_ENCRYPT");
		iRet = RegisterCommand(INX_MAC_CRYPT_RAW,               0x4C, 0xFFFF,  5000,  1,  F_NORMAL, "MAC_RAW_CRYPT");

		iRet = RegisterCommand(INX_AUTHENTICATION,              0x25, 19 + ADD_MAC_LEN,  1000,  1,  FS_ADD_MAC_UAK, "AUTHENTICATION"); 
		iRet = RegisterCommand(INX_FIRMWARE_AUTH,               0x26,      6,  1500,  1,  F_NORMAL, "FIRMWARE_AUTH");
		iRet = RegisterCommand(INX_UPDATE_PASSWORD,             0x20,      3,  1000,  1,  F_NORMAL, "UPDATE_PASSWORD");
		iRet = RegisterCommand(INX_REMOVE_INSTALL_AUTH,         0x19,      1,  1000,  1,  F_NORMAL, "REMOVE_INSTALL");
		iRet = RegisterCommand(INX_ESCAPE_RI_AUTH,              0x0F,      1,  1000,  1,  F_NORMAL, "ESCAPE REMOVE_INSTALL AUTHENTICATION");

		iRet = RegisterCommand(INX_LOCAL_IBM_PIN_OFFSET,        0x42, 0xFFFF,  3000,  1,  F_NORMAL, "LOCAL_IBM_PIN_OFFSET");
		iRet = RegisterCommand(INX_LOCAL_VISA_PIN_OFFSET,       0x43, 0xFFFF,  3000,  1,  F_NORMAL, "LOCAL_VISA_PIN_OFFSET");
		iRet = RegisterCommand(INX_LOAD_CERTIFICATION,          0x38, 0xFFFF,  5000,  1,  F_NORMAL, "LOAD_CERTIFICATION");
		iRet = RegisterCommand(INX_REPLACE_CERTIFICATION,       0x3A, 0xFFFF,  5000,  1,  F_NORMAL, "REPLACE_CERTIFICATION");
		iRet = RegisterCommand(INX_GET_CERTIFICATION,           0x39,      2,  5000,  1,  F_NORMAL, "GET_CERTIFICATION");
		iRet = RegisterCommand(INX_EMV_IMPORT_PK,               0x51, 0xFFFF,  5000,  1,  F_NORMAL, "EMV_IMPORT_PUBLIC_KEY");

		iRet = RegisterCommand(INX_RSA_RAW_ENCRYPT,             0x45, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_RAW_ENCRYPT");
		iRet = RegisterCommand(INX_RSA_ENCRYPT,                 0x47, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_ENCRYPT");
		iRet = RegisterCommand(INX_RSA_RANDOM_DATA,             0x36,      8,  1500,  1,  F_NORMAL, "RSA_RANDOM_DATA");
		iRet = RegisterCommand(INX_RSA_IMPORT_SERIAL_NUMBER,    0x31, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_IMPORT_SERIAL_NUMBER");
		iRet = RegisterCommand(INX_RSA_EXPORT_SERIAL_NUMBER,    0x32,      4,  3000,  1,  F_NORMAL, "RSA_EXPORT_SERIAL_NUMBER");
		iRet = RegisterCommand(INX_RSA_IMPORT_DES_KEY,          0x34, 0xFFFF, 10000,  1,  F_NORMAL, "RSA_IMPORT_DES_KEY");
		iRet = RegisterCommand(INX_RSA_EXPORT_DES_KEY,          0x35, 0xFFFF,  3000,  1,  F_NORMAL, "RSA_EXPORT_DES_KEY");
		iRet = RegisterCommand(INX_RSA_EXPORT_EPP_PUBLIC_KEY,   0x33,      4,  3000,  1,  F_NORMAL, "RSA_EXPORT_EPP_PUBLIC_KEY");
		iRet = RegisterCommand(INX_RSA_IMPORT_VndPK,            0x30, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_IMPORT_VndPK");
		iRet = RegisterCommand(INX_RSA_IMPORT_EppSK,            0x30, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_IMPORT_EppSK");
		iRet = RegisterCommand(INX_RSA_IMPORT_EppPK,            0x30, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_IMPORT_EppPK");
		iRet = RegisterCommand(INX_RSA_IMPORT_HostPK,           0x30, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_IMPORT_HostPK");
		iRet = RegisterCommand(INX_RSA_IMPORT_RootHostPK,       0x30, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_IMPORT_RootHostPK");
		iRet = RegisterCommand(INX_RSA_SIG_RAW,                 0x49, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_RAW_SIG");
		iRet = RegisterCommand(INX_RSA_SIG_CHECK_RAW,           0x4A, 0xFFFF,  8000,  1,  F_NORMAL, "RSA_RAW_SIG_CHECK");
		iRet = RegisterCommand(INX_RSA_GEN_KEY_PAIR,            0xEA,     10, 60000,  1,  F_NORMAL, "RSA_GEN_KEY_PAIR");

		iRet = RegisterCommand(INX_HASH_RAW,                    0x4B,  0xFFFF, 8000,  1,  F_NORMAL, "HASH_RAW");
		iRet = RegisterCommand(INX_DPASPA,                      0x64,  0xFFFF, 1000,  1,  F_NORMAL, "DPASPA_CONFIG");


		iRet = RegisterCommand(INX_IC_SET_PARAMETER,            0xB0,      3,  1500,  1,  F_NORMAL, "IC_SET_PARAMETER");
		iRet = RegisterCommand(INX_IC_READ_PARAMETER,           0xB1,      1,  1500,  1,  F_NORMAL, "IC_READ_PARAMETER"); 
		iRet = RegisterCommand(INX_IC_POWER_ON,                 0xB2,      0,  3000,  1,  F_NORMAL, "IC_POWER_ON"); 
		iRet = RegisterCommand(INX_IC_POWER_OFF,                0xB3,      0,  1500,  1,  F_NORMAL, "IC_POWER_OFF");
		iRet = RegisterCommand(INX_IC_Control,                  0xB4, 0xFFFF,  3000,  1,  F_NORMAL, "IC_Control");

		//iRet = RegisterCommand(INX_DOWN_RANDOM_FWKEY,           0x49,      1,  2000,  1,  F_NORMAL, "DOWN_RANDOM_FWKEY"); 
		//iRet = RegisterCommand(INX_DOWN_FIRMWARE,               0x4A,    259, 10000,  2,  F_NORMAL, "DOWN_FIRMWARE");
		//iRet = RegisterCommand(INX_DOWN_UPDATE_FM,              0x4A,      3, 30000,  1,  F_NORMAL, "UPDATE_FM");
		//iRet = RegisterCommand(INX_DOWN_RECOVERY_FM,            0x4A,      1,120000,  1,  F_NORMAL, "RECOVERY_FM");

		iRet = RegisterCommand(INX_SET_RTC,                     0xE5,      4,  1000,  1,  F_NORMAL, "SET_RTC");
		iRet = RegisterCommand(INX_GET_RTC,                     0xE6,      0,  1000,  1,  F_NORMAL, "GET_RTC");
		iRet = RegisterCommand(INX_CLEAR_RTC,			        0xE7,      0,  1000,  1,  F_NORMAL, "CLEAR_RTC");

		iRet = RegisterCommand(INX_SM3_ALGORITHM,               0x9A,  0xFFFF, 5000,  1,  F_NORMAL, "SM3_ALGORITHM");
		iRet = RegisterCommand(INX_SM4_LOAD_TEXT_KEY,           0x9B,  0xFFFF, 5000,  1,  F_NORMAL, "SM4_LOAD_TEXT_KEY");
		iRet = RegisterCommand(INX_SM4_EXCHANGE_KEY,            0x9C,  0xFFFF, 5000,  1,  F_NORMAL, "SM4_EXCHANGE_KEY");
		iRet = RegisterCommand(INX_SM4_CRYPT,		            0x9D,  0xFFFF, 5000,  1,  F_NORMAL, "SM4_CRYPT");
		iRet = RegisterCommand(INX_SM4_MAC,					    0x9E,  0xFFFF, 5000,  1,  F_NORMAL, "SM4_MAC");
		iRet = RegisterCommand(INX_SM4_GET_PIN_BLOCK,           0x9F,	   56, 5000,  1,  F_NORMAL, "SM4_GET_PIN_BLOCK");
		iRet = RegisterCommand(INX_SM_READ_KEY_ATTRIBUTE,       0xA3,  0xFFFF, 5000,  1,  F_NORMAL, "SM_READ_KEY_ATTRIBUTE");
		iRet = RegisterCommand(INX_SM_DELETE_KEY,               0xA1,	    2, 5000,  1,  F_NORMAL, "SM_DELETE_KEY");
		iRet = RegisterCommand(INX_SM_GET_VERSION,              0xA2,		0, 5000,  1,  F_NORMAL, "SM_GET_VERSION");
		iRet = RegisterCommand(INX_SM2_GENERATE_KEY_PAIR,		0x99,		4, 5000,  1,  F_NORMAL, "SM2_GENERATE_KEY_PAIR");
		iRet = RegisterCommand(INX_SM2_EXPORT_KEY,				0x98,		4, 5000,  1,  F_NORMAL, "SM2_EXPORT_KEY");
		iRet = RegisterCommand(INX_SM2_IMPORT_KEY,				0x91,  0xFFFF, 5000,  1,  F_NORMAL, "SM2_IMPORT_KEY");
		iRet = RegisterCommand(INX_SM2_IMPORT_SM4_KEY,			0xA0,  0xFFFF, 5000,  1,  F_NORMAL, "SM2_IMPORT_SM4_KEY");
		iRet = RegisterCommand(INX_SM2_ENCRYPT_RAW,				0x92,  0xFFFF, 5000,  1,  F_NORMAL, "SM2_ENCRYPT_RAW");
		iRet = RegisterCommand(INX_SM2_DECRYPT_RAW,				0x93,  0xFFFF, 5000,  1,  F_NORMAL, "SM2_DECRYPT_RAW");
		iRet = RegisterCommand(INX_SM2_SIGN_RAW,				0x94,  0xFFFF, 5000,  1,  F_NORMAL, "SM2_SIGN_RAW");
		iRet = RegisterCommand(INX_SM2_VERIFY_SIGNATURE_RAW,	0x95,  0xFFFF, 5000,  1,  F_NORMAL, "SM2_VERIFY_SIGNATURE_RAW");
		iRet = RegisterCommand(INX_SM2_KEY_EXCHANGE_PROTOCAL_B, 0x96,	 230,  8000,  1,  F_NORMAL, "SM2_KEY_EXCHANGE_PROTOCAL_B");
		iRet = RegisterCommand(INX_SM2_KEY_AGREEMENT_B,			0x97,	  36,  8000,  1,  F_NORMAL, "SM2_KEY_AGREEMENT_B");
		iRet = RegisterCommand(INX_SM2_GENERATE_ZA,				0xA4,  0xFFFF, 1000,  1,  F_NORMAL, "SM2_GENERATE_ZA");
		iRet = RegisterCommand(INX_SM4_CYRPT_RAW,				0xA5,  0xFFFF, 5000,  1,  F_NORMAL, "SM4_CYRPT_RAW");
		iRet = RegisterCommand(INX_SM2_ENCRYPT,				    0xA5,  0xFFFF, 5000,  1,  F_NORMAL, "SM2_ENCRYPT");
		iRet = RegisterCommand(INX_SM4_STORE_KEY,				0xA7,  0xFFFF, 1000,  1,  F_NORMAL, "SM4_STORE_KEY");

		iRet = RegisterCommand(INX_FIREWARE_LOG,                0xE1,       0, 1000,  1,  F_NORMAL, "FIREWARE_LOG");
		iRet = RegisterCommand(INX_RI_AUTH_STATUS,              0xE1,       0, 1000,  1,  F_NORMAL, "INX_RI_AUTH_STATUS");
		iRet = RegisterCommand(INX_SET_HARDWARE_VER,			0xE3,  0xFFFF, 2000,  1,  F_NORMAL, "SET_HARDWARE_VER");
		iRet = RegisterCommand(INX_CLEAR_SENSOR,			    0xE2,       0, 1000,  1,  F_NORMAL, "CLEAR_SENSOR");

		iRet = RegisterCommand(INX_DOWN_A980_START,             0xAD,      1,  8000,  1,  F_NORMAL, "A980_START"); 
		iRet = RegisterCommand(INX_DOWN_A980_LOAD,              0xAE,   1024,  5000,  2,  F_NORMAL, "A980_LOAD");
		iRet = RegisterCommand(INX_DOWN_A980_END,               0xAF,      0, 12000,  1,  F_NORMAL, "A980_END");
	}


	m_pPort->SetReturnFormat(6, 3, CPinpadBase::ParseRetLen);
	return iRet;
}


/*****************************************************************************
 * 函数说明: 获取键盘码
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CPinpadHxx::Inner_GetKeyboardCode(char *lpKeyboardCodes, long &nOutLen, ENTRYMODE eMode)	
{
	nOutLen = 0;
	if(NULL == lpKeyboardCodes || NULL == m_pPort)
	{
		return EC_POINTER_NULL;
	}

	long nRet = 0;
	char szPara[512] = {0};
	long iBufOrRetLen = sizeof(szPara);

	if(eMode == ENTRY_MODE_CLOSE)
	{
		return EC_INVALID_PARA;
	}
	else
	{
		if(eMode < 0x30)
		{
			szPara[0] = eMode + 0x30;
		}
		else
		{
			szPara[0] = eMode;
		}
		
		nRet = Execute(INX_GET_KEYBOARD_CODES, szPara, iBufOrRetLen);
		if(nRet == EC_SUCCESS)
		{
			memcpy(lpKeyboardCodes, szPara, 16); //16 FK
			memcpy(lpKeyboardCodes + 16, szPara + 32, 8);//8 FDK

			nOutLen = 24;
		}

		return nRet;
	}
}


long CPinpadHxx::Inner_ReadKeyAttr(WORD wKeyId, LPDWORD lpKeyAttr, EKCVMODE eKCV, char *lpKCVRet, WORD wVI)
{
	long nRet = 0;
	DWORD dwKeyAttr = 0;
	if(NULL != lpKeyAttr)
	{
		dwKeyAttr = *lpKeyAttr;
	}

	if(IS_SM4(wKeyId, dwKeyAttr))
	{
		OFFSET_BASE_RIGHT(wKeyId, MIN_SM4_KEY);
		return Cmd_ReadKeyAttr(wKeyId, lpKeyAttr, eKCV, lpKCVRet);
	}
	else
	{
		return Cmd_ReadKeyAttr(wKeyId, wVI, lpKeyAttr, eKCV, lpKCVRet);
	}
}


/*****************************************************************************
 * 函数说明: 获取固件信息（自毁、统计数据）
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CPinpadHxx::GetFirmwareLog(EFWLOG eMode, char *pOut, long &nOutLen)
{
	RLog2(LOG_LEVEL_COMMON, "eMode=0x%X", eMode);

	nOutLen = 0;
	long nRet = 0;	
	char szPara[256] = {0};
	long iBufOrRetLen = sizeof(szPara);

	if(eMode == eFW_SelfDestruction_Clear)
	{
		nRet = Execute(INX_CLEAR_SENSOR, szPara, iBufOrRetLen);
	}
	else
	{
		nRet = Execute(INX_FIREWARE_LOG, szPara, iBufOrRetLen);
	}

	if(nRet == EC_SUCCESS)
	{
		nOutLen = iBufOrRetLen;
		if(NULL != pOut)
		{
			memcpy(pOut, szPara, iBufOrRetLen);
		}
	}

	return nRet;
}


/*****************************************************************************
 * 函数说明: 取电池电量
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CPinpadHxx::GetPower(long *pPower)
{
	if(NULL == pPower)
	{
		return EC_POINTER_NULL;
	}

	*pPower = 0;
	char szPara[256] = {0};
	long iBufOrRetLen = sizeof(szPara);

	long iRet = Execute(INX_GET_POWER, szPara, iBufOrRetLen);
	if(iRet == 0)
	{
		if((szPara[2] & 0x01) == 0) 
			*pPower |= 0x1;

		if((szPara[2] & 0x02) == 0) 
			*pPower |= 0x2;
	}

	RLog2(LOG_LEVEL_DATA, "iRet=%d, Power=0x%X", iRet, *pPower);
	return iRet;
}

