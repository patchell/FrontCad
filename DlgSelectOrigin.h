#pragma once
#include "afxdialogex.h"


// CDlgSelectOrigin dialog
class CFrontCadDoc;

class CDlgSelectOrigin : public CDialog
{
	CFrontCadDoc* m_pDoc;

	DECLARE_DYNAMIC(CDlgSelectOrigin)

public:
	CDlgSelectOrigin(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSelectOrigin();
	void SetDoc(CFrontCadDoc* pDoc) { m_pDoc = pDoc; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHANGE_REFERENCE_ORIGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkComboSelectOrigin();
};
