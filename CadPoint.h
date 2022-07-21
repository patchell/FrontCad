#pragma once


class CCadPoint : public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_PointCount = 0;
	inline static SPointAttributes m_LastAttributes;
	inline static SPointAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood = FALSE;
	inline static BOOL m_RenderEnable = TRUE;
	double dX, dY;
	SPointAttributes m_Attributes;
public:
	CCadPoint();
	CCadPoint(double x, double y);
	CCadPoint(DOUBLEPOINT dp);
	CCadPoint(DOUBLEPOINT dp, SubType Sub, UINT SubSub);
	~CCadPoint();
	BOOL Create();
	virtual CString& GetObjDescription();
	virtual BOOL Destroy(CCadObject* pDependentObject);
	void SetX(double x) { dX = x; }
	double GetX() { return dX; }
	double GetY() { return dY; }
	void SetY(double y) { dY = y; }
	DOUBLEPOINT GetPoint() { return DOUBLEPOINT(dX, dY); }
	void SetPoint(double X, double Y) {
		dX = X;
		dY = Y;
	}
	void SetPoint(DOUBLEPOINT dP) {
		dX = dP.dX;
		dY = dP.dY;
	}
	//------------------------------------
	// Drawing Manipulation Methods
	//------------------------------------
	BOOL IsPointOnTarget(DOUBLEPOINT point);
	virtual void Move(CDoubleSize Diff);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p, 
		CCadObject** ppSelList, 
		int index, 
		int n,
		UINT nKinds
	);
	virtual CCadObject* CopyObject();
	//----------- Paint to Screen Ops ----------------------
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale);
	void MoveTo(CDC* pDC, DOUBLEPOINT ULHC, CScale& Scale);
	void LineTo(CDC* pDC, DOUBLEPOINT ULHC, CScale& Scale);
	void LineFromHereToThere(DOUBLEPOINT There, CDC* pDC, DOUBLEPOINT ULHC, CScale& Scale);
	void LineFromHereToThere(CDoubleSize There, CDC* pDC, DOUBLEPOINT ULHC, CScale& Scale);
	BOOL FloodFill(CDC* pDC, COLORREF colorBoundry, DOUBLEPOINT ULHC, CScale& Scale);
	CPoint ToPixelPoint(DOUBLEPOINT ULHC, CScale& Scale);
	//---------- Load/ Save --------------------
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex, DocFileParseToken TypeToken);
	virtual void Save(FILE* pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	//---------- Attributes --------------------
	void CopyAttributesTo(SPointAttributes* pAttrb);
	void CopyAttributesFrom(SPointAttributes* pAttrb);
	SPointAttributes& GetAttributes() { return m_Attributes; }
	SPointAttributes* GetPtrToAttributes() { return &m_Attributes; }
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties();
	//----------------------------------
	// Pointy Things to do with lines
	//----------------------------------
	double Slope(CCadPoint* pPoint);
	double Slope(DOUBLEPOINT point);
	double OrthogonalSlope(CCadPoint point);
	UINT LineIs(DOUBLEPOINT OtherPoint);
	double YIntercept(double m);
	double XIntercept(double m);
	BOOL PointOnLineAtDistance(
		double d,	//distance
		double m,	//slope
		double b,	//Y intercept
		CCadPoint& dest1,	//solution #1
		CCadPoint& dest2	//solution #2
	);
	BOOL PointOnLineAtDistance(
		DOUBLEPOINT P1,
		DOUBLEPOINT ROT,
		double Distance
	);
	void PointOnLineAtDistance(
		CCadPoint* pP1, 
		double m, 
		double Distance
	);
	void PointOnLineAtDistance(
		DOUBLEPOINT P1, 
		double m, 
		double Distance
	);
	void PointOnLineAtDistance(
		CCadPoint* P1,	//starting point
		DOUBLEPOINT RotationPoint,
		double Distance
	);
	CCadPoint& CenterPoint(CCadPoint& Result, CCadPoint& OtherPoint);
	BOOL IsPointBetween(CCadPoint* pP1, CCadPoint* pP2);
	void Print(const char* s);
	//----------------------------------
	// Operators
	//----------------------------------

	operator DOUBLEPOINT() const {
		DOUBLEPOINT res;
		res.dX = dX;
		res.dY = dY;
		return res;
	}
	CCadPoint operator=(CCadPoint const &a) {
		dX = a.dX;
		dY = a.dY;
		return *this;
	}
	CCadPoint operator+(CCadPoint const &a) {
		CCadPoint result;

		result.dX = dX + a.dX;
		result.dY = dY + a.dY;
		return result;
	}
	CCadPoint operator+(CDoubleSize const &a) {
		CCadPoint result;

		result.dX = dX + a.dCX;
		result.dY = dY + a.dCY;
		return result;
	}
	CDoubleSize operator-(CCadPoint const &p) {
		CDoubleSize Result;

		Result.dCX = dX - p.dX;
		Result.dCY = dY - p.dY;
		return Result;
	}
	CCadPoint operator-(CDoubleSize const &a) {
		CCadPoint result;

		result.dX = dX - a.dCX;
		result.dY = dY - a.dCY;
		return result;
	}
	CCadPoint operator-=(CCadPoint const &a) {
		dX -= a.dX;
		dY -= a.dY;
		return *this;
	}
	CCadPoint operator-=(CDoubleSize const &a) {
		dX -= a.dCX;
		dY -= a.dCY;
		return *this;
	}
	CCadPoint operator-() {
		return CCadPoint(-dX, -dY);
	}
	CCadPoint operator+=(CCadPoint const &a) {
		dX += a.dX;
		dY += a.dY;
		return *this;
	}
	CCadPoint operator+=(CDoubleSize const &a) {
		dX += a.dCX;
		dY += a.dCY;
		return *this;
	}
	CCadPoint operator*(CScale const &s) {
		CCadPoint result;

		result.dX = dX * s.m_ScaleX;
		result.dY = dY * s.m_ScaleY;
		return result;
	}
	BOOL operator!=(CCadPoint const &p) {
		return (dX != p.dX || dY != p.dY);
	}
	BOOL operator==(CCadPoint const &p) {
		return (dX == p.dX && dY == p.dY);
	}
	//------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SPointAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}

};

//inline DOUBLEPOINT operator+(CCadPoint CCP, DOUBLEPOINT& DP) {
//	DOUBLEPOINT result;
//	result.dX = CCP.GetX() + DP.dX;
//	result.dY = CCP.GetY() + DP.dY;
//	return result;
//}

inline DOUBLEPOINT operator-(CCadPoint CCP, DOUBLEPOINT& DP) {
	DOUBLEPOINT result;
	result.dX = CCP.GetX() - DP.dX;
	result.dY = CCP.GetY() - DP.dY;
	return result;
}
