#pragma once

// CDlgArrowProperties dialog

class CDlgArrowProperties : public CDialog
{
	//--------------------------------
	// Data
	//--------------------------------
	CCadArrow* m_pArrow;
	SArrowAttributes m_OriginalAttributes;
	//--------------------------------
	// Controls
	//--------------------------------
	CStaticBitmap m_Static_Color;
	CStaticPreview m_Static_Preview;
	CEditDecimal m_Edit_Width;
	CEditDecimal m_Edit_Arrow_L1;
	CEditDecimal m_Edit_Arrow_L2;
	CEditDecimal m_Edit_LineWidth;
	CStaticBitmap m_Static_LineColor;
	DECLARE_DYNAMIC(CDlgArrowProperties)
public:
	CDlgArrowProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgArrowProperties();
	void SetArrow(CCadArrow* pA);
	CCadArrow* GetArrow() const { return m_pArrow; }
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticColor();
	virtual void OnCancel();
	void UpdateControls();
	void UpdateArrowData();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ARROW_ATTRIBUTES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnArrowAttrbValues(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStnClickedStaticLineColor();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};

