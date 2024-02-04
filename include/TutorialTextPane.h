#pragma once

#include <vector>
// CCommandHistoryPane

class CCommandHistoryPane : public CDockablePane
{
	DECLARE_DYNAMIC(CCommandHistoryPane)

public:
	CCommandHistoryPane();
	virtual ~CCommandHistoryPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);



	
	

private:
	static HHOOK m_kbdHook;
	static HHOOK m_mouseHook;
	static CCommandHistoryPane* m_pTutorialTextPane;
	//static CString m_displayString;

	static std::vector<CString> m_messageQueue;

	static LRESULT KeyBoardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	
public:
	afx_msg void OnDestroy();


};


