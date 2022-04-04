#pragma once

#include "BaseTreeView.h"

// CFrontCadLibView view

class CFrontCadLibView : public CBaseTreeView
{
	DECLARE_DYNCREATE(CFrontCadLibView)

protected:
	CFrontCadLibView();           // protected constructor used by dynamic creation
	virtual ~CFrontCadLibView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


