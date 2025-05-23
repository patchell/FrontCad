#include "pch.h"

CCadArrow::CCadArrow():CCadObject(ObjectType::ARROW)
{
	GetName().Format(_T("Arrow%d"), ++CCadArrow::m_ArrowCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetArrowAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadArrow::~CCadArrow()
{
}

BOOL CCadArrow::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;

	CCadObject::Create(pParent, type);
	if (pParent == NULL)
		pParent = this;
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ARROW_TIP);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ARROW_TOP);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ARROW_END);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ARROW_BOT);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::ARROW_ROTATION);
	AddObjectAtTail(Obj.pCadObject);

	return TRUE;
}

void CCadArrow::UpdateEnclosure()
{
}

void CCadArrow::Move(CDoubleSize Diff)
{
	//--------------------------------------------------
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

void CCadArrow::Save(
	CFile* pcfFile, 
	int Indent
)
{
	//--------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
}

void CCadArrow::Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
{
	//--------------------------------------------------
	// Draw
	//	This Method draws the document to the device
	// parameters:
	//	pDC.....pointer to the device context
	//	mode....drawing mode
	//	Offset..Offset to draw objects at
	//	Scale..Scale factor to draw objects at
	//
	// return value:none
	//--------------------------------------------------
	CPen *pOldPen,penLine;
	CBrush *pOldBr, brushFill;
	CRect rect;
	CPoint ArrowPoints[4];
	CADObjectTypes TIP, TOP, END, BOT, ROT;
	int Lw;

	if (IsRenderEnabled())
	{
		MakeCPointArray(ArrowPoints, pDC, mode, LLHC, Scale);
		Lw = GETAPP.RoundDoubleToInt(GetAttributes().m_LineWidth * Scale.GetScaleX());
		if (Lw < 1)
			Lw = 1;
		TIP.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_TIP, 0);
		TOP.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_TOP, 0);
		END.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_END, 0);
		BOT.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_END, 0);
		ROT.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_ROTATION, 0);
		switch (mode.PaintMode)
		{
		case MODE::ObjectPaintMode::FINAL:
			if (IsSelected())
			{
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorSelected);
				brushFill.CreateStockObject(NULL_BRUSH);
			}
			else
			{
				penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
				brushFill.CreateSolidBrush(GetAttributes().m_colorFill);
			}
			pOldPen = pDC->SelectObject(&penLine);
			pOldBr = pDC->SelectObject(&brushFill);
			pDC->Polygon(ArrowPoints, 4);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		case MODE::ObjectPaintMode::SKETCH:
			penLine.CreatePen(PS_SOLID, 1, GetAttributes().m_colorSelected);
			pOldPen = pDC->SelectObject(&penLine);
			brushFill.CreateStockObject(NULL_BRUSH);
			pOldBr = pDC->SelectObject(&brushFill);
			pDC->Polygon(ArrowPoints, 4);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBr);
			break;
		}
	}
}

BOOL CCadArrow::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

BOOL CCadArrow::PointInThisObject(DOUBLEPOINT point)
{
	DOUBLEPOINT Points[4];
	CADObjectTypes Obj;
	BOOL rV;

	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_TIP, 0);
	Points[ARROW_TIP] = DOUBLEPOINT(*Obj.pCadPoint);
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_TOP, 0);
	Points[ARROW_TOP] = DOUBLEPOINT(*Obj.pCadPoint);
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_END, 0);
	Points[ARROW_BACK] = DOUBLEPOINT(*Obj.pCadPoint);
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_BOT, 0);
	Points[ARROW_BOT] = DOUBLEPOINT(*Obj.pCadPoint);
	rV = GETAPP.PtEnclosedInPolygon(point, Points,4);
	return rV;
}

int CCadArrow::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject* pExcludeObject,
	CCadObject** ppSelList,
	int index,
	int n,
	UINT nKinds
)
{
	//--------------------------------------------------
	// PointInObjectAndSelect
	//	This Method is used to see if an object can
	// be selected at point p.
	//
	// parameters:
	//	p...........point to check at
	//	Offset......Offset of drawing
	//	ppSelList...pointer to list of selected objects
	//	index.......current index into the selection list
	//	n...........Total number of spaces in selection list
	//
	// return value:
	//	returns true if point is within object
	//	otherwise, false
	//--------------------------------------------------

	if (index < n || n == 0)
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

CString& CCadArrow::GetTypeString()
{
	//--------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = CString(_T("Arrow"));
	return csName;
}

CString& CCadArrow::GetObjDescription()
{
	GetDescription().Format(_T("Base Obj Class"));
	return GetDescription();
}

CCadObject * CCadArrow::Copy()
{
	//--------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadArrow *pArrow = new CCadArrow;
	pArrow->Create(GetParent(), GetSubType());
	CCadObject::CopyObject(pArrow);
	return pArrow;
}

void CCadArrow::CopyAttributes(CCadObject* pToObj)
{
	((CCadArrow*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}

CDoubleSize CCadArrow::GetSize()
{
	//---------------------------------------
	// GetSize
	// 
	// Get the size of the rectangle that
	// encloses the arrow
	//---------------------------------------
	double MinX, MaxX, MinY, MaxY;
	CDoubleSize Size;

	GETAPP.GetPolyMinMax(GetAttributes().m_aArrowShape, 4, MinX, MaxX, MinY, MaxY);
	Size = CDoubleSize(MaxX - MinX, MinY - MaxY);
	return Size;
}


void CCadArrow::Parse(
	CParser* pParser,	// pointer to parser
	Token TypeToken// Token type to save object as
)
{
	//--------------------------------------------------
	// Parse
	//	This Method is used to parse this 
	// object out of an input stream
	//
	// parameters:
	//	Token......Token for this object
	//	pLex.......Pointer to Lexer for the data stream
	//	TypeToken..Alternative type other than the default
	// 
	// return value:
	//	returns lookahead token on success, or
	//			throws exception on error
	//--------------------------------------------------
}

void CCadArrow::CopyAttributesTo(SArrowAttributes *pAttrib)
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

void CCadArrow::CopyAttributesFrom(SArrowAttributes *pAttrib)
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

ObjectDrawState CCadArrow::ProcessDrawMode(ObjectDrawState DrawState)
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
	CADObjectTypes Obj;

	switch (DrawState)
	{
	case ObjectDrawState::START_DRAWING:
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
		CopyAttributesFrom(&m_CurrentAttributes);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETVIEW->EnableAutoScroll(TRUE);
		GETAPP.UpdateStatusBar(_T("Arrow:Locate Arrow Tip Point"));
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
		GETVIEW->EnableAutoScroll(FALSE);
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		Id = EditProperties();
		if (IDOK == Id)
		{
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_TIP, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_ROTATION, 0);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::ROTATE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Place Rotation Point"));
		break;
	case ObjectDrawState::ROTATE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::ROTATE_LBUTTON_UP;
		break;
	case ObjectDrawState::ROTATE_LBUTTON_UP:
		Rotate(MousePos);
		GetParent()->AddObjectAtTail(this);
		Obj.pCadArrow = new CCadArrow;
		Obj.pCadArrow->Create(GetParent(), GetSubType());
		GETVIEW->SetObjectTypes(Obj.pCadObject);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Arrow:Locate Arrow Tip Point"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}



ObjectDrawState CCadArrow::MouseMove(ObjectDrawState DrawState)
{
	//-------------------------------------------------------
	// MouseMove
	//		This is the state machine for creating this
	//	object on the screen.  This Method is for when
	//	the left mouse is moved.
	//
	//	parameters:
	//		pASV......pointer to view that is creating object
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	DOUBLEPOINT MousePos = GETVIEW->GetCurrentMousePosition();

	switch (DrawState)
	{
		case ObjectDrawState::ROTATE_LBUTTON_DOWN:
			Rotate(MousePos);
			break;
	}
	GETVIEW->Invalidate();
	return DrawState;
}

void CCadArrow::Rotate(DOUBLEPOINT MousePos)
{
	CADObjectTypes TIP, TOP, END, BOT, ROT;
	double m;
	double Dist;
	UINT SlopeType;
	CCadPoint pointExtent;


	TIP.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_TIP, 0);
	TOP.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_TOP, 0);
	END.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_END, 0);
	BOT.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_BOT, 0);
	//------------------------------------------------------------
	// This point defines the angle that the arrow is 
	ROT.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_ROTATION, 0);
	ROT.pCadPoint->SetPoint(MousePos);
	//------------------------------------
	// rotate the End point
	//------------------------------------
	SlopeType = TIP.pCadPoint->Slope(&m, ROT.pCadPoint);
	switch (SlopeType)
	{
	case SLOPE_HORIZONTAL:
		if (MousePos.dX > TIP.pCadPoint->GetX())
		{
			TOP.pCadPoint->SetPoint(*TIP.pCadPoint + GetAttributes().m_aArrowShape[ARROW_TOP]);
			END.pCadPoint->SetPoint(*TIP.pCadPoint + GetAttributes().m_aArrowShape[ARROW_BACK]);
			BOT.pCadPoint->SetPoint(*TIP.pCadPoint + GetAttributes().m_aArrowShape[ARROW_BOT]);
		}
		else
		{
			TOP.pCadPoint->SetPoint(*TIP.pCadPoint - GetAttributes().m_aArrowShape[ARROW_TOP]);
			END.pCadPoint->SetPoint(*TIP.pCadPoint - GetAttributes().m_aArrowShape[ARROW_BACK]);
			BOT.pCadPoint->SetPoint(*TIP.pCadPoint - GetAttributes().m_aArrowShape[ARROW_BOT]);
		}
		break;
	case SLOPE_VERTICAL:
		if (MousePos.dY < TIP.pCadPoint->GetY())
		{
			TOP.pCadPoint->SetPoint(*TIP.pCadPoint - DOUBLEPOINT(
				GetAttributes().m_aArrowShape[ARROW_TOP].dY,
				GetAttributes().m_aArrowShape[ARROW_TOP].dX	)
			);
			BOT.pCadPoint->SetPoint(*TIP.pCadPoint - DOUBLEPOINT(
				GetAttributes().m_aArrowShape[ARROW_BOT].dY,
				GetAttributes().m_aArrowShape[ARROW_BOT].dX	)
			);
			END.pCadPoint->SetPoint(*TIP.pCadPoint - DOUBLEPOINT(
				GetAttributes().m_aArrowShape[ARROW_BACK].dY,
				GetAttributes().m_aArrowShape[ARROW_BACK].dX)
			);
		}
		else
		{
			TOP.pCadPoint->SetPoint(*TIP.pCadPoint + DOUBLEPOINT(
				GetAttributes().m_aArrowShape[ARROW_TOP].dY,
				GetAttributes().m_aArrowShape[ARROW_TOP].dX
				)
			);
			BOT.pCadPoint->SetPoint(*TIP.pCadPoint + DOUBLEPOINT(
				GetAttributes().m_aArrowShape[ARROW_BOT].dY,
				GetAttributes().m_aArrowShape[ARROW_BOT].dX
			)
			);
			END.pCadPoint->SetPoint(*TIP.pCadPoint + DOUBLEPOINT(
				GetAttributes().m_aArrowShape[ARROW_BACK].dY,
				GetAttributes().m_aArrowShape[ARROW_BACK].dX
				)
			);
		}
		break;
	case SLOPE_NOT_ORTHOGONAL:
		//------------------------------------------
		// Rotate the top and bottom points
		// These two points will be on a line that
		// goes through a point that is on the axis
		// of the arrow.  Confused?  So am I
		//------------------------------------------
		Dist = GetAttributes().m_aArrowShape[ARROW_BACK].dX - GetAttributes().m_aArrowShape[ARROW_TIP].dX;
		END.pCadPoint->PointOnLineAtDistance(TIP.pCadPoint, m, Dist);
		Dist = GetAttributes().m_aArrowShape[ARROW_TOP].dX - GetAttributes().m_aArrowShape[ARROW_TIP].dX;
		pointExtent.PointOnLineAtDistance(TIP.pCadPoint, m, Dist);
		Dist = GetAttributes().m_aArrowShape[ARROW_TIP].dY - GetAttributes().m_aArrowShape[ARROW_TOP].dY;
		m = -1.0 / m;
		TOP.pCadPoint->PointOnLineAtDistance(&pointExtent, m, Dist);
		BOT.pCadPoint->PointOnLineAtDistance(&pointExtent, m, -Dist);
		if (MousePos.dX < TIP.pCadPoint->GetX())
		{
			END.pCadPoint->Reflect(TIP.pCadPoint, POINT_REFLECT_BOTH);
			TOP.pCadPoint->Reflect(TIP.pCadPoint, POINT_REFLECT_BOTH);
			BOT.pCadPoint->Reflect(TIP.pCadPoint, POINT_REFLECT_BOTH);
		}
		break;
	}
}

int CCadArrow::EditProperties()
{
	int Id;
	CDlgArrowProperties Dlg;

	Dlg.SetArrow(this);
	Id = Dlg.DoModal();
	return Id;
}

void CCadArrow::MakeCPointArray(CPoint* PolyPoints, CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
{
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_TIP, 0);
	PolyPoints[ARROW_TIP] = Obj.pCadPoint->ToPixelPoint(LLHC, Scale);
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_TOP, 0);
	PolyPoints[ARROW_TOP] = Obj.pCadPoint->ToPixelPoint(LLHC, Scale);
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_END, 0);
	PolyPoints[ARROW_BACK] = Obj.pCadPoint->ToPixelPoint(LLHC, Scale);
	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::ARROW_BOT, 0);
	PolyPoints[ARROW_BOT] = Obj.pCadPoint->ToPixelPoint(LLHC, Scale);
}

//-------------------------------------------
// Methods used to edit the arrow shape
// in the object attributes
//-------------------------------------------
double CCadArrow::GetL1()
{
	double L1;
	double TipX, TopX;
	TipX = GetAttributes().m_aArrowShape[ARROW_TIP].dX;
	TopX = GetAttributes().m_aArrowShape[ARROW_TOP].dX;
	L1 = TipX - TopX;
	return fabs(L1);
}

double CCadArrow::GetL2()
{
	double L2;
	double TipX, EndX;
	TipX = GetAttributes().m_aArrowShape[ARROW_TIP].dX;
	EndX = GetAttributes().m_aArrowShape[ARROW_BACK].dX;
	L2 = TipX - EndX;
	return fabs(L2);
}

double CCadArrow::GetW()
{
	double W;
	double TopY, BotY;

	TopY = GetAttributes().m_aArrowShape[ARROW_TOP].dY;
	BotY = GetAttributes().m_aArrowShape[ARROW_BOT].dY;
	W = TopY - BotY;
	return fabs(W);
}

void CCadArrow::ApplyParameters(double L1, double L2, double W)
{
	//------------------------------------
	// ApplyParameters
	//	This method is used by the 
	// properties editor to define the
	// shape of the Arrow object.
	// 
	// Parameters:
	//	L1.....Over Length
	//	L2.....Length to bend from tip
	//	W......Width, opposite tip
	//------------------------------------
	GetAttributes().m_aArrowShape[ARROW_TIP] = DOUBLEPOINT(0, 0);
	GetAttributes().m_aArrowShape[ARROW_TOP] = DOUBLEPOINT(L1, W / 2.0);
	GetAttributes().m_aArrowShape[ARROW_BACK] = DOUBLEPOINT(L2, 0.0);
	GetAttributes().m_aArrowShape[ARROW_BOT] = DOUBLEPOINT(L1, -W / 2.0);
}

