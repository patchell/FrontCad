#pragma once

// CDlgLineAttributes dialog

class CDlgLineAttributes : public CDialog
{
	//=---------------------------------
	// Data
	//----------------------------------
	CCadLine* m_pLine;
	//----------------------------------
	// Controls
	//----------------------------------
	CEditDecimal m_Edit_Width;
	CStaticBitmap m_Static_LineColor;
	CButton m_Check_LockLength;
	CButton m_Check_Snap_P1_ToPoint;
	CButton m_Check_Snap_P2_ToLine;
	CButton m_Check_Snap_P2_ToPoint;
	CEditDecimal m_Edit_LineLength;
	CStaticBitmap m_Static_Line_SelectedColor;
	DECLARE_DYNAMIC(CDlgLineAttributes)
public:
	CDlgLineAttributes(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLineAttributes();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void SetLine(CCadLine* pCL) { m_pLine = pCL; }
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LINE_ATTRB };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
