#include "pch.h"


CCadElispe::CCadElispe():CCadObject()
{
	m_pPenLine = 0;
	m_pBrFill = 0;
	SetType(ObjectType::ELIPSE);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetEllipseAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadElispe::CCadElispe(CCadElispe &e) :CCadObject()
{
	m_P1 = e.m_P1;
	m_P2 = e.m_P2;
	GetAttributes().CopyFrom(e.GetPtrToAttributes());
	m_pPenLine = 0;
	m_pBrFill = 0;
	SetType(ObjectType::ELIPSE);
}

CCadElispe::~CCadElispe()
{
	if (m_pPenLine) delete m_pPenLine;
	if (m_pBrFill) delete m_pBrFill;
}

void CCadElispe::OnCreate()
{
	GetName().Format(_T("Ellipse%d"), ++m_DocCount);
}

void CCadElispe::Move(CDoubleSize Diff)
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

void CCadElispe::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

	fprintf(pO, "%s%s(%s(\n",
		GETAPP.MkIndentString(IndentString, Indent, ' '),
		CLexer::TokenToString(DocFileParseToken::ELIPSE),
		CLexer::TokenToString(DocFileParseToken::POINT)
	);
	m_P1.Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	fprintf(pO, ",");
	m_P2.Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	fprintf(pO, ")\n");
	GetAttributes().Save(pO, Indent, flags);
	delete[] IndentString;
}

void CCadElispe::SetVertex(int v, CDoubleSize sz)
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
	if (v)
		m_P2 += sz;
	else
		m_P1 += sz;
}


int CCadElispe::GrabPoint(CDoublePoint p)
{
	//***************************************************
	// GrabPoint
	//	This Method checks for a vertex at point p
	//
	// parameters:
	//	p.....point to check for presence of a vertex
	//
	// return value:
	//	returns index of vertex if succesful
	//	returns -1 on fail
	//	return 0 for P1
	//  return 1 for P2
	//--------------------------------------------------
	int rV = -1;
	CScale Scale = GETVIEW()->GetGrid().GetInchesPerPixel();
	double Inches = Scale.GetScaleX() * 10.0 /* Pixels */;
	//------ A 10 pixel target sounds good
	CDoubleSize Diff(Inches, Inches);
	CDoubleRect rect;
	rect.SetPoints(m_P1 + Diff, m_P1 - Diff);
	if (rect.PointInRectangle(p))
		rV = 0;
	else
	{
		rect.SetPoints(m_P2 + Diff, m_P2 - Diff);
		if (rect.PointInRectangle(p))
			rV = 1;
	}
	return rV;
}


void CCadElispe::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
	CRect rect;
	CSize rectLWcomp;
	CPoint P1, P2;
	int Lw;

	if (IsRenderEnabled())
	{
		P1 = m_P1.ToPixelPoint(Offset, Scale);;
		P2 = m_P2.ToPixelPoint(Offset, Scale);
		if ((Lw = int(Scale.m_ScaleX * GetLineWidth())) < 1) Lw = 1;
		if (Lw <= 1 || ObjectDrawMode::SKETCH == mode.DrawMode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);
		if ((!IsLastModeSame(mode) )|| IsDirty())
		{
			if (m_pPenLine) delete m_pPenLine;
			switch (mode.DrawMode)
			{
			case ObjectDrawMode::FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw,GetAttributes().m_colorLine);
				break;
			case ObjectDrawMode::SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, GetAttributes().m_colorLine ^0x00f0f0f0);
				break;
			case ObjectDrawMode::SKETCH:
				m_pPenLine = new CPen(PS_DOT, 1, GetAttributes().m_colorLine);
				break;
			}
		}
		if (m_pBrFill == 0 || IsDirty())
		{
			if (m_pBrFill) delete m_pBrFill;
			if (GetTransparent())
			{
				m_pBrFill = new CBrush();
				m_pBrFill->CreateStockObject(NULL_BRUSH);
			}
			else
				m_pBrFill = new CBrush(GetFillColor());
			SetDirty(0);
		}
		rect = CDoubleRect(m_P1, m_P2).ToCRect(Offset, Scale);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Ellipse(&rect);
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
			pOldBr = pDC->SelectObject(m_pBrFill);
			pDC->Ellipse(&rect);
			pDC->SelectObject(&SelPen);
			pDC->SelectObject(&SelBrush);
			CRect rect;
			CSize p = CSize(4, 4);
			rect.SetRect(P1 - p, P1 + p);
			pDC->Rectangle(&rect);
			rect.SetRect(P2 - p, P2 + p);
			pDC->Rectangle(&rect);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOld);
		}
		break;
		case ObjectDrawMode::SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->Ellipse(&rect);
			pDC->SelectObject(pOld);
			break;
		}
		SetLastMode(mode);
	}
}

BOOL CCadElispe::PtInElipse(CDoublePoint p)
{
	double a, b, v;
	BOOL rV;
	
	CDoublePoint Center = CDoubleRect(m_P1, m_P2).GetCenter(Center);

	a = (m_P2.dX - m_P1.dX) / 2.0;
	b = (m_P2.dY - m_P1.dY) / 2.0;
	v = GETAPP.Ellipse(a, b, p, Center);
	if (v < 1.0) rV = TRUE;
	else rV = FALSE;
	return rV;
}

int CCadElispe::EditProperties()
{
	CDlgEllispeProperties Dlg;

	Dlg.SetEllipse(this);
	return Dlg.DoModal();
}

BOOL CCadElispe::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadElispe::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

BOOL CCadElispe::PointInObjectAndSelect(
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
	if (index < n || n == 0)
	{
		//---------------------------------------
		// is point in Ellipse
		//---------------------------------------
		if (PtInElipse(p))
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

CDoublePoint CCadElispe::GetReference()
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

void CCadElispe::AdjustReference(CDoubleSize Ref)
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

CDoubleRect& CCadElispe::GetRect(CDoubleRect& rect)
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

CString& CCadElispe::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Ellipse");
	return csName;
}

CCadElispe CCadElispe::operator=(CCadElispe &v)
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
	CCadElispe returnEllipse;

	returnEllipse.m_P1 = v.m_P1;
	returnEllipse.m_P2 = v.m_P2;
	returnEllipse.GetAttributes().CopyFrom(v.GetPtrToAttributes());
	return returnEllipse;
}

CCadObject * CCadElispe::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadElispe *pCE = new CCadElispe;
	*pCE = *this;
	return pCE;
}
	
CDoublePoint& CCadElispe::GetCenter(CDoublePoint& Center)
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	return CDoubleRect(m_P1,m_P2).GetCenter(Center);
}

void CCadElispe::ChangeCenter(CSize p)
{
	//***************************************************
	// ChangeCenter
	//	Change the center position of the object
	// parameters:
	//	p......amount to change center by
	//
	// return value:
	//--------------------------------------------------
	m_P1 -= p;
	m_P2 -= p;
}

CDoubleSize& CCadElispe::GetSize(CDoubleSize& size)
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
	return GetRect(rect).GetSize(size);
}

void CCadElispe::ChangeSize(CDoubleSize Sz)
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

DocFileParseToken CCadElispe::Parse(
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
	//	Token....Look Ahead Token
	//	pLex.....Pointer to Lexer for the data stream
	//	TypeToken..Token that specifies the type of this object
	//
	// return value:
	//	returns lookahead token on success, or
	//			throws an exception
	//--------------------------------------------------
	Token = pLex->Accept(Token, TypeToken);
	Token = pLex->Accept(Token, DocFileParseToken('('));
	Token = pLex->Point(DocFileParseToken::POINT, m_P1, Token);
	Token = pLex->Accept(Token, DocFileParseToken(','));
	Token = pLex->Point(DocFileParseToken::POINT, m_P2, Token);
	Token = pLex->Accept(Token, DocFileParseToken(')'));
	Token = GetAttributes().Parse(Token, pLex);
	return Token;
}

void CCadElispe::CopyAttributesTo(SEllipseAttributes *pAttrib)
{
	/***************************************************
	*	GetAttributes
	*		This Method is used to copy the
	*	attributes from this object into
	*	an extern attribute structure
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy into
	***************************************************/
	GetAttributes().CopyTo(pAttrib);
}


void CCadElispe::CopyAttributesFrom(SEllipseAttributes *pAttrib)
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

ObjectDrawState CCadElispe::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("Ellipse::Place First Point Defining Shape"));
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
		m_P1 = m_P2 = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_P1 = m_P2 = MousePos;
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Ellipse:Place Second Point Defining Shape"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_P2 = MousePos;
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		m_P2 = MousePos;
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadElispe);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Ellipse::Place First Point Defining Shape"));
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadElispe::MouseMove(ObjectDrawState DrawState)
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
			GETVIEW()->Invalidate();
			break;
	}
	return DrawState;
}