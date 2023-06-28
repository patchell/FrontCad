#pragma once

class CFileParser;

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
	CDoubleSize operator - (CDoubleSize v) {
		CDoubleSize Result;
		Result.dCX = dCX - v.dCX;
		Result.dCY = dCY - v.dCY;
		return Result;
	}
	CDoubleSize operator * (CScale Scale) {
		CDoubleSize Result;
		Result.dCX = dCX * Scale.GetScaleX();
		Result.dCY = dCY * Scale.GetScaleY();
		return Result;
	}
	CDoubleSize operator * (double Scaler) {
		CDoubleSize Result;
		Result.dCX = dCX * Scaler;
		Result.dCY = dCY * Scaler;
		return Result;
	}
	CDoubleSize operator/(double x) {
		CDoubleSize Result;
		Result = CDoubleSize(dCX / x, dCY / x);
		return Result;
	}
	CLexer::Tokens Parse(
		CLexer::Tokens Token,	// Lookahead Token
		CFileParser* pParser,	// pointer to parser
		CLexer::Tokens TypeToken = CLexer::Tokens::DEFAULT // Token type to save object as
	);
	double Magnitude() {
		return sqrt(dCX * dCX + dCY * dCY);
	}
	double Slope() { return dCY / dCX;}
	double Angle(BOOL DegreesMode = DOUBLESIZE_DEGREES);
	CSize ToPixelSize(CScale& Scale) const;
	operator DOUBLEPOINT() const  {
		DOUBLEPOINT result;
		result.dX = dCX;
		result.dY = dCY;
		return result;
	}
	inline operator CPoint() const;
};

inline DOUBLEPOINT operator+(DOUBLEPOINT pt, CDoubleSize sz)
{
	DOUBLEPOINT ResultPoint;
	ResultPoint.dX = pt.dX + sz.dCX;
	ResultPoint.dY = pt.dY + sz.dCY;
	return ResultPoint;
}

inline DOUBLEPOINT operator-(DOUBLEPOINT pt, CDoubleSize sz)
{
	DOUBLEPOINT ResultPoint;
	ResultPoint.dX = pt.dX - sz.dCX;
	ResultPoint.dY = pt.dY - sz.dCY;
	return ResultPoint;
}

//I want this one
inline CDoubleSize operator-(DOUBLEPOINT pt1, DOUBLEPOINT pt2)
{
	DOUBLEPOINT ResultPoint;
	ResultPoint.dX = pt1.dX - pt2.dX;
	ResultPoint.dY = pt1.dY - pt2.dY;
	return ResultPoint;
}
