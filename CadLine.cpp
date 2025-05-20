#include "pch.h"

CCadLine::CCadLine():CCadObject(ObjectType::LINE)
{
	m_Length = 0.0;
	GetName().Format(_T("Line_%d"), ++m_LineCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetLineAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadLine::CCadLine(CCadLine &line) :CCadObject(ObjectType::LINE)
{
	CopyAttributesFrom(&line.GetAttributes());
	CCadPoint* pCP = (CCadPoint*)line.GetHead();
	while (pCP)
	{
		AddObjectAtTail(pCP->Copy());
		pCP = (CCadPoint * )pCP->GetNext();
	}
}

CCadLine::~CCadLine()
{
}

BOOL CCadLine::Create(CCadObject* pParent, SubTypes type)
{
	CCadPoint* pPoint;
	CCadPolygon* pPolygon;

	CCadObject::Create(pParent, type);
	if (pParent == NULL)
		pParent = this;
	pPoint = new CCadPoint;
	pPoint->Create(pParent, CCadObject::SubTypes::VERTEX);
	pPoint->SetSubSubType(1);
	AddObjectAtTail(pPoint);
	pPoint = new CCadPoint;
	pPoint->Create(pParent, CCadObject::SubTypes::VERTEX);
	pPoint->SetSubSubType(2);
	AddObjectAtTail(pPoint);
	pPolygon = new CCadPolygon;
	pPolygon->Create(pParent, CCadObject::SubTypes::POLY_ENCLOSE_OBJ);
	pPolygon->AddVertices(4, CCadObject::SubTypes::POLY_ENCLOSE_OBJ);
	AddObjectAtTail(pPolygon);
	if (type == SubTypes::LINE_FIXED_LEN_HYPOTENUS)
	{
		pPoint = new CCadPoint;
		pPoint->Create(pParent, CCadObject::SubTypes::RIGHTANGLE_VERTEX);
		AddObjectAtTail(pPoint);
	}
	m_Length = 0.0;
	return TRUE;
}

void CCadLine::UpdateEnclosure()
{
	CCadPolygon* pPoly;
	double Slope = 0.0, OrthogonalSlope = 0.0;
	CCadPoint* P1, * P2;
	CCadPoint E[4];	// enclosing points

	pPoly = (CCadPolygon*)FindObject(CCadObject::ObjectType::POLYGON, CCadObject::SubTypes::ENCLOSING_SHAPE, 0);
	if (pPoly)
	{
		P1 = (CCadPoint*)FindObject(CCadObject::ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
		P2 = (CCadPoint*)FindObject(CCadObject::ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
		P1->Slope(&Slope, P2);
		P1->OrthogonalSlope(&OrthogonalSlope, P2);
	}
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


void CCadLine::Save(
	CFile* pcfFile,
	int Indent
)
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
	char* s1 = new char[256];
	char* s2 = new char[2556];
	CCadObject* pObjectToSave;
	CString csOut;
	int n;

	GETAPP.IndentString(pIndent, 256, Indent, ' ');
	n = sprintf_s(
		s1,
		256,
		"%s%s(%s,%d){\n",
		pIndent,
		CLexer::TokenLookup(Token::LINE),
		CCadObject::GetSubTypeString(s2,GetSubType()),
		GetSubSubType()
	);
	pcfFile->Write(s1, n);
	pObjectToSave = FindObject(ObjectType::POINT, SubTypes::VERTEX, 1);
	if (pObjectToSave)
		pObjectToSave->Save(pcfFile,Indent);
	pObjectToSave = FindObject(ObjectType::POINT, SubTypes::VERTEX, 2);
	if (pObjectToSave)
		pObjectToSave->Save(pcfFile, Indent);
	pObjectToSave = FindObject(ObjectType::RECT, SubTypes::ARC_RECTSHAPE, 0);
	if (pObjectToSave)
		pObjectToSave->Save(pcfFile, Indent);
	GetAttributes().Save(pcfFile, Indent + 2);
	n = sprintf_s(s1, 256, "%s}\n", pIndent);
	pcfFile->Write(s1, n);
	delete[] pIndent;
}


void CCadLine::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
			pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
			pObj.pCadPoint->MoveTo(pDC, LLHC, Scale);
			pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
			pObj.pCadPoint->LineTo(pDC, LLHC, Scale);
			break;
		case MODE::ObjectPaintMode::SKETCH:
			switch (GetCurrentDrawState())
			{
			case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
				pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RIGHTANGLE_VERTEX, SUBSUBTYPE_ANY);
				pObj.pCadPoint->MoveTo(pDC, LLHC, Scale);
				pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
				pObj.pCadPoint->LineTo(pDC, LLHC, Scale);
				break;
			case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_DOWN:
				pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RIGHTANGLE_VERTEX, SUBSUBTYPE_ANY);
				pObj.pCadPoint->MoveTo(pDC, LLHC, Scale);
				pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
				pObj.pCadPoint->LineTo(pDC, LLHC, Scale);
				break;
			case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN:
				pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::RIGHTANGLE_VERTEX, SUBSUBTYPE_ANY);
				pObj.pCadPoint->MoveTo(pDC, LLHC, Scale);
				pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
				pObj.pCadPoint->LineTo(pDC, LLHC, Scale);
				break; //TODO Huh
			case ObjectDrawState::PLACE_LBUTTON_DOWN:
				pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
				pObj.pCadPoint->MoveTo(pDC, LLHC, Scale);
				pObj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
				pObj.pCadPoint->LineTo(pDC, LLHC, Scale);
				break;
			}//end of switch draw state
			break;
		}	//end of switch(mode)
		pDC->SelectObject(pOld);
		//-------------------------------------
		// Draw the Children
		//-------------------------------------
		pObj.pCadObject = GetHead();
		while (pObj.pCadObject)
		{
			if (pObj.pCadObject->GetType() == ObjectType::POINT)
			{
				pObj.pCadObject->Draw(pDC, mode, LLHC, Scale);
			}
			pObj.pCadObject = pObj.pCadObject->GetNext();
		}
	}	//end of if(rederEnabled)
}

BOOL CCadLine::IsPointEnclosed(DOUBLEPOINT p)
{
	BOOL rV = FALSE;;

	return rV;
}

BOOL CCadLine::PointInThisObject(DOUBLEPOINT point)
{
	BOOL rV = FALSE;
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::ARC_RECTSHAPE, 0);
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
	//	n...........Total number of spaces in selection list
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


CString& CCadLine::GetTypeString()
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
		CCadObject::SubTypes::VERTEX,
		1
	);
	P1 = DOUBLEPOINT(*pPoint);
	pPoint = (CCadPoint*)FindObject(
		ObjectType::POINT,
		CCadObject::SubTypes::VERTEX,
		2
	);
	P2 = DOUBLEPOINT(*pPoint);
	GetDescription().Format(_T("LINE((%6.3lf,%6.3lf),(%6.3lf,%6.3lf))"), 
		P1.dX, P1.dY,
		P2.dX, P2.dY
	);
	return GetDescription();
}

CCadObject * CCadLine::Copy()
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
	newObj.pCadLine->CopyAttributesFrom(GetPtrToAttributes());
	newObj.pCadLine->m_Length = m_Length;
	return newObj.pCadObject;
}

void CCadLine::CopyAttributes(CCadObject* pToObj)
{
	((CCadLine*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}


CDoubleSize CCadLine::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Returns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	DOUBLEPOINT P1, P2;
	CCadPoint* pPoint;

	pPoint = (CCadPoint*)FindObject(
		ObjectType::POINT,
		CCadObject::SubTypes::VERTEX,
		1
	);
	P1 = DOUBLEPOINT(*pPoint);
	pPoint = (CCadPoint*)FindObject(
		ObjectType::POINT,
		CCadObject::SubTypes::VERTEX,
		2
	);
	P2 = DOUBLEPOINT(*pPoint);
	return CDoubleSize(fabs(P1.dX - P2.dX), fabs(P1.dY - P2.dY));
}

void CCadLine::Parse(
	CParser* pParser,	// pointer to parser
	Token TypeToken // Token type to save object as
)
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
	pParser->Expect(TypeToken);
	pParser->Expect(Token('('));
//	pLex->Point(Token::POINT, m_Line.GetPoint(LinePoint::P1), Token);
	pParser->Expect(Token(','));
//	pLex->Point(Token::POINT, m_Line.GetPoint(LinePoint::P2), Token);
	pParser->Expect(Token(')'));
	GetAttributes().Parse(pParser);
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
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			1
		);
		ObjP1.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Place Second Point"));
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		GETVIEW->EnableAutoScroll(FALSE);
		ObjP2.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			2
		);
		ObjP2.pCadPoint->SetPoint(MousePos);
		GetParent()->AddObjectAtTail(this);
		pNewLine = new CCadLine;
		pNewLine->Create(GetParent(), GetSubType());
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
		// so the procedure will be to
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
		Corner.pCadPoint->Create(this, CCadObject::SubTypes::RIGHTANGLE_VERTEX);
		AddObjectAtTail(Corner.pCadObject);
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
			GETAPP.UpdateStatusBar(_T("Fixed Len Line:Place Right Angle Point"));
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
		Corner.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::RIGHTANGLE_VERTEX,
			SUBSUBTYPE_ANY
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
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			1
		);
		ObjP1.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN;
		GETAPP.UpdateStatusBar(_T("Fixed Len Line:Second Point"));
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN:
		DrawState = ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_UP;
		SetCurrentDrawState(DrawState);
		break;
	case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_UP:
		Corner.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::RIGHTANGLE_VERTEX,
			SUBSUBTYPE_ANY
		);
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			1
		);
		ObjP2.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			2
		);
		//-------------------------------------------------
		// What we do now is calculate the 
		// position of P2 from the hypotenuse, which is the
		// length of the fixed line, and the distance from
		// the point where the right angle is, to P1
		// from the position
		//-------------------------------------------------
		CalcFixedPoint(
			MousePos,
			Corner.pCadPoint,
			ObjP1.pCadPoint,
			ObjP2.pCadPoint
		);
		GetParent()->AddObjectAtTail(this);
		pNewLine = new CCadLine;
		pNewLine->Create(GetParent(), GetSubType());
		Corner.pCadPoint = new CCadPoint;
		Corner.pCadPoint->Create(pNewLine, CCadObject::SubTypes::RIGHTANGLE_VERTEX);
		pNewLine->AddObjectAtTail(Corner.pCadObject);
		pNewLine->SetLength(GetLength());
		GETVIEW->SetObjectTypes(pNewLine);

		GETVIEW->GetCursorPosition(&pointSaved);
		Id = pNewLine->EditProperties();
		if (Id == IDOK)
		{
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
			DrawState = ObjectDrawState::FIXED_LINE_RIGHTANGLE_MOUSE_DOWN;
			GETAPP.UpdateStatusBar(_T("Fixed Len Line:Place Right Angle Point"));
		}
		else
		{
			//------- End Drawing ------
			if (m_AttributesDirty)
			{
				Id = GETVIEW->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
				if (IDYES == Id)
				{
					m_CurrentAttributes.CopyTo(&m_LastAttributes);
				}
				m_AttributesDirty = FALSE;
			}
			if (GETVIEW->IsAutoScrollEnabled())
				GETVIEW->EnableAutoScroll(FALSE);
			DrawState = ObjectDrawState::NULL_STATE;
			GETAPP.UpdateStatusBar(_T(""));
		}
		//---- Restore Cursor position
		GETVIEW->SetCursorPosition(pointSaved);


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
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT, 
			CCadObject::SubTypes::VERTEX, 
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
					GETAPP.UpdateStatusBar(_T("Line:Place Second Point"));
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
		ObjP2.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			2
		);
		ObjP2.pCadPoint->SetPoint(MousePos);
		break;
		//-------------------------------------------------
		// State machine for drawing a line of fixed length
		//-------------------------------------------------
	case ObjectDrawState::FIXED_LINE_FIRST_POINT_MOUSE_DOWN://MouseMove
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			1
		);
		ObjP2.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			2
		);
		ObjP1.pCadPoint->SetPoint(MousePos);
		ObjP2.pCadPoint->SetPoint(MousePos);
		break;
	case ObjectDrawState::FIXED_LINE_SECOND_POINT_MOUSE_DOWN://MouseMove
		ObjP1.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			1
		);
		ObjP2.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			2
		);
		ObjCorner.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::RIGHTANGLE_VERTEX,
			SUBSUBTYPE_ANY
		);
		printf("Mouse Move ------------------\n");
//		ObjP1.pCadPoint->Print("IN");
//		ObjP2.pCadPoint->Print("IN");

		CalcFixedPoint(
			MousePos, 
			ObjCorner.pCadPoint, 
			ObjP1.pCadPoint, 
			ObjP2.pCadPoint
		);
//		ObjP1.pCadPoint->Print("OUT");
//		ObjP2.pCadPoint->Print("OUT");
		printf("Mouse Move ^^^^^^^^^^^^^^^^^^\n");
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}

BOOL CCadLine::CalcFixedPoint(
	DOUBLEPOINT MousePos,	//current mouse position
	CCadPoint* pPtRtAgl,	//where the right angle is
	CCadPoint* pPtP1,		// P1 of the Line
	CCadPoint* pPtP2		// P2 of the Line
) const
{
	//------------------------------------------
	// CalcFixedPoint
	// Calculate P2
	// 1. Does P1 == RtAngle , then rotate around P1
	// 2. else
	// 3. Slope of R1->P1
	// 4. Slope Orthogonal to R1-> P1
	// 5. Length of R1->P2
	// 6. Point on Line R1->P2 intersected by P1->P2
	//
	//------------------------------------------
	BOOL rV = TRUE;
	double b;
	double a;
	double SideSlope, LineSlope;
	double yIntercept, x;
	UINT SlopeType;

	if (*pPtRtAgl == *pPtP1)
	{
		//-----------------------
		// Simple case
		//-----------------------
		pPtP2->PointOnLineAtDistance(pPtP1, MousePos, m_Length);
	}
	else
	{
		a = pPtRtAgl->DistanceTo(pPtP1);
		b = m_Length *m_Length - a * a;
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
//			printf("Fixed Line a=%7.4lf  b=%7.4lf\n", a, b);
			SlopeType = pPtP1->OrthogonalSlope(&SideSlope, pPtRtAgl);
			switch (SlopeType)
			{
			case SLOPE_HORIZONTAL:
				printf("Horizontal\n");
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
				break;
			case SLOPE_VERTICAL:
				// Vertical Slope
				if (MousePos.dX > pPtRtAgl->GetX())
				{
					pPtP2->SetX(pPtRtAgl->GetX() + b);
					pPtP2->SetY(pPtRtAgl->GetY());
				}
				else
				{
					pPtP2->SetX(pPtRtAgl->GetX() - b);
					pPtP2->SetY(pPtRtAgl->GetY());
				}
				break;
			case SLOPE_NOT_ORTHOGONAL:
				//--------------------------------------
				//somewhere in between slope
				//--------------------------------------
				// Find Slope of the line
				//--------------------------------------
				pPtP2->PointOnLineAtDistance(pPtRtAgl, SideSlope, b);
				pPtP2->Slope(&LineSlope, pPtP1);
				yIntercept = pPtP1->YIntercept(LineSlope);
				x = (MousePos.dY - yIntercept) / LineSlope;
				if (MousePos.dX - x > 0.0)
					pPtP2->Reflect(pPtRtAgl, POINT_REFLECT_BOTH);
				break;
			}
		}
	}
	return rV;
}

void CCadLine::ProcessZoom(CScale& InchesPerPixel)
{
	//-------------------------------------
	// ProcessZoom
	// Makes changes necessary when the
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

	p1.Create(this, CCadObject::SubTypes::DEFAULT);
	p2.Create(this, CCadObject::SubTypes::DEFAULT);
	//--------------------------------------
	// Get the objects that define the
	// Enclosing rectangle
	//--------------------------------------
	ObjRect.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::ARC_RECTSHAPE, 0);
	ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
	ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
	//-------------------------------------
	// Figure out how big the rectangle
	// needs to be.  It needs to be at
	// least 10 pixels, so that is what
	// I am going to use
	//-------------------------------------
	Inches = CDoubleSize(10.0,10.0) * InchesPerPixel;
	dist = Inches.Magnitude();
	ObjP1.pCadPoint->Slope(&m1, ObjP2.pCadPoint);

	p1.PointOnLineAtDistance(ObjP1.pCadPoint, m1, dist);
	p2.PointOnLineAtDistance(ObjP2.pCadPoint, m1, dist);
	//------------------------------------
	// Update enclosing rectangle
	//------------------------------------
	((CCadPoint*)ObjRect.pCadRect->GetVertex(1))->SetPoint(&p1);
	((CCadPoint*)ObjRect.pCadRect->GetVertex(2))->SetPoint(p1.Reflect(ObjP1.pCadPoint, POINT_REFLECT_BOTH));
	((CCadPoint*)ObjRect.pCadRect->GetVertex(3))->SetPoint(&p2);
	((CCadPoint*)ObjRect.pCadRect->GetVertex(4))->SetPoint(p2.Reflect(ObjP2.pCadPoint, POINT_REFLECT_BOTH));
}

int CCadLine::EditProperties()
{
	CDlgLineAttributes Dlg;
	int Id;

	Dlg.SetLine(this);
	Id = Dlg.DoModal();
	return Id;
}

void CCadLine::CreateThePen(MODE mode, CPen* pen, int Lw)
{
	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if (IsSelected())
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
		else
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
		break;
	case MODE::ObjectPaintMode::SKETCH:
		pen->CreatePen(PS_DOT, Lw, GetAttributes().m_colorSelected);
		break;
	}
}
