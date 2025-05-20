//----------------------------------------------------------------------------------------------------------
// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.
//----------------------------------------------------------------------------------------------------------

#ifndef PCH_H
#define PCH_H

enum class Token{
	DEFAULT = -2,
	EOFToken = -1,
	BASE_OBJ,
	MATCHED,
	NUMINT = 256,
	NUMDOUBLE,
	STRING,			//1
	//--------------------------------
	// Drawing Objects
	//--------------------------------
	ARC,				//2
	ARC_CENTER,		//3
	ARROW,
	BITMAP,
	CIRCLE,			//4
	DIMENSION,
	RECT,				//5
	RNDRECT,			//6
	POLY,				//7
	LINE,				//9
	HOLERND,			//10
	HOLERECT,			//11
	HOLE_1FLAT,		//12
	HOLE_2FLAT,		//13
	LIBPART,			//14
	ORG,	//token for origin object
	POINT,
	PRINTRECT,
	TEXT,				//15
	VERTEX_POINT,
	START_POINT,
	END_POINT,
	//colors
	BK_TRANSPARENT,
	BACKGROUND_COLOR,	//17
	LINE_COLOR,
	TEXT_COLOR,
	FILL_COLOR,
	SELECTED_COLOR,
	RED,
	GREEN,
	BLUE,
	//-------
	FONT,
	FONT_WEIGHT,
	FONTHEIGHT,
	FONTWIDTH,
	FONT_FORMAT,
	FONT_NAME,
	//--------------------
	RADIUS,
	FLATDIST,
	VERTEX,
	RECTWIDTH,
	RECTHIEGHT,
	ANGLE,
	REFERENCE,
	FILENAME,
	LENGTH,
	SIZE,
	SCALE,
	LINE_WIDTH,
	ARROW_LENGTH,
	ARROW_WIDTH,
	SELECTED_UNITS,
	//-------------------
	DRAWFILE,
	LIBFILE,
	// --- Attributes ---
	ATTRIB_ARC,
	ATTRIB_ARCCENTER,
	ATTRIB_ARROW,
	ATTRIB_BITMAP,
	ATTRIB_DIMENSION,
	ATTRIB_ELLIPSE,
	ATTRIB_GRID,
	ATTRIB_HOLERECT,
	ATTRIB_HOLERND1F,
	ATTRIB_HOLERND2F,
	ATTRIB_HOLERND,
	ATTRIB_LINE,
	ATTRIB_ORIGIN,
	ATTRIB_POINT,
	ATTRIB_POLYGON,
	ATTRIB_RECT,
	ATTRIB_RNDRECT,
	ATTRIB_RULERS,
	ATTRIB_TEXT,
	ERRORToken,
	ENDOFTOKENS
};


#include "framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <afxwin.h>
#include <math.h>
#include <limits>
#include "afxdialogex.h"
#include <afxmt.h>

#include "Exception.h"
#include "MyEvent.h"
#include "Primitives.h"
#include "FrontCadDefines.h"
#include "MyBitmap.h"
#include "EditDecimal.h"
#include "EditText.h"
#include "StaticBitmap.h"
#include "Scale.h"
#include "DoubleSize.h"
#include "resource.h"
#include "ObjectAttributs.h"
#include "ComboBoxFontWeight.h"
#include "MyBitmap.h"
#include "StaticDouble.h"
//-------------------------------------------------
#include "MainFrm.h"
//-------------- Drawing Objects ------------------
#include "CadObject.h"

#include "Lexer.h"	//lexer base class
#include "LexerFrontCad.h"
#include "Parser.h"
#include "ParserFrontCad.h"
#include "ParserFrontCadLib.h"

#include "EdaObject.h"
#include "SchObject.h"
#include "PcbObject.h"
#include "Clipboard.h"
#include "CadPoint.h"
#include "CadArc.h"
#include "CadArcCent.h"
#include "CadArrow.h"
#include "CadBitmap.h"
#include "CadElispe.h"
#include "CadHoleRect.h"
#include "CadHoleRnd1Flat.h"
#include "CadHoleRnd2Flat.h"
#include "CadHoleRound.h"
#include "CadLine.h"
#include "CadOrigin.h"
#include "CadPolygon.h"
#include "CadRect.h"
#include "CadRndRect.h"
#include "CadText.h"
#include "CadDimension.h"
#include "CadLibPart.h"
#include "CadLibrary.h"
#include "CadDrawing.h"
//-----------------------------
#include "SchWire.h"
#include "SchBus.h"
#include "SchJunction.h"
#include "SchBusEntry.h"
#include "SchLabel.h"
#include "SchSymbol.h"
#include "SchSheetSymbol.h"
#include "SchSheetSchematic.h"
#include "SchNoConnect.h"
#include "SchGlobalLabel.h"
#include "SchHierarchalLabel.h"
#include "SchSheet.h"
#include "SchSheetPin.h"

#include "Frontcad.h"

//------------------------------------------
#include "StaticPreview.h"
//------------ Dialog Boxes ----------------
#include "DlgArcProperties.h"
#include "DlgArcCentProperies.h"
#include "DlgArrowProperties.h"
#include "DlgDimensionPropertiex.h"
#include "DlgEllispeProperties.h"
#include "DlgLineAttributes.h"
#include "DlgPolygonProperties.h"
#include "DlgRectProperties.h"
#include "DlgRndRectProperties.h"
#include "DlgRndHole1Flat.h"
#include "DlgRndHole2FlatProp.h"
#include "DlgRoundHoleProp.h"
#include "DlgSquareHoleProp.h"
#include "DlgTextProperties.h"
#include "DlgBitmapProperties.h"
#include "DlgNewName.h"
#include "DlgObjectEnable.h"
#include "DlgNewDrawing.h"
#include "DlgSelectOrigin.h"
#include "DlgOriginAttributes.h"
#include "DlgPointProperties.h"
#include "DlgDoubleData.h"
#include "DlgSnapSelector.h"
#include "DlgSnapOrder.h"
//------------ Views -----------------------
#include "Grid.h"
#include "RulerInfo.h"
#include "RulerSplitterWindow.h"
#include "RulerCornerView.h"
#include "RulerView.h"
#include "MyToolBarSplitter.h"
#include "MyToolBarView.h"
#include "ChildFrameBase.h"
#include "FrontCadChildFrame.h"
#include "BaseDocument.h"
#include "DocumentFrontCad.h"
#include "ChildViewBase.h"
#include "FrontCadView.h"
#include "FrontCadChildFrame.h"


#endif //PCH_H
