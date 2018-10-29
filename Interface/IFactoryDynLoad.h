/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IFactoryDynLoad.h
* Description: the factory interface for dynamic load DLL/SO     
*
* Author:      liuguoxun
* Date:        2015-11-09
*
*****************************************************************************/


#ifndef  _IFACTORYDYNLOAD_H_20151109
#define  _IFACTORYDYNLOAD_H_20151109

#include <IBaseSystem.h>


typedef LPVOID (WINAPI *FUNC_INS)();
typedef LPVOID (WINAPI *FUNC_INS2)(I32, LPVOID, LPCTSTR);
typedef LPVOID (WINAPI *FUNC_INS3)(I32, I32, LPCTSTR);
typedef LPVOID (WINAPI *FUNC_INS4)(I32, I32, LPCTSTR, I32);

typedef void   (WINAPI *FUNC_REL)(LPVOID);


INTERFACE_I IDynLoadFactory
{
public:
	virtual LPVOID Instance(LPCTSTR lpLibName, LPCSTR lpInsName) = 0;
	virtual LPVOID Instance(LPCTSTR lpLibName, LPCSTR lpInsName, I32 PA, LPVOID PB, LPCTSTR PC) = 0;
	virtual LPVOID Instance(LPCTSTR lpLibName, LPCSTR lpInsName, I32 PA, I32 PB, LPCTSTR PC) = 0;
	virtual LPVOID Instance(LPCTSTR lpLibName, LPCSTR lpInsName, I32 PA, I32 PB, LPCTSTR PC, I32 PD) = 0;

	virtual LPVOID GetFuncAddr(LPCTSTR lpLibName, LPCSTR lpFuncName) = 0;


	//don't call it becase factory will auto release all object at the end
	//you can call it to release object(p) at the halfway 
	virtual void Release(LPCTSTR lpLibName, LPCSTR lpRelName, LPVOID p) = 0;
};


extern "C" IDynLoadFactory * WINAPI InstanceDynLoadFactory();
extern "C" void WINAPI ReleaseDynLoadFactory();

#endif  //_IFACTORYDYNLOAD_H_20151109

