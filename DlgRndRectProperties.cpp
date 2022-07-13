// DlgRndRectProperties.cpp : implementation file
//

#include "pch.h"


// CDlgRndRectProperties dialog

IMPLEMENT_DYNAMIC(CDlgRndRectProperties, CDialog)

CDlgRndRectProperties::CDlgRndRectProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ROUNDEDRECT_PROPERTIES, pParent)
{
	m_pRndRect = 0;
}

CDlgRndRectProperties::~CDlgRndRectProperties()
{
}

void CDlgRndRectProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CORNERRADIUS, m_Edit_RadiusX);
	DDX_Control(pDX, IDC_EDIT_CORNERRADIUS_Y, m_Edit_RadiusY);
	DDX_Control(pDX, IDC_EDIT_LINEWIDTH, m_Edit_LineWidth);
	DDX_Control(pDX, IDC_STATIC_FILLCOLOR, m_Static_FillColor);
	DDX_Control(pDX, IDC_STATIC_LINECOLOR, m_Static_LineColor);
	DDX_Control(pDX, IDC_CHECK_RNDRECT_NOFILL, m_Check_NoFill);
}


BEGIN_MESSAGE_MAP(CDlgRndRectProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, &CDlgRndRectProperties::OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_LINECOLOR, &CDlgRndRectProperties::OnStnClickedStaticLinecolor)
	ON_BN_CLICKED(IDC_CHECK_RNDRECT_NOFILL, &CDlgRndRectProperties::OnBnClickedCheckRndrectNofill)
END_MESSAGE_MAP()


// CDlgRndRectProperties message handlers


BOOL CDlgRndRectProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_RadiusX.SetDecimalPlaces(3);
	m_Edit_RadiusX.SetValue(m_pRndRect->GetAttributes().m_dszCornerRadius.ToCSize().cx);
	m_Edit_RadiusY.SetDecimalPlaces(3);
	m_Edit_RadiusY.SetValue(m_pRndRect->GetAttributes().m_dszCornerRadius.ToCSize().cy);
	m_Edit_LineWidth.SetDecimalPlaces(3);
	m_Edit_LineWidth.SetDoubleValue(m_pRndRect->GetAttributes().m_LineWidth);
	m_Static_LineColor.SetColor(m_pRndRect->GetAttributes().m_colorLine);
	m_Static_FillColor.SetColor(m_pRndRect->GetAttributes().m_colorFill);
	m_Check_NoFill.SetCheck(m_pRndRect->GetAttributes().m_TransparentFill);
	if(m_pRndRect->GetAttributes().m_TransparentFill)
		m_Static_FillColor.ShowWindow(0);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgRndRectProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			m_pRndRect->GetAttributes().m_colorLine = m_Static_LineColor.GetColor();
			m_pRndRect->GetAttributes().m_dszCornerRadius = CDoubleSize(m_Edit_RadiusX.GetDoubleValue(), m_Edit_RadiusY.GetDoubleValue());
			m_pRndRect->GetAttributes().m_TransparentFill = m_Check_NoFill.GetCheck();
			m_pRndRect->GetAttributes().m_colorFill = m_Static_FillColor.GetColor();
			m_pRndRect->GetAttributes().m_LineWidth = m_Edit_LineWidth.GetDoubleValue();
			CDialog::OnOK();
			break;
	}
}

void CDlgRndRectProperties::OnStnClickedStaticFillcolor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_FillColor.SetColor(Dlg.GetColor());
}


void CDlgRndRectProperties::OnStnClickedStaticLinecolor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}


void CDlgRndRectProperties::OnBnClickedCheckRndrectNofill()
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
