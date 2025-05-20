#include "pch.h"


CEdaObject::CEdaObject()
{
}

CEdaObject::~CEdaObject()
{
}

BOOL CEdaObject::Create(CCadObject* pParent, SubTypes type)
{
	return 0;
}

BOOL CEdaObject::AddPropertyInstance(SEdaObjProperty* pEOP)
{
	return 0;
}

BOOL CEdaObject::RemovePropertyInstance(int Instance)
{
	return 0;
}

//-------------------------------------------
// CEdaObject::SEdaObjProperty class
//-------------------------------------------

CEdaObject::SEdaObjProperty::SEdaObjProperty()
{
}

CEdaObject::SEdaObjProperty::~SEdaObjProperty()
{
}

BOOL CEdaObject::SEdaObjProperty::Create()
{
	return 0;
}

BOOL CEdaObject::SEdaObjProperty::AddField(SPropertyField* pField)
{
	return 0;
}

BOOL CEdaObject::SEdaObjProperty::RemoveField(CString& csFieldName)
{
	return 0;
}
