/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CCCB.cpp
* Description: extend class for CCB
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#include "CCCB.h"


CCCB::CCCB(IXFSSPManage *pManage, IXFSSPFace *pFace, IXFSPINBase *pBase)
	: CXFSPINEx(pManage, pFace, pBase)
{
	RLog2(LOG_LEVEL_DEBUG, "Enter");
}

CCCB::~CCCB()
{
	RLog2(LOG_LEVEL_COMMON, "Enter");
}

//===========================================================================//
//                          Base class virtual function                      //
//===========================================================================//
HRESULT CCCB::OnExecute(DWORD dwCommand, LPVOID lpCmdData, REQUESTID RequestID)
{	
	RLog2(LOG_LEVEL_DEBUG, "Start: dwCommand=%d RequestID=%d", dwCommand, RequestID);
	
	HRESULT hRes = WFS_SUCCESS;
	if(dwCommand == WFS_CMD_PIN_INITIALIZATION)
	{
		ComeOnBaby();
	}
	else if(dwCommand == WFS_CMD_PIN_RESET)
	{
		hRes = Reset();
		//m_pSPKernel->SetResult();

		ComeOnBaby();
		return hRes;
	}
	
	SAFE_CALL(m_pSPFace, OnExecute(dwCommand, lpCmdData, RequestID));
}

//===========================================================================//
//                          Extend function                                  //
//===========================================================================//
HRESULT CCCB::Reset()
{
	RLog2(LOG_LEVEL_COMMON, "Enter");

	SAFE_CALL(m_pSPBase, Reset());
}

//===========================================================================//
//                          Decorator function                               //
//===========================================================================//
void CCCB::ComeOnBaby()
{
	RLog2(LOG_LEVEL_COMMON, "ComeOnBaby");
	//m_pPinpad->GetStatus();
}

