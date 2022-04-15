#pragma once


class CCadPoint : public CCadObject
{
	double dX, dY;
public:
	CCadPoint();
	CCadPoint(double x, double y);
	~CCadPoint();
	virtual BOOL Destroy(CCadObject* pDependentObject);
	void Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale);
	void SetX(double x) { dX = x; }
	void SetY(double y) { dY = y; }
	CDoublePoint GetPoint() { return CDoublePoint(dX, dY); }
	void SetPoint(CDoublePoint dP) {
		dX = dP.dX;
		dY = dP.dY;
	}
	//----------------------------------
	// Operators
	//----------------------------------
	CCadPoint operator=(CCadPoint a) {
		dX = a.dX;
		dY = a.dY;
		return *this;
	}
	CCadPoint operator+(CCadPoint a) {
		CCadPoint result;

		result.dX = dX + a.dX;
		result.dY = dY + a.dY;
		return result;
	}
	CCadPoint operator+(CCadPoint a) {
		CCadPoint result;

		result.dX = dX + a.dCX;
		result.dY = dY + a.dCY;
		return result;
	}
	CDoubleSize operator-(CCadPoint p) {
		CDoubleSize Result;

		Result.dCX = dX - p.dX;
		Result.dCY = dY - p.dY;
		return Result;
	}
	CCadPoint operator-(CDoubleSize a) {
		CCadPoint result;

		result.dX = dX - a.dCX;
		result.dY = dY - a.dCY;
		return result;
	}
	CCadPoint operator-=(CDouCCadPointblePoint a) {
		dX = dX - a.dX;
		dY = dY - a.dY;
		return *this;
	}
	CCadPoint operator-=(CCadPoint a) {
		dX = dX - a.dCX;
		dY = dY - a.dCY;
		return *this;
	}
	CCadPoint operator-() {
		return CCadPoint(-dX, -dY);
	}
	CCadPoint operator+=(CCadPoint a) {
		dX = dX + a.dX;
		dY = dY + a.dY;
		return *this;
	}
	CCadPoint operator+=(CDoubleSize a) {
		dX = dX + a.dCX;
		dY = dY + a.dCY;
		return *this;
	}
	CCadPoint operator*(SCALE s) {
		CCadPoint result;

		result.dX = dX * s.m_ScaleX;
		result.dY = dY * s.m_ScaleY;
		return result;
	}
	BOOL operator!=(CCadPoint p) {
		BOOL rv = FALSE;
		if (dX == p.dX)
		{
			if (dY != p.dY)
				rv = TRUE;
		}
		else
			rv = TRUE;
		return rv;
	}
	BOOL operator==(CCadPoint p) {
		BOOL rV = FALSE;
		if (dX == p.dX && dY == p.dY)
			rV = TRUE;
		return rV;
	}

};

