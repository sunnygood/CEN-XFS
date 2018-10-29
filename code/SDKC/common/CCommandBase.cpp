/*****************************************************************************
* Copyright(C) 1993-2012,ZT Corporation.
*
* File Name:   CCommandBase.cpp
* Description: the base class of all the command device       
*
* Author:      liuguoxun
* Date:        2012-04-18
* History 1: 
*
*****************************************************************************/


#include "CCommandBase.h"


TCHAR g_cRunPath[MAX_PATH] = {0};



/*****************************************************************************
 * 函数说明: 
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
CCommandBase::CCommandBase(EPORT ePort, EExchange eType)
{
	m_CmdSeq     = 0;	
	m_iErrorCode = 0;
	m_bCmdSeqAdd = FALSE;

	for(int i = MAX_CMD_NUM - 1; i >= 0; i--)
	{
		m_apCommand[i] = NULL;
	}

	m_pPort   = InstanceIPort(ePort, eType, NULL);
}


/*****************************************************************************
 * 函数说明: 
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
CCommandBase::~CCommandBase()
{
	UnRegisterAllCommand();

	if(NULL != m_pPort)
	{
		ReleaseIPort(m_pPort);
		m_pPort = NULL;
	}
}


/*****************************************************************************
 * 函数说明: 注册命令
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::RegisterCommand(BYTE  ucIndex, BYTE ucCommand, WORD  wParaLen, DWORD dwTimeOut, BYTE ucMaxCnt, EFORMAT eFormat, LPCSTR pDes)
{
	if(ucIndex >= MAX_CMD_NUM) //检查是否超出允许注册的命令数
	{
		return EC_FULL_NOW;
	}

	stCommand* pCommand = new stCommand();
	if(NULL != pCommand)
	{
		pCommand->ucIndex        = ucIndex;
		pCommand->ucCommand      = ucCommand;
		pCommand->wParaLen       = wParaLen;
		pCommand->dwTimeOut      = dwTimeOut;
		pCommand->ucMaxCnt       = ucMaxCnt;
		pCommand->eFormat        = eFormat;
		if(NULL != pDes)
		{
			STRCPY_S(pCommand->cDes, MAX_CMD_DES - 1, pDes);
		}

		if(NULL != m_apCommand[ucIndex])
		{
			DELETE_P(m_apCommand[ucIndex]);
		}

		m_apCommand[ucIndex]     = pCommand;
		return EC_SUCCESS;
	}
	else
	{
		return EC_ALLOC_FAILED;
	}	
}


/*****************************************************************************
 * 函数说明: 注销所有命令
 * 参数定义: 无
 * 返 回 值: 无 
 *****************************************************************************/
void CCommandBase::UnRegisterAllCommand(void)
{
	for(int i = MAX_CMD_NUM - 1; i >= 0; i--)
	{
		DELETE_P(m_apCommand[i]);
	}
}


/*****************************************************************************
 * 函数说明: 数据打包
 * 参数定义: pstCmd:命令结构指针，pcData:命令参数，iDataLen：参数长度，pcPackData：输出的打包数据
 * 返 回 值: 数据包长度 
 *****************************************************************************/
long CCommandBase::PackData(PSTCOMMAND pstCmd, LPCSTR pcData, WORD wDataLen, char *pcPackData)
{
	return 0;
}


/*****************************************************************************
 * 函数说明: 数据解包
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::UnPackData(PSTCOMMAND pstCmd, LPCSTR pcData, WORD wDataLen, char *pcRetData, long &iBufOrRetLen)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 
 * 参数定义: 
 * 返 回 值: 错误码 
 *****************************************************************************/
long CCommandBase::Open(LPTSTR lpDesc)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 取当前串口波特率
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
DWORD CCommandBase::GetCurBaudrate()
{
	if(NULL == m_pPort || (!m_pPort->IsOpen()))
	{
		return 0;
	}

    return (m_pPort->GetBaudrate());
}


/*****************************************************************************
 * 函数说明: 重新打开串口，参数为当前设置
 * 参数定义: dwAfterTime: 关闭串口后多少毫秒再重新打开
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::ReOpen(DWORD dwAfterTime)
{
	if(NULL == m_pPort)
	{
		return EC_POINTER_NULL;
	}

    return m_pPort->ReOpen(dwAfterTime);
}


/*****************************************************************************
 * 函数说明: 检测串口是否打开
 * 参数定义: 无
 * 返 回 值:  
 *****************************************************************************/
bool CCommandBase::IsOpen(void) const
{
	if(NULL == m_pPort)
	{
		return false;
	}

    return m_pPort->IsOpen();
}


/*****************************************************************************
 * 函数说明: 关闭串口
 * 参数定义: 无
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::Close(void)
{
	if(NULL == m_pPort)
	{
		return EC_POINTER_NULL;
	}

	return m_pPort->Close();
}


/*****************************************************************************
 * 函数说明: 取消接收数据
 * 参数定义: 无
 * 返 回 值: 查看错误码表 
 *****************************************************************************/
long CCommandBase::Cancel(void)
{
	if(NULL == m_pPort)
	{
		return EC_POINTER_NULL;
	}

	return m_pPort->Cancel();
}


/*****************************************************************************
 * 函数说明:
 * 参数定义:
 * 返 回 值:
 *****************************************************************************/
long CCommandBase::SendData(const char *pData, DWORD dwDataLen, DWORD dwTimeout)
{
	if(NULL == m_pPort || NULL == pData)
	{
		return EC_POINTER_NULL;
	}

	return m_pPort->SendData(pData, dwDataLen, dwTimeout);
}


/*****************************************************************************
 * 函数说明: 从串口接收数据
 * 参数定义: dwTimeOut：超时时间（毫秒）
 * 返 回 值: 查看错误码表 
 *****************************************************************************/
long CCommandBase::ReadData(char *pData, DWORD &dwInOutLen, DWORD dwTimeout)
{
	if(NULL == m_pPort || NULL == pData)
	{
		return EC_POINTER_NULL;
	}

	long iRet = 0;
	DWORD dwRead = 0;
	char szPara[MAX_PACKET_LEN] = {0};

	iRet = m_pPort->ReceiveData(szPara, dwInOutLen, &dwRead, dwTimeout);
	if(dwRead > 0)
	{
		memcpy(pData, szPara, dwRead);
	}

	dwInOutLen = dwRead;
	return iRet;
}


/*****************************************************************************
 * 函数说明: 设置波特率(硬件及串口)
 * 参数定义: dwBaudrate: 新波特率
 * 返 回 值: 查看错误码表 
 *****************************************************************************/
long CCommandBase::SetBaudrate(DWORD dwBaudrate, BOOL bOnlyPort)
{
	if(NULL == m_pPort)
	{
		return EC_POINTER_NULL;
	}

	long iRet = 0;

	if(bOnlyPort)
	{
		iRet = m_pPort->SetBaudrate(dwBaudrate);
	}
	else
	{
		iRet = EC_UNSUPPORT;
	}

	return iRet;
}


/*****************************************************************************
 * 函数说明: 获取固件文件数据(返回文件长度)
 * 参数定义: 
 * 返 回 值: 文件长度 
 *****************************************************************************/
DWORD CCommandBase::OpenFile(char *pOutBuf, LPCTSTR lpFileName, LPCSTR lpDecryptKey, WORD wKeyLen)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 检测芯片/与芯片交互
 * 参数定义: 
 * 返 回 值: 
 *****************************************************************************/
long CCommandBase::ChipCommunicate(BYTE Mode, char *pChipInfo)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 下载固件程序
 * 参数定义:
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::DownProgram(LPCSTR pcFileData, long nFileLen, BYTE Mode, void *hWnd)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 设备初始化(同时会清除所有密钥并参数复位)
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::Init(long nMode)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 设备复位(相当于重新上电)
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::Reset(void)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 设备进行自检
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::SelfTest(long iComponent)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 获取设备状态
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::GetStatus(void)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 获取设备能力
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::GetCapabilities(void *lpvCap)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 读硬件ID
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::GetHardwareID(char *pcModelId)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 读硬件生产序列号
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::GetHardwareSN(char *pcSn)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 读硬件版本号
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::GetHardwareVersion(char *pcVersion)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: Loader(STM)版本号
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::GetLoaderVersion(char *pcVersion)
{
	return EC_UNSUPPORT;
}

	
/*****************************************************************************
 * 函数说明: 读软件版本号
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::GetSoftVersion(char *pcSoftVersion, void *hModule)
{
	TCHAR cFileName[MAX_PATH] = {0};

	#ifdef __ms_windows__
	GetModuleFileName((HMODULE)hModule, cFileName, sizeof(cFileName));
	#endif

	return GetFileVersion(cFileName, pcSoftVersion);
}


/*****************************************************************************************
 * 函数说明: 执行命令
 * 参数定义: iCmdIndex:命令索引号，pcBuffer：[IN]命令参数 [OUT]返回数据
 *           iBufOrRetLen：[IN]缓冲区长度 [OUT]返回数据长度
 * 返 回 值:  
 *****************************************************************************************/
long CCommandBase::Execute(long iCmdIndex, char *pcBuffer, long &iBufOrRetLen)
{
	return EC_UNSUPPORT;
}


/*****************************************************************************
 * 函数说明: 获取错误码及错误码解释
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
long CCommandBase::GetLastErrorCode(void)
{
	return m_iErrorCode;
}


/*****************************************************************************
 * 函数说明: 取错误码解释
 * 参数定义: 
 * 返 回 值:  
 *****************************************************************************/
char * CCommandBase::GetErrorInfo(long iCode)
{
	switch(iCode)
	{
		case EC_SUCCESS:
		{
			return("Success.");
		}
		case EC_OPEN_FAILED:
		{
			return("Open port failed.");
		}
		case EC_INVALID_PORT:
		{
			return("Invalid port.");
		}
		case EC_INVALID_PARA:
		{
			return("Invalid parameter.");
		}
		case EC_INVALID_DATA:
		{
			return("Invalid data.");
		}
		case EC_SEND_FAILED:
		{
			return("Send data failed.");
		}
		case EC_RECEIVED_FAILED:
		{
			return("Received data failed.");
		}
		case EC_USER_CANCEL:
		{
			return("User cancel all operate.");
		}
		case EC_DATA_TOOLONG:
		{
			return("Data is too long.");
		}
		case EC_NAK_RECEIVED:
		{
			return("Received NAK.");
		}
		case EC_READ_TIMEOUT:
		{
			return("Read data timeout.");
		}
		case EC_WRITE_TIMEOUT:
		{
			return("Write data timeout.");
		}
		case EC_WAITEVENT_FAILED:
		{
			return("Wait event failed.");
		}
		case EC_SET_FAILED:
		{
			return("Set parameter failed.");
		}
		case EC_STEP_ERROR:
		{
			return("Step error.");
		}
		case EC_POINTER_NULL:
		{
			return("Pointer is NULL.");
		}
		case EC_FULL_NOW:
		{
			return("Queue or space or other full now.");
		}
		case EC_NET_ERROR:
		{
			return("Net error.");
		}
		case EC_INVALID_FILE:
		{
			return("Invalid file.");
		}
		case EC_TEST_MODE:
		{
			return("Test mode.");
		}
		case EC_EXIT:
		{
			return("Process or thread exit.");
		}
		case EC_ALLOC_FAILED:
		{
			return("Alloc failed.");
		}
		case EC_TYPE_UNMATCH:
		{
			return("Type unmatch.");
		}
		case EC_RETURN_FAILED:
		{
			return("Return failed.");
		}
		case EC_SERIOUS_ERROR:
		{
			return("Serious error.");
		}	
		case EC_UNSUPPORT:
		{
			return("Unsupport.");
		}
		default:
		{
			return("Undefined error code.");
		}
	}

	return EC_SUCCESS;
}

