/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IXFSEvent.h
* Description: the standard interface of XFS Event(include message)
*
* Author:      liuguoxun
* Date:        2015-12-18
*
*****************************************************************************/


#ifndef  _IXFSEVENT_H_20151218
#define  _IXFSEVENT_H_20151218

#include "XFSAPI.h"
#include "IBaseSystem.h"


//used by function FormatOut
enum COLOR_TYPE
{
	  TYPE_OK         =  0L       //blue  (HRESULT = 0)
	, TYPE_NORMAL     =  1L       //black (HRESULT > 0)
	, TYPE_ERROR      = -1L       //red   (HRESULT < 0)
	, TYPE_ANNOTATION = 0xFFFFFFL //green
};

/********************************************************************************/

INTERFACE_I  IXFSEvent
{
public:
	//overwrite these function only, they will be auto call
	virtual HRESULT OnComplete(LPWFSRESULT lpResult, UINT nMsgID) = 0;
	virtual HRESULT OnEvent(LPWFSRESULT lpResult, UINT nMsgID) = 0;


	//Auto format then call this function output to user
	virtual BOOL FormatOut(long nColorType, LPCTSTR lpFormat) = 0;
};


#endif  //_IXFSEVENT_H_20151218
