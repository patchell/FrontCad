#pragma once

enum {
	TEXT_STRING_EXTERNAL,
	TEXT_STRING_LOCAL
};

class CCadText:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_TextCount;
	inline static STextAttributes m_LastAttributes;
	inline static STextAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	STextAttributes m_Attrib;
	CString m_csText;	//Text that will be displayed
	double m_Angle;
public:
	CCadText();
	virtual ~CCadText();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual void UpdateEnclosure();
	virtual Token GetDefaultToken() { return Token::TEXT; }
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
	virtual CString& GetTypeString();
	virtual CString& GetObjDescription();
	void CopyAttributesFrom(STextAttributes *pAttrb);
	void CopyAttributesTo(STextAttributes *pAttrb);
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
	void SetText(CString& csText) {
		m_csText = csText;
//		Rotate(m_Angle, m_rectSelect);
	}
	void GetText(CString& csText) const { csText = m_csText; }
	CString& GetText() { return m_csText; }
	void SetAngle(double Angle) {
		m_Angle = Angle;
		//		Rotate(Angle, m_rectSelect);
	}
	double GetAngle() { return m_Angle; }
	//----------------------------------------
	// Attribute methods
	//----------------------------------------
	STextAttributes &GetAttributes() { return m_Attrib; }
	STextAttributes *GetPtrToAttributes() { return &m_Attrib; }
	void SetFontName(CString& csFontName) {
		GetAttributes().m_csFontName = csFontName;
	}
	CString& GetFontName() { return GetAttributes().m_csFontName; }
	void Rotate();
	void SetFontWeight(UINT w) { GetAttributes().m_fontWeight = w; }
	UINT GetFontWeight() { return GetAttributes().m_fontWeight; }
	void SetFontHeight(double h) { GetAttributes().m_fontHeight = h; }
	double GetFontHeight() { return GetAttributes().m_fontHeight; }
	void SetFontWidth(double w) { GetAttributes().m_fontWidth = w; }
	double GetFontWidth() { return GetAttributes().m_fontWidth; }
	void SetTextColor(COLORREF c) { GetAttributes().m_colorText; }
	COLORREF GetTextColor() { return GetAttributes().m_colorText; }
	void SetBackgroundColor(COLORREF c) { GetAttributes().m_colorBK = c; }
	COLORREF GetBackgroundColor() { return GetAttributes().m_colorBK; }
	void SetFormat(UINT f = DT_BOTTOM | DT_SINGLELINE) { GetAttributes().m_Format = f; }
	UINT GetFormat() { return GetAttributes().m_Format; }
	void SetTransparent(UINT t = TRUE) { GetAttributes().m_Transparent = t; }
	UINT GetTransparent() { return GetAttributes().m_Transparent; }
	//--------------------------------------------
	static void RenderEnable(int e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static STextAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

