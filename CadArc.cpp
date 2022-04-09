#include "pch.h"

CCadArc::CCadArc():CCadObject()
{
	SetType(ObjectType::ARC);
	GetName().Format(_T("ARC_%d"), ++m_ArcCount);
	m_pPenLine = 0;
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetArcAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
	m_ArcCount++;
}

CCadArc::~CCadArc()
{
	if (m_pPenLine) delete m_pPenLine;
}

void CCadArc::Create(CRect Shape, CPoint ptStart, CPoint ptEnd)
{
}

void CCadArc::Move(CDoubleSize Diff)
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
	m_pointStart += Diff;
	m_pointStart += Diff;
}

void CCadArc::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* pIndentString1 = new char[256];

	fprintf(pO, "%s%s(%s(%8.3lf,%8.3lf),%s(%8.3lf,%8.3lf))\n",
		GETAPP.MkIndentString(pIndentString1, Indent, ' '),
		CLexer::TokenToString(DocFileParseToken::ARC),
		CLexer::TokenToString(DocFileParseToken::POINT),
		m_pointStart.dX, m_pointStart.dY,
		CLexer::TokenToString(DocFileParseToken::POINT),
		m_pointEnd.dX, m_pointEnd.dY
	);
	GetAttributes().Save(pO,Indent+1,flags);
}

void CCadArc::SetVertex(int v, CPoint p)
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


int CCadArc::GrabPoint(CDoublePoint p)
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
	int rV = -1;
	return rV;
}

void CCadArc::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
	CPen *pOld;
	CRect rect;
	CSize rectLWcomp;
	CPoint Center;
	CPoint P1, P2, Start, End;
	int Lw;

	if (CCadArc::IsRenderEnabled())
	{
		Start = m_pointStart.ToPixelPoint(Offset, Scale);
		End = m_pointEnd.ToPixelPoint(Offset, Scale);
		P1 = m_rectShape.GetPoint(RectPoint::UPPERLEFT).ToPixelPoint(Offset,Scale);
		P2 = m_rectShape.GetPoint(RectPoint::LOWERRIGHT).ToPixelPoint(Offset, Scale);
		Lw = GETAPP.RoundDoubleToInt(GetLineWidth() * Scale.m_ScaleX);
		if (Lw <= 1 || ObjectDrawMode::SKETCH == mode.DrawMode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);
		rect.SetRect(P1,P2);
		rect.DeflateRect(rectLWcomp);
		if ((!IsLastModeSame(mode)) || IsDirty())
		{
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
			SetDirty(0);
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
			Center = rect.CenterPoint();
			pDC->MoveTo(Center);
			pDC->LineTo(Start);
			pDC->SelectObject(pOld);
			break;
		case ObjectDrawMode::ARCEND:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Arc(&rect, Start, End);
			Center = rect.CenterPoint();
			pDC->MoveTo(Center);
			pDC->LineTo(End);
			pDC->SelectObject(pOld);
			break;
		}
		SetLastMode(mode);
	}
}

BOOL CCadArc::PtInArc(CDoublePoint p)
{
	BOOL rV = FALSE;
	double StartAngle;
	double EndAngle;
	double Angle;
	CDoublePoint Center;
	//------------------------------
	// See if the point is in the
	// shape rectangle
	//------------------------------
	if (m_rectShape.PointInRectangle(p))
	{
		//------------------------------
		// Is the angle between the
		// Start and End anble?
		//------------------------------
		Center = m_rectShape.GetCenter(Center);
		StartAngle = GETAPP.ArcTan((double(m_pointStart.dX) - Center.dX), -(double(m_pointStart.dY) - Center.dY));
		EndAngle = GETAPP.ArcTan((double(m_pointEnd.dX) - Center.dX), -(double(m_pointEnd.dY) - Center.dY));
		Angle = GETAPP.ArcTan(double(p.dX) - Center.dX, -(double(p.dX) - Center.dY));
		if (GETAPP.CheckAngle(StartAngle, EndAngle, Angle))
		{
			//----------------------------
			// Check to see if the radius
			// is at least sort of close
			// to the actual radius.
			//----------------------------
			// 1 = x^2/A^2 + y^2/B^2
			//----------------------------
			double A, B;
			CDoublePoint Center;

			A = m_rectShape.GetWidth()/2.0;
			B = m_rectShape.GetHeight() / 2.0;
			rV = GETAPP.TestEllipsePoint(A, B, p, m_rectShape.GetCenter(Center), TOLERANCE_10_PERCENT);
		}
	}
	return rV;
}

BOOL CCadArc::PointInObjectAndSelect(CDoublePoint p, CCadObject ** ppSelList , int index, int n, DrawingCheckSelectFlags flag)
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

CDoublePoint CCadArc::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_rectShape.GetReference();
}

void CCadArc::AdjustReference(CDoubleSize Ref)
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
	m_rectShape.AdjustReference(Ref);
	m_pointStart -= Ref;
	m_pointEnd -= Ref;
}

CDoubleRect& CCadArc::GetRect(CDoubleRect& rect)
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
	m_rectShape = rect;
	return rect;
}

CString& CCadArc::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("Arc"));
	return csTypeName;
}

CCadArc CCadArc::operator=(CCadArc &v)
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
	m_rectShape = v.GetRect(m_rectShape);
	m_pointStart = v.GetStartPoint();
	m_pointEnd = v.GetEndPoint();
	CopyAttributesFrom(&v.GetAttributes());
	return *this;
}

CCadObject * CCadArc::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadArc *pArc = new CCadArc;
	*pArc = *this;
	return pArc;
}

CDoublePoint& CCadArc::GetCenter(CDoublePoint& Center)
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	Center = m_rectShape.GetCenter(Center);
	return Center;
}

void CCadArc::ChangeCenter(CSize p)
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

CDoubleSize& CCadArc::GetSize(CDoubleSize& size)
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

void CCadArc::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
	//Cannot rotate an arc
	//--------------------------------------------------
}

DocFileParseToken CCadArc::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
{
	//--------------------------------------------------
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
	Token = pLex->Expect( Token, TypeToken);
	Token = pLex->Expect(Token, DocFileParseToken('('));
	Token = pLex->DoubleRect(DocFileParseToken::SHAPE, m_rectShape, Token);
	Token = pLex->Expect(Token, DocFileParseToken(','));
	Token = pLex->Point(DocFileParseToken::START, m_pointStart, Token);
	Token = pLex->Expect(Token, DocFileParseToken(','));
	Token = pLex->Point(DocFileParseToken::END, m_pointEnd, Token);
	Token = pLex->Expect(Token, DocFileParseToken(')'));
	Token = GetAttributes().Parse(Token, pLex);
	return Token;
}

void CCadArc::CopyAttributesTo(SArcAttributes *pAttrib)
{
	/***************************************************
	*	GetAttributes
	*		This Method is used to copy the
	*	attributes from this object into one pointed 
	*	to by the parameter.
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy into
	***************************************************/
	GetAttributes().CopyTo(pAttrib);
}

void CCadArc::CopyAttributesFrom(SArcAttributes *pAttrib)
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
	m_AttributesGood = TRUE;
}

BOOL CCadArc::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadArc::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

ObjectDrawState CCadArc::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("ARC:Place First Corner of Circle Shape"));
		break;
	case ObjectDrawState::END_DRAWING:
		Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
		if (IDYES == Id)
		{
			m_CurrentAttributes.CopyTo(&m_LastAttributes);
		}
		GETAPP.UpdateStatusBar(_T(""));
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		Id = EditProperties();
		if (IDOK == Id)
		{
			CopyAttributesTo(&m_CurrentAttributes);
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		GETVIEW()->EnableAutoScroll(TRUE);
		m_rectShape.SetPoints(MousePos, MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_rectShape.SetPoints(MousePos, MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Place Second Corner of Circle"));
		GETVIEW()->Invalidate();
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_rectShape.SetSecondPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		m_rectShape.SetSecondPoint(MousePos);
		DrawState = ObjectDrawState::ARCSTART_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC:Place Begining of ARC"));
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_DOWN:
		SetStartPoint(MousePos);
		DrawState = ObjectDrawState::ARCSTART_LBUTTON_UP;
		break;
	case ObjectDrawState::ARCSTART_LBUTTON_UP:
		SetStartPoint(MousePos);
		DrawState = ObjectDrawState::ARCEND_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC:Place End of ARC"));
		GETVIEW()->Invalidate();
		break;
	case ObjectDrawState::ARCEND_LBUTTON_DOWN:
		SetEndPoint(MousePos);
		DrawState = ObjectDrawState::ARCEND_LBUTTON_UP;
		break;
	case ObjectDrawState::ARCEND_LBUTTON_UP:
		GETVIEW()->EnableAutoScroll(FALSE);
		SetEndPoint(MousePos);
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadArc);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("ARC:Place First Corner of Circle Shape"));
		GETVIEW()->Invalidate();
		break;
	case ObjectDrawState::SECOND_POINT_LBUTTON_DOWN:
		break;
	case ObjectDrawState::SECOND_POINT_LBUTTON_UP:
		break;
	}
	return DrawState;
}

ObjectDrawState CCadArc::MouseMove(ObjectDrawState DrawState)
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
		m_rectShape.SetPoints(MousePos, MousePos);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		m_rectShape.SetPoints(MousePos, MousePos);
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_rectShape.SetSecondPoint(MousePos);
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

int CCadArc::EditProperties(void)
{
	int Id;
	CDlgArcProperties Dlg;

	Dlg.SetArc(this);
	Id = Dlg.DoModal();
	return Id;
}
