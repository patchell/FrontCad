#include "pch.h"


//----- Arc Attributes --------

void SArcAttributes::LoadSettings(SArcAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csString;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ARC"), _T("COLOR"), RGB(192, 192, 192));
	csString  = GETAPP.GetProfileStringW(_T("ARC"), _T("WIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csString);
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("ARC"), _T("COLOR_SELECTED"), RGB(255, 0, 0));
}

void SArcAttributes::SaveSettings(SArcAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("ARC"), _T("COLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("ARC"), _T("COLOR_SELECTED"), pAttrib->m_colorSelected);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ARC"), _T("WIDTH"), csString);
}

void SArcAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SArcAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// ParseParse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pLex......Pointer to the Lexer object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_ARC);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//----- Arc Center Attributes --------

void SArcCenterAttributes::LoadSettings(SArcCenterAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csVal;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ARC_CENTER"), _T("COLOR"), RGB(255, 0, 0));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("ARC_CENTER"), _T("SELECTEDCOLOR"), RGB(0, 0, 1922));
	csVal = GETAPP.GetProfileStringW(_T("ARC_CENTER"), _T("WIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csVal);
}

void SArcCenterAttributes::SaveSettings(SArcCenterAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;
	GETAPP.WriteProfileInt(_T("ARC_CENTER"), _T("COLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("ARC_CENTER"), _T("SELECTEDCOLOR"), pAttrib->m_colorSelected);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ARC_CENTER"), _T("WIDTH"), csString);
}

void SArcCenterAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SArcCenterAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// Parse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pParser...Pointer to the Parser object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_ARCCENTER);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//--------------- Arrow Attributes ----------

CString DefaultArrowX[4] = {
	_T("0.0"),
	_T("0.20"),
	_T("0.15"),
	_T("0.20")
};

CString DefaultArrowY[4] = {
	_T("0.0"),
	_T("0.10"),
	_T("0.0"),
	_T("-0.10")
};

void SArrowAttributes::LoadSettings(SArrowAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CCadPoint* Points;
	UINT NumOfPoints;
	UINT i;
	CString csString;
	CString csVertexNameX;
	CString csVertexNameY;
	double x, y;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ARROW"), _T("COLOR"), RGB(255,0,0));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("ARROW"), _T("SELECTEDCOLOR"), RGB( 0, 255, 0));
	pAttrib->m_colorFill = GETAPP.GetProfileIntW(_T("ARROW"), _T("FILLCOLOR"), RGB(0,0,255));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csString);
	csString = GETAPP.GetProfileStringW(_T("ARROW"), _T("LINEWIDTH"), _T("0.020"));
	NumOfPoints = GETAPP.GetProfileIntW(_T("ARROW"), _T("NUM_OF_VERTEX"), 4);
	Points = new CCadPoint[NumOfPoints];
	for (i = 0; i < NumOfPoints; ++i)
	{
		csVertexNameX.Format(_T("VERTEX_%d_X"), i);
		csVertexNameY.Format(_T("VERTEX_%d_Y"), i);
		csString = GETAPP.GetProfileStringW(
			_T("ARROW"), 
			csVertexNameX, 
			DefaultArrowX[i]
		);
		x = GETAPP.StringToDouble(csString);
		csString = GETAPP.GetProfileStringW(
			_T("ARROW"),
			csVertexNameY,
			DefaultArrowY[i]
		);
		y = GETAPP.StringToDouble(csString);
		Points[i] = CCadPoint(x, y);
	}
}

void SArrowAttributes::SaveSettings(SArrowAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
}

void SArrowAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SArrowAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// Parse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pLex......Pointer to the Lexer object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_ARROW);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//------------- Bitmap Attributes -----------------

void SBitmapAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_BITMAP);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

void SBitmapAttributes::Save(
	CFile* pcfFile, 
	int Indent
)
{

}

void SBitmapAttributes::LoadSettings(SBitmapAttributes* pAttrb)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/

}

void SBitmapAttributes::SaveSettings(SBitmapAttributes* pAttrb)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/

}

//----------- Dimension Attributes -----------------


void SCadDimAttributes::LoadSettings(SCadDimAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	int i;
	CString csValue;
	CString csType;
	CString csDefult;

	csValue = GETAPP.GetProfileStringW(_T("DIM"), _T("LINE_WIDTH"), _T("0.020"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csValue);;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("DIM"),_T("LINE_COLOR"),RGB(128, 128, 128));
	pAttrib->m_colorText = GETAPP.GetProfileIntW(_T("DIM"), _T("TEXT_COLOR"), RGB(128, 128, 128));

	csValue = GETAPP.GetProfileStringW(_T("DIM"), _T("TEXT_HIEGHT"), _T("0.300"));
	pAttrib->m_TextHeight = GETAPP.StringToDouble(csValue);

	pAttrib->m_colorBKG = GETAPP.GetProfileIntW(_T("DIM"), _T("BACKGROUND_COLOR"), RGB(0, 0, 0));

	csValue = GETAPP.GetProfileStringW(_T("DIM"), _T("EXT_LINE_GAP"), _T("0.125"));
	pAttrib->m_ExtLineGap = GETAPP.StringToDouble(csValue);

	csValue = GETAPP.GetProfileStringW(_T("DIM"), _T("DIM_LINE_INSET"), _T("0.250"));
	pAttrib->m_DimLineInset = GETAPP.StringToDouble(csValue);

	for (i = 0; i < 4; ++i)
	{
		csDefult.Format(_T("%6.3lf"), m_aDefaultArrow[i].dX);
		csType.Format(_T("ARROW_X_%d"),i);
		csValue = GETAPP.GetProfileStringW(_T("DIM"), csType, csDefult);
		pAttrib->m_aArrowShape[i].dX = GETAPP.StringToDouble(csValue);

		csDefult.Format(_T("%6.3lf"), m_aDefaultArrow[i].dY);
		csType.Format(_T("ARROW_Y_%d"), i);
		csValue = GETAPP.GetProfileStringW(_T("DIM"), csType, csDefult);
		pAttrib->m_aArrowShape[i].dX = GETAPP.StringToDouble(csValue);
	}
}

void SCadDimAttributes::SaveSettings(SCadDimAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	int i;
	CString csValue;
	CString csType;

	csValue.Format(_T("%6.3lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("DIM"), _T("LINE_WIDTH"), csValue);

	GETAPP.WriteProfileInt(_T("DIM"), _T("LINE_COLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("DIM"), _T("TEXT_COLOR"), pAttrib->m_colorText);

	csValue.Format(_T("%6.3lf"), pAttrib->m_TextHeight);
	GETAPP.WriteProfileStringW(_T("DIM"), _T("TEXT_HIEGHT"), csValue);

	GETAPP.WriteProfileInt(_T("DIM"), _T("BACKGROUND_COLOR"), pAttrib->m_colorBKG);

	csValue = GETAPP.GetProfileStringW(_T("DIM"), _T("EXT_LINE_GAP"), _T("0.125"));
	pAttrib->m_ExtLineGap = GETAPP.StringToDouble(csValue);

	csValue.Format(_T("%6.3lf"), pAttrib->m_DimLineInset);
	GETAPP.WriteProfileStringW(_T("DIM"), _T("DIM_LINE_INSET"), csValue);

	for (i = 0; i < 4; ++i)
	{
		csType.Format(_T("ARROW_X_%d"), i);
		csValue.Format(_T("%6.3lf"), pAttrib->m_aArrowShape[i].dX);
		GETAPP.WriteProfileStringW(_T("DIM"), csType, csValue);

		csType.Format(_T("ARROW_Y_%d"), i);
		csValue.Format(_T("%6.3lf"), pAttrib->m_aArrowShape[i].dY);
		GETAPP.WriteProfileStringW(_T("DIM"), csType, csValue);
	}
}

void SCadDimAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SCadDimAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// Parse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pLex......Pointer to the Lexer object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_DIMENSION);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//-------------- Ellipse Attributes -----------------


void SEllipseAttributes::LoadSettings(SEllipseAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csVal;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ELIPSE"), _T("LINECOLOR"), RGB(0,0,255));
	pAttrib->m_colorLineSelected = GETAPP.GetProfileIntW(_T("ELIPSE"), _T("SELECTEDCOLOR"), RGB(0, 255, 0));
	pAttrib->m_colorFill = GETAPP.GetProfileIntW(_T("ELIPSE"), _T("FILLCOLOR"), RGB(255, 0, 0));
	csVal = GETAPP.GetProfileStringW(_T("ELIPSE"), _T("LINEWIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csVal);
	pAttrib->m_TransparentFill = GETAPP.GetProfileIntW(_T("ELIPSE"), _T("TRANSPARENT_FILL"), 0);
}

void SEllipseAttributes::SaveSettings(SEllipseAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("ELIPSE"),_T("LINECOLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("ELIPSE"), _T("SELECTEDCOLOR"), pAttrib->m_colorLineSelected);
	GETAPP.WriteProfileInt(_T("ELIPSE"), _T("FILLCOLOR"), pAttrib->m_colorFill);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ELIPSE"), _T("LINEWIDTH"), csString);
	GETAPP.WriteProfileInt(_T("ELIPSE"), _T("TRANSPARENT_FILL"), pAttrib->m_TransparentFill);
}

void SEllipseAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SEllipseAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// Parse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pLex......Pointer to the Lexer object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_ELLIPSE);
	pParser->Expect( Token('{'));
	while (Loop)
	{
			switch (pParser->GetLookaHead())
		{
			case Token::LINE_WIDTH:
				break;
			case Token::LINE_COLOR:
				break;;
			case Token::SELECTED_COLOR:
				break;
			default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//------------ Line Attributes -----------------------


void SLineAttributes::LoadSettings(SLineAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csVal;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("LINE"), _T("COLOR"), (int)RGB(128, 128, 192));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("LINE"), _T("SELECTED_COLOR"), (int)RGB(192, 0, 0));
	csVal = GETAPP.GetProfileStringW(_T("LINE"), _T("WIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csVal);
	pAttrib->m_LockLength = GETAPP.GetProfileIntW(_T("LINE"), _T("LEN_LOCKED"), 0);

	pAttrib->m_P1_SNAP_POINT = GETAPP.GetProfileIntW(_T("LINE"), _T("P1_SNAP_POINT"), FALSE);
	pAttrib->m_P2_FIXED_LEN_SNAP_LINE = GETAPP.GetProfileIntW(_T("LINE"), _T("P2_SNAP_LINE"), FALSE);
	pAttrib->m_P2_SNAP_POINT = GETAPP.GetProfileIntW(_T("LINE"), _T("P2_SNAP_POINT"), FALSE);
}

void SLineAttributes::SaveSettings(SLineAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("LINE"), _T("COLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("LINE"), _T("SELECTED_COLOR"), pAttrib->m_colorSelected );
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("LINE"), _T("WIDTH"), csString);
	GETAPP.WriteProfileInt(_T("LINE"), _T("LEN_LOCKED"), pAttrib->m_LockLength);
	GETAPP.WriteProfileInt(_T("LINE"), _T("P1_SNAP_POINT"), pAttrib->m_P1_SNAP_POINT);
	GETAPP.WriteProfileInt(_T("LINE"), _T("P2_SNAP_LINE"), pAttrib->m_P2_FIXED_LEN_SNAP_LINE);
	GETAPP.WriteProfileInt(_T("LINE"), _T("P2_SNAP_POINT"), pAttrib->m_P2_SNAP_POINT);
}

void SLineAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SLineAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// Parse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pLex......Pointer to the Lexer object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_LINE);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//------------ Origin Attributes ---------------------


void SOriginAttributes::LoadSettings(SOriginAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csVal;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ORIGIN"), _T("LINECOLOR"), RGB(192,192,192));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("ORIGIN"), _T("SELECTEDCOLOR"), RGB(255, 0, 0));
	csVal = GETAPP.GetProfileStringW(_T("ORIGIN"), _T("LINEWIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csVal);
	csVal = GETAPP.GetProfileStringW(_T("ORIGIN"), _T("RADIUS"), _T("0.25"));
	pAttrib->m_Radius = GETAPP.StringToDouble(csVal);
}

void SOriginAttributes::SaveSettings(SOriginAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("ORIGIN"), _T("LINECOLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("ORIGIN"), _T("SELECTEDCOLOR"), pAttrib->m_colorSelected);
	csString.Format(_T("%6.3lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ORIGIN"), _T("LINEWIDTH"), csString);
	csString.Format(_T("%6.3lf"), pAttrib->m_Radius);
	GETAPP.WriteProfileStringW(_T("ORIGIN"), _T("RADIUS"), csString);
}

void SOriginAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SOriginAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// Parse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pLex......Pointer to the Lexer object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_ORIGIN);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//---------- Polygon Attributes -------------------

void SPolyAttributes::LoadSettings(SPolyAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csVal;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("POLYGON"), _T("LINECOLOR"), RGB(192,192,192));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("POLYGON"), _T("SELECTCOLOR"), RGB(255, 0, 64));
	pAttrib->m_colorFill = GETAPP.GetProfileIntW(_T("POLYGON"), _T("FILLCOLOR"), RGB(255, 0, 0));
	csVal = GETAPP.GetProfileStringW(_T("POLYGON"), _T("LINEWIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csVal);
	pAttrib->m_TransparentFill = GETAPP.GetProfileIntW(_T("POLYGON"), _T("TRANSPARENT_FILL"), 0);
}

void SPolyAttributes::SaveSettings(SPolyAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("POLYGON"), _T("LINECOLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("POLYGON"), _T("SELECTCOLOR"),pAttrib->m_colorSelected);
	GETAPP.WriteProfileInt(_T("POLYGON"), _T("FILLCOLOR"), pAttrib->m_colorFill);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("POLYGON"), _T("LINEWIDTH"), csString);
	GETAPP.WriteProfileInt(_T("POLYGON"), _T("TRANSPARENT_FILL"), pAttrib->m_TransparentFill);
}

void SPolyAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SPolyAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// Parse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pLex......Pointer to the Lexer object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_POLYGON);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//--------- Rectangle Attributes ------------------


void SRectAttributes::LoadSettings(SRectAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csVal;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("RECTANGLE"), _T("LINECOLOR"), RGB(128, 128, 192));
	pAttrib->m_colorFill = GETAPP.GetProfileIntW(_T("RECTANGLE"), _T("FILLCOLOR"), RGB(255, 0, 0));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("RECTANGLE"), _T("SELLECTEDCOLOR"), RGB(255, 255, 0));
	pAttrib->m_colorSketch = GETAPP.GetProfileIntW(_T("RECTANGLE"), _T("SKETCHCOLOR"), RGB(255, 255, 255));
	csVal= GETAPP.GetProfileStringW(_T("RECTANGLE"), _T("LINEWIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csVal);
	pAttrib->m_TransparentFill = GETAPP.GetProfileIntW(_T("RECTANGLE"), _T("TRANSPARENT_FILL"), 0);
}

void SRectAttributes::SaveSettings(SRectAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("RECTANGLE"), _T("LINECOLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("RECTANGLE"), _T("FILLCOLOR"), pAttrib->m_colorFill);
	GETAPP.WriteProfileInt(_T("RECTANGLE"), _T("SELLECTEDCOLOR"), pAttrib->m_colorSelected);
	GETAPP.WriteProfileInt(_T("RECTANGLE"), _T("SKETCHCOLOR"), pAttrib->m_colorSketch);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("RECTANGLE"), _T("LINEWIDTH"), csString);
	GETAPP.WriteProfileInt(_T("RECTANGLE"), _T("TRANSPARENT_FILL"), pAttrib->m_TransparentFill);
}

void SRectAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SRectAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// Parse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pLex......Pointer to the Lexer object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_RECT);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//----- Rounded Rectangle Attributes --------------

void SRoundedRectAttributes::LoadSettings(SRoundedRectAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csVal;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("RNDRECT"), _T("LINECOLOR"), RGB(128, 128, 192));
	pAttrib->m_colorFill = GETAPP.GetProfileIntW(_T("RNDRECT"), _T("FILLCOLOR"), RGB(255, 0, 0));
	pAttrib->m_colorLineSelected = GETAPP.GetProfileIntW(_T("RNDRECT"), _T("LINESELECTED"), RGB(255, 128, 0));
	csVal = GETAPP.GetProfileStringW(_T("RNDRECT"), _T("LINEWIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csVal);
	pAttrib->m_TransparentFill = GETAPP.GetProfileIntW(_T("RNDRECT"), _T("TRANSPARENT_FILL"), 0);
	csVal = GETAPP.GetProfileStringW(_T("RNDRECT"), _T("C_RADIUS_X"), _T("0.1"));
	pAttrib->m_dszCornerRadius.dCX = GETAPP.StringToDouble(csVal);
	csVal = GETAPP.GetProfileStringW(_T("RNDRECT"), _T("C_RADIUS_Y"), _T("0.1"));
	pAttrib->m_dszCornerRadius.dCY = GETAPP.StringToDouble(csVal);
}

void SRoundedRectAttributes::SaveSettings(SRoundedRectAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("RNDRECT"), _T("LINECOLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("RNDRECT"), _T("FILLCOLOR"), pAttrib->m_colorFill);
	GETAPP.WriteProfileInt(_T("RNDRECT"), _T("LINESELECTED"), pAttrib->m_colorLineSelected);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("RNDRECT"), _T("LINEWIDTH"), csString);
	GETAPP.WriteProfileInt(_T("RNDRECT"), _T("TRANSPARENT_FILL"), pAttrib->m_TransparentFill);
	csString.Format(_T("%10.6lf"), pAttrib->m_dszCornerRadius.dCX);
	GETAPP.WriteProfileStringW(_T("RNDRECT"), _T("C_RADIUS_X"), csString);
	csString.Format(_T("%10.6lf"), pAttrib->m_dszCornerRadius.dCY);
	GETAPP.WriteProfileStringW(_T("RNDRECT"), _T("C_RADIUS_Y"), csString);
}

void SRoundedRectAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SRoundedRectAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	//----------------------------------------
	// Parse
	//	This Method parses the ascii script
	//	that makes up the data for the object
	//	attributes
	//
	// parameters:
	//	Token.....Input (lookahead) Token
	//	pLex......Pointer to the Lexer object
	// returns:
	//	The next lookahead token
	//---------------------------------------
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_RNDRECT);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//----------- Text Attributes ---------------------

void Parse(
	CParser* pParser	// pointer to parser
)
{
}

void STextAttributes::Save(
	CFile* pcfFile, 
	int Indent
)
{
	char* IndentString = new char[256];
	char* IndentString2 = new char[256];
	char* s = new char[256];
	CString csOut;

	GETAPP.IndentString(IndentString, 256, Indent,' ');
	GETAPP.IndentString(IndentString2, 256, Indent+2,' ');
	csOut.Format(
		_T("%hs%hs(\n"),
		IndentString,
		CLexer::TokenLookup(Token::ATTRIB_TEXT)
	);
	csOut.Format(
		_T("%hs%hs\n"),
		IndentString2,
		CParser::SaveStringS(s, 64, Token::FONT_NAME, m_csFontName)
	);
	csOut.Format(
		_T("%hs%hs\n"),
		IndentString2,
		CParser::SaveColorS(s, 64, Token::TEXT_COLOR, m_colorText)
	);
	csOut.Format(
		_T("%hs%hs\n"),
		IndentString2,
		CParser::SaveColorS(s, 64, Token::BACKGROUND_COLOR, m_colorBK)
	);
	csOut.Format( _T("%hs%hs\n"),
		IndentString2,
		CParser::SaveDoubleValueS(s, 64, Token::FONTHEIGHT, m_fontHeight)
	);
	csOut.Format(
		_T("%hs%hs\n"),
		IndentString2,
		CParser::SaveDoubleValueS(s, 64, Token::FONTWIDTH, m_fontWidth)
	);
	csOut.Format(
		_T("%hs%hs\n"),
		IndentString2,
		CParser::SaveUINTValueS(s, 64, Token::FONT_FORMAT, m_Format)
	);
	csOut.Format(
		_T("%hs%hs\n"),
		IndentString2,
		CParser::SaveUINTValueS(s, 64, Token::BK_TRANSPARENT, m_Transparent)
	);
	csOut.Format( 
		_T("%hs%hs\n"),
		IndentString2,
		CParser::SaveUINTValueS(s, 64, Token::FONT_WEIGHT, m_fontWeight)
	);
	csOut.Format(
		_T("%hs)\n"),
		IndentString
	);
	delete[]s;
	delete[] IndentString2;
	delete[] IndentString;
}

void STextAttributes::LoadSettings(STextAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	pAttrib->m_colorText = GETAPP.GetProfileIntW(_T("TEXT"), _T("COLOR"), (int)RGB(0, 0, 0));
	pAttrib->m_colorBK = GETAPP.GetProfileIntW(_T("TEXT"), _T("BACKGROUND_COLOR"), (int)RGB(255, 255, 255));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("TEXT"), _T("SELECTED_COLOR"), (int)RGB(255, 0, 0));
	pAttrib->m_fontHeight = GETAPP.GetProfileIntW(_T("TEXT"), _T("FONT_HEIGHT"), 60);
	pAttrib->m_fontWidth = GETAPP.GetProfileIntW(_T("TEXT"), _T("FONT_WIDTH"), 0);
	pAttrib->m_Format = GETAPP.GetProfileIntW(_T("TEXT"), _T("FORMAT"), DT_BOTTOM | DT_SINGLELINE);
	pAttrib->m_Transparent = GETAPP.GetProfileIntW(_T("TEXT"), _T("TRASPARENT"), 1);
	pAttrib->m_fontWeight = GETAPP.GetProfileIntW(_T("TEXT"), _T("WEIGHT"), FW_DEMIBOLD);
	pAttrib->m_csFontName = GETAPP.GetProfileStringW(_T("TEXT"), _T("FONT_NAME"), _T("ARIAL"));
}

void STextAttributes::SaveSettings(STextAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("TEXT"), _T("COLOR"), (int)pAttrib->m_colorText);
	GETAPP.WriteProfileInt(_T("TEXT"), _T("BACKGROUND_COLOR"), (int)pAttrib->m_colorBK);
	GETAPP.WriteProfileInt(_T("TEXT"), _T("SELECTED_COLOR"), pAttrib->m_colorSelected);

	csString.Format(_T("%lf"), pAttrib->m_fontHeight);
	GETAPP.WriteProfileStringW(_T("TEXT"), _T("FONT_HEIGHT"), csString);

	csString.Format(_T("%lf"), pAttrib->m_fontWidth);
	GETAPP.WriteProfileStringW(_T("TEXT"), _T("FONT_WIDTH"), csString);

	GETAPP.WriteProfileInt(_T("TEXT"), _T("FORMAT"), pAttrib->m_Format);
	GETAPP.WriteProfileInt(_T("TEXT"), _T("TRASPARENT"), pAttrib->m_Transparent);
	GETAPP.WriteProfileInt(_T("TEXT"), _T("WEIGHT"), pAttrib->m_fontWeight);
	GETAPP.WriteProfileStringW(_T("TEXT"), _T("FONT_NAME"), pAttrib->m_csFontName);
}

//----------- Rectangular Hole ------------------------

void SRectHoleAttributes::LoadSettings(SRectHoleAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csString;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("RECTHOLE"), _T("LINECOLOR"), RGB(192, 192, 192));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("RECTHOLE"), _T("SELECTEDCOLOR"), RGB(255, 0, 0));
	csString = GETAPP.GetProfileStringW(_T("RECTHOLE"), _T("LINEWIDTH"), _T("0.04"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csString);
	csString = GETAPP.GetProfileStringW(_T("RECTHOLE"), _T("HOLEWIDTH"), _T("0.25"));
	pAttrib->m_HoleWidth = GETAPP.StringToDouble(csString);
}

void SRectHoleAttributes::SaveSettings(SRectHoleAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("RECTHOLE"), _T("LINECOLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("RECTHOLE"), _T("SELECTEDCOLOR"), pAttrib->m_colorSelected);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("RECTHOLE"), _T("LINEWIDTH"), csString);
	csString.Format(_T("%lf"), pAttrib->m_HoleWidth);
	GETAPP.WriteProfileStringW(_T("RECTHOLE"), _T("WIDTH"), csString);
}

void SRectHoleAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_HOLERECT);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//-------------- Round Hole ---------------------------


void SRoundHoleAttributes::LoadSettings(SRoundHoleAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csString;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ROUNDHOLE"), _T("LINECOLOR"), RGB(192, 192, 192));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("ROUNDHOLE"), _T("SELECTEDCOLOR"), RGB(255, 0, 0));
	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE"), _T("LINEWIDTH"), _T("0.04"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csString);
	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE"), _T("RADIUS"), _T("0.125"));
	pAttrib->m_HoleRadius = GETAPP.StringToDouble(csString);
}

void SRoundHoleAttributes::SaveSettings(SRoundHoleAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("ROUNDHOLE"), _T("LINECOLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("ROUNDHOLE"), _T("SELECTEDCOLOR"), pAttrib->m_colorSelected);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE"), _T("LINEWIDTH"), _T("0.04"));
	csString.Format(_T("%lf"), pAttrib->m_HoleRadius);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE"), _T("RADIUS"), csString);
}

void SRoundHoleAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_HOLERND);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//-------- Round with 1 flat Hole ---------------------

void SRndHole1FlatAttributes::LoadSettings(SRndHole1FlatAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csString;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ROUNDHOLE_1_FLAT"), _T("LINECOLOR"), RGB(192, 192, 192));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("ROUNDHOLE_1_FLAT"), _T("SELECTEDCOLOR"), RGB(255, 0, 0));
	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("LINEWIDTH"), _T("0.04"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csString);
	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("RADIUS"), _T("0.250"));
	pAttrib->m_HoleRadius = GETAPP.StringToDouble(csString);

	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("CENT_TO_FLAT"), _T("0.150"));
	pAttrib->m_FlatDistanceFromCenter = GETAPP.StringToDouble(csString);
}

void SRndHole1FlatAttributes::SaveSettings(SRndHole1FlatAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("ROUNDHOLE_1_FLAT"), _T("LINECOLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("ROUNDHOLE_1_FLAT"), _T("SELECTEDCOLOR"), pAttrib->m_colorSelected);

	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("LINEWIDTH"), csString);

	csString.Format(_T("%lf"), pAttrib->m_HoleRadius);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("RADIUS"), csString);

	csString.Format(_T("%lf"), pAttrib->m_FlatDistanceFromCenter);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("CENT_TO_FLAT"), csString );
}

void SRndHole1FlatAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SRndHole1FlatAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_HOLERND1F);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}
//---------Round with 2 flats Hole --------------------


void SRndHole2FlatAttributes::LoadSettings(SRndHole2FlatAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csString;

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ROUNDHOLE_2_FLAT"), _T("LINECOLOR"), RGB(192, 192, 192));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("ROUNDHOLE_2_FLAT"), _T("SELECTEDCOLOR"), RGB(255, 0, 0));
	
	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_2_FLAT"), _T("LINEWIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csString);

	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_2_FLAT"), _T("RADIUS"), _T("0.25"));
	pAttrib->m_HoleRadius = GETAPP.StringToDouble(csString);

	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_2_FLAT"), _T("CENT_TO_FLAT"), _T("0.180"));
	pAttrib->m_FlatDistanceFromCenter = GETAPP.StringToDouble(csString);
} 

void SRndHole2FlatAttributes::SaveSettings(SRndHole2FlatAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csString;

	GETAPP.WriteProfileInt(_T("ROUNDHOLE_1_FLAT"), _T("LINECOLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("ROUNDHOLE_1_FLAT"), _T("SELECTEDCOLOR"), pAttrib->m_colorSelected);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("LINEWIDTH"), csString);

	csString.Format(_T("%lf"), pAttrib->m_HoleRadius);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("RADIUS"), csString);

	csString.Format(_T("%lf"), pAttrib->m_FlatDistanceFromCenter);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("CENT_TO_FLAT"), csString);
}

void SRndHole2FlatAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SRndHole2FlatAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	BOOL Loop = TRUE;

	pParser->Expect(Token::ATTRIB_HOLERND2F);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

//------------------- Grid Attributes ---------------------------------

void SGridAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SGridAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_GRID);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

void SGridAttributes::LoadSettings(SGridAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	CString csValue;
	double cx, cy;

	pAttrib->m_colorMajorLine = GETAPP.GetProfileInt(_T("GRID"), _T("MAJORCOLOR"),RGB(0,0,63 ));
	pAttrib->m_colorHalfGrid = GETAPP.GetProfileIntW(_T("GRID"), _T("HALFCOLOR"), RGB(0, 31, 0));
	pAttrib->m_colorSnapLine = GETAPP.GetProfileInt(_T("GRID"), _T("SNAPCOLOR"), RGB(63,0,0) );
	pAttrib->m_MajorLineWidth = GETAPP.GetProfileInt(_T("GRID"), _T("MAJORLINEWIDTH"),1);
	pAttrib->m_SnapLineWidth = GETAPP.GetProfileInt(_T("GRID"), _T("SNAPLINECOLOR"), 1);
	pAttrib->m_Snap_Lines_Dots = GETAPP.GetProfileIntW(_T("GRID"), _T("SNAP_LINES_DOTS"), FALSE);
	pAttrib->m_Major_Lines_Dots = GETAPP.GetProfileIntW(_T("GRID"), _T("MAJOR_LINES_DOTS"), TRUE);

	csValue = GETAPP.GetProfileStringW(_T("GRID"), _T("MAJOR_GRID_SPACING_CX"), _T("1.0"));
	cx = GETAPP.StringToDouble(csValue);
	csValue = GETAPP.GetProfileStringW(_T("GRID"), _T("MAJOR_GRID_SPACING_CY"), _T("1.0"));
	cy = GETAPP.StringToDouble(csValue);
	pAttrib->m_MajorGridSpacing = CDoubleSize(cx, cy);

	csValue = GETAPP.GetProfileStringW(_T("GRID"), _T("SNAP_GRID_SPACING_CX"), _T("0.125"));
	cx = GETAPP.StringToDouble(csValue);
	csValue = GETAPP.GetProfileStringW(_T("GRID"), _T("SNAP_GRID_SPACING_CY"), _T("0.125"));
	cy = GETAPP.StringToDouble(csValue);
	pAttrib->m_SnapGridSpacing = CDoubleSize(cx, cy);
}

void SGridAttributes::SaveSettings(SGridAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	CString csValue;

	GETAPP.WriteProfileInt(_T("GRID"), _T("MAJORCOLOR"), pAttrib->m_colorMajorLine);
	GETAPP.WriteProfileInt(_T("GRID"), _T("HALFCOLOR"), pAttrib->m_colorHalfGrid);
	GETAPP.WriteProfileInt(_T("GRID"), _T("SNAPCOLOR"), pAttrib->m_colorSnapLine);
	GETAPP.WriteProfileInt(_T("GRID"), _T("MAJORLINEWIDTH"), pAttrib->m_MajorLineWidth);
	GETAPP.WriteProfileInt(_T("GRID"), _T("SNAPLINECOLOR"), pAttrib->m_SnapLineWidth);
	GETAPP.WriteProfileInt(_T("GRID"), _T("SNAP_LINES_DOTS"), pAttrib->m_Snap_Lines_Dots);
	GETAPP.WriteProfileInt(_T("GRID"), _T("MAJOR_LINES_DOTS"), pAttrib->m_Major_Lines_Dots);

	csValue.Format(_T("%8.4lf"), pAttrib->m_MajorGridSpacing.dCX);
	GETAPP.WriteProfileStringW(_T("GRID"), _T("MAJOR_GRID_SPACING_CX"), csValue);
	csValue.Format(_T("%8.4lf"), pAttrib->m_MajorGridSpacing.dCY);
	GETAPP.WriteProfileStringW(_T("GRID"), _T("MAJOR_GRID_SPACING_CY"), csValue);

	csValue.Format(_T("%8.4lf"), pAttrib->m_SnapGridSpacing.dCX);
	GETAPP.WriteProfileStringW(_T("GRID"), _T("SNAP_GRID_SPACING_CX"), csValue);
	csValue.Format(_T("%8.4lf"), pAttrib->m_SnapGridSpacing.dCY);
	GETAPP.WriteProfileStringW(_T("GRID"), _T("SNAP_GRID_SPACING_CY"), csValue);
}

//------------------- Rulers -----------------------------

void SRulerAttributes::Save(
	CFile* pcfFile,				//File to print to
	int Indent
)
{

}

void SRulerAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_RULERS);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_WIDTH:
			break;
		case Token::LINE_COLOR:
			break;;
		case Token::SELECTED_COLOR:
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

void SRulerAttributes::LoadSettings(SRulerAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	//-------------------- Ruler Tick Mark Font ------------------------------------
	pAttrib->m_csFontName = GETAPP.GetProfileStringW(_T("RULERS"), _T("FONT"),_T("ARIAL"));
	pAttrib->m_colorBk = GETAPP.GetProfileIntW(_T("RULERS"), _T("BKCOLOR"),RGB(92,0,64) );
	pAttrib->m_colorText = GETAPP.GetProfileIntW(_T("RULERS"), _T("TEXTCOLOR"), RGB(255, 255, 255));
	pAttrib->m_FontSize = GETAPP.GetProfileIntW(_T("RULERS"), _T("FONTSIZE"), 16);
	//--------------- Corner ----------------------------------------------------
	pAttrib->m_colorCorner = GETAPP.GetProfileIntW(_T("RULERS"), _T("CORNERCOLOR"), RGB(0,255,0) );
	//-----------------Position Cursor ------------------------------------------
	pAttrib->m_colorCursor = GETAPP.GetProfileIntW(_T("RULERS"), _T("CURSORCOLOR"), RGB(255,255,128) );
	//--------------- Major Tick -------------------------------------
	pAttrib->m_MajTickLength = GETAPP.GetProfileIntW(_T("RULERS"), _T("MAJORTICKLEN"), 16);
	pAttrib->m_colorMajorTick = GETAPP.GetProfileIntW(_T("RULERS"), _T("MAJORTICKCOLOR"), RGB(32, 192, 255));
	//------------Half Tick Mark --------------------------------------------
	pAttrib->m_HalfTickLength = GETAPP.GetProfileIntW(_T("RULERS"), _T("HALFTICKLEN"), 12);
	pAttrib->m_colorHalfTick = GETAPP.GetProfileIntW(_T("RULERS"), _T("HALFTICKCOLOR"), RGB(0, 255, 0));
	//--------------Snap Tick --------------------------
	pAttrib->m_colorTickMark = GETAPP.GetProfileIntW(_T("RULERS"), _T("TICKMARKCOLOR"), RGB(255, 0, 0));
	pAttrib->m_TickLength = GETAPP.GetProfileIntW(_T("RULERS"), _T("SNAPTICKLEN"), 8);
	//----------- Ruler Size -----------------------
	pAttrib->m_RulerSize = GETAPP.GetProfileIntW(_T("RULERS"), _T("RULERSIZE"), 32);

}

void SRulerAttributes::SaveSettings(SRulerAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/

	GETAPP.WriteProfileStringW(_T("RULERS"), _T("FONT"), pAttrib->m_csFontName);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("BKCOLOR"), pAttrib->m_colorBk);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("CORNERCOLOR"), pAttrib->m_colorCorner);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("CURSORCOLOR"), pAttrib->m_colorCursor);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("TEXTCOLOR"), pAttrib->m_colorText);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("TICKMARKCOLOR"), pAttrib->m_colorTickMark);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("MAJORTICKCOLOR"), pAttrib->m_colorMajorTick);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("FONTSIZE"), pAttrib->m_FontSize);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("MAJORTICKLEN"), pAttrib->m_MajTickLength);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("RULERSIZE"), pAttrib->m_RulerSize);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("SNAPTICKLEN"), pAttrib->m_TickLength);
	//------------Half Tick Mark --------------------------------------------
	GETAPP.WriteProfileInt(_T("RULERS"), _T("HALFTICKLEN"), pAttrib->m_HalfTickLength);
	GETAPP.WriteProfileInt(_T("RULERS"), _T("HALFTICKCOLOR"), pAttrib->m_colorHalfTick);
}

//------------- Point Attributes -------------------------

void SPointAttributes::Save(
	CFile* pcfFile, 
	int Indent
) 
{
	char* pBuff = new char[2048];
	int n;
	char* pIndent = new char[128];
	char* pIndent2 = new char[128];
	char* s1 = new char[64];
	char* s2 = new char[64];
	char* s3 = new char[64];
	CString csOut;

	n = sprintf_s( 
		pBuff,
		2048,
		"%s%s{\n",
		GETAPP.IndentString(pIndent, 128,Indent,' '),
		CLexer::TokenLookup(Token::ATTRIB_POINT)
	);
	pcfFile->Write(pBuff,n);
	GETAPP.IndentString(pIndent2, 128, Indent + 2, ' ');
	n = sprintf_s(
		pBuff,
		2048,
		"%s%s\n%s%s\n%s%s\n%s}\n",
		pIndent2,
		CParser::SaveColorS(s1,64,Token::LINE_COLOR, m_colorLine),
		pIndent2,
		CParser::SaveColorS(s2,64,Token::SELECTED_COLOR,m_colorSelected),
		pIndent2,
		CParser::SaveIntValueS(s3, 64, Token::SELECTED_UNITS, m_SelectedObjectUnits),
		pIndent
	);
	pcfFile->Write(pBuff, n);
	delete[] s3;
	delete[] s2;
	delete[] s1;
	delete[] pIndent2;
	delete[] pIndent;
	delete[] pBuff;
}

void SPointAttributes::Parse(
	CParser* pParser	// pointer to parser
)
{
	BOOL Loop = TRUE;

	pParser->Expect( Token::ATTRIB_POINT);
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LINE_COLOR:
			pParser->Color(Token::LINE_COLOR, m_colorLine);
			break;
		case Token::SELECTED_COLOR:
			pParser->Color(Token::SELECTED_COLOR, m_colorSelected);
			break;
		case Token::SELECTED_UNITS:
			pParser->IntValue(Token::SELECTED_UNITS, m_SelectedObjectUnits);
			break;
		case Token('}'):
			pParser->Expect( Token('}') );
			Loop = FALSE;
			break;
		default:
			//--------------------------------------
			// Houston, we have a syntax error
			//--------------------------------------
			Exception.ErrorString.Format(
				_T("Error::Line %d  Column %d\nUnExpected Token: %hs\n"),
				pParser->GetLexer()->GetLine(),
				pParser->GetLexer()->GetCol(),
				CLexer::TokenLookup(pParser->GetLookaHead())
			);
			throw(Exception);
			break;
		}
	}
}

void SPointAttributes::LoadSettings(SPointAttributes* pAttrib)
{
	//------------------------------------------------
	// LoadSetting
	//	This Method loads the default settings
	// for this object.
	//*
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to load settings into
	//------------------------------------------------/
	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("POINT"), _T("NORMAL_COLOR"), RGB(192,192,192));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("POINT"), _T("SELECTED_COLOR"), RGB(255,0,80));
	pAttrib->m_SelectedObjectUnits = GETAPP.GetProfileIntW(_T("POINT"), _T("SELECTED_UNITS"),POINT_PIXELUNITS);
}

void SPointAttributes::SaveSettings(SPointAttributes* pAttrib)
{
	//------------------------------------------------
	// SaveSettings
	//	This Method saves the default settings
	// for this object to the registry.
	//
	// Parameter:
	//	pAttrib......pointer to attribute structure
	//				to save settings from
	//------------------------------------------------/
	GETAPP.WriteProfileInt(_T("POINT"), _T("NORMAL_COLOR"), pAttrib->m_colorLine);
	GETAPP.WriteProfileInt(_T("POINT"), _T("SELECTED_COLOR"), pAttrib->m_colorSelected);
	GETAPP.WriteProfileInt(_T("POINT"), _T("POINT"), pAttrib->m_SelectedObjectUnits);
}
