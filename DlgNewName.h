#pragma once

// CDlgNewName dialog

class CDlgNewName : public CDialog
{
	//----------------------------------
	// Dialog Data
	//----------------------------------
	CString m_csDialogTitle;
	CString m_csNewName;
	//----------------------------------
	// Dialog Controls
	//----------------------------------
	CEdit m_Edit_Name;
	DECLARE_DYNAMIC(CDlgNewName)
public:
	CDlgNewName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNewName();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void SetDialogTitle(CString& tit) { m_csDialogTitle = tit; }
	void SetNameString(CString& csName) { m_csNewName = csName; }
	CString& GetNameString() {return m_csNewName;}
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NEWNAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
