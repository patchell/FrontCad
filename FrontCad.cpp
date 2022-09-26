
// FrontCad.cpp : Defines the class behaviors for the application.
//

#include "pch.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString CFrontCadApp::csDrawState[16] = {
	_T("SET_ATTRIBUTES"),
	_T("WAITFORMOUSE_DOWN"),
	_T("MOVE"),
	_T("PLACE"),
	_T("PLACE_AUTO"),
	_T("ARCSTART"),
	_T("ARCEND"),
	_T("GETREFERENCE"),
	_T("DRAG"),
	_T("SELECT_NOTHING"),	//nothing is selected
	_T("SELECT_MOVE"),
	_T("SELECT_COPY"),
	_T("VERTEX_GRABBED"),
	_T("ROTATE"),
	_T("SECOND_POINT"),
	_T("EXTENSION_LINES")
};

UINT CFrontCadApp::Id = 0;

// CFrontCadApp

BEGIN_MESSAGE_MAP(CFrontCadApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CFrontCadApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CFrontCadApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CFrontCadApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CFrontCadApp construction

CFrontCadApp::CFrontCadApp() noexcept
{
	Pi = atan(1.0) * 4.0;
	SetAppID(_T("FrontCad.AppID.NoVersion"));
	m_pMainFrame = 0;
	pConsol = 0;
}

// The one and only CFrontCadApp object

CFrontCadApp theApp;


// CFrontCadApp initialization

BOOL CFrontCadApp::InitInstance()
{
	AfxOleInit();
	//-------------------------------------------------------------
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	//-------------------------------------------------------------
	INITCOMMONCONTROLSEX InitCtrls;
	memset(&InitCtrls, 0, sizeof(INITCOMMONCONTROLSEX));
	InitCtrls.dwSize = sizeof(InitCtrls);
	//-------------------------------------------------------------
	// Set this to include all the common control classes you want to use
	// in your application.
	//-------------------------------------------------------------
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	LoadSettings();
	EnableTaskbarInteraction(FALSE);

	//-------------------------------------------------------------
	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();
	//-------------------------------------------------------------
	//---------------------------------------
	// Open up a "DOS" window to print debug
	// Information to.
	//---------------------------------------
	AllocConsole();
	freopen_s(&pConsol, "CONOUT$", "w", stdout);
	printf("Ready\n");
	//-------------------------------------------------------------
	// Initialize Windows Classes
	//-------------------------------------------------------------
	try
	{
		m_csPopUpMenuClass = AfxRegisterWndClass(
			CS_VREDRAW | CS_HREDRAW | CS_SAVEBITS | CS_OWNDC,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)::GetStockObject(BLACK_BRUSH),
			::LoadIcon(NULL, IDI_APPLICATION));
	}
	catch (CResourceException* pEx)
	{
		AfxMessageBox(_T("Couldn't register class! (Already registered?)"));
		pEx->Delete();
	}
	//-------------------------------------------------------------
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//-------------------------------------------------------------
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	//-------------------------------------------------------------
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	//-------------------------------------------------------------
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_FrontCadTYPE,
		RUNTIME_CLASS(CFrontCadDoc),
		RUNTIME_CLASS(CFrontCadChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFrontCadView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	//-------------------------------------------------------------
	// create main MDI Frame window
	//-------------------------------------------------------------
	CMainFrame* pMainFrame = new CMainFrame;
	printf("Create Main Frame\n");
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	m_pMainFrame = pMainFrame;
	pMainFrame->ShowWindow(m_nCmdShow);

	//-------------------------------------------------------------
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	//-------------------------------------------------------------
	m_pMainWnd->DragAcceptFiles();

	//-------------------------------------------------------------
	// Parse command line for standard shell commands, DDE, file open
	//-------------------------------------------------------------
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//-------------------------------------------------------------
	// Enable DDE Execute open
	//-------------------------------------------------------------
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	//-------------------------------------------------------------
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	//-------------------------------------------------------------
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	//-------------------------------------------------------------
	// The main window has been initialized, so show and update it
	//-------------------------------------------------------------
	pMainFrame->UpdateWindow();
	pMainFrame->PostMessageW(UINT(WindowsMsg::WM_MAINFRAME_FINISHUPDATE));
	return TRUE;
}

int CFrontCadApp::ExitInstance()
{
	fclose(pConsol);
	return CWinApp::ExitInstance();
}

//-----------------------------------
// CFrontCadApp message handlers
//-----------------------------------

//-------------------------------------
// CAboutDlg dialog used for App About
//-------------------------------------
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

//-----------------------------------------------

CWnd* CFrontCadApp::GetCurrentView()
{
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)GetMainWnd();
	// Get the active MDI child window.
	CMDIChildWnd* pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
	// Get the active view attached to the active MDI child window.
	CFrontCadView* pView = (CFrontCadView*)pChild->GetActiveView();	
	return pView;
}

//----------------------------------
// App command to run the dialog
//----------------------------------

void CFrontCadApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

//--------------------------------
// General Polygon Methods
//-------------------------------


void CFrontCadApp::SetClipBoardRef(DOUBLEPOINT p)
{
	m_ClipBoard.SetRef(p);
}

//----------------------------------------------
// Methods to manage Object Attributes
//----------------------------------------------

void CFrontCadApp::LoadSettings()
{
	SArcAttributes::LoadSettings(&m_ArcAttributes);
	SArcCenterAttributes::LoadSettings(&m_ArcCenterAttributes);
	SArcAngleAttributes::LoadSettings(&m_ArcAngleAttributes);
	SArrowAttributes::LoadSettings(&m_ArrowAttributes);
	SCadDimAttributes::LoadSettings(&m_DimensionAttributes);
	SEllipseAttributes::LoadSettings(&m_EllipseAttributes);
	SLineAttributes::LoadSettings(&m_LineAttributes);
	SOriginAttributes::LoadSettings(&m_OriginAttributes);
	SPolyAttributes::LoadSettings(&m_PolygonAttributes);
	SRectAttributes::LoadSettings(&m_RectangleAttributes);
	SRoundedRectAttributes::LoadSettings(&m_RoundedRectangleAttributes);
	STextAttributes::LoadSettings(&m_TextAttributes);
	SRectHoleAttributes::LoadSettings(&m_RectangularHoleAtrributes);
	SRoundHoleAttributes::LoadSettings(&m_RoundHoleAttributes);
	SRndHole1FlatAttributes::LoadSettings(&m_RoundHole1FlatAttributes);
	SRndHole2FlatAttributes::LoadSettings(&m_RoundHole2FlatsAttributes);
	SGridAttributes::LoadSettings(&m_GridAttributes);
	SRullerAttributes::LoadSettings(&m_RulerAttributes);
	SPointAttributes::LoadSettings(&m_PointAttributes);
}

void CFrontCadApp::SaveSettings()
{
	SArcAttributes::SaveSettings(&m_ArcAttributes);
	SArcCenterAttributes::SaveSettings(&m_ArcCenterAttributes);
	SArcAngleAttributes::SaveSettings(&m_ArcAngleAttributes);
	SArrowAttributes::SaveSettings(&m_ArrowAttributes);
	SCadDimAttributes::SaveSettings(&m_DimensionAttributes);
	SEllipseAttributes::SaveSettings(&m_EllipseAttributes);
	SLineAttributes::SaveSettings(&m_LineAttributes);
	SOriginAttributes::SaveSettings(&m_OriginAttributes);
	SPolyAttributes::SaveSettings(&m_PolygonAttributes);
	SRectAttributes::SaveSettings(&m_RectangleAttributes);
	SRoundedRectAttributes::SaveSettings(&m_RoundedRectangleAttributes);
	STextAttributes::SaveSettings(&m_TextAttributes);
	SRectHoleAttributes::SaveSettings(&m_RectangularHoleAtrributes);
	SRoundHoleAttributes::SaveSettings(&m_RoundHoleAttributes);
	SRndHole1FlatAttributes::SaveSettings(&m_RoundHole1FlatAttributes);
	SRndHole2FlatAttributes::SaveSettings(&m_RoundHole2FlatsAttributes);
	SGridAttributes::SaveSettings(&m_GridAttributes);
	SRullerAttributes::SaveSettings(&m_RulerAttributes);
	SPointAttributes::SaveSettings(&m_PointAttributes);
}

void* CFrontCadApp::GetObjectDefaultAttributes(ObjectType ObjectType)
{
	void* pAttributes;

	switch (ObjectType)
	{
		case ObjectType::ARC:
			pAttributes = (void*)&m_ArcAttributes;
			break;
		case ObjectType::ARCCENTERED:
			pAttributes = (void*)&m_ArcCenterAttributes;
			break;
		case ObjectType::ARCANGLE:
			pAttributes = (void*)&m_ArcAngleAttributes;
			break;
		case ObjectType::ARROW:
			pAttributes = (void*)&m_ArrowAttributes;
			break;
		case ObjectType::BITMAP:
			pAttributes = (void*)0;
			break;
		case ObjectType::DIMENSION:
			pAttributes = (void*)&m_DimensionAttributes;
			break;
		case ObjectType::ELIPSE:
			pAttributes = (void*)&m_EllipseAttributes;
			break;
		case ObjectType::HOLE_RECTANGLE:
			pAttributes = (void*)&m_RectangularHoleAtrributes;
			break;
		case ObjectType::HOLE_RND1FLAT:
			pAttributes = (void*)&m_RoundHole1FlatAttributes;
			break;
		case ObjectType::HOLE_RND2FLAT:
			pAttributes = (void*)&m_RoundHole2FlatsAttributes;
			break;
		case ObjectType::HOLE_ROUND:
			pAttributes = (void*)&m_RoundHoleAttributes;
			break;
		case ObjectType::LIBCOMP:
			pAttributes = (void*)0;
			break;
		case ObjectType::LINE:			//drawing case ObjectType::OBJECTs
			pAttributes = (void*)&m_LineAttributes;
			break;
		case ObjectType::ORIGIN:
			pAttributes = (void*)&m_OriginAttributes;
			break;
		case ObjectType::POLYGON:
			pAttributes = (void*)&m_PolygonAttributes;
			break;
		case ObjectType::RECT:
			pAttributes = (void*)&m_RectangleAttributes;
			break;
		case ObjectType::ROUNDEDRECT:
			pAttributes = (void*)&m_RoundedRectangleAttributes;
			break;
		case ObjectType::TEXT:
			pAttributes = (void*)&m_TextAttributes;
			break;
		default:
			pAttributes = 0;
			break;
	}
	return pAttributes;
}

//----------------------------------------------
// String Methods
//----------------------------------------------

char * CFrontCadApp::MkIndentString(char *pDest, int count, int c)
{
	//-----------------------------------------
	// MkIndentString
	//	This method is used to create an Indent
	// that is generally used in saving files.
	//	parameters:
	// pDest......Destination of indent string
	// count......How much of an indent
	// c..........character to use in string
	//	returns:
	// The destination string pDest
	//-----------------------------------------

	int i;

	for (i = 0; i < count; ++i)
		pDest[i] = c;
	pDest[i] = 0;
	return pDest;
}

char * CFrontCadApp::ConvertCStringToChar(char* cpDest, CString& csSource)
{

	const size_t newsizew = (csSource.GetLength() + 1) * 2;
	size_t convertedCharsw = 0;
	wcstombs_s(&convertedCharsw, cpDest, newsizew, csSource, _TRUNCATE);
	return cpDest;
}

//-------------------------------------
// Point Operations
// ------------------------------------

BOOL CFrontCadApp::IsOnSegment(DOUBLEPOINT p, DOUBLEPOINT q, DOUBLEPOINT r)
{
	//----------------------------------------
	// IsOnSegment
	// Given three points that are colinear,
	// check to see if q in on segment p,r
	//----------------------------------------
	BOOL rV = FALSE;
		if (q.dX <= max(p.dX, r.dX) && q.dX >= min(p.dX, r.dX) &&
			q.dY <= max(p.dY, r.dY) && q.dY >= min(p.dY, r.dY))
			rV = TRUE;
	return rV;
}

BOOL CFrontCadApp::DoLinesIntersect(
	DOUBLEPOINT p1,
	DOUBLEPOINT q1,
	DOUBLEPOINT p2,
	DOUBLEPOINT q2
)
{
	BOOL rV = FALSE;
	int o1, o2, o3, o4;
	// Find the four orientations needed for general and
	// special cases
	o1 = Orientation(p1, q1, p2);
	o2 = Orientation(p1, q1, q2);
	o3 = Orientation(p2, q2, p1);
	o4 = Orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		rV = TRUE;
	else if (o1 == 0 && IsOnSegment(p1, p2, q1))
		rV = TRUE;
	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	else if (o2 == 0 && IsOnSegment(p1, q2, q1))
		rV = TRUE;
	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	else if (o3 == 0 && IsOnSegment(p2, p1, q2))
		rV = TRUE;
	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	else if (o4 == 0 && IsOnSegment(p2, q1, q2))
		rV = TRUE;
	return rV;
}

int CFrontCadApp::Orientation(DOUBLEPOINT p, DOUBLEPOINT q, DOUBLEPOINT r)
{
	int result;
	double val;

	val = (q.dY - p.dY) * (r.dX - q.dX) -
		(q.dX - p.dX) * (r.dY - q.dY);

	if (val == 0.0)
		result = ORIENTATION_COLINEAR;  // collinear
	else if (val > 0.0)
		result = ORIENTATION_CLOCKWISE;
	else
		result = ORIENTATION_COUNTERCLOCKWISE;
	return result;
}


DOUBLEPOINT CFrontCadApp::CalcCenter(DOUBLEPOINT p1, DOUBLEPOINT p2) 
{
	DOUBLEPOINT Result;

	Result.dX = (p1.dX + p2.dX) / 2.0;
	Result.dY = (p1.dY + p2.dY) / 2.0;
	return Result;
}

DOUBLEPOINT CFrontCadApp::CalcCenter(CCadPoint* pP1, CCadPoint* pP2)
{
	DOUBLEPOINT Result;

	pP1->Print("P1");
	pP2->Print("P2");
	Result.dX = pP2->GetX() - (pP2->GetX() - pP1->GetX()) / 2.0;
	Result.dY = pP2->GetY() - (pP2->GetY() - pP1->GetY()) / 2.0;
	return Result;;
}

//-------------------------------------
// General Polygon Methods
// ------------------------------------

BOOL CFrontCadApp::DoLinesIntersect(
	DOUBLEPOINT L1P1,
	DOUBLEPOINT L1P2,
	DOUBLEPOINT L2P1,
	DOUBLEPOINT L2P2,
	DOUBLEPOINT& Intersection
)
{
	double s1_x, s1_y, s2_x, s2_y;
	double s, t;
	BOOL rV = FALSE;

	s1_x = L1P2.dX - L1P1.dX;
	s1_y = L1P2.dY - L1P1.dY;

	s2_x = L2P2.dX - L2P1.dX;
	s2_y = L2P2.dY - L2P1.dY;

	s = (-s1_y * (L1P1.dX - L2P1.dX) + s1_x * (L1P1.dY - L2P1.dY)) / (-s2_x * s1_y + s1_x * s2_y);
//		s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (L1P1.dY - L2P1.dY) - s2_y * (L1P1.dX - L2P1.dX)) / (-s2_x * s1_y + s1_x * s2_y);
//		t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		Intersection = DOUBLEPOINT(L1P1.dX + (t * s1_x), L1P1.dY + (t * s1_y));
		rV = TRUE;
	}

	return rV; 
}

DOUBLEPOINT CFrontCadApp::GetPolygonCenter(DOUBLEPOINT *pPoly, int n)
{
	double x=0.0, y = 0.0;

	for (int i = 0; i < n; i++)
	{
		x += pPoly[i].dX;
		y += pPoly[i].dY;
	}
	x /= double(n);
	y /= double(n);
	return DOUBLEPOINT(x,y);
}

int CFrontCadApp::GetNumberOfIntersections(
	DOUBLEPOINT* pPoly,		//set of points of a polygon
	int n,					//Number of points
	DOUBLEPOINT L1P1,		//endpoint of line
	DOUBLEPOINT L1P2		//The other endpoint
)
{
	//-----------------------------------------
	// GetNumberOfIntersections
	// Given a line between the points L1P1 and
	// L1P2, count how many times it crosses
	// the lines between the point of the
	// polygon.
	//-----------------------------------------
	int Result = 0;
	DOUBLEPOINT PolyPoint1, PolyPoint2;
	int i;

	PolyPoint1 = pPoly[0];
	for (i = 1; i < n; ++i)
	{
		if (i & 1)	//is i odd?
			PolyPoint2 = pPoly[i];
		else
			PolyPoint1 = pPoly[i];
		if (DoLinesIntersect(L1P1, L1P2, PolyPoint1, PolyPoint2))
			Result++;
	}
	return Result;
}

BOOL CFrontCadApp::ValidatePolygon(DOUBLEPOINT* pPoly, int n)
{
	BOOL rV = FALSE;

	return rV;
}

BOOL CFrontCadApp::GeneratePointInPolygon(DOUBLEPOINT* Poly, int n, DOUBLEPOINT& ThePoint)
{
	CCadRect rect;	
	double MaxX, MinX;
	double MaxY, MinY;
	double SizeX, SizeY;
	int LeftCount = 0;
	int RightCount = 0;
	int TopCount = 0;
	int BotCount = 0;
	BOOL Result = FALSE;
	BOOL Loop = TRUE;
	BOOL DoX = FALSE;
	BOOL DoY = FALSE;
	DOUBLEPOINT P1, P2, P3;

	ThePoint = GetPolygonCenter(Poly, n);
	if (PtEnclosedInPolygon(ThePoint, Poly, n))
	{
		Result = TRUE;
	}
	else
	{
		//----------------------------------
		// This polygon is not going to come
		// along easily, gonna take a bit of
		// work
		//----------------------------------
		GetPolyMinMax(Poly, n, MinX, MaxX, MinY, MaxY);
		SizeX = MaxX - MinX;
		SizeY = MaxY - MinY;
		//----------------------------
		// Try to guess which way is
		// the best to try
		//----------------------------
		if (ThePoint.dX < MaxX && ThePoint.dX > MinX)
		{
			DoX = TRUE;
		}
		else if (ThePoint.dY < MaxY && ThePoint.dY > MinY)
		{
			DoY = TRUE;
		}
		else if (SizeX > SizeY)
		{
			DoX = TRUE;
		}
		else
		{
			DoY = TRUE;
		}
		while (Loop)
		{
			if (DoX)
			{
				P1 = ThePoint;
				P2 = DOUBLEPOINT(P1.dX + SizeX, P1.dY);
				P3 = DOUBLEPOINT(P1.dX - SizeX, P1.dY);
				LeftCount = GetNumberOfIntersections(Poly, n, P1,P2);
				RightCount = GetNumberOfIntersections(Poly, n, P1, P3);
				SizeX /= 2.0;
				if(LeftCount > RightCount)
					ThePoint = DOUBLEPOINT(P1.dX + SizeX, P1.dY);
				else
					ThePoint = DOUBLEPOINT(P1.dX - SizeX, P1.dY);
				if (PtEnclosedInPolygon(ThePoint, Poly, n))
				{
					Loop = FALSE;
					Result = TRUE;
				}
				else if (SizeX < 0.05)
				{
					Loop = FALSE;
					Result = FALSE;
				}
			}
			else if (DoY)
			{
				P1 = ThePoint;
				P2 = DOUBLEPOINT(P1.dX, P1.dY + SizeY);
				P3 = DOUBLEPOINT(P1.dX, P1.dY - SizeY);
				BotCount = GetNumberOfIntersections(Poly, n, P1, P2);
				TopCount = GetNumberOfIntersections(Poly, n, P1, P3);
				SizeY /= 2.0;
				if (BotCount > TopCount)
					ThePoint = DOUBLEPOINT(P1.dX, P1.dY + SizeY);
				else
					ThePoint = DOUBLEPOINT(P1.dX, P1.dY - SizeY);
				if (PtEnclosedInPolygon(ThePoint, Poly, n))
				{
					Loop = FALSE;
					Result = TRUE;
				}
				else if (SizeY < 0.05)
				{
					Loop = FALSE;
					Result = FALSE;
				}
			}
		}
	}
	return Result;
}

BOOL CFrontCadApp::PtEnclosedInPolygon(
	DOUBLEPOINT ptPoint,
	DOUBLEPOINT* ptArray,
	UINT nVeticies
)
{
	//----------------------------------------*
	// PtEnclosedInPolygon
	//	This Method determines if a point
	// is enclosed within a polygon.
	//
	// parameters:
	//	ptPoint....point to test
	//	ptArray....array of points defining polygon
	//	nVeticies..number of verticies in polygon
	//	ptOffset...polygon offset
	//
	// Returns: TRUE if point inside
	//          FALSE if point is outside
	//----------------------------------------*/
	UINT   i;
	int j = nVeticies - 1;
	double MaxX = -numeric_limits<double>::infinity(), MinX = numeric_limits<double>::infinity();
	double MaxY = -numeric_limits<double>::infinity(), MinY = numeric_limits<double>::infinity();
	BOOL  Enclosed = FALSE;
	double Xintercept;

	for (i = 0; i < nVeticies; ++i)
	{
		//used to define bounding rectangle
		if (ptArray[i].dX < MinX)
			MinX = ptArray[i].dX;
		if (ptArray[i].dX > MaxX)
			MaxX = ptArray[i].dX;
		if (ptArray[i].dY < MinY)
			MinY = ptArray[i].dY;
		if (ptArray[i].dY > MaxY)
			MaxY = ptArray[i].dY;
	}
	//--------------------------------------------
	// Do a precheck agains the rectangle that
	// encloses the polygon
	//--------------------------------------------
	if (MinX < ptPoint.dX &&
		MaxX > ptPoint.dX &&
		MinY < ptPoint.dY &&
		MaxY > ptPoint.dY
		)
	{
		//----------------------------------
		// very good chance now that point
		// is in polygon, so make a
		// thorough check
		//---------------------------------
		for (i = 0; i < nVeticies; i++)
		{
			if (((ptArray[i].dY < ptPoint.dY && ptArray[j].dY >= ptPoint.dY)
				|| (ptArray[j].dY < ptPoint.dY && ptArray[i].dY >= ptPoint.dY))
				&& (ptArray[i].dX <= ptPoint.dX || ptArray[j].dX <= ptPoint.dX))
			{
				Xintercept = ptArray[i].dX + ((ptPoint.dY - ptArray[i].dY) * (ptArray[j].dX - ptArray[i].dX)) / (ptArray[j].dY - ptArray[i].dY);
				Enclosed ^= Xintercept < ptPoint.dX;
			}
			j = i;
		}
	}
	return Enclosed;
}

void CFrontCadApp::GetPolyMinMax(
	DOUBLEPOINT* pPoly, 
	int n, 
	double& MinX, 
	double& MaxX, 
	double& MinY, 
	double& MaxY
)
{
	MaxX = -numeric_limits<double>::infinity();
	MinX = numeric_limits<double>::infinity();
	MaxY = -numeric_limits<double>::infinity();
	MinY = numeric_limits<double>::infinity();

	int i;
	for (i = 0; i < n; ++i)
	{
		if (pPoly[i].dX < MinX)
			MinX = pPoly[i].dX;
		if (pPoly[i].dX > MaxX)
			MaxX = pPoly[i].dX;
		if (pPoly[i].dY < MinY)
			MinY = pPoly[i].dY;
		if (pPoly[i].dY > MaxY)
			MaxY = pPoly[i].dY;
	}
}

void CFrontCadApp::ShiftDoublePointArray(CCadPoint* pdptPoints, UINT nPoints, UINT Direction)
{
	int i;
	CCadPoint dptTemp;

	if (Direction)	//Clockwise
	{
		dptTemp = pdptPoints[0];
		for (i = 1; i < int(nPoints); ++i)
		{
			pdptPoints[i - 1] = pdptPoints[i];
		}
		pdptPoints[nPoints - 1] = dptTemp;
	}
	else        //Counter Clockwise
	{
		dptTemp = pdptPoints[nPoints - 1];
		for (i = int(nPoints) - 2; i >= 0; --i)
		{
			pdptPoints[i + 1] = pdptPoints[i];
		}
		pdptPoints[0] = dptTemp;
	}
}


CPoint* CFrontCadApp::MakeCPointPolygonFromDOUBLEPOINTS(
	CPoint* dest,
	DOUBLEPOINT* src,
	int n,
	DOUBLEPOINT& ULHC,
	CScale& Scale
)
{
	int i;

	for (i = 0; i < n; ++i)
	{
		dest[i] = CPoint((ULHC + src[i]) * Scale);
	}
	return dest;
}

//--------------------------------------
//  General Elipse Methods
//--------------------------------------
BOOL CFrontCadApp::PointInEllipse(
	double A,	// Major Axis
	double B,	// Minor Axis
	DOUBLEPOINT Point,	//Test this point 
	DOUBLEPOINT Center	// Cneter point of Ellipse
)
{
	BOOL rV = FALSE;
	if (GETAPP.Ellipse(
		A,	// Major Axis
		B,	// Minor Axis
		Point,	//Test this point 
		Center	// Cneter point of Ellipse
	) <= 1.00
		)
		rV = TRUE;
	return rV;
}

double CFrontCadApp::Ellipse(
	double A,	// Major Axis
	double B,	// Minor Axis
	DOUBLEPOINT Point,	//Test this point 
	DOUBLEPOINT Center	// Cneter point of Ellipse
)
{
	//----------------------------
	// Test to see if a point 
	// lies on an ellipse
	//----------------------------
	// Square Everything
	Point.dX -= Center.dX;
	Point.dY -= Center.dY;
	Point.dX *= Point.dX;
	Point.dY *= Point.dY;
	A *= A;
	B *= B;
	// Calculate Result
	return Point.dX / A + Point.dY / B;
}

BOOL CFrontCadApp::TestEllipsePoint(
	double A,	// Major Axis
	double B,	// Minor Axis
	DOUBLEPOINT Point,	//Test this point 
	DOUBLEPOINT Center,	// Cneter point of Ellipse
	double Tolerance	// just how close do we need to be
)
{
	//------------------------------------------------------
	// TestEllipsePoint
	// Evalutes the point to see how close it is to being
	// on the ellipse.
	// 
	// parameters:
	// A.......radia axis length in the X direcction
	// B.......radial axis length in the Y direction
	// Point...point that is being tgested
	// Center..Center Point of Ellipse
	// Tolerance..How close it needs to be
	//	the Tolerance will be like this:
	// 1.1 indicates 10 percent
	// 1.2 indicates 20 percent
	// 1.0 indicates it must be exact (0 percent)
	// etc
	// returns
	// TRUE if the point is within tolerance
	//------------------------------------------------------
	BOOL OK = FALSE;
	double Delta;
	double Lower = 1.0 / Tolerance;

	// Calculate Delta
	Delta = Ellipse(A,B,Point,Center) - 1.0;
	if (Delta < Tolerance && Tolerance > Lower)
		OK = TRUE;
	return OK;
}

//---------------------------------------------
// Math Methods
//---------------------------------------------

BOOL CFrontCadApp::QuadradicEquation(double a, double b, double c, double& X1, double& X2)
{
	//-------------------------------------
	// QuadradicEquation
	// calculates the solution of a
	// quadradic equation
	// 
	// parameters:
	//	a.......X^2 coeficient
	//	b.......X coeficient
	//	c.......Constant
	// 
	// Returns:
	// FALSE on fail (b^2 - 4ac) < 0
	// TRUE on Success
	//		X1 and X2 will be the solutions
	//-------------------------------------
	BOOL rv = FALSE;
	double t;

	t = b * b - 4.0 * a * c;
	if (t >= 0.0)
	{
		rv = TRUE;
		t = sqrt(t);
		X1 = (-b + t) / (2.0 * a);
		X2 = (-b - t) / (2.0 * a);
	}
	return rv;
}

double CFrontCadApp::ArcTan(double X, double Y)
{
	///------------------------------------
	/// ArcTan
	///		This Method calculates the
	///	Arctangent of X/Y.  The resulting
	///	angle will be between 0 and 2*PI
	///
	///	parameters
	///		X.......X coordinate of vector
	///		Y.......Y coordinate of vector
	///	returns
	///		The angle whose tangent is X/Y in Radians
	///------------------------------------
	double Angle;
	///-------------------------------------------------------
	/// Calculate the initial phase
	///-------------------------------------------------------
	if      ((X > 0) && (Y > 0)) Angle = atan2(Y, X);	//Quadrant 1
	else if ((X < 0) && (Y > 0)) Angle = -atan2(-Y, X);	//Quadrant 2
	else if ((X < 0) && (Y < 0)) Angle = Pi + atan2(-Y, -X);	//Quadrant 3
	else if ((X > 0) && (Y < 0)) Angle = Pi - atan2(Y, -X);	//Quadrant 4
	else if ((X == 0) && (Y < 0)) Angle = 3.0 * Pi / 2.0;
	else if ((X == 0) && (Y > 0)) Angle = Pi / 2.0;
	else if ((X < 0) && (Y == 0)) Angle = Pi;
	else if ((X > 0) && (Y == 0)) Angle = 0.0;
	else if ((X == 0) && (Y == 0)) Angle = 0.0;
	return Angle;
}

double CFrontCadApp::RoundToNearset(double v, double roundto)
{
	//-----------------------------------
	// RoundToNearset
	//	This methode will round a double
	// value to the nearset increment
	// Example v = 6.4, roundto = .250
	//	will yeild 6.5
	// If v = 6.3, will yeild 6.25
	//-----------------------------------
	double rV = v;
	double FracPart;
	double IntPart = 0.0;

	if (roundto != 0.0)
	{
		FracPart = modf(v / roundto, &IntPart);
		if (FracPart > 0.0)
		{
			if (FracPart >= 0.5)
				rV = (IntPart + 1.0) * roundto;
			else
				rV = IntPart * roundto;
		}
		else if (FracPart < 0.0)
		{
			if (FracPart <= -0.5)
				rV = (IntPart - 1) * roundto;
			else
				rV = (IntPart)*roundto;
		}
		else
			rV = v;
	}
	return rV;
}

DOUBLEPOINT CFrontCadApp::RoundToNearest(DOUBLEPOINT point, CDoubleSize roundto)
{
	if (roundto.dCX != 0.0 && roundto.dCY != 0.0)
	{
		double FracPartX, FracPartY;
		double IntPartX, IntPartY;
		double X, Y;
		
		FracPartX = modf(point.dX / roundto.dCX, &IntPartX);
		FracPartY = modf(point.dY / roundto.dCY, &IntPartY);
		if (FracPartX > 0.0)
		{
			if (FracPartX >= 0.5)
				X = (IntPartX + 1.0) * roundto.dCX;
			else
				X = IntPartX * roundto.dCX;
		}
		else if (FracPartX < 0.0)
		{
			if (FracPartX <= -0.5)
				X = (IntPartX - 1) * roundto.dCX;
			else
				X = (IntPartX)* roundto.dCX;
		}
		else
			X = point.dX;
		//-------------------------
		if (FracPartY > 0.0)
		{
			if (FracPartY >= 0.5)
			{
				Y = (IntPartY + 1.0) * roundto.dCX;
			}
			else
			{
				Y = IntPartY * roundto.dCY;
			}
		}
		else if (FracPartY < 0.0)
		{
			if (FracPartY <= -0.5)
			{
				Y = (IntPartY - 1.0) * roundto.dCY;
			}
			else
			{
				Y = (IntPartY)*roundto.dCY;
			}
		}
		else
			Y = point.dY;
		point.dX = X;
		point.dY = Y;
	}
	return point;
}

double CFrontCadApp::RoundDownToNearest(double v, double roundDownTo)
{
	double rV = v;
	double FracPart;
	double IntPart = 0.0;

	if (roundDownTo != 0.0)
	{
		FracPart = modf(v / roundDownTo, &IntPart);
		if (FracPart > 0.0)
			rV = IntPart * roundDownTo;
		else if (FracPart < 0.0)
			rV = (IntPart + 1) * roundDownTo;
		else
			rV = 0.0;
	}
	return rV;
}

double CFrontCadApp::RoundUpToNearest(double v, double roundupto)
{
	//-----------------------------------
	// RoundUpToNearest
	//	This methode will round a double
	// value to the nearset increment
	// above it in value.
	// Example v = 6.4, roundto = .250
	//	will yeild 6.5
	// If v = 6.3, will also yeild 6.5
	// 
	// NOTE: You CANNOT round up to the
	// nearest 0.00!!!
	//-----------------------------------
	double rV;
	double FracPart;
	double IntPart = 0.0;

	if (roundupto == 0.0)	//special case ERROR, but we fix
		roundupto = 1.0;
	FracPart = modf(v / roundupto, &IntPart);
	if (FracPart > 0.0)
		rV = (IntPart + 1.0) * roundupto;
	else
		rV = IntPart * roundupto;
	return rV;
}

BOOL CFrontCadApp::CheckAngle(double Start, double End, double Angle)
{
	///-------------------------------------
	/// CheckAngle
	///		Determines if an angle is between
	///	two other angles.
	///
	///	parameters:
	///		Start.......First angle to compare to
	///		End.........Second Angle to compare to
	///		Angle.......Is Angle between Start and End
	///	Returns:
	///		True if Angle is between Start and End
	///-------------------------------------
	BOOL rV = FALSE;
	if (Start > End)
	{
		if ((Angle > 0.0) && (Angle < End))
			rV = TRUE;
		else if (Angle > Start)
			rV = TRUE;
	}
	else
	{
		if ((Angle > Start) && (Angle < End))
			rV = TRUE;
	}
	return rV;
}

double CFrontCadApp::ArcSin(double X, double Y)
{
	double Radius;
	double Angle;

	Radius = sqrt(X * X + Y * Y);
	Angle = asin(Y / Radius);
	//-------------------------------
	// A sine will return a value
	// of Pi/2 -> -Pi/2 (Y 1 to -1)
	//------------------------------
	if (GETAPP.IsPositive(Y) && GETAPP.IsNegative(X))
		//Quadrant II 0 -> Pi/ 2, Add Pi/2
		Angle += GETAPP.GetPi() / 2.0;
	else if (GETAPP.IsNegative(Y) && GETAPP.IsNegative(X))
		//Quadrant III 0 -> -Pi/2 => -Angle+Pi
		Angle = -Angle + GETAPP.GetPi();
	else if (GETAPP.IsNegative(Y) && GETAPP.IsPositive(X))
		//Quadrant IV -Pi/2 -> 0 -Angle + 2 * Pi
		Angle = -Angle + 2.0 * GETAPP.GetPi();
	Angle = Angle *  180.0 / GETAPP.GetPi();//convert to degrees
		return Angle;
}

//------------------------------------------------
// Saving Various Attributes
//-----------------------------------------------

void CFrontCadApp::SaveColor(
	FILE* pO, 
	int Indent, 
	COLORREF color, 
	DocFileParseToken Token)
{
	char* IndentString = new char[256];
	MkIndentString(IndentString, Indent, ' ');
	fprintf(pO, "%s%s(", 
		IndentString,
		CLexer::TokenToString(Token));
	fprintf(pO, "%s(%d),",
		CLexer::TokenToString(DocFileParseToken::RED),
		RED(color)
	);
	fprintf(pO, "%s(%d),",
		CLexer::TokenToString(DocFileParseToken::GREEN),
		GREEN(color)
	);
	fprintf(pO, "%s(%d)",
		CLexer::TokenToString(DocFileParseToken::BLUE),
		BLUE(color)
	);
	CloseParen(pO);
	NewLine(pO);
	delete[]IndentString;
}

void CFrontCadApp::SaveString(FILE* pO, int Indent, CString& csStr, DocFileParseToken Token)
{
	char* IndentString = new char[256];
	char* StringString = new char[256];

	MkIndentString(IndentString, Indent);
	fprintf(pO, "%s%s(\"%s\"\n",
		IndentString,
		CLexer::TokenToString(Token),
		ConvertCStringToChar(StringString, csStr)
	);
	delete[] StringString;
	delete[] IndentString;
}

void CFrontCadApp::SaveDouble(FILE* pO, int Indent, double v, DocFileParseToken Token)
{
	char* IndentString = new char[256];

	MkIndentString(IndentString, Indent);
	fprintf(pO, "%s%s(%6.1lf)\n",
		IndentString,
		CLexer::TokenToString(Token),
		v
	);
	delete[] IndentString;
}

void CFrontCadApp::SaveUINT(FILE* pO, int Indent, UINT v, DocFileParseToken Token)
{
	char* IndentString = new char[256];

	MkIndentString(IndentString, Indent);
	fprintf(pO, "%s%s(%u)\n",
		IndentString,
		CLexer::TokenToString(Token),
		v
	);
	delete[] IndentString;
}

void CFrontCadApp::OnFileOpen()
{

}

//-----------------------------------------------------------

BOOL CFrontCadApp::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_NCMBUTTONDOWN:
		printf("Middle Button Down NC\n");
		break;
	case WM_NCMBUTTONUP:
		printf("Middle Button UP NC\n");
		break;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}
