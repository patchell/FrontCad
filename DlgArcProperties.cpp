// DlgArcProperties.cpp : implementation file
//

#include "pch.h"


// CDlgArcProperties dialog

IMPLEMENT_DYNAMIC(CDlgArcProperties, CDialog)

CDlgArcProperties::CDlgArcProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ARCPROPERTIES, pParent)
{
	m_pArc = 0;
}

CDlgArcProperties::~CDlgArcProperties()
{
}

void CDlgArcProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LINECOLOR, m_Static_Color);
	DDX_Control(pDX, IDC_EDIT_END_X, m_Edit_EndX);
	DDX_Control(pDX, IDC_EDIT_END_Y, m_Edit_EndY);
	DDX_Control(pDX, IDC_EDIT_LINEWIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_EDIT_START_X, m_Edit_StartX);
	DDX_Control(pDX, IDC_EDIT_START_Y, m_Edit_StartY);
}


BEGIN_MESSAGE_MAP(CDlgArcProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_LINECOLOR, &CDlgArcProperties::OnStnClickedStaticLinecolor)
END_MESSAGE_MAP()


// CDlgArcProperties message handlers


BOOL CDlgArcProperties::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateControls();
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgArcProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			UpdateData();
			CDialog::OnOK();
			break;
	}

}

void CDlgArcProperties::OnStnClickedStaticLinecolor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
		m_Static_Color.SetColor(Dlg.GetColor());
}

void CDlgArcProperties::UpdateControls()
{
	CADObjectTypes Obj;

	m_Edit_EndX.SetDecimalPlaces(3);
	m_Edit_EndX.SetDecimalPlaces(3);
	Obj.pCadObject = m_pArc->FindChildObject(ObjectType::POINT, SubType::ENDPOINT, 0);
	m_Edit_EndX.SetDoubleValue(Obj.pCadPoint->GetX());
	m_Edit_EndY.SetDecimalPlaces(3);
	m_Edit_EndY.SetDoubleValue(Obj.pCadPoint->GetY());

	m_Edit_StartX.SetDecimalPlaces(3);
	Obj.pCadObject = m_pArc->FindChildObject(ObjectType::POINT, SubType::STARTPOINT, 0);
	m_Edit_StartX.SetDoubleValue(Obj.pCadPoint->GetX());
	m_Edit_StartY.SetDecimalPlaces(3);
	m_Edit_StartY.SetDoubleValue(Obj.pCadPoint->GetY());
	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetDoubleValue(m_pArc->GetAttributes().m_LineWidth);
	m_Static_Color.SetColor(m_pArc->GetAttributes().m_colorLine);
}

void CDlgArcProperties::UpdateData()
{
	CADObjectTypes Obj;

	m_pArc->GetAttributes().m_colorLine = m_Static_Color.GetColor();
	m_pArc->GetAttributes().m_LineWidth = m_Edit_Width.GetDoubleValue();
	Obj.pCadObject = m_pArc->FindChildObject(ObjectType::POINT, SubType::ENDPOINT, 0);
	Obj.pCadPoint->SetPoint(
		m_Edit_EndX.GetDoubleValue(),
		m_Edit_EndY.GetDoubleValue()
	);
	Obj.pCadObject = m_pArc->FindChildObject(ObjectType::POINT, SubType::STARTPOINT, 0);
	Obj.pCadPoint->SetPoint(
		m_Edit_StartX.GetDoubleValue(),
		m_Edit_StartY.GetDoubleValue()
	);
}
