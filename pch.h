//----------------------------------------------------------------------------------------------------------
// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.
//----------------------------------------------------------------------------------------------------------

#ifndef PCH_H
#define PCH_H

enum {
	TOKEN_DEFAULT = -2,
	TOKEN_EOFToken = -1,
	TOKEN_BASE_OBJ,
	TOKEN_MATCHED,
	TOKEN_NUMINT = 256,
	TOKEN_NUMDOUBLE,
	TOKEN_STRING,			//1
	//--------------------------------
	// Drawing Objects
	//--------------------------------
	TOKEN_ARC,				//2
	TOKEN_ARC_CENTER,		//3
	TOKEN_ARROW,
	TOKEN_BITMAP,
	TOKEN_CIRCLE,			//4
	TOKEN_DIMENSION,
	TOKEN_RECT,				//5
	TOKEN_RNDRECT,			//6
	TOKEN_POLY,				//7
	TOKEN_LINE,				//9
	TOKEN_HOLERND,			//10
	TOKEN_HOLERECT,			//11
	TOKEN_HOLE_1FLAT,		//12
	TOKEN_HOLE_2FLAT,		//13
	TOKEN_LIBPART,			//14
	TOKEN_ORG,	//token for origin object
	TOKEN_POINT,
	TOKEN_PRINTRECT,
	TOKEN_TEXT,				//15
	TOKEN_VERTEX_POINT,
	TOKEN_START_POINT,
	TOKEN_END_POINT,
	//colors
	TOKEN_BK_TRANSPARENT,
	TOKEN_BACKGROUND_COLOR,	//17
	TOKEN_LINE_COLOR,
	TOKEN_TEXT_COLOR,
	TOKEN_FILL_COLOR,
	TOKEN_SELECTED_COLOR,
	TOKEN_RED,
	TOKEN_GREEN,
	TOKEN_BLUE,
	//-------
	TOKEN_FONT,
	TOKEN_FONT_WEIGHT,
	TOKEN_FONTHEIGHT,
	TOKEN_FONTWIDTH,
	TOKEN_FONT_FORMAT,
	TOKEN_FONT_NAME,
	//--------------------
	TOKEN_RADIUS,
	TOKEN_FLATDIST,
	TOKEN_VERTEX,
	TOKEN_RECTWIDTH,
	TOKEN_RECTHIEGHT,
	TOKEN_ANGLE,
	TOKEN_REFERENCE,
	TOKEN_FILENAME,
	TOKEN_LENGTH,
	TOKEN_SIZE,
	TOKEN_SCALE,
	TOKEN_LINE_WIDTH,
	TOKEN_ARROW_LENGTH,
	TOKEN_ARROW_WIDTH,
	TOKEN_SELECTED_UNITS,
	//-------------------
	TOKEN_DRAWFILE,
	TOKEN_LIBFILE,
	// --- Attributes ---
	TOKEN_ATTRIB_ARC,
	TOKEN_ATTRIB_ARCCENTER,
	TOKEN_ATTRIB_ARROW,
	TOKEN_ATTRIB_BITMAP,
	TOKEN_ATTRIB_DIMENSION,
	TOKEN_ATTRIB_ELLIPSE,
	TOKEN_ATTRIB_GRID,
	TOKEN_ATTRIB_HOLERECT,
	TOKEN_ATTRIB_HOLERND1F,
	TOKEN_ATTRIB_HOLERND2F,
	TOKEN_ATTRIB_HOLERND,
	TOKEN_ATTRIB_LINE,
	TOKEN_ATTRIB_ORIGIN,
	TOKEN_ATTRIB_POINT,
	TOKEN_ATTRIB_POLYGON,
	TOKEN_ATTRIB_RECT,
	TOKEN_ATTRIB_RNDRECT,
	TOKEN_ATTRIB_RULERS,
	TOKEN_ATTRIB_TEXT,
	TOKEN_ERRORToken
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

#include "FileParser.h"

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
#include "FrontCadDoc.h"
#include "ChildViewBase.h"
#include "FrontCadView.h"
#include "FrontCadChildFrame.h"


#endif //PCH_H
