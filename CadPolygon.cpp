//-----------------------------------------
// CCadPolygon.cpp
//
// This is one of my oldest drawing objects
// It dates to the Risk Game Map Editor
// I wrote a while back.  Its name has
// changed, but some of the klugy code that
// was in the original is still here.
//------------------------------------------

#include "pch.h"


CCadPolygon::CCadPolygon():CCadObject()
{
	m_PolyID = GETAPP.GetUniqueID();;
	SetSelected(0);	//initial not selected
	m_pPenLine = 0;
	SetType(ObjectType::POLYGON);
	++m_PolygonCount;
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetPolygonAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadPolygon::CCadPolygon(int nVertex) : CCadObject()
{
	m_PolyID = GETAPP.GetUniqueID();
	SetSelected(0);	//initial not selected
	m_pPenLine = 0;
	SetType(ObjectType::POLYGON);
	++m_PolygonCount;
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		GetAttributes().CopyFrom(GETAPP.GetPolygonAttributes());
	}
}

CCadPolygon::CCadPolygon(CCadPolygon &v):CCadObject()
{
	m_PolyID = GETAPP.GetUniqueID();
	m_pPenLine = 0;
	m_Poly.Copy(v.GetPoly());
	SetType(ObjectType::POLYGON);
	++m_PolygonCount;
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		GetAttributes().CopyFrom(GETAPP.GetPolygonAttributes());
	}
}

CCadPolygon::~CCadPolygon()
{
	m_Poly.Destroy();
	if (m_pPenLine) delete m_pPenLine;
}

void CCadPolygon::Move(CDoubleSize Diff)
{
	//***************************************************
	//	Move
	//		This Method is used to move the object
	// by the amount that is passed.
	//
	// parameters:
	//	p.......amount to move the object by
	//
	// return value: none
	//--------------------------------------------------
	UINT i;
	for (i = 0; i<m_Poly.GetSize(); ++i)
		m_Poly.GetPoints()[i] += Diff;
}

void CCadPolygon::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
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
		m_Poly.GetSize());

	UINT i;
	for (i = 0; i<m_Poly.GetSize(); ++i)
	{
		m_Poly.GetPoints()[i].Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	}
	m_Attrib.Save(pO, Indent + 1, flags);
	delete[] String;
}

void CCadPolygon::SetVertex(int v, CDoubleSize sz)
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
	m_Poly.SetPoint(v, m_Poly.GetPoint(v) + sz);
}

int CCadPolygon::GrabPoint(CDoublePoint  point)
{
	//***************************************************
	// GrabPoint
	//	This Method checks for a vertex at point p
	//
	// parameters:
	//	point.....point to check for presence of a vertex
	//	scale....scale factor
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	int loop, rV = -1;;
	CDoublePoint p;
	UINT i;

	CScale Scale = GETVIEW()->GetGrid().GetInchesPerPixel();
	double Inches = 20.0 * Scale.GetScaleX();
	for (i = 0, loop = 1; (i<m_Poly.GetSize()) && loop; ++i)
	{
		p = m_Poly.GetPoints()[i];
		if (p.IsPointOnTarget(point))
		{
			rV = i;
			loop = 0;
		}
	}
	return rV;
}

void CCadPolygon::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
{
	//***************************************************
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

	if (CCadPolygon::m_RenderEnable)
	{
		CPen *oldpen;
		int Lw;	//line width in pixels
		if ((Lw = int(Scale.m_ScaleX * m_Attrib.m_LineWidth)) < 1) Lw = 1;
		int i = 0;

		if (!IsLastModeSame(mode) || IsDirty())
		{
			if (m_pPenLine) delete m_pPenLine;
			switch (mode.DrawMode)
			{
			case ObjectDrawMode::FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw, m_Attrib.m_colorLine);
				break;
			case ObjectDrawMode::SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(200, 50, 50));
				mode.LinesMode = SelectedLinesMode::BOTH;
				mode.PointsMode = SelectedPointsMode::POINT_FILLED_RECT;
				break;
			case ObjectDrawMode::SKETCH:
				m_pPenLine = new CPen(PS_DOT, 1, m_Attrib.m_colorLine);
				mode.PointsMode = SelectedPointsMode::POINT_FILLED_RECT;
				break;
			}
			SetDirty(0);
		}

		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			oldpen = pDC->SelectObject(m_pPenLine);
			m_Poly.Draw(pDC, mode, Offset, Scale);
			m_Poly.Fill(pDC, mode, Offset, Scale, m_Attrib.m_colorFill);
			pDC->SelectObject(oldpen);
			SetLastMode(mode);
			break;
		case ObjectDrawMode::SELECTED:
			oldpen = pDC->SelectObject(m_pPenLine);
			mode.LinesMode = SelectedLinesMode::HIGHLIGHT;
			mode.PointsMode = SelectedPointsMode::POINT_FILLED_RECT;
			m_Poly.Draw(pDC, mode, Offset, Scale);
			pDC->SelectObject(oldpen);
			SetLastMode(mode);
			break;
		case ObjectDrawMode::SKETCH:
			oldpen = pDC->SelectObject(m_pPenLine);
			m_Poly.Draw(pDC, mode, Offset, Scale);
			SetLastMode(mode);
			break;
		}
	}
}

int CCadPolygon::PointInObjectAndSelect(CDoublePoint p, CCadObject ** ppSelList , int index, int n, DrawingCheckSelectFlags flag)
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
	//	returns ??
	//--------------------------------------------------
	if (index < n || n == 0)
	{
		//---------------------------------------
		// is point in Ellipse
		//---------------------------------------
		if (PointEnclosed(p))
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

CDoublePoint CCadPolygon::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_Poly.GetPoints()[0];
}


void CCadPolygon::AdjustReference(CDoubleSize Ref)
{
	//***************************************************
	// AdjustReference
	//	Change the reference point for an object.  This
	// operation needs to change everything else that
	// is referenced to this ppoint as well.
	// parameters:
	//	Ref.......How much to change reference by
	//
	// return value:
	//--------------------------------------------------
	UINT i;
	for (i = 0; i<m_Poly.GetSize(); ++i)
		m_Poly.SetPoint(i, m_Poly.GetPoint(i) - Ref);
}

using namespace std;

CDoubleRect& CCadPolygon::GetRect(CDoubleRect& rect)
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
	double MaxX = -numeric_limits<double>::infinity(), MinX = numeric_limits<double>::infinity();
	double MaxY = -numeric_limits<double>::infinity(), MinY = numeric_limits<double>::infinity();
	UINT i;

	//Find the rectangle that encloses the figure
	for (i = 0; i < m_Poly.GetSize(); ++i)
	{
		if (MaxX < m_Poly.GetPoints()[i].dX) MaxX = m_Poly.GetPoints()[i].dX;
		if (MinX > m_Poly.GetPoints()[i].dX) MinX = m_Poly.GetPoints()[i].dX;
		if (MaxY < m_Poly.GetPoints()[i].dY) MaxY = m_Poly.GetPoints()[i].dY;
		if (MinY > m_Poly.GetPoints()[i].dY) MinY = m_Poly.GetPoints()[i].dY;
	}
	// Create a rectangle
	rect = CDoubleRect(CDoublePoint(MinX, MinY), CDoublePoint(MaxX, MaxY));
	return rect;
}

CString& CCadPolygon::GetTypeString()
{
	//***************************************************
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

CCadPolygon CCadPolygon::operator=(CCadPolygon &v)
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
	CCadPolygon PolyResult;

	PolyResult.Copy(&v);
	return PolyResult;
}

CCadObject * CCadPolygon::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadPolygon *pP = new CCadPolygon;
	pP->GetAttributes().CopyFrom(&m_Attrib);
	pP->GetPoly()->Copy(&m_Poly);
	return pP;
}

void CCadPolygon::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//	None
	//--------------------------------------------------
}

void CCadPolygon::RenderEnable(int e)
{
	//***************************************************
	// RenderEnable
	//	chhanges the state of the render enable flag.
	// The base class does not contain this flag.
	// The render enable flag is a static member of
	// the derived class.
	// parameters:
	//	e......new state of enable flag
	//
	// return value:
	//--------------------------------------------------
	CCadPolygon::m_RenderEnable = e;
}

CDoublePoint CCadPolygon::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	return GetPoly()->GetCenter();
}

void CCadPolygon::ChangeCenter(CSize p)
{
	//***************************************************
	// ChangeCenter
	//	Change the center position of the object
	// parameters:
	//	p......amount to change center by
	//
	// return value:
	//--------------------------------------------------
}

CDoubleSize& CCadPolygon::GetSize(CDoubleSize& size)
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CDoubleRect rect = GetRect(rect);
	size = rect.GetSize(size);
	return size;
}

void CCadPolygon::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object Not Implemented
	// for Polygons
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
}

DocFileParseToken CCadPolygon::Parse(
	DocFileParseToken Token, 
	CLexer *pLex, 
	DocFileParseToken TypeToken 
)
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		GETVIEW()->EnableAutoScroll(TRUE);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Polygon:Place First Point"));
		break;
	case ObjectDrawState::END_DRAWING:
		Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
		if (IDYES == Id)
		{
			m_CurrentAttributes.CopyTo(&m_LastAttributes);
		}
		GETVIEW()->EnableAutoScroll(FALSE);
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		Id = EditProperties();
		if (IDOK == Id)
		{
			CopyAttributesTo(&m_CurrentAttributes);
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		SetCurrentVertex(MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		AddPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		SetCurrentVertex(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		if ((m_Poly.GetPoint(0) == MousePos) && m_Poly.GetPointCount())	//is figure closed?
		{
			//------
			//done
			//-----
			GETVIEW()->AddObjectAtFrontIntoDoc(this);
			GETVIEW()->SetObjectTypes(new CCadPolygon);
			DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
			GETAPP.UpdateStatusBar(_T("Polygon:Place First Point"));
			GETVIEW()->Invalidate();
		}
		else
		{
			//-----------------------------------
			// Keep Looping and adding points
			//----------------------------------
			AddPoint(MousePos);
			GETAPP.UpdateStatusBar(_T("Polygon:Place Next Point :End by Placing on First Point"));
			GETVIEW()->Invalidate();
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();
	switch (DrawState)
	{
		case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
			SetCurrentVertex(MousePos);
			GETVIEW()->Invalidate();
			break;
			case ObjectDrawState::PLACE_LBUTTON_DOWN:
			SetCurrentVertex(MousePos);
			GETVIEW()->Invalidate();
			break;
	}
	return DrawState;
}

void CCadPolygon::Reset(void)
{
	m_Poly.Destroy();
	SetSelected(0);	//initial not selected
}

BOOL CCadPolygon::CompareToLast(CDoublePoint nP)
{
	/*********************************
	** CompareToLast
	**	This Method compares a point
	** to the last point that was added
	**to a polygon.
	**
	** parameter:
	**	nP.....point to compare
	**
	** REtuns: TRUE if equal
	**	       FALSE if not equal.
	*********************************/
	BOOL rV = FALSE;
	if (nP == m_Poly.GetLastPoint())
		rV = TRUE;
	return rV;
}

UINT CCadPolygon::DeleteLastPoint(void)
{
	/**********************************
	** DeleteLastPoint
	**	Deletes last point added to
	** polygon.
	** RETURNS:new vertex count.
	**********************************/
	return m_Poly.DeleteLastPoint();
}

int CCadPolygon::GetCount(void)
{
	/***********************************
	** GetCount
	**	Retturns the number of points
	** (verticies) in the polygon
	***********************************/
	return m_Poly.GetSize();
}

BOOL CCadPolygon::AddPoint(CDoublePoint newPoint)
{
	/*************************************
	** AddPoint
	**
	** Adds a new vertex to the polygon.
	**
	** parameters:
	**	newPoint....point of the new vertex.
	** Return: TRUE on success
	*************************************/
	return m_Poly.AddPoint(newPoint);
}

BOOL CCadPolygon::PointEnclosed(CDoublePoint point)
{
	/*****************************************
	** PointEnclosed
	**	This Method determines if a point
	** is enclosed within a polygon.
	**
	** parameters:
	**	point....point to test
	** Returns: TRUE if point inside
	**          FALSE if point is outside
	*****************************************/
	return m_Poly.PointInPolygon(point);
}

void CCadPolygon::CopyAttributesFrom(SPolyAttributes* pAttrib)
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

void CCadPolygon::CopyAttributesTo(SPolyAttributes* pAttrib)
{
	/***************************************************
	*	CopyAttributesTo
	*		This Method is used to copy the
	*	attributes from this object into
	*	an external attributes stucture
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyTo(pAttrib);
}

void CCadPolygon::Flip(CDoublePoint Pivot, Axis Direction)
{
	//-------------------------------------------------
	// Flip
	//		Flips the object along an axis
	//
	// parameters:
	//	Origin.........specifies either x or y axis
	//	Direction......0=X Axis, 1 = Y Axis
	//-------------------------------------------------
	UINT i;

	for (i = 0; i < m_Poly.GetSize(); ++i)
		m_Poly.GetPoint(i).Flip(Pivot, Direction);
}

BOOL CCadPolygon::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadPolygon::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

int CCadPolygon::EditProperties()
{
	int Id;
	CDlgPolygonProperties Dlg;

	Dlg.SetPolygon(this);
	Id = Dlg.DoModal();
	return Id;
}