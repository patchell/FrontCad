// MyToolBarView.cpp : implementation file
//

#include "pch.h"


// CMyToolBar

IMPLEMENT_DYNCREATE(CMyToolBarView, CWnd)

CMyToolBarView::CMyToolBarView()
{

}

CMyToolBarView::~CMyToolBarView()
{
}


BEGIN_MESSAGE_MAP(CMyToolBarView, CWnd)
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyToolBarView diagnostics

#ifdef _DEBUG
void CMyToolBarView::AssertValid() const
{
	CWnd::AssertValid();
}

void CMyToolBarView::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}

#endif //_DEBUG

BOOL CMyToolBarView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (cs.lpszClass == NULL)
	{
		HBRUSH hBr = CreateSolidBrush(GetSysColor(COLOR_MENU));
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_BYTEALIGNWINDOW, NULL, hBr);
	}
	return CWnd::PreCreateWindow(cs);
}

void CMyToolBarView::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	((CSplitterWnd*)GetParent())->SetActivePane(1, 0);
}


// CMyToolBar message handlers

void CMyToolBarView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages
}

BOOL CMyToolBarView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}

void CMyToolBarView::InitToolbar()
{
	//----------------------------------------
	// InitToolbar
	//	Add controls to the tool bar, etc
	//----------------------------------------
	// X,Y display
	//----------------------------------------
	CPoint UL = CPoint(8, 4);
	CSize Sz = CSize(100, 24);
	int Spacing = 16;
	m_Static_X.Create(_T("0.000"), WS_BORDER | WS_CHILD, CRect(UL, Sz), this, TOOLBAR_STATIC_X);
	UL.x += Sz.cx + Spacing;
	m_Static_Y.Create(_T("0.000"), WS_BORDER | WS_CHILD, CRect(UL, Sz), this, TOOLBAR_STATIC_Y);
}

