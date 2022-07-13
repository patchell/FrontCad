// EditText.cpp : implementation file
//

#include "pch.h"

// CEditText

IMPLEMENT_DYNAMIC(CEditText, CEdit)

BEGIN_MESSAGE_MAP(CEditText, CEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, &CEditText::OnEnChange)
END_MESSAGE_MAP()



// CEditText message handlers




void CEditText::OnEnChange()
{
	if (!m_bDirty)
	{
		m_bDirty = TRUE;
//		if (GetParent())
//			GetParent()->PostMessageW((UINT)WindowsMsg::WM_DLG_CONTROL_DIRTY);
	}
}
