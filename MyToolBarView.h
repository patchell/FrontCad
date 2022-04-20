#pragma once

constexpr auto TOOLBAR_HIEGHT = 32;
constexpr auto TOOLBAR_STATIC_X = 800;
constexpr auto TOOLBAR_STATIC_Y = 801;

// CMyToolBar

class CMyToolBarView : public CWnd
{
	CStatic m_Static_X, m_Static_Y;
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
	void InitToolbar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


