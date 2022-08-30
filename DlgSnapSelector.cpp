// DlgSnapSelector.cpp : implementation file
//

#include "pch.h"


// CDlgSnapSelector dialog

IMPLEMENT_DYNAMIC(CDlgSnapSelector, CDialogEx)

CDlgSnapSelector::CDlgSnapSelector(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_SNAP_TYPE, pParent)
{
	m_pSnapTypesActive = 0;
	m_pSnapTypesPool = 0;
}

CDlgSnapSelector::~CDlgSnapSelector()
{
}

void CDlgSnapSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SNAP_LIST, m_Listbox_SnapList);
	DDX_Control(pDX, IDC_LISTSNAP_POOL, m_Listbox_Snap_Pool);
	DDX_Control(pDX, IDC_BUTTON_SNAP_SEL_TO, m_Button_To);
	DDX_Control(pDX, IDC_BUTTON_SNAP_LIST_FROM, m_Button_From);
}


BEGIN_MESSAGE_MAP(CDlgSnapSelector, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SNAP_LIST_FROM, &CDlgSnapSelector::OnClickedButtonSnapListFrom)
	ON_BN_CLICKED(IDC_BUTTON_SNAP_SEL_TO, &CDlgSnapSelector::OnClickedButtonSnapSelTo)
	ON_LBN_SELCHANGE(IDC_LISTSNAP_POOL, &CDlgSnapSelector::OnSelchangeListsnapPool)
	ON_LBN_SELCHANGE(IDC_LIST_SNAP_LIST, &CDlgSnapSelector::OnSelchangeListSnapList)
	ON_LBN_DBLCLK(IDC_LIST_SNAP_LIST, &CDlgSnapSelector::OnDblclkListSnapList)
	ON_LBN_DBLCLK(IDC_LISTSNAP_POOL, &CDlgSnapSelector::OnDblclkListsnapPool)
END_MESSAGE_MAP()


// CDlgSnapSelector message handlers


void CDlgSnapSelector::OnClickedButtonSnapListFrom()
{
	// TODO: Add your control notification handler code here
}


void CDlgSnapSelector::OnClickedButtonSnapSelTo()
{
	// TODO: Add your control notification handler code here
}


BOOL CDlgSnapSelector::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSnapSelector::OnOK()
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
	}
}


void CDlgSnapSelector::OnSelchangeListsnapPool()
{
	// TODO: Add your control notification handler code here
}


void CDlgSnapSelector::OnSelchangeListSnapList()
{
	// TODO: Add your control notification handler code here
}


void CDlgSnapSelector::OnDblclkListSnapList()
{
	// TODO: Add your control notification handler code here
}


void CDlgSnapSelector::OnDblclkListsnapPool()
{
	// TODO: Add your control notification handler code here
}
