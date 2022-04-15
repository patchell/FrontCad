#include "pch.h"

CCadHoleRnd2Flat::CCadHoleRnd2Flat():CCadObject()
{
	m_pPenLine = 0;
	SetType(ObjectType::HOLE_RND2FLAT);
	GetName().Format(_T("RoundHole2Flat_%d"), ++m_RoundHole2FlatCount);
	if (!m_AttributesGood)
	{
		m_AttributesGood = TRUE;
		m_LastAttributes.CopyFrom(GETAPP.GetRoundHole2FlatAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadHoleRnd2Flat::~CCadHoleRnd2Flat()
{
}

void CCadHoleRnd2Flat::Move(CDoubleSize Diff)
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

void CCadHoleRnd2Flat::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadHoleRnd2Flat::SetVertex(int v, CDoubleSize sz)
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


int CCadHoleRnd2Flat::GrabPoint(CDoublePoint p)
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


void CCadHoleRnd2Flat::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
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
	double FlatDistance;
	double Radius;
	double dx;
	CPoint CenterPoint;
	double Angle;
	CDoublePoint C1, C2, C3, C4;	//cross targer
	CDoublePoint P1, P2, P3, P4;
	int Lw;


	if (CCadHoleRnd2Flat::m_RenderEnable)
	{
		FlatDistance = GetAttributes().m_FlatDistanceFromCenter;
		Radius = GetAttributes().m_HoleRadius;

		dx = 3.0 * Radius / 4.0;
		C1 = m_Center + CDoubleSize(dx, dx);
		C2 = m_Center + CDoubleSize(-dx, dx);
		C3 = m_Center + CDoubleSize(-dx, -dx);
		C4 = m_Center + CDoubleSize(dx, -dx);

		P1 = SolveIntersection(Intercept::LEFTUPPER, m_Center, FlatDistance, Radius);
		P2 = SolveIntersection(Intercept::LEFTLOWER, m_Center, FlatDistance, Radius);
		P3 = SolveIntersection(Intercept::RIGHTUPPER, m_Center, FlatDistance, Radius);
		P4 = SolveIntersection(Intercept::RIGHTLOWER, m_Center, FlatDistance, Radius);

		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.GetScaleX());
		if (Lw < 1) Lw = 1;

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
		CenterPoint = m_Center.ToPixelPoint(Offset, Scale);
		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
		case ObjectDrawMode::SELECTED:
		case ObjectDrawMode::SKETCH:
			pOldPen = pDC->SelectObject(m_pPenLine);
			Angle = CalculateAngle(FlatDistance, Radius);
			pDC->AngleArc(
				CenterPoint.x,
				CenterPoint.y,
				GETAPP.RoundDoubleToInt(Radius * Scale.GetScaleX()),
				float(Angle),
				float(180 - 2 * Angle)
			);
			pDC->AngleArc(
				CenterPoint.x,
				CenterPoint.y,
				GETAPP.RoundDoubleToInt(Radius * Scale.GetScaleX()),
				float(Angle + 180.0),
				float(180 - 2 * Angle)
			);
			pDC->MoveTo(P1.ToPixelPoint(Offset, Scale));
			pDC->LineTo(P2.ToPixelPoint(Offset, Scale));
			pDC->MoveTo(P3.ToPixelPoint(Offset, Scale));
			pDC->LineTo(P4.ToPixelPoint(Offset, Scale));

			pDC->MoveTo(C1.ToPixelPoint(Offset, Scale));
			pDC->LineTo(C3.ToPixelPoint(Offset, Scale));
			pDC->MoveTo(C2.ToPixelPoint(Offset, Scale));
			pDC->LineTo(C4.ToPixelPoint(Offset, Scale));

			pDC->SelectObject(pOldPen);
			break;
		}
		SetLastMode(mode);
	}
}

double CCadHoleRnd2Flat::CalculateAngle(
	double FlatDist,
	double Radius
)
{
	double Angle;
	Angle = acos(FlatDist / Radius);
	Angle = Angle * GETAPP.GetPi() / 180.0;
	return Angle;
}

CDoublePoint CCadHoleRnd2Flat::SolveIntersection(
	Intercept mode,
	CDoublePoint Center,
	double FlatDist,
	double Radius
)
{
	CDoublePoint Result;
	switch (mode)
	{
	case Intercept::LEFTUPPER:
		Result = CDoublePoint(
			Center.dX - FlatDist,
			Center.dY + sqrt(Radius * Radius - FlatDist * FlatDist)
		);
		break;
	case Intercept::LEFTLOWER:
		Result = CDoublePoint(
			Center.dX - FlatDist,
			Center.dY - sqrt(Radius * Radius - FlatDist * FlatDist)
		);
		break;
	case Intercept::RIGHTUPPER:
		Result = CDoublePoint(
			Center.dX + FlatDist,
			Center.dY + sqrt(Radius * Radius - FlatDist * FlatDist)
		);
		break;
	case Intercept::RIGHTLOWER:
		Result = CDoublePoint(
			Center.dX + FlatDist,
			Center.dY - sqrt(Radius * Radius - FlatDist * FlatDist)
		);
		break;
	}
	return Result;
}

int CCadHoleRnd2Flat::PointInObjectAndSelect(
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
	return 0;
}

CDoublePoint CCadHoleRnd2Flat::GetReference()
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


CDoubleRect& CCadHoleRnd2Flat::GetRect(CDoubleRect& rect)
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
	double Radius = GetHoleRadius();
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

CString& CCadHoleRnd2Flat::GetTypeString(void)
{
	//***************************************************
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Rnd Hole 2 Flats");
	return csName;
}

CCadHoleRnd2Flat CCadHoleRnd2Flat::operator=(CCadHoleRnd2Flat &v)
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
	return CCadHoleRnd2Flat();
}

CPoint CCadHoleRnd2Flat::ScalePoint(CPoint p, int Scale, int ScaleDiv)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------
	return CPoint();
}

CCadObject * CCadHoleRnd2Flat::CopyObject(void)
{
	//***************************************************
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadHoleRnd2Flat *pHR = new CCadHoleRnd2Flat;
	*pHR = *this;
	return pHR;
}

void CCadHoleRnd2Flat::SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw)
{
	//***************************************************
	// parameters:
	//
	// return value:
	//--------------------------------------------------

}

void CCadHoleRnd2Flat::RenderEnable(int e)
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

CDoublePoint CCadHoleRnd2Flat::GetCenter()
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

CDoubleSize CCadHoleRnd2Flat::GetSize()
{
	//***************************************************
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	double Radius = GetHoleRadius();
	return CDoubleSize(Radius,Radius);
}

DocFileParseToken CCadHoleRnd2Flat::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
{
	//***************************************************
	// Parse
	//	This Method is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token....Lookahead Token for this object
	//	pLex.....Pointer to Lexer for the data stream
	//	TypeToken..Token type for this parse
	// return value:
	//	returns lookahead token on success, or
	//			negative value on error
	//--------------------------------------------------
	return Token;
}

void CCadHoleRnd2Flat::CopyAttributesTo(SRndHole2FlatAttributes *pAttrib)
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

void CCadHoleRnd2Flat::CopyAttributesFrom(SRndHole2FlatAttributes *pAttrib)
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

ObjectDrawState CCadHoleRnd2Flat::ProcessDrawMode(ObjectDrawState DrawState)
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
		GETAPP.UpdateStatusBar(_T("Round Hole with Two Flat Side"));
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
		GETVIEW()->SetObjectTypes(new CCadHoleRnd2Flat);
		GETAPP.UpdateStatusBar(_T("Round Hole with Two Flat Side"));
		GETVIEW()->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadHoleRnd2Flat::MouseMove(ObjectDrawState DrawState)
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

int CCadHoleRnd2Flat::EditProperties()
{
	int Id;
	CDlgRndHole2FlatProp Dlg;
	Dlg.SetRndHole2Flat(this);
	Id = Dlg.DoModal();
	return Id;
}

BOOL CCadHoleRnd2Flat::NeedsAttributes()
{
	return (m_AttributesGood == FALSE);
}

void CCadHoleRnd2Flat::ClearNeedsAttributes()
{
	m_AttributesGood = TRUE;
}
