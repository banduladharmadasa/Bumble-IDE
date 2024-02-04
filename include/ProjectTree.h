#pragma once
#include <map>

// CProjectTree

class CProjectTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CProjectTree)

public:
	CProjectTree();
	virtual ~CProjectTree();



	void AddFile(const CString& file);



	

protected:
	DECLARE_MESSAGE_MAP()

	TV_INSERTSTRUCT tvInsertStruct;
	HTREEITEM hRootItem;
	CImageList m_imgList;
	std::vector<CString> m_extensions;

	std::map<HTREEITEM, CString> m_fileMap;


	HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR strPath);

	LPCTSTR GetSubPath(LPCTSTR strPath);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void AddProject(const CString& projectName);
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void CreateBitmapForExtension(const CString& extension);
	BOOL AlreadyExists(const CString& file);
};


