#pragma once


// CDlgOriginAttributes dialog

class CDlgOriginAttributes : public CDialog
{
	CCadOrigin* m_pOrg;
	CStaticBitmap m_Static_LineColor;
	CStaticBitmap m_Static_SelectionColor;
	CEditDecimal m_Edit_LineWidth;
	CEditDecimal m_Edit_Radius;
	CEditText m_Edit_Name;
	DECLARE_DYNAMIC(CDlgOriginAttributes)

public:
	CDlgOriginAttributes(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgOriginAttributes();
	void SetOrigin(CCadOrigin* pOrg) { m_pOrg = pOrg; }
	CCadOrigin* GetOrigin() { return m_pOrg; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ORIGIN_ATTRIBUTES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
protected:
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
