#pragma once

enum {
	TEXT_STRING_EXTERNAL,
	TEXT_STRING_LOCAL
};

class CCadText:public CCadObject
{
	inline static int m_RndRectCount;
	inline static STextAttributes m_LastAttributes;
	inline static STextAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CDoublePoint m_P1;
	STextAttributes m_Attrib;
	CString m_csText;	//Text that will be displayed
	CFont *m_pFont;
	CPen *m_pSelPen;
	CDoubleRect m_rectSelect;
	double m_LastScaleX;
public:
	CCadText();
	CCadText(CCadText& v);
	virtual ~CCadText();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int PointInObjectAndSelect(
		CDoublePoint p, 
		CCadObject ** ppSelList = 0, 
		int index = 0, int n = 0, 
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual void AdjustReference(CDoubleSize Ref);
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadText operator=(CCadText &v);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CDoublePoint& GetCenter(CDoublePoint& Center);
	virtual void ChangeCenter(CDoubleSize p);
	virtual CDoubleSize& GetSize(CDoubleSize& size);
	virtual void ChangeSize(CSize Sz);
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	void CopyAttributesFrom(STextAttributes *pAttrb);
	void CopyAttributesTo(STextAttributes *pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties(void);
	void UpdateText(void);
	CDoublePoint CalcTextShiftonRotation(CDoublePoint Pivot, double radius, double angle);
	void SetText(CString& csText) {
		m_csText = csText;
		/// create the selection rectangle
		m_rectSelect = GetTextRectangle(m_rectSelect);
		Rotate(GetAttributes().m_Angle, m_rectSelect);
	}
	void GetText(CString& csText) { csText = m_csText; }
	CString& GetText(void) { return m_csText; }
	//----------------------------------------
	// Attribute methods
	//----------------------------------------
	STextAttributes &GetAttributes() { return m_Attrib; }
	STextAttributes *GetPtrToAttributes() { return &m_Attrib; }
	CDoubleRect& GetTextRectangle(CDoubleRect &rect);
	CDoubleRect& GetTextRectangle(CDC* pDC, CDoubleRect& rect, CScale Scale);
	void SetFontName(CString& csFontName) {
		GetAttributes().m_csFontName = csFontName;
	}
	CString& GetFontName() { return GetAttributes().m_csFontName; }
	void Rotate(double Angle, CDoubleRect& rect);
	void SetAngle(double Angle) {
		GetAttributes().m_Angle = Angle;
		m_rectSelect = GetTextRectangle(m_rectSelect);
		Rotate(Angle, m_rectSelect);
	}
	double GetAngle() { return GetAttributes().m_Angle; }
	void SetFontWeight(UINT w) { GetAttributes().m_fontWeight = w; }
	UINT GetFontWeight() { return GetAttributes().m_fontWeight; }
	void SetFontHeight(UINT h) { GetAttributes().m_fontHeight = h; }
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
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	STextAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

