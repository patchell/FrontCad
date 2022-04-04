#pragma once


// CDlgObjectEnable dialog

class CDlgObjectEnable : public CDialogEx
{
	CButton m_Check_ArcEnable;
	CButton m_Check_ArcCenteredEnable;
	CButton m_Check_ArrowEnable;
	CButton m_Check_DimensionEnable;
	CButton m_Check_EllipseEnable;
	CButton m_Check_FillPolygonEnable;
	CButton m_Check_LineEnable;
	CButton m_Check_OneFlatHoleEnable;
	CButton m_Check_OriginEnable;
	CButton m_Check_PolygonEnable;
	CButton m_Check_RectEnable;
	CButton m_Check_RectHoleEnable;
	CButton m_CheckRoundedRectEnable;
	CButton m_Check_RoundHole;
	CButton m_Check_TextEnable;
	CButton m_Check_TwoFlatRoundHoleEnable;
	DECLARE_DYNAMIC(CDlgObjectEnable)

public:
	CDlgObjectEnable(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgObjectEnable();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RENDER_ENABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnClickedCheckArccenteredenable();
	afx_msg void OnClickedCheckArcenable();
	afx_msg void OnClickedCheckArrowenable();
	afx_msg void OnClickedCheckDimensionenable();
	afx_msg void OnClickedCheckEllispeenable();
	afx_msg void OnClickedCheckFilledpolyenable();
	afx_msg void OnClickedCheckLineenable();
	afx_msg void OnClickedCheckOneflatenable();
	afx_msg void OnClickedCheckOriginenable();
	afx_msg void OnClickedCheckPolygonenable();
	afx_msg void OnClickedCheckRectenable();
	afx_msg void OnClickedCheckRectholeenable();
	afx_msg void OnClickedCheckRoundedrectenable();
	afx_msg void OnClickedCheckRoundholeenable();
	afx_msg void OnClickedCheckTextenabled();
	afx_msg void OnClickedCheckTwoflatenable();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
