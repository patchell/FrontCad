// StaticPreview.cpp : implementation file
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticPreview

CStaticPreview::CStaticPreview()
{
	m_pObj = 0;
	m_X = 0;
	m_Y = 1;
	m_Scale = CScale(0.3, 0.3);
	m_OffsetMode = StaticPreivewMode::STATICPREVIEW_OFFSET_CENTER;
}

CStaticPreview::~CStaticPreview()
{
}


BEGIN_MESSAGE_MAP(CStaticPreview, CStatic)
	//{{AFX_MSG_MAP(CStaticPreview)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticPreview message handlers

CScale CStaticPreview::CalculateScale(
	CDoubleSize& ObjSize
)
{
	//---------------------------------
	// CalculateScale
	//	Calculate the Scale that is
	// needed to fill the client 
	// rectangle with the object
	// in question
	// 
	// parameters:
	//	ObjectSize..Size of the object in inches
	// 
	// Return Value
	// Returns the Scale in pixels per inch
	// to do the job.
	//---------------------------------
	double ScaleX;
	double ScaleY;
	double Scale;
	CRect rectClient;

	GetClientRect(&rectClient);
	ScaleX = double(rectClient.Width()) / ObjSize.dCX;
	ScaleY = double(rectClient.Height()) / ObjSize.dCY;
	//-----------------------------------
	// We need to pick only one, but
	// we must choose wisely
	//-----------------------------------
	if (ScaleX > ScaleY)
		Scale = ScaleY;
	else
		Scale = ScaleX;
	return CScale(Scale,Scale);
}

void CStaticPreview::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectClient;
	CBrush br(RGB(255,255,255));
	CPen pen, * oldPen;
	CCadObject* pO;

	GetClientRect(&rectClient);
	dc.FillRect(&rectClient,&br);
	pO = m_pObj;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 128, 240));
	oldPen = dc.SelectObject(&pen);
	if (m_Y)	//Y axis enabled
	{
		dc.MoveTo(m_Offset.cx, 0);
		dc.LineTo(m_Offset.cx, rectClient.BottomRight().y);
	}
	if (m_X)	//X axis enabled
	{
		dc.MoveTo(0, m_Offset.cy);
		dc.LineTo(rectClient.BottomRight().x, m_Offset.cy);
	}
	dc.SelectObject(oldPen);
	MODE mode;
	DOUBLEPOINT ulhc;
	while(pO)
	{
		pO->Draw(&dc, mode, ulhc, m_Scale);
		pO = pO->GetNext();
	}
}

void CStaticPreview::Init()
{
	CRect rectClient;

	GetClientRect(&rectClient);
	m_Offset = rectClient.CenterPoint();
}


void CStaticPreview::SetViewOffset(CPoint Point)
{
	m_Offset = Point;
}


BOOL CStaticPreview::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL rV = CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
	if (rV) Init();
	return rV;
}


BOOL CStaticPreview::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL rV = CStatic::PreCreateWindow(cs);
	if (rV) Init();
	return rV;
}

void CStaticPreview::AutoScale()
{
	//-------------------------------------------------
	// AutoScale
	// This Method uses the rectangle that surronds
	// the object that is desired to show and calculates
	// an optimized offset and scale
	//
	//---------------------------------------------------
	CRect RectClient;
	CDoubleSize ObjectSize;
	DOUBLEPOINT pointScratch;
	CPoint ClientCenterPoint;
	DOUBLEPOINT dblPtZero;

	GetClientRect(&RectClient);
	ClientCenterPoint = RectClient.CenterPoint();
	ObjectSize = m_pObj->GetSize();
	m_Scale = CalculateScale(ObjectSize);
	//-----------------------------------
	// Now we need to calculate the
	// offset value so that the drawing
	// can start inside of the rectangle
	//-----------------------------------
//	m_Offset = rect.GetCenter(pointScratch).ToPixelPoint(dblPtZero, m_Scale) - ClientCenterPoint;
}
