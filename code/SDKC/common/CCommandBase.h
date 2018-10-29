/*****************************************************************************
* Copyright(C) 1993-2012,ZT Corporation.
*
* File Name:   CCommandBase.h
* Description: the base class of all the command device
*
* Author:      liuguoxun
* Date:        2012-04-18
* History 1: 
*
*****************************************************************************/


#ifndef COMMANDBASE_H_20140513
#define COMMANDBASE_H_20140513


#include "IBasePort.h"
#include "IBaseLog.h"


#define   MAX_CMD_NUM    256 //0x0 - 0xFF
#define   MAX_CMD_DES     64 //command remarks


extern TCHAR g_cRunPath[MAX_PATH];


class CCommandBase
{
public:
	//通讯控制函数
	virtual long Open(LPTSTR lpDesc);
	virtual long ReOpen(DWORD dwAfterTime = 0);
	virtual bool IsOpen(void) const;
	virtual long Close(void);
	virtual long SetBaudrate(DWORD dwBaudrate, BOOL bOnlyPort = 0);
	virtual DWORD GetCurBaudrate(void);
	virtual long SendData(const char *pData, DWORD dwDataLen, DWORD dwTimeout);
	virtual long ReadData(char *pData, DWORD &dwInOutLen, DWORD dwTimeout);
	virtual long Cancel(void);
	virtual void *GetPortPointer(void) { return m_pPort; }


	//固件下载
	virtual DWORD OpenFile(char *pOutBuf, LPCTSTR lpFileName, LPCSTR lpDecryptKey = 0, WORD wKeyLen = 0); 
	virtual long ChipCommunicate(BYTE Mode, char *pChipInfo = 0);
	virtual long DownProgram(LPCSTR pcFileData, long nFileLen, BYTE Mode, void *hWnd = 0);



	//基本操作
	virtual long Init(long nMode = 0);										  //设备初始化(清除数据)
	virtual long Reset(void);											      //设备复位
	virtual long SelfTest(long iComponent = 0);							      //设备进行自检
	virtual long GetStatus(void);										      //获取设备状态
	virtual long GetCapabilities(void *lpvCap);							      //获取设备能力
	virtual long Execute(long iCmdIndex, char *pcBuffer, long &iBufOrRetLen); //执行命令
	virtual long GetLastErrorCode(void);	    					          //获取错误码及错误码解释
	virtual char *GetErrorInfo(long iCode);				                      //取错误码解释
	virtual long GetHardwareID(char *pcModelId);						      //读硬件ID
	virtual long GetHardwareSN(char *pcSn);						              //读硬件生产序列号	
	virtual long GetHardwareVersion(char *pcVersion);				          //读硬件版本号
	virtual long GetLoaderVersion(char *pcVersion);
	virtual long GetSoftVersion(char *pcSoftVersion, void *hModule = 0);      //读软件版本号



public:
	CCommandBase(EPORT ePort, EExchange eType);
    virtual ~CCommandBase();



protected:
	typedef enum
	{
		  F_NORMAL        = 0x0   //正常
	    , FS_NO_HEAD      = 0x1	  //发送无报文头
		, FS_NO_END       = 0x2	  //发送无报文尾
		, FS_NO_CHECK     = 0x4	  //发送报文无校验码
		, FS_ADD_MAC_KBPK = 0x8	  //发送报文增加KBPK为密钥的MAC校验
		, FS_ADD_MAC_UAK  = 0x10  //发送报文增加UAK为密钥的MAC校验

		, FR_NO_HEAD      = 0x100 //接收无报文头
		, FR_NO_END       = 0x200 //接收无报文尾
		, FR_NO_CHECK     = 0x400 //接收报文无校验码
	}EFORMAT;

	typedef struct stCommand
	{
		BYTE    ucIndex;          // 命令索引(花瓶)
		BYTE    ucCommand;        // 命令字
		WORD    wParaLen;         // 参数长度，0xFFFF表示非定长(此时长度在参数的前2个字节，低位在前)			
		DWORD   dwTimeOut;        // 命令超时时间	
		BYTE    ucMaxCnt;         // 命令最大执行次数
		EFORMAT eFormat;          // 命令格式
		char    cDes[MAX_CMD_DES];// 命令描述
	}STCOMMAND, *PSTCOMMAND;
   

	//注册命令
	long RegisterCommand(BYTE  ucIndex, BYTE ucCommand, WORD  wParaLen, DWORD dwTimeOut, BYTE ucMaxCnt, EFORMAT eFormat, LPCSTR pDes = NULL);
	void UnRegisterAllCommand(void);


protected:
	virtual long PackData(PSTCOMMAND pstCmd, LPCSTR pcData, WORD wDataLen, char *pcPackData);//打包数据(HEX格式、非拆分字符串格式传入)
	virtual long UnPackData(PSTCOMMAND pstCmd, LPCSTR pcData, WORD wDataLen, char *pcRetData, long &iBufOrRetLen); //数据解包(HEX格式、非拆分字符串格式输出)


protected:
	BYTE   m_CmdSeq;	  //命令流水号
    BOOL   m_bCmdSeqAdd;  //命令流水号是否增加
	long   m_iErrorCode;  //错误码

	IPort *m_pPort;
    PSTCOMMAND m_apCommand[MAX_CMD_NUM]; //命令类指针的指针数组
};


#endif //COMMANDBASE_H_20140513

