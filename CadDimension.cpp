#include "pch.h"


CCadDimension::CCadDimension():CCadObject()
{
	SetType(ObjectType::DIMENSION);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetDimensionAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	m_nTotalObjects = 0;
	CopyAttributesFrom(&m_CurrentAttributes);
}

void CCadDimension::OnCreate(void)
{
	++CCadDimension::m_DimensionCount;
	GetName().Format(_T("Dimension_%d"), m_DimensionCount);
}

CCadDimension::~CCadDimension()
{
}

void CCadDimension::Move(CDoubleSize Diff)
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

void CCadDimension::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadDimension::SetVertex(int v, CDoubleSize sz)
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


int CCadDimension::GrabPoint(CDoublePoint p)
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


void CCadDimension::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
	if (IsRenderDimensionEnabled())
	{
		if (GetHead())
		{
			CCadObject* pObj = GetHead();
			while (pObj)
			{
				pObj->Draw( pDC, mode, Offset, Scale);
				pObj = pObj->GetNext();
			}
		}
	}
}

BOOL CCadDimension::PointInObjectAndSelect(CPoint p, CSize Offset, CCadObject ** ppSelList , int index, int n, DrawingCheckSelectFlags flag)
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

CDoublePoint CCadDimension::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return CDoublePoint();
}

int CCadDimension::IsDirty(void)
{
	//***************************************************
	// IsDirty
	//	returns the status of the Dirty State of the
	// object
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CCadObject::IsDirty();
}

void CCadDimension::SetDirty(int d)
{
	//***************************************************
	// SetDirty
	// Changes the state of the dirty flag
	// parameters:
	//	d.......new state of dirty flag.
	//
	// return value:
	//--------------------------------------------------
	CCadObject::SetDirty(d);
}

int CCadDimension::IsSelected()
{
	//***************************************************
	// IsSelected
	//	returns the state of the selected flag
	// parameters:
	//
	// return value:State of the selected flag
	//--------------------------------------------------
	return CCadObject::IsSelected();
}

void CCadDimension::SetSelected(int Flag)
{
	//***************************************************
	// SetSelected
	//	Changes the state of the selcted flag.
	// parameters:
	// Flag......New state of the selected flag
	//
	// return value:
	//--------------------------------------------------
	CCadObject::SetSelected(Flag);
}

void CCadDimension::AdjustReference(CDoubleSize Ref)
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

}

CDoubleRect& CCadDimension::GetRect(CDoubleRect& rect)
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
	rect = CDoubleRect();
	return rect;
}

CString& CCadDimension::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csType = _T("Dimension");
	return csType;
}

CCadDimension CCadDimension::operator=(CCadDimension &v)
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
	return CCadDimension();
}

CPoint CCadDimension::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadDimension::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadDimension *pCD = new CCadDimension;
	*pCD = *this;
	return pCD;
}

void CCadDimension::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

CDoublePoint CCadDimension::GetCenter()
{
	//***************************************************
	// GetCenter
	//	Get the point at the "center" of the object.
	// parameters:
	//
	// return value:the center point
	//--------------------------------------------------
	return CDoublePoint();
}

void CCadDimension::ChangeCenter(CSize p)
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

CDoubleSize CCadDimension::GetSize()
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

void CCadDimension::ChangeSize(CSize Sz)
{
	//***************************************************
	// ChangeSize
	//	Change the size of the object
	// parameters:
	//	sz.....size to change object by (not change to)
	// return value:
	//--------------------------------------------------
}

DocFileParseToken CCadDimension::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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

void CCadDimension::CopyAttributesFrom(SCadDimAttributes *pAttrib)
{
	/***************************************************
	*	CopyAttributesFrom
	*		This Method is used to copy the
	*	attributes pointed to by the parameter into
	*	this object
	*
	* Parameters:
	*	pAttrib.....pointer to attributes structure to copy
	***************************************************/
	GetAttributes().CopyFrom(pAttrib);
	ClearNeedsAttributes();
}

void CCadDimension::CopyAttributesTo(SCadDimAttributes* pAttrib)
{
	GetAttributes().CopyTo(pAttrib);
}

BOOL CCadDimension::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadDimension::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}

int CCadDimension::EditProperties()
{
	CDlgDimensionPropertiex Dlg;
	int ID;

	Dlg.SetDimension(this);
	ID = Dlg.DoModal();
	return ID;
}

ObjectDrawState CCadDimension::ProcessDrawMode(ObjectDrawState DrawState)
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
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		DrawState = ObjectDrawState::SECOND_POINT_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::SECOND_POINT_LBUTTON_DOWN:
		DrawState = ObjectDrawState::SECOND_POINT_LBUTTON_UP;
		break;
	case ObjectDrawState::SECOND_POINT_LBUTTON_UP:
		DrawState = ObjectDrawState::EXTENSION_LINES_LBUTTON_DOWN;
		break;
	case ObjectDrawState::EXTENSION_LINES_LBUTTON_DOWN:
		DrawState = ObjectDrawState::EXTENSION_LINES_LBUTTON_UP;
		break;
	case ObjectDrawState::EXTENSION_LINES_LBUTTON_UP:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETVIEW()->EnableAutoScroll(FALSE);
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadDimension);
		GETAPP.UpdateStatusBar(_T("Arrow:Locate Arrow Tip Point"));
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadDimension::MouseMove(ObjectDrawState DrawState)
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
		break;
	case ObjectDrawState::SECOND_POINT_LBUTTON_DOWN:
		break;
	case ObjectDrawState::EXTENSION_LINES_LBUTTON_DOWN:
		break;
	}
	return DrawState;
}

void CCadDimension::AddDimObject(CCadObject* pObj)
{
	if (GetHead() == 0)
	{
		SetHead(pObj);
		SetTail(pObj);;
	}
	else
	{
		pObj->SetNext(GetHead());
		GetHead()->SetPrev(pObj);
		SetHead(pObj);
	}
	++m_nTotalObjects;
}

void CCadDimension::RemoveDimObject(CCadObject* pObj)
{
	if (pObj == GetHead())
	{
		SetHead(pObj->GetNext());
		if (GetHead())
			GetHead()->SetPrev(0);
		else
			SetTail(0);
	}
	else if (pObj == GetTail())
	{
		SetTail(pObj->GetPrev());
		if (GetTail())
			GetTail()->SetNext(0);
		else
			SetHead(0);
	}
	else
	{
		pObj->GetNext()->SetPrev(pObj->GetPrev());
		pObj->GetPrev()->SetNext(pObj->GetNext());
	}
}

double CCadDimension::Slope()
{
	//-----------------------------------
	// Get the slope of the line defined
	// by m_P1, m_P2
	//-----------------------------------
	double m;
	if (m_P1.dX != m_P2.dX)
		m = (m_P1.dY - m_P2.dY) / (m_P1.dX - m_P2.dX);
	return m;
}

double CCadDimension::OrthogonalSlope()
{
	//-----------------------------------
	// Get the slope of the line defined
	// by m_P1, m_P2 that is perpendicular
	//-----------------------------------
	double m;
	if (m_P1.dY != m_P2.dY)
		m = (m_P1.dX - m_P2.dX)/ (m_P1.dY - m_P2.dY);
	return m;
}

UINT CCadDimension::LineIs()
{
	UINT rV = 0;	//line is whatever

	if (m_P1.dY == m_P2.dY)
		rV = DIMLINE_HORIZONTAL;
	else if (m_P1.dX == m_P2.dX)
		rV = DIMLINE_VERTICAL;
	return rV;
}

double CCadDimension::YIntercept(double m, CDoublePoint p)
{
	//-----------------------------
	// finds the Y intercept fopr
	// a line that goes through
	// point p with slope m
	//
	// y = mx + b or
	// b = y - mx
	// parameters:
	//	m.......slope of the line
	//	p.......point the line
	//          goes through
	// Return Vaule: the Y intercept
	//-----------------------------
	double b;

	b = p.dY - m * p.dX;
	return b;
}