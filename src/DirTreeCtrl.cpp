// DirTreeCtrl.cpp: 
// 
// wrapped CTreeCtrl to select and or display folders and files (optional )
// 

#include "stdafx.h"
#include "BumbleEdit.h"
#include "DirTreeCtrl.h"
#include "SortStringArray.h"
#include "resource.h"
#include "CFileBrowserDockPane.h"
#include <string>
#include <vector>
#include <pathcch.h>

#pragma comment(lib, "Pathcch.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileTreeWnd

CFileTreeWnd::CFileTreeWnd()
{

	m_strRoot = "";      // Auf Leer setzen
	m_filterString = L"*.*";

}

CFileTreeWnd::~CFileTreeWnd()
{
	m_imgList.Detach();
}

void CFileTreeWnd::SetFilters(const CString& filters)
{
	m_filterString = filters;
	DisplayTree(m_currentPath, TRUE);
	SetSelPath(m_currentPath);
}




BEGIN_MESSAGE_MAP(CFileTreeWnd, CTreeCtrl)
	//{{AFX_MSG_MAP(CFileTreeWnd)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CFileTreeWnd::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_CLICK, &CFileTreeWnd::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CFileTreeWnd::OnNMDblclk)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CFileTreeWnd::OnTvnEndlabeledit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFileTreeWnd 

BOOL CFileTreeWnd::DisplayTree(LPCTSTR strRoot, BOOL bFiles)
{
	DWORD dwStyle = GetStyle();   // read the windowstyle
	if (dwStyle & TVS_EDITLABELS)
	{
		// Don't allow the user to edit ItemLabels
		ModifyStyle(TVS_EDITLABELS, 0);
	}

	// Display the DirTree with the Rootname e.g. C:\
	// if Rootname == NULL then Display all Drives on this PC
	// First, we need the system-ImageList

	DeleteAllItems();

	if (!GetSysImgList())
		return FALSE;
	m_bFiles = bFiles;  // if TRUE, Display Path- and Filenames 
	if (strRoot == NULL || strRoot[0] == '\0')
	{
		if (!DisplayDrives())
			return FALSE;
		m_strRoot = "";
	}
	else
	{
		m_strRoot = strRoot;
		if (m_strRoot.Right(1) != '\\')
			m_strRoot += "\\";
		HTREEITEM hParent = AddItem(TVI_ROOT, m_strRoot);
		DisplayPath(hParent, strRoot);
	}
	return TRUE;
}
/////////////////////////////////////////////////
BOOL CFileTreeWnd::GetSysImgList()
/////////////////////////////////////////////////
{
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;

	if (GetImageList(TVSIL_NORMAL))
		m_imgList.Detach();

	hImgList = (HIMAGELIST)SHGetFileInfo(L"C:\\",
		0,
		&shFinfo,
		sizeof(shFinfo),
		SHGFI_SYSICONINDEX |
		SHGFI_SMALLICON);
	if (!hImgList)
	{
		m_strError = "Cannot retrieve the Handle of SystemImageList!";
		return FALSE;
	}

	m_imgList.m_hImageList = hImgList;

	SetImageList(&m_imgList, TVSIL_NORMAL);
	return TRUE;   // OK
}

BOOL CFileTreeWnd::DisplayDrives()
{
	//
	// Displaying the Availible Drives on this PC
	// This are the First Items in the TreeCtrl
	//
	DeleteAllItems();
	TCHAR  szDrives[128];
	TCHAR* pDrive;

	if (!GetLogicalDriveStrings(sizeof(szDrives), szDrives))
	{
		m_strError = "Error Getting Logical DriveStrings!";
		return FALSE;
	}

	pDrive = szDrives;
	while (*pDrive)
	{
		HTREEITEM hParent = AddItem(TVI_ROOT, pDrive);
		if (FindSubDir(pDrive))
			InsertItem(L"", 0, 0, hParent);
		pDrive += wcslen(pDrive) + 1;
	}


	return TRUE;

}



template<class Pattern, class Text>
BOOL wildcard(
	Pattern const pat_begin, Pattern const pat_end,
	Text text_begin, Text const text_end)
{
	ptrdiff_t const pat_size = pat_end - pat_begin;
	ptrdiff_t stackbuf[64];
	size_t c = sizeof(stackbuf) / sizeof(*stackbuf);
	ptrdiff_t* p = stackbuf;
	size_t n = 0;
	p[n++] = 0;
	while (n > 0 && text_begin != text_end)
	{
		for (size_t i = 0; i < n; i++)
		{
			if (p[i] == pat_size)
			{
				p[i--] = p[--n];
				continue;
			}
			switch (*(pat_begin + p[i]))
			{
			case '?': ++p[i]; break;
			case '*':
				ptrdiff_t off;
				off = p[i];
				while (off < pat_size &&
					*(pat_begin + off) == '*')
				{
					++off;
				}
				if (n == c)
				{
					ptrdiff_t const* const old = p;
					c *= 2;
					if (c == 0) { ++c; }
					size_t const size = c * sizeof(*p);
					p = (ptrdiff_t*)realloc(
						old == stackbuf ? NULL : p,
						size);
					if (old == stackbuf)
					{
						memcpy(p, old, n * sizeof(*old));
					}
				}
				p[n++] = off;
				break;
			default:
				if (*(pat_begin + p[i]) == *text_begin)
				{
					++p[i];
				}
				else { p[i--] = p[--n]; }
				break;
			}
		}
		++text_begin;
	}
	BOOL success = false;
	if (text_begin == text_end)
	{
		while (!success && n > 0)
		{
			--n;
			while (p[n] != pat_size &&
				*(pat_begin + p[n]) == '*')
			{
				++p[n];
			}
			if (p[n] == pat_size)
			{
				success = true;
			}
		}
	}
	if (p != stackbuf) { free(p); }
	return success;
}

BOOL wildcard(char const* const pattern, char const* const text)
{
	return wildcard(
		pattern,
		pattern + (pattern ? strlen(pattern) : 0),
		text,
		text + (text ? strlen(text) : 0));
}

BOOL wildcard(wchar_t const* const pattern, wchar_t const* const text)
{
	return wildcard(
		pattern,
		pattern + (pattern ? wcslen(pattern) : 0),
		text,
		text + (text ? wcslen(text) : 0));
}

BOOL CFileTreeWnd::IsWildcardMatched(const CString& str)
{
	int nTokenPos = 0;
	CString strToken = m_filterString.Tokenize(_T(","), nTokenPos);

	while (!strToken.IsEmpty())
	{
		// do something with strToken
		if (wildcard(strToken.Trim(), str)) {

			return TRUE;
		}

		strToken = m_filterString.Tokenize(_T(","), nTokenPos);
	}

	return FALSE;
}

void CFileTreeWnd::DisplayPath(HTREEITEM hParent, LPCTSTR strPath)
{
	//
	// Displaying the Path in the TreeCtrl
	//
	CFileFind find;
	CString   strPathFiles = strPath;
	BOOL      bFind;
	CSortStringArray strDirArray;
	CSortStringArray strFileArray;

	if (strPathFiles.Right(1) != "\\")
		strPathFiles += "\\";
	strPathFiles += L"*.*";

	bFind = find.FindFile(strPathFiles);

	while (bFind)
	{
		bFind = find.FindNextFile();
		if (find.IsDirectory() && !find.IsDots())
		{
			strDirArray.Add(find.GetFilePath());
		}
		if (!find.IsDirectory() && m_bFiles) {

			if (IsWildcardMatched(find.GetFileName())) {
				strFileArray.Add(find.GetFilePath());
			}

		}


	}

	strDirArray.Sort();
	SetRedraw(FALSE);
	CWaitCursor wait;

	for (int i = 0; i < strDirArray.GetSize(); i++)
	{
		HTREEITEM hItem = AddItem(hParent, strDirArray.GetAt(i));
		if (FindSubDir(strDirArray.GetAt(i)))
			InsertItem(L"", 0, 0, hItem);
	}

	if (m_bFiles)
	{
		strFileArray.Sort();
		for (int i = 0; i < strFileArray.GetSize(); i++)
		{
			HTREEITEM hItem = AddItem(hParent, strFileArray.GetAt(i));

		}
	}

	SetRedraw(TRUE);


}

void CFileTreeWnd::DisplayPath(HTREEITEM hParent, LPCTSTR strPath, LPCTSTR strExt)
{

}

HTREEITEM CFileTreeWnd::AddItem(HTREEITEM hParent, LPCTSTR strPath)
{
	// Adding the Item to the TreeCtrl with the current Icons
	SHFILEINFO shFinfo;
	int iIcon, iIconSel;
	CString    strTemp = strPath;

	if (strTemp.Right(1) != '\\')
		strTemp += "\\";
	if (!SHGetFileInfo(strTemp,
		0,
		&shFinfo,
		sizeof(shFinfo),
		SHGFI_ICON |
		SHGFI_SMALLICON))
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return NULL;
	}

	iIcon = shFinfo.iIcon;

	// we only need the index from the system image list

	DestroyIcon(shFinfo.hIcon);

	if (!SHGetFileInfo(strTemp,
		0,
		&shFinfo,
		sizeof(shFinfo),
		SHGFI_ICON | SHGFI_OPENICON |
		SHGFI_SMALLICON))
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return NULL;
	}

	iIconSel = shFinfo.iIcon;

	// we only need the index of the system image list

	DestroyIcon(shFinfo.hIcon);

	if (strTemp.Right(1) == "\\")
		strTemp.SetAt(strTemp.GetLength() - 1, '\0');

	if (hParent == TVI_ROOT)
		return InsertItem(strTemp, iIcon, iIconSel, hParent);

	return InsertItem(GetSubPath(strTemp), iIcon, iIconSel, hParent);
}

LPCTSTR CFileTreeWnd::GetSubPath(LPCTSTR strPath)
{
	//
	// getting the last SubPath from a PathString
	// e.g. C:\temp\readme.txt
	// the result = readme.txt
	static CString strTemp;
	int     iPos;

	strTemp = strPath;
	if (strTemp.Right(1) == '\\')
		strTemp.SetAt(strTemp.GetLength() - 1, '\0');
	iPos = strTemp.ReverseFind('\\');
	if (iPos != -1)
		strTemp = strTemp.Mid(iPos + 1);

	return (LPCTSTR)strTemp;
}

BOOL CFileTreeWnd::FindSubDir(LPCTSTR strPath)
{
	//
	// Are there subDirs ?
	//
	CFileFind find;
	CString   strTemp = strPath;
	BOOL      bFind;

	if (strTemp[strTemp.GetLength() - 1] == '\\')
		strTemp += "*.*";
	else
		strTemp += "\\*.*";

	bFind = find.FindFile(strTemp);


	while (bFind)
	{
		bFind = find.FindNextFile();

		if (find.IsDirectory() && !find.IsDots())
		{
			return TRUE;
		}
		if (!find.IsDirectory() && m_bFiles && !find.IsHidden())
			return TRUE;

	}

	return FALSE;

}

void CFileTreeWnd::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CString strPath;

	if (pNMTreeView->itemNew.state & TVIS_EXPANDED)
	{
		//UINT uTest = TVIS_EXPANDEDONCE;
		ExpandItem(pNMTreeView->itemNew.hItem, TVE_EXPAND);
		/*
		//
		// Delete All items
		// And display the subpath
		//
		HTREEITEM hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		while ( hChild )
		{
			DeleteItem( hChild );
			hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		}

		strPath = GetFullPath( pNMTreeView->itemNew.hItem );
		DisplayPath( pNMTreeView->itemNew.hItem, strPath );
		*/
	}
	else
	{
		//
		// Delete the Items, but leave one there, for 
		// expanding the item next time
		//
		HTREEITEM hChild = GetChildItem(pNMTreeView->itemNew.hItem);

		while (hChild)
		{
			DeleteItem(hChild);
			hChild = GetChildItem(pNMTreeView->itemNew.hItem);
		}
		InsertItem(L"", pNMTreeView->itemNew.hItem);
	}

	*pResult = 0;
}

CString CFileTreeWnd::GetFullPath(HTREEITEM hItem)
{
	// get the Full Path of the item
	CString strReturn;
	CString strTemp;
	HTREEITEM hParent = hItem;

	strReturn = "";

	while (hParent)
	{

		strTemp = GetItemText(hParent);
		strTemp += "\\";
		strReturn = strTemp + strReturn;
		hParent = GetParentItem(hParent);
	}

	strReturn.TrimRight('\\');

	return strReturn;

}

void CFileTreeWnd::SetSelPathCurrent(DWORD command, BOOL val)
{
	wchar_t system_buffer[MAX_PATH];
	system_buffer[0] = 0;


	if (command == ID_SELECTFILTER_NOFILTER) {
		DisplayDrives();
		return;
	}

	switch (command)
	{
	case ID_SELECTFILTER_BUMBLEEDIT:
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, system_buffer))) {

			m_currentPath = system_buffer;
			m_currentPath.Append(L"\\Bumble Projects");
		}
	}
	break;

	case ID_SELECTFILTER_DESKTOP:
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, SHGFP_TYPE_CURRENT, system_buffer))) {
			m_currentPath = system_buffer;
		}
	}
	break;

	case ID_SELECTFILTER_DOCUMENT:
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, system_buffer))) {
			m_currentPath = system_buffer;
		}
	}
	break;
	case ID_SELECTFILTER_PROGRAMFILES:
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, system_buffer))) {
			m_currentPath = system_buffer;
		}
	}
	break;

	case ID_SELECTFILTER_PROGRAMFILES32980:
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILESX86, NULL, SHGFP_TYPE_CURRENT, system_buffer))) {
			m_currentPath = system_buffer;
		}
	}
	break;


	default:

		break;
	}

	if (PathIsDirectory(m_currentPath)) {
		DeleteAllItems();
		DisplayTree(m_currentPath, TRUE);
		SetSelPath(m_currentPath);
	}
}

BOOL CFileTreeWnd::SetSelPath(LPCTSTR strPath)
{
	// Setting the Selection in the Tree
	HTREEITEM hParent = TVI_ROOT;
	int       iLen = wcslen(strPath) + 2;
	TCHAR* pszPath = new TCHAR[iLen];
	TCHAR* pPath = pszPath;
	BOOL      bRet = FALSE;

	if (!IsValidPath(strPath))
	{
		delete[] pszPath; // this must be added 29.03.99
		return FALSE;
	}

	wcscpy_s(pszPath, iLen, strPath);
	_wcsupr_s(pszPath, iLen);

	if (pszPath[wcslen(pszPath) - 1] != '\\')
		wcscat_s(pszPath, iLen, L"\\");

	int iLen2 = wcslen(pszPath);

	for (WORD i = 0; i < iLen2; i++)
	{
		if (pszPath[i] == L'\\')
		{
			SetRedraw(FALSE);
			pszPath[i] = '\0';
			hParent = SearchSiblingItem(hParent, pPath);
			if (!hParent)  // Not found!
				break;
			else
			{
				// Info:
				// the notification OnItemExpanded 
				// will not called every time 
				// after the call Expand. 
				// You must call Expand with TVE_COLLAPSE | TVE_COLLAPSERESET
				// to Reset the TVIS_EXPANDEDONCE Flag

				UINT uState;
				uState = GetItemState(hParent, TVIS_EXPANDEDONCE);
				if (uState)
				{
					Expand(hParent, TVE_EXPAND);
					Expand(hParent, TVE_COLLAPSE | TVE_COLLAPSERESET);
					InsertItem(L"", hParent); // insert a blank child-item
					Expand(hParent, TVE_EXPAND); // now, expand send a notification
				}
				else
					Expand(hParent, TVE_EXPAND);
			}
			pPath += wcslen(pPath) + 1;
		}
	}

	delete[] pszPath;

	if (hParent) // Ok the last subpath was found
	{
		SelectItem(hParent); // select the last expanded item
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	SetRedraw(TRUE);

	//Expand the selection
	HTREEITEM hItem;
	hItem = GetFirstVisibleItem();
	Expand(hItem, TVE_EXPAND);


	return bRet;
}

HTREEITEM CFileTreeWnd::SearchSiblingItem(HTREEITEM hItem, LPCTSTR strText)
{
	HTREEITEM hFound = GetChildItem(hItem);
	CString   strTemp;
	while (hFound)
	{
		strTemp = GetItemText(hFound);
		strTemp.MakeUpper();
		if (strTemp == strText)
			return hFound;
		hFound = GetNextItem(hFound, TVGN_NEXT);
	}

	return NULL;
}


void CFileTreeWnd::ExpandItem(HTREEITEM hItem, UINT nCode)
{
	CString strPath;

	if (nCode == TVE_EXPAND)
	{
		HTREEITEM hChild = GetChildItem(hItem);
		while (hChild)
		{
			DeleteItem(hChild);
			hChild = GetChildItem(hItem);
		}

		strPath = GetFullPath(hItem);
		DisplayPath(hItem, strPath);
	}
}

BOOL CFileTreeWnd::IsValidPath(LPCTSTR strPath)
{
	// This function check the Pathname

	HTREEITEM hChild;
	CString   strItem;
	CString   strTempPath = strPath;
	BOOL      bFound = FALSE;
	CFileFind find;

	hChild = GetChildItem(TVI_ROOT);
	strTempPath.MakeUpper();
	strTempPath.TrimRight('\\');

	while (hChild)
	{
		strItem = GetItemText(hChild);
		strItem.MakeUpper();
		if (strItem == strTempPath.Mid(0, strItem.GetLength()))
		{
			bFound = TRUE;
			break;
		}
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}

	if (!bFound)
		return FALSE;

	strTempPath += "\\nul";
	if (find.FindFile(strTempPath))
		return TRUE;

	return FALSE;
}


void CFileTreeWnd::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here



	*pResult = 0;
}



void CFileTreeWnd::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;



	HTREEITEM hItem;
	CPoint ptMouse;
	POINT sMouse;
	UINT uFlags;
	::GetCursorPos(&sMouse);
	ptMouse = sMouse;
	ScreenToClient(&ptMouse);
	hItem = HitTest(ptMouse, &uFlags);

	if (hItem == NULL)
		return;



	m_currentPath = GetFullPath(hItem);


}


void CFileTreeWnd::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	HTREEITEM hItem;
	CPoint ptMouse;
	POINT sMouse;
	UINT uFlags;
	::GetCursorPos(&sMouse);
	ptMouse = sMouse;
	ScreenToClient(&ptMouse);
	hItem = HitTest(ptMouse, &uFlags);

	if (hItem == NULL)
		return;

	CString str = GetFullPath(hItem);
	theApp.OpenFile(str);


}





void CFileTreeWnd::OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString itemText = pTVDispInfo->item.pszText;
	if (itemText.IsEmpty()) {
		return;
	}

	CFileBrowserDockPane* pParent = (CFileBrowserDockPane*)GetParent();

	if (!pParent) {
		return;
	}

	CString oldName = pParent->GetSelectedPath();
	CString rootName(oldName);

	

	if (PathCchRemoveFileSpec(&rootName.GetBuffer()[0], rootName.GetLength()) != S_OK) {
		theApp.ThrowLastError(L"Unable to rename the file");

		return;
	}

	CString newName;
	newName.Append(rootName);
	newName.Append(L"\\");
	newName.Append(itemText);

	
	theApp.OnAfterFileRenamed(oldName, newName, pResult);
	
	
	

}
