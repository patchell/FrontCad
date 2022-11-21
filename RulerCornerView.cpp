#include "pch.h"

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView

IMPLEMENT_DYNCREATE(CRulerCornerView, CView)

CRulerCornerView::CRulerCornerView()
{
	m_pRulerInfo = 0;
}

CRulerCornerView::~CRulerCornerView()
{
}


BEGIN_MESSAGE_MAP(CRulerCornerView, CView)
	//{{AFX_MSG_MAP(CRulerCornerView)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView drawing

void CRulerCornerView::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush br;
	br.CreateSolidBrush(GetRulerInfo()->GetCornerColor());
	pDC->FillRect(&rect, &br);
}

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView diagnostics

#ifdef _DEBUG
void CRulerCornerView::AssertValid() const
{
	CView::AssertValid();
}

void CRulerCornerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRulerCornerView message handlers

BOOL CRulerCornerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// create a brush using the same color as the background
	if (cs.lpszClass == NULL)
	{
		HBRUSH hBr = CreateSolidBrush(GetSysColor(COLOR_MENU));
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_BYTEALIGNWINDOW, NULL, hBr);
	}

	return CView::PreCreateWindow(cs);
}

void CRulerCornerView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	((CSplitterWnd*)GetParent())->SetActivePane(1, 1);
}


void CRulerCornerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	printf("Ruler Corner: Type=%d cx=%d cy=%d\n", nType, cx, cy);
	// TODO: Add your message handler code here
}
