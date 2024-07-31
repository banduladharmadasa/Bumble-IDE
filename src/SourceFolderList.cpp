// SourceFolderList.cpp : implementation file
//

#include "stdafx.h"
#include "BumbleEdit.h"
#include "SourceFolderList.h"
#include "CTabSource.h"

// CSourceFolderList

IMPLEMENT_DYNAMIC(CSourceFolderList, CVSListBox)

CSourceFolderList::CSourceFolderList()
{
}

CSourceFolderList::~CSourceFolderList()
{
}

void CSourceFolderList::OnBrowse()
{
	CFileDialog dlgOpenFile(TRUE);
	// add OFN_ALLOWMULTISELECT flag
	dlgOpenFile.GetOFN().Flags |= OFN_ALLOWMULTISELECT;

	try
	{
		// set a buffer to keep at least 100 full path and file names
		const int nBufferSize = 100 * (MAX_PATH + 1) + 1;
		CString strBuffer;
		LPTSTR pBuffer = strBuffer.GetBufferSetLength(nBufferSize);
		dlgOpenFile.GetOFN().lpstrFile = pBuffer;
		dlgOpenFile.GetOFN().nMaxFile = nBufferSize;
		dlgOpenFile.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_SOURCE_FILE_DLG);
		// show modal Open File dialog
		if (IDOK == dlgOpenFile.DoModal())
		{
			// get selected files
			POSITION pos = dlgOpenFile.GetStartPosition();
			CString strLst;
			while (NULL != pos)
			{
				CString strFilePath = dlgOpenFile.GetNextPathName(pos);
				AddItem(strFilePath);
			}
		}
		// release buffer
		strBuffer.ReleaseBuffer();
	}
	catch (CException *e)
	{
		e->ReportError();
		e->Delete();
	}

	CWnd *pParent = GetParent();

	if (!pParent)
	{
		return;
	}

	CTabSource *_pParent = (CTabSource *)pParent;
}

int CSourceFolderList::OnGetImage(LV_ITEM *pItem)
{
	return 2;
}

BEGIN_MESSAGE_MAP(CSourceFolderList, CVSListBox)
ON_WM_CREATE()

// ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CSourceFolderList::OnLvnItemchangedList1)

END_MESSAGE_MAP()

// CSourceFolderList message handlers

int CSourceFolderList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CVSListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	VERIFY(m_checkLCImgList.Create(IDB_CHECKBOXES, 16, 4, RGB(255, 0, 255)));
	m_pWndList->SetImageList(&m_checkLCImgList, LVSIL_SMALL);

	EnableToolTips();
	SetStandardButtons(AFX_VSLISTBOX_BTN_NEW | AFX_VSLISTBOX_BTN_DELETE);
	EnableBrowseButton();

	return 0;
}
