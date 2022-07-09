#pragma once


// CDlgRectProperties dialog

class CDlgRectProperties : public CDialog
{
	BOOL m_bDirty;
	//------------------------------------------
	// Data
	//-------------------------------------------
	CCadRect* m_pRect;
	//-------------------------------------------
	// Controlls
	//-------------------------------------------
	CEditDecimal m_Edit_LineWidth;
	CStaticBitmap m_Static_FillColor;
	CStaticBitmap m_Static_LineColor;
	CButton m_Check_NoFill;
	DECLARE_DYNAMIC(CDlgRectProperties)
public:
	CDlgRectProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRectProperties();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickedIidcCheckRectNofill();
	void SetRectangle(CCadRect* pCR) { m_pRect = pCR; }
	BOOL IsDirty() { return m_bDirty; }
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RECTPROPERTIES };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
