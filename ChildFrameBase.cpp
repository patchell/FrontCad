// ChildFrameBase.cpp : implementation file
//

#include "pch.h"


// CChildFrameBase

IMPLEMENT_DYNAMIC(CChildFrameBase, CMDIChildWnd)

CChildFrameBase::CChildFrameBase() noexcept
{
	m_pNext = 0;
	m_pPrev = 0;
}

CChildFrameBase::~CChildFrameBase()
{
}


BEGIN_MESSAGE_MAP(CChildFrameBase, CMDIChildWnd)
END_MESSAGE_MAP()



// CChildFrameBase message handlers

BOOL CChildFrameBase::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;
	return TRUE;
}
