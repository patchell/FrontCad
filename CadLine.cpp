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
	CCadPoint* pCP = (CCadPoint*)line.GetHead();
	while (pCP)
	{
		AddObjectAtHead(pCP);
		pCP = (CCadPoint * )pCP->GetNext();
	}
	SetType(ObjectType::LINE);
}

CCadLine::~CCadLine()
{
}

BOOL CCadLine::Create()
{
	CCadPoint* pPoint;
	CCadRect* pRect;

	pPoint = new CCadPoint;
	pPoint->Create();
	pPoint->SetSubType(SubType::VERTEX);
	pPoint->SetSubSubType(1);
	AddObjectAtTail(pPoint);
	pPoint = new CCadPoint;
	pPoint->Create();
	pPoint->SetSubType(SubType::VERTEX);
	pPoint->SetSubSubType(2);
	AddObjectAtTail(pPoint);
	pRect = new CCadRect;
	pRect->Create();
	pRect->SetSubType(SubType::RECTSHAPE);
	AddObjectAtTail(pRect);
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
		((CCadPoint*)GetHead())->GetX(),
		((CCadPoint*)GetHead())->GetY(),
		CLexer::TokenToString(DocFileParseToken::POINT),
		((CCadPoint*)GetHead()->GetNext())->GetX(),
		((CCadPoint*)GetHead()->GetNext())->GetY()
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

	if (IsRenderEnabled())
	{
		Lw = int(Scale.m_ScaleX * GetLineWidth());
		if (Lw < 1) Lw = 1;
		switch (mode.DrawMode)
		{
			case ObjectDrawMode::FINAL:
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
				pOld = pDC->SelectObject(&penLine);
				if (GetHead())
				{
					((CCadPoint*)GetHead())->MoveTo(pDC, ULHC, Scale);
					((CCadPoint*)GetHead()->GetNext())->LineTo(pDC, ULHC, Scale);
				}
				pDC->SelectObject(pOld);
				break;
			case ObjectDrawMode::SELECTED:
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
				pOld = pDC->SelectObject(&penLine);
				if (GetHead())
				{
					((CCadPoint*)GetHead())->MoveTo(pDC, ULHC, Scale);
					((CCadPoint*)GetHead()->GetNext())->LineTo(pDC, ULHC, Scale);
				}
				pDC->SelectObject(pOld);
				break;
			case ObjectDrawMode::SKETCH:
				penLine.CreatePen(PS_DOT, 1, GetAttributes().m_colorSelected);
				pOld = pDC->SelectObject(&penLine);
				if (GetHead())
				{
					((CCadPoint*)GetHead())->MoveTo(pDC, ULHC, Scale);
					((CCadPoint*)GetHead()->GetNext())->LineTo(pDC, ULHC, Scale);
				}
				pDC->SelectObject(pOld);
				break;
		}	//end of switch(mode)
	}	//end of if(rederEnabled)
}

BOOL CCadLine::PointInThisObject(DOUBLEPOINT point)
{
	BOOL rV = FALSE;
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
	rV = Obj.pCadRect->PointInThisObject(point);
	return rV;
}

int CCadLine::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject** ppSelList,
	int index,
	int n
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
	int ix;

	if (index < n)
	{
		//---------------------------------------
		// is point in the Arc?
		//---------------------------------------
		if (PointInThisObject(p))
		{
			ppSelList[index++] = this;
			ix = CCadObject::PointInObjectAndSelect(
				p,
				ppSelList,
				index,
				n
			);
			index += ix;
		}
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

	pPoint = (CCadPoint*)FindObject(
		ObjectType::POINT,
		SubType::VERTEX,
		1
	);
	P1 = DOUBLEPOINT(*pPoint);
	pPoint = (CCadPoint*)FindObject(
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

	pPoint = (CCadPoint*)FindObject(
		ObjectType::POINT,
		SubType::VERTEX,
		1
	);
	P1 = DOUBLEPOINT(*pPoint);
	pPoint = (CCadPoint*)FindObject(
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
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		ObjP1.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Place Second Popint"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		GETVIEW->EnableAutoScroll(FALSE);
		ObjP2.pCadObject = FindObject(
			ObjectType::POINT,
			SubType::VERTEX,
			2
		);
		ObjP2.pCadPoint->SetPoint(MousePos);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		pNewLine = new CCadLine;
		pNewLine->Create();
		GETVIEW->SetObjectTypes(pNewLine);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
		GETVIEW->Invalidate();
		break;
		//-----------------------------
		// Fixed Line Length processing
		//-----------------------------
	case ObjectDrawState::START_DRAWING_LINE_FIXED_LEN:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		GetAttributes().m_LockLength = TRUE;
		Id = EditProperties();
		if (Id == IDOK)
		{
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
			DrawState = ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_DOWN;
			GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
		}
		else
		{
			DrawState = ObjectDrawState::END_DRAWING;
			GETAPP.UpdateStatusBar(_T(""));
		}
		break;
	case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_DOWN:
		DrawState = ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_UP;
		break;
	case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_UP:
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		ObjP1.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN;
		m_SavedSnapEnable = GETVIEW->GetGrid().EnableSnap(FALSE);
		GETAPP.UpdateStatusBar(_T("Line:Place Second Point"));
		break;
	case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN:
		DrawState = ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_UP;
		break;
	case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_UP:
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		ObjP2.pCadObject = FindObject(
			ObjectType::POINT,
			SubType::VERTEX,
			2
		);
		ObjP2.pCadPoint->PointOnLineAtDistance(ObjP1.pCadPoint, MousePos, m_Length);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		pNewLine = new CCadLine;
		pNewLine->Create();
		GETVIEW->SetObjectTypes(pNewLine);
		DrawState = ObjectDrawState::START_DRAWING_LINE_FIXED_LEN;
		GETVIEW->GetGrid().EnableSnap(m_SavedSnapEnable);
		GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
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
	CADObjectTypes ObjP1, ObjP2;

	switch (DrawState)
	{
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		ObjP2.pCadObject = FindObject(
			ObjectType::POINT,
			SubType::VERTEX,
			2
		);
		ObjP2.pCadPoint->SetPoint(MousePos);
		break;
	case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN:
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		ObjP2.pCadObject = FindObject(
			ObjectType::POINT,
			SubType::VERTEX,
			2
		);
		ObjP1.pCadPoint->Print("P1->::");
		ObjP2.pCadPoint->PointOnLineAtDistance(ObjP1.pCadPoint, MousePos, m_Length);
		ObjP2.pCadPoint->Print("Move Mouse To:Line");
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
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

	p1.Create();
	p2.Create();
	//--------------------------------------
	// Get the objects that define the
	// Enclosing rectangle
	//--------------------------------------
	ObjRect.pCadObject = FindObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
	ObjP1.pCadObject = FindObject(ObjectType::POINT, SubType::VERTEX, 1);
	ObjP2.pCadObject = FindObject(ObjectType::POINT, SubType::VERTEX, 2);
	//-------------------------------------
	// Figure out how big the rectangle
	// needs to be.  It needs to be at
	// least 10 pixels, so that is what
	// I am going to use
	//-------------------------------------
	Inches = CDoubleSize(10.0,10.0) * InchesPerPixel;
	dist = Inches.Magnitude();
	m1 = ObjP1.pCadPoint->Slope(ObjP2.pCadPoint);
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

DOUBLEPOINT CCadLine::SnapToObuject(DOUBLEPOINT MousePos, ObjectDrawState DrawState)
{
	//-------------------------------------------
	// SnapToObuject
	//	We check to see if the mouse cursor is
	// near an object that this particular wants
	// to snap to.
	//-------------------------------------------
	DOUBLEPOINT Result;

	Result = MousePos;
	return Result;
}

int CCadLine::EditProperties(void)
{
	CDlgLineAttributes Dlg;
	int Id;

	Dlg.SetLine(this);
	Id = Dlg.DoModal();
	return Id;
}
