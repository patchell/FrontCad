#pragma once

// CDlgArcProperties dialog

class CDlgArcProperties : public CDialog
{
	//---------------------------
	// Data
	//----------------------------
	CCadArc* m_pArc;
	//---------------------------
	// Controls
	//---------------------------
	CStaticBitmap m_Static_Color;
	CEditDecimal m_Edit_EndX;
	CEditDecimal m_Edit_EndY;
	CEditDecimal m_Edit_Width;
	CEditDecimal m_Edit_StartX;
	CEditDecimal m_Edit_StartY;
	DECLARE_DYNAMIC(CDlgArcProperties)
public:
	CDlgArcProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgArcProperties();
	CCadArc* GetArc() { return m_pArc; }
	void SetArc(CCadArc* pA) { m_pArc = pA; }
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticLinecolor();
	void UpdateControls();
	void UpdateData();
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ARCPROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

