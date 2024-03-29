// DlgSquareHoleProp.cpp : implementation file
//

#include "pch.h"


// CDlgSquareHoleProp dialog

IMPLEMENT_DYNAMIC(CDlgSquareHoleProp, CDialogEx)

CDlgSquareHoleProp::CDlgSquareHoleProp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDlgSquareHoleProp, pParent)
{
	m_pHole = 0;
}

CDlgSquareHoleProp::~CDlgSquareHoleProp()
{
}

void CDlgSquareHoleProp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ROUNDHOLE_COLOR, m_Static_LineColor);
	DDX_Control(pDX, IDC_EDIT_ROUNDHOLE_RADIUS, m_Edit_Radius);
	DDX_Control(pDX, IDC_EDIT_ROUNDHOLE_WIDTH, m_Edit_LineWidth);
	DDX_Control(pDX, IDC_EDIT_SQUARE_HOLE_HIEGTH, m_Edit_Hiegth);
}


BEGIN_MESSAGE_MAP(CDlgSquareHoleProp, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_ROUNDHOLE_COLOR, &CDlgSquareHoleProp::OnStnClickedStaticRoundholeColor)
END_MESSAGE_MAP()


// CDlgSquareHoleProp message handlers


BOOL CDlgSquareHoleProp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Edit_Radius.SetDecimalPlaces(3);
	m_Edit_Radius.SetDoubleValue(m_pHole->GetAttributes().m_HoleWidth);
	m_Edit_Hiegth.SetDecimalPlaces(3);
	m_Edit_Hiegth.SetDoubleValue(m_pHole->GetAttributes().m_HoleHeight);
	m_Edit_LineWidth.SetDecimalPlaces(3);
	m_Edit_LineWidth.SetDoubleValue(m_pHole->GetLineWidth());
	m_Static_LineColor.SetColor(m_pHole->GetLineColor());
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgSquareHoleProp::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
	case IDOK:
		m_pHole->SetLineColor(m_Static_LineColor.GetColor());
		m_pHole->SetHoleRadius(m_Edit_Radius.GetDoubleValue());
		m_pHole->GetAttributes().m_HoleHeight = m_Edit_Hiegth.GetDoubleValue();
		m_pHole->SetLineWidth(m_Edit_LineWidth.GetDoubleValue());
		CDialogEx::OnOK();
		break;
	default:
		GetNextDlgTabItem(pW)->SetFocus();
		break;
	}
}

void CDlgSquareHoleProp::OnStnClickedStaticRoundholeColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_LineColor.SetColor(Dlg.GetColor());
}

