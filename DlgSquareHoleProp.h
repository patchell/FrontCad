#pragma once

// CDlgSquareHoleProp dialog

class CDlgSquareHoleProp : public CDialogEx
{
	//-------------------------------------
	// Data
	//-------------------------------------
	CCadHoleRect *m_pHole;
	//-------------------------------------
	// Controls
	//-------------------------------------
	CStaticBitmap m_Static_LineColor;
	CEditDecimal m_Edit_Radius;
	CEditDecimal m_Edit_Hiegth;
	CEditDecimal m_Edit_LineWidth;
	//-------------------------------------
	DECLARE_DYNAMIC(CDlgSquareHoleProp)
public:
	CDlgSquareHoleProp(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSquareHoleProp();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticRoundholeColor();
	void SetRectHole(CCadHoleRect* CCHR) { m_pHole = CCHR; }
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgSquareHoleProp };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
