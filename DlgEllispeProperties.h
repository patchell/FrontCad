#pragma once

// CDlgEllispeProperties dialog

class CDlgEllispeProperties : public CDialog
{
	//-------------------------------------
	//Data
	//-------------------------------------
	CCadEllipse* m_pEllipse;
	//-------------------------------------
	//Controls
	//-------------------------------------
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_FillColor;
	CStaticBitmap m_Static_LineColor;
	CButton m_Check_NoFill;
	DECLARE_DYNAMIC(CDlgEllispeProperties)
public:
	CDlgEllispeProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEllispeProperties();
	void SetEllipse(CCadEllipse* pCE) { m_pEllipse = pCE; }
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticLinecolor();
	afx_msg void OnBnClickedCheckEllipseNofill();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ELLIPSE_PROPERTIES };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
