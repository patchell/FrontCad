// FrontCadLibDoc.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "JamCadDefines.h"
#include "JamCad.h"
#include "FrontCadLibDoc.h"
#include "FrontCadLibDwg.h"
#include "DlgNewName.h"

// CFrontCadLibDoc

IMPLEMENT_DYNCREATE(CFrontCadLibDoc, CBaseTreeDoc)

CFrontCadLibDoc::CFrontCadLibDoc()
{
}

BOOL CFrontCadLibDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	CDlgNewName Dlg;
	m_pDrawing = new CFrontCadLibDwg;
	m_pDrawing->Create();
	Dlg.SetPropertyField(m_pDrawing->GetPropertiesPointer());
	Dlg.SetPropertyNameIndex(PF_NAME_FRONTCADLIBRARY);
	Dlg.DoModal();
	SetTitle(m_pDrawing->GetProperties().FindProperty(PropFieldNames[PF_NAME_FRONTCADLIBRARY])->GetFieldValue());
	return TRUE;
}

CFrontCadLibDoc::~CFrontCadLibDoc()
{
}


BEGIN_MESSAGE_MAP(CFrontCadLibDoc, CBaseTreeDoc)
END_MESSAGE_MAP()


// CFrontCadLibDoc diagnostics

#ifdef _DEBUG
void CFrontCadLibDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CFrontCadLibDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CFrontCadLibDoc serialization

void CFrontCadLibDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CFrontCadLibDoc commands

char *CFrontCadLibDoc::GetDocName(void)
{
	return GetDrawing()
		->GetProperties()
		.FindProperty(PropFieldNames[PF_NAME_FRONTCADLIBRARY])
		->GetFieldValue();
}