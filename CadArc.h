#pragma once


class CCadArc :public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_ArcCount;
	inline static SArcAttributes m_LastAttributes;
	inline static SArcAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	//----------- Atributes ------------------
	SArcAttributes m_Attrb;
public:
	CCadArc();
	virtual ~CCadArc();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual void UpdateEnclosure();
	virtual Token GetDefaultToken() { return Token::ARC; }
	virtual void Move(CDoubleSize Diff);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
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
	virtual CDoubleSize GetSize();
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
//	void DrawArc(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	double CalcY(double x, double A, double B);
	BadDelta DeltaIsBad(CPoint P1, CPoint P2);
	CDoubleSize SlopeIsOneAt(double Asquared, double Bsquared);
	//----------------------------------------
	// Attribute Methods
	//----------------------------------------
	virtual int EditProperties();
	void CopyAttributesTo(SArcAttributes* pAttrb);
	void CopyAttributesFrom(SArcAttributes* pAttrb);
	SArcAttributes& GetAttributes() { return m_Attrb; }
	SArcAttributes* GetPtrToAttributes() { return &m_Attrb; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth = w; }
	//--------------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SArcAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

