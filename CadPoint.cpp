#include "pch.h"
#include "CadPoint.h"

CCadPoint::CCadPoint():CCadObject(ObjectType::POINT)
{
	dX = 0.0;
	dY = 0.0;
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetPointAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
	GetName().Format(_T("Point_%d"), ++m_PointCount);
}

CCadPoint::CCadPoint(double x, double y)
{
	dX = x;
	dY = y;
	SetType(ObjectType::POINT);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetPointAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
	GetName().Format(_T("Point_%d"), ++m_PointCount);
}

CCadPoint::CCadPoint(DOUBLEPOINT dp)
{
	dX = dp.dX;
	dY = dp.dY;
	SetType(ObjectType::POINT);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetPointAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
	GetName().Format(_T("Point_%d"), ++m_PointCount);
}

CCadPoint::CCadPoint(DOUBLEPOINT dp, SubTypes Sub, UINT SubSub)
{
	dX = dp.dX;
	dY = dp.dY;
	SetType(ObjectType::POINT);
	SetSubType(Sub);
	SetSubSubType(SubSub);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetPointAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
	GetName().Format(_T("Point_%d"), ++m_PointCount);
}

CCadPoint::~CCadPoint()
{
}

BOOL CCadPoint::Create(CCadObject* pParent, SubTypes type)
{
	CCadObject::Create(pParent, type);
	return TRUE;
}

void CCadPoint::UpdateEnclosure()
{
}

CString& CCadPoint::GetObjDescription()
{
	GetDescription().Format(_T("Point(%6.3lf,%6.3lf)"), dX,dY);
	return GetDescription();
}

CString& CCadPoint::GetTypeString()
{
	//--------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("POINT"));
	return csTypeName;
}

BOOL CCadPoint::IsPointOnTarget(DOUBLEPOINT point)
{
	BOOL Result = FALSE;
	CDoubleSize Difference;
	double distance;
	char* s = new char[256];
	
	//--------------------------
	// would like a rectangle
	// with a 10 pixel margin
	//-------------------------
	Difference = *this - point;
	distance = Difference.Magnitude();
	if (distance < GETVIEW->GetGrid().GetSnapGrid().Magnitude())
		Result = TRUE;
//	sprintf_s(s, 256, "Is the Point Hovered Over? --%hs", Result ? "Yes" : "NO!");
//	Print(s);
	delete[] s;
	return Result;
}

void CCadPoint::Move(CDoubleSize Diff)
{
	dX += Diff.dCX;
	dY += Diff.dCY;
}

BOOL CCadPoint::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

BOOL CCadPoint::PointInThisObject(DOUBLEPOINT point)
{
	return IsPointOnTarget(point);
}

int CCadPoint::PointInObjectAndSelect(
	DOUBLEPOINT p, 
	CCadObject* pExcludeObject,
	CCadObject** ppSelList,
	int index, 
	int n,
	UINT nKinds
)
{
	//---------------------------------------------------
	// PointInObjectAndSelect
	//	This Method is used to see if an object can
	// be selected at point p.
	// 
	// The CCadPoint Object is special because it
	// will never have any children, so this is the
	// end of the line.
	//
	// parameters:
	//	p...........point to check at
	//	ppSelList...pointer to list of selected objects
	//	index.......current index into the selection list
	//	n...........Total number of spaces in selection list
	//	flag........Determines what sort of objects selected
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------
	if (index < n)	//Is there room?
	{
		//-----------------------------------------------
		// is the point on target?
		//-----------------------------------------------
		if (PointInThisObject(p))
		{
			if (IsItThisKind(nKinds) && DontExclude(pExcludeObject))
			{
				ppSelList[index++] = this;
				ShouldWeSelectThisObjectAndDidIt(nKinds);
			}
		}
		//------------------------------------
		// CCadPoint is a terminal object.
		// It will not contain any other
		// Objects
		// Recursion stops here
		//-----------------------------------
	}
	return index;
}

CCadObject* CCadPoint::Copy()
{
	CCadPoint* pCP;

	pCP = new CCadPoint;
	pCP->SetX(dX);
	pCP->SetY(dY);
	pCP->CopyAttributesFrom(GetPtrToAttributes());
	return pCP;
}

void CCadPoint::CopyAttributes(CCadObject* pToObj)
{
	((CCadPoint*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}

void CCadPoint::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
{
	//---------------------------------------------------
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	LLHC....Offset to draw objects at
	//	Scale..Scale factor to draw objects at
	//
	// return value:none
	//--------------------------------------------------
	CPoint PP;
	int x=0, y=0;
	CBrush brushFill,*oldBrush;
	CPen penOutline, * oldPen;
	CRect rect;
	
	if (IsRenderEnabled())
	{
		PP = ToPixelPoint(LLHC, Scale);
		rect.SetRect(PP - CSize(2, 2), PP + CSize(2, 2));
		CreateThePen(mode, &penOutline, 1);
		CreateTheBrush(mode, &brushFill);
		oldBrush = pDC->SelectObject(&brushFill);
		oldPen = pDC->SelectObject(&penOutline);
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
		case MODE::ObjectPaintMode::SKETCH:
			pDC->Rectangle(&rect);
			break;
		}
		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}
}

void CCadPoint::MoveTo(CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale)
{
	pDC->MoveTo(ToPixelPoint(LLHC, Scale));
}

void CCadPoint::LineTo(CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale)
{
	pDC->LineTo(ToPixelPoint(LLHC, Scale));
}

void CCadPoint::MoveTo(CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale, CCadPoint* pPivot, CCadPoint* pRotation)
{
	pDC->MoveTo(ToPixelPointWithRotation(LLHC, Scale, pPivot, pRotation));
}

void CCadPoint::LineTo(CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale, CCadPoint* pPivot, CCadPoint* pRotation)
{
	pDC->LineTo(ToPixelPointWithRotation(LLHC, Scale, pPivot, pRotation));
}

void CCadPoint::LineFromHereToThere(DOUBLEPOINT There, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale)
{
	pDC->MoveTo(ToPixelPoint(LLHC, Scale));
	pDC->LineTo(There.ToPixelPoint(LLHC, Scale.GetScaleX(), Scale.GetScaleY()));
}

void CCadPoint::LineFromHereToThere(CDoubleSize There, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale)
{
	CCadPoint pointThere;

	pointThere.SetPoint(DOUBLEPOINT(dX,dY )+ There);
	MoveTo(pDC, LLHC, Scale);
	pointThere.LineTo(pDC, LLHC, Scale);
}

void CCadPoint::LineFromHereToThere(CCadPoint* pThere, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale)
{
	MoveTo(pDC, LLHC, Scale);
	pThere->LineTo(pDC, LLHC, Scale);
}

BOOL CCadPoint::FloodFill(
	COLORREF colorBorder,
	COLORREF colorFill,
	CDC* pDC, 
	DOUBLEPOINT& LLHC,
	CScale& Scale
)
{
	BOOL rV;
	CPoint p;

	p = ToPixelPoint(LLHC, Scale);
	rV = pDC->FloodFill(p.x, p.y, colorBorder);
	return rV;
}

CPoint CCadPoint::ToPixelPoint(DOUBLEPOINT& LLHC, CScale& Scale)
{
	DOUBLEPOINT dp = (GetPoint() - LLHC);	//offset to client window
	dp = dp * Scale;	//convert to pixels
	return CPoint(dp);
}

CPoint CCadPoint::ToPixelPointWithRotation(
	DOUBLEPOINT& LLHC, 
	CScale& Scale, 
	CCadPoint* pPivot, 
	CCadPoint* pRotation
)
{
	CDoubleSize cdsRot;
	double Angle;
	DOUBLEPOINT dpP;
	double dx, dy;
	CPoint ptPoint;
	
	cdsRot = *pRotation - *pRotation;
	Angle = cdsRot.Angle(DOUBLESIZE_RADIANS);
	dx = cos(Angle);
	dy = sin(Angle);
	dx += 2 * dX - pRotation->dX;
	dy += 2 * dY - pRotation->dY;
	return CPoint(CCadPoint(dx,dy).ToPixelPoint(LLHC,Scale));
}

void CCadPoint::Parse(
	CParser* pParser,
	Token TypeToken
)
{
}

void CCadPoint::Save(
	CFile* pcfFile, 
	int Indent
)
{
	char* pcBuffer = new char[1024];
	char* pcIndent = new char[256];
	char* pS1 = new char[64];
	char* pS2 = new char[256];
	char* pS3 = new char[64];
	int n;

	GETAPP.IndentString(pcIndent, 256, Indent, ' ');
	n = sprintf_s(
		pcBuffer, 
		1024,
		"%hs%hs(%hs,%hs,%hs:%d){\n",
		pcIndent,
		CLexer::TokenLookup(Token::POINT),
		CParser::SaveDoubleValue(pS1,64,dX),	//X coordinate
		CParser::SaveDoubleValue(pS2,64,dY),	//Y coordinate
		GetSubTypeString(pS3, GetSubType()),
		GetSubSubType()
	);
	pcfFile->Write(pcBuffer, n);
	GetAttributes().Save(pcfFile, Indent + 1);
	delete[] pcBuffer;
	delete[] pcIndent;
	delete[]pS1;
	delete[]pS2;
	delete[]pS3;
}

void CCadPoint::CopyAttributesTo(SPointAttributes* pAttrb)
{
	//---------------------------------------------------
	//	CopyAttributesTo
	//		This Method is used to copy the
	//	attributes from this object into one pointed
	//	to by the parameter.
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy into
	//-------------------------------------------------/
	GetAttributes().CopyTo(pAttrb);
}

void CCadPoint::CopyAttributesFrom(SPointAttributes* pAttrb)
{
	GetAttributes().CopyFrom(pAttrb);
	SetAttributesValid();
}

ObjectDrawState CCadPoint::ProcessDrawMode(ObjectDrawState DrawState)
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
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();
	CPoint MouseScreenCoordinate;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Point:Place Point"));
		break;
	case ObjectDrawState::END_DRAWING:
		if (m_AttributesDirty)
		{
			Id = GETVIEW->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
			if (IDYES == Id)
			{
				m_CurrentAttributes.CopyTo(&m_LastAttributes);
			}
			m_AttributesDirty = FALSE;
		}
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		GETVIEW->GetCursorPosition(&MouseScreenCoordinate);
		Id = EditProperties();
		if (IDOK == Id)
		{
			if (m_AttributesDirty)
				CopyAttributesTo(&m_CurrentAttributes);
			DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
			GETVIEW->EnableAutoScroll(1);
		}
		GETVIEW->SetCursorPosition(MouseScreenCoordinate);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		GETVIEW->EnableAutoScroll(TRUE);
		SetPoint(MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		SetPoint(MousePos);
		GetParent()->AddObjectAtTail(this);
		GETVIEW->SetObjectTypes(new CCadPoint);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Point:Place Point"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadPoint::MouseMove(ObjectDrawState DrawState)
{
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();
	MousePos = GETVIEW->GetCurrentMousePosition();

	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		SetPoint(MousePos);
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

int CCadPoint::EditProperties()
{
	int Id;
	CDlgPointProperties Dlg;

	Dlg.SetPoint(this);
	Id = Dlg.DoModal();
	return 0;
}

void CCadPoint::ToPixelRect(CCadPoint* pP2, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale)
{
	CRect Rect;

	Rect.SetRect(ToPixelPoint(LLHC, Scale), pP2->ToPixelPoint(LLHC, Scale));
	pDC->Rectangle(&Rect);
}

void CCadPoint::ToPixelRect(double w_half, double h_half, CRect& rect, DOUBLEPOINT& LLHC, CScale& Scale)
{
	//------------------------------------
	// "this" is the center point of the
	// rectangle.  Calculate the CRect
	// in view corrordinates.
	// 
	// Parameters:
	// w_half.......half of the width
	// h_half.......half of the height
	// rect.........reference to where to put CRect
	// LLHC.........coordinate of the uper left corner of view
	// Scale........Pixels per Inch
	//------------------------------------
	CPoint p1, p2;
	CSize szRect;

	szRect = CDoubleSize(w_half, h_half).ToPixelSize(Scale);
	p1 = ToPixelPoint(LLHC, Scale) - szRect;
	p2 = ToPixelPoint(LLHC, Scale) + szRect;
	rect.SetRect(p1, p2);
}

void CCadPoint::ToPixelRect(CCadPoint* pP2, CRect& Rect, DOUBLEPOINT& LLHC, CScale& Scale)
{
	Rect.SetRect(ToPixelPoint(LLHC, Scale), pP2->ToPixelPoint(LLHC, Scale));
}

void CCadPoint::ToPixelArc(
	CCadPoint* pP2, 
	CCadPoint* pStart, 
	CCadPoint* pEnd, 
	CDC* pDC, 
	DOUBLEPOINT& LLHC,
	CScale& Scale
)
{
	CRect Rect;

	Rect.SetRect(ToPixelPoint(LLHC, Scale), pP2->ToPixelPoint(LLHC, Scale));
	pDC->Arc(&Rect,pStart->ToPixelPoint(LLHC,Scale),pEnd->ToPixelPoint(LLHC,Scale));
}

void CCadPoint::ToPixelArc(
	double Radius_A,	// radius in X direction
	double Radius_B,	// radius in Y direction
	CCadPoint* pStart,	// arc start point
	CCadPoint* pEnd,	//arc end point
	CDC* pDC,			//device context
	DOUBLEPOINT& LLHC,	//upper left hand corner coordinate
	CScale& Scale		//pixels per inch
)
{
	//-----------------------------------------
	// ToPixelArc
	// "this" is the center point of the arc
	//-----------------------------------------
	CRect Rect;
	
	ToPixelRect(Radius_A, Radius_B, Rect, LLHC, Scale);
	pDC->Arc(
		&Rect,
		pStart->ToPixelPoint(LLHC, Scale),
		pEnd->ToPixelPoint(LLHC, Scale)
	);
}

void CCadPoint::ToPixelEllipse(CCadPoint* pP2, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale)
{
	CRect Rect;

	Rect.SetRect(ToPixelPoint(LLHC, Scale), pP2->ToPixelPoint(LLHC, Scale));
	pDC->Ellipse(&Rect);
}

void CCadPoint::ToPixelRndRect(CCadPoint* pP2, CCadPoint* pP3, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale)
{
}

CCadPoint* CCadPoint::Reflect(CCadPoint* pReflect, UINT mode)
{
	//-------------------------------------
	// Reflect
	// 
	// Reflects this point around point
	// pReflect
	//-------------------------------------
	
	double deltaX, deltaY;

	switch (mode)
	{
	case POINT_REFLECT_BOTH:
		deltaX = dX - pReflect->dX;
		deltaY = dY - pReflect->dY;
		dX = pReflect->dX - deltaX;
		dY = pReflect->dY - deltaY;
		break;
	case POINT_REFLECT_X:
		//-------------------------------
		// Reflect Along the X axis
		//-------------------------------
		deltaY = dY - pReflect->dY;
		dY = pReflect->dY - deltaY;
		break;
	case POINT_REFLECT_Y:
		//-------------------------------
		// Reflect Along the Y axis
		//-------------------------------
		deltaX = dX - pReflect->dX;
		dX = pReflect->dX - deltaX;
		break;
	}
	return this;
}

UINT CCadPoint::Slope(double *pSlope, CCadPoint* pPoint) const
{
	//-----------------------------------
	// Get the slope of the line defined
	// by m_P1, m_P2
	//-----------------------------------
	UINT SlopeType;

	if (dY == pPoint->dY)
	{
		SlopeType = SLOPE_HORIZONTAL;
		*pSlope = 0.0;
	}
	else 	if (dX == pPoint->dX)
		SlopeType = SLOPE_VERTICAL;
	else
	{
		SlopeType = SLOPE_NOT_ORTHOGONAL;
		*pSlope = (dY - pPoint->dY) / (dX - pPoint->dX);
	}
	return SlopeType;
}

UINT CCadPoint::Slope(double* pSlope, DOUBLEPOINT point) const
{
	//-----------------------------------
	// Get the slope of the line defined
	// by m_P1, m_P2
	//-----------------------------------
	UINT SlopeType;

	if (dY == point.dY)
	{
		SlopeType = SLOPE_HORIZONTAL;
		*pSlope = 0.0;
	}
	else 	if (dX == point.dX)
		SlopeType = SLOPE_VERTICAL;
	else
	{
		SlopeType = SLOPE_NOT_ORTHOGONAL;
		*pSlope = (dY - point.dY) / (dX - point.dX);
	}
	return SlopeType;
}

UINT CCadPoint::OrthogonalSlope(double* pSlope, CCadPoint *pPoint)
{
	//------------------------------------------------
	// Get the slope of the line defined
	// by m_P1, m_P2 that is perpendicular
	// 
	// *this.....first point of line
	// parameters:
	// pSlope....pointer to where slope is returned
	//           if the slope is vertical, this
	//           location is not set
	// pPoint....other end of line
	// 
	// returns:
	// Type of slope, HORZ, VERT and !(HORZ || VERT)
	//------------------------------------------------
	UINT SlopeType;

	if (dY == pPoint->dY)
	{
		SlopeType = SLOPE_HORIZONTAL;
	}
	else if (dX == pPoint->dX)
	{
		SlopeType = SLOPE_VERTICAL;
		*pSlope = 0.0;
	}
	else
	{
		SlopeType = SLOPE_NOT_ORTHOGONAL;
		*pSlope = -(dX - pPoint->dX) / (dY - pPoint->dY);
	}
	return SlopeType;
}

UINT CCadPoint::LineIs(DOUBLEPOINT OtherPoint)
{
	//-----------------------------------------
	// LisneIs
	// Determins if the line is Horizontal, 
	// Vertical, or somewhere in between
	// The line is between this and the
	// OtherPoint
	//-----------------------------------------

	UINT rV = LINE_IS_WHATEVER;	//line is whatever

	if (dY == OtherPoint.dY)
		rV = LINE_IS_HORIZONTAL;
	else if (dX == OtherPoint.dX)
		rV = LINE_IS_VERTICAL;
	return rV;
}

double CCadPoint::YIntercept(double m)
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
	//          goes through
	// Return Vaule: the Y intercept
	//-----------------------------
	double b;

	b = dY - m * dX;
	return b;
}

double CCadPoint::XIntercept(double m)
{
	//-----------------------------
	// finds the X intercept fopr
	// a line that goes through
	// point p with slope m
	//
	// x = y/m + a or
	// a = x - y/m
	// parameters:
	//	m.......slope of the line
	//          goes through
	// Return Vaule: the Y intercept
	//-----------------------------
	double a;

	a = dX - dY/m;
	return a;
}

BOOL CCadPoint::PointOnLineAtDistance(double d, double m, double b, CCadPoint& dest1, CCadPoint& dest2)
{
	//---------------------------------------
	// PointOnLineAtDistance
	//	This method will calculate a point
	// at distance d from this point with
	// slope m and y intercept b
	// 
	// Parameters:
	//	d............distance
	//	m............slope of the line
	//	b............Y intercept
	//	dest1........destination of solution 1
	//	dest2........destination of solution 2
	// 
	// Returns: False on fail (no real solution)
	//			TRUE on success.  Answers are
	//			in Dest1 and Dest2
	//---------------------------------------
	BOOL rV = FALSE;
	double A, B, C;
	double t;
	double x1, x2;
	t = dY - b;
	A = 1.0 + m * m;
	B = 2.0 * (dX + m * t);
	C = dX * dX + t * t - d * d;
	if (GETAPP.QuadradicEquation(A, B, C, x1, x2))
	{
		rV = TRUE;
	}
	return rV;
}

BOOL CCadPoint::PointOnLineAtDistance(DOUBLEPOINT P1, DOUBLEPOINT P2, double Distance)
{
	//--------------------------------
	// PointOnLineAtDistance
	// Given a "pivot" point and a
	// point at the other end of a
	// line though the pivot, find a
	// point on that line a given
	// distance from the pivot
	// 
	// parameters:
	// P1....... point (Pivotorigin)
	// P2.......A point that dfines 
	//			the line
	// distance.Distance from the
	//			pivot points we want
	//			to know the point of.
	// 
	// Result:
	// The value of this object
	// (CCadPoint) will be set to
	// the point we are looking for.
	// 
	// Returns:
	//	True on sucess.  If the line
	// is vertical, returns FALSE.
	//--------------------------------
	BOOL rV = FALSE;
	double X, Y;
	double m;

	if (P1.dX != P2.dX)
	{
		m = (P2 - P1).Slope();
		X = sqrt((Distance * Distance) / (1 + m * m));
		Y = m * X;
		if (Distance < 0)
		{
			SetX(P1.dX - X);
			SetY(P1.dY - Y);
		}
		else
		{
			SetX(P1.dX + X);
			SetY(P1.dY + Y);
		}
		rV = TRUE;
	}
	return rV;
}

void CCadPoint::PointOnLineAtDistance(
	CCadPoint* pP1, 
	double m, 
	double Distance
)
{
	//--------------------------------
	// PointOnLineAtDistance
	// Given a "pivot" point and a
	// slope of a line through
	// that point, find the 
	// point on that line at the given
	// distance from the pivot
	// 
	// parameters:
	// pP1.......Pivot point (origin)
	// m.......Slope of line through 
	//			the Pivot
	// distance.Distance from the
	//			pivot points we want
	//			to know the point of.
	// 
	// Result:
	// The value of this object
	// (CCadPoint) will be set to
	// the point we are looking for.
	// 
	//--------------------------------

	double X, Y;

	X = sqrt((Distance * Distance) / (1 + m * m));
	Y = m * X;
	if (Distance < 0)
	{
		SetX(pP1->dX - X);
		SetY(pP1->dY - Y);
	}
	else
	{
		SetX(pP1->dX + X);
		SetY(pP1->dY + Y);
	}
}

void CCadPoint::PointOnLineAtDistance(
	DOUBLEPOINT P1, 
	double m, 
	double Distance
)
{
	//--------------------------------
	// PointOnLineAtDistance
	// Given a "pivot" point and a
	// slope of a line through
	// that point, find the 
	// point on that line at the given
	// distance from the pivot
	// 
	// parameters:
	// P1.......Pivot point (origin)
	// m.......Slope of line through 
	//			the Pivot
	// distance.Distance from the
	//			pivot points we want
	//			to know the point of.
	// 
	// Result:
	// The value of this object
	// (CCadPoint) will be set to
	// the point we are looking for.
	// 
	//--------------------------------

	double X, Y;

	X = sqrt((Distance * Distance) / (1 + m * m));
	Y = m * X;
	if (Distance < 0)
	{
		SetX(P1.dX - X);
		SetY(P1.dY - Y);
	}
	else
	{
		SetX(P1.dX + X);
		SetY(P1.dY + Y);
	}
}

void CCadPoint::PointOnLineAtDistance(
	CCadPoint* pP1, 
	DOUBLEPOINT RotationPoint, 
	double Distance
)
{
	//-----------------------------------
	// PointOnLineAtDistance
	//	Calculates coordinates of
	// this point based on the the
	// first point, and a point that
	// defines the angle that the
	// point is rotated at
	// If the snap is set to
	// something, the point will
	// get snapped.
	// 
	// parameters:
	//	P1.......first point of the line
	// RotationPoint...yep, that, defines where line goes to
	// Distance...between points 
	//-----------------------------------

	UINT LineType;
	double m;

	LineType = pP1->LineIs(RotationPoint);
	switch (LineType)
	{
	case LINE_IS_WHATEVER:	//Hard
		pP1->Slope(&m, RotationPoint);
		if(pP1->GetX() < RotationPoint.dX)
			dX = sqrt((Distance * Distance) / (1 + m * m));
		else
			dX = -sqrt((Distance * Distance) / (1 + m * m));
		dY = m * dX;
		dX += pP1->GetX();
		dY += pP1->GetY();
		break;
	case LINE_IS_VERTICAL:	//easy
		SetX(pP1->GetX());
		if(pP1->GetY() > RotationPoint.dY)
			SetY(pP1->GetY() - Distance);
		else
			SetY(pP1->GetY() + Distance);
		break;
	case LINE_IS_HORIZONTAL:	//easy
		if(pP1->GetX() < RotationPoint.dX)
			SetX(pP1->GetX() + Distance);
		else
			SetX(pP1->GetX() - Distance);
		SetY(pP1->GetY());
		break;
	}
}

CCadPoint* CCadPoint::CenterPoint(CCadPoint* pFirstPoint, CCadPoint* pOtherPoint)
{
	//----------------------------------------------------
	// CenterPoint
	// Find the center point between pFirstPoint and
	// pOtherPoint and put the results in this and
	// return this.
	//----------------------------------------------------

	dX = (pFirstPoint->dX + pOtherPoint->dX) / 2.0;
	dY = (pFirstPoint->dY + pOtherPoint->dY) / 2.0;
	return this;
}

BOOL CCadPoint::IsPointBetween(CCadPoint* pP1, CCadPoint* pP2)
{
	BOOL rV = FALSE;

	if (pP1->GetX() < GetX() &&
		GetX() < pP2->GetX() &&
		pP1->GetY() < GetY() &&
		GetY() < pP2->GetY()
		)
		rV = TRUE;
	return rV;
}

int CCadPoint::WhichSideOfLineIsPoint(CCadPoint* pPtOther, DOUBLEPOINT Point)
{
	double b;
	double m;
	double b_prime;
	UINT TypeOfSlope;
	int rV = 0;

	TypeOfSlope = Slope(&m, pPtOther);
	switch (TypeOfSlope)
	{
	case SLOPE_HORIZONTAL:
		if (Point.dY > dY)
			rV = 1;
		else if (Point.dY < dY)
			rV = -1;
		break;
	case SLOPE_VERTICAL:
		if (Point.dX > dX)
			rV = 1;
		else if (Point.dX < dX)
			rV = -1;
		break;
	case SLOPE_NOT_ORTHOGONAL:
		b = YIntercept(m);
		b_prime = Point.YIntercept(m);
		if (m > 0 && b_prime > b)
			rV = -1;
		else if (m > 0 && b_prime < b)
			rV = 1;
		else if (m < 0 && b_prime > b)
			rV = 1;
		else if (m < 0 && b_prime < b)
			rV = -1;
		break;
	}
	return rV;
}

double CCadPoint::DistanceTo(CCadPoint* pP)
{
	double distance;
	double rise;
	double run;

	rise = dY - pP->dY;
	run = dX - pP->dX;
	distance = sqrt(rise * rise + run * run);
	return distance;
}

void CCadPoint::Print(const char* s)
{
	char* temp = new char[256];
	printf("%hs::X=%7.3lf, Y=%7.3lf SubType:%hs  ID=%d\n", 
		s, 
		dX, 
		dY,
		GetCharSubTypeString(temp,GetSubType()),
		GetId()
	);
	delete[]temp;
}

void CCadPoint::CreateThePen(MODE mode, CPen* pen, int Lw)
{
	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if (IsSelected())
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
		else
			pen->CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
		break;
	case MODE::ObjectPaintMode::SKETCH:
		pen->CreatePen(PS_DOT, Lw, GetAttributes().m_colorSelected);
		break;
	}
}

void CCadPoint::CreateTheBrush(MODE mode, CBrush* brushFill)
{
	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if (IsSelected())
			brushFill->CreateSolidBrush(GetAttributes().m_colorSelected);
		else
			brushFill->CreateSolidBrush(GetAttributes().m_colorLine);
		break;
	case MODE::ObjectPaintMode::SKETCH:
		brushFill->CreateSolidBrush(GetAttributes().m_colorSelected);
		break;
	}
}
