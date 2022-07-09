//------------------------------------------------
// DlgTextProperties.cpp : implementation file
//------------------------------------------------

#include "pch.h"


//------------------------------------------------
// CDlgTextProperties dialog
//------------------------------------------------

IMPLEMENT_DYNAMIC(CDlgTextProperties, CDialogEx)

CDlgTextProperties::CDlgTextProperties(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TEXT_PROPERTIES, pParent)
{
	m_bDirty = FALSE;
	m_pText = 0;
}

CDlgTextProperties::~CDlgTextProperties()
{
}

void CDlgTextProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_FONT, m_Button_Font);
	DDX_Control(pDX, IDC_CHECK_TRANSPARENT, m_Check_Transparent);
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_Edit_Angle);
	DDX_Control(pDX, IDC_EDIT_TEXT_HEIGHT, m_Edit_Height);
	DDX_Control(pDX, IDC_EDIT_TEXT_STRING, m_Edit_TextString);
	DDX_Control(pDX, IDC_EDIT_TEXT_WIDTH, m_Edit_FontWidth);
	DDX_Control(pDX, IDC_STATIC_BACKGROUND_COLOR, m_Static_BackGroundColor);
	DDX_Control(pDX, IDC_STATIC_TEXT_COLOR, m_Static_TextColor);
	DDX_Control(pDX, IDC_COMBO_FONTWEIGHT, m_Combo_FontWeight);
}


BEGIN_MESSAGE_MAP(CDlgTextProperties, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FONT, &CDlgTextProperties::OnBnClickedButtonFont)
	ON_STN_CLICKED(IDC_STATIC_TEXT_COLOR, &CDlgTextProperties::OnStnClickedStaticTextColor)
	ON_STN_CLICKED(IDC_STATIC_BACKGROUND_COLOR, &CDlgTextProperties::OnStnClickedStaticBackgroundColor)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTWEIGHT, &CDlgTextProperties::OnSelchangeComboFontweight)
	ON_MESSAGE((UINT)WindowsMsg::WM_DLG_CONTROL_DIRTY, &CDlgTextProperties::OnDlgControlDirty)
END_MESSAGE_MAP()


// CDlgTextProperties message handlers


BOOL CDlgTextProperties::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//---------------------------------------
	// Initialize ComboBox
	//---------------------------------------
	m_Combo_FontWeight.Init();
	m_Combo_FontWeight.SetFontWeight(m_pText->GetAttributes().m_fontWeight);

	m_Edit_Height.SetDecimalPlaces(3);
	m_Edit_Height.SetDoubleValue(m_pText->GetAttributes().m_fontWeight);

	m_Edit_FontWidth.SetDecimalPlaces(3);
	m_Edit_FontWidth.SetDoubleValue(m_pText->GetAttributes().m_fontWidth);

	m_Edit_Angle.SetDecimalPlaces(1);
	m_Edit_Angle.SetDoubleValue(m_pText->GetAngle());

	m_Static_BackGroundColor.SetColor(m_pText->GetAttributes().m_colorBK);

	m_Static_TextColor.SetColor(m_pText->GetAttributes().m_colorText);

	m_Button_Font.SetWindowTextW(m_pText->GetAttributes().m_csFontName);

	m_Check_Transparent.SetCheck(m_pText->GetTransparent());

	m_Edit_TextString.SetWindowTextW(m_pText->GetText());

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgTextProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int id = pW->GetDlgCtrlID();
	switch (id)
	{
		case IDOK:
			CDialogEx::OnOK();
			//---------------------------------
			// Update Data from controls
			//---------------------------------
			if (IsDirty())
			{
				m_pText->SetAngle(m_Edit_Angle.GetDoubleValue());
				m_pText->SetBackgroundColor(m_Static_BackGroundColor.GetColor());
				m_pText->SetTextColor(m_Static_TextColor.GetColor());
				m_pText->SetFontHeight(m_Edit_Height.GetDoubleValue());
				m_pText->SetFontWidth(m_Edit_FontWidth.GetDoubleValue());
				m_Button_Font.GetWindowTextW(m_pText->GetFontName());
				m_pText->SetTransparent(m_Check_Transparent.GetCheck());
				m_pText->SetFontWeight(m_Combo_FontWeight.GetFontWeight());
				m_Edit_TextString.GetWindowTextW(m_pText->GetText());
			}
			break;
	}
}

void CDlgTextProperties::OnBnClickedButtonFont()
{
	CFontDialog Dlg;
	int Id;
	LOGFONT lF;
	CString csFontName;

	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		Dlg.GetCurrentFont(&lF);
		m_Button_Font.SetWindowTextW(lF.lfFaceName);
		csFontName = CString(lF.lfFaceName);
		m_pText->SetFontName(csFontName);
	}
}

void CDlgTextProperties::OnStnClickedStaticTextColor()
{
	CColorDialog Dlg;
	int Id;

	Id = Dlg.DoModal();
	if (IDOK == Id)
		m_Static_TextColor.SetColor(Dlg.GetColor());
}


void CDlgTextProperties::OnStnClickedStaticBackgroundColor()
{
	CColorDialog Dlg;
	int Id;

	Id = Dlg.DoModal();
	if (IDOK == Id)
		m_Static_BackGroundColor.SetColor(Dlg.GetColor());
}

void CDlgTextProperties::OnSelchangeComboFontweight()
{
	// TODO: Add your control notification handler code here
}


afx_msg LRESULT CDlgTextProperties::OnDlgControlDirty(WPARAM wParam, LPARAM lParam)
{
	m_bDirty = TRUE;
	return 0;
}
