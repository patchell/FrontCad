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
	UINT Row(UINT row, UINT col) { return row; }
	UINT Col(UINT row, UINT col) { return col; }
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};

#define PANE_VRULER				1,0
#define PANE_MAINPANE			1,1
#define PANE_RULERCORNER		0,0
#define PANE_HRULER				0,1

#define HORIZONTALRULER		((CRulerView*)GetPane(PANE_HRULER))
#define VERTICALRULER		((CRulerView*)GetPane(PANE_VRULER))
#define CORNER				((CRulerCornerView*)GetPane(PANE_RULERCORNER))
#define MAINVIEW			((CFrontCadView*)GetPane(PANE_MAINPANE))
