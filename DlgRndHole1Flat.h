#pragma once


// CDlgRndHole1Flat dialog

class CDlgRndHole1Flat : public CDialogEx
{
	//------------------------------------
	// Data
	//------------------------------------
	CCadHoleRnd1Flat* m_pHole;;
	//------------------------------------
	// Controls
	//------------------------------------
	CStaticBitmap m_Static_Color;
	CEditDecimal m_Edit_LineWidth;
	CEditDecimal m_Edit_Radius;
	CEditDecimal m_Edit_Flat;
	//------------------------------------
	DECLARE_DYNAMIC(CDlgRndHole1Flat)
public:
	CDlgRndHole1Flat(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgRndHole1Flat();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedRndhole1flatColor();
	void SetRndHole1Flat(CCadHoleRnd1Flat* pCHR1F) { m_pHole = pCHR1F; }
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RNDHOLE1FLATPROP };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//--------------------------------------------
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
