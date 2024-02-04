#pragma once



// CShellThread

class CShellThread : public CWinThread
{
	DECLARE_DYNCREATE(CShellThread)

public:
	CShellThread(const CString &args = CString());           // protected constructor used by dynamic creation
	virtual ~CShellThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:

	BOOL CreatePipes();

	int Execute();
	void Stop();
	void AddMessage(const CString& str, AppMessageType type);
	void ReadFromPipe(HANDLE hndl, BOOL isErr);
	

	BOOL m_bRunning;
private:
	HANDLE hChildStd_IN_Rd = NULL;
	HANDLE hChildStd_IN_Wr = NULL;
	HANDLE hChildStd_OUT_Rd = NULL;
	HANDLE hChildStd_OUT_Wr = NULL;
	HANDLE hChildStd_ERR_Rd = NULL;
	HANDLE hChildStd_ERR_Wr = NULL;
	CString m_strArgs;
	
	PROCESS_INFORMATION processInfo;
};


