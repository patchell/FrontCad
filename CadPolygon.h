#pragma once

class CCadPolygon : public CCadObject
{
	friend CCadPoint;
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_PolygonCount;
	inline static SPolyAttributes m_LastAttributes;
	inline static SPolyAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	UINT m_NumVertices;
	DOUBLEPOINT m_FirstPoint;
	SPolyAttributes m_Attrib;
public:
	CCadPolygon();
	virtual ~CCadPolygon();
	virtual BOOL Create(CCadObject* pParent, CCadObject* pOrigin, SubType type = SubType::DEFALT);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE* pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	BOOL DrawPolygon(CDC* pDC, MODE mode, DOUBLEPOINT& ULHC, CScale& Scale);
	CCadPoint* GetCenter();
	void FillPolygon(CDC* pDC, MODE mode, DOUBLEPOINT& ULHC, CScale& Scale);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& ULHC, CScale& Scale);
	BOOL GetPoints(DOUBLEPOINT* pPoints);
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
	virtual CCadObject* CopyObject(void);
	BOOL GetMinMaxXY(double& MinX, double& MaxX, double& MinY, double& MaxY);
	virtual CDoubleSize GetSize();
	UINT GetNumVerticies() { return m_NumVertices; }
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex, DocFileParseToken TypeToken);
	//---------------------------------------
	// Other methodes
	//---------------------------------------
	CCadPoint *AddPoint(DOUBLEPOINT Point);
	BOOL PointEnclosed(CCadPoint p);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties();
	COLORREF CreateThePen(MODE mode, CPen* pen, int Lw);
	void CreateTheBrush(MODE mode, CBrush* brushFill);
	void CopyAttributesFrom(SPolyAttributes* pPA);
	void CopyAttributesTo(SPolyAttributes* pA);
	//------------------------------------------
	// Attribute Methods
	//-----------------------------------------
	SPolyAttributes* GetPtrToAttributes() { return &m_Attrib; }
	SPolyAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor(void) { GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor(void) { GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	UINT GetTransparent(){ return GetAttributes().m_TransparentFill; }
	void SetTransparent(UINT t) { GetAttributes().m_TransparentFill = t; }
	//--------------------------------------------
	static void RenderEnable(int e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static SPolyAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

