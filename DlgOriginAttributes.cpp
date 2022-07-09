// DlgOriginAttributes.cpp : implementation file
//

#include "pch.h"


// CDlgOriginAttributes dialog

IMPLEMENT_DYNAMIC(CDlgOriginAttributes, CDialog)

CDlgOriginAttributes::CDlgOriginAttributes(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_ORIGIN_ATTRIBUTES, pParent)
{
	m_pOrg = 0;
	m_bDirty = FALSE;
}

CDlgOriginAttributes::~CDlgOriginAttributes()
{
}

void CDlgOriginAttributes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ORIGIN_LINE_COLOR, m_Static_LineColor);
	DDX_Control(pDX, IDC_STATIC_ORIGIN_SELECTED_COLOR, m_Static_SelectionColor);
	DDX_Control(pDX, IDC_EDIT_ORIGIN_RADIUS, m_Edit_Radius);
	DDX_Control(pDX, IDC_EDIT_ORIGIN_NAME, m_Edit_Name);
	DDX_Control(pDX, IDC_EDIT_ORIGIN_LINE_WIDTH, m_Edit_LineWidth);
}


BEGIN_MESSAGE_MAP(CDlgOriginAttributes, CDialog)
	ON_MESSAGE((UINT)WindowsMsg::WM_DLG_CONTROL_DIRTY, &CDlgOriginAttributes::OnDlgControlDirty)
END_MESSAGE_MAP()


// CDlgOriginAttributes message handlers

BOOL CDlgOriginAttributes::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_LineWidth.SetDecimalPlaces(3);
	m_Edit_LineWidth.SetDoubleValue(m_pOrg->GetAttributes().m_LineWidth);
	m_Edit_Radius.SetDecimalPlaces(3);
	m_Edit_Radius.SetDoubleValue(m_pOrg->GetAttributes().m_Radius);
	m_Static_LineColor.SetColor(m_pOrg->GetAttributes().m_colorLine);
	m_Static_SelectionColor.SetColor(m_pOrg->GetAttributes().m_colorSelected);
	m_Edit_Name.SetWindowTextW(m_pOrg->GetName());
	return TRUE;  
}


void CDlgOriginAttributes::OnOK()
{
	CWnd* pWnd = GetFocus();
	UINT Id = pWnd->GetDlgCtrlID();

	switch (Id)
	{
	case IDOK:
		if (m_bDirty)
		{
			m_pOrg->GetAttributes().m_LineWidth = m_Edit_LineWidth.GetDoubleValue();
			m_pOrg->GetAttributes().m_Radius = m_Edit_Radius.GetDoubleValue();
			m_Edit_Name.GetWindowTextW(m_pOrg->GetName());
			m_pOrg->GetAttributes().m_colorLine = m_Static_LineColor.GetColor();
			m_pOrg->GetAttributes().m_colorSelected = m_Static_SelectionColor.GetColor();
		}
		CDialog::OnOK();
		break;
	case IDC_EDIT_ORIGIN_LINE_WIDTH:
	case IDC_EDIT_ORIGIN_NAME:
	case IDC_EDIT_ORIGIN_RADIUS:
		pWnd->GetNextDlgTabItem(pWnd)->SetFocus();
	default:
		pWnd->GetNextDlgTabItem(pWnd)->SetFocus();
		break;
	}
}


afx_msg LRESULT CDlgOriginAttributes::OnDlgControlDirty(WPARAM wParam, LPARAM lParam)
{
	m_bDirty = TRUE;
	return 0;
}
