#pragma once

//----------------------------------
struct SCALE {
	double dSX;
	double dSY;
	SCALE() {
		dSX = 0.0; 
		dSY = 0.0;
	}
	void Print(const char* s) {
		printf("%s (dSX,dSY)=(%7.3lf,%7.3lf)\n", s, dSX, dSY);
	}
} ;


struct DOUBLESIZE {
	double dCX;
	double dCY;
	DOUBLESIZE() { dCX = 0; dCY = 0; }
	DOUBLESIZE(double cx, double cy) { dCX = cx; dCY = cy; }
};

struct DOUBLEPOINT {
	double dX;
	double dY;
	DOUBLEPOINT() { dX = 0; dY = 0; }
	DOUBLEPOINT(double x, double y) { dX = x; dY = y; }
	DOUBLEPOINT(CPoint p) { dX = double(p.x);  dY = double(p.y); }
	inline operator DOUBLESIZE() const {
		DOUBLESIZE result;
		result.dCX = dX;
		result.dCY = dY;
		return result;
	}
	inline operator CPoint() const {
		CPoint cP;
		cP.x = int(dX + 0.5 - (dX < 0.0));
		cP.y = int(dY + 0.5 - (dY < 0.0));
		return cP;
	}
	DOUBLEPOINT Snap(DOUBLESIZE SnapSize, BOOL SnapEnabled) {
		//-----------------------------------------------------
		// Snap
		// sets the position to nearest snap grid position
		//
		// Parameters:
		//	SnapSize.Number of units for the snap grid
		// for example, for a 1/4 inch grid, SnapUnits
		// would be 0.250
		//
		// Returns:
		//	Cursor position in real units for the view
		//-----------------------------------------------------
		double x_mod, x, y_mod, y;

		if (SnapEnabled && SnapSize.dCX != 0.0 && SnapSize.dCY != 0.0)
		{
			x_mod = fmod(dX, SnapSize.dCX);
			if (x_mod < (SnapSize.dCX / 2.0))
				x = dX - x_mod;
			else
				x = dX + SnapSize.dCX - x_mod;
			y_mod = fmod(dY, SnapSize.dCY);
			if (y_mod < (SnapSize.dCY / 2.0))
				y = dY - y_mod;
			else
				y = dY + SnapSize.dCY - y_mod;
			dX = x;
			dY = y;
		}
		return *this;
	}
	DOUBLEPOINT operator-()
	{
		return DOUBLEPOINT(-dX, -dY);
	}
	CPoint ToPixelPoint(DOUBLEPOINT& LLHC, double cX, double cY);
	DOUBLEPOINT LLHCfromPixelPoint(CPoint point, double cX, double cY);
	void Print(const char* s) {
		printf("%s:POINT(%7.3lf,%7.3lf\n", s, dX, dY);
	}
	DOUBLEPOINT Raw(CPoint p, DOUBLEPOINT& LLHC, SCALE Scale);
	double YIntercept(double m) {
		double b;

		b = dY - m * dX;
		return b;
	}
};	//end of class DOUBLEPOINT

//---------------------------------------
// Object Operators
//---------------------------------------

inline DOUBLEPOINT operator * (DOUBLEPOINT pt, SCALE& scale) {
	DOUBLEPOINT result;
	result.dX = pt.dX * scale.dSX;
	result.dY = pt.dY * scale.dSY;
	return result;
}

inline DOUBLEPOINT operator * (DOUBLEPOINT pt, double s) {
	DOUBLEPOINT result;
	result.dX = pt.dX * s;
	result.dY = pt.dY * s;
	return result;
}

inline DOUBLEPOINT operator+=(DOUBLEPOINT pt, DOUBLESIZE& sz) {
	pt.dX += sz.dCX;
	pt.dY += sz.dCY;
	return pt;
}

inline DOUBLEPOINT operator+(DOUBLEPOINT pt, DOUBLESIZE& sz)
{
	DOUBLEPOINT ResultPoint;
	ResultPoint.dX = pt.dX + sz.dCX;
	ResultPoint.dY = pt.dY + sz.dCY;
	return ResultPoint;
}

inline DOUBLEPOINT operator-=(DOUBLEPOINT pt, DOUBLESIZE sz)
{
	pt.dX -= sz.dCX;
	pt.dY -= sz.dCY;
	return pt;
}

inline DOUBLEPOINT operator-=(DOUBLEPOINT pt, DOUBLEPOINT point)
{
	pt.dX -= point.dX;
	pt.dY -= point.dY;
	return pt;
}

inline DOUBLEPOINT operator-(DOUBLEPOINT pt, DOUBLESIZE sz)
{
	DOUBLEPOINT ResultPoint;
	ResultPoint.dX = pt.dX - sz.dCX;
	ResultPoint.dY = pt.dY - sz.dCY;
	return ResultPoint;
}

inline BOOL operator==(DOUBLEPOINT pt1, DOUBLEPOINT pt2)
{
	BOOL rV;

	rV = (pt1.dX == pt2.dX) && (pt1.dY == pt2.dY);
	return rV;
}
