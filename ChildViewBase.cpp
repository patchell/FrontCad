// ChildViewBase.cpp : implementation file
//

#include "pch.h"


// CChildViewBase

IMPLEMENT_DYNAMIC(CChildViewBase, CView)

CChildViewBase::CChildViewBase()
{
	m_pMessageList = 0;
	m_NumOfMessListEntries = 0;
	m_thisID = GETAPP.GetUniqueID();
}

CChildViewBase::~CChildViewBase()
{
}


BEGIN_MESSAGE_MAP(CChildViewBase, CView)
END_MESSAGE_MAP()



// CChildViewBase message handlers

BOOL CChildViewBase::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	return TRUE;
}

