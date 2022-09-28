//---------------------------------------------
// FrontCadView.cpp : implementation file
//----------------------------------------------

#include "pch.h"

// CFrontCadView

IMPLEMENT_DYNCREATE(CFrontCadView, CChildViewBase)

CFrontCadView::CFrontCadView()
{
	m_pParentFrame = 0;
	m_ObjectEnables = 0;	//all objects disabled
	m_ControlKeyDown = FALSE;
	m_ShiftKeyDown = FALSE;
	m_DrawMode = DrawingMode::SELECT;
	m_DrawState = ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN;
	m_CadObj.pCadObject = 0;
	m_MouseLeftWindow = FALSE;
	m_MouseLocation = MouseIsHere::NOWHERE;
	m_AutoScrollTimerId = TIMER_ID_AUTOSCROLL;
	m_MouseLButtonHeldDownId = TIMER_ID_LBUTTON_DOWN;;
	m_MouseLButtonHeldDown = FALSE;
	memset(&m_HScrollInfo, 0, sizeof(SCROLLINFO));
	memset(&m_VScrollInfo, 0, sizeof(SCROLLINFO));
	m_HScrollInfo.cbSize = sizeof(SCROLLINFO);
	m_HScrollInfo.cbSize = sizeof(SCROLLINFO);
	m_R_KeyDown = FALSE;
	m_GrabbedObject.pCadObject = 0;
	m_ppObjList = 0;
	m_NumOfMessListEntries = 0;
	nLastItemIndex = -1;
}

CFrontCadView::~CFrontCadView()
{
}

BEGIN_MESSAGE_MAP(CFrontCadView, CChildViewBase)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSELEAVE()
	ON_WM_TIMER()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_MESSAGE(UINT(WindowsMsg::WM_FROM_TOOLBAR_MESSAGE), &CFrontCadView::OnFromToolbarMessage)
	ON_MESSAGE(UINT(WindowsMsg::WM_PU_MENU_HOVER_INDEX),&CFrontCadView::OnPuMenuHoverIndex)
	ON_MESSAGE(UINT(WindowsMsg::WM_PU_MENU_SELECTED_INDEX),&CFrontCadView::OnPuMenuSelectedIndex)
	//----- Menu Draw Objects ------
	ON_COMMAND(ID_ARC_ARCFROMCENTER, &CFrontCadView::OnDrawArcCenter)
	ON_UPDATE_COMMAND_UI(ID_ARC_ARCFROMCENTER, &CFrontCadView::OnUpdateDrawArccnter)
	ON_COMMAND(ID_ARC_ARCINRECT, &CFrontCadView::OnDrawArc)
	ON_UPDATE_COMMAND_UI(ID_ARC_ARCINRECT, &CFrontCadView::OnUpdateDrawArc)
	ON_COMMAND(ID_DRAW_ARROW, &CFrontCadView::OnDrawArrow)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ARROW, &CFrontCadView::OnUpdateDrawArrow)
	ON_COMMAND(ID_DRAW_ELLIPSE, &CFrontCadView::OnDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, &CFrontCadView::OnUpdateDrawEllipse)
	ON_COMMAND(ID_LINE_LINE, &CFrontCadView::OnDrawLine)
	ON_UPDATE_COMMAND_UI(ID_LINE_LINE, &CFrontCadView::OnUpdateDrawLine)
	ON_COMMAND(ID_DRAW_ORIGIN, &CFrontCadView::OnDrawOrigin)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ORIGIN, &CFrontCadView::OnUpdateDrawOrigin)
	ON_COMMAND(ID_DRAW_POINT, &CFrontCadView::OnDrawPoint)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POINT, &CFrontCadView::OnUpdateDrawPoint)
	ON_COMMAND(ID_LINE_POLYGON, &CFrontCadView::OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_LINE_POLYGON, &CFrontCadView::OnUpdateDrawPolygohn)
	ON_UPDATE_COMMAND_UI(ID_LINE_RECTANGLE, &CFrontCadView::OnUpdateDrawRectangle)
	ON_COMMAND(ID_LINE_RECTANGLE, &CFrontCadView::OnDrawRectangle)
	ON_COMMAND(ID_HOLE_SQUAREHOLE, &CFrontCadView::OnDrawRectangularhole)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGULARHOLE, &CFrontCadView::OnUpdateDrawRectangularhole)
	ON_COMMAND(ID_LINE_ROTATEDRECT, &CFrontCadView::OnDrawRotatedrectangle)
	ON_UPDATE_COMMAND_UI(ID_LINE_ROTATEDRECT, &CFrontCadView::OnUpdateDrawRotatedrectangle)
	ON_COMMAND(ID_DRAW_ROUNDEDRECTANGLE, &CFrontCadView::OnDrawRoundedrectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROUNDEDRECTANGLE, &CFrontCadView::OnUpdateDrawRoundedrectangle)
	ON_COMMAND(ID_HOLE_ROUNDHOLE, &CFrontCadView::OnDrawRoundhole)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROUNDHOLE, &CFrontCadView::OnUpdateDrawRoundhole)
	ON_COMMAND(ID_DRAW_ROUNDHOLEWITH1FLAT, &CFrontCadView::OnDrawRoundholewith1flat)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROUNDHOLEWITH1FLAT, &CFrontCadView::OnUpdateDrawRoundholewith1flat)
	ON_COMMAND(ID_DRAW_ROUNDHOLEWITH2FLATS, &CFrontCadView::OnDrawRoundholewith2flats)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROUNDHOLEWITH2FLATS, &CFrontCadView::OnUpdateDrawRoundholewith2flats)
	ON_COMMAND(ID_DRAW_TEXT, &CFrontCadView::OnDrawText)
	ON_UPDATE_COMMAND_UI(ID_DRAW_TEXT, &CFrontCadView::OnUpdateDrawText)
	//------ Menu Edit Commands
	ON_COMMAND(ID_EDIT_CLEAR, &CFrontCadView::OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &CFrontCadView::OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, &CFrontCadView::OnEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, &CFrontCadView::OnUpdateEditClearAll)
	ON_COMMAND(ID_EDIT_COPY, &CFrontCadView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CFrontCadView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CFrontCadView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CFrontCadView::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CFrontCadView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CFrontCadView::OnUpdateEditPaste)
	//--------- Menu Modify Commands
	ON_COMMAND(ID_MODIFY_LINE, &CFrontCadView::OnModifyLine)
	ON_UPDATE_COMMAND_UI(ID_MODIFY_LINE, &CFrontCadView::OnUpdateModifyLine)
	ON_COMMAND(ID_MODIFY_MOVEPOINT, &CFrontCadView::OnModifyMovepoint)
	ON_UPDATE_COMMAND_UI(ID_MODIFY_MOVEPOINT, &CFrontCadView::OnUpdateModifyMovepoint)
	ON_COMMAND(ID_MODIFY_REC, &CFrontCadView::OnModifyRec)
	ON_UPDATE_COMMAND_UI(ID_MODIFY_REC, &CFrontCadView::OnUpdateModifyRec)
	//------- Menu Snap Grid
	ON_COMMAND(ID_SNAP_SNAPGRIDON, &CFrontCadView::OnSnapSnapgridon)
	ON_UPDATE_COMMAND_UI(ID_SNAP_SNAPGRIDON, &CFrontCadView::OnUpdateSnapSnapgridon)
	ON_COMMAND(ID_SNAP_SNAPGRIDPROPERTIES, &CFrontCadView::OnSnapSnapgridproperties)
	ON_UPDATE_COMMAND_UI(ID_SNAP_SNAPGRIDPROPERTIES, &CFrontCadView::OnUpdateSnapSnapgridproperties)
	ON_COMMAND(ID_SNAP_SNAPTOOBJECT, &CFrontCadView::OnSnapSnaptoobject)
	ON_UPDATE_COMMAND_UI(ID_SNAP_SNAPTOOBJECT, &CFrontCadView::OnUpdateSnapSnaptoobject)
	ON_COMMAND(ID_SNAP_SNAPTOTANGENT, &CFrontCadView::OnSnapSnaptotangent)
	ON_UPDATE_COMMAND_UI(ID_SNAP_SNAPTOTANGENT, &CFrontCadView::OnUpdateSnapSnaptotangent)
	//------------ Menu View Commands
	ON_COMMAND(ID_VIEW_ZOOMIN, &CFrontCadView::OnViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, &CFrontCadView::OnUpdateViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, &CFrontCadView::OnViewZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, &CFrontCadView::OnUpdateViewZoomout)
	ON_COMMAND(ID_DRAW_PLACEBITMAP, &CFrontCadView::OnDrawPlaceBitmap)
	ON_UPDATE_COMMAND_UI(ID_DRAW_PLACEBITMAP, &CFrontCadView::OnUpdateDrawPlacebitmap)
	ON_COMMAND(ID_LINE_LINEFIXEDLEN, &CFrontCadView::OnLineLinefixedlen)
	ON_UPDATE_COMMAND_UI(ID_LINE_LINEFIXEDLEN, &CFrontCadView::OnUpdateLineLinefixedlen)
	ON_COMMAND(ID_LINE_POLYLINE, &CFrontCadView::OnLinePolyline)
	ON_UPDATE_COMMAND_UI(ID_LINE_POLYLINE, &CFrontCadView::OnUpdateLinePolyline)
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_MENUSELECT()
	ON_COMMAND(ID_SNAP_SNAPORDER, &CFrontCadView::OnSnapSnaporder)
	ON_UPDATE_COMMAND_UI(ID_SNAP_SNAPORDER, &CFrontCadView::OnUpdateSnapSnaporder)
END_MESSAGE_MAP()


// CFrontCadView drawing

void CFrontCadView::OnDraw(CDC* pDC)
{
//	CCadPoint UprLHCorner;
	CDC memDC;
	CRect rectClient;
	CBitmap memDCbitmap;
	CBitmap* pOldbm;
	CFrontCadDoc* pDoc = GetDocument();
	CCadObject* pDrawingObjectList;
	DOUBLEPOINT ULHC;		//upper left hand corner offset
	CBrush br;
	MODE mode;
	static int count;

	ULHC = GetRulerInfo().GetUpperLeft();
	GetClientRect(&rectClient);
	memDC.CreateCompatibleDC(pDC);
	memDCbitmap.CreateCompatibleBitmap(pDC, rectClient.Width(), rectClient.Height());
	pOldbm = memDC.SelectObject(&memDCbitmap);
	pDrawingObjectList = pDoc->GetHead();
	CScale Scale = GetGrid().GetPixelsPerInch();
	mode.DrawMode = ObjectDrawMode::FINAL;

	br.CreateSolidBrush(RGB(0, 0, 0));
	memDC.FillRect(&rectClient, &br);

	GetGrid().Draw(&memDC, mode, ULHC, Scale, rectClient);
	while (pDrawingObjectList)
	{
		pDrawingObjectList->Draw(&memDC, mode, ULHC, Scale);
		pDrawingObjectList = pDrawingObjectList->GetNext();
	}
	if (GetObjectTypes().pCadObject)	//is an object being draw?
	{
		switch (GetDrawState())
		{
		case ObjectDrawState::SET_ATTRIBUTES:
		case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN :
		case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		case ObjectDrawState::PLACE_LBUTTON_DOWN:
		case ObjectDrawState::PLACE_LBUTTON_UP:
		case ObjectDrawState::PLACE_AUTO:
		case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_DOWN:
		case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_UP:
		case ObjectDrawState::FIXED_LINE_RIGHTANGLE_MOUSE_DOWN:
		case ObjectDrawState::FIXED_LINE_RIGHTANGLE_MOUSE_UP:
		case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN:
		case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_UP:
			mode.DrawMode = ObjectDrawMode::SKETCH;
			break;
		case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		case ObjectDrawState::ARCSTART_LBUTTON_UP:
			mode.DrawMode = ObjectDrawMode::ARCSTART;
			break;
		case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		case ObjectDrawState::ARCEND_LBUTTON_UP:
			mode.DrawMode = ObjectDrawMode::ARCEND;
			break;
		}
		GetObjectTypes().pCadObject->Draw(&memDC, mode, ULHC, Scale);
	}
//	if (GetMoveObjectes())	//are there any objects being moved?
//		GetMoveObjectes()->Draw(&memDC, UprLHCorner, Scale);
//	pDoc->DrawReference(&memDC, Offset, Scale, rectClient);
	DrawCursor(
		&memDC, 
		GetCurrentMousePosition(),
		&rectClient,
		ULHC,
		Scale,
		RGB(255, 255, 255)
	);
	pDC->BitBlt(
		0, 
		0, 
		rectClient.right, 
		rectClient.bottom, 
		&memDC, 
		0, 
		0, 
		SRCCOPY
	);
	//------------- Cleanup ------------------------
	memDC.SelectObject(pOldbm);

}

// CFrontCadView diagnostics

#ifdef _DEBUG
void CFrontCadView::AssertValid() const
{
	CChildViewBase::AssertValid();
}

#ifndef _WIN32_WCE
void CFrontCadView::Dump(CDumpContext& dc) const
{
	CChildViewBase::Dump(dc);
}
#endif
#endif //_DEBUG


// CFrontCadView message handlers


//--------------------------------------------
// User Interface Input devices
//-------------------------------------------

BOOL CFrontCadView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	DOUBLEPOINT pointMousePos;
	ScreenToClient(&pt);

	pointMousePos = ConvertMousePosition(
		pt,
		GetRulerInfo().GetUpperLeft(),
		GetGrid().GetInchesPerPixel(),
		GetGrid().GetSnapGrid(),
		GetGrid().IsSnapOn()
	);

	if (IsShiftKeyDown())	//scroll up or down
	{
		DoVScroll(double(zDelta)/120.0 * GetGrid().GetSnapGrid().dCX);
	}
	else if (IsControlKeyDown())	//scroll left or right
	{
		DoHScroll(double(zDelta) / 120.0 * GetGrid().GetSnapGrid().dCY);
	}
	else
	{
		// Zoom In or Out
		zDelta /= 120;
		if (zDelta > 0)	//zoom in
		{
			ZoomIn(pointMousePos);
		}
		else  //Zoom Out
		{
			ZoomOut(pointMousePos);
		}
		PostMessageToRulers(RW_ZOOM);
	}
	CFrontCadDoc* pDoc = GetDocument();
	return CChildViewBase::OnMouseWheel(nFlags, zDelta, pt);
}

//--------------------------------------------------------
//  Mouse State Machine
// The combination of OnLButtonDown, OnLButtonUp and
// OnMouseMove methods together process the state machine
// that controls almost all drawing actions.  Each
// drawing mode, pretty much, has it's own state machine
// however, the state is a combination of both the draw
// mode, weather the left mouse button is up or down
// and the draw state
// -------------------------------------------------------
// 
// DRAWMODE:SELECT
//		Mouse Down With CTRL key
//			Select object, append to selected list
//			If Mouse Down for time T
//				Drawmode = Move
//		Mouse Down Only
//			Clear Selected List
//			Add Object to Selected List
//			If Mouse Down for time T
//				Drawmode = Move
//		Mouse Down but No objects to select
//			Drawmode = SelectRegion
// DRAWMODE:MOVE
//		While Mouse Button is Down
//			Move Object(s) around.
//		When Button released
//			Place Objects
// DRAWMODE:COPY
//		Not a mouse operation (CTRL-C)
// DRAWMODE:CUT
//		Not a mouse operation (CTRL-C)
// DRAWMODE:PASTE
//		While Mouse is Moving
//			Move objects around
//		When button is clicked
//			Place objects.
// DRAWMODE:SELECTREGION
//		While Mouse Button down
//			Draw selection region
//		When Mouse button is up
//			Select Objects in region
// DRAWMODE:Everything else
//		Set Drawmode for specified object.
//		Sequence defined by the object.
//--------------------------------------------------------

void CFrontCadView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//-------------------------------------------------------
	// OnLButtonDown
	//
	//      This Method handles messages created when the
	// presses the left mouse button.
	//
	//  nFlags......various flag bits
	//  point.......mouse pointer coordinates for this window
	//------------------------------------------------------
	DOUBLEPOINT MousePos;
	CFrontCadDoc* pDoc = GetDocument();
	CCadObject* pObj = 0;

	m_LeftMouseButtonDown = TRUE;
	MousePos = ConvertMousePosition(
		point,
		GetRulerInfo().GetUpperLeft(),
		GetGrid().GetInchesPerPixel(),
		GetGrid().GetSnapGrid(),
		GetGrid().IsSnapOn()
	);
	//------------------------------------
	// Start a timer to see if the left
	// button is being held down
	// 
	// If it is being held down, then
	// the mouse will continue to be
	// processed in the MouseMove and
	// OnLButtonUp methods
	//------------------------------------
	m_MouseLButtonHeldDownId = SetTimer(
		m_MouseLButtonHeldDownId,
		TIMER_LBUTTON_DOWN_TIME,
		NULL
	);
	m_MouseLButtonHeldDown = FALSE;
	//-----------------------------------------
	switch (m_DrawMode)
	{
	case DrawingMode::SELECT:	// OnLButtonDown
		//-----------------------------------
		// DrawingMode::SELECT
		// This is the default Draw Mode
		//-----------------------------------
		switch (m_DrawState)
		{
		case ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN:	//Mode:Select OnLButtonDown
			m_DrawState = ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN_TIMEOUT;
			break;
		case ObjectDrawState::SELECT_MOVE:		//Mode:Select OnLButtonDown
			break;
		case ObjectDrawState::SELECT_COPY:		//Mode:Select OnLButtonDown
			break;
		}
		Invalidate();
		break;
	case DrawingMode::MOVE:		// OnLButtonDown
		break;
	case DrawingMode::COPY:		// OnLButtonDown
		break;
	case DrawingMode::CUT:		// OnLButtonDown
		break;
	case DrawingMode::PASTE:	//OnLButtonDown
		switch (m_DrawState)
		{
		case ObjectDrawState::PASTE_WAITFOR_LBUTTON_DOWN:	//Mode:PASTE OnLButtonDown
			m_DrawState = ObjectDrawState::PASTE_WAITFOR_LBUTTON_DOWN;
			Invalidate();
			break;
		}	//End of Switch(DrawState)
		break;
	case DrawingMode::SELECTREGION:	// OnLButtonDown
		break;
	default:	// OnLButtonDownit must be something we are going to draw.
		if (m_CadObj.pCadObject)
		{
			m_DrawState = m_CadObj.pCadObject->ProcessDrawMode(m_DrawState);
			if (m_DrawState == ObjectDrawState::END_DRAWING)
			{
				m_DrawState = m_CadObj.pCadObject->ProcessDrawMode(m_DrawState);
				delete m_CadObj.pCadObject;
				m_CadObj.pCadObject = 0;
			}
		}
		break;
	}
	CChildViewBase::OnLButtonDown(nFlags, point);
}

void CFrontCadView::OnLButtonUp(UINT nFlags, CPoint point)
{
	//-------------------------------------------------------
	// OnLButtonUp
	//
	//      This Method handles messages created when the
	// releases the left mouse button.
	//
	//  nFlags......various flag bits
	//  point.......mouse pointer coordinates for this window
	//------------------------------------------------------
	CCadObject** ppSel, * pObjSel, * pObj;
	int n;
	DOUBLEPOINT MousePos;
	CFrontCadDoc* pDoc = GetDocument();

	MousePos = ConvertMousePosition(
		point,
		GetRulerInfo().GetUpperLeft(),
		GetGrid().GetInchesPerPixel(),
		GetGrid().GetSnapGrid(),
		GetGrid().IsSnapOn()
	);
	m_LeftMouseButtonDown = FALSE;
	m_MouseLButtonHeldDown = FALSE;
	switch (m_DrawMode)
	{
	case DrawingMode::SELECT:
		//-----------------------------------------------------
		//-----------------------------------------------------
		switch (m_DrawState)
		{
		case ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN_TIMEOUT:
			KillTimer(m_MouseLButtonHeldDownId);	//Don't need timer anymore
			//go onto Wait for mouse button up processing
		case ObjectDrawState::SELECT_WAITFOR_LBUTTON_UP:	// Mode:Select/LButtonUP
			if (m_ControlKeyDown)	//Select/Toggle Multiple Ofjects
			{
				ppSel = new CCadObject * [DEFALT_SELECT_BUFFER_SIZE];
				for (int i = 0; i < DEFALT_SELECT_BUFFER_SIZE; ++i)
					ppSel[i] = 0;
				//-------------------------------------
				// If there is a selected object,
				// add it to the selected list
				//-------------------------------------
				n = pDoc->PointInObjectAndSelect(
					MousePos,
					NULL,
					ppSel,
					DEFALT_SELECT_BUFFER_SIZE,
					OBJKIND_ALL
				);
			}
			else
			{
				//------------------------------------
				// Clear all selected objects and
				// check to see if the mouse clicked
				// in an object
				//------------------------------------
				pDoc->UnSelectAll();
			}
			ppSel = new CCadObject * [DEFALT_SELECT_BUFFER_SIZE];
			if (n = pDoc->PointInObjectAndSelect(
				GetCurrentMousePosition(),
				NULL,
				ppSel,
				DEFALT_SELECT_BUFFER_SIZE,
				OBJKIND_ALL)
			)
			{											//OnLButtonUp
				//we have found some objects to select
				if (n > 1)
				{
					//Let the user choose the object
					int index = pDoc->SelectAnObject(ppSel, n, MousePos);
					if (index < 0)
						break;
					pObjSel = ppSel[index];
				}
				else
					pObjSel = ppSel[0];
				if (pObjSel->IsSelected())
				{
					pObjSel->SetSelected(0);	//deselect object
					pDoc->RemoveSelectedObject(pObjSel);
				}
				else
				{
					if (pDoc->GetTotalSelectedItems() && !m_ControlKeyDown)
					{
						CCadObject* pO = pDoc->GetSelListHead();
						while (pO)
						{
							pDoc->RemoveSelectedObject(pO);	//remove object from selection list
							pO->SetSelected(0);	//set to unselected
							pO = pDoc->GetSelListHead();	//get next object, if any
						}
					}
					pObjSel->SetSelected(1);
					pDoc->AddToSelListHead(pObjSel);
				}
				Invalidate();
			}//end of if(n = pDoc->PointInObjectAndSelect(m_ScalePos, pSel, 8, DrawingCheckSelectFlags::FLAG_ALL))
			else
			{
				//--------------------------------
				// No objects were selected
				// So, just to irritate the user
				// we will deselect all of the objects
				// that have already been selected
				//---------------------------------
				if (pDoc->GetTotalSelectedItems() && !m_ControlKeyDown)
				{
					CCadObject* pO = pDoc->GetSelListHead();
					while (pO)
					{
						pDoc->RemoveSelectedObject(pO);	//remove object from selection list
						pO->SetSelected(FALSE);	//set to unselected
						pO = pDoc->GetSelListHead();	//get next object, if any
					}
					Invalidate();
				}
			}
			break;
		case ObjectDrawState::SELECT_LBUTTON_DOWN_DRAGGING:
			m_DrawState = ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN;
			break;
		case ObjectDrawState::SELECT_MOVE:	// Mode:Select/LButtonUP
		case ObjectDrawState::SELECT_COPY:	// Mode:Select/LButtonUP
			pObj = pDoc->RemoveSelectedObjectFromHead();
			while (pObj)
			{
				pObj->Move(GetDeltaMousePos());
				pObj->SetSelected(0);
				pDoc->AddObjectAtTail(pObj);
				pObj = pDoc->RemoveSelectedObjectFromHead();
			}
			m_DrawState = ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN;
			Invalidate();
			break;
		}	//End of switch(DrawState) LButtonUp
		if (m_GrabbedObject.pCadObject)
		{
			m_GrabbedObject.pCadObject->Move(GetCurrentMousePosition());
			m_GrabbedObject.pCadObject = 0;
		}
		break;
	case DrawingMode::MOVE:		//LButton Up
		//-----------------------------------------------------
		//-----------------------------------------------------
		break;
	case DrawingMode::COPY:		//LButton Up
		GETAPP.GetClipBoard().RemoveAndDestroyALL();		//clear old contents from clipboard
		pDoc->CopyToClipBoard();
		GETAPP.GetClipBoard().SetRef(GetCurrentMousePosition());
		Invalidate();
		break;
	case DrawingMode::CUT:		//LButton Up
		pDoc->CutToClipBoard();
		GETAPP.GetClipBoard().SetRef(GetCurrentMousePosition());
		Invalidate();
		break;
	case DrawingMode::PASTE:	//Mouse Button Up
		switch (m_DrawState)
		{
		case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:		//Mode:PASTE LButtonUP
			GETAPP.UpdateStatusBar(_T("PASTE:Position Objects"));
			m_DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
			break;
		case ObjectDrawState::PLACE_LBUTTON_UP:		//Mode:PASTE LButtonUP
			GETAPP.UpdateStatusBar(_T("SELECT an Object"));
			m_DrawMode = DrawingMode::SELECT;
			m_DrawState = ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN;
			{
				//---------------------------------------
				//	Copy Everyting from the clipboard
				// and add it to the drawing
				//--------------------------------------
				pObj = GETAPP.GetClipBoard().RemoveFromHead();
				while (pObj)
				{
					pObj->SetSelected(0);		//unselect
					pObj->Move(GetDeltaMousePos());
					pDoc->AddObjectAtTail(pObj);
				}
			}
			break;
		}	//end of switch(DrawState)
		break;
	case DrawingMode::SELECTREGION:		// LButton Up
		break;
	default:	//this is where the bulk of the operations occure
				//such as drawing objects
		if (m_CadObj.pCadObject) m_DrawState = m_CadObj.pCadObject->ProcessDrawMode(m_DrawState);
		break;
	}
	CChildViewBase::OnLButtonUp(nFlags, point);
}

void CFrontCadView::OnMouseMove(UINT nFlags, CPoint point)
{
	///----------------------------------------------
	/// OnMouseMove
	///
	///     This fuction implements the methodes
	/// that handle the mouse moving in a cad drawing
	///
	/// parameters:
	///     see MSDN docs for CChildViewBase::OnMouseMove
	///-----------------------------------------------
	CString csDB;
	CFrontCadDoc* pDoc = GetDocument();
	DOUBLEPOINT OriginCenterPoint;	//center of current origin
	DOUBLEPOINT pointMousePos;
	CPoint ptScreen = point;
	DOUBLEPOINT MousePos;
	CCadObject* pObj;

	ClientToScreen(&ptScreen);

	csDB.Format(_T("C(%d, %d)  S(%d,%d)"), point.x, point.y, ptScreen.x, ptScreen.y);
	ToolBarSetDebug(csDB);
	MousePos = ConvertMousePosition(
		point,
		GetRulerInfo().GetUpperLeft(),
		GetGrid().GetInchesPerPixel(),
		GetGrid().GetSnapGrid(),
		GetGrid().IsSnapOn()
	);
	ToolBarSetMousePosition(GetCurrentMousePosition());
	//----------------------------------------------------
	// Prosses mouse position to find out if it is
	// In the View Client Area
	//----------------------------------------------------
	if (DidMouseLeaveWindow())
	{
		SetMouseLeftWindow(FALSE);
		TrackMouseEvent(&m_TrackMouseEvent);
		if (IsMiddleMouseButtonDown())
		{
			SetMiddleMouseButtonDown(FALSE);
			EnableAutoScroll(0);
		}
	}
	//------------------------------------------
	// Processing for the various draw modes
	//------------------------------------------
	switch (m_DrawMode)
	{
	case DrawingMode::SELECT:	//OnMouseMove
		switch (m_DrawState)
		{
		case ObjectDrawState::SELECT_LBUTTON_DOWN_DRAGGING:
		case ObjectDrawState::SELECT_WAITFOR_LBUTTON_UP:
			//-----------------------------------
			// If the mouse moves,
			// while the LButton is down, we are then
			// going to grab and either move or
			// copy/cut the objects
			//------------------------------------
			pObj = pDoc->GetSelListHead();
			while (pObj)
			{
				pObj->Move(GetDeltaMousePos());
				pObj = pObj->GetNext();
			}
			break;
		case ObjectDrawState::SELECT_MOVE:
		case ObjectDrawState::SELECT_COPY:
			break;
		}//end of switch ObjectDrawState
		Invalidate();
		break;
	case DrawingMode::MOVE:	//OnMouseMove
		break;
	case DrawingMode::COPY:	//OnMouseMove
		Invalidate();
		break;
	case DrawingMode::CUT:	//OnMouseMove
		Invalidate();
		break;
	case DrawingMode::PASTE:	//OnMouseMove
		Invalidate();
		break;
	case DrawingMode::SELECTREGION:	//OnMouseMove
		break;
	default:	//OnMouseMove
		//--------------------------------------------
		// If we get here, we are in the midst of
		// actually drawing an object
		//--------------------------------------------
		if (m_CadObj.pCadObject) 
			m_DrawState = m_CadObj.pCadObject->MouseMove(m_DrawState);
//		else
//			Invalidate();
		break;
	}

	CChildViewBase::OnMouseMove(nFlags, point);
	PostMessageToRulers(RW_POSITION);
}

void CFrontCadView::OnInitialUpdate()
{
	m_pParentFrame = (CFrontCadChildFrame*)GetParentFrame();
	CFrontCadDoc *pDoc = GetDocument();
	CString csOrgName = _T("Default");

	//------------ Setup Default Origin --------------
	m_DefaultOrigin.Create(NULL, NULL);
	m_DefaultOrigin.SetName(csOrgName);
	pDoc->AddObjectAtHead(&m_DefaultOrigin);
	GetRulerInfo().SetOrigin(&m_DefaultOrigin);
	pDoc->SetCurrentOrigin(&m_DefaultOrigin);
	// This will be finished up later when
	// a message is recieved that the toolbard
	// has been set up.
	//-----------------------------------------------
	pDoc->SetDocView(this);
	SetObjectEnables(
		OBJECT_ENABLE_ROUNDEDRECT |
		OBJECT_ENABLE_POLYGON |
		OBJECT_ENABLE_TEXT |
		OBJECT_ENABLE_BITMAP |
		OBJECT_ENABLE_BITMAP |
		OBJECT_ENABLE_BITMAP |
		OBJECT_ENABLE_ARROW |
		OBJECT_ENABLE_ORIGIN 
	);
	//-----------------------------------------
	GetMyFrame()->InitToolBar(this, GetPtrToRulerInfo());
	//------------- Scroll Bars ---------------
	CCadPoint ulhc = CCadPoint(0.0, 0.0);
	GetRulerInfo().SetUpperLeft(ulhc);
	GetGrid().SetSnapGrid(CDoubleSize(0.125,0.125));
	UpdateScrollbarInfo(GetRulerInfo().GetUpperLeft());
	ShowScrollBar(SB_BOTH, TRUE);
	EnableScrollBarCtrl(SB_VERT, TRUE);
	EnableScrollBarCtrl(SB_HORZ, TRUE);
	//----------------------------------------------------
	//----------------------------------------------------
	ShowToolBar(TRUE);
	//----------------------------------------------------
	// Initialize Ruler Info
	//----------------------------------------------------
	GetRulerInfo().SetBackGroundColor(GETAPP.GetRulerAttributes()->m_colorBk);
	GetRulerInfo().SetCornerColor(GETAPP.GetRulerAttributes()->m_colorCorner);
	GetRulerInfo().SetCursorColor(GETAPP.GetRulerAttributes()->m_colorCursor);
	GetRulerInfo().SetMajorTickLength(GETAPP.GetRulerAttributes()->m_MajTickLength);
	GetRulerInfo().SetDocSize(pDoc->GetDocSize());
	CRect rect;
	GetClientRect(&rect);
	GetRulerInfo().SetClientSize(rect.Size());
	GetRulerInfo().SetGrid(GetPtrToGrid());
	GetRulerInfo().SetCursorPos(GetCurrentMousePosition());
	GetRulerInfo().GetAttributes().CopyFrom(GETAPP.GetRulerAttributes());
	//------------------------------------------------------
	// Initialize Grid and Scale stuff
	//------------------------------------------------------
	GetGrid().SetMajLineColor(GETAPP.GetGridAttributes()->m_colorMajorLine);
	GetGrid().SetMajLineWidth(GETAPP.GetGridAttributes()->m_MajorLineWidth);
	GetGrid().SetMajorGrid(CDoubleSize(1.0, 1.0));
	GetGrid().SetSnapGrid(CDoubleSize(0.125, 0.125));
	GetGrid().SetZoomFactor(GRID_100PPI);
	GetGrid().GetAttributes().CopyFrom(GETAPP.GetGridAttributes());
	SendMessageToRulers(RW_INIT, LPARAM(GetPtrToRulerInfo()));
	SendMessageToRulers(RW_SHOW, TRUE);
	PostMessageToRulers(RW_ZOOM);
	PostMessageToRulers(RW_POSITION);
	
	//----------------------------------------
	// Initialize Mouse Tracking Event
	//----------------------------------------
	m_TrackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
	m_TrackMouseEvent.dwFlags = TME_LEAVE;
	m_TrackMouseEvent.dwHoverTime = INFINITE;
	m_TrackMouseEvent.hwndTrack = m_hWnd;

	CChildViewBase::OnInitialUpdate();
}

BOOL CFrontCadView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//-----------------------------------------------------
// Methods for placing objects on drawings
//-----------------------------------------------------

void CFrontCadView::OnDrawArcCenter()
{
	CCadArcCent* pArc;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	pArc = new CCadArcCent;
	pArc->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pArc);
	SetDrawMode(DrawingMode::ARC_CENTER);
	GETAPP.UpdateStatusBar(_T("Arc:Place Center Point"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawArccnter(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawArc()
{
	CCadArc* pArc;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::ARC);
	pArc = new CCadArc;
	pArc->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pArc);
	GETAPP.UpdateStatusBar(_T("Arc:Place 1st Point of Defining Rectangle"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawArc(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawArrow()
{
	CCadArrow* pArrow;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::ARROW);
	pArrow = new CCadArrow;
	pArrow->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pArrow);
	GETAPP.UpdateStatusBar(_T("Arrow:Place Location of Tip"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}

void CFrontCadView::OnUpdateDrawArrow(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawEllipse()
{
	CCadElispe* pE;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::ELIPSE);
	pE = new CCadElispe;
	pE->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pE);
	GETAPP.UpdateStatusBar(_T("Ellispe Place 1st Point of Defining Rectangle"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawEllipse(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawLine()
{
	CCadLine* pL;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::LINE);
	pL = new CCadLine;
	pL->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pL);
	GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnLineLinefixedlen()
{
	CCadLine* pL;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::LINE);
	pL = new CCadLine;
	pL->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pL);
	GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING_LINE_FIXED_LEN);
	if (m_DrawState == ObjectDrawState::END_DRAWING)
	{
		m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(m_DrawState);		delete GetObjectTypes().pCadObject;
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
}


void CFrontCadView::OnUpdateLineLinefixedlen(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnUpdateDrawLine(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawOrigin()
{
	CCadOrigin* pO;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::ORIGIN);
	pO = new CCadOrigin;
	pO->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pO);
	GETAPP.UpdateStatusBar(_T("Origin:Set Origin Name"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawOrigin(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawPoint()
{
	CCadPoint *pP;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::POINT);
	pP = new CCadPoint;
	pP->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pP);
	GETAPP.UpdateStatusBar(_T("Point:PlacePoint"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawPoint(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawPlaceBitmap()
{
	CCadBitmap* pBM;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::BITMAP);
	pBM = new CCadBitmap;
	pBM->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pBM);
	GETAPP.UpdateStatusBar(_T("Bitmap:Place 1st Point of Defining Rectangle"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}

void CFrontCadView::OnUpdateDrawPlacebitmap(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawPolygon()
{
	CCadPolygon* pPG;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::POLYGON);
	pPG = new CCadPolygon;
	pPG->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pPG);
	GETAPP.UpdateStatusBar(_T("Polygon:Place 1st Point"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawPolygohn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnLinePolyline()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateLinePolyline(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnDrawRectangle()
{
	CCadRect* pR;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::RECT);
	pR = new CCadRect;
	pR->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pR);
	GETAPP.UpdateStatusBar(_T("Rectangle:Place 1st Point"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawRectangle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawRectangularhole()
{
	CCadHoleRect* pRH;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::HOLE_RECT);
	pRH = new CCadHoleRect;
	pRH->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pRH);
	GETAPP.UpdateStatusBar(_T("Rectangular Hole:Place Location of Center"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawRectangularhole(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawRotatedrectangle()
{
}


void CFrontCadView::OnUpdateDrawRotatedrectangle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawRoundedrectangle()
{
	CCadRndRect* pRR;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::RNDRECT);
	pRR = new CCadRndRect;
	pRR->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pRR);
	GETAPP.UpdateStatusBar(_T("Rounded Rectangle:Place 1st Point of Defining Rectangle"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}

void CFrontCadView::OnUpdateDrawRoundedrectangle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawRoundhole()
{
	CCadHoleRound *pHole;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::HOLE_ROUND);
	GETAPP.UpdateStatusBar(_T("Round Hole:Place Location of Hole Center"));
	pHole = new CCadHoleRound;
	pHole->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pHole);
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawRoundhole(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawRoundholewith1flat()
{
	CCadHoleRnd1Flat* pHR1F;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::HOLE_RND1F);
	pHR1F = new CCadHoleRnd1Flat;
	pHR1F->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pHR1F);
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
	GETAPP.UpdateStatusBar(_T("Round Hole With one Flat:Place location of Center"));
}

void CFrontCadView::OnUpdateDrawRoundholewith1flat(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawRoundholewith2flats()
{
	CCadHoleRnd2Flat *pHole;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::HOLE_RND2F);
	pHole = new CCadHoleRnd2Flat;
	pHole->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pHole);
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
	GETAPP.UpdateStatusBar(_T("Round Hole With two Flats:Place Location of Center"));
}


void CFrontCadView::OnUpdateDrawRoundholewith2flats(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawText()
{
	CCadText* pText;

	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::TEXT);
	pText = new CCadText;
	pText->Create(NULL, GetDocument()->GetCurrentOrigin());
	SetObjectTypes(pText);
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
	GETAPP.UpdateStatusBar(_T("Text:Place location of Text"));
}


void CFrontCadView::OnUpdateDrawText(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

//--------------------------------------------------
// Commands to Edit the drawing
//--------------------------------------------------

void CFrontCadView::OnEditClear()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnEditClearAll()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnEditCopy()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnEditCut()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnEditPaste()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

//--------------------------------------------------
// Commands to modify the drawing
//--------------------------------------------------

void CFrontCadView::OnModifyLine()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateModifyLine(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnModifyMovepoint()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateModifyMovepoint(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnModifyRec()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateModifyRec(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

//--------------------------------------------------------
// Snap commands for drawing
//--------------------------------------------------------

void CFrontCadView::OnSnapSnapgridon()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateSnapSnapgridon(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnSnapSnapgridproperties()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateSnapSnapgridproperties(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnSnapSnaptoobject()
{
	CCadLine* pLine;

	m_ppObjList = new CCadObject * [16];

	pLine = new CCadLine;
	pLine->Create(NULL, NULL);
	m_ppObjList[0] = pLine;
	pLine = new CCadLine;
	pLine->Create(NULL, NULL);
	m_ppObjList[1] = pLine;
	pLine = new CCadLine;
	pLine->Create(NULL, NULL);
	m_ppObjList[2] = pLine;
	pLine = new CCadLine;
	pLine->Create(NULL, NULL);
	m_ppObjList[3] = pLine;
	CreateObjectSelectionMenu(
		m_ppObjList,
		4,
		CPoint(200, 200)
	);
}


void CFrontCadView::OnUpdateSnapSnaptoobject(CCmdUI* pCmdUI)
{
}


void CFrontCadView::OnSnapSnaptotangent()
{
}


void CFrontCadView::OnUpdateSnapSnaptotangent(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnSnapSnaporder()
{
	CDlgSnapOrder Dlg;

	Dlg.DoModal();
}


void CFrontCadView::OnUpdateSnapSnaporder(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

//--------------------------------------------------
// commands for changing the view
//--------------------------------------------------

void CFrontCadView::OnViewZoomin()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateViewZoomin(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnViewZoomout()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateViewZoomout(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	//----------------------------------------------------------
	// OnContextMenu
	//	This Method displays the context menus on the screen
	// when you click the right hand button on the mouse.
	// If certain keys are held down when clicking, you will
	// get a different context menu.
	//	NOKEY............default menu
	//  SHIFTKEY.........Place Menu
	//
	//  parameters:
	//      pWnd......??
	//      point.....point where mouse pointer is in screen coordinates
	//-----------------------------------------------------------
	CFrontCadDoc* pDoc = GetDocument();
	CMenu ConTexMenu;
	CMenu PlaceMenu;

	int id;
	//	CPoint m_MousePos = point;
	//	CCadPoint CurPos;

	//	ScreenToClient(&m_MousePos);
	//	CurPos = ConvertMousePosition(m_MousePos).Snap(GetSnapGrid(), IsSnapGridOn();
	CCadObject* pObj = pDoc->GetSelListHead();
	//-----------------------------------------
	// create place submenu
	//------------------------------------------
	PlaceMenu.CreatePopupMenu();
	PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_LINE, _T("Line"));
	PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_RECT, _T("Rect"));
	PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_ELLISPE, _T("Ellipse"));
	if (OBJECT_ENABLE_BITMAP & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_ARC, _T("Arc"));
	if (OBJECT_ENABLE_BITMAP & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_ARC_CENTERED, _T("Arc Centered"));
	if (OBJECT_ENABLE_ROUNDEDRECT & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_ROUNDED_RECT, _T("Rounded Rect"));
	if (OBJECT_ENABLE_ARROW & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_ARROW, _T("Arrow"));
	if (OBJECT_ENABLE_POLYGON & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_POLY, _T("Polygon"));
	//	if (OBJECT_ENABLE_POLYGONFILLED &  m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_FILLEDPOLY, _T("Filled Polygon"));
	if (OBJECT_ENABLE_LIBCOMP & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_LIBCOMP, _T("Library Part"));
	if (OBJECT_ENABLE_HOLEROUND & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_HOLEROUND, _T("Round Hole"));
	if (OBJECT_ENABLE_HOLERECT & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_HOLERECT, _T("Rectangular Hole"));
	if (OBJECT_ENABLE_HOLERND1FLAT & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_HOLERND1FLAT, _T("Rnd Hole 1 Flat"));
	if (OBJECT_ENABLE_HOLERND2FLAT & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_HOLERND2FLAT, _T("Rnd Hole 2 Flat"));
	if (OBJECT_ENABLE_TEXT & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_TEXT, _T("Text"));
	if (OBJECT_ENABLE_BITMAP & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_BITMAP, _T("Bitmap"));
	if (OBJECT_ENABLE_ORIGIN & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_ORIGIN, _T("Reference Point"));
	if (OBJECT_ENABLE_DIMENSION & m_ObjectEnables)PlaceMenu.AppendMenuW(MF_STRING, ID_CM_PLACE_DIMENSION, _T("Dimension"));
	//-------------------------------------------
	// Create Main Menu
	//--------------------------------------------
	ConTexMenu.CreatePopupMenu();
	ConTexMenu.AppendMenuW(MF_STRING | MF_MENUBREAK | MF_POPUP, (UINT_PTR)PlaceMenu.Detach(), _T("Place"));

	if (GetDrawMode() != DrawingMode::SELECT)
	{
		ConTexMenu.AppendMenuW(MF_STRING, ID_CM_ENDDRAWMODE, _T("End Draw Mode"));
		ConTexMenu.EnableMenuItem(ID_CM_ENDDRAWMODE, MF_ENABLED);
		ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EDIT_PROPERIES, _T("Edit Properties"));
		ConTexMenu.EnableMenuItem(ID_CM_EDIT_PROPERIES, MF_ENABLED);
		ConTexMenu.AppendMenu(MF_SEPARATOR);
	}
	ConTexMenu.AppendMenu(MF_STRING, ID_CM_FORWARD, _T("Send Obj Forward"));
	ConTexMenu.AppendMenu(MF_STRING, ID_CM_BACKWARD, _T("Send Obj Back"));
	ConTexMenu.AppendMenu(MF_STRING, ID_CM_DELETE, _T("Delete"));
	ConTexMenu.AppendMenu(MF_SEPARATOR);
	ConTexMenu.AppendMenuW(MF_STRING, ID_CM_DESELECT, _T("DeSelect"));
	ConTexMenu.AppendMenuW(MF_STRING, ID_CM_DESELECT_ALL, _T("DeSelect ALL"));
	if (pObj)	//is anything slected?
	{
		ConTexMenu.EnableMenuItem(ID_CM_FORWARD, MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_BACKWARD, MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_DELETE, MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_DESELECT, MF_ENABLED);
		ConTexMenu.EnableMenuItem(ID_CM_DESELECT_ALL, MF_ENABLED);
		if (pObj->GetType() == ObjectType::BITMAP)
		{
			ConTexMenu.AppendMenu(MF_STRING, ID_CM_RESTOREASPECTRATIO, _T("Restore Aspect Ratio"));
			ConTexMenu.EnableMenuItem(ID_CM_RESTOREASPECTRATIO, MF_ENABLED);
		}
	}
	else
	{
		ConTexMenu.EnableMenuItem(ID_CM_FORWARD, MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_BACKWARD, MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_DELETE, MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_DESELECT, MF_GRAYED);
		ConTexMenu.EnableMenuItem(ID_CM_DESELECT_ALL, MF_GRAYED);
	}

	id = ConTexMenu.TrackPopupMenu(
		TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
		point.x, 
		point.y, 
		this
	);
	ConTexMenu.DestroyMenu();
	switch (id)
	{
	case ID_CM_ENDDRAWMODE:
		if (GetObjectTypes().pCadObject)
		{
			GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::END_DRAWING);
			GetObjectTypes().pCadObject->Print(0);
			delete GetObjectTypes().pCadObject;
			SetObjectTypes(0);
		}
		SetDrawMode(DrawingMode::SELECT);
		SetDrawState(ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN);
		EnableAutoScroll(0);
		break;
	case ID_CM_EDIT_PROPERIES:
		if (GetObjectTypes().pCadObject)
			GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::SET_ATTRIBUTES);
		break;
	case ID_CM_FORWARD:		//make object draw last
	{
		CCadObject* pO = pDoc->GetSelListHead();
		while (pO)
		{
			pDoc->RemoveObject(pO);
			pDoc->AddObjectAtTail(pO);
			pO = pO->GetNextSel();
		}
		Invalidate();
	}
	break;
	case ID_CM_BACKWARD:	//make object draw first
	{
		CCadObject* pO = pDoc->GetSelListHead();
		while (pO)
		{
			pDoc->RemoveObject(pO);
			pDoc->AddObjectAtTail(pO);
			pO = pO->GetNextSel();
		}
		Invalidate();
	}
		break;
	case ID_CM_DELETE:	//delete selected object(s)
//		printf("Delete Object\n");
		{
			CCadObject* pO = pDoc->GetSelListHead();
			while (pO)
			{
				pDoc->RemoveSelectedObject(pO);
				pDoc->RemoveObject(pO);
				delete pO;
				pO = pDoc->GetSelListHead();
			}
			Invalidate();
		}
		break;
	case ID_CM_RESTOREASPECTRATIO:
//		printf("Restore Aspect Ratio\n");
		break;
	case ID_CM_DESELECT:
//		printf("DeSelect\n");
		{
			CCadObject* pO = pDoc->GetSelListHead();
			pDoc->RemoveSelectedObject(pO);
			pO->SetSelected(0);
			Invalidate();
		}
		break;
	case ID_CM_DESELECT_ALL:
//		printf("Deselect All\n");
		{
			CCadObject* pO = pDoc->GetSelListHead();
			while (pO)
			{
				pDoc->RemoveSelectedObject(pO);
				pO->SetSelected(0);
				pO = pDoc->GetSelListHead();
			}
			Invalidate();
		}
		break;
		//------------------------------------
		// PLACE menu response
		//-------------------------------------
	case ID_CM_PLACE_ARC:	// OnContextMenu
		OnDrawArc();
		break;
	case ID_CM_PLACE_ARC_CENTERED:	// OnContextMenu
		OnDrawArcCenter();
		break;
	case ID_CM_PLACE_ARROW:	// OnContextMenu
		OnDrawArrow();
		break;
	case ID_CM_PLACE_BITMAP:	// OnContextMenu
		OnDrawPlaceBitmap();
		break;
	case ID_CM_PLACE_DIMENSION:	// OnContextMenu
//		OnDrawPlaceDimension();
		break;
	case ID_CM_PLACE_ELLISPE:	// OnContextMenu
		OnDrawEllipse();
		break;
	case ID_CM_PLACE_HOLERECT:// OnContextMenu
		OnDrawRectangularhole();
	case ID_CM_PLACE_HOLEROUND:	// OnContextMenu
		OnDrawRoundhole();
		break;
	case ID_CM_PLACE_HOLERND1FLAT:	// OnContextMenu
		OnDrawRoundholewith1flat();
		break;
	case ID_CM_PLACE_HOLERND2FLAT:	// OnContextMenu
		OnDrawRoundholewith2flats();
		break;
	case ID_CM_PLACE_LIBCOMP:	// OnContextMenu
//		OnDrawLibraryComp();
		break;
	case ID_CM_PLACE_LINE:	// OnContextMenu
		OnDrawLine();
		break;
	case ID_CM_PLACE_ORIGIN:	// OnContextMenu
//		OnDrawReferencePoint();
		break;
	case ID_CM_PLACE_POLY:	// OnContextMenu
		OnDrawPolygon();
		break;
	case ID_CM_PLACE_RECT:	// OnContextMenu
		OnDrawRectangle();
		break;
	case ID_CM_PLACE_ROUNDED_RECT:	// OnContextMenu
		OnDrawRoundedrectangle();
		break;
	case ID_CM_PLACE_TEXT:	// OnContextMenu
		OnDrawText();
		break;
	}
}

void CFrontCadView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//----------------------------------------------------------------
	// OnKeyDown
	//
	//      This is the Method that gets called to handle a KeyDown
	// windows message.
	//
	// parameters:
	//  nChar.......Keyboard character being pushed.
	//  nRepCnt.....Repeat count
	//  nFlags......Various bit flags
	//----------------------------------------------------------------
	switch (nChar)
	{
	case VK_CONTROL:
		if (!m_ControlKeyDown)
		{
			printf("Control Down\n");
			m_ControlKeyDown = 1;
			switch (m_DrawMode)
			{
			case DrawingMode::SELECT:
				switch (m_DrawState)
				{
				case ObjectDrawState::SELECT_MOVE:
					break;
				case ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN:
					break;
				case ObjectDrawState::SELECT_COPY:
					break;

				}	//end of switch(m_DrawState
				break;
			}	//end of switch(m_DrawMode)

		}//end of if()
		break;
	case VK_SHIFT:
		m_ShiftKeyDown = 1;
		switch (m_DrawMode)
		{
		case DrawingMode::SELECT:
			switch (m_DrawState)
			{
			case ObjectDrawState::SELECT_MOVE:
				break;
			case ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN:
				break;
			case ObjectDrawState::SELECT_COPY:
				break;
			}	//end of switch(m_DrawState
			break;
		case DrawingMode::ALIGN_DIM_TO_HOLE:
		case DrawingMode::ARC:
		case DrawingMode::ARC_CENTER:
		case DrawingMode::ARROW:
		case DrawingMode::BITMAP:
		case DrawingMode::DIMENSION:
		case DrawingMode::ELIPSE:
		case DrawingMode::HOLE_RECT:
		case DrawingMode::HOLE_RND1F:
		case DrawingMode::HOLE_RND2F:
		case DrawingMode::HOLE_ROUND:
		case DrawingMode::LIBPART:
		case DrawingMode::LINE:
		case DrawingMode::ORIGIN:
		case DrawingMode::POINT:
		case DrawingMode::POLYGON:
		case DrawingMode::RECT:
		case DrawingMode::RNDRECT:
		case DrawingMode::TEXT:
			GetGrid().EnableSnap(FALSE);
			break;
		}	//end of switch(m_DrawMode)
		break;

	case 'R':
		switch (m_DrawMode)
		{
		case DrawingMode::RECT:
			m_R_KeyDown = TRUE;
			break;
		}
		break;
	}
	CChildViewBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFrontCadView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//----------------------------------------------------------------
	// OnKeyUp
	//
	//      This is the Method that gets called to handle a KEYUP
	// windows message.
	//
	// parameters:
	//  nChar.......Keyboard character being pushed.
	//  nRepCnt.....Repeat count
	//  nFlags......Various bit flags
	//----------------------------------------------------------------
	CCadObject** ppSelList;
	int n;

	CFrontCadDoc* pDoc = GetDocument();
	switch (nChar)
	{
	case 'R':
		switch (m_DrawMode)
		{
		case DrawingMode::RECT:
			m_R_KeyDown = FALSE;
			break;
		}
		break;
	case VK_ESCAPE:
		//			if (IsScrolling())
	{
		EnableAutoScroll(0);
		//				CChildViewBase::PostMessage(WM_STOPAUTOSCROLL);
	}
	if (
		ObjectDrawState::PLACE_LBUTTON_UP == m_DrawState ||
		ObjectDrawState::PLACE_LBUTTON_DOWN == m_DrawState ||
		ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP == m_DrawState || 
		ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN == m_DrawState ||
		ObjectDrawState::PLACE_AUTO == m_DrawState ||
		ObjectDrawState::ARCSTART_LBUTTON_DOWN == m_DrawState ||
		ObjectDrawState::ARCSTART_LBUTTON_UP == m_DrawState ||
		ObjectDrawState::EXTENSION_LINES_LBUTTON_DOWN == m_DrawState ||
		ObjectDrawState::EXTENSION_LINES_LBUTTON_UP == m_DrawState ||
		ObjectDrawState::ARCEND_LBUTTON_DOWN == m_DrawState ||
		ObjectDrawState::ARCEND_LBUTTON_UP == m_DrawState
	)
	{
		//--------------------------------------------
		// End Draw Mode
		//--------------------------------------------
		switch (m_DrawMode)
		{
		case DrawingMode::ARC:
		case DrawingMode::ARC_CENTER:
		case DrawingMode::ARROW:
		case DrawingMode::ELIPSE:
		case DrawingMode::LINE:
		case DrawingMode::POLYGON:
		case DrawingMode::RECT:
		case DrawingMode::RNDRECT:
		case DrawingMode::BITMAP:
		case DrawingMode::TEXT:
			m_DrawState = ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN;
			if (m_CadObj.pCadObject)
			{
				m_CadObj.pCadObject->ProcessDrawMode(m_DrawState);
				delete m_CadObj.pCadObject;
				m_CadObj.pCadObject = 0;
			}
			break;
		}
	}
	break;
	case 'X':
		ppSelList = new CCadObject * [DEFALT_SELECT_BUFFER_SIZE];
		n = pDoc->PointInObjectAndSelect(
			GetCurrentMousePosition(),
			NULL,
			ppSelList,
			DEFALT_SELECT_BUFFER_SIZE,
			OBJKIND_ALL
		);
		if (n > 0)
		{
			CCadPoint *pCenter;
			pCenter = (CCadPoint * )ppSelList[0]->FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
			ppSelList[0]->Flip(pCenter, 0);
		}
		delete[]ppSelList;
		Invalidate();
		break;
	case 'Y':
		ppSelList = new CCadObject * [DEFALT_SELECT_BUFFER_SIZE];
		n = pDoc->PointInObjectAndSelect(
			GetCurrentMousePosition(),
			NULL,
			ppSelList,
			DEFALT_SELECT_BUFFER_SIZE,
			OBJKIND_ALL
		);
		if (n > 0)
		{
			CCadPoint *pCenter;
			pCenter = (CCadPoint * )ppSelList[0]->FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
			ppSelList[0]->Flip(pCenter, 1);
		}
		delete[]ppSelList;
		Invalidate();
		break;
	case VK_CONTROL:
		printf("CONTROL UP\n");
		m_ControlKeyDown = 0;
		switch (m_DrawMode)
		{
		case DrawingMode::SELECT:
			switch (m_DrawState)
			{
			case ObjectDrawState::SELECT_MOVE:
				break;
			case ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN:
				break;
			case ObjectDrawState::SELECT_COPY:
				break;
			}	//end of switch(m_DrawState)
			break;
		}	//end of switch(m_DrawMode)
		break;
	case VK_SHIFT:
		m_ShiftKeyDown = 0;
		switch (m_DrawMode)
		{
		case DrawingMode::ALIGN_DIM_TO_HOLE:
		case DrawingMode::ARC:
		case DrawingMode::ARC_CENTER:
		case DrawingMode::ARROW:
		case DrawingMode::BITMAP:
		case DrawingMode::DIMENSION:
		case DrawingMode::ELIPSE:
		case DrawingMode::HOLE_RECT:
		case DrawingMode::HOLE_RND1F:
		case DrawingMode::HOLE_RND2F:
		case DrawingMode::HOLE_ROUND:
		case DrawingMode::LIBPART:
		case DrawingMode::LINE:
		case DrawingMode::ORIGIN:
		case DrawingMode::POINT:
		case DrawingMode::POLYGON:
		case DrawingMode::RECT:
		case DrawingMode::RNDRECT:
		case DrawingMode::TEXT:
			GetGrid().EnableSnap(TRUE);
			break;
		case DrawingMode::SELECT:
			switch (m_DrawState)
			{
			case ObjectDrawState::SELECT_MOVE:
				break;
			case ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN:
				break;
			case ObjectDrawState::SELECT_COPY:
				break;
			}	//end of switch(m_DrawState
			break;
		}	//end of switch(m_DrawMode)
		break;
	}
	CChildViewBase::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CFrontCadView::OnSize(UINT nType, int cx, int cy)
{
	//----------------------------------------------
	// OnSize
	//		This method is called in response to
	// a WM_SIZE message
	//----------------------------------------------
	switch (nType)
	{
	case SIZE_MAXIMIZED:
		break;
	case SIZE_MINIMIZED:
		break;
	case SIZE_RESTORED:
//		printf("Set Window Size To (%d,%d)\n", cx, cy);
		UpdateScrollbarInfo(GetRulerInfo().GetUpperLeft());
		GetRulerInfo().SetClientSize(CSize(cx, cy));
		if (GetRulerInfo().AreFulersReady())
			PostMessageToRulers(RW_ZOOM);
		Invalidate();
		break;
	case SIZE_MAXHIDE:
		break;
	case SIZE_MAXSHOW:
		break;
	}
	CChildViewBase::OnSize(nType, cx, cy);
}

void CFrontCadView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	///------------------------------------------------
	/// OnVScroll
	///		This is the Method that is called when
	/// the user presses the VerticalScroll bar in
	/// the view.
	///
	/// parameters:
	///		nSBCode.....part of the scrollbar clicked
	///		nPos........Position of the scrollbar
	///		pScrollBar..pointer to the scrollbar object
	///------------------------------------------------
	CFrontCadDoc* pDoc = GetDocument();
	double cY = pDoc->GetDocSize().dCY;
	int Delta = 0;
	BOOL Update = TRUE;
	BOOL Absolute = FALSE;
	double Yinches = 0.0;

	switch (nSBCode)
	{
	case SB_LINEUP:
		Yinches = -GetGrid().GetSnapGrid().dCY;
		break;
	case SB_PAGEUP:
		Yinches = -GetGrid().GetMajorGrid().dCY;
		break;
	case SB_THUMBTRACK:
		Absolute = TRUE;
		Yinches = GetVScrollPos(nPos)* pDoc->GetDocHieght();
		Yinches = GETAPP.Snap(Yinches, GetGrid().GetSnapGrid().dCY);
		break;
	case SB_LINEDOWN:
		Yinches = GetGrid().GetSnapGrid().dCY;
		break;
	case SB_PAGEDOWN:
		Yinches = GetGrid().GetMajorGrid().dCY;
		break;
	case SB_ENDSCROLL:
		Update = FALSE;
		break;
	case SB_THUMBPOSITION:
		Absolute = TRUE;
		Update = FALSE;
		break;
	}
	DoVScroll(Yinches, Absolute, Update);
	CChildViewBase::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFrontCadView::DoVScroll(double Yinches, BOOL Absolute, BOOL Update)
{
	CFrontCadDoc* pDoc = GetDocument();
	DOUBLEPOINT ULHC; //upper left hand corner

	if (Update)
	{
		if (Absolute)
		{
			ULHC = GetRulerInfo().GetUpperLeft();
			ULHC.dY = Yinches;
		}
		else
			ULHC = GetRulerInfo().GetUpperLeft() + CDoubleSize(0.0, Yinches);

		if (ULHC.dY < 0.0)
			ULHC.dY = 0.0;
		else if (ULHC.dY > (pDoc->GetDocSize().dCY - GetClientHieght()))
		{
			ULHC.dY = pDoc->GetDocSize().dCY - GetClientHieght();
			if (ULHC.dY < 0.0)
				ULHC.dY = 0.0;
		}
		GetRulerInfo().SetUpperLeft(ULHC);
		double pos = ULHC.dY;
		pos = pos * GetGrid().GetPixelsPerInch().GetScaleY();
		SetScrollPos(SB_VERT, GETAPP.RoundDoubleToInt(pos), TRUE);
		PostMessageToRulers(RW_VSCROLL);
		PostMessageToRulers(RW_POSITION);
		Invalidate();
	}

}

void CFrontCadView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	///------------------------------------------------
	/// OnHScroll
	///		This is the Method that is called when
	/// the user presses the horizonalscroll bar in
	/// the view.
	///
	/// parameters:
	///		nSBCode.....part of the scrollbar clicked
	///		nPos........Position of the scrollbar
	///		pScrollBar..pointer to the scrollbar object
	///------------------------------------------------
	CFrontCadDoc* pDoc = GetDocument();
	double dCX = pDoc->GetDocSize().dCX;
	int Delta = 0;
	BOOL Update = TRUE;
	BOOL Absolute = FALSE;
	double Xinches = 0.0;

	switch (nSBCode)
	{
	case SB_LINELEFT:
		Xinches = -GetGrid().GetSnapGrid().dCX;
		break;
	case SB_PAGELEFT:
		Xinches = -GetGrid().GetMajorGrid().dCX;
		break;
	case SB_THUMBTRACK:
		Absolute = TRUE;
		Xinches = GetHScrollPos(nPos) * pDoc->GetDocWidth();
		Xinches = GETAPP.Snap(Xinches, GetGrid().GetSnapGrid().dCX);
		break;
	case SB_THUMBPOSITION:
		Absolute = TRUE;
		Update = FALSE;
		break;
	case SB_LINERIGHT:
		Xinches = GetGrid().GetSnapGrid().dCX;
		break;
	case SB_PAGERIGHT:
		Xinches = GetGrid().GetMajorGrid().dCX;
		break;
	case SB_ENDSCROLL:
		Update = FALSE;
		break;
	}
//	printf("nPos = %d  Xinches = %8.3lf  Update=%d\n", nPos, Xinches,Update);
	DoHScroll(Xinches, Update);
	CChildViewBase::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFrontCadView::DoHScroll(double Xinches, BOOL Absolute, BOOL Update)
{
	DOUBLEPOINT ULHC;
	CFrontCadDoc* pDoc = GetDocument();

	if (Update)
	{
		if (Absolute)
		{
			ULHC = GetRulerInfo().GetUpperLeft();
			ULHC.dX = Xinches;
		}
		else
			ULHC = GetRulerInfo().GetUpperLeft() + CDoubleSize(Xinches, 0.0);
		if (ULHC.dX < 0.0)
			ULHC.dX = 0.0;
		else if (ULHC.dX > (pDoc->GetDocSize().dCX - GetClientWidth()))
		{
			ULHC.dX = pDoc->GetDocSize().dCX - GetClientWidth();
			if (ULHC.dX < 0.0)
				ULHC.dX = 0;
		}
		GetRulerInfo().SetUpperLeft(ULHC);

		double pos = ULHC.dX;
		pos = pos * GetGrid().GetPixelsPerInch().GetScaleX();
		SetScrollPos(SB_HORZ, GETAPP.RoundDoubleToInt(pos), TRUE);
		//----------------------------------------------------------
		// Update Rulers
		//---------------------------------------------------------
		PostMessageToRulers(RW_HSCROLL);
		PostMessageToRulers(RW_POSITION);
		Invalidate();
	}
}

void CFrontCadView::UpdateScrollbarInfo(DOUBLEPOINT ULHC)
{
	//---------------------------------------------------------------------
	//  UpdateScrollbarInfo
	//
	//      This Method updates the positions of the scrollbars (really?)
	// 
	// Parameters:
	// ULHC........Upper Left hand corner (in inches)
	// 
	// Returns Nothing
	// Update Upper Left Hand Corner
	//---------------------------------------------------------------------
	CFrontCadDoc* pDoc = GetDocument();
	double HScrollMax = 0.0;
	CDoubleSize DocSize = pDoc->GetDocSize();
	double ScrollArea;
	double ScrollPos;

	//------------------------------------------------
	// Calculations for Horizontal Scroll Bar
	//------------------------------------------------
	// Document Size is in inches
	// cx is in pixels (width of the work space)
	//------------------------------------------------
	ScrollArea = DocSize.dCX - GetClientWidth();
	ScrollPos = ULHC.dX / ScrollArea;
	if (GetClientWidth() < GetDocSize().dCX)
	{
		if (ScrollArea < 0)ScrollArea = 0.0;
		SetScrollPos(SB_HORZ, GETAPP.RoundDoubleToInt(min(ScrollPos, ScrollArea) * GetGrid().GetPixelsPerInch().GetScaleX()), TRUE);
	}
	m_HScrollInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	m_HScrollInfo.nMin = 0;
	m_HScrollInfo.nMax = GETAPP.RoundDoubleToInt(ScrollArea * GetGrid().GetPixelsPerInch().GetScaleX() - 1.0);
	m_HScrollInfo.nPos = GETAPP.RoundDoubleToInt(min(ScrollPos, ScrollArea) * GetGrid().GetPixelsPerInch().GetScaleX());
	m_HScrollInfo.nPage = GETAPP.RoundDoubleToInt(GetClientWidth() * GetGrid().GetPixelsPerInch().GetScaleX() );
	SetScrollInfo(SB_HORZ, &m_HScrollInfo, TRUE);
	//------------------------------------------------
	// Vertical Scrolling 
	//------------------------------------------------
	ScrollArea = DocSize.dCY - GetClientHieght();
	ScrollPos = ULHC.dY / ScrollArea;
	if (GetClientHieght() < GetDocSize().dCY)
	{
		if (ScrollArea < 0)ScrollArea = 0.0;
		SetScrollPos(SB_VERT, GETAPP.RoundDoubleToInt(min(ScrollPos, ScrollArea) * GetGrid().GetPixelsPerInch().GetScaleY()), TRUE);
	}
	m_VScrollInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	m_VScrollInfo.nMin = 0;
	m_VScrollInfo.nMax = GETAPP.RoundDoubleToInt(ScrollArea * GetGrid().GetPixelsPerInch().GetScaleY() - 1.0);
	m_VScrollInfo.nPos = GETAPP.RoundDoubleToInt(min(ScrollPos, ScrollArea) * GetGrid().GetPixelsPerInch().GetScaleY());
	m_VScrollInfo.nPage = GETAPP.RoundDoubleToInt(GetClientHieght() * GetGrid().GetPixelsPerInch().GetScaleY());

	SetScrollInfo(SB_VERT, &m_VScrollInfo, TRUE);
}


void CFrontCadView::GetDeviceScrollSizes(CSize& sizePage, CSize& sizeLine)
{
	//--------------------------------------
	// Get mapping mode
	//--------------------------------------
	SCROLLINFO Vert, Horz;
	GetScrollInfo(SB_VERT, &Vert);
	GetScrollInfo(SB_HORZ, &Horz);
	sizePage.cx = Horz.nPage;
	sizePage.cy = Vert.nPage;
	sizeLine = CSize(10, 10);
}


BOOL CFrontCadView::SendMessageToRulers(UINT msg, LPARAM data)
{
	BOOL rV = FALSE;
	CRulerSplitterWnd* pRSW;

	pRSW = GetRulerSplitterWnd();
	rV = pRSW->SendMessageW(UINT(WindowsMsg::WM_RULER_MESSAGE), msg, data);
	//	rV =  pRSW->OnRulerMessage(msg, data);
	return rV;
}
     
BOOL CFrontCadView::PostMessageToRulers(UINT msg, LPARAM data)
{
	BOOL rV = FALSE;
	CRulerSplitterWnd* pRSW;

	pRSW = GetRulerSplitterWnd();
	pRSW->PostMessageW(UINT(WindowsMsg::WM_RULER_MESSAGE), msg, data);
	//	rV =  pRSW->OnRulerMessage(msg, data);
	return rV;
}

void CFrontCadView::DrawCursor(
	CDC* pDC, 
	DOUBLEPOINT pos,// Cursor Position
	CRect* pRect,	// Client Rectangle
	DOUBLEPOINT& ULHC,
	CScale &Scale, 
	COLORREF color
)
{
	//-------------------------------------------
	// DrawCursor
	//
	//		This methode gets called to draw the
	// drawing cursor on the view.  This is different
	// from the mouse cursor as it will snap to 
	// snap grid.  Right now it is just a cross
	// hair.
	//	parameters:
	//		pDC......pointer to the device context
	//		pos......point where cursor is centered
	//		pRect....Rectangle that defines view
	//		Offset...Scroll Offset
	//		Scale....View Scale factor
	//		color....Color to Draw Cursor
	//-------------------------------------------
	CPen pen, * pOldPen;
	int w, h;
	CPoint p;

	pen.CreatePen(PS_SOLID, 1, color);
	pOldPen = pDC->SelectObject(&pen);

	p = pos.ToPixelPoint(ULHC, Scale.GetScaleX(), Scale.GetScaleY());
	w = pRect->Width() - 1;
	h = pRect->Height() - 1;
	pDC->MoveTo(0, p.y);
	pDC->LineTo(w, p.y);
	pDC->MoveTo(p.x, 0);
	pDC->LineTo(p.x, h);
	pDC->SelectObject(pOldPen);
}

// Zoom in around a point

DOUBLEPOINT CFrontCadView::CalculateNewULHC(
	CScale CurrentScale, 
	CScale NextScale, 
	DOUBLEPOINT pointLocation,
	DOUBLEPOINT pointULHC
)
{
	//--------------------------------------
	//	CalculateNewULHC
	// Calculate a New Upper Left Hand 
	// corner coordinate.  So far, primarily
	// used when zooming in and out
	// 
	// Parameters:
	//	CurrentScale.....Current scale factor
	//	NextScale........New Scale factor
	//	pointResult......Store result here
	//	pointLocation....Point to zoom around
	//	pointULHC........Current Upper Left
	//					Hand Corner
	// 
	// Returns:
	//	Reference to the new U.L.H.C
	//--------------------------------------
	double Ux=0.0, Uy=0.0;
	double Temp;
	DOUBLEPOINT pointResult;

	Temp = CurrentScale.GetScaleX() / NextScale.GetScaleX();
	Temp *= (pointLocation.dX - pointULHC.dX);
	Ux = pointLocation.dX - Temp;
	Temp = CurrentScale.GetScaleY() / NextScale.GetScaleY();
	Temp *= (pointLocation.dY - pointULHC.dY);
	Uy = pointLocation.dY - Temp;
	pointResult = DOUBLEPOINT(Ux, Uy);
	return pointResult;
}

void CFrontCadView::ZoomIn(DOUBLEPOINT point)
{
	//---------------------------------------
	// ZoomIn
	//	This Method will zoom the vew about
	// the pointer location so that the object
	// of interest remains on the screen
	// where the zoom is taking place
	//
	// parameters:
	//	point......point in view to zoom around
	// returns:nothing
	//-----------------------------------------
	CScale CurrentScale, NextScale;
	DOUBLEPOINT pointNewULHC;
	DOUBLEPOINT ptCurrentULHC;
	CFrontCadDoc* pDoc;
	CPoint p;

	ptCurrentULHC = GetRulerInfo().GetUpperLeft();
	CurrentScale = GetGrid().GetPixelsPerInch();
	p = point.ToPixelPoint(ptCurrentULHC, CurrentScale.GetScaleX(), CurrentScale.GetScaleY());
	if (GetGrid().ZoomIn())	// did a zoom?
	{
		NextScale = GetGrid().GetInchesPerPixel();
		pointNewULHC = point.ULHCfromPixelPoint(p, NextScale.GetScaleX(), NextScale.GetScaleY());
		//---------------------------------
		// Clean up ULHC ie Snap
		//--------------------------------
		pointNewULHC.Snap(GetGrid().GetSnapGrid(), TRUE);
		pointNewULHC.Print("Zoom In New ULHC");
		GetRulerInfo().SetUpperLeft(pointNewULHC);
		//--------------------------------
		// Update the Scroll Position
		//--------------------------------
		UpdateScrollbarInfo(pointNewULHC);
		pDoc = GetDocument();
		pDoc->ProcessZoom(GetGrid().GetInchesPerPixel());
		Invalidate();
	}
}


// //zoom out around a point
void CFrontCadView::ZoomOut(DOUBLEPOINT point)
{
	//---------------------------------------
	// ZoomOut
	//	This Method will zoom the vew about
	// the pointer location so that the object
	// of interest remains on the screen
	// where the zoom is taking place
	//
	// parameters:
	//	point......point in view to zoom around
	// returns:nothing
	//-----------------------------------------
	CScale CurrentScale, NextScale;
	DOUBLEPOINT pointNewULHC;
	DOUBLEPOINT ptULHC = GetRulerInfo().GetUpperLeft();
	CFrontCadDoc* pDoc = GetDocument();
	CPoint p;

	CurrentScale = GetGrid().GetPixelsPerInch();
	p = point.ToPixelPoint(ptULHC, CurrentScale.GetScaleX(), CurrentScale.GetScaleY());
	if (GetGrid().ZoomOut())	// did a zoom
	{
		NextScale = GetGrid().GetInchesPerPixel();
		pointNewULHC = point.ULHCfromPixelPoint(p, NextScale.GetScaleX(), NextScale.GetScaleY());
		pointNewULHC.Snap(GetGrid().GetSnapGrid(), TRUE);
		pointNewULHC.Print("Zoom Out New ULHC");
		GetRulerInfo().SetUpperLeft(pointNewULHC);
		//--------------------------------
		// Update the Scroll Position
		//--------------------------------
		pDoc->ProcessZoom(GetGrid().GetInchesPerPixel());
		UpdateScrollbarInfo(pointNewULHC);
		Invalidate();
	}
}

void CFrontCadView::EnableAutoScroll(BOOL flag)
{
	m_AutoScroll = flag;
	if (flag)
	{
		//------------------------------------------
		// Mouse Tracking On
		//------------------------------------------
		TrackMouseEvent(&m_TrackMouseEvent);
		//-----------------------------------------
		// Create a Timer
		//-----------------------------------------
		m_AutoScrollTimerId = SetTimer(
			m_AutoScrollTimerId, 
			TIMER_AUTOSCROLL_TIME, 
			NULL
		);
	}
	else
	{
		//------------------------------------------
		// Mouse Tracking Off
		//------------------------------------------
		KillTimer(m_AutoScrollTimerId);
	}
}
// //converts raw position

void CFrontCadView::OnMouseLeave()
{
	SetMouseLeftWindow(TRUE);
//	printf("Mouse Left the Building MB = %d\n", m_MiddleMouseButtonDown);
//	CChildViewBase::OnMouseLeave();
}

MouseIsHere CFrontCadView::WhereIsMouse()
{
	MouseIsHere rV = MouseIsHere::NOWHERE;
	CRect ClientScreneRect;
	CPoint MousePos;
	int L,R,T,B;
	int X, Y;

	GetCursorPosition(&MousePos);
	GetClientRect(&ClientScreneRect);
	ClientToScreen(&ClientScreneRect);
	L = ClientScreneRect.left;
	R = ClientScreneRect.right;
	T = ClientScreneRect.top;
	B = ClientScreneRect.bottom;
	X = MousePos.x;
	Y = MousePos.y;
	//-------------------------------
	// So, just where is that mouse
	//-------------------------------
	if (X < L && Y > T && Y < B)
		rV = MouseIsHere::LEFT;
	else if (X > R && Y > T && Y < B)
		rV = MouseIsHere::RIGHT;
	else if (X > L && X < R && Y < T)
		rV = MouseIsHere::UPPER;
	else if (X < L && Y > B)
		rV = MouseIsHere::LOWERLEFT;
	else if (X < L && Y < T)
		rV = MouseIsHere::UPPERLEFT;
	else if (X > R && Y < T)
		rV = MouseIsHere::UPPERRIGHT;
	else if (X > L && X<R && Y > B)
		rV = MouseIsHere::LOWER;
	else if (X > R && Y > B)
		rV = MouseIsHere::LOWERRIGHT;
	else
		rV = MouseIsHere::NOWHERE;
	return rV;
}

void CFrontCadView::StartMouseDragging()
{

}

void CFrontCadView::OnTimer(UINT_PTR nIDEvent)
{
	//-----------------------------------------
	// The Timer is used to perform the task
	// of Auto Scrolling the view,
	// Detecting when the mouse has been held
	// down for a bit,
	// 
	//-----------------------------------------
	MouseIsHere MouseLocation;

	switch (nIDEvent)
	{
	case TIMER_ID_AUTOSCROLL:
		if (IsAutoScrollEnabled())
		{
			MouseLocation = WhereIsMouse();
			switch (MouseLocation)
			{
			case MouseIsHere::UPPER:
				DoVScroll(-GetGrid().GetSnapGrid().dCY, FALSE, TRUE);
				break;
			case MouseIsHere::LOWER:
				DoVScroll(GetGrid().GetSnapGrid().dCY, FALSE, TRUE);
				break;
			case MouseIsHere::LEFT:
				DoHScroll(-GetGrid().GetSnapGrid().dCX, FALSE, TRUE);
				break;
			case MouseIsHere::RIGHT:
				DoHScroll(GetGrid().GetSnapGrid().dCX, FALSE, TRUE);
				break;
			case MouseIsHere::UPPERLEFT:
				break;
			case MouseIsHere::UPPERRIGHT:
				break;
			case MouseIsHere::LOWERLEFT:
				break;
			case MouseIsHere::LOWERRIGHT:
				break;
			case MouseIsHere::NOWHERE:
				break;
			}
		}
		break;
	case TIMER_ID_LBUTTON_DOWN:
		if (m_DrawState == ObjectDrawState::SELECT_WAITFOR_LBUTTON_DOWN_TIMEOUT)
		{
			m_MouseLButtonHeldDown = TRUE;
			m_DrawState = ObjectDrawState::SELECT_LBUTTON_DOWN_DRAGGING;
		}
		break;
	}
	CChildViewBase::OnTimer(nIDEvent);
}

void CFrontCadView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetMiddleMouseButtonDown(TRUE);
	EnableAutoScroll(TRUE);
	CChildViewBase::OnMButtonDown(nFlags, point);
}


void CFrontCadView::OnMButtonUp(UINT nFlags, CPoint point)
{
	SetMiddleMouseButtonDown(FALSE);
	EnableAutoScroll(FALSE);
	CChildViewBase::OnMButtonUp(nFlags, point);
}


//--------------------------------------------------------------
// Tool Bar Methods
//--------------------------------------------------------------

void CFrontCadView::AddOriginAtHead(CCadOrigin* pObj)
{
	CFrontCadDoc* pDoc = GetDocument();

	GetToolBarView()->AddOrigin(pObj);
	pDoc->AddOriginAtHead(pObj);
}

void CFrontCadView::AddOriginAtTail(CCadOrigin* pObj)
{
	CFrontCadDoc* pDoc = GetDocument();

	GetToolBarView()->AddOrigin(pObj);
	pDoc->AddOriginAtTail(pObj);
}

void CFrontCadView::RemoveOrigin(CCadOrigin* pObj)
{
}

afx_msg LRESULT CFrontCadView::OnFromToolbarMessage(WPARAM SubMessage, LPARAM Data)
{
	ToolBarMsg submsg = ToolBarMsg(SubMessage);
	CCadOrigin* pCORG = 0;
	CFrontCadDoc* pDoc = GetDocument();
	CString csName;
	
	switch (submsg)
	{
	case ToolBarMsg::ORIGIN_SEL_CHANGE:
		pCORG = (CCadOrigin *) GetOriginSelectCombo().GetItemData(Data);
		pDoc->SetCurrentOrigin(pCORG);
		GetRulerInfo().SetOrigin(pCORG);
		PostMessageToRulers(RW_ZOOM);
//		printf("Origin Changed\n");
		break;
	}
	return 0;
}

/*

void CFrontCadView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//-------------------------------------------------------
	// OnLButtonDown
	//
	//      This Method handles messages created when the
	// presses the left mouse button.
	//
	//  nFlags......various flag bits
	//  point.......mouse pointer coordinates for this window
	//------------------------------------------------------
	DOUBLEPOINT MousePos;

	m_LeftMouseButtonDown = TRUE;
	MousePos = ConvertMousePosition(
		point,
		GetRulerInfo().GetUpperLeft(),
		GetGrid().GetInchesPerPixel(),
		GetGrid().GetSnapGrid(),
		GetGrid().IsSnapOn()
	);
	switch (m_DrawMode)
	{
	case DrawingMode::SELECT:	// OnLButtonDown
		switch (m_DrawState)
		{
		case ObjectDrawState::SELECT_NOTHING:	//Mode:Select OnLButtonDown
			//------------------------------------
			// check to see if anything is selected
			// and are we are clicking in the body of
			// the object, or one of the points
			//-------------------------------------
			if (m_SelectionCount)	//are there selections?
			{
				CCadObject* pObj = GetSelListHead();
				m_GrabbedPoint = pObj->GrabPoint(NULL,GetCurrentMousePosition());
				if (m_GrabbedPoint >= 0)
				{
					m_DrawState = ObjectDrawState::POINT_GRABBED;
				}
			}
			break;
		case ObjectDrawState::SELECT_MOVE:		//Mode:Select OnLButtonDown
			break;
		case ObjectDrawState::SELECT_COPY:		//Mode:Select OnLButtonDown
			break;
		}
		Invalidate();
		break;
	case DrawingMode::MOVE:		// OnLButtonDown
		break;
	case DrawingMode::COPY:		// OnLButtonDown
		break;
	case DrawingMode::CUT:		// OnLButtonDown
		break;
	case DrawingMode::PASTE:	//OnLButtonDown
		switch (m_DrawState)
		{
		case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:	//Mode:PASTE OnLButtonDown
			m_pMoveObjects = new CMoveObjects;
			m_pMoveObjects->Copy(GETAPP.GetClipBoard());
			Invalidate();
			break;
		}	//End of Switch(DrawState)
		break;
	case DrawingMode::SELECTREGION:	// OnLButtonDown
		break;
	default:	// OnLButtonDownit must be something we are going to draw.
		if (m_CadObj.pCadObject)
		{
			m_DrawState = m_CadObj.pCadObject->ProcessDrawMode(m_DrawState);
			if (m_DrawState == ObjectDrawState::END_DRAWING)
			{
				m_DrawState = m_CadObj.pCadObject->ProcessDrawMode(m_DrawState);
				delete m_CadObj.pCadObject;
				m_CadObj.pCadObject = 0;
			}
		}
		break;
	}
	CChildViewBase::OnLButtonDown(nFlags, point);
}
*/



void CFrontCadView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	printf("Sys Key Down\n");
	CChildViewBase::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


void CFrontCadView::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	printf("Sys Key Up\n");
	CChildViewBase::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

LRESULT CFrontCadView::OnPuMenuHoverIndex(WPARAM index, LPARAM lparam)
{
	return 0;
}

LRESULT CFrontCadView::OnPuMenuSelectedIndex(WPARAM index, LPARAM lparam)
{
	return 0;
}

void CFrontCadView::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	int Index;
	CCadObject* pParent;

	CChildViewBase::OnMenuSelect(nItemID, nFlags, hSysMenu);

	if ((nItemID & POPUP_MENU_ITEM_IDS) == POPUP_MENU_ITEM_IDS)
	{
		Index = nItemID - POPUP_MENU_ITEM_IDS;
		if (nLastItemIndex >= 0)
		{
			m_ppObjList[nLastItemIndex]->SetSelected(FALSE);
			pParent = m_ppObjList[nLastItemIndex]->GetParent();
			while (pParent)
			{
				pParent->SetSelected(FALSE);
				pParent = pParent->GetParent();
			}
		}
		nLastItemIndex = Index;
		m_ppObjList[Index]->SetSelected(TRUE);
//		printf(">>> Set Selected %d  ID:%d Sel=%d\n", Index, m_ppObjList[Index]->GetId(), m_ppObjList[Index] ->IsSelected());
		pParent = m_ppObjList[Index]->GetParent();
		while (pParent)
		{
			pParent->SetSelected(TRUE);
			pParent = pParent->GetParent();
		}
//		m_ppObjList[Index]->Print(2);
	}
	else if ((nItemID == 0) && (nLastItemIndex >= 0) &&  m_ppObjList)
	{
		m_ppObjList[nLastItemIndex]->SetSelected(FALSE);
		pParent = m_ppObjList[nLastItemIndex]->GetParent();
		while (pParent)
		{
			pParent->SetSelected(FALSE);
			pParent = pParent->GetParent();
		}
		m_ppObjList = 0;
		nLastItemIndex = -1;
	}
	Invalidate();
}

UINT CFrontCadView::CreateObjectSelectionMenu(
	CCadObject **ppObjList, 
	UINT n, 
	CPoint pointLoc
)
{
	CMenu Menu;
	UINT i;
	UINT Id;

	m_ppObjList = ppObjList;
	m_NumOfMessListEntries = 0;
	Menu.CreatePopupMenu();
	for (i = 0; i < n; ++i)
	{
		Menu.AppendMenuW(MF_STRING, POPUP_MENU_ITEM_IDS + i, ppObjList[i]->GetTypeString());
		Menu.EnableMenuItem(POPUP_MENU_ITEM_IDS + i, MF_ENABLED);
	}
	Id = Menu.TrackPopupMenu(
		TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		pointLoc.x,
		pointLoc.y,
		this
	);
	m_ppObjList = 0;
	return Id;
}


CCadObject* CFrontCadView::SnapToObject(
	DOUBLEPOINT MousePos,
	UINT KindsToSnapTo,
	CCadObject* pExcludeObject,
	BOOL bChoose
)
{
	//-------------------------------------------
	// SnapToObject
	//	We check to see if the mouse cursor is
	// near an object that this particular object
	// wants to snap to.
	// 
	// Parameters:
	// MousePos........Current Cursor location
	// KindsToSnapTo...Kind of objects to snap to
	// pExcludeObject..Exclude this object and
	//                 All of its children from
	//                 Being Snapped to
	// bChoose.........???
	// 
	// return: NULL if nothing was found
	//         or a Pointer to the objeect that
	//	       was found
	//-------------------------------------------
	CCadObject* pResult = NULL;
	UINT Id;
	CCadObject* ppObjectList[8];
	CFrontCadDoc* pDoc;
	int NumberOfObjects;
	CPoint pointMouse;
	CScale PixelsPerInch;

	pDoc = GETVIEW->GetDocument();
	NumberOfObjects = pDoc->PointInObjectAndSelect(
		MousePos,
		pExcludeObject,
		ppObjectList,
		8,
		KindsToSnapTo
	);
	if (NumberOfObjects == 1)
	{
		if (KindsToSnapTo & OBJKIND_SELECT)
			pResult = ppObjectList[0];

	}
	else if (NumberOfObjects > 1)
	{
		//----------------------------
		// This is going to be tricky
		//----------------------------
		// Create a popup menu
		//----------------------------
		PixelsPerInch = GETVIEW->GetGrid().GetPixelsPerInch();
		pointMouse = MousePos.ToPixelPoint(
			GETVIEW->GetRulerInfo().GetUpperLeft(),
			PixelsPerInch.GetScaleX(),
			PixelsPerInch.GetScaleY()
		);
		GETVIEW->ClientToScreen(&pointMouse);
		Id = GETVIEW->CreateObjectSelectionMenu(
			ppObjectList,
			NumberOfObjects,
			pointMouse
		);
		if (Id)
		{
			Id -= POPUP_MENU_ITEM_IDS;
			pResult = ppObjectList[Id];
		}
	}
	else
		pResult = NULL;
	return pResult;
}

DOUBLEPOINT CFrontCadView::ConvertMousePosition(
	CPoint MousePoint,	//mouse position client ref
	DOUBLEPOINT& ULHC,	//upper left corner of client in inches
	CScale& Scale,		//Inches per Pixel
	CDoubleSize SnapGrid,
	BOOL SnapGridIsEnabled
)
{
	//-------------------------------------------------
	// ConvertMousePosition
	//		This method is used to calculate the mouse
	// psition on the worksheet.  Becasue the worksheet
	// is much bigger than the size of the screen, the
	// actual mouse position is the combination of
	// the mouse position in the view and the current
	// scroll position of the view.
	//
	// parameters:
	//	MousePoint......Position of mouse pointer in view
	//  ULHC............Upper Left Hand Corner Coordinate
	//	Scale...........Inches Per Pixel Scale
	//	SnapGrid........Snap to closest.
	//	SnapGridIsEnabled..Snap if true
	// 
	// returns:
	//		The Value, in inches, of the point.
	//-------------------------------------------------
	// Add the Upper Left Hand corner to the mouse point
	// the units of this value will be "pixels"
	//----------------------------------------------
	// Convert from pixels to inches
	//-----------------------------------------------
	DOUBLEPOINT MousePos;

	MousePos.Raw(MousePoint, ULHC, SCALE(Scale));
//	MousePos.Print("RAW:");
	MousePos.Snap(DOUBLESIZE(SnapGrid), SnapGridIsEnabled);
	m_DeltaMousePos = MousePos - m_LastMousePos;
	m_LastMousePos = m_CurMousePos;
	m_CurMousePos = MousePos;
	return m_CurMousePos;
}


