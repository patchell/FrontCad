// DlgDimensionPropertiex.cpp : implementation file
//----------------------------------------------------

#include "pch.h"

// CDlgDimensionPropertiex dialog

IMPLEMENT_DYNAMIC(CDlgDimensionPropertiex, CDialogEx)

CDlgDimensionPropertiex::CDlgDimensionPropertiex(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DIMENSION_PROPERTIES, pParent)
{
	m_pDimension = 0;
}

CDlgDimensionPropertiex::~CDlgDimensionPropertiex()
{
}

void CDlgDimensionPropertiex::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_Static_LineColor);
	DDX_Control(pDX, IDC_STATIC_TEXT_COLOR, m_Static_TextColor);
	DDX_Control(pDX, IDC_STATIC_BACKGROUND_COLOR, m_Static_BackgroundColor);
	DDX_Control(pDX, IDC_EDIT_TEXT_HEIGHT, m_Edit_TextHeight);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_Edit_LineWidth);
	DDX_Control(pDX, IDC_EDIT_DIM_TEXT, m_Edit_DimensionText);
	DDX_Control(pDX, IDC_EDIT_EXTENSION_LENGTH, m_Edit_ExtensionLength);
	DDX_Control(pDX, IDC_STATIC_DIM_PREVIW, m_Static_DimPreview);
}


BEGIN_MESSAGE_MAP(CDlgDimensionPropertiex, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_TEXT_HEIGHT, &CDlgDimensionPropertiex::OnEnChangeEditTextHeight)
	ON_STN_CLICKED(IDC_STATIC_BACKGROUND_COLOR, &CDlgDimensionPropertiex::OnClickedStaticBackgroundColor)
	ON_STN_CLICKED(IDC_STATIC_DIM_PREVIW, &CDlgDimensionPropertiex::OnClickedStaticDimPreviw)
	ON_STN_CLICKED(IDC_STATIC_LINE_COLOR, &CDlgDimensionPropertiex::OnClickedStaticLineColor)
END_MESSAGE_MAP()


// CDlgDimensionPropertiex message handlers


void CDlgDimensionPropertiex::OnEnChangeEditTextHeight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


BOOL CDlgDimensionPropertiex::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgDimensionPropertiex::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}


void CDlgDimensionPropertiex::OnClickedStaticBackgroundColor()
{
	// TODO: Add your control notification handler code here
}


void CDlgDimensionPropertiex::OnClickedStaticDimPreviw()
{
	// TODO: Add your control notification handler code here
}


void CDlgDimensionPropertiex::OnClickedStaticLineColor()
{
	// TODO: Add your control notification handler code here
}

