#pragma once
//------------------------------------------

#define GETAPP	(theApp)
#define GETVIEW()	((CFrontCadView *)(theApp.GetCurrentView()))

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

typedef union {
	class CCadObject* pCadObject;
	class CCadArc *pCadArc;
	class CCadArcCent *pCadArcCent;
	class CCadArrow *pCadArrow;
	class CCadBitmap *pCadBitmap;
	class CCadDimension *pCadDimension;
	class CCadElispe *pCadElispe;
	class CCadHoleRect *pCadHoleRect;
	class CCadHoleRnd1Flat *pCadHoleRnd1Flat;
	class CCadHoleRnd2Flat *pCadHoleRnd2Flat;
	class CCadHoleRound *pCadHoleRound;
	class CCadLibComp *pCadLibComp;
	class CCadLine *pCadLine;
	class CCadOrigin *pCadOrigin;
	class CCadPolygon *pCadPolygon;
	class CCadRect *pCadRect;
	class CCadRndRect *pCadRndRect;
	class CCadText *pCadText;
}CADObjectTypes;

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


//----------------------------------
// Defines for parsers
//----------------------------------

enum class DocFileParseToken {
	EOFToken = -1,
	ERRORToken,
	MATCHED,
	NUM	=256,
	STRING,
	LINE,
	RECT,
	ELIPSE,
	ROUNDRECT,
	POLYGON,
	LIBPART,
	HOLEROUND,
	HOLERECT,
	HOLERND1FLAT,
	HOLERND2FLAT,
	TEXT,
	ARC,
	ARCCENTERED,
	ARCANGLE,
	BITMAP,
	ARROW,
	ORIGIN,
	DIMENSION,
	COLOR,
	BKCOLOR,
	FILLCOLOR,
	WIDTH,
	POINT,
	SIZE,
	DRAWING,
	RED,
	GREEN,
	BLUE,
	RADIUS,
	VERTEX,
	FONT,
	WEIGHT,
	HEIGHT,
	ANGLE,
	TRANSPARENTToken,
	DOUBLERECT,
	SHAPE,
	START,
	END,
	CENTER,
	ARROW_TIP,
	ARROW_END,
	ATTRIB_ARC,
	ATTRIB_ARCANGLE,
	ATTRIB_ARCCENTER,
	ATTRIB_ARROW,
	ATTRIB_BITMAP,
	ATTRIB_DIMENSION,
	ATTRIB_ELLIPSE,
	ATTRIB_HOLERECT,
	ATTRIB_HOLERND1F,
	ATTRIB_HOLERND2F,
	ATTRIB_HOLERND,
	ATTRIB_LINE,
	ATTRIB_ORIGIN,
	ATTRIB_POLYGON,
	ATTRIB_RECT,
	ATTRIB_RNDRECT,
	ATTRIB_TEXT,
	TEXTCOLOR,
	LINECOLOR,
	FORMAT
};

struct KEYWORD {
	DocFileParseToken m_Token;
	const char *m_pName;
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

//----------------------------------
// CCadObject derived types
//----------------------------------

enum class ObjectType{
	BASE		=0,
	ARC,
	ARCCENTERED,
	ARCANGLE,
	ARROW,
	BITMAP,
	DIMENSION,
	ELIPSE,
	HOLE_RECTANGLE,
	HOLE_RND1FLAT,
	HOLE_RND2FLAT,
	HOLE_ROUND,
	LIBCOMP,
	LINE,			//drawing objects
	ORIGIN,
	POLYGON,
	RECT,
	ROUNDEDRECT,
	TEXT		,	
};

//--------------------------------------------
// Object DRAW mode defines
// these values are used to dtermine how
// various objects are drawin
//--------------------------------------------
enum class ObjectDrawMode {
	NOP = -1,
	FINAL=0,
	SELECTED,
	SKETCH,
	ARCSTART,
	ARCEND,
	GRABBED,
	ENDDRAWMODE
};

enum class SelectedPointsMode {
	NOP,
	POINT_ENCLOSED_IN_RECT,
	POINT_FILLED_RECT,
	POINT_BOTH_RECT_FILLED,
};

enum class SelectedLinesMode {
	NOP,
	MIDPOINT,
	HIGHLIGHT,
	BOTH
};

struct MODE {
	ObjectDrawMode DrawMode : 4;
	SelectedPointsMode PointsMode : 2;
	SelectedLinesMode LinesMode : 2;
	MODE() {
		DrawMode = ObjectDrawMode::NOP;
		PointsMode = SelectedPointsMode::NOP;
		LinesMode = SelectedLinesMode::NOP;
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
	//------------ Arrow States ----------------
	ROTATE_LBUTTON_DOWN,
	ROTATE_LBUTTON_UP,
	//----------- Misc States ------------------
	MOVE,
	PLACE_AUTO,
	GETREFERENCE,
	DRAG,
	SELECT_NOTHING,	//nothing is selected
	SELECT_MOVE,
	SELECT_COPY,
	VERTEX_GRABBED
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
	WM_FROM_TOOLBAR_MESSAGE
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

