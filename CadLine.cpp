#include "pch.h"

CCadLine::CCadLine():CCadObject()
{
	m_Length = 0.0;
	SetType(ObjectType::LINE);
	GetName().Format(_T("Line_%d"), ++m_LineCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetLineAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadLine::CCadLine(CCadLine &line) :CCadObject()
{
	CopyAttributesFrom(&line.GetAttributes());
	CCadPoint* pCP = (CCadPoint*)line.GetChildrenHead();
	while (pCP)
	{
		AddObjectAtChildHead(pCP);
		pCP = (CCadPoint * )pCP->GetNext();
	}
	SetType(ObjectType::LINE);
}

CCadLine::~CCadLine()
{
}

BOOL CCadLine::Create(CCadObject* pParent, CCadObject* pOrigin)
{
	CCadPoint* pPoint;
	CCadRect* pRect;

	CCadObject::Create(pParent, pOrigin);
	if (pParent == NULL)
		pParent = this;
	pPoint = new CCadPoint;
	pPoint->Create(pParent, pOrigin);
	pPoint->SetSubType(SubType::VERTEX);
	pPoint->SetSubSubType(1);
	AddObjectAtChildTail(pPoint);
	pPoint = new CCadPoint;
	pPoint->Create(pParent, pOrigin);
	pPoint->SetSubType(SubType::VERTEX);
	pPoint->SetSubSubType(2);
	AddObjectAtChildTail(pPoint);
	pRect = new CCadRect;
	pRect->Create(pParent, pOrigin);
	pRect->SetSubType(SubType::RECTSHAPE);
	AddObjectAtChildTail(pRect);
	m_Length = 0.0;
	return TRUE;
}

void CCadLine::Move(CDoubleSize Diff)
{
	//---------------------------------------------------
	//	Move
	//		This Method is used to move the object
	// by the amount that is passed.
	//
	// parameters:
	//	Diff.......amount to move the object by
	//
	// return value: none
	//--------------------------------------------------
	CCadObject::Move(Diff);
}


void CCadLine::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//---------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* pIndent = new char[256];

	fprintf(pO, "%s%s(%s(%8.3lf,%8.3lf),%s(%8.3lf,%8.3lf))",
		GETAPP.MkIndentString(pIndent,Indent, ' '),
		CLexer::TokenToString(DocFileParseToken::LINE),
		CLexer::TokenToString(DocFileParseToken::POINT),
		((CCadPoint*)GetChildrenHead())->GetX(),
		((CCadPoint*)GetChildrenHead())->GetY(),
		CLexer::TokenToString(DocFileParseToken::POINT),
		((CCadPoint*)GetChildrenHead()->GetNext())->GetX(),
		((CCadPoint*)GetChildrenHead()->GetNext())->GetY()
	);
	GetAttributes().Save(pO, Indent + 1, flags);
	delete[] pIndent;
}


void CCadLine::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
{
	//---------------------------------------------------
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	Offset..Offset to draw objects at
	//	Scale..Scale factor to draw objects at
	//
	// return value:none
	//--------------------------------------------------
	CPen *pOld, penLine;
	int Lw;
	CRect rect;
	CADObjectTypes pObj;

	if (IsRenderEnabled())
	{
		Lw = int(Scale.dSX * GetLineWidth());
		if (Lw < 1) Lw = 1;
		CreateThePen(mode, &penLine, Lw);
		pOld = pDC->SelectObject(&penLine);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			pObj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
			pObj.pCadPoint->MoveTo(pDC, ULHC, Scale);
			pObj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 2);
			pObj.pCadPoint->LineTo(pDC, ULHC, Scale);
			break;
		case ObjectDrawMode::SKETCH:
			switch (GetCurrentDrawState())
			{
			case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
				pObj.pCadObject = FindChildObject(ObjectType::POINT, SubType::RIGHTANGLE_VERTEX, 0);
				pObj.pCadPoint->MoveTo(pDC, ULHC, Scale);
				pObj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
				pObj.pCadPoint->LineTo(pDC, ULHC, Scale);
				break;
			case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_DOWN:
				pObj.pCadObject = FindChildObject(ObjectType::POINT, SubType::RIGHTANGLE_VERTEX, 0);
				pObj.pCadPoint->MoveTo(pDC, ULHC, Scale);
				pObj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
				pObj.pCadPoint->LineTo(pDC, ULHC, Scale);
				break;
			case ObjectDrawState::PLACE_LBUTTON_DOWN:
			case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN:
				pObj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
				pObj.pCadPoint->MoveTo(pDC, ULHC, Scale);
				pObj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 2);
				pObj.pCadPoint->LineTo(pDC, ULHC, Scale);
				break;
			}//end of switch draw state
			break;
		}	//end of switch(mode)
		pDC->SelectObject(pOld);
		//-------------------------------------
		// Draw the Children
		//-------------------------------------
		pObj.pCadObject = GetChildrenHead();
		while (pObj.pCadObject)
		{
			if (pObj.pCadObject->GetType() == ObjectType::POINT)
			{
				pObj.pCadObject->Draw(pDC, mode, ULHC, Scale);
			}
			pObj.pCadObject = pObj.pCadObject->GetNext();
		}
	}	//end of if(rederEnabled)
}

BOOL CCadLine::PointInThisObject(DOUBLEPOINT point)
{
	BOOL rV = FALSE;
	CADObjectTypes Obj;

	Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
	rV = Obj.pCadRect->PointInThisObject(point);
	return rV;
}

int CCadLine::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject* pExcludeObject,
	CCadObject** ppSelList,
	int index,
	int n,
	UINT nKinds
)
{
	//---------------------------------------------------
	// PointInObjectAndSelect
	//	This Method is used to see if an object can
	// be selected at point p.
	//
	// parameters:
	//	p...........point to check at
	//	Offset......Offset of drawing
	//	ppSelList...pointer to list of selected objects
	//	index.......current index into the selection list
	//	n...........Total number of spaces in slection list
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------

	if (index < n)
	{
		//---------------------------------------
		// is point in the Arc?
		//---------------------------------------
		if (PointInThisObject(p))
		{
			if (IsItThisKind(nKinds) && DontExclude(pExcludeObject))
			{
				ppSelList[index++] = this;
				ShouldWeSelectThisObjectAndDidIt(nKinds);
			}
		}
		index = CCadObject::PointInObjectAndSelect(
			p,
			pExcludeObject,
			ppSelList,
			index,
			n,
			nKinds
		);
	}
	return index;
}


CString& CCadLine::GetTypeString(void)
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Line");
	return csName;
}

CString& CCadLine::GetObjDescription()
{
	DOUBLEPOINT P1, P2;
	CCadPoint* pPoint;

	pPoint = (CCadPoint*)FindChildObject(
		ObjectType::POINT,
		SubType::VERTEX,
		1
	);
	P1 = DOUBLEPOINT(*pPoint);
	pPoint = (CCadPoint*)FindChildObject(
		ObjectType::POINT,
		SubType::VERTEX,
		2
	);
	P2 = DOUBLEPOINT(*pPoint);
	GetDescription().Format(_T("LINE((%6.3lf,%6.3lf),(%6.3lf,%6.3lf))"), 
		P1.dX, P1.dY,
		P2.dX, P2.dY
	);
	return GetDescription();
}

CCadObject * CCadLine::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CADObjectTypes newObj;
	newObj.pCadLine = new CCadLine;
	CCadObject::CopyObject(newObj.pCadObject);
	newObj.pCadLine->CopyAttributesFrom(GePtrTotAttributes());
	newObj.pCadLine->m_Length = m_Length;
	return newObj.pCadObject;
}


CDoubleSize CCadLine::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	DOUBLEPOINT P1, P2;
	CCadPoint* pPoint;

	pPoint = (CCadPoint*)FindChildObject(
		ObjectType::POINT,
		SubType::VERTEX,
		1
	);
	P1 = DOUBLEPOINT(*pPoint);
	pPoint = (CCadPoint*)FindChildObject(
		ObjectType::POINT,
		SubType::VERTEX,
		2
	);
	P2 = DOUBLEPOINT(*pPoint);
	return CDoubleSize(abs(P1.dX - P2.dX), abs(P1.dY - P2.dY));
}

DocFileParseToken CCadLine::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
{
	//---------------------------------------------------
	// Parse
	//	This Method is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token....Token for this object
	//	pLex.....Pointer to Lexer for the data stream
	//
	// return value:
	//	returns lookahead token on success, or
	//			negative value on error
	//--------------------------------------------------
	Token = pLex->Accept(Token, TypeToken);
	Token = pLex->Accept(Token, DocFileParseToken('('));
//	Token = pLex->Point(DocFileParseToken::POINT, m_Line.GetPoint(LinePoint::P1), Token);
	Token = pLex->Accept(Token, DocFileParseToken(','));
//	Token = pLex->Point(DocFileParseToken::POINT, m_Line.GetPoint(LinePoint::P2), Token);
	Token = pLex->Accept(Token, DocFileParseToken(')'));
	Token = GetAttributes().Parse(Token, pLex);
	return Token;
}

void CCadLine::CopyAttributesTo(SLineAttributes*pAttrib)
{
	//---------------------------------------------------
	//	CopyAttributesTo
	//		This Method is used to copy the
	//	attributes from this object into one pointed
	//	to by the parameter.
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy into
	//-------------------------------------------------/
	GetAttributes().CopyTo(pAttrib);
}

void CCadLine::CopyAttributesFrom(SLineAttributes*pAtrib)
{
	//---------------------------------------------------
	//	CopyAttributesFrom
	//		This Method is used to copy the
	//	attributes pointed to by the parameter into
	//	this object
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy
	//---------------------------------------------------/
	GetAttributes().CopyFrom(pAtrib);
	SetAttributesValid();
}

ObjectDrawState CCadLine::ProcessDrawMode(ObjectDrawState DrawState)
{
	//-------------------------------------------------------
	//	ProcessDrawMode
	//		This is the state machine for creating this
	//	object on the screen.  
	//
	//	parameters:
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	UINT Id;
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();
	CADObjectTypes ObjP1, ObjP2;
	CCadLine* pNewLine = 0;
	CPoint pointSaved;
	CADObjectTypes Corner;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
		break;
	case ObjectDrawState::END_DRAWING:
		if (m_AttributesDirty)
		{
			Id = GETVIEW->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
			if (IDYES == Id)
			{
				m_CurrentAttributes.CopyTo(&m_LastAttributes);
			}
			m_AttributesDirty = FALSE;
		}
		if(GETVIEW->IsAutoScrollEnabled())
			GETVIEW->EnableAutoScroll(FALSE);
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		Id = EditProperties();
		if (IDOK == Id)
		{
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		ObjP1.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		ObjP1.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Place Second Popint"));
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		GETVIEW->EnableAutoScroll(FALSE);
		ObjP2.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			2
		);
		ObjP2.pCadPoint->SetPoint(MousePos);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		pNewLine = new CCadLine;
		pNewLine->Create(NULL, GETVIEW->GetDocument()->GetCurrentOrigin());
		GETVIEW->SetObjectTypes(pNewLine);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
		GETVIEW->Invalidate();
		SetCurrentDrawState(DrawState);
		break;
		//-----------------------------
		// Fixed Line Length processing
		// After many failed attempts
		// to do this, I finally
		// that the best way to proceed
		// was to draw a right triangle
		// with a known Hypotenuse
		// so the proceedure will be to
		// select the point where the
		// right angle is, the second
		// point will be the BASE of
		// triangle.
		//-----------------------------
	case ObjectDrawState::START_DRAWING_LINE_FIXED_LEN:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		//-------------------------------------------------
		// Add an object to represent the point that
		// defines where the right angle is
		//-------------------------------------------------
		Corner.pCadPoint = new CCadPoint;
		Corner.pCadPoint->SetSubType(SubType::RIGHTANGLE_VERTEX);
		Corner.pCadPoint->SetOrigin(GetOrigin());
		Corner.pCadPoint->SetParent(this);
		AddObjectAtChildTail(Corner.pCadObject);
		//---------------------------------
		CopyAttributesFrom(&m_CurrentAttributes);
		GetAttributes().m_LockLength = TRUE;
		//----- Remember Cursor position
		GETVIEW->GetCursorPosition(&pointSaved);
		Id = EditProperties();
		if (Id == IDOK)
		{
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
			DrawState = ObjectDrawState::FIXED_LINE_RIGHTANGLE_MOUSE_DOWN;
			GETAPP.UpdateStatusBar(_T("Line:Place Right Angle Point"));
		}
		else
		{
			DrawState = ObjectDrawState::END_DRAWING;
			GETAPP.UpdateStatusBar(_T(""));
		}
		//---- Restore Cursor position
		GETVIEW->SetCursorPosition(pointSaved);
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::FIXED_LINE_RIGHTANGLE_MOUSE_DOWN:
		DrawState = ObjectDrawState::FIXED_LINE_RIGHTANGLE_MOUSE_UP;
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::FIXED_LINE_RIGHTANGLE_MOUSE_UP:
		Corner.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::RIGHTANGLE_VERTEX,
			0
		);
		Corner.pCadPoint->SetPoint(MousePos);
		GETAPP.UpdateStatusBar(_T("Line:First Point"));
		DrawState = ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_DOWN;
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_DOWN:
		DrawState = ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_UP;
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_UP:
		ObjP1.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		ObjP1.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Second Point"));
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN:
		DrawState = ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_UP;
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_UP:
		Corner.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::RIGHTANGLE_VERTEX,
			0
		);
		ObjP1.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		ObjP2.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			2
		);
		//-------------------------------------------------
		// What we do now is calculate the 
		// position of P2 from thehypoteneus, which is the
		// legnth of the fixed line, and the distance from
		// the point where the right angle is, to P1
		// from the position
		//-------------------------------------------------
		CalcFixedPoint(
			MousePos,
			Corner.pCadPoint,
			ObjP1.pCadPoint,
			ObjP2.pCadPoint
		);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		pNewLine = new CCadLine;
		pNewLine->Create(NULL, GETVIEW->GetDocument()->GetCurrentOrigin());
		pNewLine->SetLength(GetLength());
		GETVIEW->SetObjectTypes(pNewLine);
		DrawState = ObjectDrawState::START_DRAWING_LINE_FIXED_LEN;
		GETAPP.UpdateStatusBar(_T("Fixed Line:Place Set up Next Line"));
		SetCurrentDrawState(DrawState);
		break;
	}
	return DrawState;
}

ObjectDrawState CCadLine::MouseMove(ObjectDrawState DrawState)
{
	//-------------------------------------------------------
	// MouseMove
	//		This is the state machine for creating this
	//	object on the screen.  This Method is for when
	//	the left mouse is moved.
	//
	//	parameters:
	//		pASV......pointer to view that is creating object
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();
	DOUBLEPOINT SecondPoint;
	CADObjectTypes ObjP1, ObjP2, ObjCorner;
	UINT KindsOfObjects = 0;
	CCadObject* pSnapedObject;

	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:	//MouseMove
		ObjP1.pCadObject = FindChildObject(
			ObjectType::POINT, 
			SubType::VERTEX, 
			1
		);
		if (GetAttributes().m_P1_SNAP_POINT)
		{
			KindsOfObjects = OBJKIND_POINT | OBJKIND_SELECT;
			pSnapedObject = GETVIEW->SnapToObject(MousePos, KindsOfObjects, this, FALSE);
			if (pSnapedObject)
			{
				//--------------------------------
				// We know where we want to go so
				// we need to do the default
				// processing for
				// WAITFORMOUSE_DOWN_LBUTTON_UP
				// and go onto
				// PLACE_LBUTTON_DOWN
				//-------------------------------
				if (pSnapedObject->GetType() == ObjectType::POINT)
				{
					CCadPoint* pCP = (CCadPoint*)pSnapedObject;
					ObjP1.pCadPoint->SetPoint(pCP->GetPoint());
					DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
					GETAPP.UpdateStatusBar(_T("Line:Place Second Popint"));
				}
			}
			else
			{
				ObjP1.pCadPoint->SetPoint(MousePos);
			}
		}
		else
			ObjP1.pCadPoint->SetPoint(MousePos);
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:	//MouseMove
		ObjP2.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			2
		);
		ObjP2.pCadPoint->SetPoint(MousePos);
		break;
		//-------------------------------------------------
		// State machine for drawing a line of fixed length
		//-------------------------------------------------
	case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_DOWN://MouseMove
		ObjP1.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		ObjP2.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			2
		);
		ObjP1.pCadPoint->SetPoint(MousePos);
		ObjP2.pCadPoint->SetPoint(MousePos);
		break;
	case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN://MouseMove
		ObjP1.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		ObjP2.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			2
		);
		ObjCorner.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::RIGHTANGLE_VERTEX,
			SUBSUBTYPE_ANY
		);
		printf("Mouse Move ------------------\n");
		ObjP1.pCadPoint->Print("IN");
		ObjP2.pCadPoint->Print("IN");

		CalcFixedPoint(
			MousePos, 
			ObjCorner.pCadPoint, 
			ObjP1.pCadPoint, 
			ObjP2.pCadPoint
		);
		ObjP1.pCadPoint->Print("OUT");
		ObjP2.pCadPoint->Print("OUT");
		printf("Mouse Move ^^^^^^^^^^^^^^^^^^\n");
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}


BOOL CCadLine::CalcFixedPoint(
	DOUBLEPOINT MousePos,	//current mopuse position
	CCadPoint* pPtRtAgl,	//where the right angle is
	CCadPoint* pPtP1,		// P1 of the Line
	CCadPoint* pPtP2		// P2 of the Line
)
{
	//------------------------------------------
	// CalcFixedPoint
	// Calculate P2
	// 1. Does P1 == RtAngle , then rotate around P1
	// 2. else
	// 3. Slope of R1->P1
	// 4. Slope Orthoganol to R1-> P1
	// 5. Length of R1->P2
	// 6. Point on Line R1->P2 intersected by P1->P2
	//
	//------------------------------------------
	BOOL rV = TRUE;
	DOUBLEPOINT PtP2;
	double b;
	double a;
	double m, mOrth;
	double y, Y, yIntercept;
	BOOL bHorizontalSlope;
	BOOL bVerticalSlope;

	if (*pPtRtAgl == *pPtP1)
	{
		//-----------------------
		// Simple case
		//-----------------------
		printf("Simple Case\n");
	}
	else
	{
		a = pPtRtAgl->DistanceTo(pPtP1);
		b = m_Length *m_Length - a * a;
		printf("Fixed Line a=%7.4lf  b=%7.4lf\n", a, b);
		if (b < 0)	//this is an error
		{
			//------------------------------
			// Invalid
			//-------------------------------
			rV = FALSE;
			printf("Invalid Case\n");
		}
		else
		{
			//-------------------------------
			// Find P2
			//-------------------------------
			b = sqrt(b);
			printf("Fixed Line a=%7.4lf  b=%7.4lf\n", a, b);
			bHorizontalSlope = pPtP1->OrthogonalSlope(&mOrth, pPtRtAgl);
			bVerticalSlope = pPtP1->Slope(&m, pPtRtAgl);
			if (!bVerticalSlope && bHorizontalSlope)
			{
				//Horizontal Slop
				printf("Horizontal Slope Case\n");
				if (MousePos.dY > pPtRtAgl->GetY())
				{
					pPtP2->SetX(pPtRtAgl->GetX());
					pPtP2->SetY(pPtRtAgl->GetY() + b);
				}
				else
				{
					pPtP2->SetX(pPtRtAgl->GetX());
					pPtP2->SetY(pPtRtAgl->GetY() - b);
				}

			}
			else if (bVerticalSlope)
			{
				// Vertical Slope
				printf("Vertical Slope Case\n");
			}
			else
			{
				printf("Any Other Case\n");
				//somewhre in between slope
				pPtP2->PointOnLineAtDistance(pPtRtAgl, mOrth, m_Length);
				pPtP2->Slope(&m, pPtP1);
				yIntercept =  pPtP1->YIntercept(m);
				y = m * MousePos.dX + yIntercept;
				if (MousePos.dY - y > 0.0)
					pPtP2->Reflect(pPtRtAgl);
			}
		}
	}
	return rV;
}

void CCadLine::ProcessZoom(CScale& InchesPerPixel)
{
	//-------------------------------------
	// ProcessZoom
	// Makes changes nessesary when the
	// drawing is zoomed, in this case,
	// recalculate the enclosing
	// rectangle
	// 
	// To find this rectangle, each end
	// point of the line will be in the
	// center of rectangle's sides that
	// contain those endpoints.
	// 
	//-------------------------------------
	CDoubleSize Inches, szRect;
	CADObjectTypes ObjRect, ObjP1, ObjP2;
	CDoubleSize rectSize;
	double dist;
	double m1;
	CCadPoint p1,p2;

	p1.Create(NULL, NULL);
	p2.Create(NULL,NULL);
	//--------------------------------------
	// Get the objects that define the
	// Enclosing rectangle
	//--------------------------------------
	ObjRect.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
	ObjP1.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
	ObjP2.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 2);
	//-------------------------------------
	// Figure out how big the rectangle
	// needs to be.  It needs to be at
	// least 10 pixels, so that is what
	// I am going to use
	//-------------------------------------
	Inches = CDoubleSize(10.0,10.0) * InchesPerPixel;
	dist = Inches.Magnitude();
	ObjP1.pCadPoint->Slope(&m1, ObjP2.pCadPoint);
	//-------------------------------------
	// Convert 10 x 10 pixels into inches
	//-------------------------------------
	p1.PointOnLineAtDistance(DOUBLEPOINT(*ObjP1.pCadPoint), m1, dist);
	//-------------------------------------
	// Calculate size of rectangle
	//-------------------------------------
	szRect = Inches * 2.0 + (*ObjP2.pCadPoint - *ObjP1.pCadPoint);
	//-------------------------------------
	// Calculate rotation point
	//-------------------------------------
	p2.PointOnLineAtDistance(DOUBLEPOINT(p1), m1, szRect.Magnitude());
	//------------------------------------
	// Update enclosing rectangle
	//------------------------------------
	ObjRect.pCadRect->SetPoints(szRect,DOUBLEPOINT(p1),DOUBLEPOINT(p2));
}

int CCadLine::EditProperties(void)
{
	CDlgLineAttributes Dlg;
	int Id;

	Dlg.SetLine(this);
	Id = Dlg.DoModal();
	return Id;
}

void CCadLine::CreateThePen(MODE mode, CPen* pen, int Lw)
{
	switch (mode.DrawMode)
	{
	case ObjectDrawMode::FINAL:
		if (IsSelected())
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
		else
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
		break;
	case ObjectDrawMode::SKETCH:
		pen->CreatePen(PS_DOT, Lw, GetAttributes().m_colorSelected);
		break;
	}
}
