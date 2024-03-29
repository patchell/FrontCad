#pragma once

// CDlgRoundHoleProp dialog

class CDlgRoundHoleProp : public CDialog
{
	//---------------------------------
	// Data
	//---------------------------------
	CCadHoleRound* m_pHole;
	//---------------------------------
	// Controls
	//---------------------------------
	CEditDecimal m_Edit_Radius;
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_Color;
	DECLARE_DYNAMIC(CDlgRoundHoleProp)
public:
	CDlgRoundHoleProp(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgRoundHoleProp();
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgRoundHoleProp };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnStnClickedStaticRoundholeColor();
	void SetRoundHole(CCadHoleRound* pCHR) { m_pHole = pCHR; }
protected:
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};

