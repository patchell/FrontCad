//-----------------------------------------
// CCadPolygon.cpp
//
// This is one of my oldest drawing objects
// It dates to the Risk Game Map Editor
// I wrote a while back.  Its name has
// changed, but some of the klugy code that
// was in the original is still here.
// Well, even less now.
//------------------------------------------

#include "pch.h"

using namespace std;

CCadPolygon::CCadPolygon():CCadObject()
{
	SetSelected(0);	//initial not selected
	SetType(ObjectType::POLYGON);
	GetName().Format(_T("Polygon_%d"), ++m_PolygonCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetPolygonAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
	m_NumVertices = 0;
}


CCadPolygon::~CCadPolygon()
{
}

BOOL CCadPolygon::Create(CCadObject* pParent, CCadObject* pOrigin, SubType type)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, pOrigin, type);
	if (pParent == NULL)
		pParent = this;
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, pOrigin, SubType::CENTERPOINT);
	AddObjectAtChildTail(Obj.pCadObject);
	AddPoint(DOUBLEPOINT(0.0, 0.0));
	return TRUE;
}

void CCadPolygon::Move(CDoubleSize Diff)
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

void CCadPolygon::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//---------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* String = new char[256];

	fprintf(pO, "%s%s(%s(%d),", 
		GETAPP.MkIndentString(String, Indent),
		CLexer::TokenToString(Token),
		CLexer::TokenToString(DocFileParseToken::VERTEX),
		GetNumVerticies());

	UINT i;
	CCadPoint* pPoint;

	for (i = 0; i< GetNumVerticies(); ++i)
	{
		pPoint = (CCadPoint*)FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			i + 1
		);
		pPoint->Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	}
	m_Attrib.Save(pO, Indent + 1, flags);
	delete[] String;
}

BOOL CCadPolygon::DrawPolygon(CDC* pDC, MODE mode, DOUBLEPOINT& ULHC, CScale& Scale)
{
	CCadPoint* pPoint, *pFirstPoint;
	BOOL bFirstPoint = TRUE;
	int VertexCount = 0;
	BOOL rV = FALSE;

	pPoint = (CCadPoint*)FindChildObject(
		ObjectType::POINT,
		SubType::VERTEX,
		++VertexCount
	);
	pFirstPoint = pPoint;
	while (pPoint)
	{
		if (bFirstPoint)
		{
			bFirstPoint = FALSE;
			pPoint->MoveTo(pDC, ULHC, Scale);
		}
		else
		{
			pPoint->LineTo(pDC, ULHC, Scale);
		}
		if (VertexCount == 3)
			rV = TRUE;
		pPoint = (CCadPoint*)FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			++VertexCount
		);
	}
	if (pFirstPoint)
		pFirstPoint->LineTo(pDC, ULHC, Scale);
	return rV;
}

CCadPoint* CCadPolygon::CalculateCenterPoint()
{
	CADObjectTypes Obj;
	DOUBLEPOINT* pPolyPoints;
	DOUBLEPOINT Center;

	pPolyPoints = new DOUBLEPOINT[GetNumVerticies()];
	GetPoints(&pPolyPoints);
	Center = GETAPP.GetPolygonCenter(pPolyPoints, m_NumVertices - 1);
	Center.Print("Poly Center");
	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, SUBSUBTYPE_ANY);
	Obj.pCadPoint->SetPoint(Center);
	delete[] pPolyPoints; 
	return Obj.pCadPoint;
}

CCadPoint *CCadPolygon::GetCenter()
{
	double x = 0.0, y = 0.0;
	CADObjectTypes Obj;

	Obj.pCadObject = FindChildObject(
		ObjectType::POINT,
		SubType::CENTERPOINT,
		SUBSUBTYPE_ANY
	);
	if (Obj.pCadPoint->GetX() == 0.0 && Obj.pCadPoint->GetY() == 0.0)
		CalculateCenterPoint();
	return Obj.pCadPoint;;
}

void CCadPolygon::FillPolygon(
	COLORREF colorBoarder,
	COLORREF colorFill,
	CDC* pDC,
	MODE mode, 
	DOUBLEPOINT& ULHC, 
	CScale& Scale
)
{
	CCadPoint* pCenter;

	if (GetNumVerticies() > 3)
	{
		pCenter = GetCenter();
		if (pCenter)
		{
			pCenter->FloodFill(colorBoarder, colorFill, pDC, ULHC, Scale);
		}
	}
}

void CCadPolygon::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& ULHC, CScale& Scale)
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
	CPen* pOldPen, penLine;
	CBrush* pOldBrush, brushFill;
	int Lw;	//line width in pixels
	COLORREF colorLine;
	COLORREF colorFill;

	if (IsRenderEnabled())
	{
		if ((Lw = GETAPP.RoundDoubleToInt(Scale.dSX * m_Attrib.m_LineWidth)) < 1)
			Lw = 1;

		colorLine = CreateThePen(mode, &penLine, Lw);
		colorFill = CreateTheBrush(mode, &brushFill);
		pOldPen = pDC->SelectObject(&penLine);
		pOldBrush = pDC->SelectObject(&brushFill);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
		case ObjectDrawMode::SKETCH:
			if (DrawPolygon(pDC, mode, ULHC, Scale))
			{
				if(!GetAttributes().m_TransparentFill)
					FillPolygon(colorLine, colorFill, pDC, mode, ULHC, Scale);
			}
			break;
		}
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
}

BOOL CCadPolygon::GetPoints(DOUBLEPOINT** ppPolyPoints)
{
	BOOL rV = FALSE;
	int n = GetNumVerticies();
	int i;
	CADObjectTypes Obj;

	for (i = 0; i < n; ++i)
	{
		Obj.pCadObject = FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			i + 1
		);
		if (Obj.pCadObject)
			(* ppPolyPoints)[i] = DOUBLEPOINT(*Obj.pCadPoint);
		else
			goto exit;
	}
	rV = TRUE;
exit:
	return rV;
}

BOOL CCadPolygon::PointInThisObject(DOUBLEPOINT point)
{
	DOUBLEPOINT* pPolyPoints;
	int n = GetNumVerticies();
	pPolyPoints = new DOUBLEPOINT[n];
	BOOL rV = FALSE;

	if (GetPoints(&pPolyPoints))
	{
		rV = GETAPP.PtEnclosedInPolygon(point, pPolyPoints, n);
	}
	delete[] pPolyPoints;
	return rV;
}

int CCadPolygon::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject *pExcludeObject,
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

using namespace std;

CString& CCadPolygon::GetTypeString()
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString Name = _T("Polygon");
	return Name;
}

CString& CCadPolygon::GetObjDescription()
{
	GetDescription().Format(_T("Polygon:%d"),GetId());
	return GetDescription();
}

CCadObject * CCadPolygon::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadPolygon *pP = new CCadPolygon;
	pP->Create(NULL, GetOrigin());
	CCadObject::CopyObject(pP);
	pP->GetAttributes().CopyFrom(GetPtrToAttributes());
	pP->m_NumVertices = GetNumVerticies();
	return pP;
}

BOOL CCadPolygon::GetMinMaxXY(double& MinX, double& MaxX, double& MinY, double& MaxY)
{
	UINT i, n;
	CCadPoint* pPoint;
	BOOL rV = FALSE;
	double X, Y;

	n = GetNumVerticies();
	for (i = 0; i < n; ++i)
	{
		pPoint = (CCadPoint*)FindChildObject(
			ObjectType::POINT,
			SubType::VERTEX,
			i + 1
		);
		if (pPoint)
		{
			X = pPoint->GetX();
			Y = pPoint->GetY();
		}
		else
		{
			goto exit;
		}
		if (MaxX < X) MaxX = X;
		if (MinX > X) MinX = X;
		if (MaxY < Y) MaxY = Y;
		if (MinY > Y) MinY = Y;
	}
	rV = TRUE;
exit:
	return rV;
}

CDoubleSize CCadPolygon::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	double MaxX = -numeric_limits<double>::infinity(), MinX = numeric_limits<double>::infinity();
	double MaxY = -numeric_limits<double>::infinity(), MinY = numeric_limits<double>::infinity();
	CDoubleSize size;

	if (GetMinMaxXY(MinX, MaxX, MinY, MaxY))
	{
		size = CDoubleSize(MaxX - MinX, MaxY - MinY);
	}
	return size;
}

DocFileParseToken CCadPolygon::Parse(
	DocFileParseToken Token, 
	CLexer *pLex, 
	DocFileParseToken TypeToken 
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
	return Token;
}

ObjectDrawState CCadPolygon::ProcessDrawMode(ObjectDrawState DrawState)
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

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Polygon:Place First Point"));
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
		Id = EditProperties();
		if (IDOK == Id)
		{
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		GETVIEW->EnableAutoScroll(TRUE);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_FirstPoint = MousePos;
		Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, m_NumVertices);
		Obj.pCadPoint->SetPoint(MousePos);
		AddPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		if ((m_FirstPoint == MousePos) && (GetNumVerticies() > 3))	//is figure closed?
		{
			CCadPolygon* pPoly;
			//------
			//done
			//-----
			GETVIEW->EnableAutoScroll(FALSE);
			GETVIEW->GetDocument()->AddObjectAtTail(this);
			pPoly = new CCadPolygon;
			pPoly->Create(NULL, GETVIEW->GetDocument()->GetCurrentOrigin());
			GETVIEW->SetObjectTypes(pPoly);
			DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
			GETAPP.UpdateStatusBar(_T("Polygon:Place First Point"));
			GETVIEW->Invalidate();
		}
		else
		{
			//-----------------------------------
			// Keep Looping and adding points
			//----------------------------------
			Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, m_NumVertices);
			Obj.pCadPoint->SetPoint(MousePos);
			if (GetNumVerticies() > 3)
				CalculateCenterPoint();
			AddPoint(MousePos);
			GETAPP.UpdateStatusBar(_T("Polygon:Place Next Point :End by Placing on First Point"));
			DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
			GETVIEW->Invalidate();
		}
		break;
	}
	return DrawState;
}

ObjectDrawState CCadPolygon::MouseMove(ObjectDrawState DrawState)
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
	CADObjectTypes Obj;

	Obj.pCadObject = FindChildObject(ObjectType::POINT, SubType::VERTEX, m_NumVertices);
	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		Obj.pCadPoint->SetPoint(MousePos);
		if (GetNumVerticies() > 3)
			CalculateCenterPoint();
		GETVIEW->Invalidate();
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		Obj.pCadPoint->SetPoint(MousePos);
		if (GetNumVerticies() > 3)
			CalculateCenterPoint();
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

CCadPoint* CCadPolygon::AddPoint(DOUBLEPOINT newPoint)
{
	//------------------------------------
	// AddPoint
	//
	// Adds a new vertex to the polygon.
	//
	// parameters:
	//	newPoint....point of the new vertex.
	// Return: TRUE on success
	//------------------------------------
	CCadPoint* pPoint;

	pPoint = new CCadPoint;
	pPoint->Create(this, GetOrigin(), SubType::VERTEX);
	pPoint->SetSubSubType(++m_NumVertices);
	pPoint->SetPoint(newPoint);
	AddObjectAtChildTail(pPoint);
	return pPoint;;
}

BOOL CCadPolygon::PointEnclosed(CCadPoint point)
{
	//----------------------------------------*
	// PointEnclosed
	//	This Method determines if a point
	// is enclosed within a polygon.
	//
	// parameters:
	//	point....point to test
	// Returns: TRUE if point inside
	//          FALSE if point is outside
	//----------------------------------------*
	DOUBLEPOINT *pPolyPoints;
	BOOL rV;

	pPolyPoints = new DOUBLEPOINT[GetNumVerticies()];
	GetPoints(&pPolyPoints);
	rV = GETAPP.PtEnclosedInPolygon(point, pPolyPoints, GetNumVerticies());
	delete[] pPolyPoints;
	return rV;
}

void CCadPolygon::CopyAttributesFrom(SPolyAttributes* pAttrib)
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

void CCadPolygon::CopyAttributesTo(SPolyAttributes* pAttrib)
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

int CCadPolygon::EditProperties()
{
	int Id;
	CDlgPolygonProperties Dlg;

	Dlg.SetPolygon(this);
	Id = Dlg.DoModal();
	return Id;
}


COLORREF CCadPolygon::CreateThePen(MODE mode, CPen* pen, int Lw)
{
	COLORREF rColor = RGB(192,192,192);

	switch (mode.DrawMode)
	{
	case ObjectDrawMode::FINAL:
		if (IsSelected())
		{
			rColor = GetAttributes().m_colorSelected;
			pen->CreatePen(PS_SOLID, Lw, rColor);
		}
		else
		{
			rColor = GetAttributes().m_colorLine;
			pen->CreatePen(PS_SOLID, Lw,rColor);
		}
		break;
	case ObjectDrawMode::SKETCH:
		rColor = GetAttributes().m_colorSelected;
		pen->CreatePen(PS_DOT, Lw, rColor);
		break;
	}
	return rColor;
}

COLORREF CCadPolygon::CreateTheBrush(MODE mode, CBrush* brushFill)
{
	COLORREF rV = RGB(0, 0, 0);
	;
	switch (mode.DrawMode)
	{
	case ObjectDrawMode::FINAL:
		if (IsSelected())
		{
			rV = GetAttributes().m_colorFill;
			brushFill->CreateSolidBrush(rV);
		}
		else
		{
			rV = GetAttributes().m_colorFill;
			brushFill->CreateSolidBrush(rV);
		}
		break;
	case ObjectDrawMode::SKETCH:
		rV = GetAttributes().m_colorFill;
		brushFill->CreateSolidBrush(rV);
		break;
	}
	return rV;
}
