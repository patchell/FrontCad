#pragma once

class CCadDimension:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_DimensionCount;
	inline static SCadDimAttributes m_LastAttributes;
	inline static SCadDimAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CCadPoint m_P1, m_P2;
	SCadDimAttributes m_DimensionAttributes;
public:
	CCadDimension();
	virtual ~CCadDimension();
	virtual void UpdateEnclosure();
	virtual void OnCreate();
	virtual Token GetDefaultToken() { return Token::DIMENSION; }
	virtual void Move(CDoubleSize Diff);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual BOOL PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject ** ppSelList,
		int index, 
		int n,
		UINT nKinds
	);
	virtual CString& GetTypeString();
	virtual CString& GetObjDescription();
	virtual CPoint ScalePoint(CPoint p, int Scale, int ScaleDiv);
	void CopyAttributesFrom(SCadDimAttributes *pAttrib);
	void CopyAttributesTo(SCadDimAttributes* pAttrib);
	int EditProperties();
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
	//----------------------------------------------------
	//----------------------------------------------------
	SCadDimAttributes& GetAttributes() { return m_DimensionAttributes; }
	SCadDimAttributes* GetPtrToAttributes() { return &m_DimensionAttributes; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	COLORREF GetTextColor() { return GetAttributes().m_colorText; }
	COLORREF GetTextBkColor() { return GetAttributes().m_colorBKG; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	double GetTextHeight() { return GetAttributes().m_TextHeight; }
	//--------------------------------------------
	static void RenderEnable(int e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static SCadDimAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

class CDimLine :public CCadLine
{
	double m_Slope;
	double m_yIntercept;
	UINT m_LineType;
public:
	CDimLine() {
		m_Slope = 0.0;
		m_yIntercept = 0;
		m_LineType = 0;
	};
	void Create(CCadPoint P1, CCadPoint P2, UINT LineType);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	//---------------------------------------------
	void SetSlope(double m) { m_Slope = m; }
	double GetSlope() const { return m_Slope; }
	void SetYIntercept(double b) { m_yIntercept = b; }
	double GetYIntercept() { return m_yIntercept; }
	void SetLineType(UINT lt) { m_LineType = lt; }
	UINT GetLineType() const { return m_LineType; }
};

