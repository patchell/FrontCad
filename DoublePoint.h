#pragma once

class CLexer;
class CDoubleRect;


class CDoublePoint :public DOUBLEPOINT
{
	//--------------- Private Methods ---------------
	double m_RadAngle;	//angle in radians
	CDoublePoint* m_pPivot;
public:
	CDoublePoint() {
		dX = 0.0;
		dY = 0.0;
		m_RadAngle = 0.0;
		m_pPivot = 0;
	}
	CDoublePoint(DOUBLEPOINT pt) {
		dX = pt.dX;
		dY = pt.dY;
		m_RadAngle = 0.0;
		m_pPivot = 0;
	}
	CDoublePoint(double x, double y) {
		dX = x;
		dY = y;
		m_RadAngle = 0.0;
		m_pPivot = 0;
	}
	CDoublePoint(CPoint ptPoint) {
		dX = double(ptPoint.x);
		dY = double(ptPoint.y);
		m_RadAngle = 0.0;
		m_pPivot = 0;
	}
	CDoublePoint(int x, int y, CSize Offset, CScale Scale)
	{
		dX = double(LONG(x)) + double(Offset.cx) * Scale.GetScaleX();
		dY = (double(y + Offset.cy)) * Scale.GetScaleY();
		m_RadAngle = 0.0;
		m_pPivot = 0;
	}
	~CDoublePoint() {}
	CDoublePoint operator=(CDoublePoint a) {
		dX = a.dX;
		dY = a.dY;
		return *this;
	}
	CDoublePoint operator+(CDoublePoint a) {
		CDoublePoint result;

		result.dX = dX + a.dX;
		result.dY = dY + a.dY;
		return result;
	}
	CDoublePoint operator+(CDoubleSize a) {
		CDoublePoint result;

		result.dX = dX + a.dCX;
		result.dY = dY + a.dCY;
		return result;
	}
	CDoubleSize operator-(CDoublePoint p) {
		CDoubleSize Result;

		Result.dCX = dX - p.dX;
		Result.dCY = dY - p.dY;
		return Result;
	}
	CDoublePoint operator-(CDoubleSize a) {
		CDoublePoint result;

		result.dX = dX - a.dCX;
		result.dY = dY - a.dCY;
		return result;
	}
	CDoublePoint operator-=(CDoublePoint a) {
		dX = dX - a.dX;
		dY = dY - a.dY;
		return *this;
	}
	CDoublePoint operator-=(CDoubleSize a) {
		dX = dX - a.dCX;
		dY = dY - a.dCY;
		return *this;
	}
	CDoublePoint operator-() {
		return CDoublePoint(-dX, -dY);
	}
	CDoublePoint operator+=(CDoublePoint a) {
		dX = dX + a.dX;
		dY = dY + a.dY;
		return *this;
	}
	CDoublePoint operator+=(CDoubleSize a) {
		dX = dX + a.dCX;
		dY = dY + a.dCY;
		return *this;
	}
	CDoublePoint operator*(SCALE s) {
		CDoublePoint result;

		result.dX = dX * s.m_ScaleX;
		result.dY = dY * s.m_ScaleY;
		return result;
	}
	BOOL operator!=(CDoublePoint p) {
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
	BOOL operator==(CDoublePoint p) {
		BOOL rV = FALSE;
		if (dX == p.dX && dY == p.dY)
			rV = TRUE;
		return rV;
	}
	int intGetX();
	int intGetY();
	CDoublePoint Rotate(CDoublePoint Pivot, double Angle);
	CDoublePoint Rotate(CDoublePoint Pivot, CDoublePoint NewPoint);
	CDoublePoint Snap(CDoubleSize SnapSize, BOOL SnapEnabled);
	CPoint ToPixelPoint(CSize Offset, SCALE PixelsPerInch);
	CPoint ToCPoint();
	BOOL Hover(CDoublePoint Point);
	void FilledRect(CDC* pDC, MODE mode, CSize Offset, CScale Scale, COLORREF Border, COLORREF Fill);
	CDoubleRect EnclosePoint(UINT nPixels, CScale Scale);
	void Draw(CDC* pDC, 
		MODE mode,
		CSize Offset, 
		CScale Scale
	);
	BOOL MakeSelectionRect(
		CRect& rect, 
		SCALE Scale, 
		CSize Offset, 
		double Margin
	);
	void Flip(CDoublePoint Pivot, Axis axix);
	DocFileParseToken Parse(
		DocFileParseToken Token,
		CLexer* pLex,
		DocFileParseToken TypeToken
	);
	void SetX(double X) {
		dX = X;
	}
	void SetY(double Y) {
		dY = Y;
	}
	void Save(FILE* pO, DocFileParseToken Token, int Indent, int flags);
	CDoublePoint MidPoint(CDoublePoint P);
	BOOL IsPointOnTarget(CDoublePoint p); 
	double DistanceTo(CDoublePoint p);
	CDoublePoint Rotate();
	CDoublePoint Rotate(CDoublePoint* pPivot, CDoublePoint point);
	CDoublePoint Rotate(CDoublePoint* pPivot, double Angle);
	CDoublePoint RoundUpToNearest(CDoubleSize roundto);
	CDoublePoint RoundToNearest(CDoubleSize roundto);
	//-----------------------------------------
	void Print() { printf("Point: x=%7.2lf   y=%7.2lf\n", dX, dY); }
};