// EditDecimal.cpp : implementation file
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDecimal

CEditDecimal::CEditDecimal()
{
	m_DecimalPlaces = 0;
	m_ObjMsg = WMarrowSubMsg(0);
	m_WMsg = WindowsMsg(0);
	m_pW = 0;
	m_Dirty = 0;
}

CEditDecimal::~CEditDecimal()
{
}


BEGIN_MESSAGE_MAP(CEditDecimal, CEdit)
	//{{AFX_MSG_MAP(CEditDecimal)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDecimal message handlers

BOOL CEditDecimal::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
		case WM_KEYDOWN:
			if(VK_RETURN == pMsg->wParam)
			{
				CWnd *wC;
				wC = GetParent()->GetFocus();
				wC = GetParent()->GetNextDlgTabItem(wC);	//reset the focus
				wC->SetFocus();
				return TRUE;
			}
			break;
		default:
			return CEdit::PreTranslateMessage(pMsg);
			break;
	}
	return CEdit::PreTranslateMessage(pMsg);
}

void CEditDecimal::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_Dirty = TRUE;
	switch (nChar)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
	case VK_BACK:
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		break;
	default:
		break;
	}
}

int CEditDecimal::GetValue()
{
	CString csValue;
	char* pcDest = new char[256];
	int v;

	GetWindowTextW(csValue);
	v = atoi(GETAPP.ConvertCStringToChar(pcDest, csValue));
	delete[] pcDest;
	return v;
}

void CEditDecimal::SetDoubleValue(double v)
{
	CString csValue;
	CString csFormat;

	csFormat.Format(
		_T("%%%d.%dlf"), 
		m_DecimalPlaces + 5, 
		m_DecimalPlaces
	);
	csValue.Format(csFormat, v);
	SetWindowText(csValue);
}

double CEditDecimal::GetDoubleValue()
{
	CString csValue;
	double rV;
	char* s = new char[256];

	GetWindowTextW(csValue);
	rV = atof(GETAPP.ConvertCStringToChar(s, csValue));
	delete[] s;
	return rV;
}

void CEditDecimal::SetValue(int v)
{
	CString csValue;

	csValue.Format(_T("%d"), v);
	SetWindowTextW(csValue);
}

int CEditDecimal::GetDecimalPlaces()
{
	return m_DecimalPlaces;
}

void CEditDecimal::SetDecimalPlaces(int p)
{
	m_DecimalPlaces = p;
}

void CEditDecimal::SetupMessaging(CWnd *pWdest, WindowsMsg WMsg, WMarrowSubMsg SubMsg)
{
	m_pW = pWdest;
	m_WMsg = WMsg;
	m_ObjMsg = SubMsg;
}
