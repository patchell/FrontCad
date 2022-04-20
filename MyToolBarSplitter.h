#pragma once


// CMyToolBarSplitter

class CMyToolBarSplitter : public CSplitterWnd
{
	BOOL m_bToolBarVisible;
	DECLARE_DYNAMIC(CMyToolBarSplitter)
public:
	CMyToolBarSplitter();
	virtual ~CMyToolBarSplitter();
	BOOL CreateToolBar(
		CWnd* pParent,
		CCreateContext* pContext,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE,
		UINT nID = AFX_IDW_PANE_FIRST
	);
	void ShowToolBar(BOOL bShow = TRUE, BOOL bSave = TRUE);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

protected:
	DECLARE_MESSAGE_MAP()
};

#define TOOLBAR_PANE			0,0
#define TOOLBAR_MAINPANE	1,0

#define TOOLBAR				((CMyToolBarView*)GetPane(0, 0))

