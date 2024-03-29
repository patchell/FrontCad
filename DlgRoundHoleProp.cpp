// DlgRoundHoleProp.cpp : implementation file
//

#include "pch.h"


// CDlgRoundHoleProp dialog

IMPLEMENT_DYNAMIC(CDlgRoundHoleProp, CDialog)

CDlgRoundHoleProp::CDlgRoundHoleProp(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CDlgRoundHoleProp, pParent)
{
	m_pHole = 0;
}

CDlgRoundHoleProp::~CDlgRoundHoleProp()
{
}

void CDlgRoundHoleProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ROUNDHOLE_RADIUS, m_Edit_Radius);
	DDX_Control(pDX, IDC_EDIT_ROUNDHOLE_WIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATIC_ROUNDHOLE_COLOR, m_Static_Color);
}


BEGIN_MESSAGE_MAP(CDlgRoundHoleProp, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgRoundHoleProp::OnBnClickedOk)
	ON_STN_CLICKED(IDC_STATIC_ROUNDHOLE_COLOR, &CDlgRoundHoleProp::OnStnClickedStaticRoundholeColor)
END_MESSAGE_MAP()


// CDlgRoundHoleProp message handlers


BOOL CDlgRoundHoleProp::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_Radius.SetDecimalPlaces(3);
	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Radius.SetDoubleValue(m_pHole->GetHoleRadius());
	m_Edit_Width.SetDoubleValue(m_pHole->GetLineWidth());
	m_Static_Color.SetColor(m_pHole->GetLineColor());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRoundHoleProp::OnBnClickedOk()
{
	CWnd* pW = GetFocus();
	int Id = pW->GetDlgCtrlID();

	switch (Id)
	{
	case IDOK:
		m_pHole->SetLineColor(m_Static_Color.GetColor());
		m_pHole->SetHoleRadius(m_Edit_Radius.GetDoubleValue());
		m_pHole->SetLineWidth(m_Edit_Width.GetDoubleValue());
		CDialog::OnOK();
		break;
	default:
		break;
	}
}


void CDlgRoundHoleProp::OnStnClickedStaticRoundholeColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_Color.SetColor(Dlg.GetColor());

}

