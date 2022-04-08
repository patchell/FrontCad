#pragma once

// CFrontCadView.cpp view class

constexpr auto PROC_MOUSE_POSISTION_CORRECTED = 0;
constexpr auto PROC_MOUSE_POSISTION_SCALED = 1;
constexpr auto PROC_MOUSE_POSISTION_SNAPPED = 2;

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
	//----------------SnapGrid et all ------------------------
	CGrid m_Grid;
	//----------------- Rulers --------------------------------------------
	CRulerInfo m_RulerInfo;
	//----------------Scrolling and Document View ---------------------------
	SCROLLINFO m_HScrollInfo;	//Horizontal Scroll Info
	SCROLLINFO m_VScrollInfo;	//Vertical Scroll Info
	int m_VPageSize;	//vertical page size
	int m_HPageSize;	//horizontal page size
	int m_HScrollPos;	//position of horizontal scroll bar, in screen units
	int m_VScrollPos;	//position of virtical scroll bar, in screen units
	int m_IsScrolling;	//Indicates that the view is auto scrolling
	UINT m_AutoScrollTimerId;
	BOOL m_AutoScroll;	//enables auto scrolling flag
	//----------------Keyboard/Mouse------------------------
	BOOL m_ControlKeyDown;	//true when control key is pushed
	BOOL m_ShiftKeyDown;	// true when shift key is pushed
	BOOL m_MouseDown;	//left mouse button is down
	CDoublePoint m_CurMousePos;	//mouse position raw, screen units
	TRACKMOUSEEVENT m_TrackMouseEvent;
	BOOL m_MouseLeftWindow;
	MouseIsHere m_MouseLocation;
	BOOL m_LeftMouseButtonDown;
	//--------------------------------------------------------
	CDoublePoint m_Origin;	//More complicated than it seems
	unsigned m_ObjectEnables;
	//----------------------------------
	// Drawing Variables
	//----------------------------------
	ObjectDrawState m_DrawState;	// current state for the current drawmode
	DrawingMode m_DrawMode;		// what is going on in the drawing
	//----------------------------------
	// Object Selection Variables
	//---------------------------------
	int m_SelectionCount;	//count of selected objects
	CCadObject* m_pSelObjList;	//list of selected objects
	CCadObject* m_pSelObjEnd;	//end of selected objects list
	//---------------------------------
	// Moveing Objects Variables
	//---------------------------------
	CMoveObjects* m_pMoveObjects;	//container for objects being moved
	CCadObject* m_pGrabbedObject;	// Not Used?
	//---------------------------------
	// Misc Variables
	//---------------------------------
	int m_GrabbedVertex;	//vertex that is being dragged
	CADObjectTypes m_CadObj;
	CFrontCadChildFrame* m_pParent;

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
	void SetObjectEnables(unsigned u) { m_ObjectEnables |= u; }
	void ClearObjectEnables(unsigned u) { m_ObjectEnables &= ~u; }
	virtual void OnInitialUpdate();
	virtual void OnAutoScroll(CPoint point, BOOL bBeforeScroll);
	CADObjectTypes& GetObjectTypes() { return m_CadObj; }
	void SetObjectTypes(CCadObject* pObj) { GetObjectTypes().pCadObject = pObj; }
	//---------------------------------------------
	// Keyboard/Mouse Methods
	//---------------------------------------------
	BOOL IsControlKeyDown() { return m_ControlKeyDown; }
	BOOL IsShiftKeyDown() { return m_ShiftKeyDown; }
	BOOL IsLeftMouseButtonDown() { return m_LeftMouseButtonDown; }
	CDoublePoint GetCurrentMousePosition() { return m_CurMousePos; }
	void SetCurrentMousePosition(CDoublePoint DP) { m_CurMousePos = DP; }
	CDoublePoint ProcessMousePosition(CPoint point, CDoubleSize SnapGrid, int Which);
	CDoublePoint ConvertMousePosition(
		CPoint MousePoint,
		CSize Offset,
		CScale Scale,
		CDoubleSize SnapGrid,
		BOOL SnapGridIsEnabled
	);
	BOOL DidMouseLeaveWindow() { return m_MouseLeftWindow; }
	void SetMouseLeftWindow(BOOL flag) { m_MouseLeftWindow = flag; }
	afx_msg void OnMouseLeave();
	CPoint GetMouseScreenCoordinate(CPoint& point) {
		GetCursorPos(&point);
		return point;
	}
	CPoint ConvertScreenToClientCoordinates(CPoint& point) {
		ScreenToClient(&point);
		return point;
	}
	MouseIsHere WhereIsMouse();
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
	virtual CFrontCadDoc* GetDocument() {
		return (CFrontCadDoc*)CChildViewBase::GetDocument();
	}
	CDoubleSize GetDocSize(void) { 
		return GetDocument()->GetDocSize();
	}
	void SetDocSize(CDoubleSize sZ) { 
		GetDocument()->SetDocSize(sZ);
	}
	virtual void RemoveObject(CCadObject* pObj);
	virtual void AddObjectAtFrontIntoDoc(CCadObject* pObj);
	virtual void AddObjectAtEndIntoDoc(CCadObject* pObj);
	void SetOrigin(CDoublePoint o) { m_Origin = o; }
	CDoublePoint GetOrigin(void) { return m_Origin; }
	//-------------------------------------------------
	// Selection of objects methods
	//-------------------------------------------------
	void AddToSelList(CCadObject* pO);
	CCadObject* GetTopSelection() { return m_pSelObjList; }
	void RemoveUnselectedObjects();
	void RemoveSelectedObjects(CCadObject* pO);
	void ClearSelList();
	int SelectAnObject(CCadObject** ppObj, int n, CPoint p);
	//-----------------------------------------------
	// Moving Object Methods
	//-----------------------------------------------
	CMoveObjects* GetMoveObjectes() { return m_pMoveObjects; }
	void SetMoveObjects(CMoveObjects* pMO) { m_pMoveObjects = pMO; }
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
	afx_msg void OnDrawArcangle();
	afx_msg void OnUpdateDrawArcangle(CCmdUI* pCmdUI);
	afx_msg void OnDrawArcCenter();
	afx_msg void OnUpdateDrawArccnter(CCmdUI* pCmdUI);
	afx_msg void OnDrawArrow();
	afx_msg void OnUpdateDrawArrow(CCmdUI* pCmdUI);
	afx_msg void OnDrawEllipse();
	afx_msg void OnUpdateDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnDrawLine();
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
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
	// Scrollbars/Auto Scroll
	CSize GetScrollOffset() { return CSize(m_HScrollPos, m_VScrollPos); }
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void DoVScroll(double Vinches, BOOL Update = TRUE);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void DoHScroll(double change, BOOL Update = TRUE);
	void UpdateScrollbarInfo();
	void GetDeviceScrollSizes(CSize& sizePage, CSize& sizeLine);
	void DrawCursor(CDC* pDC, CDoublePoint pos, CRect* pRect, CSize Offset, CScale Scale, COLORREF color);
	CPoint CalculateHsVs(CPoint p, CPoint DesiredScreenLocation);
	void SetHScrollPos(int HSP) { m_HScrollPos = HSP; }
	int GetHScrollPos() { return m_HScrollPos; }
	void SetVScrollPos(int VSP) { m_HScrollPos = VSP; }
	int GetVScrollPos() { return m_VScrollPos; }
	void ZoomIn(CPoint point);
	void ZoomOut(CPoint point);
	void EnableAutoScroll(BOOL flag);
	BOOL IsAutoScrollEnabled() { return m_AutoScroll; }
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
};

