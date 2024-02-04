#include "stdafx.h"
#include "CMFCPropertyGridMultiFileProperty.h"
#include "resource.h"


CMFCPropertyGridMultiFileProperty::CMFCPropertyGridMultiFileProperty(const CString& strName, BOOL bOpenFileDialog, const CString& strFileName, LPCTSTR lpszDefExt, DWORD dwFlags, LPCTSTR lpszFilter, LPCTSTR lpszDescr, DWORD_PTR dwData)
	:CMFCPropertyGridFileProperty( strName, bOpenFileDialog, strFileName, lpszDefExt,
		 dwFlags, lpszFilter, lpszDescr, dwData)
{

}

void CMFCPropertyGridMultiFileProperty::OnClickButton(CPoint point)
{
	//CBumbleFileDialog dlgOpenFile(TRUE,0,0,0,0,0,0,FALSE);
	//// add OFN_ALLOWMULTISELECT flag
	//dlgOpenFile.GetOFN().Flags |= OFN_ALLOWMULTISELECT;
	//dlgOpenFile.GetOFN().Flags |= OFN_ENABLETEMPLATE;
	//
	//
	//try
	//{
	//	// set a buffer to keep at least 100 full path and file names
	//	const int nBufferSize = 100 * (MAX_PATH + 1) + 1;
	//	CString strBuffer;
	//	LPTSTR pBuffer = strBuffer.GetBufferSetLength(nBufferSize);
	//	dlgOpenFile.GetOFN().lpstrFile = pBuffer;
	//	dlgOpenFile.GetOFN().nMaxFile = nBufferSize;
	//	dlgOpenFile.m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_SOURCE_FILE_DLG);
	//	// show modal Open File dialog
	//	if (IDOK == dlgOpenFile.DoModal())
	//	{
	//		// get selected files
	//		POSITION pos = dlgOpenFile.GetStartPosition();
	//		CString strLst;
	//		while (NULL != pos)
	//		{
	//			CString strFilePath = dlgOpenFile.GetNextPathName(pos);
	//			// ... do something with strFilePath.
	//			strLst += L" " + strFilePath;
	//		}
	//		
	//		CString old(m_varValue);
	//		strLst.Insert(0, old);
	//		m_varValue = strLst;
	//	}
	//	// release buffer
	//	strBuffer.ReleaseBuffer();
	//}
	//catch (CException * e)
	//{
	//	e->ReportError();
	//	e->Delete();
	//}

	

	
	

}
