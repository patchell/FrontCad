#pragma once

//-------------------------
// CAutoScrollView view
//------------------------

class CAutoScrollView : public CChildViewBase
{
	//----------------SnapGrid et all ------------------------
	CGrid m_Grid;
	//----------------- Rulers --------------------------------------------
	CRulerInfo m_RulerInfo;
	//----------------Scrolling and Document View ---------------------------
	SCROLLINFO m_HScrollInfo;	//Horizontal Scroll Info
	SCROLLINFO m_VScrollInfo;	//Vertical Scroll Info
	int m_VPageSize;	//vertical page size
	int m_HPageSize;	//horizontal page size
	CDoubleSize m_DocSize;	//document size
	int m_HScrollPos;	//position of horizontal scroll bar, in screen units
	int m_VScrollPos;	//position of virtical scroll bar, in screen units
	int m_AutoScroll;	//enables auto scrolling flag
	int m_IsScrolling;	//Indicates that the view is auto scrolling
	int m_Block;		//blocks reentrance to AutoScroll method
	//----------------Keyboard/Mouse------------------------
	BOOL m_MouseDown;	//left mouse button is down
	CDoublePoint m_CurMousePos;	//mouse position raw, screen units
//--------------------------------------------------------
	CDoublePoint m_Origin;	//More complicated than it seems

	DECLARE_DYNCREATE(CAutoScrollView)
protected:
	CAutoScrollView();           // protected constructor used by dynamic creation
	virtual ~CAutoScrollView();
public:
	virtual CDocument* GetDocument() {
		return CChildViewBase::GetDocument();
	}
	CDoubleSize GetDocSize(void) { return m_DocSize; }
	void SetDocSize(CDoubleSize sZ) { m_DocSize = sZ; }
//	int IsScrolling() { return m_IsScrolling; }
	void EnableAutoScroll(int flag) { m_AutoScroll = flag; }
	void SetOrigin(CDoublePoint o) { m_Origin = o; }
	CDoublePoint GetOrigin(void) { return m_Origin; }
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//-----------------------------------------------
	// Context Menu Methods
	//-----------------------------------------------
	virtual void MoreContextMenuItems(CPoint MousePos, CMenu* pMenu, CBaseDocument* pDoc) {}
	virtual void MoreContextMenuActions(CPoint MousePos, int ID, CBaseDocument* pDoc) {}
	//-----------------------------------------------
	// Message Handlers
	//-----------------------------------------------
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CChildViewBase* pSender, LPARAM lHint, CObject* pHint);
	void UpdateScrollbarInfo();
	virtual void OnAutoScroll(CPoint point, BOOL bBeforeScroll);
	void AutoScroll(WindowsMsg nMessage);
	void GetDeviceScrollSizes(CSize& sizePage, CSize& sizeLine);
	BOOL SendMessageToRulers(UINT msg, LPARAM data);
	afx_msg void OnButtonZoomin();
	afx_msg void OnUpdateButtonZoomin(CCmdUI* pCmdUI);
	afx_msg void OnButtonZoomout();
	afx_msg void OnUpdateButtonZoomout(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	void DrawCursor(CDC* pDC, CPoint p, CRect* pRect, COLORREF color);
	// Zoom in around a point
	void ZoomIn(CPoint point);
	// zoom out around a point
	void ZoomOut(CPoint point);
	//--------------------------------
	// Rulers/Grid
	//--------------------------------
	CGrid* GetPtrToGrid() { return &m_Grid; }
	CGrid& GetGrid() { return m_Grid; }
	CRulerInfo& GetRulerInfo() { return m_RulerInfo; }
	CRulerInfo* GetPtrToRulerInfo() { return &m_RulerInfo; }
	BOOL PostMessageToRulers(UINT msg, LPARAM data = 0);
	//--------------------------------
	// Manage view attributes
	// ------------------------------
	CFrontCadChildFrame* GetParentFrame()
	{
		CFrontCadChildFrame* pFrame;
		pFrame = (CFrontCadChildFrame*)GetParent();
		return pFrame;
	}
	CRulerSplitterWnd* GetRulerSplitterWnd() {
		CRulerSplitterWnd* pRSW;

		pRSW = (CRulerSplitterWnd*)GetParent();
		return pRSW;
	}
	CDoublePoint GetCurrentMousePosition() { return m_CurMousePos; }
	void SetCurrentMousePosition(CDoublePoint DP); 
	//-----------------------------------------------
	// //converts raw position and other mouse stuff
	//-----------------------------------------------
	CDoublePoint ConvertMousePosition(
		CPoint point, 
		CSize Offset = CSize(0,0),
		CScale Scale = CScale(1.0,1.0),
		CDoubleSize SnapGrid = CDoubleSize(0.0, 0.0),
		BOOL SnapGridOn = TRUE
	);
	void SetHScrollPos(int HSP) { m_HScrollPos = HSP; }
	int GetHScrollPos() { return m_HScrollPos; }
	void SetVScrollPos(int VSP) {m_HScrollPos = VSP; }
	int GetVScrollPos() { return m_VScrollPos; }
	CSize GetScrollOffset() { return CSize(m_HScrollPos, m_VScrollPos); }
	CDoublePoint ProcessMousePosition(CPoint point, CDoubleSize SnapGrid, int Which);
	CDoublePoint GetViewUpperLeftHandCorner() {
		return CDoublePoint(GetHScrollPos(), GetVScrollPos());
	}
	//-------------------------------------------------
	// Client Area Scrolling Methods
	//------------------------------------------------
	CPoint CalculateHsVs(CPoint p, CPoint DesiredScreenLocation);
	void DoHScroll(int Amount);
	void DoVScroll(int Amount);
	//---------------------------------
	// managen objects in document
	//---------------------------------
	virtual void RemoveObject(CCadObject* pObj) {}
	virtual void AddObjectAtEndIntoDoc(CCadObject* pObj);
	virtual void AddObjectAtFrontIntoDoc(CCadObject* pObj);
	//---------------------------------
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	afx_msg LRESULT OnStopautoscroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#define PROC_MOUSE_POSISTION_CORRECTED	0
#define PROC_MOUSE_POSISTION_SCALED		1
#define PROC_MOUSE_POSISTION_SNAPPED		2
