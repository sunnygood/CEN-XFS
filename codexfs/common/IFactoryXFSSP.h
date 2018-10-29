/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IFactoryXFSSP.h
* Description: the factory interface for all SP       
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/


#ifndef  _IFACTORYXFSSP_H_20151109
#define  _IFACTORYXFSSP_H_20151109

#include <IBaseSystem.h>


INTERFACE_I IXFSSPFactory
{
public:
	virtual HRESULT SPRun(LPCTSTR lpCmdLine) = 0;
};


extern IXFSSPFactory * InstanceSPFactory();
extern void ReleaseSPFactory();

#endif  //_IFACTORYXFSSP_H_20151109

