/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IFactoryInsClass.h
* Description: the factory interface for instance class, it's use at module inner      
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/


#ifndef  _IFACTORYINSCLASS_H_20151109
#define  _IFACTORYINSCLASS_H_20151109

#include <IBaseSystem.h>

//first do below at your project factory
//typedef  IPinpad        InterfacePtr;
//typedef  CPinpadBase    ObjectBase;


INTERFACE_I IFactoryInsClass
{
public:
	virtual InterfacePtr * Instance() = 0;
	virtual InterfacePtr * Instance(I32 PA, LPVOID PB, LPCTSTR PC) = 0;
	virtual InterfacePtr * Instance(I32 PA, I32 PB, LPCTSTR PC) = 0; //SDK, SP, API
	virtual InterfacePtr * Instance(I32 PA, I32 PB, LPCTSTR PC, I32 PD) = 0; //SDK

	virtual void Release(LPVOID p) = 0;
};


#endif  //_IFACTORYINSCLASS_H_20151109

