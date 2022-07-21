//----------------------------------------------------------------------------------------------------------
// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.
//----------------------------------------------------------------------------------------------------------

#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <afxwin.h>
#include <math.h>
#include <limits>
#include "afxdialogex.h"
#include <afxmt.h>

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
#include "Lexer.h"
#include "ComboBoxFontWeight.h"
#include "MyBitmap.h"
#include "StaticDouble.h"
//-------------------------------------------------
#include "MainFrm.h"
#include "Clipboard.h"
#include "Frontcad.h"
//-------------- Drawing Objects ------------------
#include "CadObject.h"
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
//------------ Views -----------------------
#include "MyPopupMenu.h"
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
