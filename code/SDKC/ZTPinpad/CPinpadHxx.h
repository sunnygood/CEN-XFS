/*****************************************************************************
* Copyright(C) 1993-2015,XXX Corporation.
*
* File Name:   CPinpadHxx.h
* Description: the class of Hxx, for PCI3.x and PCI4.x
*
* Author:      liuguoxun
* Date:        2015-09-22
* History 1: 
*
*****************************************************************************/

#ifndef CPINPADHXX_H_20150922
#define CPINPADHXX_H_20150922


#include "CPinpadBase.h"


/*****************************************************************************/


class CPinpadHxx : public CPinpadBase
{
public:
	explicit CPinpadHxx(EPORT ePort, EExchange eType, EPIN_EXTEND eExtend);
	virtual ~CPinpadHxx();

	virtual long RegisterMyFeatures(BOOL bAll = TRUE); //注册自己的特性
	


protected:	
	virtual long Inner_GetKeyboardCode(char *lpKeyboardCodes, long &nOutLen, ENTRYMODE eMode = ENTRY_MODE_TEXT);
	virtual long Inner_ReadKeyAttr(WORD wKeyId, LPDWORD lpKeyAttr, EKCVMODE eKCV, char *lpKCVRet, WORD wVI);
	virtual long GetFirmwareLog(EFWLOG eMode, char *pOut, long &nOutLen);
	virtual long GetPower(long *pPower);
};


#endif //CPINPADHXX_H_20150922

