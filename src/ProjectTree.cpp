// ProjectTree.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "ProjectTree.h"
#include "helper.h"

// CProjectTree

IMPLEMENT_DYNAMIC(CProjectTree, CTreeCtrl)

CProjectTree::CProjectTree()
{
}

CProjectTree::~CProjectTree()
{
}

HTREEITEM FindItem(const CString &name, CTreeCtrl &tree, HTREEITEM hRoot)
{
	// check whether the current item is the searched one
	CString text = tree.GetItemText(hRoot);
	if (text.Compare(name) == 0)
		return hRoot;

	// get a handle to the first child item
	HTREEITEM hSub = tree.GetChildItem(hRoot);
	// iterate as long a new item is found
	while (hSub)
	{
		// check the children of the current item
		HTREEITEM hFound = FindItem(name, tree, hSub);
		if (hFound)
			return hFound;

		// get the next sibling of the current item
		hSub = tree.GetNextSiblingItem(hSub);
	}

	// return NULL if nothing was found
	return NULL;
}

void CProjectTree::CreateBitmapForExtension(const CString &extension)
{
	/*CBitmap bmp;
	if (bmp.CreateBitmap(16, 16, 1, 32, NULL)) {


	}*/

	// Gdiplus::Bitmap bmp(16, 16);

	// Gdiplus::Graphics g(&bmp);

	// g.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color::Red), 0, 0, 16, 16);

	// HBITMAP hSourceBmp;
	// bmp.GetHBITMAP(Gdiplus::Color::Red,&hSourceBmp);

	// CBitmap destBitmap;
	// destBitmap.Attach(CopyImage(hSourceBmp, IMAGE_BITMAP, 0, 0, 0));

	// m_imgList.Add(&destBitmap, RGB(255, 0, 0));

	// CString str = langToExtensionMap[extension];

	// InitLangToExtensionMap();
}

BOOL CProjectTree::AlreadyExists(const CString &file)
{
	for (auto it : m_fileMap)
	{
		if (it.second.CompareNoCase(file) == 0)
		{

			return TRUE;
		}
	}

	return FALSE;
}

void CProjectTree::AddFile(const CString &file)
{
	if (AlreadyExists(file))
	{
		return;
	}

	CString extension = PathFindExtension(file);

	if (HTREEITEM item = FindItem(extension, *this, NULL))
	{
		HTREEITEM _item = AddItem(item, file);

		SortChildren(item);

		if (_item)
		{
			m_fileMap[_item] = file;
		}

		return;
	}

	CreateBitmapForExtension(extension);

	tvInsertStruct.hParent = hRootItem;
	tvInsertStruct.hInsertAfter = TVI_LAST;

	tvInsertStruct.item.pszText = extension.GetBuffer();
	tvInsertStruct.item.mask = TVIF_TEXT;

	HTREEITEM hItem = InsertItem(&tvInsertStruct);
	ASSERT(hItem);

	HTREEITEM _item = AddItem(hItem, file);
	m_fileMap[_item] = file;
	SortChildren(hItem);
}

BEGIN_MESSAGE_MAP(CProjectTree, CTreeCtrl)
ON_WM_CREATE()

ON_WM_LBUTTONDOWN()
ON_WM_KEYDOWN()
ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CProjectTree message handlers

HTREEITEM CProjectTree::AddItem(HTREEITEM hParent, LPCTSTR strPath)
{
	// Adding the Item to the TreeCtrl with the current Icons
	SHFILEINFO shFinfo = {0};

	CString strTemp = strPath;

	if (strTemp.Right(1) != '\\')
		strTemp += "\\";

	if (strTemp.Right(1) == "\\")
		strTemp.SetAt(strTemp.GetLength() - 1, '\0');

	if (hParent == TVI_ROOT)
		return InsertItem(strTemp, 0, 1, hParent);

	return InsertItem(GetSubPath(strTemp), 2, 3, hParent);
}

LPCTSTR CProjectTree::GetSubPath(LPCTSTR strPath)
{
	//
	// getting the last SubPath from a PathString
	// e.g. C:\temp\readme.txt
	// the result = readme.txt
	static CString strTemp;
	int iPos;

	strTemp = strPath;
	if (strTemp.Right(1) == '\\')
		strTemp.SetAt(strTemp.GetLength() - 1, '\0');
	iPos = strTemp.ReverseFind('\\');
	if (iPos != -1)
		strTemp = strTemp.Mid(iPos + 1);

	return (LPCTSTR)strTemp;
}

int CProjectTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_imgList.Create(16, 16, ILC_MASK | ILC_COLOR32, 0, 4))
	{

		CBitmap bmp;
		BOOL bResult = bmp.LoadBitmapW(IDB_CHECKBOX);
		int nResult = m_imgList.Add(&bmp, RGB(255, 0, 0, 255));
		nResult = m_imgList.Add(&bmp, RGB(255, 0, 0, 255));
	}

	SetImageList(&m_imgList, TVSIL_NORMAL);
	AddProject(L"My Project");

	return 0;
}

void CProjectTree::AddProject(const CString &projectName)
{

	tvInsertStruct.hParent = hRootItem;
	tvInsertStruct.hInsertAfter = TVI_LAST;

	CString name(projectName);
	tvInsertStruct.item.pszText = name.GetBuffer();
	tvInsertStruct.item.mask = TVIF_TEXT;

	HTREEITEM hItem = InsertItem(projectName, 4, 5, NULL);
	ASSERT(hItem);
}

void CProjectTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	theApp.GetMainWnd()->PostMessageW(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(point.y, point.x));

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CProjectTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CProjectTree::PreTranslateMessage(MSG *pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		theApp.GetMainWnd()->PostMessageW(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
	}

	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CProjectTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	HTREEITEM selectedItem = HitTest(point, &nFlags);

	if (selectedItem)
	{

		CString file = m_fileMap[selectedItem];

		if (PathFileExists(file))
		{
			theApp.OpenDocumentFile(file);
		}
	}

	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}
