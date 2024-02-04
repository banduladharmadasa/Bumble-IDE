#ifndef AFX_DIRTREECTRL_H__1E4F97A0_B41E_11D2_955E_204C4F4F5020__INCLUDED_
#define AFX_DIRTREECTRL_H__1E4F97A0_B41E_11D2_955E_204C4F4F5020__INCLUDED_

// DirTreeCtrl.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CFileTreeWnd 

class CFileTreeWnd : public CTreeCtrl
{
// Konstruktion
public:
	CFileTreeWnd();

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CFileTreeWnd)
	//}}AFX_VIRTUAL

// Implementierung
public:
	void SetSelPathCurrent(DWORD command, BOOL val = TRUE);
	BOOL SetSelPath( LPCTSTR strPath );
	CString GetFullPath( HTREEITEM hItem );
	LPCTSTR GetSubPath( LPCTSTR strPath );
	BOOL DisplayTree( LPCTSTR strRoot, BOOL bFiles = FALSE );
	virtual ~CFileTreeWnd();
	void SetFilters(const CString& filters);
	
	// Generierte Nachrichtenzuordnungsfunktionen

	BOOL IsValidPath( LPCTSTR strPath );
	void ExpandItem( HTREEITEM hItem, UINT nCode );
	HTREEITEM SearchSiblingItem( HTREEITEM hItem, LPCTSTR strText );
	BOOL FindSubDir( LPCTSTR strPath );
	HTREEITEM AddItem( HTREEITEM hParent, LPCTSTR strPath );
	BOOL IsWildcardMatched(const CString& str);
	void DisplayPath( HTREEITEM hParent, LPCTSTR strPath );
	void DisplayPath(HTREEITEM hParent, LPCTSTR strPath, LPCTSTR strExt);


	BOOL DisplayDrives();
	BOOL m_bFiles;
	CString m_strError;
	BOOL GetSysImgList();
	CImageList m_imgList;
	CString m_strRoot;
	CString m_currentPath;

	//{{AFX_MSG(CFileTreeWnd)
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);

private:
	CString m_filterString;
public:
	afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_DIRTREECTRL_H__1E4F97A0_B41E_11D2_955E_204C4F4F5020__INCLUDED_
