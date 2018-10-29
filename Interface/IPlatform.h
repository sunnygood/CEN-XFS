/*****************************************************************************
* Copyright(C) 2017, Corporation.
*
* File Name:   IPlatform.h
* Description: platform 
*
* Author:      liuguoxun
* Date:        2017-10-28       
*
*****************************************************************************/

#ifndef _IPLATFORM_H_20171028
#define _IPLATFORM_H_20171028

#include <errorcode.h>
#include <IBaseSystem.h>
#include <IBaseSqlite.h>
#include <IBaseAlg.h>
#include <IBasePort.h>
#include <IBaseLog.h>
#include "CAutoClass.h"


/****************************************************************************/

//Module parameter table name(database)
#define  ININAME_CONFIGPARA     _T("ConfigPara.ini")
#define  DBNAME_CONFIGPARA      _T("ConfigPara.db3")
#define  DBNAME_XFSSPSTRUCT     _T("XFSSPStruct.db3")

#define  TBNAME_BASESYSTEM      _T("BaseSystem")
#define  TBNAME_BASEPORT        _T("BasePort")
//#define  TBNAME_BASESQLITE      _T("BaseSqlite")
//#define  TBNAME_BASEALG         _T("BaseAlg")


#define  TBNAME_PINPAD          _T("ZTPinpad")

#define  TBNAME_XFSSPMANAGE     _T("XFSSPManage")
#define  TBNAME_XFSSPKERNEL     _T("XFSSPKernel")
#define  TBNAME_XFSSPI          _T("XFSSPI")
#define  TBNAME_XFSPINBASE      _T("XFSPINBase")

/****************************************************************************/

//Module name
#ifdef __ms_windows__
	#ifdef _UNICODE
	#define  NAME_BASESYSTEM        _T("BaseSystemU.dll")
	#define  NAME_BASESQLITE        _T("BaseSqliteU.dll")
	#define  NAME_BASEALG           _T("BaseAlgU.dll")
	#define  NAME_BASEPORT          _T("BasePortU.dll")

	#define  NAME_PINPAD            _T("ZTPinpadU.dll")

	#define  NAME_XFSSPMANAGE       _T("XFSSPManageU.dll")
	#define  NAME_XFSSPKERNEL       _T("XFSSPKernelU.dll")
	#define  NAME_XFSSPI            _T("XFSSPIU.dll")
	#define  NAME_XFSPINBASE        _T("XFSPINBaseU.dll")
	#else
	#define  NAME_BASESYSTEM        _T("BaseSystem.dll")
	#define  NAME_BASESQLITE        _T("BaseSqlite.dll")
	#define  NAME_BASEALG           _T("BaseAlg.dll")
	#define  NAME_BASEPORT          _T("BasePort.dll")

	#define  NAME_PINPAD            _T("ZTPinpad.dll")

	#define  NAME_XFSSPMANAGE       _T("XFSSPManage.dll")
	#define  NAME_XFSSPKERNEL       _T("XFSSPKernel.dll")
	#define  NAME_XFSSPI            _T("XFSSPI.dll")
	#define  NAME_XFSPINBASE        _T("XFSPINBase.dll")
	#endif //_UNICODE
#elif defined(__gg_android__)
	#define  NAME_BASESYSTEM        _T("BaseSystem.so")
	#define  NAME_BASESQLITE        _T("BaseSqlite.so")
	#define  NAME_BASEALG           _T("BaseAlg.so")
	#define  NAME_BASEPORT          _T("BasePort.so")

	#define  NAME_PINPAD            _T("ZTPinpad.so")

	#define  NAME_XFSSPMANAGE       _T("XFSSPManage.so")
	#define  NAME_XFSSPKERNEL       _T("XFSSPKernel.so")
	#define  NAME_XFSSPI            _T("XFSSPI.so")
	#define  NAME_XFSPINBASE        _T("XFSPINBase.so")
#else
	#define  NAME_BASESYSTEM        _T("BaseSystem.so.3")
	#define  NAME_BASESQLITE        _T("BaseSqlite.so.3")
	#define  NAME_BASEALG           _T("BaseAlg.so.3")
	#define  NAME_BASEPORT          _T("BasePort.so.3")

	#define  NAME_PINPAD            _T("ZTPinpad.so.3")

	#define  NAME_XFSSPMANAGE       _T("XFSSPManage.so.3")
	#define  NAME_XFSSPKERNEL       _T("XFSSPKernel.so.3")
	#define  NAME_XFSSPI            _T("XFSSPI.so.3")
	#define  NAME_XFSPINBASE        _T("XFSPINBase.so.3")
#endif //__ms_windows__

/****************************************************************************/

//Module log filename
#define  LOG_BASESYSTEM         _T("BaseSystem.log")
#define  LOG_BASESQLITE         _T("BaseSqlite.log")
#define  LOG_BASEALG            _T("BaseAlg.log")
#define  LOG_BASEPORT           _T("BasePort.log")

#define  LOG_PINPAD             _T("ZTPinpad.log")

#define  LOG_XFSMANAGE          _T("XFSSPManage.log")
#define  LOG_XFSSPKERNEL        _T("XFSSPKernel.log")
#define  LOG_XFSSPI             _T("XFSSPI.log")
#define  LOG_XFSPIN             _T("XFSPIN.log")
#define  LOG_XFSVMD             _T("XFSVDM.log")

/****************************************************************************/

//Object instance and release function name(ANSI only)
#define  INS_NAME_DB            "InstanceISqlite"
#define  REL_NAME_DB            "ReleaseISqlite"
#define  INS_NAME_PORT          "InstanceIPort"
#define  REL_NAME_PORT          "ReleaseIPort"

#define  INS_NAME_PINPAD        "InstancePinpad"
#define  REL_NAME_PINPAD        "ReleasePinpad"

#define  INS_NAME_XFSMANAGE     "InstanceSPManage"
#define  REL_NAME_XFSMANAGE     "ReleaseSPManage"
#define  INS_NAME_XFSKERNEL     "InstanceSPKernel"
#define  REL_NAME_XFSKERNEL     "ReleaseSPKernel"
#define  INS_NAME_XFSPIN        "InstancePIN"
#define  REL_NAME_XFSPIN        "ReleasePIN"


#endif	//_IPLATFORM_H_20171028
