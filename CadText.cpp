#include "pch.h"

CCadText::CCadText():CCadObject(ObjectType::TEXT)
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

BOOL CCadText::Create(CCadObject* pParent, SubTypes type)
{
	CADObjectTypes Obj;

	if (pParent == NULL)
		pParent = this;

	CCadObject::Create(pParent, type);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::TEXT_LOCATION);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadPoint = new CCadPoint;
	Obj.pCadPoint->Create(pParent, CCadObject::SubTypes::TEXT_ROTATION);
	AddObjectAtTail(Obj.pCadObject);
	Obj.pCadRect = new CCadRect;
	Obj.pCadRect->Create(pParent, CCadObject::SubTypes::TEXT_RECT);
	AddObjectAtTail(Obj.pCadObject);
	return TRUE;
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

void CCadText::Save(FILE * pO, CLexer::Tokens Token, int Indent, int flags)
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

void CCadText::Draw(CDC * pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale)
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
		LOC.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::TEXT_LOCATION, 0);

		P1 = LOC.pCadPoint->ToPixelPoint(LLHC, Scale);
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
		if (IsSelected())
		{
			RECT.pCadObject = FindObject(ObjectType::RECT, CCadObject::SubTypes::TEXT_RECT, 0);
			penLine.CreatePen(PS_SOLID, 2, GetAttributes().m_colorSelected);
			pOldPen = pDC->SelectObject(&penLine);
			RECT.pCadRect->Draw(pDC, mode, LLHC, Scale);
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

	Obj.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::TEXT_LOCATION, 0);
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
	pCT->Create(GetParent(),GetSubType());
	CCadObject::CopyObject(pCT);
	return pCT;
}

CLexer::Tokens CCadText::Parse(
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
	Token = pParser->Expect(Token, CLexer::Tokens::TEXT);
	Token = pParser->Expect(Token, CLexer::Tokens('('));
	int loop = 1;
	CString csError,csTemp;
	int TempInt = 0;

	while (loop)
	{
		switch (Token)
		{
		case CLexer::Tokens::STRING:
			csTemp = CString(pParser->GetLexer()->GetLexBuff());
			SetText(csTemp);
			Token = pParser->Expect(Token, CLexer::Tokens::STRING);
			break;
		case CLexer::Tokens::POINT:
//			Token = pLex->Point(CLexer::Tokens::POINT, m_P1, Token);
			break;
		case CLexer::Tokens::TEXT_COLOR:
			Token = pParser->Color(
				CLexer::Tokens::TEXT_COLOR,
				GetAttributes().m_colorText, 
				Token
			);
			break;
		case CLexer::Tokens::BACKGROUND_COLOR:
			Token = pParser->Color(
				CLexer::Tokens::BACKGROUND_COLOR,
				GetAttributes().m_colorBK, 
				Token
			);
			break;
		case CLexer::Tokens::FONT:
			Token = pParser->StringValue(
				CLexer::Tokens::FONT, 
				GetAttributes().m_csFontName, 
				Token
			);
			break;
		case CLexer::Tokens::FONT_WEIGHT:
			Token = pParser->UINTValue(
				CLexer::Tokens::FONT_WEIGHT, 
				GetAttributes().m_fontWeight,
				Token
			);
			GetAttributes().m_fontWeight = TempInt;
			break;
		case CLexer::Tokens::FONTHEIGHT:
			Token = pParser->DoubleValue(
				CLexer::Tokens::FONTHEIGHT,
				GetAttributes().m_fontHeight, 
				Token
			);
			break;
		case CLexer::Tokens::FONTWIDTH:
			Token = pParser->DoubleValue(
				CLexer::Tokens::FONTWIDTH,
				GetAttributes().m_fontWidth, 
				Token
			);
			break;
		case CLexer::Tokens::ANGLE:
			Token = pParser->DoubleValue(
				CLexer::Tokens::ANGLE, 
				m_Angle, 
				Token
			);
			break;
		case CLexer::Tokens::BK_TRANSPARENT:
			Token = pParser->UINTValue(
				CLexer::Tokens::BK_TRANSPARENT,
				GetAttributes().m_Transparent, 
				Token
			);
			break;
		case CLexer::Tokens(','):
			Token = pParser->Expect(CLexer::Tokens(','), Token);
			break;
		case CLexer::Tokens(')'):
			loop = 0;
			Token = pParser->Expect(CLexer::Tokens(')'), Token);
			break;
		default:
			loop = 0;
//			csError.Format(_T("Unexpected Token %lS"), CString(pLex->TokenLookup(Token)).GetBuffer());
//			pLex->Error(csError);
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
		Obj.pCadObject = FindObject(ObjectType::POINT,CCadObject::SubTypes::TEXT_LOCATION,0);
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
		GetParent()->AddObjectAtTail(this);
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
			Obj.pCadObject = FindObject(
				ObjectType::POINT, 
				CCadObject::SubTypes::TEXT_ROTATION,
				SUBSUBTYPE_ANY
			);
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
	LOC.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::TEXT_LOCATION, 0);
	ROT.pCadObject = FindObject(ObjectType::POINT, CCadObject::SubTypes::TEXT_ROTATION, 0);
	RECT.pCadObject = FindObject(ObjectType::RECT,CCadObject::SubTypes::TEXT_RECT,0);
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
//	RECT.pCadRect->SetPoints(dszTextRect,DOUBLEPOINT(*LOC.pCadPoint),DOUBLEPOINT(*ROT.pCadPoint));
}


int CCadText::EditProperties(void)
{
	int Id;
	CDlgTextProperties Dlg;

	Dlg.SetTextObject(this);
	Id = Dlg.DoModal();
	return Id;
}
