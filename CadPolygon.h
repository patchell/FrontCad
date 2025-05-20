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
	virtual BOOL Create(CCadObject* pParent, SubTypes type = CCadObject::SubTypes::DEFAULT);
	virtual void UpdateEnclosure();
	void AddVertices(int VertexCount, CCadObject::SubTypes SubType);
	virtual Token GetDefaultToken() { return Token::POLY; }
	virtual void Move(CDoubleSize Diff);
	BOOL GetPoints(DOUBLEPOINT** pPoints);
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
	virtual CString& GetTypeString();
	virtual CString& GetObjDescription();
	BOOL GetMinMaxXY(double& MinX, double& MaxX, double& MinY, double& MaxY);
	virtual CDoubleSize GetSize();
	UINT GetNumVertices() const { return m_NumVertices; }
	//---------------------------------------
	// Other methods
	//---------------------------------------
	CCadPoint *AddPoint(DOUBLEPOINT Point);
	BOOL PointEnclosed(CCadPoint p);
	//---------------------------------------------
	// Paint Methods
	//---------------------------------------------
	BOOL DrawPolygon(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	CCadPoint* CalculateCenterPoint();
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	COLORREF CreateTheBrush(MODE mode, CBrush* brushFill);
	COLORREF CreateThePen(MODE mode, CPen* pen, int Lw);
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
	CCadPoint* GetCenter();
	virtual int EditProperties();
	void CopyAttributesFrom(SPolyAttributes* pPA);
	void CopyAttributesTo(SPolyAttributes* pA);
	//------------------------------------------
	// Attribute Methods
	//-----------------------------------------
	SPolyAttributes* GetPtrToAttributes() { return &m_Attrib; }
	SPolyAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor() { GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor() { GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
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

