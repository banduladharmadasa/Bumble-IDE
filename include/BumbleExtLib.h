#pragma once

#include "BumbleExtInterface.h"
#include <memory>

#define DEFAULT_THREAD_START_UP 0x8000

namespace BumbleIDE
{

	class CBumbleExtLib
	{
	public:
		CBumbleExtLib(BumbleIDE::CBumbleExtInterface *pPlugin);
		virtual ~CBumbleExtLib();
		virtual CMenu *GetMenu();
		virtual void OnIDECommand(UINT nID, BumbleDocument *pDoc);
		virtual CString GetName();

	public:
	protected:
		BumbleDocument *m_pDoc;
		BumbleStartUpInfo m_startUpInfo = {};

		CString m_extName;
		BumbleIDE::CBumbleExtInterface *m_pPlugin;
	};

}
