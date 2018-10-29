/*****************************************************************************
* Copyright(C) 1993-2012,SZZT Corporation.
*
* File Name:   ModulePinpad.cpp 
* Description: Pinpad module entry       
*
* Author:      liuguoxun
* Date:        2012-04-05
* History 1: 
*
*****************************************************************************/

#include "CPinpadBase.h"
#include "CPinpadEPP.h"
#include "CPinpadEPP_R.h"
#include "CPinpadVISA.h"
#include "CPinpadVISA_R.h"
#include "CPinpadWOSA.h"
#include "CPinpadWOSA_R.h"
#include "CPinpadHxx.h"
#include "CEccPCI.h"
#include "CEccVISA.h"
#include "CPinpadSimulate.h"
#include "CDynLoadFactory.h"


typedef  IPinpad        InterfacePtr;
typedef  CPinpadBase    ObjectBase;
#include "CInsClassFactory.h"


class CMyFactory : public CInsClassFactory
{
public:
	InterfacePtr * Instance(I32 PA, I32 PB, LPCTSTR PC, I32 PD)
	{
		EPORT ePort = static_cast<EPORT>(PA);
		EPIN_TYPE eType = static_cast<EPIN_TYPE>(PB);
		EPIN_EXTEND eExtend = eEX_1_2;

		if(PD & eEX_SAVE2EPP)
		{
			if(PD & eEX_ENLARGE_KEY)
			{
				eExtend = eEX_1_2_8;
			}
			else if(PD & eEX_PC_KB)
			{
				eExtend = eEX_1_4_8;
			}
			else
			{
				eExtend = eEX_1_8;
			}
		}
		else
		{
			if(PD & eEX_ENLARGE_KEY)
			{
				eExtend = eEX_1_2;
			}
			else if(PD & eEX_PC_KB)
			{
				eExtend = eEX_1_4;
			}
			else
			{
				eExtend = eEX_STRING_NAME;
			}
		}

		#ifndef __ms_windows__
		GetRunPath(g_cRunPath, MAX_PATH, NULL);
		#endif

		TCHAR cDLLVer[64] = {0};
		TCHAR cFileName[MAX_PATH] = {0};
		STRCPY_S(cFileName, MAX_PATH, g_cRunPath);
		STRCAT_S(cFileName, MAX_PATH, NAME_PINPAD);
		GetFileVersion(cFileName, cDLLVer);
		RLog2(LOG_LEVEL_DATA, _T("%s Ver=%s"), cFileName, cDLLVer);

		STRCPY_S(cFileName, MAX_PATH, g_cRunPath);
		STRCAT_S(cFileName, MAX_PATH, NAME_BASEPORT);
		GetFileVersion(cFileName, cDLLVer);
		RLog2(LOG_LEVEL_DATA, _T("%s Ver=%s"), cFileName, cDLLVer);

		STRCPY_S(cFileName, MAX_PATH, g_cRunPath);
		STRCAT_S(cFileName, MAX_PATH, NAME_BASEALG);
		GetFileVersion(cFileName, cDLLVer);
		RLog2(LOG_LEVEL_DATA, _T("%s Ver=%s"), cFileName, cDLLVer);

		STRCPY_S(cFileName, MAX_PATH, g_cRunPath);
		STRCAT_S(cFileName, MAX_PATH, NAME_BASESYSTEM);
		GetFileVersion(cFileName, cDLLVer);
		RLog2(LOG_LEVEL_DATA, _T("%s Ver=%s"), cFileName, cDLLVer);

		if(NULL == PC)
		{
			RLog2(LOG_LEVEL_DATA, "ePort=0x%X, eType=0x%X, eExtend=0x%X, lpDesc=NULL", ePort, PB, eExtend);
		}
		else
		{
			RLog2(LOG_LEVEL_DATA, "ePort=0x%X, eType=0x%X, eExtend=0x%X, lpDesc=%s", ePort, PB, eExtend, PC);
		}

		ObjectBase *_instance = NULL;
		EPIN_TYPE _type = ePIN_UNKNOWN;
		WORD wSubType = SUBVER_NONE;
	
		if(eType == ePIN_UNKNOWN)
		{
			RLog2(LOG_LEVEL_DATA, "Auto detect pinpad......");
			_type = (EPIN_TYPE)AutoDetectType(ePort, PC, &wSubType);
		}
		else
		{
			_type = eType;
		}

		switch(_type)
		{
			case ePIN_EPP_BR:
			{
				_instance = new CPinpadEPP_R(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with EPP_BR command.");
				break;
			}
			case ePIN_EPP:		
			{
				_instance = new CPinpadEPP(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with EPP command.");
				break;
			}
			case ePIN_WOSA_3X:			
			{
				_instance = new CPinpadWOSA_R(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with WOSA_3X command.");
				break;
			}
			case ePIN_WOSA:
			{
				_instance = new CPinpadWOSA(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with WOSA command.");
				break;
			}
			case eECC_VISA:
			{
				_instance = new CEccVISA(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with ECC_VISA command.");
				break;
			}
			case ePIN_VISA_3X:
			{
				_instance = new CPinpadVISA_R(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with VISA_3X command.");
				break;
			}
			case ePIN_VISA:
			{
				_instance = new CPinpadVISA(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with VISA command.");
				break;
			}
			case eECC_PCI:
			{
				_instance = new CEccPCI(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with ECC_PCI command.");
				break;
			}		
			case ePIN_PCI_3X:
			{
				_instance = new CPinpadHxx(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with New PCI command.");
				break;
			}
			case ePIN_PCI:
			{
				_instance = new CPinpadBase(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with PCI command.");
				break;
			}
			case ePIN_SIMULATE:
			{
				_instance = new CPinpadSimulate(ePort, eEXCHANGE_FORMAT, eExtend);
				RLog2(LOG_LEVEL_DATA, "Instance pinpad with Simulate.");
				break;
			}
			default:
			{
				RLog2(LOG_LEVEL_DATA, "Error parameter of eType.");
				break;
			}
		}

		if(NULL != _instance)
		{
			AddObject(_instance);

			_instance->SetPinpadType(_type, wSubType);
			_instance->RegisterMyFeatures();
		}

		return (dynamic_cast<InterfacePtr*>(_instance));
	}

protected:
	I32 AutoDetectType(I32 PA, LPCTSTR PC, LPWORD lpSubType)
	{
		if(NULL == PC)
		{
			return ePIN_UNKNOWN;
		}

		long nRet = 0;
		WORD wSubType = 0;
		EPIN_TYPE ePinType = ePIN_UNKNOWN;

		CDynLoadFactory cDLF;
		IPort *pPort = (IPort *)cDLF.Instance(NAME_BASEPORT, INS_NAME_PORT, PA, eEXCHANGE_FORMAT, PC);
		if(NULL != pPort)
		{
			nRet = pPort->Open(PC);
			if(nRet == EC_SUCCESS)
			{
				if(    (ePinType = CPinpadEPP::IsMyself (wSubType, pPort)) == ePIN_UNKNOWN
					&& (ePinType = CPinpadBase::IsMyself(wSubType, pPort)) == ePIN_UNKNOWN
					&& (ePinType = CPinpadVISA::IsMyself(wSubType, pPort)) == ePIN_UNKNOWN
					&& (ePinType = CPinpadWOSA::IsMyself(wSubType, pPort)) == ePIN_UNKNOWN)
				{
					RLog2(LOG_LEVEL_DATA, "Failed to detect pinpad command type.");
				}
			}
			else
			{
				RLog2(LOG_LEVEL_DATA, "Failed to open pinpad, nRet = %d", nRet);
			}

			cDLF.Release(NAME_BASEPORT, REL_NAME_PORT, pPort);
		}

		if(NULL != lpSubType)
		{
			*lpSubType = wSubType;
		}

		return ePinType;
	}
};

HMODULE g_hModule = NULL;
CMyFactory cMyFactory;

InterfacePtr * WINAPI InstancePinpad(EPORT ePort, EPIN_TYPE eType, LPCTSTR lpDesc, EPIN_EXTEND eExtend)
{
	RLog2(LOG_LEVEL_DEBUG, _T("ePort=0x%X eType=0x%X eExtend=0x%X"), ePort, eType, eExtend);
	return cMyFactory.Instance(static_cast<I32>(ePort), static_cast<I32>(eType), lpDesc, static_cast<I32>(eExtend));
}

void WINAPI ReleasePinpad(IPinpad *p)
{
	cMyFactory.Release(p);
}

