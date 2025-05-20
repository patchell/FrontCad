#pragma once

class CEdaObject :public CCadObject
{
	struct SPropertyField {
		SPropertyField* m_pNext;
		SPropertyField* m_pPrev;
		CString m_csName;
		CString m_csValue;
	};
	struct SEdaObjProperty {
		//--------------------------------
		// Everybody gets one of these
		// properties
		//--------------------------------
		int m_Instance;
		SPropertyField m_Reference;
		SPropertyField m_Value;
		SPropertyField m_FootPrint;
		SPropertyField m_DataSheet;
		SPropertyField m_Description;
		SPropertyField* m_pHead;	//head of properties
		SPropertyField* m_pTail;
		SEdaObjProperty* m_pNext;
		SEdaObjProperty* m_pPrev;
		//-----------------------------------
		// Constructor/Destructor
		//-----------------------------------
		SEdaObjProperty();
		virtual ~SEdaObjProperty();
		BOOL Create();
		BOOL AddField(SPropertyField* pField);
		BOOL RemoveField(CString& csFieldName);
		SEdaObjProperty* GetNext() const { return m_pNext; }
		void SetNext(SEdaObjProperty* pN) { m_pNext = pN; }
		SEdaObjProperty* GetPrev() const { return m_pPrev; }
		void SetPrev(SEdaObjProperty* pP) { m_pPrev = pP; }
	};
	SEdaObjProperty* m_pHead;	// head of property Instances
	SEdaObjProperty* m_pTail;	// Tail of property Instances
public:
	CEdaObject();
	virtual ~CEdaObject();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	BOOL AddPropertyInstance(SEdaObjProperty* pEOP);
	BOOL RemovePropertyInstance(int Instance);
};

