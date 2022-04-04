#pragma once

// CDlgBitmapProperties dialog

class CDlgBitmapProperties : public CDialog
{
	//-----------------------------------------
	// Data
	//-----------------------------------------
	CCadBitmap* m_pBitmap;;
	//-----------------------------------------
	//Copntrols
	//-----------------------------------------
	CButton m_Check_MaintainAspecRation;
	CEdit m_Edit_FileName;
	DECLARE_DYNAMIC(CDlgBitmapProperties)
public:
	CDlgBitmapProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBitmapProperties();
	afx_msg void OnBnClickedCheckMaintainAspectRatio();
	afx_msg void OnBnClickedButtonChooseFile();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BITMAP_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};

extern int DoBitmapPropertiesDlg();
