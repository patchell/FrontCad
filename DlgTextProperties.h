#pragma once

// CDlgTextProperties dialog

class CDlgTextProperties : public CDialogEx
{
	//----------------------------------------
	// Data
	//----------------------------------------
	CCadText*m_pText;
	//----------------------------------
	// Controls
	//---------------------------------
	CButton m_Button_Font;
	CButton m_Check_Transparent;
	CEditDecimal m_Edit_Angle;
	CEditDecimal m_Edit_Height;
	CEditDecimal m_Edit_FontWidth;
	CEdit m_Edit_TextString;
	CStaticBitmap m_Static_BackGroundColor;
	CStaticBitmap m_Static_TextColor;
	CComboBoxFontWeight m_Combo_FontWeight;
	DECLARE_DYNAMIC(CDlgTextProperties)
public:
	CDlgTextProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTextProperties();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonFont();
	afx_msg void OnStnClickedStaticTextColor();
	afx_msg void OnStnClickedStaticBackgroundColor();
	void SetTextObject(CCadText* pCT) { m_pText = pCT; }

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TEXT_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeComboFontweight();
protected:
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
