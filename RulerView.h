#pragma once

/////////////////////////////////////////////////////////////////////////////
// CRulerView view

class CRulerView : public CWnd
{
	// Attributes
	CRulerInfo* m_pRulerInfo;
	UINT   m_rulerType;
	CPoint m_lastPos;
	CFont m_Font;
	CBitmap m_RulerBackground;
protected:
	CRulerView();           // protected constructor used by dynamic creation
	virtual ~CRulerView();
	DECLARE_DYNCREATE(CRulerView)
public:
	void Scroll();
	void SetCursor();
	//-------------------------------------------------
	void SetRulersInfo(CRulerInfo* pRulerInfo) { m_pRulerInfo = pRulerInfo; }
	CRulerInfo* GetRulersInfo() { return m_pRulerInfo; }
	BOOL IsBackGroundValid(CSize czClient);
	void SetRulerType(UINT rulerType = RT_HORIZONTAL);
	int GetRulerType(void) { return m_rulerType; }
	CFont& GetFont() { return m_Font; }
	//----------------------------------------
	//	Draw Methods
	//----------------------------------------
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void Draw();
	void DrawTicker(
		CDC* pDC,
		CRect& rulerRect
	);
	void DrawCursorPos(CDC* pDC, DOUBLEPOINT NewPos);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
