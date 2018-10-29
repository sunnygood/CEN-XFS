/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSPINETS.h
* Description: EPP sub class of PIN
*
* Author:      liuguoxun
* Date:        2015-11-09         
*
*****************************************************************************/

#ifndef _CXFSPINETS_H_20151109
#define _CXFSPINETS_H_20151109

#include "CXFSPINHSM.h"


class CXFSPINETS : public CXFSPINHSM
{
public:
	explicit CXFSPINETS();
	virtual ~CXFSPINETS();


protected: 
	//Info Commands
	//3.30
	virtual HRESULT GetLayout(LPWFSPINGETLAYOUT lpLayout);

protected: 
	//Execute Commands
	//3.30
	virtual HRESULT DefineLayout(LPWFSPINLAYOUT *lppLayout);
	virtual HRESULT StartAuthenticate(LPWFSPINSTARTAUTHENTICATE lpSA);
	virtual HRESULT Authenticate(LPWFSPINAUTHENTICATE lpAuth);
};

#endif	//_CXFSPINETS_H_20151109
