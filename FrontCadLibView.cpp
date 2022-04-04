// FrontCadLibView.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "JamCadDefines.h"
#include "JamCad.h"
#include "FrontCadLibView.h"


// CFrontCadLibView

IMPLEMENT_DYNCREATE(CFrontCadLibView, CBaseTreeView)

CFrontCadLibView::CFrontCadLibView()
{

}

CFrontCadLibView::~CFrontCadLibView()
{
}

BEGIN_MESSAGE_MAP(CFrontCadLibView, CBaseTreeView)
END_MESSAGE_MAP()


// CFrontCadLibView diagnostics

#ifdef _DEBUG
void CFrontCadLibView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CFrontCadLibView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFrontCadLibView message handlers
