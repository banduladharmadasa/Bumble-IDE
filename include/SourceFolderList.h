#pragma once

// CSourceFolderList

class CSourceFolderList : public CVSListBox
{
	DECLARE_DYNAMIC(CSourceFolderList)

public:
	CSourceFolderList();
	virtual ~CSourceFolderList();

	virtual void OnBrowse() override;
	virtual int OnGetImage(LV_ITEM *pItem);

protected:
	DECLARE_MESSAGE_MAP()
	CImageList m_checkLCImgList;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
