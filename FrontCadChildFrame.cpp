// FrontCadChildFrame.cpp : implementation file
//

#include "pch.h"


// CFrontCadChildFrame

IMPLEMENT_DYNCREATE(CFrontCadChildFrame, CMDIChildWnd)

CFrontCadChildFrame::CFrontCadChildFrame()
{

}

CFrontCadChildFrame::~CFrontCadChildFrame()
{
}


BEGIN_MESSAGE_MAP(CFrontCadChildFrame, CMDIChildWnd)
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()


// CFrontCadChildFrame message handlers


BOOL CFrontCadChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;
	return TRUE;
}

BOOL CFrontCadChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	printf("");
	SetWindowPos(this,0, 0, 1024, 768, 0);
	
	if(!m_ToolBarSplitter.CreateToolBar(this, pContext))
	{
		TRACE("Error creation of ToolBar\n");
		return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	}

	if (!m_RulerSplitter.CreateRulers(
		&m_ToolBarSplitter, 
		pContext, WS_CHILD | WS_VISIBLE | WS_BORDER, 
		m_ToolBarSplitter.IdFromRowCol(1, 0)
		)
	)
	{
		TRACE("Error creation of rulers\n");
		return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	}
	return TRUE;
}


void CFrontCadChildFrame::InitToolBar(CWnd* pWndMessageDest, CRulerInfo* pRulerInfo)
{
	CWnd* pWnd = 0;

	pWnd = m_ToolBarSplitter.GetPane(TOOLBAR_PANE);
	((CMyToolBarView*)pWnd)->InitToolbar(pWndMessageDest, pRulerInfo);
}

void CFrontCadChildFrame::ShowRulers(int nShow)
{
	m_RulerSplitter.ShowRulers(nShow);
}

void CFrontCadChildFrame::ShowToolBar(int nShow)
{
//	m_ToolBarSplitter.ShowToolBar(nShow);
}


void CFrontCadChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	printf("ChildFrame Onsize: Type=%d cx=%d  cy=%d\n", nType, cx, cy);
	// TODO: Add your message handler code here
}


void CFrontCadChildFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	CMDIChildWnd::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
}
