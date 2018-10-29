/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CCCB.h
* Description: extend class for CCB
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#ifndef _CCCB_H_20151109
#define _CCCB_H_20151109

#include "CXFSPINEx.h"


/********************************************************************************/

class CCCB : public CXFSPINEx
{
public:
	explicit CCCB(IXFSSPManage *pManage, IXFSSPFace *pFace, IXFSPINBase *pBase);
	virtual ~CCCB();


public:
	virtual HRESULT OnExecute(DWORD dwCommand, LPVOID lpCmdData, REQUESTID RequestID);

public:
	virtual HRESULT Reset();


public:
	void ComeOnBaby();
};

#endif	//_CCCB_H_20151109
