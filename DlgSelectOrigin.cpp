// DlgSelectOrigin.cpp : implementation file
//

#include "pch.h"


// CDlgSelectOrigin dialog

IMPLEMENT_DYNAMIC(CDlgSelectOrigin, CDialog)

CDlgSelectOrigin::CDlgSelectOrigin(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_CHANGE_REFERENCE_ORIGIN, pParent)
{
	m_pDoc = 0;
}

CDlgSelectOrigin::~CDlgSelectOrigin()
{
}

void CDlgSelectOrigin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSelectOrigin, CDialog)
	ON_CBN_DBLCLK(IDC_COMBO_SELECT_ORIGIN, &CDlgSelectOrigin::OnDblclkComboSelectOrigin)
END_MESSAGE_MAP()


// CDlgSelectOrigin message handlers


BOOL CDlgSelectOrigin::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSelectOrigin::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}


void CDlgSelectOrigin::OnDblclkComboSelectOrigin()
{
	// TODO: Add your control notification handler code here
}
