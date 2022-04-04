#pragma once

class CLexer;

constexpr auto DOUBLESIZE_RADIANS = FALSE;
constexpr auto DOUBLESIZE_DEGREES = TRUE;;

class CDoubleSize: public DOUBLESIZE
{
public:
	CDoubleSize() {
		dCX = 0.0;
		dCY = 0.0;
	}
	CDoubleSize(double cx, double cy) {
		dCX = cx;
		dCY = cy;
	}
	CDoubleSize(CSize szSize);
	CDoubleSize(int cx, int cy) {
		dCX = double(cx);
		dCY = double(cy);
	}
	CDoubleSize(DOUBLEPOINT p) {
		dCX = p.dX;
		dCY = p.dY;
	}
	virtual ~CDoubleSize() {}
	CSize ToCSize() { return CSize(int(round(dCX)), (int)round(dCY)); }
	BOOL operator!=(CDoubleSize a) {
		BOOL rv = TRUE;
		if ((dCX == a.dCX) && (dCY == a.dCY))
			rv = FALSE;
		return rv;
	}
	CDoubleSize operator * (CScale Scale) {
		CDoubleSize Result;
		Result.dCX = dCX * Scale.GetScaleX();
		Result.dCY = dCY * Scale.GetScaleY();
		return Result;
	}
	CDoubleSize operator/(double x) {
		CDoubleSize Result;
		Result = CDoubleSize(dCX / x, dCY / x);
		return Result;
	}
	DocFileParseToken Parse(
		DocFileParseToken Token,
		CLexer* pLex
	);
	double Magnitude() {
		return sqrt(dCX * dCX + dCY * dCY);
	}
	double Angle(BOOL DegreesMode = DOUBLESIZE_DEGREES);
	CSize ToPixelSize(CScale Scale);
};


