// CFileMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "AvailEdit.h"
#include "CFileMonitor.h"
#include "AvailEdit.h"
#include <map>


CCriticalSection m_criticalSection;

DWORD dwFilters = FILE_NOTIFY_CHANGE_SECURITY | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_ACCESS | FILE_NOTIFY_CHANGE_LAST_WRITE
| FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_FILE_NAME;
FILE_NOTIFY_INFORMATION Buffer[1024];
DWORD BytesReturned;

void ThreadProc(void* pParam)
{
	USES_CONVERSION;

	CFileMonitor* pObject = (CFileMonitor*)pParam;

	if (pObject == NULL ||
		!pObject->IsKindOf(RUNTIME_CLASS(CFileMonitor)))
		return;   // if pObject is not valid

	
	while (ReadDirectoryChangesW(pObject->m_hDir , &Buffer, sizeof(Buffer), FALSE, dwFilters, &BytesReturned, NULL, NULL))
	{
		switch (Buffer[0].Action)
		{
		case FILE_ACTION_ADDED: 
			break;
		case FILE_ACTION_REMOVED:
			break;
		case FILE_ACTION_MODIFIED:
			break;
		case FILE_ACTION_RENAMED_OLD_NAME:
			break;
		case FILE_ACTION_RENAMED_NEW_NAME:
			break;
		}


		int i = 0;
		do
		{
			m_criticalSection.Lock();

			
			/*CString* _str = new CString();
			_str->Append(pObject->m_strDir);
			_str->Append(L"\\");
			_str->Append(CString(Buffer[i].FileName).Left(Buffer[i].FileNameLength / 2));
			
			if (!theApp.PostThreadMessageW(ID_FILEMONITOR_FILECHANGED, (WPARAM)_str, 0)) {

				delete _str;
			}*/


			i++;
			m_criticalSection.Unlock();
		} while (!Buffer[i].NextEntryOffset && pObject->m_bRunning);


	}

	_endthreadex(0);
}

CFileMonitor::CFileMonitor()
	:m_hThread(NULL)
	,m_hDir(NULL)
	,m_bRunning(FALSE)
{
	
}

CFileMonitor::~CFileMonitor()
{
	m_bRunning = FALSE;
	CancelIoEx(m_hDir, NULL);
}

void CFileMonitor::AddDirectory(const CString& dir)
{
	m_bRunning = TRUE;
	m_strDir = dir;
	m_hDir = CreateFile(dir, FILE_LIST_DIRECTORY, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	m_hThread = (HANDLE)_beginthread(ThreadProc, 0, (void*)this);
	
}





