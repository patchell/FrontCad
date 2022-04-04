#pragma once

class CCadHoleRnd1Flat:public CCadObject
{
	inline static int m_DocCount;
	inline static SRndHole1FlatAttributes m_LastAttributes;
	inline static SRndHole1FlatAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CPen *m_pPenLine;
	SRndHole1FlatAttributes m_Attrib;
	CDoublePoint m_Center;
public:
	CCadHoleRnd1Flat();
	virtual void OnCreate();
	virtual ~CCadHoleRnd1Flat();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1, 0.1));
	double CalculateAngle(double FlatDist, double Radius);
	CDoublePoint SolveIntersection(
		int mode,
		CDoublePoint Center,
		double FlatDist,
		double Radius
	);
	virtual int PointInObjectAndSelect(
		CDoublePoint p,
		CCadObject** ppSelList = 0,
		int index = 0,
		int n = 0,
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual void AdjustReference(CDoubleSize DeltaRef);
	CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadHoleRnd1Flat operator=(CCadHoleRnd1Flat &v);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CDoublePoint GetCenter();
	virtual void ChangeCenter(CDoubleSize p);
	virtual CDoubleSize GetSize();
	virtual void ChangeSize(CSize Sz);
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	void CopyAttributesTo(SRndHole1FlatAttributes *pAttrb);
	void CopyAttributesFrom(SRndHole1FlatAttributes *pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//---------------------------------------------
	// Object Attributes
	//---------------------------------------------
	virtual int EditProperties();
	SRndHole1FlatAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF color) { GetAttributes().m_colorLine = color; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth; }
	double GetHoleRadius() { return GetAttributes().m_HoleRadius; }
	void SetHoleRadius(double d) { GetAttributes().m_HoleRadius = d; }
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SRndHole1FlatAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

