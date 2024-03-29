#pragma once

// CDlgRndHole2FlatProp dialog

class CDlgRndHole2FlatProp : public CDialogEx
{
	//-------------------------------------
	// Data
	//-------------------------------------
	CCadHoleRnd2Flat *m_pHole;
	//-------------------------------------
	// Controls
	//-------------------------------------
	CEditDecimal m_Edit_LineWidth;
	CEditDecimal m_Edit_Radius;
	CEditDecimal m_Edit_FlatDistanceFromCenter;
	CStaticBitmap m_Static_LineColor;
	DECLARE_DYNAMIC(CDlgRndHole2FlatProp)
public:
	CDlgRndHole2FlatProp(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgRndHole2FlatProp();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedRndhole1flatColor();
	void SetRndHole2Flat(CCadHoleRnd2Flat* pCHR) { m_pHole = pCHR; }
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RNDHOLE2FLATPROP };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};

