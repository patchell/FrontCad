// DlgArrowProperties.cpp : implementation file
//
#include "pch.h"


// CDlgArrowProperties dialog

IMPLEMENT_DYNAMIC(CDlgArrowProperties, CDialog)

CDlgArrowProperties::CDlgArrowProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ARROW_ATTRIBUTES, pParent)
{
	m_pArrow = 0;
}

CDlgArrowProperties::~CDlgArrowProperties()
{
}

void CDlgArrowProperties::SetArrow(CCadArrow* pA)
{
	m_pArrow = pA;
	m_pArrow->CopyAttributesTo(&m_OriginalAttributes);
}

void CDlgArrowProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_Static_Color);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_Static_Preview);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_Edit_Width);
	DDX_Control(pDX, IDC_EDIT_L1, m_Edit_Arrow_L1);
	DDX_Control(pDX, IDC_EDIT_L2, m_Edit_Arrow_L2);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_Edit_LineWidth);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_Static_LineColor);
}

LRESULT CDlgArrowProperties::OnArrowAttrbValues(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}


BEGIN_MESSAGE_MAP(CDlgArrowProperties, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, &CDlgArrowProperties::OnStnClickedStaticColor)
	ON_MESSAGE(UINT(WindowsMsg::WM_ARROWATTRB), &CDlgArrowProperties::OnArrowAttrbValues)
	ON_STN_CLICKED(IDC_STATIC_LINE_COLOR, &CDlgArrowProperties::OnStnClickedStaticLineColor)
END_MESSAGE_MAP()


// CDlgArrowProperties message handlers


BOOL CDlgArrowProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit_Width.SetDecimalPlaces(3);
	m_Edit_Width.SetupMessaging(this, WindowsMsg::WM_ARROWATTRB, WMarrowSubMsg::WM_ARROWATTRB_VALUE_W);
	m_Static_Preview.Init();
	m_Static_Preview.SetPreviewObject(m_pArrow);
	m_Static_Preview.SetOffsetMode(StaticPreivewMode::STATICPREVIEW_OFFSET_X10);
	UpdateControls();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgArrowProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDC_EDIT_L1:
		case IDC_EDIT_L2:
		case IDC_EDIT_WIDTH:
			m_pArrow->ApplyParameters(
				m_Edit_Arrow_L1.GetDoubleValue(),
				m_Edit_Arrow_L2.GetDoubleValue(),
				m_Edit_Width.GetDoubleValue()
			);
			m_Static_Preview.Invalidate();
			break;
		case IDOK:
			UpdateArrowData();
			CDialog::OnOK();
			break;
	}
}

void CDlgArrowProperties::OnStnClickedStaticColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
	{
		m_Static_Color.SetColor(Dlg.GetColor());
		m_pArrow->SetFillColor(Dlg.GetColor());
		m_Static_Preview.Invalidate();
	}
}

void CDlgArrowProperties::OnCancel()
{
	// Undo any changes to the object
	m_pArrow->CopyAttributesFrom(&m_OriginalAttributes);
	CDialog::OnCancel();
}

void CDlgArrowProperties::UpdateControls()
{
	CADObjectTypes Obj;

	if (GetArrow())
	{
		m_Static_Preview.AutoScale();
		m_Edit_Width.SetDoubleValue(GetArrow()->GetW());
		m_Edit_Arrow_L1.SetDoubleValue(GetArrow()->GetL1());
		m_Edit_Arrow_L2.SetDoubleValue(GetArrow()->GetL2());
		m_Static_Color.SetColor(GetArrow()->GetAttributes().m_colorLine);
		m_Static_Preview.Invalidate();
	}
}

void CDlgArrowProperties::UpdateArrowData()
{
	m_pArrow->SetFillColor(m_Static_Color.GetColor());
	m_pArrow->SetLineColor(m_Static_LineColor.GetColor());
	m_pArrow->ApplyParameters(
		m_Edit_Arrow_L1.GetDoubleValue(),
		m_Edit_Arrow_L2.GetDoubleValue(),
		m_Edit_Width.GetDoubleValue()
	);
}

void CDlgArrowProperties::OnStnClickedStaticLineColor()
{
	CColorDialog Dlg;

	if (IDOK == Dlg.DoModal())
	{
		m_Static_Color.SetColor(Dlg.GetColor());
		m_pArrow->SetLineColor(Dlg.GetColor());
		m_Static_Preview.Invalidate();
	}
}

