// MyToolBarSplitter.cpp : implementation file
//

#include "pch.h"


// CMyToolBarSplitter

IMPLEMENT_DYNAMIC(CMyToolBarSplitter, CSplitterWnd)

CMyToolBarSplitter::CMyToolBarSplitter()
{
	m_cxSplitter = 0;
	m_cySplitter = 0;
	m_cxBorderShare = 0;
	m_cyBorderShare = 0;
	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;
	m_bToolBarVisible = TRUE;
}

CMyToolBarSplitter::~CMyToolBarSplitter()
{
}


BEGIN_MESSAGE_MAP(CMyToolBarSplitter, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

BOOL CMyToolBarSplitter::CreateToolBar(CWnd* pParent, CCreateContext* pContext, DWORD dwStyle, UINT nID)
{
	BOOL rV = FALSE;
	CRect rect;
	pParent->GetClientRect(&rect);

	if (CreateStatic(pParent, 2, 1))
	{
		//Toolbar
		if (CreateView(TOOLBAR_PANE, RUNTIME_CLASS(CMyToolBarView), CSize(0, 0), pContext))
		{
			SetColumnInfo(0, rect.Width(), 0);
			SetRowInfo(0, TOOLBAR_HIEGHT, 0);
			SetRowInfo(1, rect.Height() - 32, 0);
			rV = TRUE;
		}
	}
	return rV;
}

void CMyToolBarSplitter::ShowToolBar(BOOL bShow, BOOL bSave)
{
	int nSize = (bShow) ? RULER_SIZE : 0;
	int nSizeBorder = (bShow) ? 3 : 1;

	SetRowInfo(0, nSize, 0);
	SetColumnInfo(0, nSize, 0);
	m_cxSplitterGap = nSizeBorder;
	m_cySplitterGap = nSizeBorder;
	m_bToolBarVisible = (bSave) ? bShow : m_bToolBarVisible;
	RecalcLayout();
}




// CMyToolBarSplitter message handlers




void CMyToolBarSplitter::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CSplitterWnd::OnLButtonDown(nFlags, point);
}


void CMyToolBarSplitter::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CSplitterWnd::OnMouseMove(nFlags, point);
}


BOOL CMyToolBarSplitter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
}
