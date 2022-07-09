#pragma once


// CDlgPolygonProperties dialog

class CDlgPolygonProperties : public CDialog
{
	BOOL m_bDirty;
	//--------------------
	// Data
	//---------------------
	CCadPolygon* m_pPolygon;
	//---------------------
	// Controls
	//---------------------
	CButton m_Check_NoFill;
	CEditDecimal m_Edit_LineWidth;
	CStaticBitmap m_Static_FillColor;
	CStaticBitmap m_Static_LineColor;
	DECLARE_DYNAMIC(CDlgPolygonProperties)

public:
	CDlgPolygonProperties(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgPolygonProperties();
	BOOL IsDirty() { return m_bDirty; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POLY_EDPROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void SetPolygon(CCadPolygon* pCP) { m_pPolygon = pCP; }
	afx_msg void OnClickedStaticFillColor();
	afx_msg void OnClickedStaticLineColor();
protected:
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
