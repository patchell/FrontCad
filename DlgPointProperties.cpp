// DlgPointProperties.cpp : implementation file
//

#include "pch.h"


// CDlgPointProperties dialog

IMPLEMENT_DYNAMIC(CDlgPointProperties, CDialog)

CDlgPointProperties::CDlgPointProperties(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_POINT_PROPERTIES, pParent)
{
	m_bDirty = FALSE;
	m_pPoint = 0;
}

CDlgPointProperties::~CDlgPointProperties()
{
}

void CDlgPointProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_POINT_COLOR_NORMAL, m_Static_NormalColor);
	DDX_Control(pDX, IDC_STATIC_POINT_COLOR_SELECTED, m_Static_SelectedColor);
}


BEGIN_MESSAGE_MAP(CDlgPointProperties, CDialog)
	ON_MESSAGE((UINT)WindowsMsg::WM_DLG_CONTROL_DIRTY, &CDlgPointProperties::OnDlgControlDirty)
END_MESSAGE_MAP()


// CDlgPointProperties message handlers


BOOL CDlgPointProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Static_NormalColor.SetColor(m_pPoint->GetAttributes().m_colorNormal);
	m_Static_SelectedColor.SetColor(m_pPoint->GetAttributes().m_colorSelected);
	return TRUE;  
}


void CDlgPointProperties::OnOK()
{
	if (m_bDirty)
	{
		if (m_pPoint)
		{
			m_pPoint->GetAttributes().m_colorNormal = m_Static_NormalColor.GetColor();
			m_pPoint->GetAttributes().m_colorSelected = m_Static_SelectedColor.GetColor();
		}
	}
	CDialog::OnOK();
}


afx_msg LRESULT CDlgPointProperties::OnDlgControlDirty(WPARAM wParam, LPARAM lParam)
{
	m_bDirty = TRUE;
	return 0;
}
