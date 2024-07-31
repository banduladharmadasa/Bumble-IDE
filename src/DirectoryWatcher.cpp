

#include "stdafx.h"
#include "DirectoryWatcher.h"
#include "BumbleEditDoc.h"
#include <BumbleEdit.h>
#include <filesystem>
#include <pathcch.h>

void CSingleDirectoryWatcher::AddPath(_In_ PCWSTR path)
{
	m_dir = path;
	// m_notify.reset(reinterpret_cast<FILE_NOTIFY_INFORMATION*>(new char[c_bufferSize]));

	m_hDir = (CreateFile(path,
						 FILE_LIST_DIRECTORY,
						 FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
						 nullptr,
						 OPEN_EXISTING,
						 FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
						 nullptr));
	if (m_hDir == INVALID_HANDLE_VALUE)
	{
		BumbleMessageBox(HRESULT_FROM_WIN32(GetLastError()));
	}
	else
	{
		this->WatchDirectory();
	}
}

concurrency::task<void> CSingleDirectoryWatcher::WatchDirectory()
{
	auto token = m_cancellationTokenSource.get_token();
	return concurrency::create_task([this, token]
									{
										char buffer[4096];
										ZeroMemory(buffer, sizeof(buffer));

										while (true)
										{
											FILE_NOTIFY_INFORMATION fni;
											ReadDirectoryChangesW(m_hDir,
																  buffer,
																  sizeof(buffer),
																  FALSE,
																  FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE,
																  nullptr,
																  &m_overlapped,
																  nullptr);

											DWORD transferred;
											if (GetOverlappedResultEx(m_hDir, &m_overlapped, &transferred, 1000, FALSE))
											{

												FILE_NOTIFY_INFORMATION *next = (FILE_NOTIFY_INFORMATION *)buffer;

												std::vector<CString> fileList;
												while (true)
												{

													fileList.push_back(CString(next->FileName, next->FileNameLength / sizeof(TCHAR)));

													next += next->NextEntryOffset;
													Sleep(10);

													if (next->NextEntryOffset == 0)
													{
														break;
													}
												}

												Sleep(1000); // prevent for having two notifications for a single change
												for (auto it : fileList)
												{

													CString *str = new CString(it);
													str->Insert(0, L"\\");
													str->Insert(0, CString(m_dir.c_str()));
													if (!theApp.PostThreadMessageW(ID_FILEMONITOR_FILECHANGED, (WPARAM)str, 0))
													{

														delete str;
													}
												}
											}

											if (token.is_canceled())
											{

												concurrency::cancel_current_task();
												return;
											}
										}
									},
									token);
}

void CSingleDirectoryWatcher::Cancel()
{
	wprintf(L"Canceling watcher\n");
	m_cancellationTokenSource.cancel();
}

BOOL CSingleDirectoryWatcher::IsEqual(const CString &otherPath)
{
	return (m_dir.compare(otherPath.GetString()) == 0);
}

//===========================================================================================
CDirectoryWatcher::CDirectoryWatcher()
	: m_bIgnoreFileWatcher(FALSE)
{
}

CDirectoryWatcher::~CDirectoryWatcher()
{
}

void CDirectoryWatcher::OnFileMonitorFileChanged(WPARAM wparam, LPARAM lparam)
{
	CString *fileWatched = (CString *)wparam;
	if (m_bIgnoreFileWatcher)
	{
		m_bIgnoreFileWatcher = FALSE;
		delete fileWatched;
		return;
	}

	// loop through application's document templates
	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while (NULL != posDocTemplate)
	{
		CDocTemplate *pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);

		// get each document open in given document template
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (NULL != posDoc)
		{
			CDocument *pDoc = pDocTemplate->GetNextDoc(posDoc);

			if (!pDoc->IsKindOf(RUNTIME_CLASS(CBumbleEditDoc)))
			{
				continue;
			}

			CBumbleEditDoc *_pDoc = (CBumbleEditDoc *)pDoc;

			CString pathName = pDoc->GetPathName();
			if (!PathFileExists(pathName))
			{
				continue;
			}

			const size_t newsizew = (fileWatched->GetLength() + 1) * 2;
			char *nstringw = new char[newsizew];
			size_t convertedCharsw = 0;
			wcstombs_s(&convertedCharsw, nstringw, newsizew, *fileWatched, _TRUNCATE);

			const size_t newsizew2 = (pathName.GetLength() + 1) * 2;
			char *nstringw2 = new char[newsizew2];
			wcstombs_s(&convertedCharsw, nstringw2, newsizew2, pathName, _TRUNCATE);

			std::filesystem::path p1 = nstringw;
			std::filesystem::path p2 = nstringw2;

			std::error_code ec;
			bool bResult = std::filesystem::equivalent(p1, p2, ec);

			if (bResult)
			{
				pathName.Insert(0, L"The file ");
				pathName.Append(L" has been changed externally. Do you need to load modified version?");
				if (BumbleMessageBox(pathName, MB_YESNO) == IDOK)
				{
					((CBumbleEditDoc *)pDoc)->Reload();
				}
				break;
			}
		}
	}

	delete fileWatched;
}

void CDirectoryWatcher::AddDirToWatch(const CString &path)
{
	CString _path(path);
	if (!PathIsDirectory(_path))
	{
		PathCchRemoveFileSpec(_path.GetBuffer(), _path.GetLength());
	}

	for (auto &it : m_dirWatchList)
	{
		if (it->IsEqual(_path))
		{
			return;
		}
	}

	m_dirWatchList.push_back(std::unique_ptr<CSingleDirectoryWatcher>(new CSingleDirectoryWatcher()));
	m_dirWatchList.back()->AddPath(_path);
}

void CDirectoryWatcher::BoolIgnoreNextWatch(BOOL ignore)
{
	m_bIgnoreFileWatcher = ignore;
}
