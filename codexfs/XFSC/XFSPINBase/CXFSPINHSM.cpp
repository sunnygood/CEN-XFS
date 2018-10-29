/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSPINHSM.cpp
* Description: EPP sub class of PIN
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#include "CXFSPINHSM.h"


CXFSPINHSM::CXFSPINHSM()
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
}

CXFSPINHSM::~CXFSPINHSM()
{
	RLog2(LOG_LEVEL_COMMON, "Enter");
}

//===========================================================================//
//                          GetInfo Commands                                 //
//===========================================================================//

HRESULT CXFSPINHSM::FuncKeyDetail(LPULONG lpulFDKMask)
{
	RLog2(LOG_LEVEL_DEBUG, "m_PressKey.m_vPinKeys.size()=%d m_PinFKMask=%d", m_PressKey.m_vPinKeys.size(), m_PressKey.m_PinFKMask);
	if(NULL == lpulFDKMask)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	ULONG ulFDKMask = *lpulFDKMask;

	m_CFuncKeyDetail.Clear();
	m_CFuncKeyDetail.Set(m_PressKey.m_PinFKMask);
	
	for(int i = 0; i < m_PressKey.m_vPinKeys.size(); ++i)
	{
		ULONG ulValue = m_PressKey.m_vPinKeys.at(i)->ulFK;
		if(m_PressKey.m_vPinKeys.at(i)->wKeyType == WFS_PIN_FDK && (ulValue & ulFDKMask))
		{
			m_CFuncKeyDetail.AddFDK(ulValue, m_PressKey.m_vPinKeys.at(i)->usXPos, m_PressKey.m_vPinKeys.at(i)->usYPos);
		}
	}

	return WFS_SUCCESS;
}

HRESULT CXFSPINHSM::SecureKeyDetail()
{
	m_CSecureKeyDetail.Clear();

	m_CSecureKeyDetail.Set(m_PressKey.m_wEntryMode, 
	                        m_PressKey.m_SecureFKMask, 
							m_PressKey.m_ulClearFDKs, 
							m_PressKey.m_ulCancelFDKs, 
                            m_PressKey.m_ulBackspaceFDKs, 
							m_PressKey.m_ulEnterFDKs, 
							m_PressKey.m_wColumns, 
							m_PressKey.m_wRows);

	for(int i = 0; i < m_PressKey.m_vSecureKeys.size(); ++i)
	{
		if(m_PressKey.m_vSecureKeys.at(i)->wKeyType == WFS_PIN_FDK)
		{
			m_CSecureKeyDetail.AddFDK(m_PressKey.m_vSecureKeys.at(i)->ulFK, m_PressKey.m_vSecureKeys.at(i)->usXPos, m_PressKey.m_vSecureKeys.at(i)->usYPos);
		}
		else
		{
			m_CSecureKeyDetail.AddHexKey(m_PressKey.m_vSecureKeys.at(i)->usXPos, 
			                              m_PressKey.m_vSecureKeys.at(i)->usYPos, 
										  m_PressKey.m_vSecureKeys.at(i)->usXSize, 
										  m_PressKey.m_vSecureKeys.at(i)->usYSize, 
							              m_PressKey.m_vSecureKeys.at(i)->ulFK, 
										  m_PressKey.m_vSecureKeys.at(i)->ulShiftFK);
		}	
	}

	return WFS_SUCCESS;
}


//===========================================================================//
//                          Exectue Commands                                 //
//===========================================================================//

HRESULT CXFSPINHSM::GetData(LPWFSPINGETDATA lpPinGetData)
{
	vector<BYTE> veCodes, veDisableCodes, veCurrentCodes;
	ULONG ulFK, ulFDK;
	BYTE szBuf[128] = {0};
	DWORD dwLen = 0, i = 0;
	WORD wComp = 0;
	long nRet = 0;

	if(NULL == lpPinGetData)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	USHORT usMaxLen = lpPinGetData->usMaxLen;
	BOOL bAutoEnd = lpPinGetData->bAutoEnd;
	ULONG ulActiveFDKs = lpPinGetData->ulActiveFDKs;
	ULONG ulActiveKeys = lpPinGetData->ulActiveKeys;
	ULONG ulTerminateFDKs = lpPinGetData->ulTerminateFDKs;
	ULONG ulTerminateKeys = lpPinGetData->ulTerminateKeys;
	
	RLog2(LOG_LEVEL_COMMON, "Start: usMaxLen=[%d],bAutoEnd=[%d],ulActiveFDKs=[0x%08X],ulActiveKeys=[0x%08X],"
		"ulTerminateFDKs=[0x%08X],ulTerminateKeys=[0x%08X]", usMaxLen, bAutoEnd, ulActiveFDKs, ulActiveKeys, ulTerminateFDKs, ulTerminateKeys);

	if(usMaxLen <= 0)
	{
		bAutoEnd = FALSE;
	}

	FAIL_RET(nRet, this, CheckActiveKey(ulActiveKeys, ulActiveFDKs, ulTerminateKeys, ulTerminateFDKs, m_PressKey.m_DataFKMask, m_PressKey.m_DataFDKMask));

	FAIL_RET(nRet, this, CheckOpen());
 	
	m_PressKey.GetDataCodes(0xFFFFFFFF, 0xFFFFFFFF, veCodes);
	CAutoRecoverKeyCode AutoRecoverKeyCode(m_pPinpad, veCodes, m_nIsDisablePressKey, ENTRY_MODE_TEXT);

	//打开并激活键盘
	m_PressKey.GetDataCodes(ulActiveKeys, ulActiveFDKs, veCodes);
	m_PressKey.GetDataCodes((WFS_PIN_FK_ENTER|WFS_PIN_FK_CANCEL|WFS_PIN_FK_CLEAR|WFS_PIN_FK_BACKSPACE) & ulActiveKeys, 
		                   ulActiveFDKs, veDisableCodes);

 	//写键码	
	if(m_nIsDisablePressKey)
	{
		if(nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size(), ENTRY_MODE_TEXT))
		{
			RLog2(LOG_LEVEL_ERROR, "write key codes fail, nRet=%d.", nRet);
			return Convert2SPError(nRet);
		}
		veCurrentCodes = veCodes;
	}

	//打开明文输入
	if(nRet = m_pPinpad->OpenKeyboardAndSound(ESOUND(m_nDataEntryVoice), ENTRY_MODE_TEXT))
	{
		RLog2(LOG_LEVEL_ERROR, "open input fail, nRet=%d.", nRet);
		return Convert2SPError(nRet);
	}
	CAutoCloseEntry AutoCloseEntry(m_pPinpad);
	

	//开始读明文
	m_GetDataResult.Clear();
	if(m_pSPKernel->GetXFSVersion() >= XFSVER_310)
	{
		FireEnterData();
	}

	m_CStatus.fwDevice = WFS_PIN_DEVBUSY;

	while(TRUE)
	{
		if(m_pSPKernel->IsRequestCancelled())
		{
			m_CStatus.fwDevice = WFS_PIN_DEVONLINE;
			RLog2(LOG_LEVEL_WARN, "pin input is cancelled.");
			return WFS_ERR_CANCELED;
		}
		if(m_pSPKernel->IsRequestTimeout())
		{
			m_CStatus.fwDevice = WFS_PIN_DEVONLINE;
			RLog2(LOG_LEVEL_WARN, "pin input is timeout.");
			return WFS_ERR_TIMEOUT;
		}
		
		//获取按键
		dwLen = 0;
		nRet = m_pPinpad->ReadText((char*)szBuf, dwLen, 100);
		for(i = 0; i < dwLen; ++i)
		{
			if(szBuf[i] == m_cLongPressCode)  //长按键超时
			{
				if(m_bIsLongPressedTimeout)
				{
					RLog2(LOG_LEVEL_WARN, "pin input long pressed timeout occupy.");	
					return WFS_ERR_TIMEOUT;
				}
				continue;
			}	
			if(szBuf[i] == m_cTimeOutCode)  //不按键超时，因键盘已经关闭输入，只能返回超时
			{
				RLog2(LOG_LEVEL_WARN, "pin input long not press timeout occupy.");
				return WFS_ERR_TIMEOUT;
			}

			//转换键值
			m_PressKey.GetKeyValue(szBuf[i], ulFK, ulFDK);

			//不在激活键范围内
			if(!(ulFK & ulActiveKeys || ulFDK & ulActiveFDKs) )
				continue;

			//功能键的处理//终止键处理
			wComp = WFS_PIN_COMPCONTINUE;
			switch(ulFK)
			{
			case 0:     //FDK
				if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs) 
				{
					wComp = WFS_PIN_COMPFDK;
				}
				else
				{
					wComp = WFS_PIN_COMPCONTFDK;
				}
				if(usMaxLen == 0 || m_GetDataResult.GetCount() < usMaxLen)
				{ 
					m_GetDataResult.Add(wComp, ulFK);
					FireKey(wComp, ulFK + ulFDK);
				}
				break;
			case WFS_PIN_FK_ENTER:
				if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
				{
					wComp = WFS_PIN_COMPENTER;
				}
				FireKey(wComp, ulFK + ulFDK);
				break;
			case WFS_PIN_FK_CANCEL:
				m_GetDataResult.Clear();
				if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
				{
					wComp = WFS_PIN_COMPCANCEL;
				}
				FireKey(wComp, ulFK + ulFDK);		
				break;
			case WFS_PIN_FK_CLEAR:
				m_GetDataResult.Clear();
				if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
				{
					wComp = WFS_PIN_COMPCLEAR;
				}
				FireKey(wComp, ulFK + ulFDK);	
				break;
			case WFS_PIN_FK_BACKSPACE:
				m_GetDataResult.DeleteEnd();
				if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
				{
					wComp = WFS_PIN_COMPBACKSPACE;
				}
				FireKey(wComp, ulFK + ulFDK);
				break;
			case WFS_PIN_FK_HELP:
				if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
				{
					wComp = WFS_PIN_COMPHELP;
				}
				if(usMaxLen == 0 || m_GetDataResult.GetCount() < usMaxLen)
				{ 
					m_GetDataResult.Add(wComp, ulFK);
					FireKey(wComp, ulFK + ulFDK);
				}
				break;
			case WFS_PIN_FK_00:
				switch(m_n00[0])
				{
				case 1: //一个WFS_PIN_FK_00
					if(usMaxLen == 0 || m_GetDataResult.GetCount() < usMaxLen)
					{
						m_GetDataResult.Add(wComp, ulFK);
						FireKey(wComp, ulFK + ulFDK);
					}
					break;
				case 2: //两个WFS_PIN_FK_0
					if(usMaxLen == 0 || m_GetDataResult.GetCount() + 2 <= usMaxLen)
					{
						m_GetDataResult.Add(wComp, WFS_PIN_FK_0); 
						FireKey(wComp, WFS_PIN_FK_0);
						m_GetDataResult.Add(wComp, WFS_PIN_FK_0);
						FireKey(wComp, WFS_PIN_FK_0);
					}
					break;
				default: //什么都不处理
					break;
				}
				break;
			case WFS_PIN_FK_000:
				switch(m_n000[0])
				{
				case 1: //一个WFS_PIN_FK_000
					if(usMaxLen == 0 || m_GetDataResult.GetCount() < usMaxLen)
					{
						m_GetDataResult.Add(wComp, ulFK);
						FireKey(wComp, ulFK + ulFDK);
					}
					break;
				case 2: //三个WFS_PIN_FK_0
					if(usMaxLen == 0 || m_GetDataResult.GetCount() + 3 <= usMaxLen)
					{
						m_GetDataResult.Add(wComp, WFS_PIN_FK_0);
						FireKey(wComp, WFS_PIN_FK_0);
						m_GetDataResult.Add(wComp, WFS_PIN_FK_0);
						FireKey(wComp, WFS_PIN_FK_0);
						m_GetDataResult.Add(wComp, WFS_PIN_FK_0);
						FireKey(wComp, WFS_PIN_FK_0);
					}
					break;
				default: //什么都不处理
					break;
				}
				break;
			default:
				if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs) //终止键处理
				{
					wComp = WFS_PIN_COMPFK;
				}
				if(usMaxLen == 0 || m_GetDataResult.GetCount() < usMaxLen)
				{ 
					m_GetDataResult.Add(wComp, ulFK);
					FireKey(wComp, ulFK + ulFDK);
				}
				break;
			}

			//自动终止处理
			if(bAutoEnd && (m_GetDataResult.GetCount() >= usMaxLen))
			{
				wComp = WFS_PIN_COMPAUTO;
				goto Exit;
			}	
			
			//终止键处理
			if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
			{
				goto Exit;
			}

			//按键数小于最大值时，恢复正常键码
			if(m_nIsDisablePressKey && m_GetDataResult.GetCount() < usMaxLen && veCurrentCodes != veCodes)
			{
				if(nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size()))
				{
					RLog2(LOG_LEVEL_ERROR, "write key codes fail, nRet=%d.", nRet);
					return Convert2SPError(nRet);
				}								
				veCurrentCodes = veCodes;
			}		

			//最大值>0, 且按键数>=最大值，且自动终止为假时，禁止键
			if(m_nIsDisablePressKey && usMaxLen > 0 && m_GetDataResult.GetCount() >= usMaxLen && veCurrentCodes != veDisableCodes)
			{
				if(nRet = m_pPinpad->SetKeyboardCode((char*)(veDisableCodes.data()), veDisableCodes.size()))
				{
					RLog2(LOG_LEVEL_ERROR, "write key codes fail, nRet=%d.", nRet);
					return Convert2SPError(nRet);
				}									
				veCurrentCodes = veDisableCodes;
			}
		} //for
	} //while

Exit:
	m_CStatus.fwDevice = WFS_PIN_DEVONLINE;
	m_GetDataResult.Set(wComp);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINHSM::GetPin(LPWFSPINGETPIN lpGetPin)
{
	vector<BYTE> veCodes, veDisableCodes, veCurrentCodes;
	ULONG ulFK, ulFDK;
	BYTE szBuf[512];
	DWORD i, dwLen = 0;
	WORD wComp = 0;
	long nRet = 0;

	if(NULL == lpGetPin)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	USHORT usMinLen = lpGetPin->usMinLen;
	USHORT usMaxLen = lpGetPin->usMaxLen;
	BOOL bAutoEnd = lpGetPin->bAutoEnd;
	CHAR cEcho = lpGetPin->cEcho;
	ULONG ulActiveFDKs = lpGetPin->ulActiveFDKs;
	ULONG ulActiveKeys = lpGetPin->ulActiveKeys;
	ULONG ulTerminateFDKs = lpGetPin->ulTerminateFDKs;
	ULONG ulTerminateKeys = lpGetPin->ulTerminateKeys;
	
	RLog2(LOG_LEVEL_COMMON,
		"Start: usMinLen=[%d],usMaxLen=[%d],bAutoEnd=[%d],cEcho=[0x%X],ulActiveFKs=[0x%X],"
		"ulActiveFDKs=[0x%X],ulTerminateFKs=[0x%X],ulTerminateFDKs=[0x%X]",
		usMinLen,usMaxLen, bAutoEnd, cEcho, ulActiveKeys, ulActiveFDKs, ulTerminateKeys, ulTerminateFDKs);

	DWORD dwEcho = ((m_nEcho != 0xFFFF) ? m_nEcho : cEcho);
	m_usPinLen = 0;


	if(usMaxLen > 0 && usMinLen > usMaxLen)
	{
		RLog2(LOG_LEVEL_ERROR, "usMinLen[%d] is greater than usMaxLen[%d].", usMinLen, usMaxLen);
		return WFS_ERR_PIN_MINIMUMLENGTH;
	}

	if(usMaxLen == 0 && bAutoEnd)
	{
		RLog2(LOG_LEVEL_ERROR, "AutoEnd is true, so max length can't be 0.");
		return WFS_ERR_PIN_MINIMUMLENGTH;
	}

	FAIL_RET(nRet, this, CheckTerminateKey(ulTerminateKeys, ulTerminateFDKs, bAutoEnd));
	FAIL_RET(nRet, this, CheckActiveKey(ulActiveKeys, ulActiveFDKs, ulTerminateKeys, ulTerminateFDKs, m_PressKey.m_PinFKMask, m_PressKey.m_PinFDKMask));

	FAIL_RET(nRet, this, CheckOpen());

	//特殊处理Cxx系列的clear和backspace键
	if(m_stPinCaps.ePinpadType & ePIN_EPP)
	{
		SetKeyCodeforCxx(ulActiveKeys);
	}

	m_PressKey.GetPinCodes(0xFFFFFFFF, 0xFFFFFFFF, veCodes);
	CAutoRecoverKeyCode AutoRecoverKeyCode(m_pPinpad, veCodes, m_nIsDisablePressKey, ENTRY_MODE_PIN);

	//激活按键
	m_PressKey.GetPinCodes(ulActiveKeys, ulActiveFDKs, veCodes);
	m_PressKey.GetPinCodes(~(WFS_PIN_FK_0|WFS_PIN_FK_1|WFS_PIN_FK_2|WFS_PIN_FK_3|WFS_PIN_FK_4|WFS_PIN_FK_5|
		              WFS_PIN_FK_6|WFS_PIN_FK_7|WFS_PIN_FK_8|WFS_PIN_FK_9) & ulActiveKeys, ulActiveFDKs, veDisableCodes);

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
	if(nRet = m_pPinpad->StartPinInput(ESOUND(m_nPinEntryVoice), (BYTE)usMaxLen, (BYTE)usMinLen, bAutoEnd ? true : false, m_nStartPinInputTimeout))
	{
		RLog2(LOG_LEVEL_ERROR, "open input fail, nRet=%d.", nRet);
		return Convert2SPError(nRet);
	}	
	CAutoCloseEntry AutoCloseEntry(m_pPinpad);

	BOOL bExitNow = FALSE;

	if(m_pSPKernel->GetXFSVersion() >= XFSVER_310)
	{
		FireEnterData();
	}

	m_CStatus.fwDevice = WFS_PIN_DEVBUSY;

	while(TRUE)
	{
		if (m_pSPKernel->IsRequestCancelled())
		{
			m_CStatus.fwDevice = WFS_PIN_DEVONLINE;
			RLog2(LOG_LEVEL_WARN, "pin input is cancelled.");
			if(usMaxLen > 0 && m_usPinLen < usMinLen)
				m_usPinLen = 0;
			if(m_bIsAysnCancelClearPins) 
				m_usPinLen = 0;

			return WFS_ERR_CANCELED;
		}

		if(m_pSPKernel->IsRequestTimeout())
		{
			m_CStatus.fwDevice = WFS_PIN_DEVONLINE;
			RLog2(LOG_LEVEL_WARN, "pin input is timeout.");
			if(usMaxLen > 0 && m_usPinLen < usMinLen)
				m_usPinLen = 0;
			if(m_bIsTimeoutClearPins) 
				m_usPinLen = 0;

			return WFS_ERR_TIMEOUT;
		}

		//获取一次按键
		dwLen = 0;
		nRet = m_pPinpad->ReadText((char*)szBuf, dwLen, 100);
		for(i = 0; i < dwLen; ++i)
		{
			if(szBuf[i] == m_cLongPressCode)  //长按键超时
			{
				if(m_bIsLongPressedTimeout)
				{
					RLog2(LOG_LEVEL_WARN, "pin input long pressed timeout occupy.");
					if(usMaxLen > 0 && m_usPinLen < usMinLen)
						m_usPinLen = 0;
					if(m_bIsTimeoutClearPins) 
						m_usPinLen = 0;
					return WFS_ERR_TIMEOUT;
				}
				continue;
			}		
			if(szBuf[i] == m_cTimeOutCode)  //不按键超时，因键盘已经关闭输入，只能返回超时
			{
				RLog2(LOG_LEVEL_WARN, "pin input long not press timeout occupy.");
				if(usMaxLen > 0 && m_usPinLen < usMinLen)
					m_usPinLen = 0;
				if(m_bIsTimeoutClearPins) 
					m_usPinLen = 0;	
				return WFS_ERR_TIMEOUT;
			}

			m_PressKey.GetKeyValue(szBuf[i], ulFK, ulFDK);
			//密码键处理
			if(szBuf[i] == 0x00 || (ulFK == 0 && ulFDK == 0) || szBuf[i] == m_cEchoHW)
			{
				//计数密码位
				if(usMaxLen > 0 && m_usPinLen >= usMaxLen)
					continue;

				m_usPinLen++;
				FireKey(WFS_PIN_COMPCONTINUE, dwEcho);

				//禁止数字键
				if(m_nIsDisablePressKey && usMaxLen > 0 && m_usPinLen >= usMaxLen && veCurrentCodes != veDisableCodes)
				{
					if(nRet = m_pPinpad->SetKeyboardCode((char*)(veDisableCodes.data()), veDisableCodes.size(), ENTRY_MODE_PIN))
					{
						RLog2(LOG_LEVEL_ERROR, "write key codes fail, nRet=%d.", nRet);
						if(usMaxLen > 0 && m_usPinLen < usMinLen)
							m_usPinLen = 0;
						return Convert2SPError(nRet);
					}
					veCurrentCodes = veDisableCodes;
				}

				//自动终止
				if(usMaxLen > 0 && m_usPinLen >= usMaxLen && bAutoEnd == TRUE)
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
				if((ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs) && m_usPinLen >= usMinLen)
				{
					wComp = WFS_PIN_COMPFDK;
					bExitNow = TRUE;
				}
				else
				{
					wComp = WFS_PIN_COMPCONTFDK;
				}
				break;
			case WFS_PIN_FK_ENTER:
				if((ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs) && m_usPinLen >= usMinLen)
				{
					wComp = WFS_PIN_COMPENTER;
					bExitNow = TRUE;
				}
				break;
			case WFS_PIN_FK_CANCEL:
				if(ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs)
				{
					wComp = WFS_PIN_COMPCANCEL;
					bExitNow = TRUE;
				}

				for ( ; m_usPinLen > 0; --m_usPinLen)
				{
					m_pPinpad->DelEntryBuffer();
				}

				break;
			case WFS_PIN_FK_CLEAR:
				if((ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs) && m_usPinLen >= usMinLen)
				{
					wComp = WFS_PIN_COMPCLEAR;
					bExitNow = TRUE;
				}

				for ( ; m_usPinLen > 0; --m_usPinLen)
				{
					m_pPinpad->DelEntryBuffer();
				}

				break;
			case WFS_PIN_FK_BACKSPACE:
				if((ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs) && m_usPinLen >= usMinLen)
				{
					wComp = WFS_PIN_COMPBACKSPACE;
					bExitNow = TRUE;
				}

				if(m_usPinLen > 0)
				{
					m_usPinLen--;
					m_pPinpad->DelEntryBuffer();
				}

				break;
			case WFS_PIN_FK_HELP:
				if((ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs) && m_usPinLen >= usMinLen)
				{
					wComp = WFS_PIN_COMPHELP;
					bExitNow = TRUE;
				}
				break;
			default:
				if((ulFK & ulTerminateKeys || ulFDK & ulTerminateFDKs) && m_usPinLen >= usMinLen)
				{
					wComp = WFS_PIN_COMPFK;
					bExitNow = TRUE;
				}
				break;
			}
			FireKey(wComp, ulFK + ulFDK);


			if(bExitNow)
			{
				goto Exit;
			}

			//恢复数字键
			if(m_nIsDisablePressKey &&  m_usPinLen < usMaxLen && veCurrentCodes != veCodes)
			{
				if(nRet = m_pPinpad->SetKeyboardCode((char*)(veCodes.data()), veCodes.size(), ENTRY_MODE_PIN))
				{
					RLog2(LOG_LEVEL_ERROR, "write key codes fail, nRet=%d.", nRet);
					if(usMaxLen > 0 && m_usPinLen < usMinLen)
						m_usPinLen = 0;
					return Convert2SPError(nRet);
				}
				veCurrentCodes = veCodes;
			}	
		} //for
	} //while

Exit:
	m_CStatus.fwDevice = WFS_PIN_DEVONLINE;
	m_GetPinResult.Set(m_usPinLen, wComp);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINHSM::GetPinBlock(LPWFSPINBLOCK lpPinBlock)
{
	LPSTR lpsCustomerData, lpsXORData, lpsKey, lpsKeyEncKey;
	BYTE PinBlock[32] = {0}, bPadding;
	WORD wKeyId = KEY_INVALID, wEnKeyId = KEY_INVALID;
	long nRet = 0;
	WORD wFormat = 0;

	if(!lpPinBlock)
	{
		return WFS_ERR_INVALID_POINTER;
	}
	lpsCustomerData = lpPinBlock->lpsCustomerData;
	lpsXORData = lpPinBlock->lpsXORData;
	bPadding = Padding2Bin(lpPinBlock->bPadding);
	wFormat = lpPinBlock->wFormat;
	lpsKey = lpPinBlock->lpsKey;
	lpsKeyEncKey = lpPinBlock->lpsKeyEncKey;

	if(NULL == lpsCustomerData)
	{
		RLog2(LOG_LEVEL_COMMON, "Start: lpsXORData=[%s],bPadding=[0x%X],wFormat=[0x%04X],lpsKey=[%s],lpsKeyEncKey=[%s]",
			lpsXORData, bPadding, wFormat, lpsKey, lpsKeyEncKey);
	}
	else
	{
		RLog2(LOG_LEVEL_COMMON, "Start: lpsCustomerData=[********%s],lpsXORData=[%s],bPadding=[0x%X],wFormat=[0x%04X],lpsKey=[%s],lpsKeyEncKey=[%s]",
			lpsCustomerData + 8, lpsXORData, bPadding, wFormat, lpsKey, lpsKeyEncKey);
	}
	
	if(!(m_stPinCaps.fwPinFormats & wFormat))
	{
		RLog2(LOG_LEVEL_ERROR, "format is not supported.");
		return WFS_ERR_PIN_FORMATNOTSUPP;
	}

	if(m_usPinLen <= 0)
	{
		RLog2(LOG_LEVEL_ERROR, "there is no pins in pinpad.");
		return WFS_ERR_PIN_NOPIN;
	}

	FAIL_RET(nRet, this, CheckOpen());

	FAIL_RET(nRet, this, FindKey(lpsKey, wKeyId, (WFS_PIN_USEFUNCTION)));

	if(lpsKeyEncKey && lpsKeyEncKey[0] != '\0')
	{
		DWORD dwUsage = 0;
		//判断是否需要检查二次密钥用途，兼容wEnKeyId是主密钥的ZT598B用法
		if(m_nIsCheckEnKeyUsage > 0)
		{
			dwUsage = WFS_PIN_USEFUNCTION | WFS_PIN_USERSAPUBLIC; //只检查PIN二次加密密钥
		}

		FAIL_RET(nRet, this, FindKey(lpsKeyEncKey, wEnKeyId, dwUsage));
	}

	
	char szCustomerData[20] = {0x00}, *pCustomerData = NULL;
	int nLen;
	pCustomerData = (char*)lpsCustomerData;
	if((wFormat == WFS_PIN_FORMANSI || wFormat == WFS_PIN_FORMISO0 || wFormat == WFS_PIN_FORMISO3) && pCustomerData != NULL && pCustomerData[0] != '\0')
	{
		nLen = strlen(pCustomerData);
		memset(szCustomerData, 0x30, sizeof(szCustomerData));
		if(m_nCustomerDataMode == 1) //SP处理和带校验位
		{
			if(nLen >= 13)
				memcpy(szCustomerData, pCustomerData+nLen-13, 12);	
			else
			   memcpy(szCustomerData+13-nLen, pCustomerData, nLen-1);

			szCustomerData[12] = 0x00;
			pCustomerData = szCustomerData;
		}
		else if(m_nCustomerDataMode == 2) //SP处理和不带校验位
		{
			if(nLen >= 12)
				memcpy(szCustomerData, pCustomerData+nLen-12, 12);	
			else	
			  memcpy(szCustomerData+12-nLen, pCustomerData, nLen);

			szCustomerData[12] = 0x00;
			pCustomerData = szCustomerData;
		}
		else //SP不处理
		{
			;
		}
	}
	
	WORD wOutLen = 0;
	if(nRet = m_pPinpad->GetPinBlock(wKeyId, (BYTE)m_usPinLen, (EPINFORMAT)wFormat, pCustomerData, (char*)PinBlock, wOutLen, bPadding, wEnKeyId))
	{
		RLog2(LOG_LEVEL_ERROR, "get pin block fail, nRet=[%d]", nRet);
		nRet = Convert2SPError(nRet);
		if(nRet == WFS_ERR_PIN_ALGORITHMNOTSUPP)
		{
			nRet = WFS_ERR_PIN_FORMATNOTSUPP;
		}

		return nRet;
	}

	m_XDataResult.Set((LPBYTE)PinBlock, wOutLen);
	m_usPinLen = 0;
	RLog2(LOG_LEVEL_COMMON, "End:");		
	return WFS_SUCCESS;
}

HRESULT CXFSPINHSM::CreateOffset(LPWFSPINCREATEOFFSET lpPinOffset)
{
	if(NULL == lpPinOffset)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	char szOffset[128] = {0};
	long nRet = 0;

	LPSTR lpsValidationData = lpPinOffset->lpsValidationData;
	BYTE bPadding = lpPinOffset->bPadding;
	USHORT usMaxPIN = lpPinOffset->usMaxPIN;
	USHORT usValDigits = lpPinOffset->usValDigits;
	LPSTR lpsKey = lpPinOffset->lpsKey;
	LPWFSXDATA lpxKeyEncKey = lpPinOffset->lpxKeyEncKey;
	LPSTR lpsDecTable = lpPinOffset->lpsDecTable;

	BYTE ucMode = m_nOffsetGenAlgorithm;

	RLog2(LOG_LEVEL_COMMON, "Start: lpsValidationData=[%s],bPadding=[0x%X],usMaxPIN=[%d],"
		"usValDigits=[%d],lpsKey=[%s],lpsDecTable=[%s],GenAlg=0x%X",
		lpsValidationData, bPadding, usMaxPIN, usValDigits, lpsKey, lpsDecTable, ucMode);

	if(nRet = LocalIBM(lpsKey, lpxKeyEncKey, usMaxPIN, bPadding, lpsDecTable, lpsValidationData, usValDigits, ucMode, szOffset))
	{
		RLog2(LOG_LEVEL_ERROR, "Create offset failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}
	
	m_CharResult.Set(szOffset);
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINHSM::LocalDes(LPWFSPINLOCALDES lpLocalDes)
{
	if(NULL == lpLocalDes)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	char szOffset[128] = {0};
	long nRet = 0;

	LPSTR lpsValidationData = lpLocalDes->lpsValidationData;
	BYTE bPadding = lpLocalDes->bPadding;
	USHORT usMaxPIN = lpLocalDes->usMaxPIN;
	USHORT usValDigits = lpLocalDes->usValDigits;
	BOOL bNoLeadingZero = lpLocalDes->bNoLeadingZero;
	LPSTR lpsKey = lpLocalDes->lpsKey;
	LPWFSXDATA lpxKeyEncKey = lpLocalDes->lpxKeyEncKey;
	LPSTR lpsDecTable = lpLocalDes->lpsDecTable;

	BYTE ucMode = m_nOffsetVerifyAlgorithm;

	if(NULL != lpLocalDes->lpsOffset)
	{
		memcpy(szOffset, lpLocalDes->lpsOffset, strlen(lpLocalDes->lpsOffset));
	}

	RLog2(LOG_LEVEL_COMMON, "Start: lpsValidationData=[%s],lpsOffset=[%s],bPadding=[0x%X],usMaxPIN=[%d],"
		"usValDigits=[%d],bNoLeadingZero=[%d],lpsKey=[%s],lpsDecTable=[%s],VerifyAlg=0x%X",
		lpsValidationData, szOffset, bPadding, usMaxPIN, usValDigits, bNoLeadingZero, lpsKey, lpsDecTable, ucMode);

	m_BoolResult = FALSE;
	if(nRet = LocalIBM(lpsKey, lpxKeyEncKey, usMaxPIN, bPadding, lpsDecTable, lpsValidationData, usValDigits, ucMode, szOffset))
	{
		if(nRet != PIN_PIN_VERIFICATION_FAILED)
		{
			RLog2(LOG_LEVEL_ERROR, "Local des failed, nRet=[%d]", nRet);
			return Convert2SPError(nRet);
		}
	}
	else
	{
		m_BoolResult = TRUE;
	}
	
	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINHSM::LocalEurocheque(LPWFSPINLOCALEUROCHEQUE lpLocalEurocheque)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINHSM::LocalVisa(LPWFSPINLOCALVISA lpLocalVisa)
{
	LPSTR lpsPAN, lpsPVV, lpsKey;
	WORD wPVVDigits;
	LPWFSXDATA lpxKeyEncKey;
	long nRet, nLen;
	WORD wKeyId = KEY_INVALID;
	char lpKeyEncKey[32] = {0x00};
	BYTE bKeyEncKeyLen = 0;
	char szPvv[128] = {0};
	BOOL bResult = TRUE;

	if(!lpLocalVisa)
	{
		return WFS_ERR_INVALID_POINTER;
	}
	lpsPAN = lpLocalVisa->lpsPAN;
	lpsPVV = lpLocalVisa->lpsPVV;
	wPVVDigits = lpLocalVisa->wPVVDigits;
	lpsKey = lpLocalVisa->lpsKey;
	lpxKeyEncKey = lpLocalVisa->lpxKeyEncKey;

	RLog2(LOG_LEVEL_COMMON, "Start: lpsPAN=[%s],lpsPVV=[%s],wPVVDigits=[%d],lpsKey=[%s]",	lpsPAN, lpsPVV, wPVVDigits, lpsKey);

	if(lpxKeyEncKey && lpxKeyEncKey->lpbData && lpxKeyEncKey->usLength > 0)
	{
		bKeyEncKeyLen = (BYTE)lpxKeyEncKey->usLength;
		if(bKeyEncKeyLen != 8 && bKeyEncKeyLen != 16 && bKeyEncKeyLen != 24)
		{
			RLog2(LOG_LEVEL_ERROR, "twice encrypted key length is wrong, real length=[%d].", bKeyEncKeyLen);
			return WFS_ERR_PIN_INVALIDKEYLENGTH;
		}
		memcpy(lpKeyEncKey, lpxKeyEncKey->lpbData, bKeyEncKeyLen);
	}

	//检查PVV参数
	if(!lpsPVV || lpsPVV[0] == '\0')
	{
		RLog2(LOG_LEVEL_ERROR, "pvv data is empty.");
		return WFS_ERR_PIN_INVALIDDATA;
	}	
	nLen = strlen(lpsPVV);
	if(nLen < wPVVDigits)
	{
		RLog2(LOG_LEVEL_ERROR, "pvv data length is less than wPVVDigits.");
		return WFS_ERR_PIN_INVALIDDATA;
	}	
	memcpy(szPvv, lpsPVV, wPVVDigits);

	FAIL_RET(nRet, this, CheckOpen());

	FAIL_RET(nRet, this, FindKey(lpsKey, wKeyId, (WFS_PIN_USEFUNCTION | WFS_PIN_USEPINLOCAL)));

	m_BoolResult = FALSE;
	if(nRet = m_pPinpad->PO_LocalVISA(wKeyId, lpKeyEncKey, bKeyEncKeyLen, lpsPAN, m_nVisaAlgorithm, szPvv, m_nVisaLen, m_nVisaIndex))
	{
		if(nRet != PIN_PIN_VERIFICATION_FAILED)
		{
			RLog2(LOG_LEVEL_ERROR, "local des fail, nRet=[%d]", nRet);
			return Convert2SPError(nRet);
		}
	}
	else
	{
		m_BoolResult = TRUE;
	}

	if(m_nVisaAlgorithm == 0x10 || m_nVisaAlgorithm == 0x11) //Generate
	{
		RLog2(LOG_LEVEL_DATA, "Generate PVV=%s", szPvv);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINHSM::SecureKeyEntry(LPWFSPINSECUREKEYENTRY lpSecureKeyEntry)
{
	long nRet = 0;
	if(NULL == lpSecureKeyEntry)
	{
		RLog2(LOG_LEVEL_ERROR, "Pointer is NULL.");
		return WFS_ERR_INVALID_POINTER;
	}

	USHORT usKeyLen = lpSecureKeyEntry->usKeyLen;
	BOOL bAutoEnd = lpSecureKeyEntry->bAutoEnd; 
	ULONG ulActiveFDKs = lpSecureKeyEntry->ulActiveFDKs; 
	ULONG ulActiveKeys = lpSecureKeyEntry->ulActiveKeys; 
	ULONG ulTerminateFDKs = lpSecureKeyEntry->ulTerminateFDKs; 
	ULONG ulTerminateKeys = lpSecureKeyEntry->ulTerminateKeys; 
	WORD wVerificationType = lpSecureKeyEntry->wVerificationType;

	RLog2(LOG_LEVEL_COMMON, "usKeyLen=[%d],bAutoEnd=[%d],ulActiveFKs=[0x%X],ulActiveFDKs=[0x%X],"
		"ulTerminateFKs=[0x%X],ulTerminateFDKs=[0x%X]", usKeyLen, bAutoEnd, ulActiveKeys, ulActiveFDKs, ulTerminateKeys,ulTerminateFDKs);

	m_usKeyLen = 0;

	//检查密钥长度
	if(usKeyLen != 16 && usKeyLen != 32)
	{
		RLog2(LOG_LEVEL_ERROR, "the key length is wrong, usKeyLen=[%d].", usKeyLen);
		return WFS_ERR_PIN_INVALIDKEYLENGTH;
	}

	FAIL_RET(nRet, this, CheckTerminateKey(ulTerminateKeys, ulTerminateFDKs, bAutoEnd));
	FAIL_RET(nRet, this, CheckActiveKey(ulActiveKeys, ulActiveFDKs, ulTerminateKeys, ulTerminateFDKs, m_PressKey.m_SecureFKMask, m_PressKey.m_SecureFDKMask));


	//标准要求不能支持00,000,DECPOINT键
	/*if(m_PressKey.m_SecureFKMask & (WFS_PIN_FK_00 | WFS_PIN_FK_000 | WFS_PIN_FK_DECPOINT))
	{
		RLog2(LOG_LEVEL_ERROR, "Secure entry pinpad can't support 00|000|DECPOINT.");
		return WFS_ERR_CFG_INVALID_VALUE;		
	}*/

	//标准要求不能支持00,000,DECPOINT键
	/*if(ulActiveKeys & (WFS_PIN_FK_00 | WFS_PIN_FK_000 | WFS_PIN_FK_DECPOINT))
	{
		RLog2(LOG_LEVEL_ERROR, "Active FKs can't include 00|000|DECPOINT.");
		return WFS_ERR_INVALID_DATA;		
	}*/

	//标准要求不能支持00,000,DECPOINT键
	if(ulTerminateKeys & (WFS_PIN_FK_00 | WFS_PIN_FK_000 | WFS_PIN_FK_DECPOINT))
	{
		RLog2(LOG_LEVEL_ERROR, "Terminate FKs can not used 00|000|DECPOINT.");
		return WFS_ERR_PIN_KEYNOTSUPPORTED;		
	}

	FAIL_RET(nRet, this, CheckOpen());

	if(m_PressKey.m_wEntryMode == WFS_PIN_SECUREKEY_REG_UNIQUE || m_PressKey.m_wEntryMode == WFS_PIN_SECUREKEY_IRREG_UNIQUE)
	{
		if(m_PressKey.m_eShiftMode != SHIFT_NONE)
		{
			RLog2(LOG_LEVEL_ERROR, "Invalid ShiftMode parameter.");
			return WFS_ERR_CFG_INVALID_VALUE;
		}
		else if(nRet = SecureKeyEntryForN(usKeyLen, bAutoEnd, ulActiveFDKs, ulActiveKeys, ulTerminateFDKs, ulTerminateKeys, wVerificationType))
		{
			RLog2(LOG_LEVEL_ERROR, "unique mode key entry fail, nRet=[%d].", nRet);
			return nRet;
		}
	}
	else if(m_PressKey.m_wEntryMode == WFS_PIN_SECUREKEY_REG_SHIFT || m_PressKey.m_wEntryMode == WFS_PIN_SECUREKEY_IRREG_SHIFT)
	{
		if(m_PressKey.m_eShiftMode == SHIFT_NONE)
		{
			RLog2(LOG_LEVEL_ERROR, "Invalid ShiftMode parameter.");
			return WFS_ERR_CFG_INVALID_VALUE;
		}
		else if(m_PressKey.m_eShiftMode == SHIFT_HW)
		{
			if(nRet = SecureKeyEntryForH(usKeyLen, bAutoEnd, ulActiveFDKs, ulActiveKeys, ulTerminateFDKs, ulTerminateKeys, wVerificationType))
			{
				RLog2(LOG_LEVEL_ERROR, "hardware shift mode key entry fail, nRet=[%d].", nRet);
				return nRet;
			}
		}
		else
		{
			if(nRet = SecureKeyEntryForS(usKeyLen, bAutoEnd, ulActiveFDKs, ulActiveKeys, ulTerminateFDKs, ulTerminateKeys, wVerificationType))
			{
				RLog2(LOG_LEVEL_ERROR, "software shift mode key entry fail, nRet=[%d].", nRet);
				return nRet;
			}
		}
	}
	else
	{
		RLog2(LOG_LEVEL_ERROR, "Entry mode is wrong.");
		return WFS_ERR_CFG_INVALID_VALUE;
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINHSM::KeyPressBeep(LPWORD lpwMode)
{
	WORD wMode = 0;
	if(NULL != lpwMode)
	{
		wMode = *lpwMode;
	}

	RLog2(LOG_LEVEL_COMMON, "Start: wMode=[%d]", wMode);

	long nRet = 0;
	FAIL_RET(nRet, this, CheckOpen());

	if(wMode == WFS_PIN_BEEP_ON_ACTIVE)
	{
		nRet = m_pPinpad->OpenKeyboardAndSound(SOUND_OPEN, ENTRY_MODE_CLOSE);
	}
	else
	{
		nRet = m_pPinpad->OpenKeyboardAndSound(SOUND_CLOSE, ENTRY_MODE_CLOSE);
	}

	if(nRet)
	{
		RLog2(LOG_LEVEL_ERROR, "KeyPressBeep failed, nRet=[%d].", nRet);
		return Convert2SPError(nRet);
	}

	RLog2(LOG_LEVEL_COMMON, "End:");
	return WFS_SUCCESS;
}

HRESULT CXFSPINHSM::GetPinBlockEx(LPWFSPINBLOCKEX lpPinBlockEx)
{
	long nRet = 0;
	BYTE PinBlock[256] = {0};
	WORD wKeyId = KEY_INVALID, wEnKeyId = KEY_INVALID;
	char szCustomerData[32] = {0}, *pCustomerData = NULL;

	if(NULL == lpPinBlockEx)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	LPSTR lpsCustomerData = lpPinBlockEx->lpsCustomerData;
	LPSTR lpsXORData = lpPinBlockEx->lpsXORData;
	BYTE bPadding = Padding2Bin(lpPinBlockEx->bPadding);
	DWORD dwFormat = lpPinBlockEx->dwFormat;
	LPSTR lpsKey = lpPinBlockEx->lpsKey;
	LPSTR lpsKeyEncKey = lpPinBlockEx->lpsKeyEncKey;
	DWORD dwAlgorithm = lpPinBlockEx->dwAlgorithm;

	RLog2(LOG_LEVEL_COMMON, "Start: lpsCustomerData=[%s],lpsXORData=[%s],bPadding=[0x%X],dwFormat=[0x%08X],dwAlgorithm=[0x%08X],lpsKey=[%s],lpsKeyEncKey=[%s]",
		lpsCustomerData, lpsXORData, bPadding, dwFormat, dwAlgorithm, lpsKey, lpsKeyEncKey);

	if(!(m_stPinCaps.fwPinFormats & dwFormat))
	{
		RLog2(LOG_LEVEL_ERROR, "format is not supported.");
		return WFS_ERR_PIN_FORMATNOTSUPP;
	}

	if(m_usPinLen <= 0)
	{
		RLog2(LOG_LEVEL_ERROR, "there is no pins in pinpad.");
		return WFS_ERR_PIN_NOPIN;
	}

	FAIL_RET(nRet, this, CheckOpen());

	FAIL_RET(nRet, this, FindKey(lpsKey, wKeyId, (WFS_PIN_USEFUNCTION | WFS_PIN_USERSAPUBLIC)));

	if(lpsKeyEncKey && lpsKeyEncKey[0] != '\0')
	{
		DWORD dwUsage = 0;
		//判断是否需要检查二次密钥用途，兼容wEnKeyId是主密钥的ZT598B用法
		if(m_nIsCheckEnKeyUsage > 0)
		{
			dwUsage = WFS_PIN_USEFUNCTION | WFS_PIN_USERSAPUBLIC; //只检查PIN二次加密密钥
		}

		FAIL_RET(nRet, this, FindKey(lpsKeyEncKey, wEnKeyId, dwUsage));
	}

	pCustomerData = (char*)lpsCustomerData;
	if((dwFormat == WFS_PIN_FORMANSI || dwFormat == WFS_PIN_FORMISO0 || dwFormat == WFS_PIN_FORMISO3) && pCustomerData != NULL && pCustomerData[0] != '\0')
	{
		int nLen = strlen(pCustomerData);
		memset(szCustomerData, 0x30, sizeof(szCustomerData));
		if(m_nCustomerDataMode == 1) //SP处理和带校验位
		{
			if(nLen >= 13)
				memcpy(szCustomerData, pCustomerData+nLen - 13, 12);	
			else
			   memcpy(szCustomerData + 13 - nLen, pCustomerData, nLen - 1);

			szCustomerData[12] = 0x00;
			pCustomerData = szCustomerData;
		}
		else if(m_nCustomerDataMode == 2) //SP处理和不带校验位
		{
			if(nLen >= 12)
				memcpy(szCustomerData, pCustomerData+nLen - 12, 12);	
			else	
			  memcpy(szCustomerData + 12 - nLen, pCustomerData, nLen);

			szCustomerData[12] = 0x00;
			pCustomerData = szCustomerData;
		}
		else //SP不处理
		{
			;
		}		
	}


	DWORD dwAttr = 0;
	PINKEYDETAIL stDetail;
	nRet = m_pPinpad->XFS_GetKeyDetail(wKeyId, &stDetail);
	if(nRet == 0)
	{
		dwAttr = stDetail.dwKeyAttr;
	}

	WORD wOutLen = 0;
	if(dwAlgorithm == WFS_PIN_CRYPTDESECB || dwAlgorithm == WFS_PIN_CRYPTTRIDESECB || dwAlgorithm == WFS_PIN_CRYPTSM4 || (dwAttr & ATTR_SM4))
	{
		if(nRet = m_pPinpad->GetPinBlock(wKeyId, (BYTE)m_usPinLen, (EPINFORMAT)dwFormat, pCustomerData, (char*)PinBlock, wOutLen, bPadding, wEnKeyId))
		{
			RLog2(LOG_LEVEL_ERROR, "get pin block fail, nRet=[%d]", nRet);
			return Convert2SPError(nRet);
		}
	}
	else
	{
		RLog2(LOG_LEVEL_ERROR, "algorithm not supported");
		return WFS_ERR_PIN_ALGORITHMNOTSUPP;
	}

	m_XDataResult.Set((LPBYTE)PinBlock, wOutLen);
	m_usPinLen = 0;
	RLog2(LOG_LEVEL_COMMON, "End:");		
	return WFS_SUCCESS;
}

