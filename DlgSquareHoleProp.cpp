// DlgSquareHoleProp.cpp : implementation file
//

#include "pch.h"


// CDlgSquareHoleProp dialog

IMPLEMENT_DYNAMIC(CDlgSquareHoleProp, CDialogEx)

CDlgSquareHoleProp::CDlgSquareHoleProp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDlgSquareHoleProp, pParent)
{
	m_bDirty = FALSE;
	m_pHole = 0;
}

CDlgSquareHoleProp::~CDlgSquareHoleProp()
{
}

void CDlgSquareHoleProp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ROUNDHOLE_COLOR, m_Static_LineColor);
	DDX_Control(pDX, IDC_EDIT_ROUNDHOLE_RADIUS, m_Edit_Radius);
	DDX_Control(pDX, IDC_EDIT_ROUNDHOLE_WIDTH, m_Edit_LineWidth);
}


BEGIN_MESSAGE_MAP(CDlgSquareHoleProp, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_ROUNDHOLE_COLOR, &CDlgSquareHoleProp::OnStnClickedStaticRoundholeColor)
	ON_MESSAGE((UINT)WindowsMsg::WM_DLG_CONTROL_DIRTY, &CDlgSquareHoleProp::OnDlgControlDirty)
END_MESSAGE_MAP()


// CDlgSquareHoleProp message handlers


BOOL CDlgSquareHoleProp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Edit_Radius.SetDecimalPlaces(3);
	m_Edit_Radius.SetDoubleValue(m_pHole->GetAttributes().m_HoleWidth);
	m_Edit_LineWidth.SetDecimalPlaces(3);
	m_Edit_LineWidth.SetDoubleValue(m_pHole->GetLineWidth());
	m_Static_LineColor.SetColor(m_pHole->GetLineColor());
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgSquareHoleProp::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
	case IDOK:
		if (IsDirty())
		{
			m_pHole->SetLineColor(m_Static_LineColor.GetColor());
			m_pHole->SetHoleRadius(m_Edit_Radius.GetDoubleValue());
			m_pHole->SetLineWidth(m_Edit_LineWidth.GetDoubleValue());
		}
		CDialogEx::OnOK();
		break;
	default:
		GetNextDlgTabItem(pW)->SetFocus();
		break;
	}
}

void CDlgSquareHoleProp::OnStnClickedStaticRoundholeColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}


afx_msg LRESULT CDlgSquareHoleProp::OnDlgControlDirty(WPARAM wParam, LPARAM lParam)
{
	m_bDirty = TRUE;
	return 0;
}
