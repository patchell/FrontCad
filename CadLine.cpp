#include "pch.h"

CCadLine::CCadLine():CCadObject()
{
	m_Length = 0.0;
	m_pLinePen = 0;
	SetType(ObjectType::LINE);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetLineAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadLine::CCadLine(CCadLine &line) :CCadObject()
{
	CopyAttributesFrom(&line.GetAttributes());
	m_pLinePen = 0;
	m_Line = line.m_Line;
	SetType(ObjectType::LINE);
}

CCadLine::~CCadLine()
{
	if (m_pLinePen) delete m_pLinePen;
}

void CCadLine::OnCreate()
{
	GetName().Format(_T("Line_%d"), ++m_DocCount);
}

void CCadLine::Move(CDoubleSize Diff)
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
	m_Line.Move(Diff);
}

void CCadLine::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
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
		m_Line.dP1.dX, m_Line.dP1.dY,
		CLexer::TokenToString(DocFileParseToken::POINT),
		m_Line.dP2.dX, m_Line.dP2.dY
	);
	GetAttributes().Save(pO, Indent + 1, flags);
	delete[] pIndent;
}


void CCadLine::SetVertex(int Vi, CDoubleSize sz)
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
	if (Vi)
	{
		m_Line.dP2 += sz;
	}
	else
		m_Line.dP1 += sz;
}


int CCadLine::GrabPoint(CDoublePoint point)
{
	//***************************************************
	// GrabPoint
	//	This Method checks for a vertex at point p
	//
	// parameters:
	//	point.....point to check for presence of a vertex
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//--------------------------------------------------
	int rV = -1;
	CDoubleRect rect;
	CDoubleSize dR;
	CDoublePoint P1, P2;
	double Inches;

	Inches = GETVIEW()->GetGrid().GetInchesPerPixel().GetScaleX() * 10.0;
	dR = CDoubleSize(Inches, Inches);
	rect.SetPointsFromCenter(P1, P1 + dR, P1);
	if (rect.PointInRectangle(point))
		rV = 0;
	else
	{
		rect.SetPointsFromCenter(P2, P1 + dR, P1);
		if (rect.PointInRectangle(point))
			rV = 1;
	}
	return rV;
}


void CCadLine::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
	CPoint P1, P2, Diff;
	int Lw;
	CRect rect;

	if (CCadLine::m_RenderEnable)
	{
		P1 = CDoublePoint(m_Line.dP1).ToPixelPoint(Offset,Scale);
		P2 = CDoublePoint(m_Line.dP2).ToPixelPoint(Offset,Scale);
		Lw = int(Scale.m_ScaleX * GetLineWidth());
		if (Lw < 1) Lw = 1;
		if (!IsLastModeSame(mode) || IsDirty())
		{
			//----------------------------------
			// If mode changed, get rid of pen
			// and make a new one
			//----------------------------------
			if (m_pLinePen) delete m_pLinePen;
			switch (mode.DrawMode)
			{
				case ObjectDrawMode::FINAL:
					m_pLinePen = new CPen(PS_SOLID, Lw, GetLineColor());
					break;
				case ObjectDrawMode::SELECTED:
					m_pLinePen = new CPen(PS_SOLID, 1, GetLineColor() ^ 0x00f0f0f0);
					break;
				case ObjectDrawMode::SKETCH:
					m_pLinePen = new CPen(PS_DOT, 1, GetLineColor());
					break;
			}
			SetLastMode(mode);
			SetDirty(0);
		}
		switch (mode.DrawMode)
		{
			case ObjectDrawMode::FINAL:
				pOld = pDC->SelectObject(m_pLinePen);
				pDC->MoveTo(P1);
				pDC->LineTo(P2);
				pDC->SelectObject(pOld);
				break;
			case ObjectDrawMode::SELECTED:
				pOld = pDC->SelectObject(m_pLinePen);
				Diff = CPoint(4, 4);

				rect.SetRect(P1 + (-Diff), P1 + Diff);
				pDC->Rectangle(&rect);
				rect.SetRect(P2 + (-Diff), P2 + Diff);
				pDC->Rectangle(&rect);
				pDC->SelectObject(m_pLinePen);
				pDC->MoveTo(P1);
				pDC->LineTo(P2);
				pDC->SelectObject(pOld);
				break;
			case ObjectDrawMode::SKETCH:
				Diff = CPoint(4, 4);
				pOld = pDC->SelectObject(m_pLinePen);
				pDC->MoveTo(P1);
				pDC->LineTo(P2);
				pDC->SelectObject(pOld);
				break;
		}	//end of switch(mode)
	}	//end of if(rederEnabled)
}

CDoubleRect& CCadLine::EncloseLineInRectangle(CDoubleRect& rect, int WidthInPixels)
{
	//-----------------------------
	// enclose the line inside
	// of a rectangle
	// parameters:
	//	w.....The amount that will
	//		be added to the width
	//		of the line to make
	//		the rectangle.
	//-----------------------------
	return m_Line.EncloseLine(rect, WidthInPixels);
}

int CCadLine::PointInObjectAndSelect(
	CDoublePoint p, 
	CCadObject ** ppSelList , 
	int index, 
	int n, 
	DrawingCheckSelectFlags flag
)
{
	//***************************************************
	// PointInObjectAndSelect
	//	This Method is used to see if an object can
	// be selected at point p.
	//
	// parameters:
	//	p...........point to check at
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
		//-----------------------------------------------
		// is the point inside the polygon?
		//-----------------------------------------------
		if (m_Line.IsPointOnLine(p))
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

void CCadLine::AdjustReference(CDoubleSize Ref)
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
	m_Line.dP1 -= Ref;
	m_Line.dP2 -= Ref;
}

CString& CCadLine::GetTypeString(void)
{
	//***************************************************
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

CCadLine CCadLine::operator=(CCadLine &v)
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
	m_Line.dP1 = v.m_Line.dP1;
	m_Line.dP2 = v.m_Line.dP2;
	GetAttributes().CopyFrom(v.GePtrTotAttributes());
	return *this;
}

CCadObject * CCadLine::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadLine *pCL = new CCadLine;
	*pCL = *this;
	return pCL;
}

void CCadLine::RenderEnable(int e)
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
	CCadLine::m_RenderEnable = e;
}

CDoublePoint CCadLine::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	double x, y;
	x = (m_Line.dP1.dX + m_Line.dP2.dX) / 2.0;
	y = (m_Line.dP1.dY + m_Line.dP2.dY) / 2.0;
	return CDoublePoint(x,y);
}

void CCadLine::ChangeCenter(CDoubleSize sz)
{
	//***************************************************
	// ChangeCenter
	//	Change the center position of the object
	// parameters:
	//	sz......amount to change center by
	//
	// return value:
	//--------------------------------------------------
	m_Line.dP1 -= sz;
	m_Line.dP2 -= sz;
}

CDoubleSize CCadLine::GetSize()
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	return CDoubleSize(abs(m_Line.dP1.dX - m_Line.dP2.dX), abs(m_Line.dP1.dY - m_Line.dP2.dY));
}

void CCadLine::ChangeSize(CDoubleSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
	m_Line.dP2 += Sz;
}

DocFileParseToken CCadLine::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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
	Token = pLex->Accept(Token, TypeToken);
	Token = pLex->Accept(Token, DocFileParseToken('('));
	Token = pLex->Point(DocFileParseToken::POINT, m_Line.GetPoint(LinePoint::P1), Token);
	Token = pLex->Accept(Token, DocFileParseToken(','));
	Token = pLex->Point(DocFileParseToken::POINT, m_Line.GetPoint(LinePoint::P2), Token);
	Token = pLex->Accept(Token, DocFileParseToken(')'));
	Token = GetAttributes().Parse(Token, pLex);
	return Token;
}

void CCadLine::CopyAttributesTo(SLineAttributes*pAttrib)
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

void CCadLine::CopyAttributesFrom(SLineAttributes*pAtrib)
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
	GetAttributes().CopyFrom(pAtrib);
	ClearNeedsAttributes();
}

BOOL CCadLine::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadLine::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
		break;
	case ObjectDrawState::END_DRAWING:
		Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
		if (IDYES == Id)
		{
			m_CurrentAttributes.CopyTo(&m_LastAttributes);
		}
		if(GETVIEW()->IsAutoScrollEnabled())
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
		GETVIEW()->EnableAutoScroll(TRUE);
		m_Line.dP1 = m_Line.dP2 = DOUBLEPOINT(MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_Line.dP1 = DOUBLEPOINT(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Place Second Popint"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_Line.dP2 = DOUBLEPOINT(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		GETVIEW()->EnableAutoScroll(FALSE);
		m_Line.dP2 = DOUBLEPOINT(MousePos);
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadLine);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Line:Place First Point"));
		GETVIEW()->Invalidate();
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		m_Line.dP1 = m_Line.dP2 = DOUBLEPOINT(MousePos);
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_Line.dP2 = DOUBLEPOINT(MousePos);
		break;
	}
	GETVIEW()->Invalidate();
	return DrawState;
}

int CCadLine::EditProperties(void)
{
	CDlgLineAttributes Dlg;
	int Id;
	Dlg.SetLine(this);
	Id = Dlg.DoModal();
	return Id;
}