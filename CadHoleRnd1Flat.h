#pragma once

class CCadHoleRnd1Flat:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RndHole1FlatCount;
	inline static SRndHole1FlatAttributes m_LastAttributes;
	inline static SRndHole1FlatAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SRndHole1FlatAttributes m_Attrib;
public:
	CCadHoleRnd1Flat();
	virtual ~CCadHoleRnd1Flat();
	virtual BOOL Create(CCadObject* pParent, CCadObject* pOrigin, SubType type = SubType::DEFALT);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& ULHC, CScale& Scale);
	void SolveIntersection();
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject** ppSelList,
		int index,
		int n,
		UINT nKinds
	);
	virtual CString& GetTypeString(void);
	virtual CString& GetObjDescription();
	virtual CCadObject * CopyObject(void);
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
	void CreateThePen(MODE mode, CPen* pen, int Lw);
	SRndHole1FlatAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF color) { GetAttributes().m_colorLine = color; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth; }
	double GetHoleRadius() { return GetAttributes().m_HoleRadius; }
	void SetHoleRadius(double d) { GetAttributes().m_HoleRadius = d; }
	//--------------------------------------------
	static void RenderEnable(int e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static SRndHole1FlatAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
	//----------------------------------------------
	virtual void Print(const char* pTitle);
};

