#pragma once

class CCadArrow :public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_ArrowCount;
	inline static SArrowAttributes m_LastAttributes;
	inline static SArrowAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SArrowAttributes m_Attrib;
public:
	CCadArrow();
	virtual ~CCadArrow();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual void UpdateEnclosure();
	virtual Token GetDefaultToken() { return Token::ARROW; }
	virtual void Move(CDoubleSize Diff);
	virtual CDoubleSize GetSize();
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT pPoint,
		CCadObject* pExcludeObject,
		CCadObject** ppSelList,
		int index,
		int n,
		UINT nKinds
	);
	virtual CString& GetTypeString();
	virtual CString& GetObjDescription();
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
	void Rotate(DOUBLEPOINT MousePos);
	//----------------------------------
	// attributes Methods
	//----------------------------------
	virtual int EditProperties();
	SArrowAttributes& GetAttributes() { return m_Attrib; }
	SArrowAttributes* GetPtrToAttributes() { return &m_Attrib; }
	void CopyAttributesTo(SArrowAttributes* pAttrb);
	void CopyAttributesFrom(SArrowAttributes* pAttrb);
	void ApplyParameters(double L1, double L2, double W);
	COLORREF GetLineColor() { GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c){ GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor() { GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	double GetL1();
	double GetL2();
	double GetW();
	void MakeCPointArray(CPoint* PolyPoints, CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);;
	//--------------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SArrowAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

