#pragma once

// CDlgArcCentProperies dialog

class CDlgArcCentProperies : public CDialog
{
	BOOL m_bDirty;
	//--------------------------
	// Data
	//--------------------------
	CCadArcCent* m_pArc;
	//--------------------------
	// Controls
	//--------------------------
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_Color;
	CEditDecimal m_Edit_StartX;
	CEditDecimal m_Edit_StartY;
	CEditDecimal m_Edit_EndX;
	CEditDecimal m_Edit_EndY;
	DECLARE_DYNAMIC(CDlgArcCentProperies)
public:
	CDlgArcCentProperies(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgArcCentProperies();
	void SetArcCentered(CCadArcCent* pAC) { m_pArc = pAC; }
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	BOOL IsDirty() { return m_bDirty; }
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ARCCENTPROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	void UpdateControls();
	void UpdateData();
protected:
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};

