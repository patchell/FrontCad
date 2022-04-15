#pragma once


class CLexer;
class CDoubleLine;

enum class RectPoint : int {
	NONE = -1,
	UPPERLEFT,
	LOWERLEFT,
	LOWERRIGHT,
	UPPERRIGHT,
	CENTER,
	CENTER_LEFTLINE,
	CENTER_RIGHTLINE,
	CENTER_TOPLINE,
	CENTER_BOTTOMLINE,
	PIVOT,
	REFERENCE,
	ENDOF_POINTTYPES
};

enum class RectSide {
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

class CDoubleRect
{
	CDoublePoint m_dptPoints[4];
	double m_RotationAngle;
	CDoublePoint m_dptPivotPoint;
	RectPoint m_ReferencePoint;
public:
	CDoubleRect();
	CDoubleRect(CDoublePoint P1, CDoubleSize SZ) {
		m_dptPoints[0] = P1;
		m_dptPoints[1] = P1 + CDoubleSize(0.0, SZ.dCY);
		m_dptPoints[2] = P1 + SZ;
		m_dptPoints[3] = P1 + CDoubleSize(SZ.dCX, 0.0);
		m_RotationAngle = 0.0;
		m_dptPivotPoint = P1;
	}
	CDoubleRect(CDoublePoint P1, CDoublePoint P2) {
		m_dptPoints[0] = P1;
		m_dptPoints[1] = CDoublePoint(P1.dX, P2.dY);;
		m_dptPoints[2] = P2;
		m_dptPoints[3] = CDoublePoint(P2.dX, P1.dY);
		m_RotationAngle = 0.0;
		m_dptPivotPoint = P1;
	}
	virtual ~CDoubleRect();
	CDoubleRect SetPoints(
		CRect rect,
		CDoublePoint ptPivot,
		double Angle
	);
	CDoubleRect SetPoints(
		CDoublePoint dptRefPoint,
		CDoubleSize dptSize,
		CDoublePoint ptPivot,
		double Angle
	);
	CDoubleRect SetPoints(CDoublePoint* pdptPoints);
	CDoubleRect SetPoints(CDoublePoint P1, CDoublePoint P2);
	CDoubleRect SetSecondPoint(CDoublePoint P2);
	CDoubleRect SetPointsFromCenter(
		CDoublePoint Center,
		CDoublePoint P1,
		CDoublePoint Pivot
	);
	CDoubleRect operator=(CDoubleRect r) {
		int i;
		for (i = 0; i < 4; ++i)
			GetDoublePoints()[i] = r.GetDoublePoints()[i];
		SetRotationAngle(r.GetRotationAngle());
		return *this;

	}
	void Rotate(CDoublePoint ptPivot, double angle, RectPoint Reference);
	void Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale);
//	CDoublePoint Extend(CDoublePoint point, RectangleExtendMode mode);
	BOOL PointInRectangle(CDoublePoint dptPoint);
	CRect ToCRect(CSize Offset, CScale Scale);
	CDoublePoint* GetDoublePoints() { return m_dptPoints; }
	CDoublePoint& GetCenter(CDoublePoint& point);
	CDoublePoint GetPoint(RectPoint PointNumber);
	void NormalizeRect();
	double GetWidth() {
		return m_dptPoints[2].dX - m_dptPoints[0].dX / 2.0;
	}
	void SetWidth(double NewWidth, RectSide From){
		switch (From)
		{
		case RectSide::LEFT:
			break;
		case RectSide::RIGHT:
			break;
		}
	}
	double GetHeight() {
		return m_dptPoints[3].dY - m_dptPoints[1].dY / 2.0;
	}
	void SetHeight(double NewHeitght, RectSide From) {
		switch (From)
		{
		case RectSide::TOP:
			break;
		case RectSide::BOTTOM:
			break;
		}
	}
	CDoubleSize& GetSize(CDoubleSize& size) { 
		return (size = CDoubleSize(GetWidth(), GetHeight())); 
	}
	void SetSize(CDoubleSize szNewSize) {}
	CDoublePoint GetReference() { return m_dptPoints[3]; }
	void AdjustReference(CDoubleSize DeltaRef);
	DocFileParseToken Parse(
		DocFileParseToken Token, 
		CLexer* pLex,
		DocFileParseToken TypeToken
	);
	void Save(
		FILE* pO, 
		DocFileParseToken Token, 
		int Indent, 
		int flags
	);
	double GetRotationAngle() { return m_RotationAngle; }
	void SetRotationAngle(double A) { m_RotationAngle = A; }
	RectPoint WhichPointIsPointOn(CDoublePoint point);
	RectSide WhichSideIsPointOn(CDoublePoint point);
	CDoubleLine GetLine(RectSide side);
};

