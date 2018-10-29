/*****************************************************************************
* Copyright(C) 1993-2012,XXX Corporation.
*
* File Name:   CPinpadFace.h
* Description: the business class of all the pinpad device, it base on IPinpad.h
*
* Author:      liuguoxun
* Date:        2012-05-28
* History 1: 
*
*****************************************************************************/


#include "CPinpadFace.h"


/*****************************************************************************
 * 函数说明: 
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
CPinpadFace::CPinpadFace(EPORT ePort, EExchange eType, EPIN_EXTEND eExtend)
	: CCommandBase(ePort, eType)
	, m_eExtend(eExtend)
{
	m_nChipSubType = 0;
	m_dwActiveKey = 0;
	m_dwActiveFDK = 0;

	memset(m_szEnlargeFile, 0, sizeof(TCHAR) * MAX_PATH);
	memset(m_szKeyCode, 0, sizeof(m_szKeyCode));
	memset(m_szKeyNameBuf, 0, MAX_MAP_KEY * (MAX_KEY_NAME + 1));
	memset(m_stArtKey, 0, sizeof(ARTKEY) * MAX_MAP_KEY);
}


/*****************************************************************************
 * 函数说明: 
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
CPinpadFace::~CPinpadFace()
{
}
