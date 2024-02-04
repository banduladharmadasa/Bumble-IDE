#pragma once


// CStaticEx

class CStaticEx : public CStatic
{
	DECLARE_DYNAMIC(CStaticEx)

public:
	CStaticEx();
	virtual ~CStaticEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


