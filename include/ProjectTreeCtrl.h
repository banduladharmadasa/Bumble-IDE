#pragma once

// CMyProjectTreeCtrl

class CMyProjectTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CMyProjectTreeCtrl)

public:
	CMyProjectTreeCtrl();
	virtual ~CMyProjectTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void Recurse(LPCTSTR pstr);
	void DisplayPath(HTREEITEM hParent, LPCTSTR strPath);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void DisplaySubPath(const CString &path, const CString &filter, const CString &category);
	BOOL IsWildcardMatched(const CString &str);

	HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR strPath);

	LPCTSTR GetSubPath(LPCTSTR strPath);

	BOOL FindSubDir(LPCTSTR strPath);

	BOOL GetSysImgList();

protected:
	TV_INSERTSTRUCT tvInsertStruct;
	HTREEITEM hRootItem;

	std::vector<CString> m_pathVector;

	BOOL m_bFiles;

	CString m_filterString;

	CString m_strError;
	CImageList m_imgList;
};

BOOL wildcard(char const *const pattern, char const *const text);

BOOL wildcard(wchar_t const *const pattern, wchar_t const *const text);
