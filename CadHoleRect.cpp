#include "pch.h"


CCadHoleRect::CCadHoleRect():CCadObject()
{
	SetType(ObjectType::HOLE_RECTANGLE);
	GetName().Format(_T("RectangleHole_%d"), ++m_RectHoleCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetRectHoleAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}
		
CCadHoleRect::~CCadHoleRect()
{
}

BOOL CCadHoleRect::Create(CCadObject* pParent, CCadObject* pOrigin)
{
	CADObjectTypes Obj;

	if (pParent == NULL)
		pParent = this;
	Obj.pCadRect = new CCadRect;
	Obj.pCadRect->Create(pParent, pOrigin);
	Obj.pCadRect->SetSubType(SubType::RECTSHAPE);
	Obj.pCadRect->SetSubSubType(0);
	Obj.pCadRect->GetAttributes().m_colorLine = GetAttributes().m_colorLine;
	Obj.pCadRect->GetAttributes().m_LineWidth = GetAttributes().m_LineWidth;
	Obj.pCadRect->GetAttributes().m_TransparentFill = TRUE;
	Obj.pCadRect->SetPointsFromCenter(GetAttributes().m_HoleWidth / 2.0, GetAttributes().m_HoleHeight / 2.0);
	AddObjectAtChildTail(Obj.pCadObject);
	return 0;
}

BOOL CCadHoleRect::Destroy(CCadObject* pDependentObjects)
{
	return 0;
}

void CCadHoleRect::Move(CDoubleSize Diff)
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

void CCadHoleRect::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
{
	//---------------------------------------------------
	// Save
	//		This Method save the document
	// parameters:
	//	pO......pointer to output stream to save file to
	//
	// return value:none
	//--------------------------------------------------
}

void CCadHoleRect::Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
{
	//---------------------------------------------------
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
	CPen* pOldPen, penLine;
	CADObjectTypes Obj;
	CADObjectTypes ObjCenter;
	double Delta = 0.075;
	DOUBLEPOINT CENTER;
	int Lw;


	if (IsRenderEnabled())
	{
		Lw = GETAPP.RoundDoubleToInt(Scale.m_ScaleX * GetAttributes().m_LineWidth);
		if (Lw < 1) Lw = 1;

		switch (mode.DrawMode)
		{
		case ObjectDrawMode::FINAL:
		case ObjectDrawMode::SKETCH:
			penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
			Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
			Obj.pCadRect->Draw(pDC, mode, ULHC, Scale);
			ObjCenter.pCadObject = Obj.pCadRect->FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
			pOldPen = pDC->SelectObject(&penLine);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(Delta,Delta),pDC,ULHC,Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(Delta, -Delta), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-Delta, Delta), pDC, ULHC, Scale);
			ObjCenter.pCadPoint->LineFromHereToThere(CDoubleSize(-Delta, -Delta), pDC, ULHC, Scale);
			break;
		case ObjectDrawMode::SELECTED:
			penLine.CreatePen(PS_SOLID, Lw, GetAttributes().m_colorLine);
			Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
			Obj.pCadRect->Draw(pDC, mode, ULHC, Scale);
			ObjCenter.pCadObject = Obj.pCadRect->FindChildObject(ObjectType::POINT, SubType::CENTERPOINT, 0);
			pOldPen = pDC->SelectObject(&penLine);
			ObjCenter.pCadPoint->Draw(pDC, mode, ULHC, Scale);
			break;
		}
	}
}

BOOL CCadHoleRect::PointInThisObject(DOUBLEPOINT point)
{
	CADObjectTypes Obj;
	BOOL rV;
	Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
	rV = Obj.pCadRect->PointInThisObject(point);
	return rV;
}

int CCadHoleRect::PointInObjectAndSelect(
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

CString& CCadHoleRect::GetTypeString(void)
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csName = _T("Rect Hole");
	return csName;
}

CString& CCadHoleRect::GetObjDescription()
{
	GetObjDescription().Format(_T("RectHole"));
	return GetObjDescription();
}

CCadObject * CCadHoleRect::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CADObjectTypes newObj;
	newObj.pCadHoleRect = new CCadHoleRect;
	CCadObject::CopyObject(newObj.pCadObject);
	newObj.pCadHoleRect->CopyAttributesFrom(GetPtrToAttributes());
	return newObj.pCadObject;
}


CDoubleSize CCadHoleRect::GetSize()
{
	//---------------------------------------------------
	// GetSize
	//	Get the size of the object.  Reutrns the size
	// of the enclosing rectangle.
	// parameters:
	//
	// return value:returns size of the object
	//--------------------------------------------------
	return CSize();
}

DocFileParseToken CCadHoleRect::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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
	return Token;
}

void CCadHoleRect::CopyAttributesTo(SRectHoleAttributes * pAttrib)
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

void CCadHoleRect::CopyAttributesFrom(SRectHoleAttributes *pAttrib)
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

ObjectDrawState CCadHoleRect::ProcessDrawMode(ObjectDrawState DrawState)
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
		//---------------------------------------------
		// Create parts for the Rectangular Hole
		//---------------------------------------------
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangular Hole:Place Center Point"));
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
		Id = EditProperties();
		if (IDOK == Id)
		{
			m_AttributesDirty = TRUE;
			CopyAttributesTo(&m_CurrentAttributes);
		}
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
		Obj.pCadRect->SetCenterPoint(MousePos);
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
		Obj.pCadRect->SetCenterPoint(MousePos);
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		Obj.pCadHoleRect = new CCadHoleRect;
		GETVIEW->SetObjectTypes(Obj.pCadHoleRect);
		Obj.pCadHoleRect->Create(GetParent(), GetOrigin());
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("Rectangular Hole:Place Center Point"));
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

ObjectDrawState CCadHoleRect::MouseMove(ObjectDrawState DrawState)
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
	CADObjectTypes Obj;

	switch (DrawState)
	{
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		Obj.pCadObject = FindChildObject(ObjectType::RECT, SubType::RECTSHAPE, 0);
		Obj.pCadRect->SetCenterPoint(MousePos);
		GETVIEW->Invalidate();
		break;
	}
	return DrawState;
}

int CCadHoleRect::EditProperties()
{
	int Id;
	CDlgSquareHoleProp Dlg;

	Dlg.SetRectHole(this);
	Id = Dlg.DoModal();
	return Id;
}
