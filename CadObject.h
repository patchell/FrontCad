#pragma once

class CCadPoint;

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

class CCadObject
{
	//--------------------------------------
	// Object Type/ Object Kind Cross Ref
	//--------------------------------------
	static inline TypeKindXREF XREF[21] = {
		{ ObjectType::BASE,OBJKIND_ALL},
		{ ObjectType::ARC,OBJKIND_ARC},
		{ ObjectType::ARCCENTERED,OBJKIND_ARCCENTERED},
		{ ObjectType::ARCANGLE,OBJKIND_ARCANGLE},
		{ ObjectType::ARROW,OBJKIND_ARROW},
		{ ObjectType::BITMAP,OBJKIND_BITMAP},
		{ ObjectType::DIMENSION,OBJKIND_DIMENSION},
		{ ObjectType::ELIPSE,OBJKIND_ELIPSE},
		{ ObjectType::HOLE_RECTANGLE,OBJKIND_HOLE_RECTANGLE},
		{ ObjectType::HOLE_RND1FLAT,OBJKIND_HOLE_RND1FLAT},
		{ ObjectType::HOLE_RND2FLAT,OBJKIND_HOLE_RND2FLAT},
		{ ObjectType::HOLE_ROUND,OBJKIND_HOLE_ROUND},
		{ ObjectType::LIBCOMP,OBJKIND_LIBCOMP},
		{ ObjectType::LINE,	OBJKIND_LINE},
		{ ObjectType::ORIGIN,OBJKIND_ORIGIN},
		{ ObjectType::POINT,OBJKIND_POINT},
		{ ObjectType::POLYGON,OBJKIND_POLYGON},
		{ ObjectType::RECT,OBJKIND_RECT},
		{ ObjectType::ROUNDEDRECT,OBJKIND_ROUNDEDRECT},
		{ ObjectType::TEXT,OBJKIND_TEXT}
	};
	static inline CString SubTypeStrings[23] = {
	_T("ANY"),
	//--- Arc/Ellipse ---
	_T("RECTSHAPE"),
	_T("STARTPOINT"),
	_T("ENDPOINT"),
	//--- Arrow ---
	_T("ARROW_TIP"),
	_T("ARROW_END"),
	_T("ARROW_TOP"),
	_T("ARROW_BOT"),
	_T("ARROW_ROTATION"),
	//--- Rectangle ---
	_T("RECT_TOP_CENTER"),
	_T("RECT_BOT_CENTER"),
	_T("RECT_LEFT_CENTER"),
	_T("RECT_RGIHT_CENTER"),
	_T("CORNER_RADIUS"),
	//--- Text ---
	_T("TEXT_LOCATION"),
	_T("TEXT_ROTATION"),
	_T("TEXT_RECT"),
	//--- Misc ---
	_T("ORIGIN_LOCATION"),
	_T("CENTERPOINT"),
	_T("PIVOTPOINT"),
	_T("MIDPOINT"),
	_T("VERTEX"),
	_T("LINE_FIXED_LEN")
	};
	//--------------------------------------
	// Object Properties
	//--------------------------------------
	ObjectDrawState m_CurrentDrawState;
	CString m_csName;
	CString m_csDescription;
	UINT m_Id;	//unique ID number
	ObjectType m_Type;		//specifies the type of object
	SubType m_SubType;		//Specifies the subtype of the object
	UINT m_SubSubType;		//Differentiates between different objects of same subtype
	BOOL m_Selected;	//indicates that object is in the selected state
	BOOL m_bAttributesValid;
	//------------ Links for Main List of Drawing Objects ------------------
	CCadObject *m_pNext;	//next object in drawing
	CCadObject *m_pPrev;	//previous object in drawing
	//------------ Links for List of Selected Objects --------------------------------
	CCadObject *m_pNextSel;	//next selected object
	CCadObject* m_pPrevSel;	//previous selected object
	//----- child object List for compound objects -----------------------
	CCadObject* m_pChildHead;	//head pointer of child objects 
	CCadObject* m_pChildTail;	//tail pointer of child objects 
	UINT m_nTotalChildObjects;	//total number of child objects
	//------------------------------------------------
	//             dependent objects  
	//------------------------------------------------
	//----------- Dependent Children ----------------
	CCadObject* m_pHeadDependentChildren;
	CCadObject* m_pTailDependentChildren;
	CCadObject* m_pNextDependentChild;
	CCadObject* m_pPrevDependentChild;
	UINT m_nTotalDependentChildren;
	//---------- Dependent Parent List -------------
	CCadObject* m_pHeadDependentParents;
	CCadObject* m_pTailDependentParents;
	CCadObject* m_pNextDependentParent;
	CCadObject* m_pPrevDependentParent;
	UINT m_nTotalDependentParents;
	//---------------------------------
	// List of Origin Objects Links
	//---------------------------------
	CCadObject* m_pNextOrigin;
	CCadObject* m_pPrevOrigin;
	//------------------------------------
	// Clipboard Linked List
	//------------------------------------
	CCadObject* m_pNextClipBoard;
	CCadObject* m_pPrevClipBoard;
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
	virtual ~CCadObject();
	virtual BOOL Create(CCadObject* pParent, CCadObject* pOrigin) {
		m_pParentObject = pParent;
		m_pOrigin = pOrigin;
		return TRUE;
	}
	virtual BOOL Destroy(CCadObject* pDependentObject) { return TRUE; }
	int Print(int Indent);
	virtual void SetParent(CCadObject* pP) { m_pParentObject = pP; }
	virtual CCadObject* GetParent() const { return m_pParentObject; }
	virtual void SetOrigin(CCadObject* pOrg) { m_pOrigin = pOrg; }
	virtual CCadObject* GetOrigin() { return m_pOrigin; }
	UINT GetId() { return m_Id; }
	ObjectType GetType(void) { return m_Type; }
	void SetType(ObjectType type) { m_Type = type; }
	SubType GetSubType() { return m_SubType; }
	void SetSubType(SubType subtype) { m_SubType = subtype; }
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
	virtual void Move(CDoubleSize Diff);
	//------------------------------------
	// load and save files
	//------------------------------------
	virtual void Save(FILE* pO, DocFileParseToken Token, int Indent = 0, int flags = 0) {}
	virtual DocFileParseToken Parse(
		DocFileParseToken Token,
		CLexer* pLex
	)
	{
		return Token;
	}
	//-----------------------------------------
	// Draw The Drawing
	//-----------------------------------------
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale);
	//------------------------------------------
	// Get things about the object
	//------------------------------------------
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
	virtual BOOL IsSelected(void) const { return m_Selected; }
	virtual void SetSelected(BOOL Flag) { m_Selected = Flag; }
	CString& GetName() { return m_csName; }
	void SetName(CString& csName) { m_csName = csName; }
	CString& GetDescription() { return m_csDescription; }
	virtual CString& GetTypeString(void);
	CString& GetSubTypeString(SubType SubTypeEnum) { return SubTypeStrings[UINT(SubTypeEnum)]; }
	char* GetCharSubTypeString(char *pDest, SubType SubTypeEnum) {
		CString csST = GetSubTypeString(SubTypeEnum);
		return GETAPP.ConvertCStringToChar(pDest, csST);
	}
	virtual CString& GetObjDescription();
	void CopyObject(CCadObject* pObjDestination);
	virtual CCadObject* CopyObject() { return NULL; }
	virtual CDoubleSize GetSize() { 
		return CDoubleSize(0.0, 0.0);
	};
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState) { return DrawState; }
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState) { return DrawState; }
	virtual void Flip(CCadPoint* Pivot, int Direction){}
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
	CCadObject* GetNext(void) { return m_pNext; }
	void SetNext(CCadObject* pO) { m_pNext = pO; }
	CCadObject* GetPrev(void) { return m_pPrev; }
	void SetPrev(CCadObject* pO) { m_pPrev = pO; }
	//-------- Selected Object Links ----------------
	CCadObject* GetNextSel(void) { return m_pNextSel; }
	void SetNextSel(CCadObject* pO) { m_pNextSel = pO; }
	CCadObject* GetPrevSel(void) { return m_pPrevSel; }
	void SetPrevSel(CCadObject* pO) { m_pPrevSel = pO; }
	//------------------------------------------------------
	// Manage list of child drawing objects
	//------------------------------------------------------
	CCadObject* GetChildrenHead() { return m_pChildHead; }
	void SetChildrenHead(CCadObject* pObj) { m_pChildHead = pObj; }
	CCadObject* GetChildrenTail() { return m_pChildTail; }
	void SetChildrenTail(CCadObject* pObj) { m_pChildTail = pObj; }
	//-- For the Children
	void AddObjectAtChildHead(CCadObject* pObj);
	void AddObjectAtChildTail(CCadObject* pObj);
	void RemoveChildObject(CCadObject* pObj);
	CCadObject* DeleteChildObject(CCadObject* pObj) {
		CCadObject* pObjNext;

		pObjNext = pObj->GetNext();
		Destroy(pObj);
		delete pObj;
		return pObjNext;
	}
	CCadObject* FindChildObject(ObjectType Type, SubType SubType, UINT SubSubType = 0);
	//-------------------------------------------------------------
	//				List of Dependent Children ------------
	//-------------------------------------------------------------
	CCadObject* GetDependentChildrenHead() { return m_pHeadDependentChildren; }
	void SetDependentChildrenHead(CCadObject* pObj) { m_pHeadDependentChildren = pObj; }
	CCadObject* GetDependentChildrenTail() { return m_pTailDependentChildren; }
	void SetDependentChildrenTail(CCadObject* pObj) { m_pTailDependentChildren = pObj; }
	//--Links--
	CCadObject* GetNextDependentChild() { return m_pNextDependentChild; }	//get next
	void SetNextDependentChild(CCadObject* pObj) { m_pNextDependentChild = pObj; } //set next
	CCadObject* GetPrevDependentChild() { return m_pPrevDependentChild; }	//get previous
	void SetPrevDependentChild(CCadObject* pObj) { m_pPrevDependentChild = pObj; }
	//-- Manage List
	void AddDepChildObjectAtHead(CCadObject* pObj);
	void AddDepChildObjectAtTail(CCadObject* pObj);
	void RemoveDepChildObject(CCadObject* pObj);
	CCadObject* FindDepChildObject(ObjectType Type, SubType SubType, UINT SubSubType = 0);
	//-----------------------------------------------------------------
	//			List of dependent parents 
	//-----------------------------------------------------------------
	CCadObject* GetDependentParentsHead() { return m_pHeadDependentParents; }
	void SetDependentParentHead(CCadObject* pH) { m_pHeadDependentParents = pH; }
	CCadObject* GetDependentParrentsTail() { return m_pTailDependentParents; }
	void SetDependentParentTail(CCadObject* pT) { m_pTailDependentParents = pT; }
	//-- Links --
	CCadObject* GetNextDependentParent() { return m_pNextDependentParent; }
	void SetNextDependentParent(CCadObject* pN) { m_pNextDependentParent = pN; }
	CCadObject* GetPrevDependentParent() { return m_pPrevDependentParent; }
	void SetPrevDependentParent(CCadObject* pP) { m_pPrevDependentParent = pP; }
	//-- Manage List --
	void AddDepParentObjectAtHead(CCadObject* pObj);
	void AddDepParentObjectAtTail(CCadObject* pObj);
	void RemoveDepParentObject(CCadObject* pObj);
	CCadObject* FindDepParentObject(ObjectType Type, SubType SubType, UINT SubSubType = 0);
	//---------------- Origin List Links -----------------------
	void SetNextOrigin(CCadObject* pObj) { m_pNextOrigin = pObj; }
	CCadObject* GetNextOrigin() { return m_pNextOrigin; }
	void SetPrevOrigin(CCadObject* pObj) { m_pPrevOrigin = pObj; }
	CCadObject* GetPrevOrigin() { return m_pPrevOrigin; }
	//---------------------------------------------------
	// Clip Board Linked List Methods
	//--------------------------------------------------
	CCadObject* GetNextClipBoard() { return m_pNextClipBoard; }
	void SetNextClipBoard(CCadObject* pObj) { m_pNextClipBoard = pObj; }
	CCadObject* GetPrevClipBoard() { return m_pPrevClipBoard; }
	void SetPrevClipBoard(CCadObject* pObj) { m_pPrevClipBoard = pObj; }
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
