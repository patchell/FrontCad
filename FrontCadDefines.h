#pragma once
//------------------------------------------

constexpr auto MAX_STRING_FIELD_SIZE = 128;
#define GETAPP	(theApp)
#define GETVIEW	((CFrontCadView *)(theApp.GetCurrentView()))
//------------------------------------------
// Point Reflect Modes
//------------------------------------------
constexpr auto POINT_REFLECT_X = 1;
constexpr auto POINT_REFLECT_Y = 2;
constexpr auto POINT_REFLECT_BOTH = 0;
//------------------------------------------
// types of slopes that are calculated
// by CPoint::Slope and CPoint::Ortoganol
//------------------------------------------
constexpr auto SLOPE_NOT_ORTHOGONAL = 0;
constexpr auto SLOPE_HORIZONTAL = 1;
constexpr auto SLOPE_VERTICAL = 2;


//---------------------------------------
// Snap Order Class
//---------------------------------------
enum class SnapType {
	SNAP_NONE,
	SNAP_GRID,
	SNAP_POINT,
	SNAP_LINE,
	SNAP_CENTER,
	SNAP_TANGENT
};
struct SnapOrder
{
	CString m_csName;
	SnapType m_Id;
	UINT m_Order;
	BOOL m_Enabled;
	SnapOrder* m_pNext;
	SnapOrder* m_pPrev;
	SnapOrder() {
		m_Id = SnapType::SNAP_NONE;
		m_Order = 0;
		m_Enabled = FALSE;
		m_pNext = 0;
		m_pPrev = 0;
	}
	SnapOrder* GetNext() { return m_pNext; }
	void SetNext(SnapOrder* pSO) { m_pNext = pSO; }
	SnapOrder* GetPrev() { return m_pPrev; }
	void SetPrev(SnapOrder* pSO) { m_pPrev = pSO; }
	int GetIndex(CString csSearchFor) {

	}
};

class SnapOrderList {
	SnapOrder* m_pHead;
	SnapOrder* m_pTail;
	UINT m_nNumberOfObjects;
public:
	SnapOrderList() {
		m_pHead = 0;
		m_pTail = 0;
		m_nNumberOfObjects = 0;
	}
	void SetHead(SnapOrder* pSO) { m_pHead = pSO; }
	SnapOrder* GetHead() { return m_pHead; }
	void SetTail(SnapOrder* pSO) { m_pTail = pSO; }
	SnapOrder* GetTail() { return m_pTail; }
	UINT GetNumberOfSnaps() { return m_nNumberOfObjects; }
	void SetNumberOfSnaps(UINT n) { m_nNumberOfObjects = n; }
	void AddSnapAtHead(SnapOrder* pSO);
	void AddSnapAtTail(SnapOrder* pSO);
	BOOL InsertSnap(SnapOrder* pPrevSO, SnapOrder* pSO);
	SnapOrder* Remove(SnapOrder* pSO, BOOL bDelete = FALSE);
};

//---------------------------------------
// IDs for context menus
//---------------------------------------

constexpr auto ID_CM_FORWARD = 0x2000;
constexpr auto ID_CM_BACKWARD = 0x2001;
constexpr auto ID_CM_DELETE = 0x2002;
constexpr auto  ID_CM_RESTOREASPECTRATIO = 0x2003;
constexpr auto  ID_CM_ENDDRAWMODE = 0x2004;
constexpr auto  ID_CM_DESELECT = 0x2005;
constexpr auto  ID_CM_DESELECT_ALL = 0x2006;
constexpr auto  ID_CM_EDIT_PROPERIES = 0x2007;
constexpr auto  ID_CM_PLACE = 0x208;

//place submenu
constexpr auto ID_CM_PLACE_LINE = 0x2100;
constexpr auto  ID_CM_PLACE_RECT = 0x2101;
constexpr auto  ID_CM_PLACE_ELLISPE = 0x2102;
constexpr auto  ID_CM_PLACE_ARC = 0x2103;
constexpr auto  ID_CM_PLACE_ARC_CENTERED = 0x2104;
constexpr auto  ID_CM_PLACE_ROUNDED_RECT = 0x2105;
constexpr auto  ID_CM_PLACE_ARROW = 0x2106;
constexpr auto  ID_CM_PLACE_POLY = 0x2107;
constexpr auto  ID_CM_PLACE_FILLEDPOLY = 0x2108;
constexpr auto ID_CM_PLACE_LIBCOMP = 0x2109;
constexpr auto  ID_CM_PLACE_HOLEROUND = 0x210A;
constexpr auto  ID_CM_PLACE_HOLERECT = 0x210B;
constexpr auto  ID_CM_PLACE_HOLERND1FLAT = 0x210C;
constexpr auto  ID_CM_PLACE_HOLERND2FLAT = 0x210D;
constexpr auto  ID_CM_PLACE_TEXT = 0x210E;
constexpr auto  ID_CM_PLACE_BITMAP = 0x210F;
constexpr auto ID_CM_PLACE_ORIGIN = 0x2110;
constexpr auto ID_CM_PLACE_DIMENSION = 0x2111;

//----------------------------------
// ID Constants for Selecting an
// Object(s) that the cursor is near
//----------------------------------

constexpr auto POPUP_MENU_ITEM_IDS = 0X3000;

//----------------------------------
// Constants for Line Types
//----------------------------------
constexpr auto LINE_IS_WHATEVER = 0;
constexpr auto LINE_IS_HORIZONTAL = 1;
constexpr auto LINE_IS_VERTICAL = 2;
//----------------------------------
// Document and Paper Size
//----------------------------------
constexpr auto DOCUMENT_PAPER_X = 11.0;
constexpr auto DOCUMENT_PAPER_Y = 8.5;
constexpr auto DOCUMENT_DRAWING_X = 22.0;
constexpr auto DOCUMENT_DRAWING_Y = 17.0;
//----------------------------------
// It should be noted that point mP1
// is used as the center poiot, and
// m_P2 is used as the radius
//----------------------------------
#define TOLERANCE_10_PERCENT	(0.1)
#define TOLERANCE_20_PERCENT	(0.2)

//---------------------------------

union CADObjectTypes {
	class CCadObject* pCadObject;
	class CCadArc *pCadArc;
	class CCadArcCent *pCadArcCent;
	class CCadArrow *pCadArrow;
	class CCadBitmap *pCadBitmap;
	class CCadDimension *pCadDimension;
	class CCadDrawing* pCadDrawing;
	class CCadEllipse *pCadElispe;
	class CCadHoleRect *pCadHoleRect;
	class CCadHoleRnd1Flat *pCadHoleRnd1Flat;
	class CCadHoleRnd2Flat *pCadHoleRnd2Flat;
	class CCadHoleRound *pCadHoleRound;
	class CCadLibPart*pCadLibPart;
	class CCadLibrary* pLibrary;
	class CCadLine *pCadLine;
	class CCadOrigin *pCadOrigin;
	class CCadPoint* pCadPoint;
	class CCadPolygon *pCadPolygon;
	class CCadRect *pCadRect;
	class CCadRndRect *pCadRndRect;
	class CCadText *pCadText;
	CADObjectTypes() { pCadObject = 0; }
};

//-----------------------------------------------------
// Save flags
//-----------------------------------------------------
constexpr auto CADOBJECT_SAVE_ADDCOMMA_ATEND = 1;
//-----------------------------------------------------
constexpr auto MAX_ZOOM = 7;	//maximum number of zoom levels;

#define RED(x)		((x & 0xff0000) >> 16)
#define GREEN(x)	((x & 0xff00) >> 8)
#define BLUE(x)		(x & 0x0ff)

//-----------------------------------
// Types of Flips
//-----------------------------------
enum class Axis {
	X,
	Y
};

//-----------------------------------------
// For Testing Arcs/Ellipses
//-----------------------------------------


enum class BadDelta {
	GOOD,
	X,
	Y
};

//---------------------------------------
enum class RectangleExtendMode {
	BOTH,
	WIDTH,
	HEIGHT
};

enum class LineExtendMode {
	LENGTH,
	LENGTH_ANGLE
};
//-----------------------------------
// Defines for drawing check selected
//-----------------------------------

enum class DrawingCheckSelectFlags {
	FLAG_ALL,
	FLAG_UNSEL,
	FLAG_SEL
};


//-----------------------------------
// CCadObject Enable Flags
//-----------------------------------
constexpr auto OBJECT_ENABLE_ROUNDEDRECT = 0x0001;
constexpr auto OBJECT_ENABLE_POLYGON = 0x0002;
constexpr auto OBJECT_ENABLE_LIBCOMP = 0x0008;
constexpr auto OBJECT_ENABLE_HOLEROUND = 0x0010;
constexpr auto OBJECT_ENABLE_HOLERECT = 0x0020;
constexpr auto OBJECT_ENABLE_HOLERND1FLAT = 0x0040;
constexpr auto OBJECT_ENABLE_HOLERND2FLAT = 0x0080;
constexpr auto OBJECT_ENABLE_TEXT = 0x0100;
constexpr auto OBJECT_ENABLE_BITMAP = 0x0200;
constexpr auto OBJECT_ENABLE_ARROW = 0x0400;
constexpr auto OBJECT_ENABLE_ORIGIN = 0x0200;
constexpr auto OBJECT_ENABLE_DIMENSION = 0x1000;
constexpr auto OBJECT_ENABLE_POINT = 0x2000;
//----------------------------------
// Kinds of Objects
//----------------------------------
constexpr auto OBJKIND_ALL = 0xFFFFFFFF;
constexpr auto OBJKIND_ARC = 0x00000001;
constexpr auto OBJKIND_ARCCENTERED = 0x00000002;
constexpr auto OBJKIND_ARCANGLE = 0x00000004;
constexpr auto OBJKIND_ARROW = 0x00000008;
constexpr auto OBJKIND_BITMAP = 0x00000010;
constexpr auto OBJKIND_DIMENSION = 0x00000020;
constexpr auto OBJKIND_ELIPSE = 0x00000040;
constexpr auto OBJKIND_HOLE_RECTANGLE = 0x00000080;
constexpr auto OBJKIND_HOLE_RND1FLAT = 0x00000100;
constexpr auto OBJKIND_HOLE_RND2FLAT = 0x00000200;
constexpr auto OBJKIND_HOLE_ROUND = 0x00000400;
constexpr auto OBJKIND_LIBPART = 0x00000800;
constexpr auto OBJKIND_LINE = 0x00001000;			
constexpr auto OBJKIND_ORIGIN = 0x00002000;
constexpr auto OBJKIND_POINT = 0x00004000;
constexpr auto OBJKIND_POLYGON = 0x00008000;
constexpr auto OBJKIND_RECT = 0x00010000;
constexpr auto OBJKIND_RECT_ROTATED = 0X00020000;
constexpr auto OBJKIND_ROUNDEDRECT = 0x00040000;
constexpr auto OBJKIND_TEXT = 0x00080000;
constexpr auto OBJKIND_DRAWING = 0x00100000;
constexpr auto OBJKIND_CHOOSE = 0x40000000;
constexpr auto OBJKIND_SELECT = 0x80000000;


constexpr auto MAX_SNAPS = 16;



//--------------------------------------------
// Object DRAW mode defines
// these values are used to dtermine how
// various objects are drawin
//--------------------------------------------

struct MODE {
	enum ObjectPaintMode {
		NOP = -1,
		FINAL = 0,
		SKETCH,
		ARCSTART,
		ARCEND,
		GRABBED,
	};
	ObjectPaintMode PaintMode;
	MODE() {
		PaintMode = ObjectPaintMode::NOP;
	}
};

//------------------------------------
// DrawingMode represent what it is
// that is being drawn.
//------------------------------------

enum class DrawingMode{
	SELECT = 0,
	ALIGN_DIM_TO_HOLE,
	ARC,
	ARC_ANGLE,
	ARC_CENTER,
	ARROW,
	BITMAP,
	COPY,
	CUT,
	DIMENSION,
	ELIPSE,
	GETREF,
	HOLE_RECT,
	HOLE_RND1F,
	HOLE_RND2F,
	HOLE_ROUND,
	LIBPART,
	LINE,
	MOVE,
	ORIGIN,
	PASTE,
	POINT,
	PRINTRECT,
	RECT,
	RNDRECT,
	POLYGON,
	SELECTREGION,
	TEXT,
};

//----------------------------------------
// The state values that represent the
// steps that are taken to do a specific
// drawing task
//----------------------------------------
enum class ObjectDrawState {
	NULL_STATE,
	START_DRAWING,
	END_DRAWING,
	SET_ATTRIBUTES,
	WAITFORMOUSE_DOWN_LBUTTON_DOWN,
	WAITFORMOUSE_DOWN_LBUTTON_UP,
	PLACE_LBUTTON_DOWN,
	PLACE_LBUTTON_UP,
	//-----Copy/Cut/Pastse -------
	COPY_LBUTTON_DOWN,
	COPY_LBUTTON_UP,
	PASTE_LBUTTON_DOWN,
	PASTE_LBUTTON_UP,
	//------ Arc States ------
	ARCSTART_LBUTTON_DOWN,
	ARCSTART_LBUTTON_UP,
	ARCEND_LBUTTON_DOWN,
	ARCEND_LBUTTON_UP,
	//------- Dimension States --------
	SELECT_OBJECT_TO_DIMENSION_LBUTTON_DOWN,
	SELECT_OBJECT_TO_DIMENSION_LBUTTON_UP,
	SECOND_POINT_LBUTTON_DOWN,
	SECOND_POINT_LBUTTON_UP,
	EXTENSION_LINES_LBUTTON_DOWN,
	EXTENSION_LINES_LBUTTON_UP,
	//------------ Arrow/ROTATE States ----------------
	SELECT_PIVOT_LBUTTON_DOWN,
	SELECT_PIVOT_LBUTTON_UP,
	ROTATE_LBUTTON_DOWN,
	ROTATE_LBUTTON_UP,
	//-------- Fixed Lenght Line States --------------
	START_DRAWING_LINE_FIXED_LEN,
	FIXED_LINE_RIGHTANGLE_MOUSE_DOWN,
	FIXED_LINE_RIGHTANGLE_MOUSE_UP,
	FIXED_LINE_FIRST_POINT_MOUSE_DOWN,
	FIXED_LINE_FIRST_POINT_MOUSE_UP,
	FIXED_LINE_SECOND_POINT_MOUSE_DOWN,
	FIXED_LINE_SECOND_POINT_MOUSE_UP,
	//---------------------------------------------
	// Rectangle Drawing
	//---------------------------------------------
	//-------- Rotated Rectangle States Mode 1 ----
	RECT_ROT_ROTATED_START_DRAWING,
	RECT_ROT_ROTATION_PIVOT_MOUSE_DOWN,
	RECT_ROT_ROTATION_PIVOT_MOUSE_UP,
	RECT_ROT_FIRST_POINT_MOUSE_DOWN,
	RECT_ROT_FIRST_POINT_MOUSE_UP,
	RECT_ROT_SECOND_POINT_MOUSE_DOWN,
	RECT_ROT_SECOND_POINT_MOUSE_UP,
	RECT_ROT_ROTATE_POINT_MOUSE_DOWN,
	RECT_ROT_ROTATE_POINT_MOUSE_UP,
	//-------- Rect Width/Height/Rotate --------
	RECT_HWR_START_DRAWING,
	RECT_HWR_PIVOT_MOUSE_DOWN,
	RECT_HWR_PIVOT_MOUSE_UP,
	RECT_HWR_BASE_FIRST_POINT_MOUSE_DOWN,
	RECT_HWR_BASE_FIRST_POINT_MOUSE_UP,
	RECT_HWR_BASE_SECOND_POINT_MOUSE_DOWN,
	RECT_HWR_BASE_SECOND_POINT_MOUSE_UP,
	RECT_HWR_HIEGTH_MOUSE_DOWN,
	RECT_HWR_HIEGTH_MOUSE_UP,
	//------------Rect From Center -------------
	RECT_CENT_START_DRAWING,
	RECT_CENT_PIVOT_MOUSE_DOWN,
	RECT_CENT_PIVOT_MOUSE_UP,
	RECT_CENT_CENTER_MOUSE_DOWN,
	RECT_CENT_CENTER_MOUSE_UP,
	RECT_CENT_SECPMD_POINT_MOUSE_DOWN,
	RECT_CENT_SECOND_POINT_MOUSE_UP,
	RECT_CENT_ROTATION_POINT_MOUSE_DOWN,
	RECT_CENT_ROTATION_POINT_MOUSE_UP,
	//------------------------------------------
	//----------- Misc States ------------------
	//------------------------------------------
	MOVE,
	PLACE_AUTO,
	GETREFERENCE,
	DRAG,
	PLACE_RADIUS_LBUTTON_DOWN,
	PLACE_RADIUS_LBUTTON_UP,
	//----------------------------------------------
	SELECT_WAITFOR_LBUTTON_DOWN,	//nothing is selected
	SELECT_WAITFOR_LBUTTON_DOWN_TIMEOUT,
	SELECT_LBUTTON_DOWN_DRAGGING,
	SELECT_WAITFOR_LBUTTON_UP,
	//--------------------------------------------
	PASTE_WAITFOR_LBUTTON_DOWN,
	PASTE_WAITFOR_LBUTTON_UP,
	SELECT_MOVE,
	SELECT_COPY,
};
//--------------------------------------------
// Ruler  And ToolBar Defines
// -------------------------------------------

constexpr auto RULER_SIZE = (32);
constexpr auto RULER_SNAP_TICK_LEN = 8;
constexpr auto RULER_HALF_TICK_LEN = 12;
constexpr auto RULER_MAJOR_TICK_LEN = 16;
constexpr auto RULER_FONT_HEIGHT = 16;

constexpr auto TOOLBAR_HIEGHT = 32;

constexpr auto TOOLBAR_ID_BASE = 1250;
constexpr auto IDC_STATIC_TOOLBAR_X = TOOLBAR_ID_BASE;
constexpr auto IDC_STATIC_TOOLBAR_Y = TOOLBAR_ID_BASE + 1;
constexpr auto IDC_COMBO_ORIGINLIST = TOOLBAR_ID_BASE + 2;

//--------------------------------------------
// Windows User Messages
//--------------------------------------------

enum class WindowsMsg {
	WM_ARROWATTRB =	WM_USER + 1,
	//-------------------------
	// CAutoScrollView Messages
	//-------------------------
	WM_STOPAUTOSCROLL,
	WM_RULER_MESSAGE,
	WM_MAINFRAME_FINISHUPDATE,
	//--------------------------
	// Messages From Tool Bar
	//--------------------------
	WM_FROM_TOOLBAR_MESSAGE,
	//--------------------------
	// Messages pertaining to
	// CMyPopupMenu
	//--------------------------
	WM_PU_MENU_HOVER_INDEX,
	WM_PU_MENU_SELECTED_INDEX
};

//----------------------------
// WM_FROM_TOOLBAR_MESSAGE
//	Submessages
//---------------------------

enum class ToolBarMsg {
	ORIGIN_SEL_CHANGE
};

enum class WMarrowSubMsg {
WM_ARROWATTRB_VALUE_L1,
WM_ARROWATTRB_VALUE_L2,
WM_ARROWATTRB_VALUE_W,

};
/*------------------------------------------------------
	Defines of objects that go inside of control bars
------------------------------------------------------*/
enum {
	ID_COMBO_BOX_UNIT = 2000,
};

