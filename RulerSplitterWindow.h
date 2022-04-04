#pragma once

/////////////////////////////////////////////////////////////////////////////
// CRulerSplitterWnd window

class CRulerSplitterWnd : public CSplitterWnd
{
	BOOL m_bRulersVisible;
	CRulerInfo* m_pRulerInfo;
	DECLARE_DYNCREATE(CRulerSplitterWnd)
public:
	CRulerSplitterWnd();
	virtual ~CRulerSplitterWnd();
	BOOL CreateRulers(
		CWnd* pParent, 
		CCreateContext* pContext, 
		DWORD dwStyle = WS_CHILD | WS_VISIBLE, 
		UINT nID = AFX_IDW_PANE_FIRST
	);
	afx_msg LRESULT OnRulerMessage(WPARAM msg, LPARAM auz);
	void ShowRulers(BOOL bShow = TRUE, BOOL bSave = TRUE);
	void SetRulersInfo(CRulerInfo*pRulerInfo);
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};

#define HRULER				0,1
#define VRULER				1,0
#define RULERCORNER			0,0
#define MAINPANE			1,1
#define HORIZONTALRULER		((CRulerView*)GetPane(0, 1))
#define VERTICALRULER		((CRulerView*)GetPane(1, 0))
#define CORNER				((CRulerCornerView*)GetPane(0, 0))
#define MAINVIEW			((CRulerView*)GetPane(1, 1))
