#include "pch.h"

CCadRect::CCadRect():CCadObject()
{
	SetType(ObjectType::RECT);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetRectangleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
	GetName().Format(_T("Rectangle_%d"), ++CCadRect::m_RectCount);
}

CCadRect::~CCadRect()
{
	CCadObject* pObj, *pNextObj;

	pObj = GetChildrenHead();
	while (pObj)
	{
		pNextObj = pObj->GetNext();
		pObj->DeleteChildObject(pObj);
		pObj = pNextObj;
	}
}

BOOL CCadRect::Destroy(CCadObject* pDependentObject)
{
	CCadObject* pObj;
	BOOL rV = TRUE;

	if (GetDependentChildrenHead())
	{
		pObj = GetDependentChildrenHead();
		while (pObj)
		{
			//------------------------------
			// Go up the chain of dependent
			// children and prepare them
			//-----------------------------
			pObj->Destroy(this);
			pObj = DeleteChildObject(pObj);
		}
	}
	return rV;
}

BOOL CCadRect::Create(CCadObject* pParent, CCadObject* pOrigin)
{
	//---------------------------------------
	// Create
	// add initial child objects
	//---------------------------------------
	int i;
	CCadPoint* pPoint;

	CCadObject::Create(pParent, pOrigin);
	if (pParent == NULL)
		pParent = this;
	for (i = 0; i < 4; ++i)
	{
		pPoint = new CCadPoint;
		pPoint->Create(pParent, pOrigin);
		pPoint->SetSubType(SubType::VERTEX);
		pPoint->SetSubSubType(i + 1);
		AddObjectAtChildTail(pPoint);
	}
	pPoint = new CCadPoint;
	pPoint->Create(pParent, pOrigin);
	pPoint->SetSubType(SubType::CENTERPOINT);
	pPoint->SetSubSubType(0);
	AddObjectAtChildTail(pPoint);
	pPoint = new CCadPoint;
	pPoint->Create(pParent, pOrigin);
	pPoint->SetSubType(SubType::PIVOTPOINT);
	AddObjectAtChildTail(pPoint);
	return TRUE;
}

void CCadRect::SetPoints(DOUBLEPOINT P1, DOUBLEPOINT P2, DOUBLEPOINT RotDef)
{
	//----------------------------------------------
	// Set the points on a boring rectangle defined
	// by points P1 and P2
	// 
	// parameters:
	//	P1.......Upper Left hand corner
	//	P2.......Lower Right hand corner
	//----------------------------------------------
	CDoubleSize Diff;

	Diff = P2 - P1;
	SetPoints(Diff, P1, RotDef);
}

void CCadRect::SetPoints(CDoubleSize sz, DOUBLEPOINT p1, DOUBLEPOINT pointRot)
{
	//----------------------------------------
	// Calculate the points on the rectangle
	// tgat us rotated around p1 defined
	// at an angle defined by pointRot
	//
	// parameters:
	// sz........size of the rectangle
	// p1........origin of the rectangle
	// pointRot..rotation definition point
	//---------------------------------------
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4, ObjCenter;
	double m, W,H, Rise, Run;

	//--------------------------------------
	// Get the objects to set
	//--------------------------------------
	ObjP1.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
	ObjP2.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 2);
	ObjP3.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 3);
	ObjP4.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 4);
	ObjCenter.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	Run = pointRot.dX - p1.dX;	//run
	Rise = p1.dY - pointRot.dY;	//rise
	W = sz.dCX;
	H = sz.dCY;

	if (pointRot == p1)
	{
		//--------------------------------
		// No Rotation
		//--------------------------------
		ObjP1.pCadPoint->SetPoint(p1);
		ObjP2.pCadPoint->SetPoint(p1 + CDoubleSize(0.0, H));
		ObjP3.pCadPoint->SetPoint(p1 + sz);
		ObjP4.pCadPoint->SetPoint(p1 + CDoubleSize(W, 0.0));
	}
	else if (Rise == 0.0)
	{
		//--------------------------------
		// Horizontal Rectangle
		//--------------------------------
		ObjP1.pCadPoint->SetPoint(p1);
		ObjP2.pCadPoint->SetPoint(p1 + CDoubleSize(0.0, H));
		ObjP3.pCadPoint->SetPoint(p1 + sz);
		ObjP4.pCadPoint->SetPoint(p1 + CDoubleSize(W, 0.0));
	}
	else if (Run == 0.0)
	{
		//-------------------------------
		// Vertical Rectangle
		//-------------------------------
		ObjP1.pCadPoint->SetPoint(p1);
		ObjP2.pCadPoint->SetPoint(p1 + CDoubleSize(W, 0.0));
		ObjP3.pCadPoint->SetPoint(p1 + sz);
		ObjP4.pCadPoint->SetPoint(p1 + CDoubleSize(0.0, H));
	}
	else
	{
		//---------------------------------
		// Non orthogonal Rotation
		//---------------------------------
		m = Rise / Run;	//rise/run
		ObjP1.pCadPoint->SetPoint(p1);
		ObjP4.pCadPoint->PointOnLineAtDistance(ObjP1.pCadPoint, m, W);
		ObjP2.pCadPoint->PointOnLineAtDistance(ObjP1.pCadPoint, -1.0 / m, H);
		ObjP3.pCadPoint->PointOnLineAtDistance(ObjP4.pCadPoint, -1.0 / m, H);
	}
	ObjCenter.pCadPoint->SetPoint(GETAPP.CalcCenter(ObjP1.pCadPoint, ObjP3.pCadPoint));
}

CCadRect& CCadRect::SetSecondPoint(DOUBLEPOINT P2)
{
	CDoubleSize Diff;
	CADObjectTypes ObjP1;
	DOUBLEPOINT P1;

	ObjP1.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
	P1 = DOUBLEPOINT(*ObjP1.pCadPoint);
	SetPoints(P1, P2,P1);
	return *this;
}

CCadRect& CCadRect::SetPointsFromCenter(DOUBLEPOINT P1)
{
	CDoubleSize Diff;
	CADObjectTypes ObjCenter;
	DOUBLEPOINT Center;

	ObjCenter.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	Center = DOUBLEPOINT(*ObjCenter.pCadPoint);
	Diff = (Center - P1) * 2.0;

	SetPoints(Diff,P1,P1);
	return *this;
}

CCadRect& CCadRect::SetPointsFromCenter(double halfWidth, double halfHeight)
{
	CDoubleSize Diff;
	CCadPoint* pPoint;
	DOUBLEPOINT Center;

	pPoint = (CCadPoint*)FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	Center = DOUBLEPOINT(*(CCadPoint*)pPoint);
	Diff = CDoubleSize(halfWidth, halfHeight);

	pPoint = (CCadPoint*)FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
	pPoint->SetPoint(Center - Diff);
	pPoint = (CCadPoint*)FindChildObject(ObjectType::POINT, SubType::VERTEX, 2);
	pPoint->SetPoint(Center + CDoubleSize(Diff.dCX, -Diff.dCY));
	pPoint = (CCadPoint*)FindChildObject(ObjectType::POINT, SubType::VERTEX, 3);
	pPoint->SetPoint(Center + Diff);
	pPoint = (CCadPoint*)FindChildObject(ObjectType::POINT, SubType::VERTEX, 4);
	pPoint->SetPoint(Center + CDoubleSize(-Diff.dCX, Diff.dCY));
	return *this;
}

CCadRect& CCadRect::SetCenterPoint(DOUBLEPOINT newCenter)
{
	CDoubleSize Diff;
	CCadPoint* pCenter, *pPoint;
	int i;

	pCenter = (CCadPoint*)FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
	Diff = newCenter - DOUBLEPOINT(*pCenter);
	pCenter->SetPoint(newCenter);
	for (i = 0; i < 4; ++i)
	{
		pPoint = (CCadPoint*)FindChildObject(ObjectType::POINT, SubType::VERTEX, i + 1);
		pPoint->SetPoint(DOUBLEPOINT(*pPoint) + Diff);
	}
	return *this;
}

CRect CCadRect::ToCRect(DOUBLEPOINT ULHC, CScale& Scale)
{
	CRect rect;
	CADObjectTypes ObjP1, ObjP2;
	CPoint P1, P2;

	ObjP1.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
	ObjP2.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 3);
	P1 = ObjP1.pCadPoint->ToPixelPoint(ULHC, Scale);
	P2 = ObjP2.pCadPoint->ToPixelPoint(ULHC, Scale);
	rect.SetRect(P1, P2);
	rect.NormalizeRect();
	return rect;
}

void CCadRect::Move(CDoubleSize Diff)
{
	//---------------------------------------------------
	//	Move
	//		This Method is used to move the object
	// by the amount that is passed.
	//
	// parameters:
	//	p.......amount to move the object by
	//
	// return value: none
	//--------------------------------------------------
	CCadObject::Move(Diff);
}

void CCadRect::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//---------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* psIndent = new char[256];
	CADObjectTypes Obj;

	GETAPP.MkIndentString(psIndent, Indent, ' ');
	Obj.pCadObject = GetChildrenHead();
	fprintf(pO, "%s%s(\n",
		psIndent,
		CLexer::TokenToString(DocFileParseToken::RECT)
	);
	fprintf(pO, "%s%s(\n",
		psIndent,
		CLexer::TokenToString(Token)
	);
	while (Obj.pCadObject)
	{
		if (Obj.pCadObject->GetType() == ObjectType::POINT)
			Obj.pCadPoint->Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
		Obj.pCadObject = Obj.pCadObject->GetNext();
	}
	fprintf(pO, "%s)\n", psIndent);


	GetAttributes().Save(pO, Indent + 1, flags);
	fprintf(pO, "%s}\n", psIndent);
	delete []psIndent;
}

void CCadRect::DrawRect(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale, BOOL bFill)
{
	int i;
	CCadPoint* pPoint;

	for (i = 0; i < 4; ++i)
	{
		if (i == 0)
		{
			pPoint = (CCadPoint*)FindChildObject(
				ObjectType::POINT,
				SubType::VERTEX,
				i + 1);
			if (pPoint)
				pPoint->MoveTo(pDC, ULHC, Scale);
			else
				goto exit;
		}
		else
		{
			pPoint = (CCadPoint*)FindChildObject(
				ObjectType::POINT,
				SubType::VERTEX,
				i + 1
			);
			if (pPoint)
			{
				pPoint->LineTo(pDC, ULHC, Scale);
				pPoint->Draw(pDC, mode, ULHC, Scale);
			}
			else
				goto exit;
		}
		pPoint = (CCadPoint*)FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			1
		);
		if (pPoint)
		{
			pPoint->LineTo(pDC, ULHC, Scale);
			pPoint->Draw(pDC, mode, ULHC, Scale);
		}
		else
			goto exit;
	}	//end of for loop

	if (bFill)
	{
		((CCadPoint*)(FindChildObject(
			ObjectType::POINT,
			SubType::CENTERPOINT,
			SUBSUBTYPE_ANY)))->FloodFill(
				pDC,
				GetAttributes().m_colorLine,
				ULHC,
				Scale
			);
	}
exit:
	return;
}

void CCadRect::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
{
	//---------------------------------------------------
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	ULHC....Offset to draw objects at
	//	Scale..Scale factor to draw objects at
	//
	// return value:none
	//--------------------------------------------------

	if (IsRenderEnabled())
	{
		CPen* ppenOld, penLine;;
		CBrush* pbrushOld, brushFill;
		int Lw;
		CSize rectLWcomp;
		MODE pointMode = mode;;

		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.m_ScaleX);
		if (Lw < 1 || ObjectDrawMode::SKETCH == mode.DrawMode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);
		//SetRect(rect, P1, P2, rectLWcomp);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
			brushFill.CreateSolidBrush(GetAttributes().m_colorFill);
			ppenOld = pDC->SelectObject(&penLine);
			pbrushOld = pDC->SelectObject(&brushFill);
			DrawRect(pDC, mode, ULHC, Scale, GetAttributes().m_TransparentFill == FALSE);
			pDC->SelectObject(pbrushOld);
			pDC->SelectObject(ppenOld);
			break;
		case ObjectDrawMode::SELECTED:
			pointMode.PointsMode = SelectedPointsMode::POINT_BOTH_RECT_FILLED;
			penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
			ppenOld = pDC->SelectObject(&penLine);
			DrawRect(pDC, pointMode, ULHC, Scale, FALSE);
			pDC->SelectObject(ppenOld);
			break;
		case ObjectDrawMode::SKETCH:
			pointMode.PointsMode = SelectedPointsMode::POINT_BOTH_RECT_FILLED;
			penLine.CreatePen(PS_DOT, 1, GetAttributes().m_colorSketch);
			ppenOld = pDC->SelectObject(&penLine);
			DrawRect(pDC, pointMode, ULHC, Scale,FALSE);
			pDC->SelectObject(ppenOld);
			break;
		}	//end of switch draw mode
	}	//end of if render
}

CCadPoint* CCadRect::GetRectPoints(CCadPoint** ppPointDest, int n)
{
	CADObjectTypes Obj;
	int index = 0;
	BOOL loop = TRUE;

	Obj.pCadObject = GetChildrenHead();

	while (Obj.pCadObject && loop)
	{
		if (Obj.pCadObject->GetType() == ObjectType::POINT)
		{
			ppPointDest[index++] = Obj.pCadPoint;
			if (index == 4)
				loop = FALSE;
		}
		Obj.pCadObject = Obj.pCadObject->GetNext();
	}
	return ppPointDest[0];
}

BOOL CCadRect::PointInThisObject(DOUBLEPOINT point)
{
	DOUBLEPOINT pointsRect[4];
	CADObjectTypes Vertex;
	int i;
	BOOL rV = FALSE;

	for (i = 0; i < 4; ++i)
	{
		Vertex.pCadObject =FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			i + 1
		);
		if (Vertex.pCadObject)
			pointsRect[i] = DOUBLEPOINT(*Vertex.pCadPoint);
		else
			goto exit;
	}
	rV = GETAPP.PtEnclosedInPolygon(point, pointsRect, 4);
exit:
	return rV;
}

int CCadRect::PointInObjectAndSelect(
	DOUBLEPOINT p,
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
			if (IsItThisKind(nKinds))
				ppSelList[index++] = this;
		}
		index = CCadObject::PointInObjectAndSelect(
			p,
			ppSelList,
			index,
			n,
			nKinds
		);
	}
	return index;
}

CString& CCadRect::GetTypeString(void)
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Rectangle");
	return csName;
}

CString& CCadRect::GetObjDescription()
{
	CCadPoint* pP1, * pP2;
	DOUBLEPOINT P1, P2;

	pP1 = (CCadPoint*)FindChildObject(
		ObjectType::POINT,
		SubType::VERTEX,
		1
	);
	pP2 = (CCadPoint*)FindChildObject(
		ObjectType::POINT,
		SubType::VERTEX,
		3
	);
	P1 = DOUBLEPOINT(*pP1);
	P2 = DOUBLEPOINT(*pP2);
	GetDescription().Format(_T("Rect((%6.3lf,%6.3lf),(%6.3lf,%6.3lf))"),
		P1.dX,P1.dY,
		P2.dX,P2.dY
	);
	return GetDescription();
}

CCadObject * CCadRect::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy.
	// This Method DOES not make a true copy in
	// that the pointers for linked lists are set
	// to NULL
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadRect *pRect = new CCadRect;
	*pRect = *this;
	return pRect;
}

CDoubleSize CCadRect::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	
	return CDoubleSize(GetWidth(), GetHeight());
}

double CCadRect::GetWidth()
{
	double Result;
	CCadPoint* pP1, * pP2;
	CCadObject *pObj;

	pObj = GetChildrenHead();
	pP1 = (CCadPoint*)pObj->FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_UL);
	pP2 = (CCadPoint*)pObj->FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_LR);
	Result = pP2->GetX() - pP1->GetX();
	return Result;
}

void CCadRect::SetWidth(double width)
{
	CADObjectTypes Obj;
	double Xref;

	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_UL);
	Xref = Obj.pCadPoint->GetX();
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_LR);
	Obj.pCadPoint->SetX(Xref + width);
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_UR);
	Obj.pCadPoint->SetX(Xref + width);
}

double CCadRect::GetHeight()
{
	double Result;
	CCadPoint* pP1, * pP2;
	CCadObject* pObj;

	pObj = GetChildrenHead();
	pP1 = (CCadPoint*)pObj->FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_UL);
	pP2 = (CCadPoint*)pObj->FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_LR);
	Result = pP2->GetY() - pP1->GetY();
	return Result;
}

void CCadRect::SetHeight(double Height)
{
	CADObjectTypes Obj;
	double Yref;

	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_UL);
	Yref = Obj.pCadPoint->GetY();
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_LR);
	Obj.pCadPoint->SetY(Yref + Height);
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, RECT_SUBSUB_LL);
	Obj.pCadPoint->SetY(Yref + Height);
}

DocFileParseToken CCadRect::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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
	Token = pLex->Accept (Token, DocFileParseToken::RECT);
	Token = pLex->Expect(Token, DocFileParseToken('('));
	Token = pLex->CadRect(DocFileParseToken::RECT, *this, Token);
	Token = GetAttributes().Parse(Token, pLex);
	Token = pLex->Expect(Token, DocFileParseToken(')') );
	return Token;
}

void CCadRect::CopyAttributesTo(SRectAttributes *pAttrib)
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


void CCadRect::CopyAttributesFrom(SRectAttributes *pAttrib)
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
	GetAttributes().CopyFrom(pAttrib);
	SetAttributesValid();
}

ObjectDrawState CCadRect::ProcessDrawMode(ObjectDrawState DrawState)
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
	CADObjectTypes Obj;

	CPoint MouseScreenCoordinate;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Place First Point"));
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
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		GETVIEW->GetCursorPosition(&MouseScreenCoordinate);
		Id = EditProperties();
		if (IDOK == Id)
		{
			if (m_AttributesDirty)
				CopyAttributesTo(&m_CurrentAttributes);
			DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
			GETVIEW->EnableAutoScroll(1);
		}
		GETVIEW->SetCursorPosition(MouseScreenCoordinate);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		GETVIEW->EnableAutoScroll(TRUE);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, 1);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Set Second Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		SetSecondPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		SetSecondPoint(MousePos);
		if (GETVIEW->IsR_KeyDown())
		{
			//-----------------------------------
			// Rotate Rectangle
			//-----------------------------------

			DrawState = ObjectDrawState::SELECT_PIVOT_LBUTTON_DOWN;
			GETAPP.UpdateStatusBar(_T("Rectangle:Select Pivot Point"));
		}
		else
		{
			//----------------------------------------------
			// Just a regular orthoganol Rectangle
			//---------------------------------------------
			GETVIEW->GetDocument()->AddObjectAtTail(this);
			Obj.pCadRect = new CCadRect;
			Obj.pCadRect->Create(NULL, GETVIEW->GetDocument()->GetCurrentOrigin());
			GETVIEW->EnableAutoScroll(FALSE);
			GETVIEW->SetObjectTypes(Obj.pCadObject);
			DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
			GETAPP.UpdateStatusBar(_T("Rectangle:Place First Point"));
		}
		GETVIEW->Invalidate();
		break;
	case ObjectDrawState::SELECT_PIVOT_LBUTTON_DOWN:
		DrawState = ObjectDrawState::SELECT_PIVOT_LBUTTON_UP;
		break;
	case ObjectDrawState::SELECT_PIVOT_LBUTTON_UP:
		DrawState = ObjectDrawState::ROTATE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Set Angle Definition Point"));
		break;
	case ObjectDrawState::ROTATE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::ROTATE_LBUTTON_UP;
		break;
	case ObjectDrawState::ROTATE_LBUTTON_UP:
		DrawState = ObjectDrawState::SELECT_PIVOT_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Place First Point"));
		break;
	}
	return DrawState;
}

ObjectDrawState CCadRect::MouseMove(ObjectDrawState DrawState)
{
	//-------------------------------------------------------
	// MouseMove
	//		This is the state machine for creating this
	//	object on the screen.  This Method is for when
	//	the left mouse is moved.
	//
	//	parameters:
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		SetSecondPoint(MousePos);
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}

int CCadRect::EditProperties()
{
	int Id;
	CDlgRectProperties Dlg;

	Dlg.SetRectangle(this);
	Id = Dlg.DoModal();
	return Id;
}
