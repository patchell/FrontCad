// DlgRndHole1Flat.cpp : implementation file
//

#include "pch.h"


// CDlgRndHole1Flat dialog

IMPLEMENT_DYNAMIC(CDlgRndHole1Flat, CDialogEx)

CDlgRndHole1Flat::CDlgRndHole1Flat(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RNDHOLE1FLATPROP, pParent)
{
	m_pHole = 0;
}

CDlgRndHole1Flat::~CDlgRndHole1Flat()
{
}

void CDlgRndHole1Flat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RNDHOLE1FLAT_COLOR, m_Static_Color);
	DDX_Control(pDX, IDC_RNDHOLE1FLAT_WIDTH, m_Edit_LineWidth);
	DDX_Control(pDX, IDC_RNDHOLE1FLAT_RADIUS, m_Edit_Radius);
	DDX_Control(pDX, IDC_RNDHOLE1FLAT_FLAT, m_Edit_Flat);
}


BEGIN_MESSAGE_MAP(CDlgRndHole1Flat, CDialogEx)
	ON_STN_CLICKED(IDC_RNDHOLE1FLAT_COLOR, &CDlgRndHole1Flat::OnStnClickedRndhole1flatColor)
END_MESSAGE_MAP()


// CDlgRndHole1Flat message handlers


BOOL CDlgRndHole1Flat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Edit_LineWidth.SetDecimalPlaces(3);
	m_Edit_LineWidth.SetDoubleValue(m_pHole->GetAttributes().m_LineWidth);
	m_Static_Color.SetColor(m_pHole->GetAttributes().m_colorLine);
	m_Edit_Flat.SetDecimalPlaces(3);
	m_Edit_Flat.SetDoubleValue(m_pHole->GetAttributes().m_FlatDistanceFromCenter);
	m_Edit_Radius.SetDecimalPlaces(3);
	m_Edit_Radius.SetDoubleValue(m_pHole->GetHoleRadius());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRndHole1Flat::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
	case IDOK:
		m_pHole->GetAttributes().m_colorLine = m_Static_Color.GetColor();
		m_pHole->GetAttributes().m_FlatDistanceFromCenter = m_Edit_Flat.GetDoubleValue();
		m_pHole->GetAttributes().m_HoleRadius = m_Edit_Radius.GetDoubleValue();
		m_pHole->GetAttributes().m_LineWidth = m_Edit_LineWidth.GetDoubleValue();
		CDialogEx::OnOK();
		break;
	default:
		GetNextDlgTabItem(pW)->SetFocus();
		break;
	}
}

void CDlgRndHole1Flat::OnStnClickedRndhole1flatColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_Color.SetColor(Dlg.GetColor());

}

int DoRoundHole1FlatPropertiesDlg(CCadHoleRnd1Flat* pRndHole1Flat) 
{
	CDlgRndHole1Flat Dlg;
	Dlg.SetRndHole1Flat(pRndHole1Flat);
	int Id = Dlg.DoModal();
	return Id;
}
