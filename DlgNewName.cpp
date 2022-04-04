//---------------------------------------
// DlgNewName.cpp : implementation file
//---------------------------------------

#include "pch.h"


// CDlgNewName dialog

IMPLEMENT_DYNAMIC(CDlgNewName, CDialog)

CDlgNewName::CDlgNewName(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_NEWNAME, pParent)
{
}

CDlgNewName::~CDlgNewName()
{
}

void CDlgNewName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NEWNAME, m_Edit_Name);
}


BEGIN_MESSAGE_MAP(CDlgNewName, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgNewName::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNewName message handlers

BOOL CDlgNewName::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowTextW(m_csDialogTitle);
	m_Edit_Name.SetWindowTextW(m_csNewName);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgNewName::OnBnClickedOk()
{
	m_Edit_Name.GetWindowTextW(m_csNewName);
	CDialog::OnOK();
}
