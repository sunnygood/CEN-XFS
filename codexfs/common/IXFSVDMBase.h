/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IXFSVDMBase.h
* Description: base interface for XFS VDM
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#ifndef _IXFSVDMBASE_H_20151109
#define _IXFSVDMBASE_H_20151109

#include <IBaseSystem.h>
#include <CENXFSVDM.h>


INTERFACE_I  IXFSVDMBase
{
public:
	//Info Commands
	virtual HRESULT Status() = 0;
	virtual HRESULT Capabilities() = 0;
	//3.10/3.20
	virtual HRESULT ActiveInterface() = 0;


public: 
	//Execute Commands
	virtual HRESULT EnterModeReq(REQUESTID RequestID) = 0;
	virtual HRESULT EnterModeAck(REQUESTID RequestID) = 0;
	virtual HRESULT ExitModeReq(REQUESTID RequestID) = 0;
	virtual HRESULT ExitModeAck(REQUESTID RequestID) = 0;
	//3.10/3.20
	virtual HRESULT SetActiveInterface(LPWFSVDMACTIVEINTERFACE lpActiveInterface) = 0;


public:
	//Events
	virtual void FireEnterModeReq() = 0;
	virtual void FireExitModeReq() = 0;
	virtual void FireModeEntered() = 0;
	virtual void FireModeExited() = 0;
	//3.10/3.20
	virtual void FireInterfaceChanged(WORD wActiveInterface) = 0;
};

#endif	//_IXFSVDMBASE_H_20151109
