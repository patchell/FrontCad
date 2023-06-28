#include "pch.h"

CCadRect::CCadRect():CCadObject(ObjectType::RECT)
{
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetRectangleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
	GetName().Format(_T("Rectangle_%d"), ++CCadRect::m_RectCount);
}

CCadRect::~CCadRect()
{
}

BOOL CCadRect::Create(CCadObject* pParent, CCadObject::SubTypes Type)
{
	//---------------------------------------
	// Create
	// add initial child objects
	//---------------------------------------
	int i;
	CCadPoint* pPoint;

	CCadObject::Create(pParent, Type);
	if (pParent == NULL)
		pParent = this;
	for (i = 0; i < 4; ++i)
	{
		pPoint = new CCadPoint;
		pPoint->Create(pParent, CCadObject::SubTypes::VERTEX);
		pPoint->SetSubSubType(i + 1);
		AddObjectAtTail(pPoint);
	}
	pPoint = new CCadPoint;
	pPoint->Create(pParent, CCadObject::SubTypes::CENTERPOINT);
	AddObjectAtTail(pPoint);
	switch (Type)
	{
	case CCadObject::SubTypes::RECT_ROTATED:
		pPoint = new CCadPoint;
		pPoint->Create(pParent, CCadObject::SubTypes::PIVOTPOINT);
		AddObjectAtTail(pPoint);
		pPoint = new CCadPoint;
		pPoint->Create(pParent, CCadObject::SubTypes::ROTATION_POINT);
		AddObjectAtTail(pPoint);
		break;
	case CCadObject::SubTypes::RECT_BASE_DEFINED:
		pPoint = new CCadPoint;
		pPoint->Create(pParent, CCadObject::SubTypes::PIVOTPOINT);
		AddObjectAtTail(pPoint);
		pPoint = new CCadPoint;
		pPoint->Create(pParent, CCadObject::SubTypes::ROTATION_POINT);
		AddObjectAtTail(pPoint);
		break;
	case CCadObject::SubTypes::DEFAULT:
		break;
	}
	return TRUE;
}

//----------------------------------------------------
// Set Rectangle Coordinates
//----------------------------------------------------

void CCadRect::SetRect(DOUBLEPOINT P1, DOUBLEPOINT P2)
{
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4;

	ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
	ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
	ObjP3.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 3);
	ObjP4.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 4);

	*ObjP1.pCadPoint = P1;
	*ObjP3.pCadPoint = P2;
	ObjP2.pCadPoint->SetX(P1.dX);
	ObjP2.pCadPoint->SetY(P2.dY - P1.dY);
	ObjP4.pCadPoint->SetX(P2.dX - P1.dX);
	ObjP4.pCadPoint->SetY(P1.dY);
}

void CCadRect::SetRect(CCadPoint* pP1, CCadPoint* pP2)
{
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4;

	ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
	ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
	ObjP3.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 3);
	ObjP4.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 4);

	*ObjP1.pCadPoint = *pP1;
	*ObjP3.pCadPoint = *pP2;
	ObjP2.pCadPoint->SetX(pP1->GetX());
	ObjP2.pCadPoint->SetY(pP2->GetY() - pP1->GetY());
	ObjP4.pCadPoint->SetX(pP2->GetX() - pP1->GetX());
	ObjP4.pCadPoint->SetY(pP1->GetY());
}

void CCadRect::SetRect(
	DOUBLEPOINT P1, 
	DOUBLEPOINT P2, 
	DOUBLEPOINT P3, 
	DOUBLEPOINT P4
)
{

	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4;

	ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
	ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
	ObjP3.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 3);
	ObjP4.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 4);

	*ObjP1.pCadPoint = P1;
	*ObjP2.pCadPoint = P2;
	*ObjP3.pCadPoint = P3;
	*ObjP4.pCadPoint = P4;
}

void CCadRect::SetRect(CCadPoint* pP1, CCadPoint* pP2, CCadPoint* pP3, CCadPoint* pP4)
{
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4;

	ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
	ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
	ObjP3.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 3);
	ObjP4.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 4);

	*ObjP1.pCadPoint = *pP1;
	*ObjP2.pCadPoint = *pP2;
	*ObjP3.pCadPoint = *pP3;
	*ObjP4.pCadPoint = *pP4;
}

void CCadRect::SetRect(DOUBLEPOINT P1, DOUBLEPOINT P2, double h)
{
	double m;
	UINT SlopeType;

	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4;

	ObjP1.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 1);
	ObjP2.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 2);
	ObjP3.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 3);
	ObjP4.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, 4);

	*ObjP1.pCadPoint = P1;
	*ObjP4.pCadPoint = P2;
	SlopeType = ObjP4.pCadPoint->OrthogonalSlope(&m, ObjP1.pCadPoint);
	switch (SlopeType)
	{
	case SLOPE_HORIZONTAL:
		break;
	case SLOPE_VERTICAL:
		break;
	case SLOPE_NOT_ORTHOGONAL:
		
		ObjP3.pCadPoint->PointOnLineAtDistance(ObjP4.pCadPoint, m, h);
		ObjP2.pCadPoint->PointOnLineAtDistance(ObjP1.pCadPoint, m, h);
		break;
	}
}


void CCadRect::SetRect(CCadPoint* pP1, CCadPoint* pP2, double h)
{
}

void CCadRect::Recalc24()
{
	//----------------------------------
	// Recalc24
	// Recalculate vertex 2 and 4 from
	// vertex 1 and 3
	//----------------------------------
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4;

	ObjP1.pCadObject = GetVertex(1);
	ObjP2.pCadObject = GetVertex(2);
	ObjP3.pCadObject = GetVertex(3);
	ObjP4.pCadObject = GetVertex(4);
	ObjP2.pCadPoint->SetPoint(ObjP1.pCadPoint->GetX(),ObjP3.pCadPoint->GetY());
	ObjP4.pCadPoint->SetPoint(ObjP3.pCadPoint->GetX(),ObjP1.pCadPoint->GetY());
	ReCalcCenter();
}

void CCadRect::Recalc34(double h)
{
	//--------------------------------------
	// Recalc34
	// 
	// Recalculate Points P3, P4 based on the
	// line defined by P1 and P2
	//--------------------------------------
	double m;
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4;
	UINT SlopeType;

	ObjP1.pCadObject = GetVertex(1);
	ObjP2.pCadObject = GetVertex(2);
	ObjP3.pCadObject = GetVertex(3);
	ObjP4.pCadObject = GetVertex(4);

	SlopeType = ObjP2.pCadPoint->OrthogonalSlope(&m, ObjP1.pCadPoint);
	switch (SlopeType)
	{
	case SLOPE_HORIZONTAL:
		ObjP3.pCadPoint->SetPoint(ObjP2.pCadPoint->GetX(), ObjP2.pCadPoint->GetY() + h);
		ObjP4.pCadPoint->SetPoint(ObjP2.pCadPoint->GetX(), ObjP2.pCadPoint->GetY() + h);
		break;
	case SLOPE_VERTICAL:
		ObjP3.pCadPoint->SetPoint(ObjP2.pCadPoint->GetX() + h, ObjP2.pCadPoint->GetY());
		ObjP4.pCadPoint->SetPoint(ObjP2.pCadPoint->GetX() + h, ObjP2.pCadPoint->GetY());
		break;
	case SLOPE_NOT_ORTHOGONAL:
		ObjP3.pCadPoint->PointOnLineAtDistance(ObjP2.pCadPoint, m, h);
		ObjP4.pCadPoint->PointOnLineAtDistance(ObjP1.pCadPoint, m, h);
		break;
	}
	ReCalcCenter();
}

void CCadRect::Recalc234()
{
	//----------------------------------------------
	// Recalc234
	// Calculates P2, P3, P4 based on P1 and the 
	// Center of the rectangle
	//----------------------------------------------
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4, ObjCenter;
	CDoubleSize Diff;

	ObjP1.pCadObject = GetVertex(1);
	ObjP2.pCadObject = GetVertex(2);
	ObjP3.pCadObject = GetVertex(3);
	ObjP4.pCadObject = GetVertex(4);
	ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
	Diff = *ObjP1.pCadPoint - *ObjCenter.pCadPoint;
	ObjP2.pCadPoint->SetPoint(ObjCenter.pCadPoint->GetX() - Diff.dCX, ObjCenter.pCadPoint->GetY() + Diff.dCY);
	ObjP3.pCadPoint->SetPoint(ObjCenter.pCadPoint->GetX() + Diff.dCX, ObjCenter.pCadPoint->GetY() + Diff.dCY);
	ObjP4.pCadPoint->SetPoint(ObjCenter.pCadPoint->GetX() + Diff.dCX, ObjCenter.pCadPoint->GetY() - Diff.dCY);
	ObjCenter.pCadPoint->CenterPoint(ObjP1.pCadPoint, ObjP3.pCadPoint);
}

void CCadRect::ReCalcCenter()
{
	CADObjectTypes ObjCenter;
	CADObjectTypes ObjP1, ObjP3;

	ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, 0);
	ObjP1.pCadObject = GetVertex(1);
	ObjP3.pCadObject = GetVertex(3);
	ObjCenter.pCadPoint->CenterPoint(ObjP1.pCadPoint, ObjP3.pCadPoint);
}

void CCadRect::SetAllPoints(DOUBLEPOINT p)
{
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4;

	ObjP1.pCadObject = GetVertex(1);
	ObjP2.pCadObject = GetVertex(2);
	ObjP3.pCadObject = GetVertex(3);
	ObjP4.pCadObject = GetVertex(4);

	ObjP1.pCadPoint->SetPoint(p);
	ObjP2.pCadPoint->SetPoint(p);
	ObjP3.pCadPoint->SetPoint(p);
	ObjP4.pCadPoint->SetPoint(p);
	ReCalcCenter();
}


void CCadRect::Move(CDoubleSize Diff)
{
	//---------------------------------------------------
	//	Move
	//		This Method is used to move the object
	// by the amount that is passed.
	//
	// parameters:
	//	p.......amount to move the object by
	//
	// return value: none
	//--------------------------------------------------
	CCadObject::Move(Diff);
}

void CCadRect::Save(FILE * pO, CLexer::Tokens Token, int Indent, int flags)
{
	//---------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
	char* psIndent = new char[256];
	CADObjectTypes Obj;

	;
	Obj.pCadObject = GetHead();
	fprintf(pO, "%s%s(\n",
		GETAPP.IndentString(psIndent, Indent, ' '),
		CLexer::TokenLookup(CLexer::Tokens::RECT)
	);
	fprintf(pO, "%s%s(\n",
		psIndent,
		CLexer::TokenLookup(Token)
	);
	while (Obj.pCadObject)
	{
		if (Obj.pCadObject->GetType() == ObjectType::POINT)
			Obj.pCadPoint->Save(pO, CLexer::Tokens::POINT, Indent + 1, flags);
		Obj.pCadObject = Obj.pCadObject->GetNext();
	}
	fprintf(pO, "%s)\n", psIndent);


	GetAttributes().Save(pO, Indent + 2, flags);
	fprintf(pO, "%s}\n", psIndent);
	delete []psIndent;
}

void CCadRect::Draw(
	CDC* pDC, 
	MODE mode, 
	DOUBLEPOINT& LLHC, 
	CScale& Scale
)
{
	//---------------------------------------------------
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	LLHC....Offset to draw objects at
	//	Scale..Scale factor to draw objects at
	//
	// return value:none
	//--------------------------------------------------

	if (IsRenderEnabled())
	{
		CPen* ppenOld, penLine;;
		CBrush* pbrushOld, brushFill;
		int Lw;
		CSize rectLWcomp;
		MODE pointMode = mode;;
		COLORREF colorBoarder;
		COLORREF colorFill;

		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.dSX);
		if (Lw < 1)
			Lw = 1;
		colorBoarder = CreateThePen(mode, &penLine, Lw);
		colorFill = CreateTheBrush(mode, &brushFill);
		ppenOld = pDC->SelectObject(&penLine);
		pbrushOld = pDC->SelectObject(&brushFill);
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
			DrawRect(
				pDC, 
				mode, 
				LLHC, 
				Scale, 
				GetAttributes().m_TransparentFill == FALSE
			);
			if (!GetAttributes().m_TransparentFill)
				FillRect(colorBoarder, colorFill, pDC, mode, LLHC, Scale);
			break;
		case MODE::ObjectPaintMode::SKETCH:
			//------------------------------------
			// This is going to be complicated
			// just because the rectangle is kind
			// of complicated all on its own
			//------------------------------------
			switch (GetCurrentDrawState())
			{
				//-------------------------------------
				// Just a regular ortoganol Rectangle
				//-------------------------------------
			case ObjectDrawState::PLACE_LBUTTON_DOWN:
				DrawRect(
					pDC,
					mode,
					LLHC,
					Scale,
					GetAttributes().m_TransparentFill == FALSE
				);
				break;
				//-----------------------------------------------
				// Rotated Rectangle Processing
				//-----------------------------------------------
			case ObjectDrawState::RECT_ROT_ROTATION_PIVOT_MOUSE_DOWN:
				break;
			case ObjectDrawState::RECT_ROT_FIRST_POINT_MOUSE_DOWN:
				break;
			case ObjectDrawState::RECT_ROT_SECOND_POINT_MOUSE_DOWN:
				break;
			case ObjectDrawState::RECT_ROT_ROTATE_POINT_MOUSE_DOWN:
				break;
				//------------------------------------------
				//-------- Rect Width/Height/Rotate --------
				// 
				// State machine for drawing a rotated
				// rectangle defined by a base and hiegth
				//------------------------------------------
			case ObjectDrawState::RECT_HWR_START_DRAWING:
				break;
			case ObjectDrawState::RECT_HWR_PIVOT_MOUSE_DOWN:
				break;
			case ObjectDrawState::RECT_HWR_BASE_FIRST_POINT_MOUSE_DOWN:
				break;
			case ObjectDrawState::RECT_HWR_BASE_SECOND_POINT_MOUSE_DOWN:
				break;
			case ObjectDrawState::RECT_HWR_HIEGTH_MOUSE_DOWN:
				break;
				//-----------------------------------------------------
				// Draw a Rectangle from the center point
				//-----------------------------------------------------
			case ObjectDrawState::RECT_CENT_START_DRAWING:
				break;
			case ObjectDrawState::RECT_CENT_PIVOT_MOUSE_DOWN:
				break;
			case ObjectDrawState::RECT_CENT_CENTER_MOUSE_DOWN:
				break;
			case ObjectDrawState::RECT_CENT_SECPMD_POINT_MOUSE_DOWN:
				break;
			case ObjectDrawState::RECT_CENT_ROTATION_POINT_MOUSE_DOWN:
				break;
			}
			break;
		}	//end of switch draw mode
		pDC->SelectObject(pbrushOld);
		pDC->SelectObject(ppenOld);
	}	//end of if render
}


void CCadRect::DrawRect(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale, BOOL bFill)
{
	int i;
	CADObjectTypes Obj;

	for (i = 0; i < 4; ++i)
	{
		if (i == 0)
		{
			Obj.pCadObject = FindObject(
				ObjectType::POINT,
				CCadObject::SubTypes::VERTEX,
				i + 1);
			if (Obj.pCadObject)
				Obj.pCadPoint->MoveTo(pDC, LLHC, Scale);
			else
				goto exit;	//error
		}
		else
		{
			Obj.pCadObject = FindObject(
				ObjectType::POINT,
				CCadObject::SubTypes::VERTEX,
				i + 1
			);
			if (Obj.pCadObject)
			{
				Obj.pCadPoint->LineTo(pDC, LLHC, Scale);
				if (mode.PaintMode != MODE::ObjectPaintMode::FINAL)
					Obj.pCadPoint->Draw(pDC, mode, LLHC, Scale);
			}
			else
				goto exit;	//error
		}
	}	//end of for loop
	Obj.pCadObject = FindObject(
		ObjectType::POINT,
		CCadObject::SubTypes::VERTEX,
		1
	);
	if (Obj.pCadObject)
	{
		Obj.pCadPoint->LineTo(pDC, LLHC, Scale);
		if (mode.PaintMode != MODE::ObjectPaintMode::FINAL)
			Obj.pCadPoint->Draw(pDC, mode, LLHC, Scale);
	}

exit:
	return;
}

void CCadRect::FillRect(
	COLORREF colorBoarder,
	COLORREF colorFill,
	CDC* pDC,
	MODE mode,
	DOUBLEPOINT& LLHC,
	CScale& Scale
)
{
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(
		ObjectType::POINT,
		CCadObject::SubTypes::CENTERPOINT,
		SUBSUBTYPE_ANY
	);
	if (Obj.pCadObject)
	{
		//		Obj.pCadPoint->Print("Rect Center Point");
		Obj.pCadPoint->FloodFill(
			colorBoarder,
			colorFill,
			pDC,
			LLHC,
			Scale
		);
	}
}

CCadPoint* CCadRect::GetRectPoints(CCadPoint** ppPointDest, int n)
{
	CADObjectTypes Obj;
	int index = 0;
	BOOL loop = TRUE;

	Obj.pCadObject = GetHead();

	while (Obj.pCadObject && loop)
	{
		if (Obj.pCadObject->GetType() == ObjectType::POINT)
		{
			ppPointDest[index++] = Obj.pCadPoint;
			if (index == 4)
				loop = FALSE;
		}
		Obj.pCadObject = Obj.pCadObject->GetNext();
	}
	return ppPointDest[0];
}

BOOL CCadRect::PointInThisObject(DOUBLEPOINT point)
{
	DOUBLEPOINT pointsRect[4];
	CADObjectTypes Vertex;
	int i;
	BOOL rV = FALSE;

	for (i = 0; i < 4; ++i)
	{
		Vertex.pCadObject =FindObject(
			ObjectType::POINT,
			CCadObject::SubTypes::VERTEX,
			i + 1
		);
		if (Vertex.pCadObject)
			pointsRect[i] = DOUBLEPOINT(*Vertex.pCadPoint);
		else
			goto exit;
	}
	rV = GETAPP.PtEnclosedInPolygon(point, pointsRect, 4);
exit:
	return rV;
}

int CCadRect::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject* pExcludeObject,
	CCadObject** ppSelList,
	int index,
	int n,
	UINT nKinds
)
{
	//---------------------------------------------------
	// PointInObjectAndSelect
	//	This Method is used to see if an object can
	// be selected at point p.
	//
	// parameters:
	//	p...........point to check at
	//	Offset......Offset of drawing
	//	ppSelList...pointer to list of selected objects
	//	index.......current index into the selection list
	//	n...........Total number of spaces in slection list
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------

	if (index < n)
	{
		//---------------------------------------
		// is point in the Arc?
		//---------------------------------------
		if (PointInThisObject(p))
		{
			if (IsItThisKind(nKinds) && DontExclude(pExcludeObject))
			{
				ppSelList[index++] = this;
				ShouldWeSelectThisObjectAndDidIt(nKinds);
			}
		}
		index = CCadObject::PointInObjectAndSelect(
			p,
			pExcludeObject,
			ppSelList,
			index,
			n,
			nKinds
		);
	}
	return index;
}

CString& CCadRect::GetTypeString(void)
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Rectangle");
	return csName;
}

CString& CCadRect::GetObjDescription()
{
	CCadPoint* pP1, * pP2;
	DOUBLEPOINT P1, P2;

	pP1 = (CCadPoint*)FindObject(
		ObjectType::POINT,
		CCadObject::SubTypes::VERTEX,
		1
	);
	pP2 = (CCadPoint*)FindObject(
		ObjectType::POINT,
		CCadObject::SubTypes::VERTEX,
		3
	);
	P1 = DOUBLEPOINT(*pP1);
	P2 = DOUBLEPOINT(*pP2);
	GetDescription().Format(_T("Rect((%6.3lf,%6.3lf),(%6.3lf,%6.3lf))"),
		P1.dX,P1.dY,
		P2.dX,P2.dY
	);
	return GetDescription();
}

CCadObject * CCadRect::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy.
	// This Method DOES not make a true copy in
	// that the pointers for linked lists are set
	// to NULL
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadRect *pRect = new CCadRect;
	*pRect = *this;
	return pRect;
}

CDoubleSize CCadRect::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	
	return CDoubleSize(GetWidth(), GetHeight());
}

double CCadRect::GetWidth()
{
	double Result;
	CCadPoint* pP1, * pP2;
	CCadObject *pObj;

	pObj = GetHead();
	pP1 = (CCadPoint*)pObj->FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_UL);
	pP2 = (CCadPoint*)pObj->FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_LR);
	Result = pP2->GetX() - pP1->GetX();
	return Result;
}

void CCadRect::SetWidth(double width)
{
	CADObjectTypes Obj;
	double Xref;

	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_UL);
	Xref = Obj.pCadPoint->GetX();
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_LR);
	Obj.pCadPoint->SetX(Xref + width);
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_UR);
	Obj.pCadPoint->SetX(Xref + width);
}

double CCadRect::GetHeight()
{
	double Result;
	CCadPoint* pP1, * pP2;
	CCadObject* pObj;

	pObj = GetHead();
	pP1 = (CCadPoint*)pObj->FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_UL);
	pP2 = (CCadPoint*)pObj->FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_LR);
	Result = pP2->GetY() - pP1->GetY();
	return Result;
}

void CCadRect::SetHeight(double Height)
{
	CADObjectTypes Obj;
	double Yref;

	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_UL);
	Yref = Obj.pCadPoint->GetY();
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_LR);
	Obj.pCadPoint->SetY(Yref + Height);
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::VERTEX, RECT_SUBSUB_LL);
	Obj.pCadPoint->SetY(Yref + Height);
}

CLexer::Tokens CCadRect::Parse(
	CLexer::Tokens Token,	// Lookahead Token
	CFileParser* pParser,	// pointer to parser
	CLexer::Tokens TypeToken// Token type to save object as
)
{
	//---------------------------------------------------
	// Parse
	//	This Method is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token....Token for this object
	//	pLex.....Pointer to Lexer for the data stream
	//
	// return value:
	//	returns lookahead token on success, or
	//			negative value on error
	//--------------------------------------------------
	Token = pParser->Expect(Token, CLexer::Tokens::RECT);
	Token = pParser->Expect(Token, CLexer::Tokens('('));
//	Token = ???.CadRect(CLexer::Tokens::RECT, *this, Token);
	Token = GetAttributes().Parse(Token, pParser);
	Token = pParser->Expect(Token, CLexer::Tokens(')') );
	return Token;
}

void CCadRect::CopyAttributesTo(SRectAttributes *pAttrib)
{
	//---------------------------------------------------
	//	CopyAttributesTo
	//		This Method is used to copy the
	//	attributes from this object into one pointed
	//	to by the parameter.
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy into
	//-------------------------------------------------/
	GetAttributes().CopyTo(pAttrib);
}


void CCadRect::CopyAttributesFrom(SRectAttributes *pAttrib)
{
	//---------------------------------------------------
	//	CopyAttributesFrom
	//		This Method is used to copy the
	//	attributes pointed to by the parameter into
	//	this object
	//
	// Parameters:
	//	pAttrb.....pointer to attributes structure to copy
	//---------------------------------------------------/
	GetAttributes().CopyFrom(pAttrib);
	SetAttributesValid();
}

ObjectDrawState CCadRect::ProcessDrawMode(ObjectDrawState DrawState)
{
	//-------------------------------------------------------
	//	ProcessDrawMode
	//		This is the state machine for creating this
	//	object on the screen.  
	//
	//	parameters:
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	UINT Id;
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4, ObjCenter;
	CADObjectTypes ObjPivot, ObjRotation;
	CADObjectTypes ObjRect;
	CPoint MouseScreenCoordinate;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		if (GetSubType() == CCadObject::SubTypes::DEFAULT)
			DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		else if (GetSubType() == CCadObject::SubTypes::RECT_ROTATED)
			DrawState = ObjectDrawState::RECT_ROT_ROTATION_PIVOT_MOUSE_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Place First Point"));
		break;
	case ObjectDrawState::END_DRAWING:
		if (m_AttributesDirty)
		{
			Id = GETVIEW->MessageBoxW(_T("Do you want to keep\nThe current\nAttributes?"), _T("Keep Or Toss"), MB_YESNO);
			if (IDYES == Id)
			{
				m_CurrentAttributes.CopyTo(&m_LastAttributes);
			}
			m_AttributesDirty = FALSE;
		}
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		GETVIEW->GetCursorPosition(&MouseScreenCoordinate);
		Id = EditProperties();
		if (IDOK == Id)
		{
			if (m_AttributesDirty)
				CopyAttributesTo(&m_CurrentAttributes);
			DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
			GETVIEW->EnableAutoScroll(1);
		}
		GETVIEW->SetCursorPosition(MouseScreenCoordinate);
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		GETVIEW->EnableAutoScroll(TRUE);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		SetAllPoints(MousePos);
		DrawState = ObjectDrawState::PLACE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Set Second Point"));
		break;
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::PLACE_LBUTTON_UP;
		break;
	case ObjectDrawState::PLACE_LBUTTON_UP:
		ObjP3.pCadObject = GetVertex(3);
		ObjP3.pCadPoint->SetPoint(MousePos);
		Recalc24();
		ObjCenter.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::CENTERPOINT, SUBSUBTYPE_ANY);

		//----------------------------------------------
		// Just a regular orthoganol Rectangle
		//---------------------------------------------
		GetParent()->AddObjectAtTail(this);
		ObjRect.pCadRect = new CCadRect;
		ObjRect.pCadRect->Create(GetParent(), GetSubType() );
		GETVIEW->EnableAutoScroll(FALSE);
		GETVIEW->SetObjectTypes(ObjRect.pCadObject);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangle:Place First Point"));
		GETVIEW->Invalidate();
		break;
		//---------------------------------------------
		//-------- Rotated Rectangle States Mode 1 ----
		// 
		// State machine for drawing a rotated
		// rectasngle
		//---------------------------------------------
	case ObjectDrawState::RECT_ROT_ROTATED_START_DRAWING:
		break;
	case ObjectDrawState::RECT_ROT_ROTATION_PIVOT_MOUSE_DOWN:
		DrawState = ObjectDrawState::RECT_ROT_ROTATION_PIVOT_MOUSE_UP;
		break;
	case ObjectDrawState::RECT_ROT_ROTATION_PIVOT_MOUSE_UP:
		ObjPivot.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::PIVOTPOINT, SUBSUBTYPE_ANY);
		ObjPivot.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::RECT_ROT_FIRST_POINT_MOUSE_DOWN;
		break;
	case ObjectDrawState::RECT_ROT_FIRST_POINT_MOUSE_DOWN:
		DrawState = ObjectDrawState::RECT_ROT_FIRST_POINT_MOUSE_UP;
		break;
	case ObjectDrawState::RECT_ROT_FIRST_POINT_MOUSE_UP:
		DrawState = ObjectDrawState::RECT_ROT_SECOND_POINT_MOUSE_DOWN;
		break;
	case ObjectDrawState::RECT_ROT_SECOND_POINT_MOUSE_DOWN:
		DrawState = ObjectDrawState::RECT_ROT_SECOND_POINT_MOUSE_UP;
		break;
	case ObjectDrawState::RECT_ROT_SECOND_POINT_MOUSE_UP:
		DrawState = ObjectDrawState::RECT_ROT_ROTATE_POINT_MOUSE_DOWN;
		break;
	case ObjectDrawState::RECT_ROT_ROTATE_POINT_MOUSE_DOWN:
		DrawState = ObjectDrawState::RECT_ROT_ROTATE_POINT_MOUSE_UP;
		break;
	case ObjectDrawState::RECT_ROT_ROTATE_POINT_MOUSE_UP:
		DrawState = ObjectDrawState::RECT_ROT_ROTATION_PIVOT_MOUSE_DOWN;
		break;
	//------------------------------------------
	//-------- Rect Width/Height/Rotate --------
	// 
	// State machine for drawing a rotated
	// rectangle defined by a base and hiegth
	//------------------------------------------
	case ObjectDrawState::RECT_HWR_START_DRAWING:
		break;
	case ObjectDrawState::RECT_HWR_PIVOT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_HWR_PIVOT_MOUSE_UP:
		break;
	case ObjectDrawState::RECT_HWR_BASE_FIRST_POINT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_HWR_BASE_FIRST_POINT_MOUSE_UP:
		break;
	case ObjectDrawState::RECT_HWR_BASE_SECOND_POINT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_HWR_BASE_SECOND_POINT_MOUSE_UP:
		break;
	case ObjectDrawState::RECT_HWR_HIEGTH_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_HWR_HIEGTH_MOUSE_UP:
		break;
	//---------------------------------------------
	// Draw a Rectangle form the center
	//---------------------------------------------
	case ObjectDrawState::RECT_CENT_START_DRAWING:
		break;
	case ObjectDrawState::RECT_CENT_PIVOT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_CENT_PIVOT_MOUSE_UP:
		break;
	case ObjectDrawState::RECT_CENT_CENTER_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_CENT_CENTER_MOUSE_UP:
		break;
	case ObjectDrawState::RECT_CENT_SECPMD_POINT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_CENT_SECOND_POINT_MOUSE_UP:
		break;
	case ObjectDrawState::RECT_CENT_ROTATION_POINT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_CENT_ROTATION_POINT_MOUSE_UP:
		break;
	}
	return DrawState;
}

ObjectDrawState CCadRect::MouseMove(ObjectDrawState DrawState)
{
	//-------------------------------------------------------
	// MouseMove
	//		This is the state machine for creating this
	//	object on the screen.  This Method is for when
	//	the left mouse is moved.
	//
	//	parameters:
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();
	CADObjectTypes ObjP1, ObjP2, ObjP3, ObjP4, ObjPiv, ObjRot, ObjCenter;

	switch (DrawState)
	{
	case ObjectDrawState::PLACE_LBUTTON_DOWN:
		ObjP2.pCadObject = GetVertex(3);
		ObjP2.pCadPoint->SetPoint(MousePos);
		Recalc24();
		break;
		//-----------------------------------------------
		// Rotated Rectangle Processing
		//-----------------------------------------------
	case ObjectDrawState::RECT_ROT_ROTATION_PIVOT_MOUSE_DOWN:
		ObjPiv.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::PIVOTPOINT, SUBSUBTYPE_ANY);
		ObjPiv.pCadPoint->SetPoint(MousePos);
		break;
	case ObjectDrawState::RECT_ROT_FIRST_POINT_MOUSE_DOWN:
		ObjRot.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::PIVOTPOINT, SUBSUBTYPE_ANY);
		ObjRot.pCadPoint->SetPoint(MousePos);
		break;
	case ObjectDrawState::RECT_ROT_SECOND_POINT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_ROT_ROTATE_POINT_MOUSE_DOWN:
		break;
		//------------------------------------------
		//-------- Rect Width/Height/Rotate --------
		// 
		// State machine for drawing a rotated
		// rectangle defined by a base and hiegth
		//------------------------------------------
	case ObjectDrawState::RECT_HWR_START_DRAWING:
		break;
	case ObjectDrawState::RECT_HWR_PIVOT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_HWR_BASE_FIRST_POINT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_HWR_BASE_SECOND_POINT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_HWR_HIEGTH_MOUSE_DOWN:
		break;
	//-----------------------------------------------------
	// Draw a Rectangle from the center point
	//-----------------------------------------------------
	case ObjectDrawState::RECT_CENT_START_DRAWING:
		break;
	case ObjectDrawState::RECT_CENT_PIVOT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_CENT_CENTER_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_CENT_SECPMD_POINT_MOUSE_DOWN:
		break;
	case ObjectDrawState::RECT_CENT_ROTATION_POINT_MOUSE_DOWN:
		break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}

int CCadRect::EditProperties()
{
	int Id;
	CDlgRectProperties Dlg;

	Dlg.SetRectangle(this);
	Id = Dlg.DoModal();
	return Id;
}

COLORREF CCadRect::CreateThePen(MODE mode, CPen* pen, int Lw)
{
	COLORREF rColor = RGB(192, 192, 192);

	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if (IsSelected())
		{
			rColor = GetAttributes().m_colorSelected;
			pen->CreatePen(PS_SOLID, Lw, rColor);
		}
		else
		{
			rColor = GetAttributes().m_colorLine;
			pen->CreatePen(PS_SOLID, Lw, rColor);
		}
		break;
	case MODE::ObjectPaintMode::SKETCH:
		rColor = GetAttributes().m_colorSelected;
		pen->CreatePen(PS_DOT, Lw, rColor);
		break;
	}

	return rColor;
}

COLORREF CCadRect::CreateTheBrush(MODE mode, CBrush* brushFill)
{
	COLORREF rV = RGB(0, 0, 0);;

	switch (mode.PaintMode)
	{
	case MODE::ObjectPaintMode::FINAL:
		if (IsSelected())
		{
			rV = GetAttributes().m_colorFill;
			brushFill->CreateSolidBrush(rV);
		}
		else
		{
			rV = GetAttributes().m_colorFill;
			brushFill->CreateSolidBrush(rV);
		}
		break;
	case MODE::ObjectPaintMode::SKETCH:
	{
		rV = GetAttributes().m_colorFill;
		brushFill->CreateSolidBrush(rV);
	}
	break;
	}
	return rV;
}

