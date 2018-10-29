/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   IBaseSqlite.h
* Description: the interface of sqlite, the database use UTF8 code
*
* Author:      liuguoxun
* Date:        2014-02-28 
*
*****************************************************************************/


#ifndef _ISQLITE_H_20140228
#define _ISQLITE_H_20140228

#include <IBaseSystem.h>

#define  SQLITE_MODULE_VER         PLATFORM_VER 


INTERFACE_I  ISqlite 
{
public:
	virtual void Lock(void) = 0;
	virtual void UnLock(void) = 0;


	virtual int  OpenDB(LPCTSTR szFile = NULL) = 0;
	virtual int  CloseDB(void) = 0;	
	virtual bool IsOpenDB(void) const = 0;


	virtual int  GetDBPath(LPTSTR pPath) = 0;
	virtual int  GetLastErrCode(void) = 0;
	virtual int  GetLastErrMsg(LPTSTR pMsg) = 0;
	virtual int  DBGetTime(LPTSTR pTime, int nNeedLen) = 0; //nNeedLen=8|10|12|19|23

	virtual int  BeginTransaction(void) = 0;
	virtual int  CommitTransaction(void) = 0;
	virtual int  RollbackTransaction(void) = 0;

	virtual bool IsTableExist(LPCTSTR pTableName) = 0;
	virtual int  CopyTable(LPCTSTR p2DbFile, LPCTSTR pTableName) = 0; //copy my(current open db) Table[TableName] to 2DbFile's Table[TableName]
	virtual int  JoinTable(LPCTSTR pFromDbFile, LPCTSTR pTableName) = 0; //join FromDbFile's Table[TableName] to my(current open db) Table[TableName]
	virtual int  JoinView(LPCTSTR pFromDbFile, LPCTSTR pTableName, LPCTSTR pViewName) = 0; //join FromDbFile's View[ViewName] to my(current open db) Table[TableName]
	virtual int  ExecSQLFile(LPCTSTR pFilePath) = 0;


	virtual int  ExecSQL(LPCSTR pAsciiSQL) = 0;
	virtual int  ExecSQL(LPCWSTR pWSQL) = 0;
	virtual int  PrepareSQL(LPCSTR pAsciiSQL) = 0;
	virtual int  PrepareSQL(LPCWSTR pWSQL) = 0;
	virtual int  Execte(void) = 0;
	virtual bool ExecStep(void) = 0;
	virtual int  BindInt32(int nIndex, int nValue) = 0;
	virtual int  BindInt64(int nIndex, long long llValue) = 0;
	virtual int  BindDouble(int nIndex, double dbValue) = 0;
	virtual int  BindNull(int nIndex) = 0;
	virtual int  BindString(int nIndex, LPCSTR pAscii) = 0;
	virtual int  BindString(int nIndex, LPCWSTR pWChar) = 0;
	virtual int  BindBlob(int nIndex, LPCSTR szValue, int nSize) = 0;
	virtual int  GetTypeOfIndex(int nIndex, int *_type) = 0;
	virtual int  GetInt32(int nIndex, int *_value) = 0;
	virtual int  GetInt64(int nIndex, long long *_value) = 0;
	virtual int  GetDouble(int nIndex, double *_value) = 0;
	virtual int  GetString(int nIndex, char *pAscii) = 0;
	virtual int  GetString(int nIndex, LPWSTR pWChar) = 0;
	virtual int  GetBlob(int nIndex, char *_blob, int *_size) = 0;
	virtual int  ClearBind(void) = 0;
	virtual int  ClearStmt(void) = 0;
};


class CSqliteMutex
{
public:
	explicit CSqliteMutex(ISqlite *p)
	{
		if(NULL != p)
		{
			m_p = p;
			m_p->Lock();
		}
		else
		{
			m_p = NULL;
		}
	}

	~CSqliteMutex()
	{
		if(NULL != m_p)
		{
			m_p->UnLock();
			m_p = NULL;
		}
	}


private:
	ISqlite *m_p;
};


extern "C" ISqlite* WINAPI InstanceISqlite();
extern "C" void WINAPI ReleaseISqlite(ISqlite *p);

typedef ISqlite* (WINAPI * ISQLITE_INSFUNC)();
typedef void (WINAPI * ISQLITE_DELFUNC)(ISqlite *);


#endif //_ISQLITE_H_20140228

