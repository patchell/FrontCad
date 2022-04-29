#pragma once

enum class Intercept {
	LEFTUPPER,
	LEFTLOWER,
	RIGHTUPPER,
	RIGHTLOWER
};

class CCadHoleRnd2Flat:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RoundHole2FlatCount;
	inline static SRndHole2FlatAttributes m_LastAttributes;
	inline static SRndHole2FlatAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CPen *m_pPenLine;
	SRndHole2FlatAttributes m_Attrib;
	CDoublePoint m_Center;
public:
	CCadHoleRnd2Flat();
	virtual ~CCadHoleRnd2Flat();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	double CalculateAngle(double FlatDist, double Radius);
	CDoublePoint SolveIntersection(Intercept mode, CDoublePoint Center, double FlatDist, double Radius);
	virtual int PointInObjectAndSelect(
		CDoublePoint p,
		CCadObject** ppSelList = 0,
		int index = 0,
		int n = 0,
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadHoleRnd2Flat operator=(CCadHoleRnd2Flat &v);
	virtual CPoint ScalePoint(CPoint p, int Scale, int ScaleDiv);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CDoublePoint GetCenter();
	virtual CDoubleSize GetSize();
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	void CopyAttributesTo(SRndHole2FlatAttributes *pAttrb);
	void CopyAttributesFrom(SRndHole2FlatAttributes *pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//---------------------------------------------
	// Object Attributes
	//---------------------------------------------
	virtual int EditProperties();
	SRndHole2FlatAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF color) { GetAttributes().m_colorLine = color; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth; }
	double GetHoleRadius() { return GetAttributes().m_HoleRadius; }
	void SetHoleRadius(double d) { GetAttributes().m_HoleRadius = d; }
	double GetDistanceFromCenterToFlat() { return GetAttributes().m_FlatDistanceFromCenter; }
	void SetDistanceFromCenterToFlat(double dist) { GetAttributes().m_FlatDistanceFromCenter = dist; }
	CDoublePoint GetCenterPoint() { return m_Center; }
	void SetCenterPoint(CDoublePoint pt) { m_Center = pt; }
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SRndHole2FlatAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

