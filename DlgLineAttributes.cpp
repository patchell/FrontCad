// DlgLineAttributes.cpp : implementation file
//

#include "pch.h"


// CDlgLineAttributes dialog

IMPLEMENT_DYNAMIC(CDlgLineAttributes, CDialog)

CDlgLineAttributes::CDlgLineAttributes(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_LINE_ATTRB, pParent)
{
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
	DDX_Control(pDX, IDC_CHECK_LINEPROP_LOCKLEN, m_Check_LockLength);
	DDX_Control(pDX, IDC_CHECK_P1_TO_POINT, m_Check_Snap_P1_ToPoint);
	DDX_Control(pDX, IDC_CHECK_P2_TO_LINE, m_Check_Snap_P2_ToLine);
	DDX_Control(pDX, IDC_CHECK_P2_TO_POINT, m_Check_Snap_P2_ToPoint);
	DDX_Control(pDX, IDC_EDIT_LINE_LENGTH, m_Edit_LineLength);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR_SELECTED, m_Static_Line_SelectedColor);
}


BEGIN_MESSAGE_MAP(CDlgLineAttributes, CDialog)
END_MESSAGE_MAP()


// CDlgLineAttributes message handlers


BOOL CDlgLineAttributes::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetDoubleValue(m_pLine->GetAttributes().m_LineWidth);
	m_Static_LineColor.SetColor(m_pLine->GetAttributes().m_colorLine);
	m_Static_Line_SelectedColor.SetColor(m_pLine->GetAttributes().m_colorSelected);
	if (m_pLine->GetAttributes().m_LockLength)
	{
		m_Check_LockLength.SetCheck(BST_CHECKED);
		m_Edit_LineLength.SetDecimalPlaces(3);
		m_Edit_LineLength.SetDoubleValue(m_pLine->GetLength());
		m_Check_Snap_P2_ToLine.SetCheck(m_pLine->GetAttributes().m_P2_FIXED_LEN_SNAP_LINE);
		m_Edit_LineLength.SetFocus();
		m_Edit_LineLength.SetSel(0,-1);
	}
	else
	{
		m_Check_LockLength.ShowWindow(SW_HIDE);
		m_Edit_LineLength.ShowWindow(SW_HIDE);
		m_Check_Snap_P2_ToLine.ShowWindow(SW_HIDE);
		m_Edit_Width.SetFocus();
		m_Edit_Width.SetSel(0, -1);
	}
	m_Check_Snap_P1_ToPoint.SetCheck(m_pLine->GetAttributes().m_P1_SNAP_POINT);
	m_Check_Snap_P2_ToPoint.SetCheck(m_pLine->GetAttributes().m_P2_SNAP_POINT);
	return FALSE;  // return TRUE unless you set the focus to a control
}


void CDlgLineAttributes::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			m_pLine->GetAttributes().m_colorLine = m_Static_LineColor.GetColor();
			m_pLine->GetAttributes().m_colorSelected = m_Static_Line_SelectedColor.GetColor();
			m_pLine->GetAttributes().m_LineWidth = m_Edit_Width.GetDoubleValue();
			m_pLine->SetLength(m_Edit_LineLength.GetDoubleValue());
			m_pLine->GetAttributes().m_LockLength = m_Check_LockLength.GetCheck();
			m_pLine->GetAttributes().m_P1_SNAP_POINT = m_Check_Snap_P1_ToPoint.GetCheck();
			m_pLine->GetAttributes().m_P2_FIXED_LEN_SNAP_LINE = m_Check_Snap_P2_ToLine.GetCheck();
			m_pLine->GetAttributes().m_P2_SNAP_POINT = m_Check_Snap_P2_ToPoint.GetCheck();
			CDialog::OnOK();
			break;
	}

}
