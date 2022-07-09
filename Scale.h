//------------------------------------------------
// Scale.h: interface for the CScale class.
//------------------------------------------------

#pragma once

class CScale :public SCALE
{
public:
	CScale() {
		m_ScaleX = 0.0;
		m_ScaleY = 0.0;
	};
	CScale(double x, double y) {
		m_ScaleX = x;
		m_ScaleY = y;
	}
	CScale(double xy) {
		m_ScaleX = xy;
		m_ScaleY = xy;
	}
	virtual ~CScale() {}
	double GetScaleX() { return m_ScaleX; }
	double GetScaleY() { return m_ScaleY; }
	operator SCALE() const  {
		SCALE Result;

		Result.m_ScaleX = m_ScaleX;
		Result.m_ScaleY = m_ScaleY;
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