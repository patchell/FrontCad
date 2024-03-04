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
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual int GetDefaultToken() { return TOKEN_BITMAP; }
	virtual void Move(CDoubleSize Diff);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	virtual BOOL IsEnclosedShapeIntrinsic() { return FALSE; }
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
	virtual CDoubleSize GetSize();
	//----------- Attribute Methods ------------------
	SBitmapAttributes& GetAttributes() { return m_Attributes; }
	SBitmapAttributes* GetPtrToAttributes() { return &m_Attributes; }
	void CopyAttributesTo(SBitmapAttributes* pAttrib);
	void CopyAttributesFrom(SBitmapAttributes* pAttrib);
	BOOL IsAspectRationMaintained() { return GetAttributes().m_MaintainAspectRatio; }
	//--------------------------------------------
	// Parse (LoaD) and Save Methods
	//--------------------------------------------
	virtual int Parse(
		CFile* pcfInFile,
		int Token,	// Lookahead Token
		CFileParser* pParser,	// pointer to parser
		int TypeToken = TOKEN_DEFAULT // Token type to save object as
	);
	virtual void Save(
		CFile* pcfFile,
		int Indent,
		int flags
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

