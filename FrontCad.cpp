
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


void CFrontCadApp::CutCopy(CCadObject* pObj, int mode)
{
	//---------------------------------------------------
	//	CutCopy
	//		This Method will cut/copy a chain of 
	//	objects to the clipboard object.  The copy mode
	//	actually makes a copy of the objects where cut
	//	does not.
	//
	// parameters:
	//	pObj...........pointer to an to put in clipboard
	//	mode...........0=Don't Copy (Cut), 1 = Copy
	//----------------------------------------------------

	if (mode)	///copy the object
		m_ClipBoard.AddObject(pObj->CopyObject());
	else
		m_ClipBoard.AddObject(pObj);
}


void CFrontCadApp::CopyToClipBoard(CCadObject* pObjList)
{
	CCadObject* pO = pObjList;

	while (pO)
	{
		CutCopy(pO, 1);
		pO = pO->GetNextSel();
	}
}


void CFrontCadApp::CutToClipboard(CCadObject* pSelList)
{
	CCadObject* pO = pSelList;

	while (pO)
	{
		CutCopy(pO, 0);
		pO = pO->GetNextSel();
	}
}

void CFrontCadApp::SetClipboardRef(CDoublePoint p)
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
// Polygon Methods
// ------------------------------------

BOOL CFrontCadApp::GeneratePointInPolygon(CDoublePoint* Poly, int n, CDoublePoint& point)
{
	double MaxX = -numeric_limits<double>::infinity(), MinX = numeric_limits<double>::infinity();
	double MaxY = -numeric_limits<double>::infinity(), MinY = numeric_limits<double>::infinity();
	int i;
	BOOL Result = FALSE;

	//Find the rectangle that encloses the figure
	for (i = 0; i < n; ++i)
	{
		if (MaxX < Poly[i].dX) MaxX = Poly[i].dX;
		if (MinX > Poly[i].dX) MinX = Poly[i].dX;
		if (MaxY < Poly[i].dY) MaxY = Poly[i].dY;
		if (MinY > Poly[i].dY) MinY = Poly[i].dY;
	}
	// Create a rectangle
	CDoubleRect rect(CDoublePoint(MinX, MinY), CDoublePoint(MaxX, MaxY));
	CDoublePoint Center;
	Center = rect.GetCenter(Center);
	i = 0;
	if ((Result = PtEnclosedInPolygon(Center, Poly, n)) == TRUE)
		point = Center;
	else
	{
		//first guess did not work, try four more
		int loop = 1;
		while (loop&&!Result)
		{
			CDoublePoint p1;
			//----------------------
			// well, a really odd
			// shaped polygon.
			//----------------------
			switch (i)
			{
			case 0:
				p1 = CDoublePoint(MinX, MinY);
				break;
			case 1:
				p1 = CDoublePoint(MinX, MaxY);
				break;
			case 2:
				p1 = CDoublePoint(MaxX, MaxY);
				break;
			case 3:
				p1 = CDoublePoint(MaxX, MinY);
				loop = 0; //final try
				break;
			}
			rect.SetPointsFromCenter(Center, p1, Center);
			Result = PtEnclosedInPolygon(rect.GetCenter(point), Poly, n);
		}
		point = rect.GetCenter(point);	//we can only hope
	}
	return Result;
}

BOOL CFrontCadApp::PtEnclosedInPolygon(
	CDoublePoint ptPoint,
	CDoublePoint* ptArray,
	UINT nVeticies
)
{
	/*****************************************
	** PtEnclosedInPolygon
	**	This Method determines if a point
	** is enclosed within a polygon.
	**
	** parameters:
	*	ptPoint....point to test
	*	ptArray....array of points defining polygon
	*	nVeticies..number of verticies in polygon
	*	ptOffset...polygon offset
	*
	** Returns: TRUE if point inside
	**          FALSE if point is outside
	*****************************************/
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

void CFrontCadApp::ShiftDoublePointArray(CDoublePoint* pdptPoints, UINT nPoints, UINT Direction)
{
	int i;
	CDoublePoint dptTemp;

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


CPoint* CFrontCadApp::MakePolygonFromDoublePolygon(
	CPoint* dest,
	CDoublePoint* src,
	int n,
	CSize Offset,
	CScale Scale
)
{
	int i;
//	printf("---------- - MakePolygonFromDoublePoly--------------------\n");
	for (i = 0; i < n; ++i)
	{
		dest[i] = src[i].ToPixelPoint(Offset, Scale);
//		src[i].Print();
	}
//	printf("--------------\n");
	return dest;
}

//--------------------------------------
//  General Elipse Methods
//--------------------------------------
BOOL CFrontCadApp::PointInEllipse(
	double A,	// Major Axis
	double B,	// Minor Axis
	CDoublePoint Point,	//Test this point 
	CDoublePoint Center	// Cneter point of Ellipse
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
	CDoublePoint Point,	//Test this point 
	CDoublePoint Center	// Cneter point of Ellipse
)
{
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
	CDoublePoint Point,	//Test this point 
	CDoublePoint Center,	// Cneter point of Ellipse
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
