// ProjectTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "ProjectTreeCtrl.h"
#include "SortStringArray.h"
#include <string>

using namespace std;

// CMyProjectTreeCtrl

IMPLEMENT_DYNAMIC(CMyProjectTreeCtrl, CTreeCtrl)

CMyProjectTreeCtrl::CMyProjectTreeCtrl()
	:m_bFiles(TRUE)
{

}

CMyProjectTreeCtrl::~CMyProjectTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyProjectTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMyProjectTreeCtrl message handlers




void CMyProjectTreeCtrl::Recurse(LPCTSTR pstr)
{
	CFileFind finder;

	// build a string with wildcards
	CString strWildcard(pstr);
	strWildcard += _T("\\*.*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// if it's a directory, recursively search it

		if (finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			//TRACE(str);
			//TRACE(L"\n");

			
			

			

			tvInsertStruct.hParent = hRootItem;
			tvInsertStruct.hInsertAfter = TVI_LAST;
			const LPTSTR cutJobNameTemp = str.GetBuffer(0);
			tvInsertStruct.item.pszText = cutJobNameTemp;
			tvInsertStruct.item.mask = TVIF_TEXT;

			HTREEITEM hItem = InsertItem(&tvInsertStruct);
			ASSERT(hItem);
			 //SetItemData(hItem, (DWORD_PTR)pElement);
			if (!hItem) {
				break;
			}

			



			Recurse(str);
		}
	}

	finder.Close();
}


void CMyProjectTreeCtrl::DisplayPath(HTREEITEM hParent, LPCTSTR strPath)
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


int CMyProjectTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!GetSysImgList()) {

	}

	return 0;
}


void CMyProjectTreeCtrl::DisplaySubPath(const CString& path, const CString& filter, const CString& category)
{
	m_filterString = filter;
	tvInsertStruct.hParent = hRootItem;
	tvInsertStruct.hInsertAfter = TVI_LAST;


	CString str = CString(category);
	LPWSTR cutJobNameTemp = str.GetBuffer();
	
	tvInsertStruct.item.pszText = cutJobNameTemp;
	tvInsertStruct.item.mask = TVIF_TEXT;

	HTREEITEM hItem = InsertItem(&tvInsertStruct);
	ASSERT(hItem);


	DisplayPath(hItem, path);
}



BOOL CMyProjectTreeCtrl::IsWildcardMatched(const CString& str)
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


HTREEITEM CMyProjectTreeCtrl::AddItem(HTREEITEM hParent, LPCTSTR strPath)
{
	// Adding the Item to the TreeCtrl with the current Icons
	SHFILEINFO shFinfo = {0};
	int iIcon, iIconSel;
	CString    strTemp = strPath;

	if (strTemp.Right(1) != '\\')
		strTemp += "\\";

	if (!SHGetFileInfo(strTemp, 0, &shFinfo, sizeof(shFinfo), SHGFI_ICON | SHGFI_SMALLICON))
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return NULL;
	}

	iIcon = shFinfo.iIcon;

	// we only need the index from the system image list

	DestroyIcon(shFinfo.hIcon);

	if (!SHGetFileInfo(strTemp, 0, &shFinfo, sizeof(shFinfo), SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON))
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


LPCTSTR CMyProjectTreeCtrl::GetSubPath(LPCTSTR strPath)
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

BOOL CMyProjectTreeCtrl::FindSubDir(LPCTSTR strPath)
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



BOOL CMyProjectTreeCtrl::GetSysImgList()
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
		BumbleMessageBox(L"Cannot retrieve the Handle of System Image List!");
		return FALSE;
	}

	m_imgList.m_hImageList = hImgList;

	SetImageList(&m_imgList, TVSIL_NORMAL);
	return TRUE;   // OK
}