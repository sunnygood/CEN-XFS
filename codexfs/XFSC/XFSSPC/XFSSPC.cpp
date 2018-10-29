/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSSPC.cpp
* Description: the execute module for all SP        
*
* Author:      liuguoxun
* Date:        2015-11-09    
*
*****************************************************************************/

#include "IFactoryXFSSP.h"


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	IXFSSPFactory *p = InstanceSPFactory();
	if(NULL == p)
	{
		return -1;
	}
	else
	{
		p->SPRun(lpCmdLine);
		ReleaseSPFactory();
		return 0;
	}
}

