#pragma once

// CDlgRndRectProperties dialog

class CDlgRndRectProperties : public CDialog
{
	BOOL m_bDirty;
	//----------------------------
	// Data
	//-----------------------------
	CCadRndRect* m_pRndRect;
	//------------------------------
	// Controls
	//------------------------------
	CEditDecimal m_Edit_RadiusX;
	CEditDecimal m_Edit_RadiusY;
	CEditDecimal m_Edit_LineWidth;
	CStaticBitmap m_Static_FillColor;
	CStaticBitmap m_Static_LineColor;
	CButton m_Check_NoFill;
	DECLARE_DYNAMIC(CDlgRndRectProperties)
public:
	CDlgRndRectProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRndRectProperties();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticLinecolor();
	afx_msg void OnBnClickedCheckRndrectNofill();
	void SetRndRect(CCadRndRect* pCRR) { m_pRndRect = pCRR; }
	BOOL IsDirty() { return m_bDirty; }

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROUNDEDRECT_PROPERTIES };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
