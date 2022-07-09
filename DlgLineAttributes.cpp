// DlgLineAttributes.cpp : implementation file
//

#include "pch.h"


// CDlgLineAttributes dialog

IMPLEMENT_DYNAMIC(CDlgLineAttributes, CDialog)

CDlgLineAttributes::CDlgLineAttributes(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_LINE_ATTRB, pParent)
{
	m_bDirty = FALSE;
	m_pLine = 0;
}

CDlgLineAttributes::~CDlgLineAttributes()
{
}

void CDlgLineAttributes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_Static_LineColor);
}


BEGIN_MESSAGE_MAP(CDlgLineAttributes, CDialog)
	ON_MESSAGE((UINT)WindowsMsg::WM_DLG_CONTROL_DIRTY, &CDlgLineAttributes::OnDlgControlDirty)
END_MESSAGE_MAP()


// CDlgLineAttributes message handlers


BOOL CDlgLineAttributes::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetDoubleValue(m_pLine->GetAttributes().m_LineWidth);
	m_Static_LineColor.SetColor(m_pLine->GetAttributes().m_colorLine);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgLineAttributes::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			if (IsDirty())
			{
				m_pLine->GetAttributes().m_colorLine = m_Static_LineColor.GetColor();
				m_pLine->GetAttributes().m_LineWidth = m_Edit_Width.GetDoubleValue();
			}
			CDialog::OnOK();
			break;
	}

}

afx_msg LRESULT CDlgLineAttributes::OnDlgControlDirty(WPARAM wParam, LPARAM lParam)
{
	m_bDirty = TRUE;
	return 0;
}
