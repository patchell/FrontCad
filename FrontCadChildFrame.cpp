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
	if (!m_RulerSplitter.CreateRulers(this, pContext)) 
	{
		TRACE("Error creation of rulers\n");
		return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	}
	return TRUE;
}


void CFrontCadChildFrame::ShowRulers(int nShow)
{
	m_RulerSplitter.ShowRulers(nShow);
}

