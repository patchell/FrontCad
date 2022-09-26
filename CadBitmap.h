#pragma once

class CCadPoint;

class CCadBitmap :public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_BitmapCount;
	inline static SBitmapAttributes m_LastAttributes;
	inline static SBitmapAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CMyBitmap *m_pBM;
	CString m_csBMFileName;
	SBitmapAttributes m_Attributes;
public:
	CCadBitmap();
	virtual ~CCadBitmap();
	virtual BOOL Create(CCadObject* pParent, CCadObject* pOrigin, SubType type = SubType::DEFALT);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& ULHC, CScale& Scale);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject ** ppSelList,
		int index, 
		int n,
		UINT nKinds
	);
	virtual CString& GetTypeString(void);
	virtual CString& GetObjDescription();
	virtual CCadObject * CopyObject(void);
	virtual CDoubleSize GetSize();
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	//----------- Attribute Methodes ------------------
	SBitmapAttributes& GetAttributes() { return m_Attributes; }
	SBitmapAttributes* GetPtrToAttributes() { return &m_Attributes; }
	void CopyAttributesTo(SBitmapAttributes* pAttrib);
	void CopyAttributesFrom(SBitmapAttributes* pAttrib);
	BOOL IsAspectRationMaintained() { return GetAttributes().m_MaintainAspectRatio; }
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//-----------------------------------------------
	virtual int EditProperties();
	void RestoreAspectRatio();
	void LoadBitmapImage(CString csPath);
	CString& GetBitMapFileName() { return m_csBMFileName; }
	//--------------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SBitmapAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

