#include "pch.h"

void CDoubleLine::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
{
	pDC->MoveTo(CDoublePoint(dP1).ToPixelPoint(Offset, Scale));
	pDC->LineTo(CDoublePoint(dP1).ToPixelPoint(Offset, Scale));
}

CDoublePoint& CDoubleLine::GetPoint(LinePoint Type)
{
	switch (Type)
	{
	case LinePoint::P1:
		return (CDoublePoint &)dP1;
		break;
	case LinePoint::P2:
		return (CDoublePoint&)dP2;
		break;
	}
	return (CDoublePoint&)dP1;
}

CDoublePoint ExtendLineWidth(CDoublePoint point)
{
	return CDoublePoint();
}

CDoublePoint ExtendHeight()
{
	return CDoublePoint();
}

CDoublePoint CDoubleLine::Extend(CDoublePoint p, LineExtendMode mode)
{
	//------------------------------------
	// Extend
	//	This method is used to extend
	// the line baseed on the value p.  P
	// does not need to be on the line
	// and a point is calculated so
	// that the does lie on the line.
	// This can be a bit tricky if
	// X is constant or Y is constant
	// or very nearly so.
	// 
	// parameters:
	//	p....Point to base extension
	//	mode.Both-Width-or Height
	// 
	// returns:
	//	The new point.
	//------------------------------------
	// first check to see if this line is
	// vertical or horizontal
	//------------------------------------
	CDoublePoint Result;

	switch (mode)
	{
	case LineExtendMode::LENGTH:
		break;
	case LineExtendMode::LENGTH_ANGLE:
		break;
	}
	return CDoublePoint();
}

BOOL CDoubleLine::IsPointOnLine(CDoublePoint point)
{
	BOOL rV = FALSE;
	CDoubleRect rect;

	rect = EncloseLine(rect, 10);
	rV = rect.PointInRectangle(point);
	return rV;
}

CDoubleRect& CDoubleLine::EncloseLine(CDoubleRect& rect, int WidthInPixels)
{
	//-----------------------------
	// enclose the line inside
	// of a rectangle
	// parameters:
	//	w.....The amount that will
	//		be added to the width
	//		of the line to make
	//		the rectangle.
	//-----------------------------
	double Inches;
	CDoublePoint P1, P2;

	Inches = GETVIEW()->GetGrid().GetInchesPerPixel().GetScaleX() * double(WidthInPixels);
	P1 = CDoublePoint(dP1) + CDoubleSize(Inches, Inches);
	P2 = CDoublePoint(dP2) - CDoubleSize(Inches, Inches);
	rect.SetPoints(P1, P2);
	return rect;
}
