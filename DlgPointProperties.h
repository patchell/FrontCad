#pragma once
#include "afxdialogex.h"


// CDlgPointProperties dialog

class CDlgPointProperties : public CDialog
{
	CCadPoint* m_pPoint;
	BOOL m_bDirty;
	DECLARE_DYNAMIC(CDlgPointProperties)

public:
	CDlgPointProperties(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgPointProperties();
	void SetPoint(CCadPoint* pP) { m_pPoint = pP; }
	BOOL IsDirty() { return m_bDirty; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POINT_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
private:
	CStaticBitmap m_Static_NormalColor;
	CStaticBitmap m_Static_SelectedColor;
protected:
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
