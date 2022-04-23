#include "pch.h"
//-------------------------------------------
// CRulerSplitterWnd
//------------------------------------------

IMPLEMENT_DYNCREATE(CRulerSplitterWnd, CSplitterWnd)

CRulerSplitterWnd::CRulerSplitterWnd()
{
	m_cxSplitter = 0;
	m_cySplitter = 0;
	m_cxBorderShare = 0;
	m_cyBorderShare = 0;
	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;
	m_bRulersVisible = FALSE;
	m_pRulerInfo = 0;
}

CRulerSplitterWnd::~CRulerSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CRulerSplitterWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_MESSAGE(UINT(WindowsMsg::WM_RULER_MESSAGE),&CRulerSplitterWnd::OnRulerMessage)
END_MESSAGE_MAP()

BOOL CRulerSplitterWnd::CreateRulers(CWnd* pParent, CCreateContext* pContext, DWORD dwStyle, UINT nID)
{
	BOOL rV = FALSE;

	printf("Create Ruler Spliter panes\n");

	if (CreateStatic(pParent, 2, 2, dwStyle, nID))
	{
		//corner
		if (CreateView(RULERCORNER, RUNTIME_CLASS(CRulerCornerView), CSize(0, 0), pContext))
		{
			// Horizontal Ruler
			if (CreateView(HRULER, RUNTIME_CLASS(CRulerView), CSize(0, 0), pContext))
			{
				// Vertical Ruler
				if (CreateView(VRULER, RUNTIME_CLASS(CRulerView), CSize(0, 0), pContext))
				{
					//remaining (main) view
					if (CreateView(MAINPANE, pContext->m_pNewViewClass, CSize(0, 0), pContext))
					{
						SetColumnInfo(0, 0, 0);
						SetRowInfo(0, 0, 0);

						HORIZONTALRULER->SetRulerType(RT_HORIZONTAL);
						VERTICALRULER->SetRulerType(RT_VERTICAL);

						SetActivePane(MAINPANE);
						rV = TRUE;
					}
				}
			}
		}
	}
	return rV;
}

void CRulerSplitterWnd::ShowRulers(BOOL bShow, BOOL bSave)
{
	int nSize = (bShow) ? RULER_SIZE : 0;
	int nSizeBorder = (bShow) ? 3 : 1;
	CRect rect;
	int szScrollBar = GetSystemMetrics(SM_CYHSCROLL);

	GetParent()->GetClientRect(&rect);
	SetRowInfo(0, rect.Height() -  nSize - szScrollBar - TOOLBAR_HIEGHT,0);
	SetRowInfo(1, nSize, 0);
	SetColumnInfo(0, nSize, 0);
	SetColumnInfo(1, rect.Width() - nSize, 0);
	m_cxSplitterGap = nSizeBorder;
	m_cySplitterGap = nSizeBorder;
	m_bRulersVisible = (bSave) ? bShow : m_bRulersVisible;
	RecalcLayout();
}

void CRulerSplitterWnd::SetRulersInfo(CRulerInfo* pRulerInfo)
{
	m_pRulerInfo = pRulerInfo;
	HORIZONTALRULER->SetRulersInfo(m_pRulerInfo);
	VERTICALRULER->SetRulersInfo(m_pRulerInfo);
	CORNER->SetRulersInfo(m_pRulerInfo);
}

void CRulerSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//Lock Splitter
	CWnd::OnLButtonDown(nFlags, point);
}

void CRulerSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	//Lock Splitter
	CWnd::OnMouseMove(nFlags, point);
}


BOOL CRulerSplitterWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	///lock splitter
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CRulerSplitterWnd::OnRulerMessage(WPARAM msg, LPARAM aux)
{
	int rV = 0;

	switch (msg)
	{
	case RW_VSCROLL:
	VERTICALRULER->Draw();
		break;
	case RW_HSCROLL:
		HORIZONTALRULER->Draw();
		break;
	case RW_POSITION:
		HORIZONTALRULER->SetCursor();
		VERTICALRULER->SetCursor();
		break;
	case RW_ZOOM:
		HORIZONTALRULER->Draw();
		VERTICALRULER->Draw();
		break;
	case RW_SETLABELS:
		break;
	case RW_INIT:
		m_pRulerInfo = (CRulerInfo*)aux;

		HORIZONTALRULER->SetRulersInfo((CRulerInfo*)aux);
		VERTICALRULER->SetRulersInfo((CRulerInfo*)aux);
		CORNER->SetRulersInfo((CRulerInfo*)aux);
		break;
	case RW_SHOW:
		ShowRulers(aux, aux);
		break;
	}
	return rV;
}
