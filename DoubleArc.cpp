#include "pch.h"

CDoubleArc::CDoubleArc()
{
	m_Angle = 0.0;
	m_StartAngle = 0.0;
	m_SweepAngle = 0.0;
}

void CDoubleArc::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
{
	//------------------------------
	// For right now, going to use
	// brute force and ignorance
	//-----------------------------
	CDoubleSize SlopeIsOne;
	int RadiusX, RadiusY;
	double A, B, x, y;
	CPoint Point,lastPoint;
	CScale InchesPerPixel;

	InchesPerPixel = GETVIEW()->GetGrid().GetInchesPerPixel();
	A = m_Shape.dCX;
	A *= A;
	B = m_Shape.dCY;
	B *= B;
	SlopeIsOne = SlopeIsOneAt(A, B);
	RadiusX = GETAPP.RoundDoubleToInt(SlopeIsOne.dCX * Scale.GetScaleX());
	RadiusY = GETAPP.RoundDoubleToInt(SlopeIsOne.dCY * Scale.GetScaleY());
	for (int i = 0; i < RadiusX; i++)
	{
		x = double(i) * InchesPerPixel.GetScaleX();
		y = CalcY(x, A, B);
		Point = (m_Center + CDoubleSize(x, y)).ToPixelPoint(Offset, Scale);
		pDC->SetPixel(Point, RGB(2550, 64, 255));
	}
	for (int i = 0; i < RadiusY; i++)
	{
		y = double(i) * InchesPerPixel.GetScaleY();
		//---------------------------------
		// Really using CalcY to calculate
		// x
		//---------------------------------
		x = CalcY(y, B, A);
		Point = (m_Center + CDoubleSize(x, y)).ToPixelPoint(Offset, Scale);
		pDC->SetPixel(Point, RGB(2550, 64, 255));
	}
}

double CDoubleArc::CalcY(double x, double Asquared, double Bsquared)
{
	//-------------------------------------
	// CalcY
	// Calculate the y position for x
	// 
	// x....x coordinate
	// Asquared....Major Axis Squared
	// Bsquared....Minor Axis Squared
	// reutrns:
	// y
	//-------------------------------------
	double Y;
	Y = sqrt(Bsquared * (1.0 - x * x / Asquared));
	return Y;
}

BadDelta CDoubleArc::DeltaIsBad(CPoint P1, CPoint P2)
{
	BadDelta rV = BadDelta::GOOD;
	CSize diff = P1 - P2;
	if (abs(diff.cx) > 1)
		rV = BadDelta::X;
	else if (abs(diff.cy) > 1)
		rV = BadDelta::Y;
	return rV;
}

CDoubleSize CDoubleArc::SlopeIsOneAt(double Asquared, double Bsquared)
{
	//------------------------------
	// SlopeIsOneAt
	//	This method is to find the
	// place on the curve where the
	// slope is one
	// parameters:
	//	Asquared.....Major Axis Squared
	//	Bsquared.....Minor Axis Squared
	//------------------------------
	double x;
	double y;

	x = Asquared * sqrt(1.0 / (Asquared + Bsquared));
	y = Bsquared * sqrt(1.0 / (Asquared + Bsquared));
	return CDoubleSize(x, y);
}

