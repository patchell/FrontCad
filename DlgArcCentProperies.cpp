// DlgArcCentProperies.cpp : implementation file
//

#include "pch.h"


// CDlgArcCentProperies dialog

IMPLEMENT_DYNAMIC(CDlgArcCentProperies, CDialog)

CDlgArcCentProperies::CDlgArcCentProperies(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ARCCENTPROPERTIES, pParent)
{
	m_pArc = 0;
}

CDlgArcCentProperies::~CDlgArcCentProperies()
{
}

void CDlgArcCentProperies::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINEWIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_STATIC_LINECOLOR, m_Static_Color);
	DDX_Control(pDX, IDC_EDIT_START_X, m_Edit_StartX);
	DDX_Control(pDX, IDC_EDIT_START_Y, m_Edit_StartY);
	DDX_Control(pDX, IDC_EDIT_END_X, m_Edit_EndX);
	DDX_Control(pDX, IDC_EDIT_END_Y, m_Edit_EndY);
}


BEGIN_MESSAGE_MAP(CDlgArcCentProperies, CDialog)
	ON_STN_CLICKED(IDC_STATIC_LINECOLOR, &CDlgArcCentProperies::OnStnClickedStaticLinecolor)
END_MESSAGE_MAP()

//-------------------------------------------------
// CDlgArcCentProperies message handlers
//-------------------------------------------------


BOOL CDlgArcCentProperies::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateControls();
	return TRUE;
}

void CDlgArcCentProperies::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
	case IDOK:
		UpdateControls();
		CDialog::OnOK();
		break;
	}
}

void CDlgArcCentProperies::OnStnClickedStaticLinecolor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_Color.SetColor(Dlg.GetColor());
}

void CDlgArcCentProperies::UpdateControls()
{
	m_Edit_EndX.SetDecimalPlaces(3);
	m_Edit_EndX.SetDoubleValue(m_pArc->GetStartPoint().dX);
	m_Edit_EndY.SetDecimalPlaces(3);
	m_Edit_EndY.SetDoubleValue(m_pArc->GetStartPoint().dY);
	m_Edit_StartX.SetDecimalPlaces(3);
	m_Edit_StartX.SetDoubleValue(m_pArc->GetStartPoint().dX);
	m_Edit_StartY.SetDecimalPlaces(3);
	m_Edit_StartY.SetDoubleValue(m_pArc->GetStartPoint().dX);
	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetDoubleValue(m_pArc->GetAttributes().m_LineWidth);
	m_Static_Color.SetColor(m_pArc->GetAttributes().m_colorLine);
}


void CDlgArcCentProperies::UpdateData()
{
	m_pArc->GetAttributes().m_colorLine = m_Static_Color.GetColor();
	m_pArc->GetAttributes().m_LineWidth = m_Edit_Width.GetValue();
	m_pArc->SetEndPoint(
		CDoublePoint(
			m_Edit_EndX.GetDoubleValue(), 
			m_Edit_EndY.GetDoubleValue()
		)
	);
	m_pArc->SetStartPoint(
		CDoublePoint(
			m_Edit_StartX.GetDoubleValue(), 
			m_Edit_StartY.GetDoubleValue()
		)
	);
}
