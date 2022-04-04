#pragma once

enum class LinePoint {
	P1,
	P2,
	CENTER
};

class CDoubleLine :public DOUBLELINE
{
public:
	CDoubleLine() {
		dP1 = { 0.0,0.0 };
		dP2 = { 0.0,0.0 };
	}
	CDoubleLine(CDoublePoint P1, CDoublePoint P2) {
		dP1 = DOUBLEPOINT(P1);
		dP2 = DOUBLEPOINT(P2);
	}
	~CDoubleLine() {}
	void Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale);
	CDoublePoint& GetPoint(LinePoint Type);
	CDoublePoint MidPoint() {
		return CDoublePoint(dP1).MidPoint(dP2);
	}
	CDoublePoint Extend(CDoublePoint p, LineExtendMode mode);
	BOOL IsPointOnLine(CDoublePoint point);
	virtual void Move(CDoubleSize diff){
		dP1 += diff;
		dP2 += diff;
	}
	CDoubleRect& EncloseLine(CDoubleRect& rect, int WidthInPixels);
};

