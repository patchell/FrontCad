#include "pch.h"


CCadHoleRnd1Flat::CCadHoleRnd1Flat():CCadObject()
{
	m_pPenLine = 0;
	SetType(ObjectType::HOLE_RND1FLAT);
	GetName().Format(_T("RoundHole1Flat_%d"), ++m_RndHole1FlatCount);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetRoundHole1FlatAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadHoleRnd1Flat::~CCadHoleRnd1Flat()
{
}

void CCadHoleRnd1Flat::Move(CDoubleSize Diff)
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

void CCadHoleRnd1Flat::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadHoleRnd1Flat::SetVertex(int v, CDoubleSize sz)
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


int CCadHoleRnd1Flat::GrabPoint(CDoublePoint p)
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

void CCadHoleRnd1Flat::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
	CDoubleRect rect;
	double dx,Rad, StartAngle;
	dx = 3.0 * GetAttributes().m_HoleRadius / 4.0;
	CPoint start,end;
	CDoublePoint P1, P2, P3,P4;
	int Lw;
	double Fd;	//flat side to center distance
	Rad = GetAttributes().m_HoleRadius;
	Fd = GetAttributes().m_FlatDistanceFromCenter;

	if (CCadHoleRnd1Flat::m_RenderEnable)
	{
		P1 = m_Center + CDoubleSize(dx,dx);
		P2 = m_Center + CDoubleSize(-dx, dx);
		P3 = m_Center + CDoubleSize(-dx, -dx);
		P4 = m_Center + CDoubleSize(dx, -dx);
		Lw = int(Scale.GetScaleX() * GetAttributes().m_LineWidth);
		if (Lw < 1) Lw = 1;

		rect.SetPointsFromCenter(m_Center, m_Center + CDoubleSize(Rad,Rad), m_Center);

		if (!IsLastModeSame(mode) || IsDirty())
		{
			if (m_pPenLine) delete m_pPenLine;
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
		StartAngle = CalculateAngle(Fd, Rad);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
		case ObjectDrawMode::SELECTED:
		case ObjectDrawMode::SKETCH:
			pDC->SelectStockObject(NULL_BRUSH);
			pOld = pDC->SelectObject(m_pPenLine);
			pDC->AngleArc(
				m_Center.ToPixelPoint(Offset,Scale).x,
				m_Center.ToPixelPoint(Offset,Scale).y,
				int(Rad * Scale.GetScaleX()),
				float(StartAngle),
				float(360.0 - 2.0 * StartAngle)
			);
			pDC->MoveTo(SolveIntersection(1, P1, Fd, Rad).ToPixelPoint(Offset, Scale));
			pDC->LineTo(SolveIntersection(0, P1, Fd, Rad).ToPixelPoint(Offset, Scale));
			pDC->MoveTo(P1.ToPixelPoint(Offset,Scale));
			pDC->LineTo(P3.ToPixelPoint(Offset,Scale));
			pDC->MoveTo(P2.ToPixelPoint(Offset,Scale));
			pDC->LineTo(P4.ToPixelPoint(Offset,Scale));
			pDC->SelectObject(pOld);
			break;
		}
		SetLastMode(mode);
	}
}

double CCadHoleRnd1Flat::CalculateAngle(
	double FlatDist,
	double Radius
)
{
	double Angle;
	Angle = acos(FlatDist / Radius);
	Angle = Angle * GETAPP.GetPi() / 180.0;
	return Angle;
}

CDoublePoint CCadHoleRnd1Flat::SolveIntersection(
	int mode, 
	CDoublePoint Center, 
	double FlatDist, 
	double Radius
)
{
	CDoublePoint Result;
	if (mode)
		Result = CDoublePoint(
			Center.dX + FlatDist,
			Center.dY - sqrt(Radius * Radius - FlatDist * FlatDist)
		);
	else
		Result = CDoublePoint(
			Center.dX + FlatDist,
			Center.dY + sqrt(Radius * Radius - FlatDist * FlatDist)
		);
	return Result;
}

int CCadHoleRnd1Flat::PointInObjectAndSelect(CDoublePoint p, CCadObject ** ppSelList , int index, int n, DrawingCheckSelectFlags flag)
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

CDoublePoint CCadHoleRnd1Flat::GetReference()
{
	//***************************************************
	// GetReference
	//	This Method returns the reference point for
	// the object
	// parameters:none
	//
	// return value:reference point
	//--------------------------------------------------
	return m_Center;
}


CDoubleRect& CCadHoleRnd1Flat::GetRect(CDoubleRect& rect)
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
	double Radius = GetAttributes().m_HoleRadius;
	rect.SetPointsFromCenter(
		m_Center,
		m_Center + CDoubleSize(
			Radius,
			Radius
		),
		m_Center
	);
	return rect;
}

CString& CCadHoleRnd1Flat::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("Round Hole 1 Flat"));
	return csTypeName;
}

CCadHoleRnd1Flat CCadHoleRnd1Flat::operator=(CCadHoleRnd1Flat &v)
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
	return CCadHoleRnd1Flat();
}

CCadObject * CCadHoleRnd1Flat::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadHoleRnd1Flat *pHR = new CCadHoleRnd1Flat;
	*pHR = *this;
	return pHR;
}

void CCadHoleRnd1Flat::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadHoleRnd1Flat::RenderEnable(int e)
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

CDoublePoint CCadHoleRnd1Flat::GetCenter()
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

CDoubleSize CCadHoleRnd1Flat::GetSize()
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

DocFileParseToken CCadHoleRnd1Flat::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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

void CCadHoleRnd1Flat::CopyAttributesTo(SRndHole1FlatAttributes *pAttrib)
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
	GetAttributes().CopyFrom(pAttrib);
}

void CCadHoleRnd1Flat::CopyAttributesFrom(SRndHole1FlatAttributes *pAttrib)
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

ObjectDrawState CCadHoleRnd1Flat::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("Round Hole with One Flat:Place Center Point"));
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
		m_Center = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		m_Center = MousePos;
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETVIEW()->AddObjectAtFrontIntoDoc(this);
		GETVIEW()->SetObjectTypes(new CCadHoleRnd1Flat);
		GETAPP.UpdateStatusBar(_T("Round Hole with One Flat Side:Place Center Point"));
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadHoleRnd1Flat::MouseMove(ObjectDrawState DrawState)
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
		m_Center = MousePos;
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}

int CCadHoleRnd1Flat::EditProperties()
{
	int Id;
	CDlgRndHole1Flat Dlg;
	Dlg.SetRndHole1Flat(this);
	Id = Dlg.DoModal();
	return Id;
}

BOOL CCadHoleRnd1Flat::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadHoleRnd1Flat::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}
