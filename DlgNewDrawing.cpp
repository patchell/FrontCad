// DlgNewDrawing.cpp : implementation file
//

#include "pch.h"


// CDlgNewDrawing dialog

IMPLEMENT_DYNAMIC(CDlgNewDrawing, CDialogEx)

CDlgNewDrawing::CDlgNewDrawing(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_NEW_DRAWING, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CDlgNewDrawing::~CDlgNewDrawing()
{
}

void CDlgNewDrawing::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DRAWINGSIZE_H, m_Edit_DrawingSizeH);
	DDX_Control(pDX, IDC_EDIT_DRAWOMGSOZE_W, m_Edit_DrawingSizeW);
	DDX_Control(pDX, IDC_EDIT_NEWDRAWING_pAPERSIZE_h, m_Edit_PaperSizeH);
	DDX_Control(pDX, IDC_EDIT_NEWDRAWING_PAPERSIZE_W, m_Edit_PaperSizeW);
	DDX_Control(pDX, IDC_EDIT_NEWDRAWON_NAME, m_Edit_DrawingName);
}


BEGIN_MESSAGE_MAP(CDlgNewDrawing, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_EXISTING, &CDlgNewDrawing::OnBnClickedButtonOpenExisting)
END_MESSAGE_MAP()


// CDlgNewDrawing message handlers


BOOL CDlgNewDrawing::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Edit_DrawingSizeH.SetDecimalPlaces(3);
	m_Edit_DrawingSizeH.SetDoubleValue(m_DrawingSizeY);
	m_Edit_DrawingSizeW.SetDecimalPlaces(3);
	m_Edit_DrawingSizeW.SetDoubleValue(m_DrawingSizeX);
	m_Edit_PaperSizeH.SetDecimalPlaces(3);
	m_Edit_PaperSizeH.SetDoubleValue(m_PaperSizeY);
	m_Edit_PaperSizeW.SetDecimalPlaces(3);
	m_Edit_PaperSizeW.SetDoubleValue(m_PaperSizeX);
	m_Edit_DrawingName.SetWindowTextW(m_csDocName);
	m_Edit_DrawingName.SetFocus();
	return FALSE;  
}


void CDlgNewDrawing::OnOK()
{
	CWnd* pW = GetFocus();
	int id = pW->GetDlgCtrlID();

	switch (id)
	{
	case IDC_EDIT_NEWDRAWON_NAME:
		break;
	case IDC_EDIT_NEWDRAWING_pAPERSIZE_h:
		break;
	case IDC_EDIT_NEWDRAWING_PAPERSIZE_W:
		break;
	case IDC_EDIT_DRAWINGSIZE_H:
		break;
	case IDC_EDIT_DRAWOMGSOZE_W:
		break;
	case IDOK:
		m_PaperSizeX = m_Edit_PaperSizeW.GetDoubleValue();
		m_PaperSizeY = m_Edit_PaperSizeH.GetDoubleValue();
		m_DrawingSizeX = m_Edit_DrawingSizeW.GetDoubleValue();
		m_DrawingSizeY = m_Edit_DrawingSizeH.GetDoubleValue();
		m_Edit_DrawingName.GetWindowTextW(m_csDocName);
		CDialogEx::OnOK();
		break;
	}
}


void CDlgNewDrawing::OnBnClickedButtonOpenExisting()
{
	// TODO: Add your control notification handler code here
}
