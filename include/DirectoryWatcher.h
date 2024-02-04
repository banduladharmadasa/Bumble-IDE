
#pragma once
#include <functional>
#include <concrt.h>
#include <ppltasks.h>
#include <vector>
#include <memory>

class CSingleDirectoryWatcher
{
public:
	void AddPath(_In_ PCWSTR path);
	concurrency::task<void> WatchDirectory();
	void Cancel();

	BOOL IsEqual(const CString &otherPath);
private:
	HANDLE m_hDir;
	std::wstring m_dir;
	std::unique_ptr<FILE_NOTIFY_INFORMATION> m_notify;
	OVERLAPPED m_overlapped;
	concurrency::cancellation_token_source m_cancellationTokenSource;
	
};

class CDirectoryWatcher
{
public:
	CDirectoryWatcher();
	~CDirectoryWatcher();

	void OnFileMonitorFileChanged(WPARAM wparam, LPARAM lparam);
	void AddDirToWatch(const CString& path);
	void BoolIgnoreNextWatch(BOOL ignore = TRUE);

	

protected:
	BOOL m_bIgnoreFileWatcher;
	std::vector<std::unique_ptr<CSingleDirectoryWatcher>> m_dirWatchList;
};


