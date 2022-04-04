#pragma once

//----------------------------------
struct SCALE {
	double m_ScaleX;
	double m_ScaleY;
	SCALE() {
		m_ScaleX = 0.0; 
		m_ScaleY = 0.0;
	}
} ;

struct DOUBLEPOINT {
	double dX;
	double dY;
	DOUBLEPOINT() { dX = 0; dY = 0; }
	DOUBLEPOINT(double x, double y) { dX = x; dY = y; }
};

struct DOUBLESIZE {
	double dCX;
	double dCY;
	DOUBLESIZE() { dCX = 0; dCY = 0; }
	DOUBLESIZE(double cx, double cy) { dCX = cx; dCY = cy; }
};

struct DOUBLELINE {
	DOUBLEPOINT dP1;
	DOUBLEPOINT dP2;
};

//---------------------------------------
// Object Operators
//---------------------------------------

inline DOUBLEPOINT operator+=(DOUBLEPOINT& pt, DOUBLESIZE& sz) {
	DOUBLEPOINT ResultPoint;
	ResultPoint.dX = pt.dX + sz.dCX;
	ResultPoint.dY = pt.dY + sz.dCY;
	return ResultPoint;
}

inline DOUBLEPOINT operator+(DOUBLEPOINT& pt, DOUBLESIZE& sz)
{
	DOUBLEPOINT ResultPoint;
	ResultPoint.dX = pt.dX + sz.dCX;
	ResultPoint.dY = pt.dY + sz.dCY;
	return ResultPoint;
}

inline DOUBLEPOINT operator-=(DOUBLEPOINT& pt, DOUBLESIZE& sz)
{
	DOUBLEPOINT ResultPoint;
	ResultPoint.dX = pt.dX - sz.dCX;
	ResultPoint.dY = pt.dY - sz.dCY;
	return ResultPoint;
}

inline DOUBLEPOINT operator-(DOUBLEPOINT& pt, DOUBLESIZE& sz)
{
	DOUBLEPOINT ResultPoint;
	ResultPoint.dX = pt.dX - sz.dCX;
	ResultPoint.dY = pt.dY - sz.dCY;
	return ResultPoint;
}
