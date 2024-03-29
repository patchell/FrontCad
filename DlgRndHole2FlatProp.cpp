//--------------------------------------------------
// DlgRndHole2FlatProp.cpp : implementation file
//--------------------------------------------------

#include "pch.h"


//--------------------------------------------------
// CDlgRndHole2FlatProp dialog
//--------------------------------------------------

IMPLEMENT_DYNAMIC(CDlgRndHole2FlatProp, CDialogEx)

CDlgRndHole2FlatProp::CDlgRndHole2FlatProp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RNDHOLE2FLATPROP, pParent)
{
	m_pHole = 0;
}

CDlgRndHole2FlatProp::~CDlgRndHole2FlatProp()
{
}

void CDlgRndHole2FlatProp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RNDHOLE1FLAT_WIDTH, m_Edit_LineWidth);
	DDX_Control(pDX, IDC_RNDHOLE1FLAT_RADIUS, m_Edit_Radius);
	DDX_Control(pDX, IDC_RNDHOLE1FLAT_FLAT, m_Edit_FlatDistanceFromCenter);
	DDX_Control(pDX, IDC_RNDHOLE1FLAT_COLOR, m_Static_LineColor);
}


BEGIN_MESSAGE_MAP(CDlgRndHole2FlatProp, CDialogEx)
	ON_STN_CLICKED(IDC_RNDHOLE1FLAT_COLOR, &CDlgRndHole2FlatProp::OnStnClickedRndhole1flatColor)
END_MESSAGE_MAP()


//--------------------------------------------------
// CDlgRndHole2FlatProp message handlers
//--------------------------------------------------


BOOL CDlgRndHole2FlatProp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Static_LineColor.SetColor(m_pHole->GetLineColor());
	m_Edit_FlatDistanceFromCenter.SetDecimalPlaces(3);
	m_Edit_FlatDistanceFromCenter.SetDoubleValue(m_pHole->GetDistanceFromCenterToFlat() );
	m_Edit_Radius.SetDecimalPlaces(3);
	m_Edit_Radius.SetDoubleValue(m_pHole->GetHoleRadius());
	m_Edit_LineWidth.SetDecimalPlaces(3);
	m_Edit_LineWidth.SetDoubleValue(m_pHole->GetLineWidth());
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgRndHole2FlatProp::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
	case IDOK:
		m_pHole->SetLineColor(m_Static_LineColor.GetColor());
		m_pHole->SetDistanceFromCenterToFlat(m_Edit_FlatDistanceFromCenter.GetDoubleValue());
		m_pHole->SetHoleRadius(m_Edit_Radius.GetDoubleValue());
		m_pHole->SetLineWidth(m_Edit_LineWidth.GetDoubleValue());
		CDialogEx::OnOK();
		break;
	default:
		GetNextDlgTabItem(pW)->SetFocus();
		break;
	}
}

void CDlgRndHole2FlatProp::OnStnClickedRndhole1flatColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}


