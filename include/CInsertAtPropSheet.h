#pragma once

#include "CInsertAtNumber.h"
#include "CInsertTextAt.h"
// CInsertAtPropSheet

class CInsertAtPropSheet : public CBumblePropertySheet
{
	DECLARE_DYNAMIC(CInsertAtPropSheet)

public:
	CInsertAtPropSheet();
	virtual ~CInsertAtPropSheet();

protected:
	DECLARE_MESSAGE_MAP()

	CInsertNumberAt m_numberPropSheet;
	CInsertTextAt m_textPropSheet;
public:
	afx_msg void OnIdok();

};


