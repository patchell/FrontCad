// DlgBitmapProperties.cpp : implementation file
//

#include "pch.h"


// CDlgBitmapProperties dialog

IMPLEMENT_DYNAMIC(CDlgBitmapProperties, CDialog)

CDlgBitmapProperties::CDlgBitmapProperties(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_BITMAP_PROPERTIES, pParent)
{
	m_bDirty = FALSE;
	m_pBitmap = 0;
}

CDlgBitmapProperties::~CDlgBitmapProperties()
{
}

void CDlgBitmapProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_MAINTAIN_ASPECT_RATIO, m_Check_MaintainAspecRation);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_Edit_FileName);
}


BEGIN_MESSAGE_MAP(CDlgBitmapProperties, CDialog)
	ON_BN_CLICKED(IDC_CHECK_MAINTAIN_ASPECT_RATIO, &CDlgBitmapProperties::OnBnClickedCheckMaintainAspectRatio)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_FILE, &CDlgBitmapProperties::OnBnClickedButtonChooseFile)
	ON_MESSAGE((UINT)WindowsMsg::WM_DLG_CONTROL_DIRTY, &CDlgBitmapProperties::OnDlgControlDirty)
END_MESSAGE_MAP()


// CDlgBitmapProperties message handlers


void CDlgBitmapProperties::OnBnClickedCheckMaintainAspectRatio()
{
	m_pBitmap->GetAttributes().m_MaintainAspectRatio = m_Check_MaintainAspecRation.GetCheck();
}

void CDlgBitmapProperties::OnBnClickedButtonChooseFile()
{
	CFileDialog Dlg(true,_T("*.bmp"));

	if (Dlg.DoModal() == IDOK)
	{
		m_pBitmap->GetBitMapFileName() = Dlg.GetPathName();
	}
}

void CDlgBitmapProperties::OnOK()
{
	CWnd *pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
		case IDOK:
			if (IsDirty())
			{
				m_pBitmap->GetAttributes().m_MaintainAspectRatio = m_Check_MaintainAspecRation.GetCheck();
				m_Edit_FileName.GetWindowTextW(m_pBitmap->GetBitMapFileName());
			}
			CDialog::OnOK();
			break;
	}
}


BOOL CDlgBitmapProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Check_MaintainAspecRation.SetCheck(m_pBitmap->GetAttributes().m_MaintainAspectRatio);
	m_Edit_FileName.SetWindowTextW(m_pBitmap->GetBitMapFileName());
	return TRUE;  
}


afx_msg LRESULT CDlgBitmapProperties::OnDlgControlDirty(WPARAM wParam, LPARAM lParam)
{
	m_bDirty = TRUE;
	return 0;
}
