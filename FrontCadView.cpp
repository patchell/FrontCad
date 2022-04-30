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
	m_DrawState = ObjectDrawState::SELECT_NOTHING;
	m_SelectionCount = 0;
	m_pSelObjList = 0;
	m_pSelObjEnd = 0;
	m_GrabbedVertex = 0;
	m_CadObj.pCadObject = 0;
	m_pGrabbedObject = 0;
	m_pMoveObjects = 0;
	m_MouseLeftWindow = FALSE;
	m_MouseLocation = MouseIsHere::NOWHERE;
	m_AutoScrollTimerId = 5600;
	memset(&m_HScrollInfo, 0, sizeof(SCROLLINFO));
	memset(&m_VScrollInfo, 0, sizeof(SCROLLINFO));
	m_HScrollInfo.cbSize = sizeof(SCROLLINFO);
	m_HScrollInfo.cbSize = sizeof(SCROLLINFO);
	SetMoveObjects(0);
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
	//----- Menu Draw Objects ------
	ON_COMMAND(ID_DRAW_ARCANGLE, &CFrontCadView::OnDrawArcangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ARCANGLE, &CFrontCadView::OnUpdateDrawArcangle)
	ON_COMMAND(ID_DRAW_ARCCNTER, &CFrontCadView::OnDrawArcCenter)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ARCCNTER, &CFrontCadView::OnUpdateDrawArccnter)
	ON_COMMAND(ID_DRAW_ARDC, &CFrontCadView::OnDrawArc)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ARDC, &CFrontCadView::OnUpdateDrawArc)
	ON_COMMAND(ID_DRAW_ARROW, &CFrontCadView::OnDrawArrow)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ARROW, &CFrontCadView::OnUpdateDrawArrow)
	ON_COMMAND(ID_DRAW_ELLIPSE, &CFrontCadView::OnDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, &CFrontCadView::OnUpdateDrawEllipse)
	ON_COMMAND(ID_DRAW_LINE, &CFrontCadView::OnDrawLine)
	ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, &CFrontCadView::OnUpdateDrawLine)
	ON_COMMAND(ID_DRAW_ORIGIN, &CFrontCadView::OnDrawOrigin)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ORIGIN, &CFrontCadView::OnUpdateDrawOrigin)
	ON_COMMAND(ID_DRAW_POINT, &CFrontCadView::OnDrawPoint)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POINT, &CFrontCadView::OnUpdateDrawPoint)
	ON_COMMAND(ID_DRAW_POLYGOHN, &CFrontCadView::OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGOHN, &CFrontCadView::OnUpdateDrawPolygohn)
	ON_COMMAND(ID_DRAW_RECTANGLE, &CFrontCadView::OnDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, &CFrontCadView::OnUpdateDrawRectangle)
	ON_COMMAND(ID_DRAW_RECTANGULARHOLE, &CFrontCadView::OnDrawRectangularhole)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGULARHOLE, &CFrontCadView::OnUpdateDrawRectangularhole)
	ON_COMMAND(ID_DRAW_ROTATEDRECTANGLE, &CFrontCadView::OnDrawRotatedrectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROTATEDRECTANGLE, &CFrontCadView::OnUpdateDrawRotatedrectangle)
	ON_COMMAND(ID_DRAW_ROUNDEDRECTANGLE, &CFrontCadView::OnDrawRoundedrectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROUNDEDRECTANGLE, &CFrontCadView::OnUpdateDrawRoundedrectangle)
	ON_COMMAND(ID_DRAW_ROUNDHOLE, &CFrontCadView::OnDrawRoundhole)
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
END_MESSAGE_MAP()


// CFrontCadView drawing

void CFrontCadView::OnDraw(CDC* pDC)
{
	CDoublePoint UprLHCorner;
	CDC memDC;
	CRect rectClient;
	CBitmap memDCbitmap;
	CBitmap* pOldbm;
	CFrontCadDoc* pDoc = GetDocument();
	CCadObject* pDrawingObjectList, * pOriginList;
	CDoublePoint ULHC;		//upper left hand corner offset
	CBrush br;
	MODE mode;

	ULHC = GetRulerInfo().GetUpperLeft();
	GetClientRect(&rectClient);
	memDC.CreateCompatibleDC(pDC);
	memDCbitmap.CreateCompatibleBitmap(pDC, rectClient.Width(), rectClient.Height());
	pOldbm = memDC.SelectObject(&memDCbitmap);
	pDrawingObjectList = pDoc->GetHead();
	pOriginList = pDoc->GetOriginHead();
	CScale Scale = GetGrid().GetPixelsPerInch();
	mode.DrawMode = ObjectDrawMode::FINAL;

	br.CreateSolidBrush(RGB(0, 0, 0));
	memDC.FillRect(&rectClient, &br);

	UprLHCorner = GetRulerInfo().GetUpperLeft();
	GetGrid().Draw(&memDC, mode, UprLHCorner, Scale, rectClient);
	while (pDrawingObjectList)
	{
		pDrawingObjectList->Draw(&memDC, mode, UprLHCorner, Scale);
		pDrawingObjectList = pDrawingObjectList->GetNext();
	}
	while (pOriginList)
	{
		pOriginList->Draw(&memDC, mode, UprLHCorner, Scale);
		pOriginList = pOriginList->GetNextOrigin();
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
		GetObjectTypes().pCadObject->Draw(&memDC, mode, UprLHCorner, Scale);
	}
//	if (GetMoveObjectes())	//are there any objects being moved?
//		GetMoveObjectes()->Draw(&memDC, UprLHCorner, Scale);
//	pDoc->DrawReference(&memDC, Offset, Scale, rectClient);
	DrawCursor(
		&memDC, 
		GetCurrentMousePosition(),
		&rectClient,
		UprLHCorner,
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

//-----------------------------------------------
// Methods that manage the selection of objects
//-----------------------------------------------


void CFrontCadView::RemoveUnselectedObjects()
{
	///-------------------------------------
	/// RemoveUnselectedObjects
	///remove all unselected objects
	///--------------------------------------
	CCadObject* pO, * pOr, * pPrev = 0;

	pO = m_pSelObjList;
	while (pO)
	{
		if (!pO->IsSelected())
		{
			m_SelectionCount--;

			pOr = pO;
			if (!pPrev)
			{
				m_pSelObjList = pO->GetNextSel();
				pO = m_pSelObjList;
				pOr->SetNextSel(0);
			}
			else
			{
				//pPrev will stay the same
				pO = pO->GetNextSel();
				pPrev->SetNextSel(pOr->GetNextSel());
				pOr->SetNextSel(0);
			}
		}
		else
		{
			pPrev = pO;
			pO = pO->GetNextSel();
		}

	}
	if (m_pSelObjList == 0) m_pSelObjEnd = 0;
}

void CFrontCadView::RemoveSelectedObjects(CCadObject* pO)
{
	///----------------------------------------------
	///	RemoveObject
	/// This fuction removes the specified object
	/// from the selected list
	///		parameters:
	///			pO.....pointer to object to remove
	///----------------------------------------------
	CCadObject* pL, * pPrev = 0;
	int loop = 1;

	pL = m_pSelObjList;
	while (pL && loop)
	{
		if (pL == pO)
		{
			m_SelectionCount--;
			loop = 0;
			if (pPrev == 0)
			{
				m_pSelObjList = pL->GetNextSel();
				pL->SetNextSel(0);
			}
			else
			{
				pPrev->SetNextSel(pL->GetNextSel());
				pO->SetNextSel(0);
			}
		}
		else
		{
			pPrev = pL;
			pL = pL->GetNextSel();
		}

	}
	if (m_pSelObjList == 0) m_pSelObjEnd = 0;
}

void CFrontCadView::ClearSelList()
{
	//------------------------------------
	// ClearSelList
	//		This method removes all objects
	// from the selected list and also
	// unselects any objects that are still
	// slected.
	//------------------------------------
	CCadObject* pO = GetTopSelection();
	CCadObject* next;
	while (pO)
	{
		next = pO->GetNextSel();
		pO->SetSelected(0);
		RemoveObject(pO);
		pO = next;
	}
	m_SelectionCount = 0;
}

int CFrontCadView::SelectAnObject(CCadObject** ppObj, int n, CPoint p)
{
	//---------------------------------------------
	// SelectAnObject
	//		This method is called when the user has
	// requested that an object under the mouse be
	// selected, but there are multiple objects
	// there, and JamCad needs the user to clarify
	// parameters:
	//	ppObj........points to a list of posible objects
	//	n............Number of objects in the list
	//	p............Coordinate of mouse pointer
	// returns:
	//	Index in ppObj where desired object is
	//		returns negative on error
	//---------------------------------------------
	int i, Id;
	CString csString;
	CPoint point = p;
	ClientToScreen(&point);
	CMenu ConTextMenu;

	ConTextMenu.CreatePopupMenu();
	for (i = 0; i < n; ++i)
	{
		csString.Format(
			_T("%lS (%8.3lf,%8.3lf)"),
			ppObj[i]->GetTypeString().GetBuffer(),
			ppObj[i]->GetReference().dX,
			ppObj[i]->GetReference().dY
		);
		ConTextMenu.AppendMenu(MF_STRING, 3000 + i, csString);
		ConTextMenu.EnableMenuItem(3000 + i, MF_ENABLED);;
	}
	Id = ConTextMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
	ConTextMenu.DestroyMenu();
	if (Id >= 3000) Id -= 3000;
	else Id = -1;
	return Id;
}


void CFrontCadView::AddToSelList(CCadObject* pO)
{
	///----------------------------------------
	///	AddToSelList
	///		This Method adds an object to the
	/// selection list.
	///
	///	parameter:
	///		pO.....Object to add to list
	///---------------------------------------
	if (this->m_pSelObjList)
	{
		pO->SetNextSel(m_pSelObjList);
		m_pSelObjList = pO;
	}
	else
	{
		m_pSelObjList = pO;
		m_pSelObjEnd = pO;
	}
	m_SelectionCount++;
}

//--------------------------------------------
// User Interface Input devices
//-------------------------------------------

BOOL CFrontCadView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CDoublePoint pointMousePos;
	ScreenToClient(&pt);

	pointMousePos = ConvertMousePosition(
		pt,
		GetRulerInfo().GetUpperLeft(),
		GetGrid().GetInchesPerPixel(),
		GetGrid().GetSnapGrid(),
		GetGrid().IsSnapOn()
	);
	SetCurrentMousePosition(pointMousePos);

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
	}
	CFrontCadDoc* pDoc = GetDocument();
	pDoc->SetAllDirty();
	return CChildViewBase::OnMouseWheel(nFlags, zDelta, pt);
}


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
	m_LeftMouseButtonDown = TRUE;
	SetCurrentMousePosition(
		ConvertMousePosition(
			point,
			GetRulerInfo().GetUpperLeft(),
			GetGrid().GetInchesPerPixel(),
			GetGrid().GetSnapGrid(),
			GetGrid().IsSnapOn()
		)
	);
	switch (m_DrawMode)
	{
	case DrawingMode::SELECT:
		switch (m_DrawState)
		{
		case ObjectDrawState::SELECT_NOTHING:
			//------------------------------------
			// check to see if anything is selected
			// and are we clicking in the body of
			// of the object, or one of the verticies
			//-------------------------------------
			if (m_SelectionCount)	//are there selections?
			{
				CCadObject* pObj = GetTopSelection();
				m_GrabbedVertex = pObj->GrabPoint(GetCurrentMousePosition());
				if (m_GrabbedVertex >= 0)
				{
					m_DrawState = ObjectDrawState::VERTEX_GRABBED;
//					printf("Grabbed Vertex  %d\n", m_GrabbedVertex);
				}
			}
			break;
		case ObjectDrawState::SELECT_MOVE:
			break;
		case ObjectDrawState::SELECT_COPY:
			break;
		}
		Invalidate();
		break;
	case DrawingMode::MOVE:
		break;
	case DrawingMode::COPY:
		break;
	case DrawingMode::CUT:
		break;
	case DrawingMode::PASTE:
		switch (m_DrawState)
		{
			case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
			m_pMoveObjects = new CMoveObjects;
			m_pMoveObjects->Copy(GETAPP.GetClipBoard());
			Invalidate();
			break;
		}
		break;
	case DrawingMode::SELECTREGION:
		break;
	default:	//it must be something we are going to draw.
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
	CCadObject** pSel, * pObjSel, * pObj;
	int n;
	//-----------------------------------------------------
	// CBaseDocument is the base class for all of JamCad
	// drafting documents.  They will all have the same
	// virtual Method set
	//	virtual BOOL PointInObjectAndSelect(CPoint p, CCadObject **ppL, int n, int flag) { return 0; };
	//	virtual void AddObjectAtEnd(CCadObject * pObj) {};
	//	virtual void AddObject(CCadObject * pObj) {};
	//	virtual void RemoveObject(CCadObject * pObj) {};
	//	virtual void SetReference(CPoint Ref) {};
	//	virtual void DrawReference(CDC *pDC, CSize Offset, CScale Scale, CRect &clientrect) {};
	//	virtual void UpdateReference(CPoint newRef) {};
	//-----------------------------------------------------
	CBaseDocument* pDoc = (CBaseDocument*)GetDocument();

	SetCurrentMousePosition(
		ConvertMousePosition(
			point,
			GetRulerInfo().GetUpperLeft(),
			GetGrid().GetInchesPerPixel(),
			GetGrid().GetSnapGrid(),
			GetGrid().IsSnapOn()
		)
	);
	m_LeftMouseButtonDown = FALSE;
	switch (m_DrawMode)
	{
	case DrawingMode::SELECT:
		switch (m_DrawState)
		{
		case ObjectDrawState::VERTEX_GRABBED:
		{
			CCadObject* pOBj = GetTopSelection();
			pOBj->SetVertex(m_GrabbedVertex, GetCurrentMousePosition());
			m_DrawState = ObjectDrawState::SELECT_NOTHING;
		}
		break;
		case ObjectDrawState::SELECT_NOTHING:
			pSel = new CCadObject * [8];
			if (n = pDoc->PointInObjectAndSelect(
				GetCurrentMousePosition(),
				pSel,
				8,
				DrawingCheckSelectFlags::FLAG_ALL
			))
			{
				//we have found some objects to select
				if (n > 1)
				{
					int index = SelectAnObject(pSel, n, point);
					if (index < 0)
						break;
					pObjSel = pSel[index];
				}
				else
					pObjSel = pSel[0];
				if (pObjSel->IsSelected())
				{
					pObjSel->SetSelected(0);	//deselect object
					RemoveObject(pObjSel);
				}
				else
				{
					if (m_SelectionCount && !m_ControlKeyDown)
					{
						CCadObject* pO = GetTopSelection();
						while (pO)
						{
							RemoveObject(pO);	//remove object from selection list
							pO->SetSelected(0);	//set to unselected
							pO = GetTopSelection();	//get next object, if any
						}
					}
					pObjSel->SetSelected(1);
					AddToSelList(pObjSel);
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
				if (m_SelectionCount && !m_ControlKeyDown)
				{
					CCadObject* pO = GetTopSelection();
					while (pO)
					{
						RemoveObject(pO);	//remove object from selection list
						pO->SetSelected(0);	//set to unselected
						pO = GetTopSelection();	//get next object, if any
					}
					Invalidate();
				}
			}
			break;
		case ObjectDrawState::SELECT_MOVE:
		case ObjectDrawState::SELECT_COPY:
			pObj = m_pMoveObjects->RemoveFromHead();
			while (pObj)
			{
				pObj->Move(-GetCurrentMousePosition());
				pObj->SetSelected(0);
				pDoc->AddObjectAtEnd(pObj);
				pObj = m_pMoveObjects->RemoveFromHead();
			}
			delete m_pMoveObjects;
			m_pMoveObjects = 0;
			m_DrawState = ObjectDrawState::SELECT_NOTHING;
			Invalidate();
			break;
		}
		if (m_pGrabbedObject)
		{
			m_pGrabbedObject->Move(GetCurrentMousePosition());
			m_pGrabbedObject = 0;
		}
		break;
	case DrawingMode::MOVE:
		break;
	case DrawingMode::COPY:
		GETAPP.GetClipBoard()->Clear();		//clear old contents from clipboard
		GETAPP.CopyToClipBoard(m_pSelObjList);
		GETAPP.SetClipboardRef(GetCurrentMousePosition());
		Invalidate();
		break;
	case DrawingMode::CUT:
	{
		GETAPP.CutToClipboard(this->m_pSelObjList);
		GETAPP.SetClipboardRef(GetCurrentMousePosition());
		CCadObject* pO = GetTopSelection();
		while (pO)
		{
			pDoc->RemoveObject(pO);
			this->RemoveObject(pO);
			pO = GetTopSelection();
		}
		Invalidate();
	}
	break;
	case DrawingMode::PASTE:	//Mouse Button Up
		switch (m_DrawState)
		{
		case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
			GETAPP.UpdateStatusBar(_T("PASTE:Position Objects"));
			m_DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
			break;
		case ObjectDrawState::PLACE_LBUTTON_UP:
			GETAPP.UpdateStatusBar(_T("SELECT an Object"));
			m_DrawMode = DrawingMode::SELECT;
			m_DrawState = ObjectDrawState::SELECT_NOTHING;
			{
				CCadObject* pO = m_pMoveObjects->RemoveFromHead();
				while (pO)
				{
					pO->SetSelected(0);
					pO->Move(-GetCurrentMousePosition());
					pDoc->AddObjectAtEnd(pO);
					pO = m_pMoveObjects->RemoveFromHead();
				}
				delete m_pMoveObjects;
				m_pMoveObjects = 0;
			}
			break;
		}
		break;
	case DrawingMode::SELECTREGION:
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
	CDoublePoint pointCenter;
	CDoublePoint pointMousePos;
	CPoint ptScreen = point;
	ClientToScreen(&ptScreen);

	csDB.Format(_T("C(%d, %d)  S(%d,%d)"), point.x, point.y, ptScreen.x, ptScreen.y);
	ToolBarSetDebug(csDB);
	pointMousePos = ConvertMousePosition(
		point,
		GetRulerInfo().GetUpperLeft(),
		GetGrid().GetInchesPerPixel(),
		GetGrid().GetSnapGrid(),
		GetGrid().IsSnapOn()
	);
	SetCurrentMousePosition(pointMousePos);
	if (pDoc->GetCurrentOrigin())
	{
		pDoc->GetCurrentOrigin()->GetCenter(pointCenter);
		pointCenter.dX = pointMousePos.dX - pointCenter.dX;
		pointCenter.dY = pointCenter.dY - pointMousePos.dY;
		ToolBarSetMousePosition(pointCenter);
	}
	else
		ToolBarSetMousePosition(GetCurrentMousePosition());
	if (DidMouseLeaveWindow())
	{
		SetMouseLeftWindow(FALSE);
//		printf("Mouse Entered Window > MMB=%d\n", m_MiddleMouseButtonDown);
		TrackMouseEvent(&m_TrackMouseEvent);
		if (IsMiddleMouseButtonDown())
		{
			SetMiddleMouseButtonDown(FALSE);
			EnableAutoScroll(0);
		}
	}
	switch (m_DrawMode)
	{
	case DrawingMode::SELECT:
		switch (m_DrawState)
		{
		case ObjectDrawState::VERTEX_GRABBED:
			if (IsLeftMouseButtonDown())
			{
				CCadObject* pOBj = GetTopSelection();
				pOBj->SetVertex(m_GrabbedVertex, GetCurrentMousePosition());
			}
			break;
		case ObjectDrawState::SELECT_NOTHING:
			//-----------------------------------
			// If the mouse moves,
			// while down, we are then
			// going to grab and either move or
			// copy the objects
			//------------------------------------
			if (IsLeftMouseButtonDown() && m_SelectionCount)
			{
				if (m_ShiftKeyDown)	//copy selection
				{
					m_pMoveObjects = new CMoveObjects();
					CCadObject* pObj = GetTopSelection();
					while (pObj)
					{
						m_pMoveObjects->AddObject(pObj->CopyObject());
						pObj = pObj->GetNextSel();
					}
					m_DrawState = ObjectDrawState::SELECT_COPY;
				}
				else  //move selection
				{
					m_pMoveObjects = new CMoveObjects();
					CCadObject* pObj = GetTopSelection();
					while (pObj)
					{
						RemoveObject(pObj);
						pDoc->RemoveObject(pObj);
						m_pMoveObjects->AddObject(pObj);
						pObj = GetTopSelection();
					}
					m_DrawState = ObjectDrawState::SELECT_MOVE;
				}
				m_pMoveObjects->SetRef(GetCurrentMousePosition());
			}	//end of if MouseDown
			break;
		case ObjectDrawState::SELECT_MOVE:
		case ObjectDrawState::SELECT_COPY:
			break;
		}
		Invalidate();
		if (IsLeftMouseButtonDown())
		{
			if (m_pGrabbedObject)
				m_pGrabbedObject->Move(GetCurrentMousePosition());
		}
		break;
	case DrawingMode::MOVE:
		break;
	case DrawingMode::COPY:
		Invalidate();
		break;
	case DrawingMode::CUT:
		Invalidate();
		break;
	case DrawingMode::PASTE:
		Invalidate();
		break;
	case DrawingMode::SELECTREGION:
		break;
	default:
		//--------------------------------------------
		// If we get here, we are in the midst of
		// actually drawing an object
		//--------------------------------------------
		if (m_CadObj.pCadObject) m_DrawState = m_CadObj.pCadObject->MouseMove(m_DrawState);
		else
			Invalidate();
		break;
	}

	CChildViewBase::OnMouseMove(nFlags, point);
	GetRulerInfo().SetCursorPos(GetCurrentMousePosition());
	PostMessageToRulers(RW_POSITION);
}

void CFrontCadView::OnInitialUpdate()
{
	m_pParentFrame = (CFrontCadChildFrame*)GetParentFrame();
	CFrontCadDoc *pDoc = GetDocument();
	CString csOrgName = _T("Default");

	//------------ Setup Default Origin --------------
	m_DefaultOrigin.Create(0, pDoc->GetDocHieght(), csOrgName);
	pDoc->AddOriginAtFront(&m_DefaultOrigin);
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
	GetRulerInfo().SetUpperLeft(CDoublePoint(0.0, 0.0));
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

void CFrontCadView::OnAutoScroll(CPoint /*point*/, BOOL /*bBeforeScroll*/)
{
}

void CFrontCadView::RemoveObject(CCadObject* pObj)
{
	CFrontCadDoc* pDoc = GetDocument();
	pDoc->RemoveObject(pObj);
}

void CFrontCadView::AddObjectAtFrontIntoDoc(CCadObject* pObj)
{
	CFrontCadDoc* pDoc = GetDocument();
	pDoc->AddObjectAtEnd(pObj);
}

void CFrontCadView::AddObjectAtEndIntoDoc(CCadObject* pObj)
{
	//--------------------------------------------------
	// AddObjectAtEndIntoDoc
	//		This methode inserts a new object into the
	//	document that is connected to this view.  The
	// object is placed last in line.
	//
	// parameters:
	//		pObj........point to object to insert
	//--------------------------------------------------
	CFrontCadDoc* pDoc = GetDocument();
	pDoc->AddObjectAtEnd(pObj);
}

BOOL CFrontCadView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//-----------------------------------------------------
// Methods for placing objects on drawings
//-----------------------------------------------------

void CFrontCadView::OnDrawArcangle()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateDrawArcangle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawArcCenter()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetObjectTypes(new CCadArcCent);
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
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::ARC);
	SetObjectTypes(new CCadArc);
	GETAPP.UpdateStatusBar(_T("Arc:Place 1st Point of Defining Rectangle"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawArc(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawArrow()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::ARROW);
	SetObjectTypes(new CCadArrow);
	GETAPP.UpdateStatusBar(_T("Arrow:Place Location of Tip"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}

void CFrontCadView::OnUpdateDrawArrow(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawEllipse()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::ELIPSE);
	SetObjectTypes(new CCadElispe);
	GETAPP.UpdateStatusBar(_T("Ellispe Place 1st Point of Defining Rectangle"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawEllipse(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawLine()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::LINE);
	SetObjectTypes(new CCadLine);
	GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawLine(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawOrigin()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::ORIGIN);
	SetObjectTypes(new CCadOrigin);
	GETAPP.UpdateStatusBar(_T("Origin:Set Origin Name"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawOrigin(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawPoint()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::POINT);
//	SetDataTypes(new CDoublePoint);
	GETAPP.UpdateStatusBar(_T("Point:Place Location of Point"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawPoint(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawPlaceBitmap()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::BITMAP);
	SetObjectTypes(new CCadBitmap);
	GETAPP.UpdateStatusBar(_T("Bitmap:Place 1st Point of Defining Rectangle"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}

void CFrontCadView::OnUpdateDrawPlacebitmap(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawPolygon()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::POLYGON);
	SetObjectTypes(new CCadPolygon);
	GETAPP.UpdateStatusBar(_T("Polygon:Place 1st Point"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawPolygohn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawRectangle()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::RECT);
	SetObjectTypes(new CCadRect);
	GETAPP.UpdateStatusBar(_T("Rectangle:Place 1st Point"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawRectangle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawRectangularhole()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::HOLE_RECT);
	SetObjectTypes(new CCadHoleRect);
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
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::RNDRECT);
	SetObjectTypes(new CCadRndRect);
	GETAPP.UpdateStatusBar(_T("Rounded Rectangle:Place 1st Point of Defining Rectangle"));
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}

void CFrontCadView::OnUpdateDrawRoundedrectangle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawRoundhole()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::HOLE_ROUND);
	GETAPP.UpdateStatusBar(_T("Round Hole:Place Location of Hole Center"));
	SetObjectTypes(new CCadHoleRound);
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
}


void CFrontCadView::OnUpdateDrawRoundhole(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawRoundholewith1flat()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::HOLE_RND1F);
	SetObjectTypes(new CCadHoleRnd1Flat);
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
	GETAPP.UpdateStatusBar(_T("Round Hole With one Flat:Place location of Center"));
}

void CFrontCadView::OnUpdateDrawRoundholewith1flat(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}

void CFrontCadView::OnDrawRoundholewith2flats()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::HOLE_RND2F);
	SetObjectTypes(new CCadHoleRnd2Flat);
	m_DrawState = GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::START_DRAWING);
	GETAPP.UpdateStatusBar(_T("Round Hole With two Flats:Place Location of Center"));
}


void CFrontCadView::OnUpdateDrawRoundholewith2flats(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
}


void CFrontCadView::OnDrawText()
{
	if (GetObjectTypes().pCadObject)
	{
		delete GetObjectTypes().pCadObject;
		GetObjectTypes().pCadObject = 0;
	}
	SetDrawMode(DrawingMode::TEXT);
	SetObjectTypes(new CCadText);
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
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateSnapSnaptoobject(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CFrontCadView::OnSnapSnaptotangent()
{
	// TODO: Add your command handler code here
}


void CFrontCadView::OnUpdateSnapSnaptotangent(CCmdUI* pCmdUI)
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


#define ID_CM_FORWARD				3100
#define ID_CM_BACKWARD				3101
#define ID_CM_DELETE				3102
//#define ID_CM_MOVE					3103
#define ID_CM_RESTOREASPECTRATIO	3104
#define ID_CM_ENDDRAWMODE			3105
#define ID_CM_DESELECT				3106
#define ID_CM_DESELECT_ALL			3107
#define ID_CM_EDIT_PROPERIES		3108
#define ID_CM_PLACE					3109

//place submenu
#define ID_CM_PLACE_LINE			3200
#define ID_CM_PLACE_RECT			3201
#define ID_CM_PLACE_ELLISPE			3202
#define ID_CM_PLACE_ARC				3203
#define ID_CM_PLACE_ARC_CENTERED	3204
#define ID_CM_PLACE_ROUNDED_RECT		3205
#define ID_CM_PLACE_ARROW			3206
#define ID_CM_PLACE_POLY			3207
#define ID_CM_PLACE_FILLEDPOLY		3208
#define ID_CM_PLACE_LIBCOMP			3209
#define ID_CM_PLACE_HOLEROUND		3210
#define ID_CM_PLACE_HOLERECT		3211
#define ID_CM_PLACE_HOLERND1FLAT	3212
#define ID_CM_PLACE_HOLERND2FLAT	3213
#define ID_CM_PLACE_TEXT			3214
#define ID_CM_PLACE_BITMAP			3215
#define ID_CM_PLACE_ORIGIN			3216
#define ID_CM_PLACE_DIMENSION		3217


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
	CBaseDocument* pDoc = (CBaseDocument*)GetDocument();
	CMenu ConTexMenu;
	CMenu PlaceMenu;

	int id;
	//	CPoint m_MousePos = point;
	//	CDoublePoint CurPos;

	//	ScreenToClient(&m_MousePos);
	//	CurPos = ConvertMousePosition(m_MousePos).Snap(GetSnapGrid(), IsSnapGridOn();
	CCadObject* pObj = GetTopSelection();
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
			delete GetObjectTypes().pCadObject;
			SetObjectTypes(0);
		}
		SetDrawMode(DrawingMode::SELECT);
		SetDrawState(ObjectDrawState::SELECT_NOTHING);
		EnableAutoScroll(0);
		break;
	case ID_CM_EDIT_PROPERIES:
		if (GetObjectTypes().pCadObject)
			GetObjectTypes().pCadObject->ProcessDrawMode(ObjectDrawState::SET_ATTRIBUTES);
		break;
	case ID_CM_FORWARD:		//make object draw last
	{
		CCadObject* pO = GetTopSelection();
		while (pO)
		{
			pDoc->RemoveObject(pO);
			pDoc->AddObjectAtEnd(pO);
			pO = pO->GetNextSel();
		}
		Invalidate();
	}
	break;
	case ID_CM_BACKWARD:	//make object draw first
	{
		CCadObject* pO = GetTopSelection();
		while (pO)
		{
			pDoc->RemoveObject(pO);
			pDoc->AddObjectAtEnd(pO);
			pO = pO->GetNextSel();
		}
		Invalidate();
	}
		break;
	case ID_CM_DELETE:	//delete selected objects
//		printf("Delete Object\n");
		{
			CCadObject* pO = GetTopSelection();
			while (pO)
			{
				RemoveObject(pO);
				pDoc->RemoveObject(pO);
				delete pO;
				pO = GetTopSelection();
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
			CCadObject* pO = GetTopSelection();
			RemoveObject(pO);
			pO->SetSelected(0);
			Invalidate();
		}
		break;
	case ID_CM_DESELECT_ALL:
//		printf("Deselect All\n");
		{
			CCadObject* pO = GetTopSelection();
			while (pO)
			{
				RemoveObject(pO);
				pO->SetSelected(0);
				pO = GetTopSelection();
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
				case ObjectDrawState::SELECT_NOTHING:
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
			case ObjectDrawState::SELECT_NOTHING:
				break;
			case ObjectDrawState::SELECT_COPY:
				break;
			}	//end of switch(m_DrawState
			break;
		}	//end of switch(m_DrawMode)
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

	CBaseDocument* pDoc = (CBaseDocument*)GetDocument();
	switch (nChar)
	{
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
			m_DrawState = ObjectDrawState::SELECT_NOTHING;
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
		ppSelList = new CCadObject * [8];
		n = pDoc->PointInObjectAndSelect(
			GetCurrentMousePosition(),
			ppSelList,
			8,
			DrawingCheckSelectFlags::FLAG_ALL
		);
		if (n > 0)
		{
			CDoublePoint Center;
			ppSelList[0]->Flip(ppSelList[0]->GetCenter(Center), 0);
		}
		delete[]ppSelList;
		Invalidate();
		break;
	case 'Y':
		ppSelList = new CCadObject * [8];
		n = pDoc->PointInObjectAndSelect(
			GetCurrentMousePosition(),
			ppSelList,
			8,
			DrawingCheckSelectFlags::FLAG_ALL
		);
		if (n > 0)
		{
			CDoublePoint Center;
			ppSelList[0]->Flip(ppSelList[0]->GetCenter(Center), 1);
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
			case ObjectDrawState::SELECT_NOTHING:
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
		case DrawingMode::SELECT:
			switch (m_DrawState)
			{
			case ObjectDrawState::SELECT_MOVE:
				break;
			case ObjectDrawState::SELECT_NOTHING:
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
		printf("Size 1\n");
		break;
	case SIZE_MINIMIZED:
		printf("Size 2\n");
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
		printf("Size 4\n");
		break;
	case SIZE_MAXSHOW:
		printf("Size 5\n");
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
	CFrontCadDoc* pDoc = GetDocument();;
	CDoublePoint UL;

	if (Update)
	{
		if (Absolute)
		{
			UL = GetRulerInfo().GetUpperLeft();
			UL.dY = Yinches;
		}
		else
			UL = GetRulerInfo().GetUpperLeft() + CDoubleSize(0.0, Yinches);

		if (UL.dY < 0.0)
			UL.dY = 0.0;
		else if (UL.dY > (pDoc->GetDocSize().dCY - GetClientHieght()))
		{
			UL.dY = pDoc->GetDocSize().dCY - GetClientHieght();
			if (UL.dY < 0.0)
				UL.dY = 0.0;
		}
		GetRulerInfo().SetUpperLeft(UL);
		double pos = UL.dY;
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
		printf("End ");
		Update = FALSE;
		break;
	}
	printf("nPos = %d  Xinches = %8.3lf  Update=%d\n", nPos, Xinches,Update);
	DoHScroll(Xinches, Update);
	CChildViewBase::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFrontCadView::DoHScroll(double Xinches, BOOL Absolute, BOOL Update)
{
	CDoublePoint UL;
	CFrontCadDoc* pDoc = GetDocument();

	if (Update)
	{
		if (Absolute)
		{
			UL = GetRulerInfo().GetUpperLeft();
			UL.dX = Xinches;
		}
		else
			UL = GetRulerInfo().GetUpperLeft() + CDoubleSize(Xinches, 0.0);
		if (UL.dX < 0.0)
			UL.dX = 0.0;
		else if (UL.dX > (pDoc->GetDocSize().dCX - GetClientWidth()))
		{
			UL.dX = pDoc->GetDocSize().dCX - GetClientWidth();
			if (UL.dX < 0.0)
				UL.dX = 0;
		}
		GetRulerInfo().SetUpperLeft(UL);

		double pos = UL.dX;
		pos = pos * GetGrid().GetPixelsPerInch().GetScaleX();
		SetScrollPos(SB_HORZ, pos, TRUE);
		//----------------------------------------------------------
		// Update Rulers
		//---------------------------------------------------------
		PostMessageToRulers(RW_HSCROLL);
		PostMessageToRulers(RW_POSITION);
		Invalidate();
	}
}

void CFrontCadView::UpdateScrollbarInfo(CDoublePoint ULHC)
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


CDoublePoint CFrontCadView::ConvertMousePosition(
	CPoint& MousePoint,	//mouse position client ref
	CDoublePoint& ULHC,	//upper left corner of client in inches
	CScale& Scale,		//Inches per Pixel
	CDoubleSize& SnapGrid,
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
	return (CDoublePoint(MousePoint) * Scale + ULHC).Snap(SnapGrid, SnapGridIsEnabled);
}


void CFrontCadView::DrawCursor(
	CDC* pDC, 
	CDoublePoint& pos, 
	CRect* pRect, 
	CDoublePoint& ULHC, 
	CScale& Scale, 
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
	pen.CreatePen(PS_SOLID, 1, color);
	pOldPen = pDC->SelectObject(&pen);
	int w, h;
	CPoint p;
//	CDoublePoint Org = GetDocument()->GetCurrentOrigin()->GetCenter(Org);

	pos -= GetRulerInfo().GetUpperLeft();
	pos = pos * Scale;
	pos.RoundToNearest(CDoubleSize(1.0,1.0));
	p = CPoint(GETAPP.RoundDoubleToInt(pos.dX), GETAPP.RoundDoubleToInt(pos.dY));
	w = pRect->Width();
	h = pRect->Height();
	pDC->MoveTo(0, p.y);
	pDC->LineTo(w, p.y);
	pDC->MoveTo(p.x, 0);
	pDC->LineTo(p.x, h);
	pDC->SelectObject(pOldPen);
}

// Zoom in around a point

CDoublePoint& CFrontCadView::CalculateNewULHC(
	CScale CurrentScale, 
	CScale NextScale, 
	CDoublePoint& pointResult, 
	CDoublePoint& pointLocation,
	CDoublePoint& pointULHC
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

	Temp = CurrentScale.GetScaleX() / NextScale.GetScaleX();
	Temp *= (pointLocation.dX - pointULHC.dX);
	Ux = pointLocation.dX - Temp;
	Temp = CurrentScale.GetScaleY() / NextScale.GetScaleY();
	Temp *= (pointLocation.dY - pointULHC.dY);
	Uy = pointLocation.dY - Temp;
	pointResult = CDoublePoint(Ux, Uy);
	return pointResult;
}

void CFrontCadView::ZoomIn(CDoublePoint point)
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
	CDoublePoint pointNewULHC;
	CDoublePoint ptULHC = GetRulerInfo().GetUpperLeft();

	CurrentScale = GetGrid().GetPixelsPerInch();
	if (GetGrid().ZoomIn())	// did a zoom
	{
		NextScale = GetGrid().GetPixelsPerInch();
		pointNewULHC = CalculateNewULHC(
			CurrentScale,
			NextScale,
			pointNewULHC,
			point,
			ptULHC
		);
		//---------------------------------
		// Clean up ULHC ie Snap
		//--------------------------------
		pointNewULHC.Snap(GetGrid().GetSnapGrid(), TRUE);
		//--------------------------------
		// Update the Scroll Position
		//--------------------------------
		UpdateScrollbarInfo(pointNewULHC);
		SetCursorPosition(GetCurrentMousePosition());
		Invalidate();
	}
}


// //zoom out around a point
void CFrontCadView::ZoomOut(CDoublePoint point)
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
	CDoublePoint pointNewULHC;
	CDoublePoint ptULHC = GetRulerInfo().GetUpperLeft();

	CurrentScale = GetGrid().GetPixelsPerInch();
	if (GetGrid().ZoomOut())	// did a zoom
	{
		NextScale = GetGrid().GetPixelsPerInch();
		pointNewULHC = CalculateNewULHC(
			CurrentScale,
			NextScale,
			pointNewULHC,
			point,
			ptULHC
		);
		pointNewULHC.Snap(GetGrid().GetSnapGrid(), TRUE);
		//--------------------------------
		// Update the Scroll Position
		//--------------------------------
		UpdateScrollbarInfo(pointNewULHC);
		SetCursorPosition(GetCurrentMousePosition());
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
		m_AutoScrollTimerId = SetTimer(m_AutoScrollTimerId, 100, NULL);
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
CDoublePoint CFrontCadView::ProcessMousePosition(
	CPoint point,
	CDoubleSize SnapGrid,
	int Which
)
{
	//---------------------------------------------
	// ProcessMousePosition
	//	First, we need to correct the mouse position
	// for the fact that our virtual work surface
	// is much larger than the screen
	// m_CurMousePos is still in screen units
	//
	// parameters:
	//	point.....raw mouse position
	//	SnapGrid..Snap Grid constant
	//	Which.....Selects the mode of the processing
	//--------------------------------------------
	CDoublePoint rV;

	switch (Which)
	{
	case PROC_MOUSE_POSISTION_CORRECTED:
	case PROC_MOUSE_POSISTION_SCALED:
		rV = ConvertMousePosition(
			point,
			GetRulerInfo().GetUpperLeft(),
			GetGrid().GetInchesPerPixel(),
			GetGrid().GetSnapGrid(),
			GetGrid().IsSnapOn()
		);
		break;
	case PROC_MOUSE_POSISTION_SNAPPED:
		rV = ConvertMousePosition(
			point,
			GetRulerInfo().GetUpperLeft() ,
			GetGrid().GetInchesPerPixel(),
			SnapGrid,
			TRUE
		);
		break;
	}
	return rV;
}

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

void CFrontCadView::OnTimer(UINT_PTR nIDEvent)
{
	//-----------------------------------------
	// The Timer is used to perform the task
	// of Auto Scrolling the view
	// 
	// 
	//-----------------------------------------
	MouseIsHere MouseLocation;

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
	pDoc->AddOriginAtFront(pObj);
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
		printf("Origin Changed\n");
		break;
	}
	return 0;
}

