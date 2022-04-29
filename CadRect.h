#pragma once

class CCadRect:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RectCount;
	inline static SRectAttributes m_LastAttributes;
	inline static SRectAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CPen *m_pPenLine;
	CBrush *m_pBrFill;
	SRectAttributes m_Attrib;
	CDoubleRect m_Rect;
	CString m_csName;
public:
	CCadRect();
	CCadRect(CCadRect &r);
	virtual ~CCadRect();
	void EnableRener(BOOL en) { m_RenderEnable = en; }
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	virtual int PointInObjectAndSelect(CDoublePoint p, CCadObject ** ppSelList = 0, int index = 0, int n = 0, DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL);
	virtual CDoublePoint GetReference();
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadRect operator=(CCadRect &v);
	virtual CCadObject * CopyObject(void);
	virtual void RenderEnable(int e);
	virtual CDoublePoint& GetCenter(CDoublePoint& Center);
	virtual CDoubleSize& GetSize(CDoubleSize& size);
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	void CopyAttributesTo(SRectAttributes *pAttrb);
	void CopyAttributesFrom(SRectAttributes *pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties();
	//------------------------------------------
	// Attribute Methods
	//-----------------------------------------
	SRectAttributes* GetPtrToAttributes() { return &m_Attrib; }
	SRectAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor(void) { return GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	UINT GetTransparent(){ return GetAttributes().m_TransparentFill; }
	void SetTransparent(UINT t) { GetAttributes().m_TransparentFill = t; }
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SRectAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

