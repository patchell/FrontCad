#include "pch.h"

CCadOrigin::CCadOrigin():CCadObject()
{
	SetType(ObjectType::ORIGIN);
	GetName().Format(_T("Origin_%d"), ++m_OriginCount);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetOriginAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadOrigin::~CCadOrigin()
{
}

void CCadOrigin::Move(CDoubleSize Diff)
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

void CCadOrigin::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadOrigin::SetVertex(int v, CPoint p)
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


int CCadOrigin::GrabPoint(CDoublePoint p)
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


void CCadOrigin::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
	CCadObject* pObj = GetHead(); 
	CPen LinePen, * OldPen = 0;
	CBrush fillBrush, * oldBrush = 0;
	int LineWidth;
	CRect rect;
	int rectHalfWidth, CrossHairLen;
	CPoint pointUL, pointLR, pointCenter;;
	double Radius;
	COLORREF colorLine;
	//---------------------------------------
	// Draw this Origin Object
	//---------------------------------------

	if (m_RenderEnable)
	{
		LineWidth = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.GetScaleX());
		if (LineWidth < 1)
			LineWidth = 1;
		Radius = GetAttributes().m_Radius * Scale.GetScaleX();
		CrossHairLen = GETAPP.RoundDoubleToInt(GetAttributes().m_Radius * 1.50 * Scale.GetScaleX());
		if (CrossHairLen < 15)
			CrossHairLen = 15;
		rectHalfWidth = GETAPP.RoundDoubleToInt(Radius);
		if (rectHalfWidth < 10)
		{
			rectHalfWidth = 10;
			Radius = double(rectHalfWidth);
		}
		
		pointLR = m_Origin.ToPixelPoint(Offset,Scale) + CSize(rectHalfWidth, rectHalfWidth);
		pointUL = m_Origin.ToPixelPoint(Offset, Scale) - CSize(rectHalfWidth, rectHalfWidth);
		rect.SetRect(pointUL, pointLR);
		rect.NormalizeRect();
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
			colorLine = GetAttributes().m_colorLine;
			LinePen.CreatePen(PS_SOLID, LineWidth, colorLine);
			OldPen = pDC->SelectObject(&LinePen);
			break;
		case ObjectDrawMode::SELECTED:
			colorLine = GetAttributes().m_colorSelected;
			LinePen.CreatePen(PS_SOLID, LineWidth, colorLine);
			OldPen = pDC->SelectObject(&LinePen);
			break;
		case ObjectDrawMode::SKETCH:
			colorLine = GetAttributes().m_colorLine;
			LinePen.CreatePen(PS_DOT, LineWidth, colorLine);
			OldPen = pDC->SelectObject(&LinePen);
			break;
		}
		fillBrush.CreateStockObject(NULL_BRUSH);
		oldBrush = pDC->SelectObject(&fillBrush);
		pDC->Ellipse(&rect);
		pointCenter = m_Origin.ToPixelPoint(Offset, Scale);
		pDC->MoveTo(pointCenter.x + CrossHairLen,pointCenter.y);
		pDC->LineTo(pointCenter.x - CrossHairLen, pointCenter.y);
		pDC->MoveTo(pointCenter.x,pointCenter.y + CrossHairLen);
		pDC->LineTo(pointCenter.x, pointCenter.y - CrossHairLen);
		pDC->SelectObject(oldBrush);
		if (OldPen)
			pDC->SelectObject(OldPen);
	}
	//---------------------------------------
	// Draw the child objects of this origin
	//---------------------------------------
	while (pObj)
	{
		pObj->Draw(pDC, mode, Offset, Scale);
		pObj = pObj->GetNext();
	}
}

BOOL CCadOrigin::PointInObjectAndSelect(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, DrawingCheckSelectFlags flag)
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
	return 0;
}

CDoublePoint CCadOrigin::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_Origin;
}

void CCadOrigin::AddObject(CCadObject *pO)
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

void CCadOrigin::RemoveObject(CCadObject *pO)
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

CCadObject *CCadOrigin::GetHead(void)
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



CDoubleRect& CCadOrigin::GetRect(CDoubleRect& rect)
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
	return rect;
}

CString& CCadOrigin::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Origin");

	return csName;
}

CCadOrigin CCadOrigin::operator=(CCadOrigin &v)
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
	CCadOrigin Origin;

	m_Origin = v.m_Origin;
	Origin.GetAttributes().CopyFrom(v.GetPtrToAttributes());
	return Origin;
}

CPoint CCadOrigin::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadOrigin::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadOrigin *pCO = new CCadOrigin;
	*pCO = *this;
	return pCO;
}

void CCadOrigin::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadOrigin::RenderEnable(int e)
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

}

CDoublePoint CCadOrigin::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	return m_Origin;
}


CDoubleSize CCadOrigin::GetSize()
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

DocFileParseToken CCadOrigin::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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

void CCadOrigin::CopyAttributesTo(SOriginAttributes* pAttrib)
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

void CCadOrigin::CopyAttributesFrom(SOriginAttributes * pAttrib)
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

ObjectDrawState CCadOrigin::MouseMove(ObjectDrawState DrawState)
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
	CDoublePoint MousePosition = GETVIEW()->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::SET_ATTRIBUTES:
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		m_Origin = MousePosition;
	}
	GETVIEW()->Invalidate();
	return DrawState;
}

ObjectDrawState CCadOrigin::ProcessDrawMode(ObjectDrawState DrawState)
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
	CPoint MouseScreenCoordinate;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		DrawState = ObjectDrawState::SET_ATTRIBUTES;
		GETAPP.UpdateStatusBar(_T("Origin:Place Origin Point"));
		break;
	case ObjectDrawState::END_DRAWING:
		Id = GETVIEW()->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
		if (IDYES == Id)
		{
			m_CurrentAttributes.CopyTo(&m_LastAttributes);
		}
		DrawState = ObjectDrawState::SELECT_NOTHING;
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		GETVIEW()->GetCursorPosition(&MouseScreenCoordinate);
		Id = EditProperties();
		printf("Id = %d\n", Id);
		if (IDOK == Id)
		{
			printf("IDOK\n");
			CopyAttributesTo(&m_CurrentAttributes);
			DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
			GETVIEW()->EnableAutoScroll(1);
		}
		else
			DrawState = ObjectDrawState::END_DRAWING;
		GETVIEW()->SetCursorPosition(MouseScreenCoordinate);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		m_Origin = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_Origin = MousePos;
		DrawState = ObjectDrawState::SET_ATTRIBUTES;
		GETVIEW()->AddOriginAtTail(this);
		GETVIEW()->EnableAutoScroll(0);
		GETVIEW()->SetObjectTypes(new CCadOrigin);
		GETAPP.UpdateStatusBar(_T("Origin:Set Origin Name"));
		break;
	}
	return DrawState;
}

int CCadOrigin::EditProperties()
{
	int Id;
	CDlgOriginAttributes Dlg;

	Dlg.SetOrigin(this);
	Id = Dlg.DoModal();
	return Id;
}

BOOL CCadOrigin::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadOrigin::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}
