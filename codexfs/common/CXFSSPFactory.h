/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CXFSSPFactory.h
* Description: the factory class for all SP       
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/

#ifndef  _CXFSSPFACTORY_H_20151109
#define  _CXFSSPFACTORY_H_20151109

#include <IXFSSPManage.h>
#include <IXFSSPFace.h>
#include <IFactoryXFSSP.h>
#include <IPlatform.h>
#include <CSTLUnit.h>
#include "CDynLoadFactory.h"


class CXFSSPFactory : public IXFSSPFactory
{
public:
	virtual HRESULT SPRun(LPCTSTR lpCmdLine);


	explicit CXFSSPFactory();
	virtual ~CXFSSPFactory();


protected:	
	static DWORD CALLBACK SubThreadEntry(LPVOID pParam);

	long ParseCmdLine(LPCTSTR lpCmdLine);
	long StartSP(LPCTSTR lpServiceName);
	long ThreadSPRun(DWORD dwIndex);


private:
	IDynLoadFactory *m_pDLFactory;
	IXFSSPManage    *m_pSPManage;

	std::vector<std::basic_string<TCHAR>> m_vServiceName;
	typedef std::vector<std::basic_string<TCHAR>>::iterator VSIT;
};

#endif  //_CXFSSPFACTORY_H_20151109

