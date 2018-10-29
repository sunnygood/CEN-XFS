/*****************************************************************************
* Copyright(C) 2015, Corporation.
*
* File Name:   CVDMStruct.h
* Description: struct for VDM
*
* Author:      liuguoxun
* Date:        2015-11-09         
*
*****************************************************************************/

#ifndef _CVDMSTRUCT_H_20151109
#define _CVDMSTRUCT_H_20151109

#include <CENXFSVDM.h>
#include <CSTLUnit.h>


class CWFSVDMAPPSTATUS : public WFSVDMAPPSTATUS
{
public:
	CWFSVDMAPPSTATUS()
	{
		Clear();
	}

	~CWFSVDMAPPSTATUS()
	{
		Clear();
	}

	void Clear()
	{
		lpszAppID = NULL;
		wAppStatus = 0;
		m_s.clear();
	}

	void Set(LPCTSTR szAppID, WORD wAppStatus)
	{
		Clear();

		m_s.Set(szAppID);
		this->lpszAppID = (LPSTR)(m_s.c_str());
		this->wAppStatus = wAppStatus;
	}

	operator LPWFSVDMAPPSTATUS()
	{
		return this;
	}

protected:
 	CStringZ<char> m_s;
};

class CWFSVDMSTATUS : public WFSVDMSTATUS
{
public:
	CWFSVDMSTATUS()
	{
		Clear();
		wDevice	= WFS_VDM_DEVOFFLINE;
		wService = WFS_VDM_INACTIVE;
	}

	~CWFSVDMSTATUS()
	{
		Clear();
	}

	void Clear()
	{
		lppAppStatus = NULL;
		lpszExtra	= NULL;
 		m_AppStatus.clear();
	}

	void Add(CWFSVDMAPPSTATUS *p)
	{
		if(NULL != p)
		{
			m_AppStatus.push_back(p);
		}
	}

	operator LPWFSVDMSTATUS()
	{
		if(m_AppStatus.empty())
		{
			lppAppStatus = NULL;
		}
		else
		{
			m_AppStatus.push_back(NULL);
			lppAppStatus = (LPWFSVDMAPPSTATUS*)(m_AppStatus.data());
		}

		return this;
	}

protected:
	std::vector<CWFSVDMAPPSTATUS*> m_AppStatus;
};


#endif // _CVDMSTRUCT_H_20151109
