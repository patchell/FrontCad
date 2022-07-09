// StaticBitmap.cpp : implementation file
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticBitmap

CStaticBitmap::CStaticBitmap()
{
	m_Color = RGB(0,0,0);
	m_bDirty = FALSE;
}

CStaticBitmap::~CStaticBitmap()
{
}


BEGIN_MESSAGE_MAP(CStaticBitmap, CStatic)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(STN_CLICKED, &CStaticBitmap::OnStnClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticBitmap message handlers

void CStaticBitmap::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);
	CBrush br;
	br.CreateSolidBrush(m_Color);
	dc.FillRect(&rect,&br);
}

void CStaticBitmap::OnStnClicked()
{
	CColorDialog Dlg;
	int Id;

	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		m_Color = Dlg.GetColor();
		if (!m_bDirty)
		{
			m_bDirty = TRUE;
			if (GetParent())
				GetParent()->PostMessageW(UINT(WindowsMsg::WM_DLG_CONTROL_DIRTY));
			Invalidate();
		}

	}
}
