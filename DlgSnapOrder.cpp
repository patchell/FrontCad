// DlgSnapOrder.cpp : implementation file
//

#include "pch.h"


// CDlgSnapOrder dialog

IMPLEMENT_DYNAMIC(CDlgSnapOrder, CDialogEx)

CDlgSnapOrder::CDlgSnapOrder(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SNAP_ORDER, pParent)
{

}

CDlgSnapOrder::~CDlgSnapOrder()
{
}

void CDlgSnapOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SNAP_LEVEL_1, m_Static_Order_1);
	DDX_Control(pDX, IDC_STATIC_SNAP_LEVEL_2, m_Static_Order_2);
	DDX_Control(pDX, IDC_STATIC_SNAP_LEVEL_3, m_Static_Order_3);
	DDX_Control(pDX, IDC_STATIC_SNAP_LEVEL_4, m_Static_Order_4);
	DDX_Control(pDX, IDC_STATIC_SNAP_LEVEL_5, m_Static_Order_5);
	DDX_Control(pDX, IDC_EDIT_SNAP_LEVEL_1, m_Edit_Order_1);
	DDX_Control(pDX, IDC_EDIT_SNAP_LEVEL_2, m_Edit_Order_2);
	DDX_Control(pDX, IDC_EDIT_SNAP_LEVEL_3, m_Edit_Order_3);
	DDX_Control(pDX, IDC_EDIT_SNAP_LEVEL_4, m_Edit_Order_4);
	DDX_Control(pDX, IDC_EDIT_SNAP_LEVEL_5, m_Edit_Order_5);
}


BEGIN_MESSAGE_MAP(CDlgSnapOrder, CDialogEx)
END_MESSAGE_MAP()


// CDlgSnapOrder message handlers


BOOL CDlgSnapOrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Edit_Order_1.SetFocus();
	return FALSE;
}


void CDlgSnapOrder::OnOK()
{
	CWnd* pWnd;
	int Id;

	pWnd = GetFocus();
	Id = pWnd->GetDlgCtrlID();
	switch (Id)
	{
	case IDOK:
		CDialogEx::OnOK();
		break;
	case IDC_EDIT_SNAP_LEVEL_1:
	case IDC_EDIT_SNAP_LEVEL_2:
	case IDC_EDIT_SNAP_LEVEL_3:
	case IDC_EDIT_SNAP_LEVEL_4:
	case IDC_EDIT_SNAP_LEVEL_5:
		pWnd = GetNextDlgTabItem(pWnd);
		pWnd->SetFocus();
		break;
	}
}
