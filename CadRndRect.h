#pragma once

class CCadRndRect:public CCadObject
{
	friend CCadPoint;
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RoundedRectCount;
	inline static SRoundedRectAttributes m_LastAttributes;
	inline static SRoundedRectAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SRoundedRectAttributes m_Attrib;
public:
	CCadRndRect();
	virtual ~CCadRndRect();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual void UpdateEnclosure();
	virtual Token GetDefaultToken() { return Token::RNDRECT; }
	virtual void Move(CDoubleSize Diff);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject ** ppSelList,
		int index, 
		int n,
		UINT nKinds
	);
	virtual CDoubleSize GetSize();
	virtual CString& GetTypeString();
	virtual CString& GetObjDescription();
	void CopyAttributesTo(SRoundedRectAttributes *pAttrb);
	void CopyAttributesFrom(SRoundedRectAttributes*pAttrb);
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
	COLORREF CreateThePen(MODE mode, CPen* pen, int Lw);
	void CreateTheBrush(MODE mode, CBrush* brushFill);
	virtual int EditProperties();
	//------------------------------------------
	// Attribute Methods
	//-----------------------------------------
	SRoundedRectAttributes* GetPtrToAttributes() { return &m_Attrib; }
	SRoundedRectAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor() { return GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	UINT GetTransparent() { return GetAttributes().m_TransparentFill; }
	void SetTransparent(UINT t) { GetAttributes().m_TransparentFill = t; }
	//--------------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SRoundedRectAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

