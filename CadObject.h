#pragma once

class CCadPoint;


constexpr auto SUBSUBTYPE_ANY = 0;
constexpr auto RECT_SUBSUB_UL = 1;
constexpr auto RECT_SUBSUB_LL = 2;
constexpr auto RECT_SUBSUB_LR = 3;
constexpr auto RECT_SUBSUB_UR = 4;

class CCadObject
{
public:
	//--------------------------------------
	// Object Type/ Object Kind Cross Ref
	//--------------------------------------
	enum class ObjectType {
		BASE = 0,
		ARC,
		ARCCENTERED,
		ARROW,
		BITMAPTYPE,
		DIMENSION,
		DRAWING,
		ELIPSE,
		HOLE_RECTANGLE,
		HOLE_RND1FLAT,
		HOLE_RND2FLAT,
		HOLE_ROUND,
		LIBPART,
		LIBRARY,
		LINE,			//drawing objects
		ORIGIN,
		POINT,
		POLYGON,
		PRINTRECT,
		RECT,
		RECT_ROTATED,
		ROUNDEDRECT,
		TEXT,
		CLIPBOARD
	};
private:
	struct TypeKindXREF {
		ObjectType Type;
		UINT Kind;
		TypeKindXREF() {
			Type = ObjectType::BASE;
			Kind = OBJKIND_ALL;
		}
		TypeKindXREF(ObjectType T, UINT K) {
			Type = T;
			Kind = K;
		}
	};
	static inline TypeKindXREF XREF[22] = {
		{ ObjectType::BASE,OBJKIND_ALL},
		{ ObjectType::ARC,OBJKIND_ARC},
		{ ObjectType::ARCCENTERED,OBJKIND_ARCCENTERED},
		{ ObjectType::ARROW,OBJKIND_ARROW},
		{ ObjectType::BITMAPTYPE,OBJKIND_BITMAP},
		{ ObjectType::DIMENSION,OBJKIND_DIMENSION},
		{ ObjectType::DRAWING,OBJKIND_DRAWING},
		{ ObjectType::ELIPSE,OBJKIND_ELIPSE},
		{ ObjectType::HOLE_RECTANGLE,OBJKIND_HOLE_RECTANGLE},
		{ ObjectType::HOLE_RND1FLAT,OBJKIND_HOLE_RND1FLAT},
		{ ObjectType::HOLE_RND2FLAT,OBJKIND_HOLE_RND2FLAT},
		{ ObjectType::HOLE_ROUND,OBJKIND_HOLE_ROUND},
		{ ObjectType::LIBPART,OBJKIND_LIBPART},
		{ ObjectType::LINE,	OBJKIND_LINE},
		{ ObjectType::ORIGIN,OBJKIND_ORIGIN},
		{ ObjectType::POINT,OBJKIND_POINT},
		{ ObjectType::POLYGON,OBJKIND_POLYGON},
		{ ObjectType::RECT,OBJKIND_RECT},
		{ ObjectType::RECT_ROTATED, OBJKIND_RECT_ROTATED},
		{ ObjectType::ROUNDEDRECT,OBJKIND_ROUNDEDRECT},
		{ ObjectType::TEXT,OBJKIND_TEXT}
	};
public:
	enum class SubTypes {
		NONE = -1,
		ANY,				//0
		DEFAULT,			//1
		RECT_ROTATED,		//2
		RECT_BASE_DEFINED,//3
		RECT_FROM_CENTER,
		LINE_FIXED_LEN,	//4
		//--- Arc/Ellipse ---
		RECTSHAPE,		//5
		STARTPOINT,		//6
		ENDPOINT,			//7
		//--- Arrow ---
		ARROW_TIP,		//8
		ARROW_END,		//9
		ARROW_TOP,		//10
		ARROW_BOT,		//11
		ARROW_ROTATION,	//12
		//--- Rectangle ---
		RECT_TOP_CENTER,	//13
		RECT_BOT_CENTER,	//14
		RECT_LEFT_CENTER,	//15
		RECT_RGIHT_CENTER,//16
		CORNER_RADIUS,	//17
		//--- Text ---
		TEXT_LOCATION,	//18
		TEXT_ROTATION,	//19
		TEXT_RECT,		//20
		//--- Misc ---
		ORIGIN_LOCATION,	//21
		CENTERPOINT,		//22
		PIVOTPOINT,		//23
		MIDPOINT,			//24
		ROTATION_POINT,	//25
		VERTEX,			//26
		RIGHTANGLE_VERTEX,	//27
		REFERENCE_POINT		//28
	};
private:
	struct SubTypeTable {
		SubTypes m_SubType;
		CString m_csName;
		SubTypeTable() {
			m_SubType = SubTypes(0);
		}
		SubTypeTable(SubTypes ST,CString csN) {
			m_SubType = ST;
			m_csName = csN;
		}
	};

	static inline SubTypeTable SubTypeStringsLUT[] = {
		{SubTypes::ANY,_T("ANY")},				//0
		{SubTypes::DEFAULT, _T("DEFAULT")},			//1
		{SubTypes::RECT_ROTATED,_T("RECT_ROTATED")},		//2
		{SubTypes::RECT_BASE_DEFINED,_T("RECT_BASE_DEFINED")},//3
		{SubTypes::LINE_FIXED_LEN, _T("LINE_FIXED_LEN")},	//4
		//--- Arc/Ellipse ---
		{SubTypes::RECTSHAPE, _T("RECTSHAPE")},		//5
		{SubTypes::STARTPOINT, _T("STARTPOINT")},		//6
		{SubTypes::ENDPOINT, _T("ENDPOINT")},			//7
		//--- Arrow ---
		{SubTypes::ARROW_TIP,_T("ARROW_TIP")},		//8
		{SubTypes::ARROW_END,_T("ARROW_END")},		//9
		{SubTypes::ARROW_TOP,_T("ARROW_TOP")},		//10
		{SubTypes::ARROW_BOT,_T("ARROW_BOT")},		//11
		{SubTypes::ARROW_ROTATION,_T("ARROW_ROTATION")},	//12
		//--- Rectangle ---
		{SubTypes::RECT_TOP_CENTER,_T("RECT_TOP_CENTER")},	//13
		{SubTypes::RECT_BOT_CENTER,_T("RECT_BOT_CENTER")},	//14
		{SubTypes::RECT_LEFT_CENTER,_T("RECT_LEFT_CENTER")},	//15
		{SubTypes::RECT_RGIHT_CENTER,_T("RECT_RGIHT_CENTER")},//16
		{SubTypes::CORNER_RADIUS,_T("CORNER_RADIUS")},	//17
		//--- Text ---
		{SubTypes::TEXT_LOCATION,_T("TEXT_LOCATION")},	//18
		{SubTypes::TEXT_ROTATION,_T("TEXT_ROTATION")},	//19
		{SubTypes::TEXT_RECT,_T("TEXT_RECT")},		//20
		//--- Misc ---
		{SubTypes::ORIGIN_LOCATION,_T("ORIGIN_LOCATION")},	//21
		{SubTypes::CENTERPOINT,_T("CENTERPOINT")},		//22
		{SubTypes::PIVOTPOINT,_T("PIVOTPOINT")},		//23
		{SubTypes::MIDPOINT,_T("MIDPOINT")},			//24
		{SubTypes::ROTATION_POINT,_T("ROTATION_POINT")},	//25
		{SubTypes::VERTEX,_T("VERTEX")},			//26
		{SubTypes::RIGHTANGLE_VERTEX,_T("RIGHTANGLE_VERTEX")},	//27
		{SubTypes::REFERENCE_POINT,_T("REFERENCE_POINT")},	//28
		{SubTypes::NONE,_T("")}
	};
	//--------------------------------------
	// Object Properties
	//--------------------------------------
	CString m_csName;
	CString m_csDescription;
	UINT m_Id;	//unique ID number
	//------------------------
	ObjectDrawState m_CurrentDrawState;
	//------------------------
	ObjectType m_Type;		//specifies the type of object
	SubTypes m_SubType;		//Specifies the subtype of the object
	UINT m_SubSubType;		//Differentiates between different objects of same subtype
	//-----------------------
	BOOL m_Selected;	//indicates that object is in the selected state
	BOOL m_bAttributesValid;
	//-----------------------
	DOUBLEPOINT m_Ref;	//reference point for an object (CCadLibPart for example)
	//------------ Links for Main List of Drawing Objects ------------------
	CCadObject *m_pNext;	//next object in drawing
	CCadObject *m_pPrev;	//previous object in drawing
	CCadObject* m_pHead;	//Head of main list of drawing objects
	CCadObject* m_pTail;	//Tail of main list of drawing objects
	UINT m_nTotalObjects;
	//------------------------------------
	// Parrent Objects
	//------------------------------------
	CCadObject* m_pParentObject;
	CCadObject* m_pOrigin;
public:
	//-------------------------------------
	// Object Creation/Destruction Methods
	//-------------------------------------
	CCadObject();
	CCadObject(ObjectType Type);
	virtual ~CCadObject();
	virtual BOOL Create(CCadObject* pParent, SubTypes type) {
		m_pParentObject = pParent;
		m_SubType = type;
		return TRUE;
	}
	virtual CCadObject* Destroy(CCadObject* pDependentObject);
	virtual int CheckSelected(
		DOUBLEPOINT p, 
		CCadObject** ppSelList, 
		int& n, 
		int flag
	);
	//-----------------------------------------------------
	virtual DOUBLEPOINT GetRef() { return m_Ref; }
	virtual void SetRef(DOUBLEPOINT refpoint) { m_Ref = refpoint; }
	virtual void SelectAll(UINT ObjectKind);
	virtual void UnSelectAll(UINT ObjectKind);
	virtual int Print(int Indent);
	virtual void Print(const char* pTitle) {}
	virtual void SetParent(CCadObject* pP) { m_pParentObject = pP; }
	virtual CCadObject* GetParent() const { return m_pParentObject; }
	virtual void SetOrigin(CCadObject* pOrg) { m_pOrigin = pOrg; }
	virtual CCadObject* GetOrigin() { return m_pOrigin; }
	UINT GetId() { return m_Id; }
	ObjectType GetType(void) { return m_Type; }
	void SetType(ObjectType type) { m_Type = type; }
	SubTypes GetSubType() { return m_SubType; }
	void SetSubType(SubTypes subtype) { m_SubType = subtype; }
	UINT GetSubSubType() { return m_SubSubType; }
	void SetSubSubType(UINT SST) { m_SubSubType = SST; }
	ObjectDrawState GetCurrentDrawState() { return m_CurrentDrawState; }
	void SetCurrentDrawState(ObjectDrawState State) { m_CurrentDrawState = State; }
	BOOL AttributesAreValid() {
		return m_bAttributesValid;
	}
	void SetAttributesValid() { m_bAttributesValid = TRUE; }
	//------------------------------------
	// Drawing Manipulation Methods
	//------------------------------------
	virtual void Flip(CCadPoint* Pivot, int Direction) {}
	virtual void Move(CDoubleSize Diff);
	//------------------------------------
	// load and save files
	//------------------------------------
	virtual void Save(
		FILE* pO, 
		CLexer::Tokens Token, 
		int Indent, 
		int flags = 0
	) {}
	virtual CLexer::Tokens Parse(
		CLexer::Tokens Token,
		CFileParser* pParser
	)
	{
		return Token;
	}
	//-----------------------------------------
	// Draw The Drawing
	//-----------------------------------------
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	//------------------------------------------
	// Get things about the object
	//------------------------------------------
	virtual CLexer::Tokens GetDefaultToken() { return CLexer::Tokens::BASE_OBJ; }
	virtual BOOL PointInThisObject(DOUBLEPOINT point) { return FALSE; }
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject** ppSelList,
		int index,
		int n,
		UINT nKinds
	);
	virtual BOOL DontExclude(CCadObject* ExcludeThisObject) {
		return (this != ExcludeThisObject);
	}
	virtual BOOL ShouldWeSelectThisObjectAndDidIt(UINT nKind) {
		BOOL rV = FALSE;
		if (nKind & OBJKIND_SELECT)
		{
			rV = TRUE;
			SetSelected(TRUE);
		}
		return rV;
	}
	CString& GetName() { return m_csName; }
	void SetName(CString& csName) { m_csName = csName; }
	CString& GetDescription() { return m_csDescription; }
	virtual CString& GetTypeString(void);
	CString& GetSubTypeString(SubTypes SubTypeEnum);
	char* GetSubTypeString(char* pDest, SubTypes SubTypeEnum);
	char* GetCharSubTypeString(char* pDest, SubTypes SubTypeEnum);
	virtual CString& GetObjDescription();
	void CopyObject(CCadObject* pObjDestination);
	virtual CCadObject* CopyObject() { return NULL; }
	virtual CDoubleSize GetSize() { 
		return CDoubleSize(0.0, 0.0);
	};
	virtual CCadObject* GetVertex(UINT VertexNumber);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState) { return DrawState; }
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState) { return DrawState; }
	virtual void ProcessZoom(CScale& InchesPerPixel) {}
	//--------------------------------------------------------
	// Methods for managing object properites
	//--------------------------------------------------------
	virtual int EditProperties(void) { return IDCANCEL; }
	//------------------------------------------
	// Linked List Methods
	// -----------------------------------------
	// Main Links
	//------------------------------------------
	CCadObject* GetNext() { return m_pNext; }
	void SetNext(CCadObject* pO) { m_pNext = pO; }
	CCadObject* GetPrev() { return m_pPrev; }
	void SetPrev(CCadObject* pO) { m_pPrev = pO; }
	CCadObject* GetHead() { return m_pHead; }
	void SetHead(CCadObject* pH) { m_pHead = pH; }
	CCadObject* GetTail() { return m_pTail; }
	void SetTail(CCadObject* pT) { m_pTail = pT; }
	//-----------------
	virtual void AddObjectAtHead(CCadObject* pObj);
	virtual void AddObjectAtTail(CCadObject* pObj);
	virtual void RemoveObject(CCadObject* pObj);
	virtual CCadObject* RemoveObjectFromHead();
	virtual CCadObject* RemoveObjectFromTail();
	virtual void RemoveAndDestroyAll();
	virtual CCadObject* DeleteObject(CCadObject* pObj);
	virtual CCadObject* FindObject(ObjectType Type, SubTypes SubType, UINT SubSubType);
	//-------- Selected Objects ----------------
	virtual BOOL IsSelected(void) const { return m_Selected; }
	virtual BOOL IsNotSelected(void) const { return !m_Selected; }
	virtual void SetSelected(BOOL Flag) { m_Selected = Flag; }
	virtual void RemoveAndDestroySelectedObjects();
	int GetTotalNumberOfSelectedItems();
	//---------------------------------------------------
	//---------------------------------------------------
	UINT TypeToKind(ObjectType ObjType) {
		return XREF[UINT(ObjType)].Kind;
	}
	BOOL IsItThisKind(UINT Kinds) {
		BOOL rV = FALSE;
		if (TypeToKind(GetType()) & Kinds)
			rV = TRUE;
		return rV;
	}
};
