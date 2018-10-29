/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSPINETS.cpp
* Description: EPP sub class of PIN
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#include "CXFSPINETS.h"


CXFSPINETS::CXFSPINETS()
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
}

CXFSPINETS::~CXFSPINETS()
{
	RLog2(LOG_LEVEL_COMMON, "Enter");
}

//===========================================================================//
//                          GetInfo Commands                                 //
//===========================================================================//

HRESULT CXFSPINETS::GetLayout(LPWFSPINGETLAYOUT lpLayout)
{
	if(NULL == lpLayout)
	{
		return WFS_ERR_INVALID_POINTER;
	}

	DWORD dwEntryMode = lpLayout->dwEntryMode;
	if(dwEntryMode == WFS_PIN_LAYOUT_DATA)
	{
		m_CLayout.Add(dwEntryMode, 200, 400, 60, 60, WFS_PIN_FLOATX, (LPWFSPINFK*)m_PressKey.m_vDataKeys.data());
	}
	else if(dwEntryMode == WFS_PIN_LAYOUT_PIN)
	{
		m_CLayout.Add(dwEntryMode, 200, 400, 60, 60, WFS_PIN_FLOATX, (LPWFSPINFK*)m_PressKey.m_vPinKeys.data());
	}
	else if(dwEntryMode == WFS_PIN_LAYOUT_SECURE)
	{
		m_CLayout.Add(dwEntryMode, 200, 400, 60, 60, WFS_PIN_FLOATX, (LPWFSPINFK*)m_PressKey.m_vSecureKeys.data());
	}
	else
	{
		return WFS_ERR_PIN_MODENOTSUPPORTED;
	}

	return WFS_SUCCESS;
}

//===========================================================================//
//                          Exectue Commands                                 //
//===========================================================================//
HRESULT CXFSPINETS::DefineLayout(LPWFSPINLAYOUT *lppLayout)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINETS::StartAuthenticate(LPWFSPINSTARTAUTHENTICATE lpSA)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

HRESULT CXFSPINETS::Authenticate(LPWFSPINAUTHENTICATE lpAuth)
{
	return WFS_ERR_UNSUPP_COMMAND;
}

