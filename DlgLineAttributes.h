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
	DECLARE_DYNAMIC(CDlgLineAttributes)
public:
	CDlgLineAttributes(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLineAttributes();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticLineColor();
	void SetLine(CCadLine* pCL) { m_pLine = pCL; }
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LINE_ATTRB };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};

extern int DoLineAttrbDlg(CCadLine* pCL);
