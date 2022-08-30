//------------------------------------------------
// Scale.h: interface for the CScale class.
//------------------------------------------------

#pragma once

class CScale :public SCALE
{
public:
	CScale() {
		dSX = 0.0;
		dSY = 0.0;
	};
	CScale(double x, double y) {
		dSX = x;
		dSY = y;
	}
	CScale(double xy) {
		dSX = xy;
		dSY = xy;
	}
	virtual ~CScale() {}
	double GetScaleX() { return dSX; }
	double GetScaleY() { return dSY; }
	operator SCALE() const  {
		SCALE Result;

		Result.dSX = dSX;
		Result.dSY = dSY;
		return Result;
	}
};

/*
inline DOUBLEPOINT operator * (DOUBLEPOINT const pt, CScale const &scale)
{
	DOUBLEPOINT result;

	result.dX = pt.dX * scale.m_ScaleX;
	result.dX = pt.dX * scale.m_ScaleY;
	return result;
}
*/