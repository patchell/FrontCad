#include "pch.h"

CCadArcCent::CCadArcCent():CCadObject()
{
	SetType(ObjectType::ARCCENTERED);
	m_pPenLine = 0;
	m_pointStart = { 0.0,0.0 };
	m_pointEnd = { 0.0,0.0 };
	GetName().Format(_T("ARCCENTER_%d"), ++m_ArcCentCount);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetArcCenterAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadArcCent::CCadArcCent(CCadArcCent &arc) : CCadObject()
{
	GetAttributes().CopyFrom(arc.GetAttributesPointer());
	m_pPenLine = 0;
	m_pointStart = arc.GetStartPoint();
	m_pointEnd = arc.GetEndPoint();
	CCadArcCent::m_ArcCentCount++;
}

CCadArcCent::~CCadArcCent()
{
	if (m_pPenLine) delete m_pPenLine;
}

void CCadArcCent::Create(void)
{
}


void CCadArcCent::Move(CDoubleSize Diff)
{
	//***************************************************
	//	Move
	//		This Method is used to move the object
	// by the point that is passed.
	//
	// parameters:
	//	p.......point to move the object by
	//
	// return value: none
	//--------------------------------------------------
	SetCenter(GetCenter() + Diff);
	m_rectShape.AdjustReference(Diff);
	SetStartPoint(GetStartPoint() + Diff);
	SetEndPoint(GetEndPoint() + Diff);
}

void CCadArcCent::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* TempString = new char[256];
	char* IndentString = new char[256];

	GETAPP.MkIndentString(IndentString, Indent);
	fprintf(pO, "%s%s(\n",
		IndentString,
		CLexer::TokenToString(DocFileParseToken::ARCCENTERED)
	);
	m_rectShape.Save(pO, DocFileParseToken::SHAPE, Indent + 1, flags);
	GetCenter().Save(pO, DocFileParseToken::CENTER, Indent + 1, flags);
	GetStartPoint().Save(pO, DocFileParseToken::START, Indent + 1, flags);
	GetEndPoint().Save(pO, DocFileParseToken::END, Indent + 1, flags);
	GetAttributes().Save(pO, Indent + 1, flags);
	delete[] TempString;
}

void CCadArcCent::SetVertex(int v, CPoint p)
{
	//***************************************************
	// SetVertex
	//	This Method is used to change the position of
	// a vertex.
	//
	// parameters:
	// v......index of the vertex
	// p......Amnount to change the vertex by
	//
	// return value: none
	//--------------------------------------------------
}


int CCadArcCent::GrabPoint(CDoublePoint p)
{
	//***************************************************
	// GrabPoint
	//	This Method checks for a vertex at point p
	//
	// parameters:
	//	p.....point to check for presence of a vertex
	//	scale....scale factor
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	return -1;
}


void CCadArcCent::Draw(CDC* pDC, MODE mode, CDoublePoint ULHC, CScale Scale)
{
	//***************************************************
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	Offset..Offset to draw objects at
	//	Scale..Scale factor to draw objects in Pixels/Inch
	//
	// return value:none
	//--------------------------------------------------
	CPen *pOld;
	CRect rect;
//	CSize rectLWcomp;
	CPoint p1, p2;
	CPoint Start, End,Center;
	int Lw;

	if (CCadArcCent::IsRenderEnabled())
	{
		rect = m_rectShape.ToCRect(ULHC, Scale);
		Start = m_pointStart.ToPixelPoint(ULHC, Scale);
		End = m_pointEnd.ToPixelPoint(ULHC, Scale);
		Center = GetCenter().ToPixelPoint(ULHC, Scale);
		if ((!IsLastModeSame(mode)) || IsDirty())
		{
			Lw = int(round(GetAttributes().m_LineWidth * Scale.GetScaleX()));
			if (Lw < 1)
				Lw = 1;
			if (m_pPenLine) delete m_pPenLine;
			switch (mode.DrawMode)
			{
			case ObjectDrawMode::FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw, GetLineColor());
				break;
			case ObjectDrawMode::SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(200, 50, 50));
				break;
			case ObjectDrawMode::SKETCH:
				m_pPenLine = new CPen(PS_DOT, 1, GetLineColor());
				break;
			case ObjectDrawMode::ARCSTART:
			case ObjectDrawMode::ARCEND:
				m_pPenLine = new CPen(PS_DOT, 1, GetLineColor());
				break;
			}
			SetDirty(FALSE);
		}
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			pDC->SelectObject(pOld);
			break;
		case ObjectDrawMode::SELECTED:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			pDC->SelectObject(pOld);
			break;
		case ObjectDrawMode::SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOld);
			break;
		case ObjectDrawMode::ARCSTART:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Ellipse(&rect);
			pDC->MoveTo(Center);
			pDC->LineTo(Start);
			pDC->SelectObject(pOld);
			break;
		case ObjectDrawMode::ARCEND:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			pDC->MoveTo(Center);
			pDC->LineTo(End);
			pDC->SelectObject(pOld);
			break;
		}
		SetLastMode(mode);
	}
}

BOOL CCadArcCent::PtInArc(CDoublePoint Point)
{
	//-----------------------------------------------------
	//	PtInArc
	// Determine if the point is near (on) the arc that is
	// of interest
	//
	// parameter:
	//	p.......point of interest
	//
	// Returns:
	//	TRUE if point is on arc
	//-----------------------------------------------------
	double StartAngle, EndAngle, Angle;
	BOOL rV = FALSE;
	CDoublePoint Center;

	//-------------------------------
	// do a rough test to cull
	// this object out
	//------------------------------
	if (m_rectShape.PointInRectangle(Point))
	{
		//-------------------------------------
		// first, check to see if the angle
		// formed by p is between the start
		// and end angle
		//-------------------------------------
		Center = m_rectShape.GetCenter(Center);
		Angle = GETAPP.ArcTan(Point.dX - Center.dX, Point.dY - Center.dY);
		StartAngle = GETAPP.ArcTan(
			m_pointStart.dX - Center.dX,
			m_pointStart.dY - Center.dY
		);
		EndAngle = GETAPP.ArcTan(
			m_pointEnd.dX - Center.dX,
			m_pointEnd.dY - Center.dY
		);
		if (Angle > StartAngle && Angle < EndAngle)
		{
			//----------------------------------------
			// check to see if the point is within
			// the radius of the arc
			//----------------------------------------
			rV = GETAPP.TestEllipsePoint(
				GetA(), 
				GetB(), 
				Point, 
				GetCenter(), 
				TOLERANCE_10_PERCENT
			);
		}
	}
	return rV;
}

double CCadArcCent::Evaluate(CDoublePoint p)
{
	CDoublePoint Center;
	double x, y,r;
	double A, B;

	A = GetA() * GetA();
	B = GetB() * GetB();
	Center = GetCenter();
	x = p.dX - Center.dX;
	x *= x;
	y = p.dY - Center.dY;
	y *= y;
	r = x / A + y / B;
	return r;
}

int CCadArcCent::EditProperties()
{
	return 0;
}

BOOL CCadArcCent::PointInObjectAndSelect(CDoublePoint p, CCadObject ** ppSelList , int index, int n, DrawingCheckSelectFlags flag)
{
	//***************************************************
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
	//	flag........Determines what sort of objects selected
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------
	if (index < n || n == 0)
	{
		//---------------------------------------
		// is point in Ellipse
		//---------------------------------------
		if (PtInArc(p))
		{
			if (ppSelList)
			{
				switch (flag)
				{
				case DrawingCheckSelectFlags::FLAG_ALL:
					ppSelList[index++] = this;
					break;
				case DrawingCheckSelectFlags::FLAG_UNSEL:
					if (!IsSelected())
						ppSelList[index++] = this;
					break;
				case DrawingCheckSelectFlags::FLAG_SEL:
					if (IsSelected())
						ppSelList[index++] = this;
					break;
				}
			}
			else
			{
				switch (flag)
				{
				case DrawingCheckSelectFlags::FLAG_ALL:
					index = 1;
					break;
				case DrawingCheckSelectFlags::FLAG_UNSEL:
					if (!IsSelected())
						index = 1;
					break;
				case DrawingCheckSelectFlags::FLAG_SEL:
					if (IsSelected())
						index = 1;
					break;
				}

			}
		}
	}
	return index;
}

CDoublePoint CCadArcCent::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return GetCenter();
}


CDoubleRect& CCadArcCent::GetRect(CDoubleRect& rect)
{
	//***************************************************
	// GetRect
	//	Returns the rectangle that will enclose the
	// the object
	// parameters:
	//
	// return value:Returns the rectangle that encloses
	// the object
	//--------------------------------------------------
	rect = m_rectShape;
	return rect;
}

CString& CCadArcCent::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("Arc Centered"));
	return csTypeName;
}

CCadArcCent CCadArcCent::operator=(CCadArcCent &v)
{
	//***************************************************
	// operator=
	//		Provides the Methodality when one object
	// value is assigned to another
	// parameters:
	//	v......reference to object to get value(s) from
	//
	// return value:this
	//--------------------------------------------------
	CCadArcCent Result;
	Result.SetCenter(v.GetCenter());
	Result.SetStartPoint(v.GetStartPoint());
	Result.SetEndPoint(v.GetEndPoint());
	Result.m_rectShape = v.GetRect(m_rectShape);
	return Result;
}

CCadObject * CCadArcCent::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadArcCent *pArc = new CCadArcCent;
	*pArc = *this;
	return pArc;
}

void CCadArcCent::Copy(CCadObject* pObj)
{
	if (pObj->GetType() == ObjectType::ARCCENTERED)
	{
		CCadArcCent* pAC = ((CCadArcCent*)pObj);
		GetAttributes().CopyFrom(pAC->GetAttributesPointer());
		SetStartPoint(pAC->GetStartPoint());
		SetEndPoint(pAC->GetEndPoint());
		SetCenter(pAC->GetCenter());
	}
}
	

CDoubleSize& CCadArcCent::GetSize(CDoubleSize& size)
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	size = m_rectShape.GetSize(size);
	return size;
}

DocFileParseToken CCadArcCent::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
{
	//***************************************************
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
	Token = pLex->Accept(Token, DocFileParseToken::ARCCENTERED);
	Token = pLex->Accept(Token, DocFileParseToken('('));
	Token = m_rectShape.Parse(Token, pLex, DocFileParseToken::SHAPE);
	Token = pLex->Point(DocFileParseToken::POINT, m_pointStart, Token);
	Token = pLex->Accept(Token, DocFileParseToken(','));
	Token = pLex->Point(DocFileParseToken::POINT, m_pointEnd, Token);
	Token = pLex->Accept(Token, DocFileParseToken(')'));
	Token = GetAttributes().Parse(Token, pLex);
	return Token;
}

void CCadArcCent::CopyAttributesTo(SArcCenterAttributes *pAttrib)
{
	/***************************************************
	*	CopyAttributesTo
	*		This Method is used to copy the
	*	attributes from this object into one pointed
	*	to by the parameter.
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy into
	***************************************************/
	GetAttributes().CopyTo(pAttrib);
}	

void CCadArcCent::CopyAttributesFrom(SArcCenterAttributes*pAttrib)
{
	/***************************************************
	*	CopyAttributesFrom
	*		This Method is used to copy the
	*	attributes pointed to by the parameter into
	*	this object
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyFrom(pAttrib);
	ClearNeedsAttributes();

}

BOOL CCadArcCent::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadArcCent::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

ObjectDrawState CCadArcCent::ProcessDrawMode(ObjectDrawState DrawState)
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC CENTER:Place Center Point of Circle Shape"));
		break;
	case ObjectDrawState::END_DRAWING:
		if (m_AttributesDirty)
		{
			Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
			if (IDYES == Id)
			{
				m_CurrentAttributes.CopyTo(&m_LastAttributes);
			}
			m_AttributesDirty = FALSE;
		}
		GETAPP.UpdateStatusBar(_T(""));
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
		GETVIEW()->EnableAutoScroll(TRUE);
		SetCenter(MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		SetCenter(MousePos);
		GetShapeRect().SetPointsFromCenter(GetCenter(), MousePos, MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC CENTERED:Place Corner of Circle Shape"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_rectShape.SetPointsFromCenter(GetCenter(), MousePos, MousePos);
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		GetShapeRect().SetPointsFromCenter(GetCenter(), MousePos, MousePos);
		DrawState = ObjectDrawState::ARCSTART_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC CENTERED:Locate Start of Arc"));
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		SetStartPoint(MousePos);
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_UP:
		SetStartPoint(MousePos);
		DrawState = ObjectDrawState::ARCEND_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC CENTERED:Locate End of Arc"));
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		SetEndPoint(MousePos);
		DrawState = ObjectDrawState::ARCEND_LBUTTON_UP;
		break;
	case ObjectDrawState::ARCEND_LBUTTON_UP:
		GETVIEW()->EnableAutoScroll(FALSE);
		SetEndPoint(MousePos);
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadArcCent);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC CENTERED:Locate Center Point of Arc"));
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}


ObjectDrawState CCadArcCent::MouseMove(ObjectDrawState DrawState)
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::SET_ATTRIBUTES:
		GetShapeRect().SetPointsFromCenter(MousePos, MousePos, MousePos);
		SetCenter(MousePos);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		//------------------------------------------------
		// move the center point around
		//------------------------------------------------
		GetShapeRect().SetPointsFromCenter(MousePos, MousePos, MousePos);
		SetCenter(MousePos);
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		GetCenter().Print();
		m_rectShape.SetPointsFromCenter(GetCenter(), MousePos, MousePos);
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		SetStartPoint(MousePos);
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		SetEndPoint(MousePos);
		break;
	}
	GETVIEW()->Invalidate();
	return DrawState;
}


