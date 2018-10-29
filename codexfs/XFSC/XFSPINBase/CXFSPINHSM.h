/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSPINHSM.h
* Description: EPP sub class of PIN
*
* Author:      liuguoxun
* Date:        2015-11-09         
*
*****************************************************************************/

#ifndef _CXFSPINHSM_H_20151109
#define _CXFSPINHSM_H_20151109

#include "CXFSPINBase.h"


class CXFSPINHSM : public CXFSPINBase
{
public:
	explicit CXFSPINHSM();
	virtual ~CXFSPINHSM();


protected: 
	//Info Commands	
	virtual HRESULT FuncKeyDetail(LPULONG lpulFDKMask);
	virtual HRESULT SecureKeyDetail();


protected: 
	//Execute Commands
	virtual HRESULT GetData(LPWFSPINGETDATA lpPinGetData);
	virtual HRESULT GetPin(LPWFSPINGETPIN lpGetPin);
	virtual HRESULT GetPinBlock(LPWFSPINBLOCK lpPinBlock);
	virtual HRESULT LocalDes(LPWFSPINLOCALDES lpLocalDes);
	virtual HRESULT CreateOffset(LPWFSPINCREATEOFFSET lpPinOffset);
	virtual HRESULT LocalEurocheque(LPWFSPINLOCALEUROCHEQUE lpLocalEurocheque);
	virtual HRESULT LocalVisa(LPWFSPINLOCALVISA lpLocalVisa);
	virtual HRESULT SecureKeyEntry(LPWFSPINSECUREKEYENTRY lpSecureKeyEntry);
	//3.10
	virtual HRESULT KeyPressBeep(LPWORD lpwMode);
	//3.30
	virtual HRESULT GetPinBlockEx(LPWFSPINBLOCKEX lpPinBlockEx);
};

#endif	//_CXFSPINHSM_H_20151109
