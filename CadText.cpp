#include "pch.h"

CCadText::CCadText():CCadObject()
{
	GetName().Format(_T("Text_%d"), ++m_TextCount);
	if (NeedsAttributes())
	{
		ClearNeedsAttributes();
		m_LastAttributes.CopyFrom(GETAPP.GetTextAttributes());
		m_CurrentAttributes.CopyFrom(&m_LastAttributes);
	}
	CopyAttributesFrom(&m_CurrentAttributes);
}

CCadText::~CCadText()
{
}

void CCadText::Create()
{
	CADObjectTypes Obj;

	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::TEXT_LOCATION);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create();
	Obj.pCadPoint->SetSubType(SubType::TEXT_ROTATION);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadRect = new CCadRect;
	Obj.pCadRect->Create();
	Obj.pCadRect->SetSubType(SubType::TEXT_RECT);
	AddObjectAtTail(Obj.pCadObject);
}

BOOL CCadText::Destroy(CCadObject* pDependentObject)
{
	BOOL rV = TRUE;
	return rV;
}

void CCadText::Move(CDoubleSize Diff)
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

void CCadText::Save(FILE * pO, DocFileParseToken Token, int Indent, int flags)
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

void CCadText::Draw(CDC * pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale)
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
	CFont *pOldFont, Font;
	CPen *pOldPen, penLine;
	COLORREF OldColor, OldBk;
	int FontHeight, FontWidth;
	CPoint P1;
	int OldMode;
	CADObjectTypes LOC, RECT;

	if (IsRenderEnabled())
	{
		LOC.pCadObject = FindObject(ObjectType::POINT, SubType::TEXT_LOCATION, 0);

		P1 = LOC.pCadPoint->ToPixelPoint(ULHC, Scale);
		FontHeight = GETAPP.RoundDoubleToInt(Scale.GetScaleX() * GetFontHeight());
		FontWidth = GETAPP.RoundDoubleToInt(Scale.GetScaleX() * GetFontWidth());
		if (FontHeight > 8 && FontWidth > 8)
		{
			Font.CreateFontW(
				GETAPP.RoundDoubleToInt(GetFontHeight() * Scale.GetScaleY()),
				GETAPP.RoundDoubleToInt(GetFontWidth() * Scale.GetScaleX()),
				GETAPP.RoundDoubleToInt(GetAngle() * 10.0),
				GETAPP.RoundDoubleToInt(GetAngle() * 10.0),
				GetFontWeight(),
				0, 0, 0,
				DEFAULT_CHARSET,
				OUT_CHARACTER_PRECIS,
				CLIP_CHARACTER_PRECIS,
				PROOF_QUALITY,
				DEFAULT_PITCH,
				GetFontName()
			);

		}

		OldColor = pDC->SetTextColor(GetTextColor());
		OldBk = pDC->SetBkColor(GetBackgroundColor());
		pOldFont = pDC->SelectObject(&Font);
		if (GetTransparent())
			OldMode = pDC->SetBkMode(TRANSPARENT);
		else
			OldMode = pDC->SetBkMode(OPAQUE);
		pDC->TextOutW(P1.x, P1.y, GetText());
		pDC->SetBkMode(OldMode);
		if (mode.DrawMode == ObjectDrawMode::SELECTED)
		{
			RECT.pCadObject = FindObject(ObjectType::RECT, SubType::TEXT_RECT, 0);
			penLine.CreatePen(PS_SOLID, 2, GetAttributes().m_colorSelected);
			pOldPen = pDC->SelectObject(&penLine);
			RECT.pCadRect->Draw(pDC, mode, ULHC, Scale);
			pDC->SelectObject(pOldPen);
		}
		pDC->SetBkColor(OldBk);
		pDC->SetTextColor(OldColor);
		pDC->SelectObject(pOldFont);
	}
}

BOOL CCadText::PointInThisObject(DOUBLEPOINT point)
{
	return 0;
}

int CCadText::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject** ppSelList,
	int index,
	int n
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
	int ix;

	if (index < n)
	{
		//---------------------------------------
		// is point in the Arc?
		//---------------------------------------
		if (PointInThisObject(p))
		{
			ppSelList[index++] = this;
			ix = CCadObject::PointInObjectAndSelect(
				p,
				ppSelList,
				index,
				n
			);
			index += ix;
		}
	}
	return index;
}

CString& CCadText::GetTypeString(void)
{
	//---------------------------------------------------
	// GetTypeString
	//	returns a string that describes the type of
	// object this is
	// parameters:
	//
	// return value:pointer to a string
	//--------------------------------------------------
	static CString csTypeName = CString(_T("Text"));
	return csTypeName;
}

CString& CCadText::GetObjDescription()
{
	CADObjectTypes Obj;

	Obj.pCadObject = FindObject(ObjectType::POINT, SubType::TEXT_LOCATION, 0);
	GetDescription().Format(_T("Text(%7.3lf,%7.3lf)"),Obj.pCadPoint->GetX(),Obj.pCadPoint->GetY());
	return GetDescription();
}

CCadObject * CCadText::CopyObject(void)
{
	//---------------------------------------------------
	// CopyObject
	//	Creates a copy of this and returns a pointer
	// to the copy
	// parameters:
	//
	// return value:a new copy of this
	//--------------------------------------------------
	CCadText *pCT = new CCadText;
	pCT->Create();
	CCadObject::CopyObject(pCT);
	return pCT;
}

DocFileParseToken CCadText::Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken)
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
	Token = pLex->Accept(Token, DocFileParseToken::TEXT);
	Token = pLex->Accept(Token, DocFileParseToken('('));
	int loop = 1;
	CString csError,csTemp;

	while (loop)
	{
		switch (Token)
		{
		case DocFileParseToken::STRING:
			csTemp = CString(pLex->GetLexBuff());
			SetText(csTemp);
			Token = pLex->Accept(Token, DocFileParseToken::STRING);
			break;
		case DocFileParseToken::POINT:
//			Token = pLex->Point(DocFileParseToken::POINT, m_P1, Token);
			break;
		case DocFileParseToken::COLOR:
			Token = pLex->Color(DocFileParseToken::COLOR, GetAttributes().m_colorText, Token);
			break;
		case DocFileParseToken::BKCOLOR:
			Token = pLex->Color(
				DocFileParseToken::BKCOLOR, 
				GetAttributes().m_colorBK, 
				Token
			);
			break;
		case DocFileParseToken::FONT:
			Token = pLex->csString(
				DocFileParseToken::FONT, 
				GetAttributes().m_csFontName, 
				Token
			);
			break;
		case DocFileParseToken::WEIGHT:
			Token = pLex->ReadUINTDecimalValue(
				DocFileParseToken::WEIGHT, 
				GetAttributes().m_fontWeight, 
				Token
			);
			break;
		case DocFileParseToken::HEIGHT:
			Token = pLex->ReadDoubleValue(
				DocFileParseToken::HEIGHT, 
				GetAttributes().m_fontHeight, 
				Token
			);
			break;
		case DocFileParseToken::WIDTH:
			Token = pLex->ReadDoubleValue(
				DocFileParseToken::WIDTH, 
				GetAttributes().m_fontWidth, 
				Token
			);
			break;
		case DocFileParseToken::ANGLE:
			Token = pLex->ReadDoubleValue(DocFileParseToken::ANGLE, m_Angle, Token);
			break;
		case DocFileParseToken::TRANSPARENTToken:
			Token = pLex->ReadUINTDecimalValue(
				DocFileParseToken::TRANSPARENTToken, 
				GetAttributes().m_Transparent, 
				Token
			);
			break;
		case DocFileParseToken(','):
			Token = pLex->Accept(Token, DocFileParseToken(','));
			break;
		case DocFileParseToken(')'):
			loop = 0;
			Token = pLex->Accept(Token, DocFileParseToken(')'));
			break;
		default:
			loop = 0;
			csError.Format(_T("Unexpected Token %lS"), CString(pLex->TokenToString(Token)).GetBuffer());
			pLex->Error(csError);
			break;
		}
	}
	return Token;
}

void CCadText::CopyAttributesTo(STextAttributes *pAttrib)
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

void CCadText::CopyAttributesFrom(STextAttributes *pAttrib)
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

ObjectDrawState CCadText::ProcessDrawMode(ObjectDrawState DrawState)
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
		DrawState = ObjectDrawState::SET_ATTRIBUTES;
		GETAPP.UpdateStatusBar(_T("TEXT:Enter Text Parameters"));
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
		GETAPP.UpdateStatusBar(_T(""));
		break;
	case ObjectDrawState::SET_ATTRIBUTES:
		Id = EditProperties();
		if (IDOK == Id)
		{
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
		}
		GETAPP.UpdateStatusBar(_T("TEXT:Place Text on Document"));
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_DOWN:
		DrawState = ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP;
		break;
	case ObjectDrawState::WAITFORMOUSE_DOWN_LBUTTON_UP:
		Obj.pCadObject = FindObject(ObjectType::POINT,SubType::TEXT_LOCATION,0);
		Obj.pCadPoint->SetPoint(MousePos);
		DrawState = ObjectDrawState::ROTATE_LBUTTON_DOWN;
		GETAPP.UpdateStatusBar(_T("TEXT:Define Text Angle"));
		GETVIEW->Invalidate();
		break;
	case ObjectDrawState::ROTATE_LBUTTON_DOWN:
		DrawState = ObjectDrawState::ROTATE_LBUTTON_UP;
		break;
	case ObjectDrawState::ROTATE_LBUTTON_UP:
		DrawState = ObjectDrawState::SET_ATTRIBUTES;
		GETVIEW->GetDocument()->AddObjectAtTail(this);
		GETAPP.UpdateStatusBar(_T("TEXT:Enter Text Parameters"));
		GETVIEW->SetObjectTypes(new CCadText);
		break;
	}
	return DrawState;
}


ObjectDrawState CCadText::MouseMove(ObjectDrawState DrawState)
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
	CADObjectTypes Obj;

	switch (DrawState)
	{
		case ObjectDrawState::ROTATE_LBUTTON_DOWN:
			Obj.pCadObject = FindObject(ObjectType::POINT, SubType::TEXT_ROTATION);
			Obj.pCadPoint->SetPoint(MousePos);
			GETVIEW->Invalidate();
			break;
	}
	return DrawState;
}

void CCadText::Rotate()
{
	//----------------------------------------------
	// Rotate
	//		Thhis Method calculates the angle that
	// the text is to be rotated by, and also
	// updates the rectangle that will be displayed
	// when the text is selected.
	//
	//		parameters: None
	//----------------------------------------------
	CADObjectTypes LOC, ROT, RECT;
	double X, Y;
	CSize szTextRect;
	CScale Scale;
	CDoubleSize dszTextRect;
	CFont Font, *pOldFont;
	CDC* pDC;

	//-------------------------------------
	// Get objects that define text object
	//-------------------------------------
	LOC.pCadObject = FindObject(ObjectType::POINT, SubType::TEXT_LOCATION, 0);
	ROT.pCadObject = FindObject(ObjectType::POINT, SubType::TEXT_ROTATION, 0);
	RECT.pCadObject = FindObject(ObjectType::RECT,SubType::TEXT_RECT,0);
	X = ROT.pCadPoint->GetX() - LOC.pCadPoint->GetX();	// run
	Y = LOC.pCadPoint->GetY() - ROT.pCadPoint->GetY();	// rise
	m_Angle = GETAPP.ArcTan(X, Y);
	pDC = GETVIEW->GetDC();
	Scale = GETVIEW->GetGrid().GetPixelsPerInch();
	Font.CreateFontW(
		GETAPP.RoundDoubleToInt(GetFontHeight() * Scale.GetScaleY()),
		GETAPP.RoundDoubleToInt(GetFontWidth() * Scale.GetScaleX()),
		0,	//angle is 0 for this purpose
		0,	//angle is 0 for this purpose
		GetFontWeight(),
		0, 0, 0,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,
		PROOF_QUALITY,
		DEFAULT_PITCH,
		GetFontName()
	);
	pOldFont = pDC->SelectObject(&Font);
	szTextRect = pDC->GetTextExtent(m_csText);
	dszTextRect = CDoubleSize(szTextRect);
	RECT.pCadRect->SetPoints(dszTextRect,DOUBLEPOINT(*LOC.pCadPoint),DOUBLEPOINT(*ROT.pCadPoint));
}


int CCadText::EditProperties(void)
{
	int Id;
	CDlgTextProperties Dlg;

	Dlg.SetTextObject(this);
	Id = Dlg.DoModal();
	if (IDOK == Id)
	{
		if (Dlg.IsDirty())
		{
			CopyAttributesTo(&m_CurrentAttributes);
			m_AttributesDirty = TRUE;
		}
	}
	return Id;
}
