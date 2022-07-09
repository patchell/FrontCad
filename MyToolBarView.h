#pragma once


// CMyToolBar

class CMyToolBarView : public CWnd
{
	CWnd* m_pWndMessageDestination;
	CStaticDouble m_Static_X, m_Static_Y;
	CStatic m_StaticClient;
	CComboBox m_Combo_OriginSelector;
	CRulerInfo* m_pRulerInfo;
	int m_ComboOriginIndex;
protected:
	CMyToolBarView();
	virtual ~CMyToolBarView();
	DECLARE_DYNCREATE(CMyToolBarView)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void InitToolbar(CWnd* pWnd, CRulerInfo *pRulerInfo);
	void AddOrigin(CCadOrigin* pORG);
	CCadOrigin* GetOrigin(int index);
	void SetToolBarPosition(DOUBLEPOINT pos) {
		m_Static_X.SetDoubleValue(pos.dX);
		m_Static_Y.SetDoubleValue(pos.dY);
	}
	void SetDebug(CString& csS) { m_StaticClient.SetWindowTextW(csS); }
	CComboBox& GetOriginSelCB() { return m_Combo_OriginSelector; }
	CRulerInfo* GetRulerInfo() {
		return m_pRulerInfo;
	}
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnComboOriginSel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


