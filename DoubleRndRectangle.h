#pragma once

enum class RndRectPoint : int {
	NONE = -1,
	LOWERBOTTOMLEFT,
	LOWERBOTTOMRIGHT,
	UPPERTOPLEFT,
	UPPERTOPRIGHT,
	UPPERLEFTSIDE,
	LOWERLEFTSIDE,
	LOWERRIGHTSIDE,
	UPPERRIGHTSIDE,
	CENTER,
	CENTER_LEFTLINE,
	CENTER_RIGHTLINE,
	CENTER_TOPLINE,
	CENTER_BOTTOMLINE,
	CORNER_RADIUS_UPPERLEFT,
	CORNER_RADIUS_LOWERLEFT,
	CORNER_RADIUS_UPPERRIGHT,
	CORNER_RADIUS_LOWERRIGHT,
	PIVOT,
	REFERENCE,
	ENDOF_POINTTYPES
};

enum class RndRectSide: int {
	NONE = -1,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	LOWER_LEFT,
	LOWER_RIGHT,
	UPPER_LEFT,
	UPPER_RIGHT
};

class CDoubleRndRectangle
{
	CDoublePoint m_pointsPoints[8];
	CDoubleSize m_sizeCorners[4];
	CDoublePoint m_pointPivot;
	double m_Angle;
public:
	CDoubleRndRectangle() {
		m_Angle = 0.0;
	}
	void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
};

