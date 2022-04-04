#include "pch.h"

CCadRect::CCadRect():CCadObject()
{
	m_pPenLine = 0;
	m_pBrFill = 0;
	SetType(ObjectType::RECT);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetRectangleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
	m_csName.Format(_T("Rectangle_%d"), ++CCadRect::m_RectCount);
}

CCadRect::CCadRect(CCadRect &r) :CCadObject()
{
	m_Rect = r.GetRect(m_Rect);
	m_pPenLine = 0;
	m_pBrFill = 0;
	GetAttributes().CopyFrom(&r.GetAttributes());
	SetType(ObjectType::RECT);
	m_csName.Format(_T("Rectangle_%d"), ++CCadRect::m_RectCount);
}

CCadRect::~CCadRect()
{
	if (m_pPenLine) delete m_pPenLine;
	if (m_pBrFill) delete m_pBrFill;
}

void CCadRect::Move(CDoubleSize Diff)
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
	m_Rect.AdjustReference(Diff);
}

void CCadRect::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* psIndent = new char[256];
	GETAPP.MkIndentString(psIndent, Indent, ' ');

	fprintf(pO, "%s%s(\n",
		psIndent,
		CLexer::TokenToString(DocFileParseToken::RECT)
	);
	m_Rect.Save(
		pO,
		DocFileParseToken::DOUBLERECT,
		Indent + 1,
		flags
	);
	GetAttributes().Save(pO, Indent + 1, flags);
	fprintf(pO, "%s}\n", psIndent);
	delete []psIndent;
}

int CCadRect::GrabPoint(CDoublePoint p)
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
	//--------------------------------------------------
	int rV = -1;
	int i,loop;
	CDoublePoint Corner;

	for (i = 0, loop = 1; (i < 4) && loop; ++i)
	{
		Corner = m_Rect.GetDoublePoints()[i];
		if (Corner.IsPointOnTarget(p))
		{
			rV = i;
			loop = 0;
		}
	}
	return rV;
}


void CCadRect::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
	int Lw;
	CSize rectLWcomp;

	if (CCadRect::m_RenderEnable)
	{
		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.m_ScaleX);
		if (Lw < 1 || ObjectDrawMode::SKETCH == mode.DrawMode)
		{
			Lw = 1;
			rectLWcomp = CSize(0, 0);
		}
		else
			rectLWcomp = CSize(Lw / 2, Lw / 2);
		if (!IsLastModeSame(mode) || IsDirty())
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
		//SetRect(rect, P1, P2, rectLWcomp);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBr = pDC->SelectObject(m_pBrFill);
			m_Rect.Draw(pDC, mode, Offset, Scale);
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
			m_Rect.Draw(pDC, mode, Offset, Scale);
			pDC->SelectObject(&SelPen);
			pDC->SelectObject(&SelBrush);
			mode.PointsMode = SelectedPointsMode::POINT_BOTH_RECT_FILLED;
			m_Rect.GetPoint(RectPoint::LOWERLEFT).Draw(pDC, mode, Offset, Scale);
			m_Rect.GetPoint(RectPoint::UPPERLEFT).Draw(pDC, mode, Offset, Scale);
			m_Rect.GetPoint(RectPoint::LOWERRIGHT).Draw(pDC, mode, Offset, Scale);
			m_Rect.GetPoint(RectPoint::UPPERRIGHT).Draw(pDC, mode, Offset, Scale);
			pDC->SelectObject(pOldBr);
			pDC->SelectObject(pOld);
		}
		break;
		case ObjectDrawMode::SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			m_Rect.Draw(pDC,mode,Offset,Scale);
			pDC->SelectObject(pOld);
			break;
		}
		SetLastMode(mode);
	}
}

int CCadRect::PointInObjectAndSelect(
	CDoublePoint p, 
	CCadObject ** ppSelList, 
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
	int rV = index;
	if (index < n || n==0)
	{
		if (m_Rect.PointInRectangle(p))
		{
			if (ppSelList)
			{
				switch (flag)
				{
					case DrawingCheckSelectFlags::FLAG_ALL:
						ppSelList[rV++] = this;
						break;
					case DrawingCheckSelectFlags::FLAG_UNSEL:
						if(!IsSelected())
							ppSelList[rV++] = this;
						break;
					case DrawingCheckSelectFlags::FLAG_SEL:
						if(IsSelected())
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

CDoublePoint CCadRect::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_Rect.GetPoint(RectPoint::LOWERLEFT);
}


void CCadRect::AdjustReference(CDoubleSize Ref)
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
	m_Rect.AdjustReference(Ref);
}

CDoubleRect& CCadRect::GetRect(CDoubleRect& rect)
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
	rect = m_Rect;
	return rect;
}

CString& CCadRect::GetTypeString(void)
{
	//***************************************************
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

CCadRect CCadRect::operator=(CCadRect &v)
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
	CCadRect rect;
	rect.m_Rect = v.m_Rect;
	rect.GetAttributes().CopyFrom(v.GetPtrToAttributes());
	return rect;
}

CCadObject * CCadRect::CopyObject(void)
{
	//***************************************************
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

void CCadRect::RenderEnable(int e)
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
	CCadRect::m_RenderEnable = e;
}

CDoublePoint& CCadRect::GetCenter(CDoublePoint& Center)
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	Center = m_Rect.GetCenter(Center);
	return Center;
}

void CCadRect::ChangeCenter(CDoublePoint p)
{
	//***************************************************
	// ChangeCenter
	//	Change the center position of the object
	// parameters:
	//	p......point to change center to
	//
	// return value:
	//--------------------------------------------------
	CDoublePoint Center;
	CDoubleSize Diff = p - m_Rect.GetCenter(Center);
	m_Rect.AdjustReference(Diff);
}

CDoubleSize& CCadRect::GetSize(CDoubleSize& size)
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	size = m_Rect.GetSize(size);
	return size;
}

void CCadRect::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
//	m_P2.x += Sz.cx;
//	m_P2.y += Sz.cy;
}

DocFileParseToken CCadRect::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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
	Token = pLex->Accept (Token, DocFileParseToken::RECT);
	Token = pLex->Expect(Token, DocFileParseToken('('));
	Token = pLex->DoubleRect(DocFileParseToken::DOUBLERECT, m_Rect, Token);
	Token = GetAttributes().Parse(Token, pLex);
	Token = pLex->Expect(Token, DocFileParseToken(')') );
	return Token;
}

void CCadRect::CopyAttributesTo(SRectAttributes *pAttrib)
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


void CCadRect::CopyAttributesFrom(SRectAttributes *pAttrib)
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		GETVIEW()->EnableAutoScroll(TRUE);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Place First Point"));
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
		m_Rect.SetPoints(MousePos, MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_Rect.SetPoints(MousePos, MousePos);
		DrawState = ObjectDrawState::ROTATE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Set Second Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_Rect.SetSecondPoint(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		m_Rect.SetSecondPoint(MousePos);
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadRect);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Place First Point"));
		GETVIEW()->Invalidate();
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
	CDoublePoint MousePos = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		m_Rect.SetPoints(MousePos, MousePos);
		GETVIEW()->Invalidate();
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		m_Rect.SetSecondPoint(MousePos);
		GETVIEW()->Invalidate();
		break;
	}
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

BOOL CCadRect::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadRect::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}
