//----------------------------------------------------
// DlgPolygonProperties.cpp : implementation file
//----------------------------------------------------

#include "pch.h"

//----------------------------------------------------
// CDlgPolygonProperties dialog
//----------------------------------------------------

IMPLEMENT_DYNAMIC(CDlgPolygonProperties, CDialog)

CDlgPolygonProperties::CDlgPolygonProperties(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_POLY_EDPROPERTIES, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_pPolygon = 0;
}

CDlgPolygonProperties::~CDlgPolygonProperties()
{
}

void CDlgPolygonProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_POLYGON_NOFILL, m_Check_NoFill);
	DDX_Control(pDX, IDC_EDIT_LINEWIDTH, m_Edit_LineWidth);
	DDX_Control(pDX, IDC_STATIC_FILL_COLOR, m_Static_FillColor);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_Static_LineColor);
}

BEGIN_MESSAGE_MAP(CDlgPolygonProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILL_COLOR, &CDlgPolygonProperties::OnClickedStaticFillColor)
	ON_STN_CLICKED(IDC_STATIC_LINE_COLOR, &CDlgPolygonProperties::OnClickedStaticLineColor)
END_MESSAGE_MAP()


// CDlgPolygonProperties message handlers


BOOL CDlgPolygonProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_LineWidth.SetDecimalPlaces(3);
	m_Edit_LineWidth.SetDoubleValue(m_pPolygon->GetAttributes().m_LineWidth);
	m_Static_LineColor.SetColor(m_pPolygon->GetAttributes().m_colorLine);
	m_Check_NoFill.SetCheck(m_pPolygon->GetTransparent());
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgPolygonProperties::OnOK()
{
	CWnd* pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
	case IDOK:
		m_pPolygon->GetAttributes().m_colorLine = m_Static_LineColor.GetColor();
		m_pPolygon->GetAttributes().m_colorFill = m_Static_FillColor.GetColor();
		m_pPolygon->GetAttributes().m_LineWidth = m_Edit_LineWidth.GetDoubleValue();
		m_pPolygon->SetTransparent(m_Check_NoFill.GetCheck());;
		CDialog::OnOK();
		break;
	}
}

void CDlgPolygonProperties::OnClickedStaticFillColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_FillColor.SetColor(Dlg.GetColor());
}

void CDlgPolygonProperties::OnClickedStaticLineColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}

