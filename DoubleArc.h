#pragma once

enum class BadDelta {
	GOOD,
	X,
	Y
};

class CDoubleArc
{
	CDoublePoint m_Center;
	CDoubleSize m_Shape;	//size and shape of arc
	double m_StartAngle;	//Where to begin
	double m_SweepAngle;	// How far to go
	double m_Angle;	//how much the works is rotated by
public:
	CDoubleArc();
	void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	double CalcY(double x, double A, double B);
	BadDelta DeltaIsBad(CPoint P1, CPoint P2);
	CDoubleSize SlopeIsOneAt(double Asquared, double Bsquared);
};

