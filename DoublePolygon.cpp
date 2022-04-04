#include "pch.h"

using namespace std;

CDoublePolygon::CDoublePolygon()
{
	m_pointsPolygon = 0;
	m_numPoints = 0;
	m_PointAddedCount = 0;
	Angle = 0.0;
}

CDoublePolygon::~CDoublePolygon()
{
	if (m_pointsPolygon)delete[] m_pointsPolygon;
}

void CDoublePolygon::Create(UINT numberOfPoints, CDoublePoint *Pounts)
{
	if (m_pointsPolygon)delete[] m_pointsPolygon;
	m_pointsPolygon = new CDoublePoint[numberOfPoints];
	m_numPoints = numberOfPoints;
	if (Pounts)
	{
		for (UINT i = 0; i < m_numPoints; ++i)
			m_pointsPolygon[i] = Pounts[i];
	}
}

void CDoublePolygon::Fill(CDC* pDC, MODE mode, CSize Offset, CScale Scale, COLORREF colorBoundry)
{
	//---------------------------------------
	// Fill
	//	This is going to be a tricky one.
	// We first need to find a point inside
	// of the figure to start the fill at.
	// parameters:
	//	pDC.........pointer to device context
	//	mode........Drawing mode
	//	Offset......Scrolling offset
	//	Scale.......pixels per inch
	//	colorBoundry.......Boundry color
	// 
	// Polygon is filled with the current
	// brush color
	//---------------------------------------
	CDoublePoint InsidePoint;

	if (GETAPP.GeneratePointInPolygon(m_pointsPolygon, m_numPoints, InsidePoint))
	{
		CPoint point = InsidePoint.ToPixelPoint(Offset, Scale);
		pDC->FloodFill(point.x, point.y, colorBoundry);
	}
}

void CDoublePolygon::ConnectTheDots(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
{
	UINT i;

	pDC->MoveTo(GetPoints()[0].ToPixelPoint(Offset,Scale));
	for (i = 1; i < m_numPoints; ++i)
	{
		pDC->LineTo(GetPoints()[i].ToPixelPoint(Offset, Scale));
		GetPoints()[i].Draw(pDC, mode, Offset, Scale);
	}
	pDC->LineTo(GetPoints()[0].ToPixelPoint(Offset, Scale));

}

void CDoublePolygon::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
{
	switch (mode.DrawMode)
	{
	case ObjectDrawMode::FINAL:
		ConnectTheDots(pDC, mode, Offset, Scale);
		break;
	case ObjectDrawMode::SELECTED:
		ConnectTheDots(pDC, mode, Offset, Scale);
		break;
	}
}

BOOL CDoublePolygon::PointInPolygon(CDoublePoint Point)
{
	BOOL rV = FALSE;
	if (m_pointsPolygon && m_numPoints)
		rV = GETAPP.PtEnclosedInPolygon(Point, m_pointsPolygon, m_numPoints);
	return rV;
}

void CDoublePolygon::SetSize(UINT size)
{
	m_numPoints = size;
	if (m_pointsPolygon) delete[]m_pointsPolygon;
	m_pointsPolygon = new CDoublePoint[size];
	m_PointAddedCount = 0;
}

BOOL CDoublePolygon::AddPoint(CDoublePoint p)
{
	//---------------------------------------
	// AddPoint
	//	Adds a new point to the polygon.  To
	// make this object more dynamidc, this
	// method creates a new points array
	// with each added point.
	// 
	// Parameter:
	//	p......point to add
	// Returns
	//	TRUE on success
	//---------------------------------------

	if (m_pointsPolygon)
	{
		CDoublePoint* newPoints = new CDoublePoint[m_numPoints + 1];
		memset(newPoints, 0, sizeof(CDoublePoint) * (m_numPoints + 1));
		UINT i;
		for (i = 0; i < m_numPoints; ++i)
			newPoints[i] = m_pointsPolygon[i];
		newPoints[m_numPoints++] = p;
		m_PointAddedCount++;
		delete[]m_pointsPolygon;
		m_pointsPolygon = newPoints;
	}
	else
	{
		m_pointsPolygon = new CDoublePoint[m_PointAddedCount + 1];
		m_pointsPolygon[m_PointAddedCount] = p;
		m_PointAddedCount++;
		m_numPoints++;

	}
	return TRUE;
}

CDoublePoint CDoublePolygon::GetCenter()
{
	double x = 0.0, y = 0.0;
	UINT i;

	for (i = 0; i < GetSize(); ++i)
	{
		x += GetPoint(i).dX;
		y += GetPoint(i).dY;
	}
	x /= double(i);
	y /= double(i);
	return CDoublePoint(x, y);
}
