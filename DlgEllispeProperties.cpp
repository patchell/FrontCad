// DlgEllispeProperties.cpp : implementation file
//

#include "pch.h"


// CDlgEllispeProperties dialog

IMPLEMENT_DYNAMIC(CDlgEllispeProperties, CDialog)

CDlgEllispeProperties::CDlgEllispeProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ELLIPSE_PROPERTIES, pParent)
{
	m_pEllipse = 0;
}

CDlgEllispeProperties::~CDlgEllispeProperties()
{
}

void CDlgEllispeProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATIC_FILLCOLOR, m_Static_FillColor);
	DDX_Control(pDX, IDC_STATIC_LINECOLOR, m_Static_LineColor);
	DDX_Control(pDX, IDC_CHECK_ELLIPSE_NOFILL, m_Check_NoFill);
}


BEGIN_MESSAGE_MAP(CDlgEllispeProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, &CDlgEllispeProperties::OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_LINECOLOR, &CDlgEllispeProperties::OnStnClickedStaticLinecolor)
	ON_BN_CLICKED(IDC_CHECK_ELLIPSE_NOFILL, &CDlgEllispeProperties::OnBnClickedCheckEllipseNofill)
END_MESSAGE_MAP()


// CDlgEllispeProperties message handlers


BOOL CDlgEllispeProperties::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetDoubleValue(m_pEllipse->GetAttributes().m_LineWidth);
	m_Static_LineColor.SetColor(m_pEllipse->GetAttributes().m_colorLine);
	m_Check_NoFill.SetCheck(m_pEllipse->GetAttributes().m_TransparentFill);
	m_Static_FillColor.SetColor(m_pEllipse->GetAttributes().m_colorFill);
	if(m_pEllipse->GetAttributes().m_TransparentFill)
		m_Static_FillColor.ShowWindow(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgEllispeProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
	case IDOK:
		m_pEllipse->GetAttributes().m_TransparentFill = m_Check_NoFill.GetCheck();
		m_pEllipse->GetAttributes().m_colorLine = m_Static_LineColor.GetColor();
		m_pEllipse->GetAttributes().m_colorFill = m_Static_FillColor.GetColor();
		m_pEllipse->GetAttributes().m_LineWidth = m_Edit_Width.GetDoubleValue();
		CDialog::OnOK();
		break;
	}
}

void CDlgEllispeProperties::OnStnClickedStaticFillcolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_FillColor.SetColor(Dlg.GetColor());
}


void CDlgEllispeProperties::OnStnClickedStaticLinecolor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}

void CDlgEllispeProperties::OnBnClickedCheckEllipseNofill()
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
