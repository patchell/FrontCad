// ComboBoxFontWeight.cpp : implementation file
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboBoxFontWeight

static int FW[15] = {
	FW_DONTCARE,
	FW_THIN,
	FW_EXTRALIGHT,
	FW_ULTRALIGHT,
	FW_LIGHT,
	FW_NORMAL,
	FW_REGULAR,
	FW_MEDIUM,
	FW_SEMIBOLD,
	FW_DEMIBOLD,
	FW_BOLD,
	FW_EXTRABOLD,
	FW_ULTRABOLD,
	FW_BLACK,
	FW_HEAVY
};

CComboBoxFontWeight::CComboBoxFontWeight()
{
}

CComboBoxFontWeight::~CComboBoxFontWeight()
{
}


BEGIN_MESSAGE_MAP(CComboBoxFontWeight, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxFontWeight message handlers

BOOL CComboBoxFontWeight::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)
	{
		case WM_KEYDOWN:
			if(VK_RETURN == pMsg->wParam)
			{
				CWnd *wC;
				int v = GetCurSel();
				if(m_pW)m_pW->SendMessage(m_WMsg,m_ObjMsg,FW[v]);
				wC = GetParent()->GetFocus();
				wC = GetParent()->GetNextDlgTabItem(wC);	//reset the focus
				wC->SetFocus();
				return TRUE;
			}
			break;
		default:
			return CComboBox::PreTranslateMessage(pMsg);
			break;
	}	
	return CComboBox::PreTranslateMessage(pMsg);
}

void CComboBoxFontWeight::SetupMessaging(CWnd *pW, int Wmsg, int Smsg)
{
	this->m_pW = pW;
	this->m_WMsg = Wmsg;	//window message
	this->m_ObjMsg = Smsg;	//sub message
}

void CComboBoxFontWeight::SetFontWeight(int w)
{
	int Item = 0;
	for(int i=0,loop=1;i<15 && loop;++i)
	{
		if(FW[i] ==w)
		{
			loop=0;
			Item = i;
		}
	}
	CComboBox::SetCurSel(Item);
}

static const char * const FWstrings[15] = {
	"DONTCARE",
	"THIN",
	"EXTRALIGHT",
	"ULTRALIGHT",
	"LIGHT",
	"NORMAL",
	"REGULAR",
	"MEDIUM",
	"SEMIBOLD",
	"DEMIBOLD",
	"BOLD",
	"EXTRABOLD",
	"ULTRABOLD",
	"BLACK",
	"HEAVY"
};

void CComboBoxFontWeight::Init()
{
	for(int i=0;i<15;++i)
		this->InsertString(i,CString(FWstrings[i]));
	SetCurSel(8);
}

void CComboBoxFontWeight::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	int v = GetCurSel();
	if(m_pW)m_pW->SendMessage(m_WMsg,m_ObjMsg,FW[v]);
}

int CComboBoxFontWeight::GetFontWeight()
{
	int v = GetCurSel();
	return FW[v];
}
