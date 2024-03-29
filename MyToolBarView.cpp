// MyToolBarView.cpp : implementation file
//

#include "pch.h"


/// CMyToolBar

IMPLEMENT_DYNCREATE(CMyToolBarView, CWnd)

CMyToolBarView::CMyToolBarView()
{
	m_ComboOriginIndex = 0;
	m_pWndMessageDestination = 0;
}

CMyToolBarView::~CMyToolBarView()
{
}


BEGIN_MESSAGE_MAP(CMyToolBarView, CWnd)
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
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
	printf("Set Focus\n");
	CWnd::OnSetFocus(pOldWnd);
	((CSplitterWnd*)GetParent())->SetActivePane(1, 0);
}


// CMyToolBar message handlers
// Hey, why not

void CMyToolBarView::OnPaint()
{
	CPaintDC dc(this); 
}

BOOL CMyToolBarView::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CMyToolBarView::InitToolbar(CWnd* pWnd, CRulerInfo* pRulerInfo)
{
	m_pRulerInfo = pRulerInfo;
	//----------------------------------------
	// InitToolbar
	//	Add controls to the tool bar, etc
	//----------------------------------------
	m_pWndMessageDestination = pWnd;
	//----------------------------------------
	// X,Y display
	//----------------------------------------
	CPoint UL = CPoint(8, 5);
	CSize Sz = CSize(100, 24);
	int Spacing = 16;
	m_Static_X.Create(_T("0.000"), WS_BORDER | WS_CHILD, CRect(UL, Sz), this, IDC_STATIC_TOOLBAR_X);
	m_Static_X.EnableWindow(1);
	m_Static_X.ShowWindow(1);
	UL.x += Sz.cx + Spacing;
	m_Static_Y.Create(_T("0.000"), WS_BORDER | WS_CHILD, CRect(UL, Sz), this, IDC_STATIC_TOOLBAR_Y);
	m_Static_Y.EnableWindow(1);
	m_Static_Y.ShowWindow(1);
	UL.x += Sz.cx + Spacing;
	UL.x += Sz.cx + Spacing;
	Sz.cx = 300;
	m_StaticClient.Create(_T("0.000"), WS_BORDER | WS_CHILD, CRect(UL, Sz), this, IDC_STATIC_TOOLBAR_Y);
	m_StaticClient.EnableWindow(1);
	m_StaticClient.ShowWindow(1);
}


void CMyToolBarView::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
}
