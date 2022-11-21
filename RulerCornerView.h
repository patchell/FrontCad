#pragma once
/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView view

class CRulerCornerView : public CView
{
	CRulerInfo *m_pRulerInfo;;
protected:
	CRulerCornerView();    // protected constructor used by dynamic creation
	virtual ~CRulerCornerView();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	DECLARE_DYNCREATE(CRulerCornerView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	void SetRulersInfo(CRulerInfo *pInfo) { 
		m_pRulerInfo = pInfo;
	}
	CRulerInfo* GetRulerInfo() { return m_pRulerInfo; }
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
