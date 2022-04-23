#pragma once


// CMyToolBar

class CMyToolBarView : public CWnd
{
	CWnd* m_pWndMessageDestination;
	CStaticDouble m_Static_X, m_Static_Y;
	CComboBox m_Combo_OriginSelector;
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
	void InitToolbar(CWnd* pWnd);
	void AddOrigine(CCadOrigin* pORG);
	CCadOrigin* GetOrigin(int index);
	void SetToolBarPosition(CDoublePoint pos) {
		m_Static_X.SetDoubleValue(pos.dX);
		m_Static_Y.SetDoubleValue(pos.dY);
	}
	CComboBox& GetOriginSelCB() { return m_Combo_OriginSelector; }
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnComboOriginSel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


