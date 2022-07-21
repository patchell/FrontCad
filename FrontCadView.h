#pragma once

// CFrontCadView.cpp view class

constexpr auto PROC_MOUSE_POSISTION_CORRECTED = 0;
constexpr auto PROC_MOUSE_POSISTION_SCALED = 1;
constexpr auto PROC_MOUSE_POSISTION_SNAPPED = 2;

constexpr auto TIMER_ID_AUTOSCROLL = 5600;
constexpr auto TIMER_AUTOSCROLL_TIME = 100;
constexpr auto TIMER_ID_LBUTTON_DOWN = 5601;
constexpr auto TIMER_LBUTTON_DOWN_TIME = 200;

constexpr auto DEFALT_SELECT_BUFFER_SIZE = 16;

enum class MouseIsHere {
	NOWHERE,
	LEFT,
	RIGHT,
	UPPER,
	LOWER,
	UPPERLEFT,
	UPPERRIGHT,
	LOWERLEFT,
	LOWERRIGHT
};
class CFrontCadView : public CChildViewBase
{
	CCadOrigin m_DefaultOrigin;
	//----------------SnapGrid et all ------------------------
	CGrid m_Grid;
	//----------------- Rulers --------------------------------------------
	CRulerInfo m_RulerInfo;
	//----------------Scrolling and Document View ---------------------------
	SCROLLINFO m_HScrollInfo;	//Horizontal Scroll Info
	SCROLLINFO m_VScrollInfo;	//Vertical Scroll Info
	BOOL m_AutoScroll;	//enables auto scrolling flag
	//--------------- Timer IDs -----------------------------
	UINT m_AutoScrollTimerId;
	UINT m_MouseLButtonHeldDownId;
	//----------------Keyboard/Mouse------------------------
	BOOL m_ControlKeyDown;	//true when control key is pushed
	BOOL m_ShiftKeyDown;	// true when shift key is pushed
	BOOL m_AltKeyDown;		// Alt key is down
	BOOL m_R_KeyDown;		//The R key is pressed
	DOUBLEPOINT m_CurMousePos;	//mouse position raw, screen units
	DOUBLEPOINT m_LastMousePos;
	CDoubleSize m_DeltaMousePos;
	TRACKMOUSEEVENT m_TrackMouseEvent;
	BOOL m_MouseLeftWindow;
	MouseIsHere m_MouseLocation;
	BOOL m_LeftMouseButtonDown;	//left mouse button is down
	BOOL m_MouseLButtonHeldDown;
	BOOL m_MiddleMouseButtonDown;	//Ture if button down
	//--------------------------------------------------------
	unsigned m_ObjectEnables;
	//----------------------------------
	// Drawing Variables
	//----------------------------------
	ObjectDrawState m_DrawState;	// current state for the current drawmode
	DrawingMode m_DrawMode;		// what is going on in the drawing
	//---------------------------------
	// Misc Variables
	//---------------------------------
	CADObjectTypes m_GrabbedObject;	//vertex that is being dragged
	CADObjectTypes m_CadObj;
	CFrontCadChildFrame* m_pParentFrame;

	DECLARE_DYNCREATE(CFrontCadView)
protected:
	CFrontCadView();           // protected constructor used by dynamic creation
	virtual ~CFrontCadView();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	CFrontCadChildFrame* GetMyFrame() { return  m_pParentFrame; }
	void SetObjectEnables(unsigned u) { m_ObjectEnables |= u; }
	void ClearObjectEnables(unsigned u) { m_ObjectEnables &= ~u; }
	virtual void OnInitialUpdate();
	CADObjectTypes& GetObjectTypes() { return m_CadObj; }
	void SetObjectTypes(CCadObject* pObj) { GetObjectTypes().pCadObject = pObj; }
	//---------------------------------------------
	// Keyboard/Mouse Methods
	//---------------------------------------------
	BOOL IsControlKeyDown() { return m_ControlKeyDown; }
	BOOL IsShiftKeyDown() { return m_ShiftKeyDown; }
	BOOL IsAltKeyDown() { return m_AltKeyDown; }
	BOOL IsR_KeyDown() { return m_R_KeyDown; }
	//-------
	BOOL IsLeftMouseButtonDown() { return m_LeftMouseButtonDown; }
	BOOL IsMiddleMouseButtonDown() { return m_MiddleMouseButtonDown; }
	void SetMiddleMouseButtonDown(BOOL f) { m_MiddleMouseButtonDown = f; }
	DOUBLEPOINT GetCurrentMousePosition() { return DOUBLEPOINT(m_CurMousePos); }
	void SetCurrentMousePosition(DOUBLEPOINT DP) { m_CurMousePos = DP; }
	CDoubleSize GetDeltaMousePos() { return m_DeltaMousePos; }
	void SetDeltaMousePos(CDoubleSize szDelta) { m_DeltaMousePos = szDelta; }
	DOUBLEPOINT ConvertMousePosition(
		CPoint MousePoint,	//mouse position client ref
		DOUBLEPOINT ULHC,	//upper left corner of client in inches
		CScale Scale,		//Inches per Pixel
		CDoubleSize SnapGrid,
		BOOL SnapGridIsEnabled
	);
	BOOL DidMouseLeaveWindow() { return m_MouseLeftWindow; }
	void SetMouseLeftWindow(BOOL flag) { m_MouseLeftWindow = flag; }
	afx_msg void OnMouseLeave();
	BOOL GetCursorPosition(CPoint* pMP) {
		//----------------------------
		// Get Cursor position in
		// screen coordinates
		//----------------------------
		BOOL rV;
		rV = ::GetCursorPos(pMP);
		return rV;
	}
	BOOL SetCursorPosition(DOUBLEPOINT& p)
	{
		//----------------------------------
		// Set the cursor poition, in
		// client coordiates space to the
		// position in screen coordinates
		//----------------------------------
		CPoint cp;
		CScale Scale;

		Scale = GetGrid().GetPixelsPerInch();
		cp = p.ToPixelPoint(
			GetRulerInfo().GetUpperLeft(), 
			Scale.GetScaleX(),
			Scale.GetScaleY()
		);
		ClientToScreen(&cp);
		return ::SetCursorPos(cp.x,cp.y);
	}
	BOOL SetCursorPosition(CCadPoint& p)
	{
		CPoint cp;

		cp = p.ToPixelPoint(GetRulerInfo().GetUpperLeft(), GetGrid().GetPixelsPerInch());
		ClientToScreen(&cp);
		return ::SetCursorPos(cp.x,cp.y);
	}
	BOOL SetCursorPosition(CPoint MP) {
		//------------------------------
		// Set the cursor to the screen
		// Coordinates define by MP
		//------------------------------
		BOOL rV;
		rV = ::SetCursorPos(MP.x, MP.y);
		return rV;
	}
	MouseIsHere WhereIsMouse();
	void StartMouseDragging();
	//------------------------------------------
	// Tool Bar Methods
	//------------------------------------------
	void ToolBarSetMousePosition(DOUBLEPOINT pos) {
		GetMyFrame()->ToolBarSetPosition(pos);
	}
	void ToolBarSetDebug(CString& csString) {
		GetMyFrame()->SetToolbarDebug(csString);
	}
	CComboBox& GetOriginSelectCombo() {
		return GetMyFrame()->GetToolBarView()->GetOriginSelCB();
	}
	CMyToolBarView* GetToolBarView() {
		return GetMyFrame()->GetToolBarView();
	}
	afx_msg LRESULT OnFromToolbarMessage(WPARAM wParam, LPARAM lParam);
	//------------------------------------------
	// Client Area Methods
	//------------------------------------------

	double GetClientWidth() {
		//--------------------------------------
		// returns the width of the client
		// rectangle in inches
		//-------------------------------------
		double rV = 0.0;
		CRect clientRect;
		GetClientRect(&clientRect);
		rV = double(clientRect.Width()) * GetGrid().GetInchesPerPixel().GetScaleX();
		rV = GETAPP.RoundUpToNearest(rV, GetGrid().GetSnapGrid().dCX);
		return rV;
	}
	double GetClientHieght() {
		double rV = 0.0;
		CRect clientRect;
		GetClientRect(&clientRect);
		rV = double(clientRect.Height()) * GetGrid().GetInchesPerPixel().GetScaleY();
		rV = GETAPP.RoundUpToNearest(rV, GetGrid().GetSnapGrid().dCY);
		return rV;
	}
	//---------------------------------------------
	// Manage objects in document
	//---------------------------------------------
	CFrontCadDoc* GetDocument() {
		return (CFrontCadDoc*)CChildViewBase::GetDocument();
	}
	CDoubleSize GetDocSize(void) { 
		return GetDocument()->GetDocSize();
	}
	void SetDocSize(CDoubleSize sZ) { 
		GetDocument()->SetDocSize(sZ);
	}
	//-----------------------------------------------
	// CCadOrigin management
	//-----------------------------------------------
	void AddOriginAtHead(CCadOrigin* pObj);
	void AddOriginAtTail(CCadOrigin* pObj);
	void RemoveOrigin(CCadOrigin* pObj);
	//-----------------------------------------------
	// Moving Object Methods
	//-----------------------------------------------
//	void* SetGrabbedPoint(CCadPoint* pCP) { m_pGrabbedPoint = pCP; }
	//--------------------------------------------------
	//  Drawing Methods
	//--------------------------------------------------
	ObjectDrawState GetDrawState() { return m_DrawState; }
	void SetDrawState(ObjectDrawState DS) { m_DrawState = DS; }
	DrawingMode GetDrawMode() { return m_DrawMode; }
	void SetDrawMode(DrawingMode DM) { m_DrawMode = DM; }
	//--------------------------------------------------
	// Menu Bar Commands
	//--------------------------------------------------
public:
	afx_msg void OnDrawArc();
	afx_msg void OnUpdateDrawArc(CCmdUI* pCmdUI);
	afx_msg void OnDrawArcCenter();
	afx_msg void OnUpdateDrawArccnter(CCmdUI* pCmdUI);
	afx_msg void OnDrawArrow();
	afx_msg void OnUpdateDrawArrow(CCmdUI* pCmdUI);
	afx_msg void OnDrawEllipse();
	afx_msg void OnUpdateDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnDrawLine();
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnLineLinefixedlen();
	afx_msg void OnUpdateLineLinefixedlen(CCmdUI* pCmdUI);
	afx_msg void OnLinePolyline();
	afx_msg void OnUpdateLinePolyline(CCmdUI* pCmdUI);
	afx_msg void OnDrawOrigin();
	afx_msg void OnUpdateDrawOrigin(CCmdUI* pCmdUI);
	afx_msg void OnDrawPoint();
	afx_msg void OnUpdateDrawPoint(CCmdUI* pCmdUI);
	afx_msg void OnDrawPolygon();
	afx_msg void OnUpdateDrawPolygohn(CCmdUI* pCmdUI);
	afx_msg void OnDrawRectangle();
	afx_msg void OnUpdateDrawRectangle(CCmdUI* pCmdUI);
	afx_msg void OnDrawRectangularhole();
	afx_msg void OnUpdateDrawRectangularhole(CCmdUI* pCmdUI);
	afx_msg void OnDrawRotatedrectangle();
	afx_msg void OnUpdateDrawRotatedrectangle(CCmdUI* pCmdUI);
	afx_msg void OnDrawRoundedrectangle();
	afx_msg void OnUpdateDrawRoundedrectangle(CCmdUI* pCmdUI);
	afx_msg void OnDrawRoundhole();
	afx_msg void OnUpdateDrawRoundhole(CCmdUI* pCmdUI);
	afx_msg void OnDrawRoundholewith1flat();
	afx_msg void OnUpdateDrawRoundholewith1flat(CCmdUI* pCmdUI);
	afx_msg void OnDrawRoundholewith2flats();
	afx_msg void OnUpdateDrawRoundholewith2flats(CCmdUI* pCmdUI);
	afx_msg void OnDrawText();
	afx_msg void OnUpdateDrawText(CCmdUI* pCmdUI);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnEditClearAll();
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnModifyLine();
	afx_msg void OnUpdateModifyLine(CCmdUI* pCmdUI);
	afx_msg void OnModifyMovepoint();
	afx_msg void OnUpdateModifyMovepoint(CCmdUI* pCmdUI);
	afx_msg void OnModifyRec();
	afx_msg void OnUpdateModifyRec(CCmdUI* pCmdUI);
	afx_msg void OnSnapSnapgridon();
	afx_msg void OnUpdateSnapSnapgridon(CCmdUI* pCmdUI);
	afx_msg void OnSnapSnapgridproperties();
	afx_msg void OnUpdateSnapSnapgridproperties(CCmdUI* pCmdUI);
	afx_msg void OnSnapSnaptoobject();
	afx_msg void OnUpdateSnapSnaptoobject(CCmdUI* pCmdUI);
	afx_msg void OnSnapSnaptotangent();
	afx_msg void OnUpdateSnapSnaptotangent(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomin();
	afx_msg void OnUpdateViewZoomin(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomout();
	afx_msg void OnUpdateViewZoomout(CCmdUI* pCmdUI);
	afx_msg void OnDrawPlaceBitmap();
	afx_msg void OnUpdateDrawPlacebitmap(CCmdUI* pCmdUI);
protected:
	DECLARE_MESSAGE_MAP()
public:
	//---------------------------------------------
	// Windows Messages
	//---------------------------------------------
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// mouse
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//context menu
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	// Keyboard
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//------------------------------------------------------
	// Scrollbars/Auto Scroll/ Zoom
	//------------------------------------------------------
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void DoVScroll(double Yinches, BOOL Absolute = FALSE, BOOL Update = TRUE);
	void DoHScroll(double Xinches, BOOL Absolute = FALSE, BOOL Update = TRUE);
	void UpdateScrollbarInfo(DOUBLEPOINT ULHC);
	void GetDeviceScrollSizes(CSize& sizePage, CSize& sizeLine);
	void EnableAutoScroll(BOOL flag);
	SCROLLINFO& GetHScrollBarInfo() { return m_HScrollInfo; }
	SCROLLINFO& GetVScrollBarInfo() { return m_VScrollInfo; }
	double GetHScrollPos(int nPos) { 
		double max;
		double result;

		max = double(GetHScrollBarInfo().nMax);
		result = double(nPos) / max;
		return result; 
	}
	double GetVScrollPos(int nPos) { 
		double max;
		double result;

		max = double(GetVScrollBarInfo().nMax);
		result = double(nPos) / max;
		return result;
	}
	BOOL IsAutoScrollEnabled() { return m_AutoScroll; }
	DOUBLEPOINT CalculateNewULHC(
		CScale CurrentScale,
		CScale NextScale,
		DOUBLEPOINT pointLocation,
		DOUBLEPOINT pointULHC
	);
	void ZoomIn(DOUBLEPOINT point);
	void ZoomOut(DOUBLEPOINT point);
	//----------------------------------------------
	void DrawCursor(
		CDC* pDC, 
		DOUBLEPOINT pos,
		CRect* pRect, 
		DOUBLEPOINT ULHC,
		CScale Scale, 
		COLORREF color
	);
	//------------------------------------------
	// ToolBar
	//------------------------------------------
	void ShowToolBar(BOOL bShow) {
		if(m_pParentFrame)
			m_pParentFrame->ShowToolBar(bShow);
	}
	//------------------------------------------
	// Rulers
	//------------------------------------------
	CRulerSplitterWnd* GetRulerSplitterWnd() {
		CRulerSplitterWnd* pRSW;

		pRSW = (CRulerSplitterWnd*)GetParent();
		return pRSW;
	}
	BOOL SendMessageToRulers(UINT msg, LPARAM data = 0);
	BOOL PostMessageToRulers(UINT msg, LPARAM data = 0);
	//--------------------------------
	// Rulers/Grid
	//--------------------------------
	CGrid* GetPtrToGrid() { return &m_Grid; }
	CGrid& GetGrid() { return m_Grid; }
	CRulerInfo& GetRulerInfo() { return m_RulerInfo; }
	CRulerInfo* GetPtrToRulerInfo() { return &m_RulerInfo; }
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnPuMenuHoverIndex(WPARAM index, LPARAM lparam);
	afx_msg LRESULT OnPuMenuSelectedIndex(WPARAM index, LPARAM lparam);
};

