//-----------------------------------------
// CCadPolygon.cpp
//
// This is one of my oldest drawing objects
// It dates to the Risk Game Map Editor
// I wrote a while back.  Its name has
// changed, but some of the kludgey code that
// was in the original is still here.
// Well, even less now.
//------------------------------------------

#include "pch.h"

using namespace std;

CCadPolygon::CCadPolygon():CCadObject(ObjectType::POLYGON)
{
	SetSelected(0);	//initial not selected
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

BOOL CCadPolygon::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, type);
	if (pParent == NULL)
		pParent = this;
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::CENTERPOINT);
	AddObjectAtTail(Obj.pCadObject);
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

void CCadPolygon::Save(
	CFile* pcfFile,
	int Indent, 
	int flags
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
	char* IndentString = new char[256];
	CString csOut;
	UINT i;
	CCadPoint* pPoint;

	csOut.Format( _T("%hs %hs %hs %d"), 
		 "junk", "junk","junk",5
//		GETAPP.IndentString(IndentString, 256, Indent, ' '),
//		CFileParser::TokenLookup(TOKEN_POLY),
//		CFileParser::TokenLookup(TOKEN_VERTEX),
//		GetNumVertices()
	);


	for (i = 0; i< GetNumVertices(); ++i)
	{
		pPoint = (CCadPoint*)FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			i + 1
		);
		pPoint->Save(pcfFile, Indent + 1, flags);
	}
	m_Attrib.Save(pcfFile, Indent + 1, flags);
	delete[] IndentString;
}

BOOL CCadPolygon::DrawPolygon(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
{
	CPoint* pPointArray;
	BOOL bFirstPoint = TRUE;
	int VertexCount = 0;
	BOOL rV = TRUE;
	int i;
	int n;

	n = GetNumVertices();
	pPointArray = new CPoint[n];
	for (i = 0; i < n; ++i)
	{
		pPointArray[i] = ((CCadPoint*)(FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			i + 1)))->ToPixelPoint(LLHC, Scale);
	}
	pDC->Polygon(pPointArray, GetNumVertices());
	return rV;
}

CCadPoint* CCadPolygon::CalculateCenterPoint()
{
	CADObjectTypes Obj;
	DOUBLEPOINT* pPolyPoints;
	DOUBLEPOINT Center;

	pPolyPoints = new DOUBLEPOINT[GetNumVertices()];
	GetPoints(&pPolyPoints);
	Center = GETAPP.GetPolygonCenter(pPolyPoints, m_NumVertices - 1);
	Center.Print("Poly Center");
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, SUBSUBTYPE_ANY);
	Obj.pCadPoint->SetPoint(Center);
	delete[] pPolyPoints; 
	return Obj.pCadPoint;
}

CCadPoint *CCadPolygon::GetCenter()
{
	double x = 0.0, y = 0.0;
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(
		ObjectType::POINT,
		CCadObject::SubTypes::CENTERPOINT,
		SUBSUBTYPE_ANY
	);
	if (Obj.pCadPoint->GetX() == 0.0 && Obj.pCadPoint->GetY() == 0.0)
		CalculateCenterPoint();
	return Obj.pCadPoint;;
}


void CCadPolygon::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
		case MODE::ObjectPaintMode::SKETCH:
			DrawPolygon(pDC, mode, LLHC, Scale);
			break;
		}
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
}

BOOL CCadPolygon::GetPoints(DOUBLEPOINT** ppPolyPoints)
{
	BOOL rV = FALSE;
	int n = GetNumVertices();
	int i;
	CADObjectTypes Obj;

	for (i = 0; i < n; ++i)
	{
		Obj.pCadObject = FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
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

BOOL CCadPolygon::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

BOOL CCadPolygon::PointInThisObject(DOUBLEPOINT point)
{
	DOUBLEPOINT* pPolyPoints;
	int n = GetNumVertices();
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

CCadObject * CCadPolygon::Copy()
{
	//---------------------------------------------------
	// Copy
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadPolygon *pP = new CCadPolygon;
	pP->Create(GetParent(), GetSubType());
	CCadObject::CopyObject(pP);
	return pP;
}

void CCadPolygon::CopyAttributes(CCadObject* pToObj)
{
	((CCadPolygon*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}

BOOL CCadPolygon::GetMinMaxXY(double& MinX, double& MaxX, double& MinY, double& MaxY)
{
	UINT i, n;
	CCadPoint* pPoint;
	BOOL rV = FALSE;
	double X, Y;

	n = GetNumVertices();
	for (i = 0; i < n; ++i)
	{
		pPoint = (CCadPoint*)FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
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
	//	Get the size of the object.  Returns the size
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

int CCadPolygon::Parse(
	CFile* pcfInFile,
	int Token,	// Lookahead Token
	CFileParser* pParser,	// pointer to parser
	int TypeToken// Token type to save object as
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
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, m_NumVertices);
		Obj.pCadPoint->SetPoint(MousePos);
		AddPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		if ((m_FirstPoint == MousePos) && (GetNumVertices() > 3))	//is figure closed?
		{
			CCadPolygon* pPoly;
			//------
			//done
			//-----
			GETVIEW->EnableAutoScroll(FALSE);
			GetParent()->AddObjectAtTail(this);
			pPoly = new CCadPolygon;
			pPoly->Create(GetParent(), GetSubType());
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
			Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, m_NumVertices);
			Obj.pCadPoint->SetPoint(MousePos);
			if (GetNumVertices() > 3)
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

	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, m_NumVertices);
	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		Obj.pCadPoint->SetPoint(MousePos);
		if (GetNumVertices() > 3)
			CalculateCenterPoint();
		GETVIEW->Invalidate();
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		Obj.pCadPoint->SetPoint(MousePos);
		if (GetNumVertices() > 3)
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
	pPoint->Create(this, CCadObject::SubTypes::VERTEX);
	pPoint->SetSubSubType(++m_NumVertices);
	pPoint->SetPoint(newPoint);
	AddObjectAtTail(pPoint);
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

	pPolyPoints = new DOUBLEPOINT[GetNumVertices()];
	GetPoints(&pPolyPoints);
	rV = GETAPP.PtEnclosedInPolygon(point, pPolyPoints, GetNumVertices());
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

	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
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
	case MODE::ObjectPaintMode::SKETCH:
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
	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
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
	case MODE::ObjectPaintMode::SKETCH:
		rV = GetAttributes().m_colorFill;
		brushFill->CreateSolidBrush(rV);
		break;
	}
	return rV;
}
