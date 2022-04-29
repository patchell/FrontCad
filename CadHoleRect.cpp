#include "pch.h"


CCadHoleRect::CCadHoleRect():CCadObject()
{
	m_pPenLine = 0;
	SetType(ObjectType::HOLE_RECTANGLE);
	GetName().Format(_T("RectangleHole_%d"), ++m_RectHoleCount);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetRectHoleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}
		
CCadHoleRect::~CCadHoleRect()
{
}

void CCadHoleRect::Move(CDoubleSize Diff)
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
}

void CCadHoleRect::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//***************************************************
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
}

void CCadHoleRect::SetVertex(int v, CDoubleSize sz)
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


int CCadHoleRect::GrabPoint(CDoublePoint p)
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
	return 0;
}


void CCadHoleRect::Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale)
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
	CPen *pOldPen;
	CDoubleRect rect;
	double Width = GetAttributes().m_HoleHeight;
	double Height = GetAttributes().m_HoleHeight;
	int Lw;
	CDoublePoint Horz, Vert;
	CDoubleSize dX, dY;

	dX = CDoubleSize(3.0 * Width / 8.0, 0.0);
	dY = CDoubleSize(0.0, 3.0 * Height / 8.0);
	Horz = m_Center - dX;
	Vert = m_Center - dY;

	if (CCadHoleRect::m_RenderEnable)
	{
		Lw = GETAPP.RoundDoubleToInt(Scale.m_ScaleX * GetAttributes().m_LineWidth);
		if (Lw < 1) Lw = 1;

		rect.SetPointsFromCenter(m_Center, m_Center + CDoubleSize(Width,Height),m_Center);

		if (!IsLastModeSame(mode) || IsDirty())
		{
			if (m_pPenLine) delete m_pPenLine;
			pDC->SelectStockObject(NULL_BRUSH);
			switch (mode.DrawMode)
			{
			case ObjectDrawMode::FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw, GetAttributes().m_colorLine);
				break;
			case ObjectDrawMode::SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(0, 255, 0));
				break;
			case ObjectDrawMode::SKETCH:
				m_pPenLine = new CPen(PS_SOLID, 1, GetAttributes().m_colorLine);
				break;
			}
			SetDirty(FALSE);
		}
		CRect RegularRect;
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
		case ObjectDrawMode::SELECTED:
		case ObjectDrawMode::SKETCH:
			pOldPen = pDC->SelectObject(m_pPenLine);
			RegularRect = rect.ToCRect(ULHC, Scale);
			pDC->Rectangle(&RegularRect);
			pDC->MoveTo(Horz.ToPixelPoint(ULHC,Scale));
			pDC->LineTo((m_Center + dX).ToPixelPoint(ULHC,Scale));
			pDC->MoveTo(Vert.ToPixelPoint(ULHC,Scale));
			pDC->LineTo((m_Center + dY).ToPixelPoint(ULHC,Scale));
			pDC->SelectObject(pOldPen);
			break;
		}
		SetLastMode(mode);
	}
}

int CCadHoleRect::PointInObjectAndSelect(
	CDoublePoint p, 
	CCadObject ** ppSelList , 
	int index, 
	int n, 
	DrawingCheckSelectFlags flag
)
{
	//***************************************************
	// CheckSelected
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
	//	returns 
	//	otherwise, false
	//--------------------------------------------------
	return 0;
}

CDoublePoint CCadHoleRect::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return CPoint();
}

void CCadHoleRect::AddObject(CCadObject *pO)
{
	//***************************************************
	// AddObject
	//	Adds a child object to an object.  This Method
	// is not supported by all objects, because they just
	// can have children.  A good example of an object
	// that has children would be a library object
	//
	// parameters:
	//	pO.....pointer to object to add.
	//
	// return value:none
	//--------------------------------------------------
}

void CCadHoleRect::RemoveObject(CCadObject *pO)
{
	//***************************************************
	// RemoveObject
	//	Removes a child object from an object
	//
	// parameters:
	//	pO.....pointer to object to remove
	//
	// return value:none
	//--------------------------------------------------

}

CCadObject *CCadHoleRect::GetHead(void)
{
	//***************************************************
	// GetHead
	//	Retrieves the head pointer to a list of children
	// in the object
	// parameters:
	//
	// return value: Head pointer to children
	//--------------------------------------------------
	return 0;
}


CDoubleRect& CCadHoleRect::GetRect(CDoubleRect& rect)
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
	rect.SetPointsFromCenter(
		m_Center, 
		m_Center + CDoubleSize(
			GetAttributes().m_HoleWidth / 2.0,
			GetAttributes().m_HoleHeight / 2.0
		),
		m_Center
	);
	return rect;
}

CString& CCadHoleRect::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Rect Hole");
	return csName;
}

CCadHoleRect CCadHoleRect::operator=(CCadHoleRect &v)
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
	return CCadHoleRect();
}

CPoint CCadHoleRect::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadHoleRect::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadHoleRect *pHR = new CCadHoleRect;
	*pHR = *this;
	return pHR;
}

void CCadHoleRect::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

CDoublePoint CCadHoleRect::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	return m_Center;
}

CDoubleSize CCadHoleRect::GetSize()
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	return CSize();
}

DocFileParseToken CCadHoleRect::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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

void CCadHoleRect::CopyAttributesTo(SRectHoleAttributes * pAttrib)
{
	/***************************************************
	*	GetAttributes
	*		This Method is used to copy the
	*	attributes from this object into
	*	an external attributes stucture
	*
	* Parameters:
	*	pAttrb.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyTo(pAttrib);
}

void CCadHoleRect::CopyAttributesFrom(SRectHoleAttributes *pAttrib)
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

BOOL CCadHoleRect::NeedsAttributes()
{
	return (m_AttributesGood == 0);
}

void CCadHoleRect::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

ObjectDrawState CCadHoleRect::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("Rectangular Hole:Place Center Point"));
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
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		Id = EditProperties();
		if (IDOK == Id)
		{
			m_AttributesDirty = TRUE;
			CopyAttributesTo(&m_CurrentAttributes);
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		m_Center = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_Center = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadHoleRect);
		GETAPP.UpdateStatusBar(_T("Rectangular Hole:Place Center Point"));
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadHoleRect::MouseMove(ObjectDrawState DrawState)
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
		m_Center = MousePos;
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}

int CCadHoleRect::EditProperties()
{
	int Id;
	CDlgSquareHoleProp Dlg;

	Dlg.SetRectHole(this);
	Id = Dlg.DoModal();
	return Id;
}
