#include "pch.h"
#include "DoubleRect.h"

RectPoint operator++(RectPoint& v)
{
	RectPoint rV;
	rV = RectPoint(int(v) + 1);
	return rV;
}

RectSide operator++(RectSide& v)
{
	RectSide rV;
	rV = RectSide(int(v) + 1);
	return rV;
}

CDoubleRect::CDoubleRect()
{
	m_RotationAngle = 0.0;
	m_dptPivotPoint = CDoublePoint(0.0, 0.0);
}

CDoubleRect::~CDoubleRect()
{
}

CDoubleRect CDoubleRect::SetPoints(
	CRect rect,
	CDoublePoint ptPivot,
	double Angle
)
{
	// ---------------------------------------------
	// SetPoints
	//	This method creates the
	// rotated at a given angle.
	// parameters:
	//	rect...........defines rectangle
	// 	ptPivot........Point to rotate rect around
	// Angle...........Angle of rotation in degrees
	// ---------------------------------------------
	// create a "normalized rectangle
	rect.NormalizeRect();
	m_dptPoints[0] = CDoublePoint(rect.TopLeft());
	m_dptPoints[2] = m_dptPoints[0] + CDoubleSize(rect.Width(), rect.Height());
	m_dptPoints[1] = m_dptPoints[0] + CDoubleSize(0, rect.Height());
	m_dptPoints[3] = m_dptPoints[0] + CDoubleSize(rect.Width(), 0);
	// rotate the rectangle by Angle degrees
	Rotate(ptPivot, Angle, RectPoint::CENTER);
	return *this;
}

CDoubleRect CDoubleRect::SetPoints(
	CDoublePoint dptRefPoint,
	CDoubleSize dptSize,
	CDoublePoint ptPivot,
	double Angle
)
{
	// ---------------------------------------------
	// SetPoints
	//	This method creates the
	// rotated at a given angle.
	// parameters:
	//	dptRefPoint....reference point
	//	dptSize........size of the rectangle
	// 	ptPivot........Point to rotate rect around
	// Angle...........Angle of rotation in degrees
	// ---------------------------------------------
	// create a "normalized rectangle
	m_dptPoints[0] = dptRefPoint;
	m_dptPoints[2] = dptRefPoint + dptSize;
	m_dptPoints[1] = dptRefPoint + CDoubleSize(dptSize.dCX, 0.0);
	m_dptPoints[3] = dptRefPoint + CDoubleSize(0.0, dptSize.dCY);
	// rotate the rectangle by Angle degrees
	Rotate(ptPivot, Angle, RectPoint::CENTER);
	return *this;
}

CDoubleRect CDoubleRect::SetPoints(CDoublePoint* pdptPoints)
{
	int i;

	for (i = 0; i < 4; ++i)
		m_dptPoints[i] = pdptPoints[i];
	m_dptPivotPoint = m_dptPoints[0];
	return *this;
}

CDoubleRect CDoubleRect::SetPoints(CDoublePoint P1_UpperLeft, CDoublePoint P2_LowerRight)
{
	CDoubleSize Diff;

	Diff = P2_LowerRight - P1_UpperLeft;
	
	m_dptPoints[UINT(RectPoint::UPPERLEFT)] = P1_UpperLeft;
	m_dptPoints[UINT(RectPoint::LOWERRIGHT)] = P2_LowerRight;
	m_dptPoints[UINT(RectPoint::LOWERLEFT)] = P2_LowerRight - CDoubleSize(0.0, Diff.dCY);
	m_dptPoints[UINT(RectPoint::UPPERRIGHT)] = P1_UpperLeft + CDoubleSize(0.0, Diff.dCY);
	m_dptPivotPoint = P1_UpperLeft;
	return *this;
}

CDoubleRect CDoubleRect::SetSecondPoint(CDoublePoint P2)
{
	CDoublePoint P1;
	P1 = m_dptPoints[UINT(RectPoint::UPPERLEFT)];
	SetPoints(P1, P2);
	return *this;
}

CDoubleRect CDoubleRect::SetPointsFromCenter(
	CDoublePoint Center, 
	CDoublePoint P1, 
	CDoublePoint Pivot
)
{
	CDoubleSize Diff;

	Diff = Center - P1;
	m_dptPoints[UINT(RectPoint::UPPERLEFT)] = Center - Diff;
	m_dptPoints[UINT(RectPoint::LOWERLEFT)] = Center + CDoubleSize(Diff.dCX, -Diff.dCY);
	m_dptPoints[UINT(RectPoint::LOWERRIGHT)] = Center + Diff;
	m_dptPoints[UINT(RectPoint::UPPERRIGHT)] = Center + CDoubleSize(-Diff.dCX, Diff.dCY);
	m_dptPivotPoint = Pivot;
	return *this;
}

void CDoubleRect::Rotate(CDoublePoint Pivot, double Angle, RectPoint Reference)
{
	//--------------------------------------------------
	// Rotate
	//	Rotates the rectangle about the Pivot point
	// uing one of the rectangle point as the
	// point that the rotation will be at.  i.e. if
	// the reference point is the center of the
	// rectangle, the center point will be at
	// Angle degrees with respect to the pivot
	// 
	// parameters:
	//	Pivot.......point to rotate around
	//	Angle.......Rotation angle in degrees
	//	Reference...The Point that will determin what
	//				point will be rotated by Angle,
	//				The rest of which will follow suit.
	//--------------------------------------------------

	int i;
	CDoublePoint Center = GetCenter(Center);
	CDoublePoint P1, P2, P3, P4;
	double Radius;
	double RadianAngle = GETAPP.GetPi() * Angle / 180.0;

	switch (Reference)
	{
	case RectPoint::CENTER:
		if (Pivot == Center)
		{

		}
		else
		{
			Radius = (Center - Pivot).Magnitude();
			GetPoint(RectPoint::LOWERLEFT).Rotate(Pivot, Angle);
		}
		break;
	case RectPoint::LOWERLEFT:
		break;
	case RectPoint::LOWERRIGHT:
		break;
	case RectPoint::UPPERLEFT:
		break;
	case RectPoint::UPPERRIGHT:
		break;
	case RectPoint::CENTER_LEFTLINE:
		break;
	case RectPoint::CENTER_RIGHTLINE:
		break;
	case RectPoint::CENTER_TOPLINE:
		break;
	case RectPoint::CENTER_BOTTOMLINE:
		break;
	}
	for (i = 0; i < 4; ++i)
	{
		if (Pivot != m_dptPoints[i])
			m_dptPoints[i].Rotate(Pivot, Angle);
	}
	m_RotationAngle = Angle;;
	m_dptPivotPoint = Pivot;
}

void CDoubleRect::Draw(
	CDC* pDC, 
	MODE mode, 
	CSize Offset, 
	CScale Scale
)
{
	CPoint points[4];

	pDC->Polygon(
		GETAPP.MakePolygonFromDoublePolygon(
			points,
			m_dptPoints,
			4,
			Offset,
			Scale
		), 
		4
	);
}

BOOL CDoubleRect::PointInRectangle(CDoublePoint dptPoint)
{
	return GETAPP.PtEnclosedInPolygon(dptPoint, GetDoublePoints(), 4);
}

CRect CDoubleRect::ToCRect(CSize Offset, CScale Scale)
{
	CRect rectResult;
	CPoint P1, P2;

	P1 = m_dptPoints[0].ToPixelPoint(Offset, Scale);
	P2 = m_dptPoints[2].ToPixelPoint(Offset, Scale);
	rectResult.SetRect(P1, P2);
	rectResult.NormalizeRect();
	return rectResult;
}

CDoublePoint& CDoubleRect::GetCenter(CDoublePoint& Center)
{
	//------------------------------------
	// GetCenter
	//	The big question is how do you
	// find the center of a rectangle?
	// Seems complicated.  But, draw
	// a line from one corner to the
	// oppisite corner and find the
	// midpoint of that line, you have
	// the center point of the rectangle.
	// 
	// parameters: None
	// 
	// return value: Center point of
	// this rectangle.
	//------------------------------------
	Center = m_dptPoints[0].MidPoint(m_dptPoints[2]);
	return Center;
}

CDoublePoint CDoubleRect::GetPoint(RectPoint PointNumber)
{
	CDoublePoint Point;

	switch (PointNumber)
	{
	case RectPoint::REFERENCE:
		Point = m_dptPoints[int(m_ReferencePoint)];
		break;
	case RectPoint::PIVOT:
		Point = m_dptPivotPoint;
		break;
	case RectPoint::CENTER:
		Point = GetCenter(Point);
		break;
	case RectPoint::UPPERLEFT:
	case RectPoint::LOWERLEFT:
	case RectPoint::LOWERRIGHT:
	case RectPoint::UPPERRIGHT:
		Point = m_dptPoints[int(PointNumber)];
	}
	return Point;
}

void CDoubleRect::NormalizeRect()
{
	//--------------------------------
	// NormalizeRect
	//	This method reorganizes the
	// points of the rectangle so
	// that the smallest y values will
	// be closest to the x axis and
	// the smallest x values will be
	// closest to the y axis.
	// To do this, actually, we just
	// put the point that meets the
	// above requirements in the
	// reference point position
	// rectangle[0]
	//--------------------------------
}

void CDoubleRect::AdjustReference(CDoubleSize DeltaRef)
{
	//-------------------------------
	// AdjustReference
	// Basically, move the rectangle
	//-------------------------------
	for (int i = 0; i < 4; ++i)
	{
		m_dptPoints[i] -= DeltaRef;
	}
}

void CDoubleRect::Save(FILE* pO, DocFileParseToken Token, int Indent, int flags)
{
	char* s = new char[256];
	char* IndentString;
	int i;

	CDoublePoint *rectPoints;

	IndentString = GETAPP.MkIndentString(s, Indent);
	fprintf(pO, "%s%s(\n",
		IndentString,
		CLexer::TokenToString(Token)
	);
	for (i = 0, rectPoints = m_dptPoints; i < 4; ++i, ++rectPoints)
	{
		rectPoints->Save(pO, DocFileParseToken::POINT, Indent + 1, flags);
	}
	fprintf(pO, "%s)\n", IndentString);
	delete[]s;
}

RectPoint CDoubleRect::WhichPointIsPointOn(CDoublePoint point)
{
	RectPoint i, rV = RectPoint::NONE;
	int loop;
	
	for (i = RectPoint::LOWERLEFT, loop = 1; i < RectPoint::ENDOF_POINTTYPES && loop; ++i)
	{
		if (GetPoint(i) == point)
		{
			loop = 0;
			rV = i;
		}
	}
	return rV;
}

RectSide CDoubleRect::WhichSideIsPointOn(CDoublePoint point)
{
	RectSide rV = RectSide::NONE;
	RectPoint RectanglePoint;
	int loop;
	RectSide i;

	RectanglePoint = WhichPointIsPointOn(point);
	switch (RectanglePoint)
	{
	case RectPoint::LOWERLEFT:
		rV = RectSide::LOWER_LEFT;
		break;
	case RectPoint::UPPERLEFT:
		rV = RectSide::UPPER_LEFT;
		break;
	case RectPoint::LOWERRIGHT:
		rV = RectSide::LOWER_RIGHT;
		break;
	case RectPoint::UPPERRIGHT:
		rV = RectSide::UPPER_RIGHT;
		break;
	case RectPoint::CENTER:
		rV = RectSide::NONE;
	case RectPoint::CENTER_LEFTLINE:
		rV = RectSide::LEFT;
		break;
	case RectPoint::CENTER_RIGHTLINE:
		rV = RectSide::RIGHT;
		break;
	case RectPoint::CENTER_TOPLINE:
		rV = RectSide::TOP;
		break;
	case RectPoint::CENTER_BOTTOMLINE:
		rV = RectSide::BOTTOM;
		break;
	case RectPoint::NONE:
		for (i = RectSide::LEFT, loop = 1; i < RectSide::LOWER_LEFT && loop; ++i)
		{
			if (GetLine(i).IsPointOnLine(point))
			{
				loop = 0;
				rV = i;
			}
		}
		break;
	}
	return rV;
}

CDoubleLine CDoubleRect::GetLine(RectSide side)
{
	CDoubleLine ResultLine;

	switch (side)
	{
	case RectSide::LEFT:
		ResultLine = CDoubleLine(
			GetPoint(RectPoint::LOWERLEFT),
			GetPoint(RectPoint::UPPERLEFT)
		);
		break;
	case RectSide::RIGHT:
		ResultLine = CDoubleLine(
			GetPoint(RectPoint::LOWERRIGHT),
			GetPoint(RectPoint::UPPERRIGHT)
		);
		break;
	case RectSide::TOP:
		ResultLine = CDoubleLine(
			GetPoint(RectPoint::UPPERLEFT),
			GetPoint(RectPoint::UPPERRIGHT)
		);
		break;
	case RectSide::BOTTOM:
		ResultLine = CDoubleLine(
			GetPoint(RectPoint::LOWERLEFT),
			GetPoint(RectPoint::LOWERRIGHT)
		);
		break;
	}
	return ResultLine;
}

DocFileParseToken CDoubleRect::Parse(DocFileParseToken Token, CLexer* pLex, DocFileParseToken TypeToken)
{
	//--------------------------------------------------
	// Parse
	//	This Method is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token....Lookahead Token for this object
	//	pLex.....Pointer to Lexer for the data stream
	//
	// return value:
	//	returns lookahead token on success, or
	//			negative value on error
	//--------------------------------------------------

	Token = pLex->DoubleRect(DocFileParseToken::DOUBLERECT, *this, Token);
	return Token;
}
