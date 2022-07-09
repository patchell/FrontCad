// DlgObjectEnable.cpp : implementation file
//

#include "pch.h"


// CDlgObjectEnable dialog

IMPLEMENT_DYNAMIC(CDlgObjectEnable, CDialogEx)

CDlgObjectEnable::CDlgObjectEnable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RENDER_ENABLE, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CDlgObjectEnable::~CDlgObjectEnable()
{
}

void CDlgObjectEnable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ARCCENTEREDENABLE, m_Check_ArcCenteredEnable);
	DDX_Control(pDX, IDC_CHECK_ARCENABLE, m_Check_ArcEnable);
	DDX_Control(pDX, IDC_CHECK_ARROWENABLE, m_Check_ArrowEnable);
	DDX_Control(pDX, IDC_CHECK_DIMENSIONENABLE, m_Check_DimensionEnable);
	DDX_Control(pDX, IDC_CHECK_ELLISPEENABLE, m_Check_EllipseEnable);
	DDX_Control(pDX, IDC_CHECK_FILLEDPOLYENABLE, m_Check_FillPolygonEnable);
	DDX_Control(pDX, IDC_CHECK_LINEENABLE, m_Check_LineEnable);
	DDX_Control(pDX, IDC_CHECK_ONEFLATENABLE, m_Check_OneFlatHoleEnable);
	DDX_Control(pDX, IDC_CHECK_ORIGINENABLE, m_Check_OriginEnable);
	DDX_Control(pDX, IDC_CHECK_POLYGONENABLE, m_Check_PolygonEnable);
	DDX_Control(pDX, IDC_CHECK_RECTENABLE, m_Check_RectEnable);
	DDX_Control(pDX, IDC_CHECK_RECTHOLEENABLE, m_Check_RectHoleEnable);
	DDX_Control(pDX, IDC_CHECK_ROUNDEDRECTENABLE, m_CheckRoundedRectEnable);
	DDX_Control(pDX, IDC_CHECK_ROUNDHOLEENABLE, m_Check_RoundHole);
	DDX_Control(pDX, IDC_CHECK_TEXTENABLED, m_Check_TextEnable);
	DDX_Control(pDX, IDC_CHECK_TWOFLATENABLE, m_Check_TwoFlatRoundHoleEnable);
}


BEGIN_MESSAGE_MAP(CDlgObjectEnable, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ARCCENTEREDENABLE, &CDlgObjectEnable::OnClickedCheckArccenteredenable)
	ON_BN_CLICKED(IDC_CHECK_ARCENABLE, &CDlgObjectEnable::OnClickedCheckArcenable)
	ON_BN_CLICKED(IDC_CHECK_ARROWENABLE, &CDlgObjectEnable::OnClickedCheckArrowenable)
	ON_BN_CLICKED(IDC_CHECK_DIMENSIONENABLE, &CDlgObjectEnable::OnClickedCheckDimensionenable)
	ON_BN_CLICKED(IDC_CHECK_ELLISPEENABLE, &CDlgObjectEnable::OnClickedCheckEllispeenable)
	ON_BN_CLICKED(IDC_CHECK_FILLEDPOLYENABLE, &CDlgObjectEnable::OnClickedCheckFilledpolyenable)
	ON_BN_CLICKED(IDC_CHECK_LINEENABLE, &CDlgObjectEnable::OnClickedCheckLineenable)
	ON_BN_CLICKED(IDC_CHECK_ONEFLATENABLE, &CDlgObjectEnable::OnClickedCheckOneflatenable)
	ON_BN_CLICKED(IDC_CHECK_ORIGINENABLE, &CDlgObjectEnable::OnClickedCheckOriginenable)
	ON_BN_CLICKED(IDC_CHECK_POLYGONENABLE, &CDlgObjectEnable::OnClickedCheckPolygonenable)
	ON_BN_CLICKED(IDC_CHECK_RECTENABLE, &CDlgObjectEnable::OnClickedCheckRectenable)
	ON_BN_CLICKED(IDC_CHECK_RECTHOLEENABLE, &CDlgObjectEnable::OnClickedCheckRectholeenable)
	ON_BN_CLICKED(IDC_CHECK_ROUNDEDRECTENABLE, &CDlgObjectEnable::OnClickedCheckRoundedrectenable)
	ON_BN_CLICKED(IDC_CHECK_ROUNDHOLEENABLE, &CDlgObjectEnable::OnClickedCheckRoundholeenable)
	ON_BN_CLICKED(IDC_CHECK_TEXTENABLED, &CDlgObjectEnable::OnClickedCheckTextenabled)
	ON_BN_CLICKED(IDC_CHECK_TWOFLATENABLE, &CDlgObjectEnable::OnClickedCheckTwoflatenable)
END_MESSAGE_MAP()


// CDlgObjectEnable message handlers


void CDlgObjectEnable::OnClickedCheckArccenteredenable()
{
	CCadArcCent::RenderEnable(m_Check_ArcCenteredEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckArcenable()
{
	CCadArc::RenderEnable(m_Check_ArcEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckArrowenable()
{
	CCadArrow::RenderEnable(m_Check_ArrowEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckDimensionenable()
{
	CCadDimension::RenderEnable(m_Check_DimensionEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckEllispeenable()
{
	CCadElispe::RenderEnable(m_Check_EllipseEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckFilledpolyenable()
{
	CCadPolygon::RenderEnable(m_Check_FillPolygonEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckLineenable()
{
	CCadLine::RenderEnable(m_Check_LineEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckOneflatenable()
{
	CCadHoleRnd1Flat::RenderEnable(m_Check_OneFlatHoleEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckOriginenable()
{
	CCadOrigin::RenderEnable(m_Check_OriginEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckPolygonenable()
{
	CCadPolygon::RenderEnable(m_Check_PolygonEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckRectenable()
{
	CCadRect::RenderEnable(m_Check_RectEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckRectholeenable()
{
	CCadHoleRect::RenderEnable(m_Check_RectHoleEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckRoundedrectenable()
{
	CCadRndRect::RenderEnable(m_CheckRoundedRectEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckRoundholeenable()
{
	CCadHoleRound::RenderEnable(m_Check_RoundHole.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckTextenabled()
{
	CCadText::RenderEnable(m_Check_TextEnable.GetCheck());
}


void CDlgObjectEnable::OnClickedCheckTwoflatenable()
{
	CCadHoleRnd2Flat::RenderEnable(m_Check_TwoFlatRoundHoleEnable.GetCheck());
}



BOOL CDlgObjectEnable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Check_ArcEnable.SetCheck(CCadArc::IsRenderEnabled());
	m_Check_ArcCenteredEnable.SetCheck(CCadArcCent::IsRenderEnabled());
	m_Check_ArrowEnable.SetCheck(CCadArrow::IsRenderEnabled());
	m_Check_DimensionEnable.SetCheck(CCadDimension::IsRenderEnabled());
	m_Check_EllipseEnable.SetCheck(CCadElispe::IsRenderEnabled());
	m_Check_LineEnable.SetCheck(CCadLine::IsRenderEnabled());
	m_Check_OneFlatHoleEnable.SetCheck(CCadHoleRnd1Flat::IsRenderEnabled());
	m_Check_OriginEnable.SetCheck(CCadOrigin::IsRenderEnabled());
	m_Check_PolygonEnable.SetCheck(CCadPolygon::IsRenderEnabled());
	m_Check_RectEnable.SetCheck(CCadRect::IsRenderEnabled());
	m_Check_RectHoleEnable.SetCheck(CCadHoleRect::IsRenderEnabled());
	m_CheckRoundedRectEnable.SetCheck(CCadRndRect::IsRenderEnabled());
	m_Check_RoundHole.SetCheck(CCadHoleRound::IsRenderEnabled());
	m_Check_TextEnable.SetCheck(CCadText::IsRenderEnabled());
	m_Check_TwoFlatRoundHoleEnable.SetCheck(CCadHoleRnd2Flat::IsRenderEnabled());


	return TRUE;  
}
