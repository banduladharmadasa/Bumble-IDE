#pragma once

#include <vector>
struct Param
{
	Param(DWORD param, LPCTSTR mess)
	{
		parameter = param;
		message = mess;
	};
	DWORD parameter;
	CString message;
};


// CFileMonitor

class CFileMonitor : public CObject
{
	

public:
	CFileMonitor();
	virtual ~CFileMonitor();
	void AddDirectory(const CString &dir);
	HANDLE m_hDir;
	CString m_strDir;
	BOOL m_bRunning;

protected:
	HANDLE m_hThread;
	
};


