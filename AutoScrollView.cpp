//---------------------------------------------
// FrontCad Project
// AutoScrollView.cpp : implementation file
//----------------------------------------------
// This class implements autoscrolling,
// Zooming, and rulers for the JAMCAD EDA
// package.
//----------------------------------------------

#include "pch.h"

// CAutoScrollView

IMPLEMENT_DYNCREATE(CAutoScrollView, CChildViewBase)

//-------------------------------------------------
// Construction
//--------------------------------------------------
CAutoScrollView::CAutoScrollView()
{
	m_VScrollPos = 0;
	m_HScrollPos = 0;
	m_HPageSize = 0;
	m_VPageSize = 0;
	m_AutoScroll = 0;
	m_IsScrolling = 0;
	m_CurMousePos = CPoint(0, 0);
	m_Block = 0;
}

CAutoScrollView::~CAutoScrollView()
{
}

//-------------------------------------------------------
// Message Dispatcher
//--------------------------------------------------------

BEGIN_MESSAGE_MAP(CAutoScrollView, CChildViewBase)
	//Place object Methods
END_MESSAGE_MAP()


// CAutoScrollView drawing

void CAutoScrollView::OnDraw(CDC* pDC)
{
	// Empty
}


// CAutoScrollView diagnostics

#ifdef _DEBUG
void CAutoScrollView::AssertValid() const
{
	CChildViewBase::AssertValid();
}

#ifndef _WIN32_WCE
void CAutoScrollView::Dump(CDumpContext& dc) const
{
	CChildViewBase::Dump(dc);
}
#endif
#endif //_DEBUG


// CAutoScrollView message handlers



void CAutoScrollView::OnInitialUpdate()
{
    //--------------------------------------------------------------
    //  OnInitialUpdate
    //
    //      This Method does the initial setup of the view
    //-------------------------------------------------------------
	CChildViewBase::OnInitialUpdate();

}


void CAutoScrollView::OnUpdate(CChildViewBase* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
//	UpdateRulerInfo(RW_POSITION, CPoint(m_HScrollPos, m_VScrollPos));
	Invalidate();
}

void CAutoScrollView::OnAutoScroll(CPoint /*point*/, BOOL /*bBeforeScroll*/)
{
    //--------------------------------------------------------------------
    // OnAutoScroll
    //
    //      This Method gets called when the view does an autoscroll.
    // This is a virtual Method that should be over ridden.
    //--------------------------------------------------------------------
}


void CAutoScrollView::AutoScroll(WindowsMsg nRefMessage)
{
    //----------------------------------------------------------------------
    //  AutoScroll
	//		Not my methode.  Found it on the internet but
	// as of right now, I have made minor changes, but I don't
	// remember where I got it from.
	//
	// parameters:
	//		nRefMessage.....message value to stop autoscroll mode on
    //----------------------------------------------------------------------
	static int Count2, Count3;
	MSG   msg;				// dummmy message structure to process incoming
	memset(&msg, 0, sizeof(MSG));
							// messages while autoscrolling.
	CPoint	ptDevScrollPos;	// Current scroll position - device units
	CPoint	ptCursorPos;		// Current mouse cursor position
	CRect  rc; 				// Client area
	int   dx, dy;			// Scrolling increment
//	CSize   sizeTotal;      // CView scroll data
	CSize	sizePage;
	CSize	sizeLine;
//	int    nMapMode;       // Mapping mode (WTF is this?)
	int	xMin;	// Scroll range
	int xMax;	// Scroll range
	int	yMin;	// Scroll range
	int yMax;   // Scroll range
	//---------------------------------------
	// If we are not in autoscroll mode,
	// return
	//---------------------------------------
	if (!m_AutoScroll) return;
	//--------------------------------------
	// check our semaphore flag to see if this
	// Method has already been entered
	//----------------------------------------
	if (m_Block) return;
	++m_Block;	//increment semaphore count

	msg.message = 0;   // forces at least one loop.
	SetCapture();
	GetDeviceScrollSizes(/*nMapMode, sizeTotal,*/ sizePage, sizeLine);
	//---------------------------------------------------------------------------
	// We keep track of the scroll range because CView::ScrollToPosition always
	// try to scroll even if the scroll limit has been reached. This results in screen
	// flickering when ScrollWindow is called.
	//---------------------------------------------------------------------------
	GetScrollRange(SB_HORZ, &xMin, &xMax);
	GetScrollRange(SB_VERT, &yMin, &yMax);
	//---------------------------------------------------------------------------
	// Process all messages until the relevant mouse button
	// has been released. nRefMessage depends on which button
	// has been used to trigger autoscrolling.
	//   while (msg.message != nRefMessage)
	//---------------------------------------------------------------------------
	bool bSawRefMessage = false;

	while (!bSawRefMessage && m_AutoScroll)
	{
		//---------------------------------------------------------------------------
		// Process incoming messages until autoscroll button is released
		//---------------------------------------------------------------------------
		ptDevScrollPos = CPoint(m_HScrollPos,m_VScrollPos);
		GetCursorPos(&ptCursorPos);
		ScreenToClient(&ptCursorPos);
		GetClientRect(&rc);
		dx = 0;
		dy = 0;

		if ((ptCursorPos.y < 0) && (ptDevScrollPos.y != yMin))
			// Cursor is above client area
			dy = min(-sizeLine.cy, max(-sizePage.cy, (ptCursorPos.y / 10) * sizeLine.cy));
		else if ((ptCursorPos.y > rc.bottom) && (ptDevScrollPos.y != yMax))
			// Cursor is below client area
			dy = max(sizeLine.cy, min(sizePage.cy, ((ptCursorPos.y - rc.bottom) / 10) * sizeLine.cy));
		// otherwise we can't scroll anyway

		if ((ptCursorPos.x < 0) && (ptDevScrollPos.x != xMin))
			// Cursor is on the left of the client area
			dx = min(-sizeLine.cx, max(-sizePage.cx, (ptCursorPos.x / 10) * sizeLine.cx));
		else if ((ptCursorPos.x > rc.right) && (ptDevScrollPos.x != xMax))
			// Cursor is on the right of the client area
			dx = max(sizeLine.cx, min(sizePage.cx, ((ptCursorPos.x - rc.right) / 10) * sizeLine.cx));
		// otherwise we can't scroll anyway

		// if mouse cursor is dragging outside the client area, scrolling occurs
		if ((dx != 0) || (dy != 0))
		{
			//-------------------------------------------------------
			int MaxH,MaxV,PosH,PosV;
			MaxV = m_VScrollInfo.nMax - m_VScrollInfo.nPage;
			MaxH = m_HScrollInfo.nMax - m_HScrollInfo.nPage;
			PosH = m_HScrollPos + dx;
			PosV = m_VScrollPos + dy;
			if (PosH < 0)
				m_HScrollPos = 0;
			else if (PosH > MaxH)
				m_HScrollPos = MaxH;
			else
				m_HScrollPos = PosH;
			if (PosV < 0)
				m_VScrollPos = 0;
			else if (PosV > MaxV)
				m_VScrollPos = MaxV;
			else
				m_VScrollPos = PosV;
//-----------------------------------------------------
			m_IsScrolling = TRUE;
			OnAutoScroll(ptCursorPos, TRUE);
			SetScrollPos(SB_VERT, m_VScrollPos);
			SetScrollPos(SB_HORZ, m_HScrollPos);

			GetRulerInfo().SetScrollOffset(GetScrollOffset());
			PostMessageToRulers(RW_VSCROLL);
			PostMessageToRulers(RW_HSCROLL);
			PostMessageToRulers(RW_POSITION);
			printf("--=== Message Pump #2   %8d\n",++Count2);

			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))	//message pump 2
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				//            if (msg.message == nRefMessage) break;
				if (msg.message == (UINT)nRefMessage)
					bSawRefMessage = TRUE;
			}	//end of message pump 2
			//----------------------------------------
			// call any custom processing
			//------------------------------------------
			OnAutoScroll(ptCursorPos, FALSE);
		}
		else
		{
			printf("--=== Message Pump #3   %8d\n",++Count3);
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))	//message pump #3
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				//if (msg.message == nRefMessage) break;
				if (msg.message == UINT(nRefMessage))
					bSawRefMessage = TRUE;
			}	//end of message pump #3

			m_IsScrolling = FALSE;
		}	//end of if-else
	}	//end of while loop while (!bSawRefMessage && m_AutoScroll)
	ReleaseCapture();
	m_IsScrolling = FALSE;
	//-----------------------------------------------
	// decrement "semaphore" count
	//------------------------------------------------
	--m_Block;
}

void CAutoScrollView::OnButtonZoomin()
{
	//----------------------------------------------
	// OnButtonZoomin
	//		Message pro cessed with the + magnifier
	// glass icon is pushed on the tool bar.
	//----------------------------------------------
	CBaseDocument* pDoc = (CBaseDocument * )GetDocument();
	GetGrid().ZoomIn();
	m_HScrollPos = GETAPP.RoundDoubleToInt(m_HScrollPos * GetGrid().GetScaleFactor());
	m_VScrollPos = GETAPP.RoundDoubleToInt(m_VScrollPos * GetGrid().GetScaleFactor());
	UpdateScrollbarInfo();
	GetRulerInfo().SetScrollOffset(GetScrollOffset());
	PostMessageToRulers(RW_ZOOM);
	PostMessageToRulers(RW_POSITION);
	if (pDoc)
		pDoc->SetDirty(1);
	Invalidate();
}


void CAutoScrollView::OnUpdateButtonZoomin(CCmdUI *pCmdUI)
{
	//-------------------------------------
	// OnUpdateButtonZoomin
	// enables the button for doing a Zoom In
	// on the tool bar
	//--------------------------------------
	pCmdUI->Enable();
}


void CAutoScrollView::OnButtonZoomout()
{
	//------------------------------------------------------
	// OnButtonZoomout
	//		This method is called when the ZoomOut 
	// button on the toolbar is pushed.
	//-------------------------------------------------------
	CBaseDocument* pDoc = (CBaseDocument*)GetDocument();
	m_HScrollPos = GETAPP.RoundDoubleToInt(double(m_HScrollPos) / GetGrid().GetScaleFactor());
	m_VScrollPos = GETAPP.RoundDoubleToInt(double(m_VScrollPos) / GetGrid().GetScaleFactor());
	GetGrid().ZoomOut();
	UpdateScrollbarInfo();
	GetRulerInfo().SetScrollOffset(GetScrollOffset());
	PostMessageToRulers(RW_ZOOM);
	PostMessageToRulers(RW_POSITION);
	if (pDoc)
		pDoc->SetDirty(1);
	Invalidate();
}


void CAutoScrollView::OnUpdateButtonZoomout(CCmdUI *pCmdUI)
{
	//------------------------------------------------
	//	OnUpdateButtonZoomout
	//		Enables the ZoomOut button on the toolbar
	//------------------------------------------------
	pCmdUI->Enable();
}

CPoint CAutoScrollView::CalculateHsVs(CPoint p, CPoint DesiredScreenLocation)
{
	CPoint HsVs;

	HsVs = p - DesiredScreenLocation;
	return HsVs;
}


void CAutoScrollView::SetCurrentMousePosition(CDoublePoint DP)
{
	m_CurMousePos = DP;
}

void CAutoScrollView::OnSize(UINT nType, int cx, int cy)
{
	//----------------------------------------------
	// OnSize
	//		This method is called in response to
	// a WM_SIZE message
	//----------------------------------------------
	CChildViewBase::OnSize(nType, cx, cy);
//	CreateMemDC();
	UpdateScrollbarInfo();
	GetRulerInfo().SetClientSize(CSize(cx, cy));
	GetRulerInfo().SetScrollOffset(GetScrollOffset());
	if(GetRulerInfo().AreFulersReady())
		PostMessageToRulers(RW_ZOOM);
	Invalidate();
}

afx_msg LRESULT CAutoScrollView::OnStopautoscroll(WPARAM wParam, LPARAM lParam)
{
	//------------------------------------------------
	// OnStopautoscroll
	//
	//	This method is called in reponse to a
	// WM_STOPAUTOSCROLL message
	//------------------------------------------------
	return 0;
}

void CAutoScrollView::OnDestroy()
{
	//------------------------------------------
	// OnDestroy
	//		Get called on WM_DESTROY.  AFAIK,
	// this never ever gets called
	//------------------------------------------
	EnableAutoScroll(0);
	PostMessage((UINT)WindowsMsg::WM_STOPAUTOSCROLL);
//	while (Block)
//		Sleep(1);
	CChildViewBase::OnDestroy();
}

