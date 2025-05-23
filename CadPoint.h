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
	int m_PointID;
	SPointAttributes m_Attributes;
public:
	CCadPoint();
	CCadPoint(double x, double y);
	CCadPoint(DOUBLEPOINT dp);
	CCadPoint(DOUBLEPOINT dp, SubTypes Sub, UINT SubSub);
	~CCadPoint();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual void UpdateEnclosure();
	virtual Token GetDefaultToken() { return Token::POINT; }
	void SetID(int ID) { m_PointID = ID; }
	int GetID()const  { return m_PointID; }
	virtual CString& GetObjDescription();
	virtual CString& GetTypeString();
	void SetX(double x) { dX = x; }
	double GetX() const { return dX; }
	double GetY() const { return dY; }
	void SetY(double y) { dY = y; }
	DOUBLEPOINT GetPoint() const { 
		return {dX,dY};
	}
	void SetPoint(double X, double Y) {
		dX = X;
		dY = Y;
	}
	void SetPoint(DOUBLEPOINT dP) {
		dX = dP.dX;
		dY = dP.dY;
	}
	void SetPoint(CCadPoint* pT) {
		dX = pT->dX;
		dY = pT->dY;
	}
	//------------------------------------
	// Drawing Manipulation Methods
	//------------------------------------
	BOOL IsPointOnTarget(DOUBLEPOINT point);
	virtual void Move(CDoubleSize Diff);
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p, 
		CCadObject* pExcludeObject,
		CCadObject** ppSelList,
		int index, 
		int n,
		UINT nKinds
	);
	//----------- Paint to Screen Ops ----------------------
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	void MoveTo(CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale);
	void LineTo(CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale);
	void MoveTo(
		CDC* pDC, 
		DOUBLEPOINT& LLHC, 
		CScale& Scale, 
		CCadPoint* pPivot, 
		CCadPoint* pRotation
	);
	void LineTo(
		CDC* pDC, 
		DOUBLEPOINT& LLHC, 
		CScale& Scale, 
		CCadPoint* pPivot, 
		CCadPoint* pRotation
	);
	void LineFromHereToThere(DOUBLEPOINT There, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale);
	void LineFromHereToThere(CDoubleSize There, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale);
	void LineFromHereToThere(CCadPoint* pThere, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale);
	BOOL FloodFill(
		COLORREF colorBorder,
		COLORREF colorFill,
		CDC* pDC, 
		DOUBLEPOINT& LLHC, 
		CScale& Scale
	);
	CPoint ToPixelPoint(DOUBLEPOINT& LLHC, CScale& Scale);
	CPoint ToPixelPointWithRotation(
		DOUBLEPOINT& LLHC, 
		CScale& Scale,
		CCadPoint* pPivot,
		CCadPoint* pRotation
	);
	//---------- Attributes --------------------
	void CopyAttributesTo(SPointAttributes* pAttrb);
	void CopyAttributesFrom(SPointAttributes* pAttrb);
	SPointAttributes& GetAttributes() { return m_Attributes; }
	SPointAttributes* GetPtrToAttributes() { return &m_Attributes; }
	//--------------------------------------------
	// Parse (LoaD) and Save Methods
	//--------------------------------------------
	virtual void Parse(
		CParser* pParser,	// pointer to parser
		Token TypeToken = Token::DEFAULT // Token type to save object as
	);
	virtual void Save(
		CFile* pcfFile,
		int Indent
	);
	//--------------------------------------------
	// Copy Object Methods
	//--------------------------------------------
	virtual CCadObject* Copy();
	virtual void CopyAttributes(CCadObject* pToObj);
	//---------------------------------------------
	// Draw Object Methods
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties();
	//-----------------------------------
	// Pointy Things to do with Shapes
	//-----------------------------------
	void ToPixelRect(CCadPoint* pP2, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale);
	void ToPixelRect(double w_half, double h_half, CRect& rect, DOUBLEPOINT& LLHC, CScale& Scale);
	void ToPixelRect(CCadPoint* pP2, CRect& rect, DOUBLEPOINT& LLHC, CScale& Scale);
	void ToPixelArc(
		CCadPoint* pP2, 
		CCadPoint* pStart, 
		CCadPoint* pEnd, 
		CDC* pDC, 
		DOUBLEPOINT& LLHC, 
		CScale& Scale
	);
	void ToPixelArc(
		double Radius_A,
		double Radius_B,
		CCadPoint* pStart,
		CCadPoint* pEnd,
		CDC* pDC,
		DOUBLEPOINT& LLHC,
		CScale& Scale
	);
	void ToPixelEllipse(CCadPoint* pP2, CDC* pDC, DOUBLEPOINT& LLHC, CScale& Scale);
	void ToPixelRndRect(
		CCadPoint* pP2, 
		CCadPoint* pP3, 
		CDC* pDC, 
		DOUBLEPOINT& LLHC,
		CScale& Scale
	);
	//----------------------------------
	// Pointy Things to do with lines
	//----------------------------------
	CCadPoint* Reflect(CCadPoint* pReflect, UINT mode);
	UINT Slope(double *pSlope, CCadPoint* pPoint) const;
	UINT Slope(double* pSlope, DOUBLEPOINT point) const;
	UINT OrthogonalSlope(double *Slope, CCadPoint *pPoint);
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
		DOUBLEPOINT P1,	//point (Pivot origin)
		DOUBLEPOINT P2,	//A point that dfines the line
		double Distance	//Distance from Pivot point
	);
	void PointOnLineAtDistance(
		CCadPoint* pP1,		//pivot point
		double m,			// slope of line
		double Distance		//distance from pivot
	);
	void PointOnLineAtDistance(
		DOUBLEPOINT P1,		//pivot point
		double m,			//// slope of line
		double Distance		//distance from pivot
	);
	void PointOnLineAtDistance(
		CCadPoint* P1,				//starting point
		DOUBLEPOINT RotationPoint,
		double Distance
	);
	CCadPoint* CenterPoint(CCadPoint* pFirstPoint, CCadPoint* pOtherPoint);
	BOOL IsPointBetween(CCadPoint* pP1, CCadPoint* pP2);
	int WhichSideOfLineIsPoint(CCadPoint* pPtOther, DOUBLEPOINT Point);
	double DistanceTo(CCadPoint* pP);
	//-------------------------------------------------
	void Print(const char* s);
	void CreateThePen(MODE mode, CPen* pen, int Lw);
	void CreateTheBrush(MODE mode, CBrush* brushFill);
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
	CCadPoint operator+(DOUBLEPOINT const& a) {
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
	CCadPoint operator-(DOUBLEPOINT const& a) {
		CCadPoint result;

		result.dX = dX - a.dX;
		result.dY = dY - a.dY;
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

		result.dX = dX * s.dSX;
		result.dY = dY * s.dSY;
		return result;
	}
	BOOL operator!=(CCadPoint const &p) {
		return (dX != p.dX || dY != p.dY);
	}
	BOOL operator==(CCadPoint const &p) {
		return (dX == p.dX && dY == p.dY);
	}
	//------------- Static Methods -----------------------
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
