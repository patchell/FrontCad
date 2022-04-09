#include "pch.h"

CCadRndRect::CCadRndRect():CCadObject()
{
	m_pPenLine = 0;
	m_pBrush = 0;
	SetType(ObjectType::ROUNDEDRECT);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetRoundedRectangleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}


CCadRndRect::CCadRndRect(CCadRndRect &r) :CCadObject()
{
	m_pPenLine = 0;
	m_pBrush = 0;
	GetAttributes().CopyFrom(r.GetPtrToAttributes());
	m_P1 = r.m_P1;
	m_P2 = r.m_P2;
	m_P3 = r.m_P3;
	SetType(ObjectType::ROUNDEDRECT);
}

void CCadRndRect::OnCreate()
{
	GetName().Format(_T( "RndRect%d"), ++m_RndRectCount);
}

CCadRndRect::~CCadRndRect()
{
	if (m_pPenLine) delete m_pPenLine;
	if (m_pBrush) delete m_pBrush;
}


void CCadRndRect::Move(CDoubleSize Diff)
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
	m_P1 += Diff;
	m_P2 += Diff;
}

void CCadRndRect::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* IndentString = new char[256];

	fprintf(pO, "%s%s(\n",
		GETAPP.MkIndentString(IndentString, Indent),
		CLexer::TokenToString(Token)
	);
	m_P1.Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	fprintf(pO, ",");
	m_P2.Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	fprintf(pO, ",");
	m_P3.Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	fprintf(pO, ")\n");
	GetAttributes().Save(pO, Indent + 1, flags);
	delete[] IndentString;
}

void CCadRndRect::SetVertex(int Vi, CDoubleSize sz)
{
	//***************************************************
	// SetVertex
	//	This Method is used to change the position of
	// a vertex.
	//
	// parameters:
	// Vi.....index of the vertex
	// p......Amnount to change the vertex by
	//
	// return value: none
	//--------------------------------------------------
	switch (Vi)
	{
	case 0:
		m_P1 += sz;
		break;
	case 1:
		m_P2 += sz;
		break;
	case 2:
		m_P3 += sz;
		break;
	}
}

int CCadRndRect::GrabPoint(CDoublePoint  point)
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

	if (m_P1.IsPointOnTarget(point))
	{
		rV = 0;
	}
	else if (m_P2.IsPointOnTarget(point))
	{
		rV = 1;
	}
	else if (m_P3.IsPointOnTarget(point))
	{
		rV = 2;
	}
	return rV;
}


void CCadRndRect::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
	CBrush *pOldBr;
	CPoint P1, P2, P3;
	CDoubleRect rect;
	CSize rectLWcomp;
	int Lw;

	if (CCadRndRect::m_RenderEnable)
	{
		P1 = m_P1.ToPixelPoint(Offset,Scale);
		P2 = m_P2.ToPixelPoint(Offset, Scale);;
		P3 = m_P3.ToPixelPoint(Offset, Scale);;
		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.m_ScaleX);
		if (Lw <= 1 || ObjectDrawMode::SKETCH == mode.DrawMode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);
		//	rect.SetRect(P1,P2);

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
			}
		}
		if (m_pBrush == 0 || IsDirty())
		{
			if (m_pBrush) delete m_pBrush;
			if (GetTransparent())
			{
				m_pBrush = new CBrush();
				m_pBrush->CreateStockObject(NULL_BRUSH);
			}
			else
				m_pBrush = new CBrush(GetFillColor());
			SetDirty(0);
		}
		SetRect(rect, m_P1, m_P2, rectLWcomp);

		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrush);
			pDC->RoundRect(rect.ToCRect(Offset,Scale), P3);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOld);
			break;
		case ObjectDrawMode::SELECTED:
		{
			CPen SelPen;
			CBrush SelBrush;
			SelPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			SelBrush.CreateSolidBrush(RGB(255, 0, 0));
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrush);
			pDC->RoundRect(rect.ToCRect(Offset,Scale), P3);
			pDC->SelectObject(&SelPen);
			pDC->SelectObject(&SelBrush);
			CRect rect;
			CSize p = CSize(4, 4);
			rect.SetRect(P1 - p, P1 + p);
			pDC->Rectangle(&rect);
			rect.SetRect(P2 - p, P2 + p);
			pDC->Rectangle(&rect);
			CPoint p3 = P1 + P3;
			rect.SetRect(p3 - p, p3 + p);
			pDC->Rectangle(&rect);
			p3 = P2 - P3;
			rect.SetRect(p3 - p, p3 + p);
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOld);
		}
		break;
		case ObjectDrawMode::SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->RoundRect(rect.ToCRect(Offset,Scale), P3);
			pDC->SelectObject(pOld);
			break;
		}
		SetLastMode(mode);
	}
}

int CCadRndRect::PointInObjectAndSelect(
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
	CDoubleRect rect(m_P1,m_P2);
	int rV = index;

	if (index < n || n == 0)
	{
		if (rect.PointInRectangle(p))
		{
			if (ppSelList)
			{
				switch (flag)
				{
				case DrawingCheckSelectFlags::FLAG_ALL:
						ppSelList[rV++] = this;
						break;
					case DrawingCheckSelectFlags::FLAG_UNSEL:
						if (!IsSelected())
							ppSelList[rV++] = this;
						break;
					case DrawingCheckSelectFlags::FLAG_SEL:
						if (IsSelected())
							ppSelList[rV++] = this;
						break;
				}
			}
			else
			{
				switch (flag)
				{
					case DrawingCheckSelectFlags::FLAG_ALL:
						rV = 1;
						break;
					case DrawingCheckSelectFlags::FLAG_UNSEL:
						if (!IsSelected())
							rV = 1;
						break;
					case DrawingCheckSelectFlags::FLAG_SEL:
						if (IsSelected())
							rV = 1;
						break;
				}

			}
		}
	}
	return rV;
}

CDoublePoint CCadRndRect::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_P1;
}

void CCadRndRect::AdjustReference(CDoubleSize Ref)
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
	m_P1 -= Ref;
	m_P2 -= Ref;
}

CDoubleRect& CCadRndRect::GetRect(CDoubleRect& rect)
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
	rect = CDoubleRect(m_P1, m_P2);
	return rect;
}

CString& CCadRndRect::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Rounded Rectangle");
	return csName;
}

CCadRndRect CCadRndRect::operator=(CCadRndRect &v)
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
	m_P1 = v.m_P1;
	m_P2 = v.m_P2;
	m_P3 = v.m_P3;
	GetAttributes().CopyFrom(v.GetPtrToAttributes());
	return *this;
}

CCadObject * CCadRndRect::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadRndRect *pCR = new CCadRndRect;
	*pCR = *this;
	return pCR;
}

void CCadRndRect::RenderEnable(int e)
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
	CCadRndRect::m_RenderEnable = e;
}

CDoublePoint& CCadRndRect::GetCenter(CDoublePoint& Center)
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	CDoubleRect rect = GetRect(rect);
	Center = rect.GetCenter(Center);
	return Center;
}

void CCadRndRect::ChangeCenter(CDoubleSize sz)
{
	//***************************************************
	// ChangeCenter
	//	Change the center position of the object
	// parameters:
	//	p......amount to change center by
	//
	// return value:
	//--------------------------------------------------
	m_P1 -= sz;
	m_P2 -= sz;
}

CDoubleSize& CCadRndRect::GetSize(CDoubleSize& size)
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	CDoubleRect rect;
	size = GetRect(rect).GetSize(size);
	return size;
}

void CCadRndRect::ChangeSize(CDoubleSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
	m_P2 += Sz;
}

DocFileParseToken CCadRndRect::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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
	Token = pLex->Accept(Token, DocFileParseToken::ROUNDRECT);
	Token = pLex->Accept(Token, DocFileParseToken('('));
	Token = pLex->Point(DocFileParseToken::POINT, m_P1, Token);
	Token = pLex->Accept(Token, DocFileParseToken(','));
	Token = pLex->Point(DocFileParseToken::POINT, m_P2, Token);
	Token = pLex->Accept(Token, DocFileParseToken(')'));
	Token = GetAttributes().Parse(Token, pLex);
	return Token;
}

void CCadRndRect::CopyAttributesTo(SRoundedRectAttributes* pAttrib)
{
	/***************************************************
	*	CopyAttributesTo
	* Copy the attributes in this object into the
	* attributes object pointed to by pAttrib
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyTo(pAttrib);
}

void CCadRndRect::CopyAttributesFrom(SRoundedRectAttributes*pAttrib)
{
	/***************************************************
	*	CopyAttributesFrom
	* Copy the attributes in the parameter pAttrib
	* into the attributes object for this object
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyFrom(pAttrib);
	ClearNeedsAttributes();
}

ObjectDrawState CCadRndRect::MouseMove(ObjectDrawState DrawState)
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
			m_P1 = m_P2 = MousePos;
			break;
		case ObjectDrawState::PLACE_LBUTTON_DOWN:
			m_P2 = MousePos;
			break;
	}
	GETVIEW()->Invalidate();
	return DrawState;
}

int CCadRndRect::EditProperties()
{
	int Id;
	CDlgRndRectProperties Dlg;
	Dlg.SetRndRect(this);
	Id = Dlg.DoModal();
	return Id;
}

ObjectDrawState CCadRndRect::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("Rounded Rectangle:Place First Point"));
		break;
	case ObjectDrawState::END_DRAWING:
		Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
		if (IDYES == Id)
		{
			m_CurrentAttributes.CopyTo(&m_LastAttributes);
		}
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
		m_P1 = m_P2 = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		GETVIEW()->Invalidate();
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_P1 = m_P2 = MousePos;
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rounded Rectangle:Place Second Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_P2 = MousePos;
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		GETVIEW()->EnableAutoScroll(FALSE);
		m_P2 = MousePos;
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadRndRect);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rounded Rectangle:Place First Point"));
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}

BOOL CCadRndRect::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadRndRect::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}
