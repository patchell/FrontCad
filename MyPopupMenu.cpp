// MyPopupMenu.cpp : implementation file
// 
//

#include "pch.h"


// CMyPopupMenu

IMPLEMENT_DYNAMIC(CMyPopupMenu, CWnd)

CMyPopupMenu::CMyPopupMenu()
{
	m_ppCadObjects = 0;
	m_HighlightedItem = -1;
	m_colorBkGnd = RGB(0, 0, 0);
	m_colorText = RGB(255, 255, 255);
	m_colorBorder = RGB(255, 255, 255);
	m_colorHighLight = RGB(0, 0, 128);
	m_NumOfItems = 0;
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CMyPopupMenu::~CMyPopupMenu()
{
}

BOOL CMyPopupMenu::Create(
	CWnd* pParent,	//parent window
	CPoint pointLocation, //location to make window
	CCadObject** ppObjList, //List of objects to select from
	int n,		//number of objects
	COLORREF BkGnd,
	COLORREF Text,
	COLORREF Border,
	COLORREF Highlight
)
{
	CString csClassName;
	CDC* pDC;
	CFont* pOldFont;
	int i;
	CSize szTextSize;
	int X, Y, Ytotal;;
	CSize rectsizeParent;
	CRect rectParent;

	m_NumOfItems = n;
	m_colorBkGnd = BkGnd;
	m_colorText = Text;
	m_colorBorder = Border;
	m_colorHighLight = Highlight;
	pParent->GetClientRect(&rectParent);
	rectsizeParent = rectParent.Size();
	m_ppCadObjects = ppObjList;
	csClassName = GETAPP.GetPopupMenuClassName();
	pDC = pParent->GetDC();
	//-----------------------------------
	// Create the font for the Menu
	//-----------------------------------
	m_Font.CreateFontW(
		20,
		0,
		0,
		0,
		FW_DEMIBOLD,
		0, 0, 0,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,
		PROOF_QUALITY,
		DEFAULT_PITCH,
		_T("Arial")
	);
	//---------------------------------
	// Figure out how big to make the
	// Window
	//---------------------------------
	X = 0;
	Y = 0;
	Ytotal = 0;
	pOldFont = pDC->SelectObject(&m_Font);

	for (i = 0; i < n; ++i)
	{
		szTextSize = pDC->GetTextExtent(ppObjList[0]->GetTypeString());
		if (szTextSize.cx > X)
		{
			X = szTextSize.cx;
		}
		if ((szTextSize.cy) + 2 > Y)
			Y = szTextSize.cy + 2;
	}
	Ytotal = Y * n;
	if (pointLocation.y + Y > rectsizeParent.cy)
	{
		pointLocation.y -= rectsizeParent.cy - Y;
	}
	m_szCell = CSize(X, Y);
	pDC->SelectObject(pOldFont);
	pParent->ReleaseDC(pDC);
	return CWnd::Create(
		csClassName,
		NULL,
		WS_BORDER | WS_CHILD,
		CRect(pointLocation, pointLocation + CSize(X,Ytotal)),
		pParent,
		5000
	);
}


BEGIN_MESSAGE_MAP(CMyPopupMenu, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CMyPopupMenu message handlers




void CMyPopupMenu::OnMouseMove(UINT nFlags, CPoint pointMouse)
{
	//------------------------------------------------
	// OnMouseMove
	// Here is where we fiture out which menu item is
	// being hovered over, and send a message back to
	// the view so that the drawing element can be
	// highligted.  Also, we highlite the slot as
	//------------------------------------------------
	if (m_HighlightedItem >= 0)
		m_ppCadObjects[m_HighlightedItem]->SetSelected(FALSE);
	m_HighlightedItem = pointMouse.y / m_szCell.cy;
	m_ppCadObjects[m_HighlightedItem]->SetSelected(TRUE);
	GETVIEW->Invalidate();
	CWnd::OnMouseMove(nFlags, pointMouse);
}


void CMyPopupMenu::OnLButtonDown(UINT nFlags, CPoint pointMouse)
{
	CWnd::OnLButtonDown(nFlags, pointMouse);
}


void CMyPopupMenu::OnLButtonUp(UINT nFlags, CPoint pointMouse)
{
	m_ppCadObjects[m_HighlightedItem]->SetSelected(FALSE);
	m_HighlightedItem = pointMouse.y / m_szCell.cy;
	GetParent()->PostMessageW(
		UINT(WindowsMsg::WM_PU_MENU_SELECTED_INDEX), 
		m_HighlightedItem, 
		LPARAM(m_ppCadObjects[m_HighlightedItem])
	);
	PostMessage(WM_CLOSE, 0, 0);
	CWnd::OnLButtonUp(nFlags, pointMouse);
}


void CMyPopupMenu::OnPaint()
{
	int i;
	int X, Y;
	CFont* pOldFont;
	CRect rectClient;
	COLORREF colorOldBkGnd;
	COLORREF colorOldText;
	CPaintDC dc(this); // device context for painting
	CPen* pOldPen, penLine;
	CBrush* pOldBrush, brushNull;

	GetClientRect(&rectClient);
	X = rectClient.left + 1;
	pOldFont = dc.SelectObject(&m_Font);
	colorOldBkGnd = dc.SetBkColor(m_colorBkGnd);
	colorOldText = dc.SetTextColor(m_colorText);
	for (i = 0; i < m_NumOfItems; ++i)
	{
		Y = i * m_szCell.cy + 1;
		if (m_HighlightedItem >= 0 && m_HighlightedItem == i)
		{
			dc.SetBkColor(m_colorHighLight);
			dc.TextOutW(X, Y, m_ppCadObjects[i]->GetTypeString());
		}
		else
		{
			dc.SetBkColor(m_colorBkGnd);
			dc.TextOutW(X, Y, m_ppCadObjects[i]->GetTypeString());
		}
	}
	penLine.CreatePen(PS_SOLID, 1, m_colorBorder);
	brushNull.CreateStockObject(NULL_BRUSH);
	pOldPen = dc.SelectObject(&penLine);
	pOldBrush = dc.SelectObject(&brushNull);
	dc.Rectangle(&rectClient);
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
	dc.SetTextColor(colorOldText);
	dc.SetBkColor(colorOldBkGnd);
	dc.SelectObject(pOldFont);
}


BOOL CMyPopupMenu::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
