#pragma once

/////////////////////////////////////////////////////////////////////////////
// CRulerView view

class CRulerView : public CWnd
{
	// Attributes
	CRulerInfo* m_pRulerInfo;
	UINT   m_rulerType;
	CPoint m_lastPos;
	int m_OriginOffset;
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
	void SetOriginOffset(int o) { m_OriginOffset = o; }
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
	void DrawCursorPos(CDC* pDC, CDoublePoint NewPos);
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
};