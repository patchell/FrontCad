// DlgRectProperites.cpp : implementation file
//

#include "pch.h"


// CDlgRectProperties dialog

IMPLEMENT_DYNAMIC(CDlgRectProperties, CDialogEx)

CDlgRectProperties::CDlgRectProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_RECTPROPERTIES, pParent)
{
	m_pRect = 0;
}

CDlgRectProperties::~CDlgRectProperties()
{
}

void CDlgRectProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_Edit_LineWidth);
	DDX_Control(pDX, IDC_STATIC_FILL_COLOR, m_Static_FillColor);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_Static_LineColor);
	DDX_Control(pDX, IIDC_CHECK_RECT_NOFILL, m_Check_NoFill);
	DDX_Control(pDX, IDC_STATIC_RECT_COLOR_SELECTED, m_Static_ColorSelected);
}


BEGIN_MESSAGE_MAP(CDlgRectProperties, CDialog)
	ON_BN_CLICKED(IIDC_CHECK_RECT_NOFILL, &CDlgRectProperties::OnClickedIidcCheckRectNofill)
END_MESSAGE_MAP()


// CDlgRectProperties message handlers


BOOL CDlgRectProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_LineWidth.SetDecimalPlaces(3);
	m_Edit_LineWidth.SetDoubleValue(m_pRect->GetAttributes().m_LineWidth);
	m_Static_LineColor.SetColor(m_pRect->GetAttributes().m_colorLine);
	m_Static_FillColor.SetColor(m_pRect->GetAttributes().m_colorFill);
	m_Static_ColorSelected.SetColor(m_pRect->GetAttributes().m_colorSelected);
	m_Check_NoFill.SetCheck(m_pRect->GetAttributes().m_TransparentFill);
	if(m_pRect->GetAttributes().m_TransparentFill)
		m_Static_FillColor.ShowWindow(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRectProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			m_pRect->GetAttributes().m_colorLine = m_Static_LineColor.GetColor();
			m_pRect->GetAttributes().m_colorFill = m_Static_FillColor.GetColor();
			m_pRect->GetAttributes().m_colorSelected = m_Static_ColorSelected.GetColor();
			m_pRect->GetAttributes().m_TransparentFill = m_Check_NoFill.GetCheck();
			m_pRect->GetAttributes().m_LineWidth = m_Edit_LineWidth.GetDoubleValue();
			CDialog::OnOK();
			break;
	}
}

void CDlgRectProperties::OnClickedIidcCheckRectNofill()
{
	if (m_Check_NoFill.GetCheck() == BST_CHECKED)
	{
		m_Static_FillColor.ShowWindow(0);
	}
	else
	{
		m_Static_FillColor.ShowWindow(1);
	}
}

