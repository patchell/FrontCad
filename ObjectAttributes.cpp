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

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ARC"), _T("COLOR"), RGB(128, 128, 192));
	csString  = GETAPP.GetProfileStringW(_T("ARC"), _T("WIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csString);
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
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ARC"), _T("WIDTH"), csString);
}

DocFileParseToken SArcAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
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

DocFileParseToken SArcCenterAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
}

//----- Arc Angle Attributes --------

void SArcAngleAttributes::LoadSettings(SArcAngleAttributes* pAttrib)
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

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ARC_CENTER"), _T("COLOR"), RGB(128, 1280, 1922));
	csVal = GETAPP.GetProfileStringW(_T("ARC_CENTER"), _T("WIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csVal);
}

void SArcAngleAttributes::SaveSettings(SArcAngleAttributes* pAttrib)
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
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ARC_CENTER"), _T("WIDTH"),csString);
}

DocFileParseToken SArcAngleAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
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

DocFileParseToken SArrowAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
}

//------------- Bitmap Attributes -----------------

DocFileParseToken SBitmapAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
{
	return Token;
}

void SBitmapAttributes::Save(FILE* pO, DocFileParseToken TypeToken, int Indent, int flags)
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

DocFileParseToken SCadDimAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
}

//-------------- Ellispe Attributes -----------------


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

DocFileParseToken SEllipseAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
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

DocFileParseToken SLineAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
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

DocFileParseToken SOriginAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
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

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("POLYGON"), _T("LINECOLOR"), RGB(128,128,192));
	pAttrib->m_colorSelect = GETAPP.GetProfileIntW(_T("POLYGON"), _T("SELECTCOLOR"), RGB(255, 0, 64));
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
	GETAPP.WriteProfileInt(_T("POLYGON"), _T("SELECTCOLOR"),pAttrib->m_colorSelect);
	GETAPP.WriteProfileInt(_T("POLYGON"), _T("FILLCOLOR"), pAttrib->m_colorFill);
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("POLYGON"), _T("LINEWIDTH"), csString);
	GETAPP.WriteProfileInt(_T("POLYGON"), _T("TRANSPARENT_FILL"), pAttrib->m_TransparentFill);
}

DocFileParseToken SPolyAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
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

DocFileParseToken SRectAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
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
	pAttrib->m_dszCornerRadius.dCX = GETAPP.GetProfileIntW(_T("RNDRECT"), _T("RADIUS_X"), 100);
	pAttrib->m_dszCornerRadius.dCY = GETAPP.GetProfileIntW(_T("RNDRECT"), _T("RADIUS_Y"), 100);
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
	GETAPP.WriteProfileInt(_T("RNDRECT"), _T("RADIUS_X"), (int)(pAttrib->m_dszCornerRadius.dCX * 1000.0));
	GETAPP.WriteProfileInt(_T("RNDRECT"), _T("RADIUS_Y"), (int)(pAttrib->m_dszCornerRadius.dCY * 1000.0));
}

DocFileParseToken SRoundedRectAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
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
	return Token;
}

//----------- Text Attributes ---------------------

void STextAttributes::Save(FILE* pO, int Indent, int flags)
{
	char* IndentString = new char[256];
	
	GETAPP.MkIndentString(IndentString, Indent);
	fprintf(pO, "%s%s(\n",
		IndentString,
		CLexer::TokenToString(DocFileParseToken::ATTRIB_TEXT)
	);
	GETAPP.SaveString(pO, Indent + 1, m_csFontName, DocFileParseToken::FONT);
	GETAPP.SaveColor(pO, Indent + 1, m_colorText, DocFileParseToken::TEXTCOLOR);
	GETAPP.SaveColor(pO, Indent + 1, m_colorBK, DocFileParseToken::BKCOLOR);
	GETAPP.SaveDouble(pO, Indent + 1, m_fontHeight, DocFileParseToken::HEIGHT);
	GETAPP.SaveDouble(pO, Indent + 1, m_fontWidth, DocFileParseToken::WIDTH);
	GETAPP.SaveUINT(pO, Indent + 1, m_Format, DocFileParseToken::FORMAT);
	GETAPP.SaveUINT(pO, Indent + 1, m_Transparent, DocFileParseToken::TRANSPARENTToken);
	GETAPP.SaveUINT(pO, Indent + 1, m_fontWeight, DocFileParseToken::WEIGHT);
	fprintf(pO, "%s)\n",IndentString);
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

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("RECTHOLE"), _T("LINECOLOR"), RGB(128, 128, 129));
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
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("RECTHOLE"), _T("LINEWIDTH"), csString);
	csString.Format(_T("%lf"), pAttrib->m_HoleWidth);
	GETAPP.WriteProfileStringW(_T("RECTHOLE"), _T("WIDTH"), csString);
}

DocFileParseToken SRectHoleAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
{
	return Token;
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

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ROUNDHOLE"), _T("LINECOLOR"), RGB(128, 128, 192));
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
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE"), _T("LINEWIDTH"), _T("0.04"));
	csString.Format(_T("%lf"), pAttrib->m_HoleRadius);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE"), _T("RADIUS"), csString);
}

DocFileParseToken SRoundHoleAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
{
	return Token;
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

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ROUNDHOLE_1_FLAT"), _T("LINECOLOR"), RGB(128, 128, 192));
	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("LINEWIDTH"), _T("0.04"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csString);
	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("RADIUS"), _T("0.125"));
	pAttrib->m_HoleRadius = GETAPP.StringToDouble(csString);
	pAttrib->m_FlatDistanceFromCenter = GETAPP.GetProfileIntW(_T("ROUNDHOLE_1_FLAT"), _T("CENT_TO_FLAT"), 1000);
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

	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("LINEWIDTH"), csString);

	csString.Format(_T("%lf"), pAttrib->m_HoleRadius);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("RADIUS"), csString);

	csString.Format(_T("%lf"), pAttrib->m_FlatDistanceFromCenter);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("CENT_TO_FLAT"), csString );
}

DocFileParseToken SRndHole1FlatAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
{
	return Token;
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

	pAttrib->m_colorLine = GETAPP.GetProfileIntW(_T("ROUNDHOLE_2_FLAT"), _T("LINECOLOR"), RGB(128, 128, 192));
	
	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_2_FLAT"), _T("LINEWIDTH"), _T("0.05"));
	pAttrib->m_LineWidth = GETAPP.StringToDouble(csString);

	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_2_FLAT"), _T("DIANETER"), _T("0.125"));
	pAttrib->m_HoleRadius = GETAPP.StringToDouble(csString);

	csString = GETAPP.GetProfileStringW(_T("ROUNDHOLE_2_FLAT"), _T("CENT_TO_FLAT"), _T("0.100"));
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
	csString.Format(_T("%lf"), pAttrib->m_LineWidth);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("LINEWIDTH"), csString);

	csString.Format(_T("%lf"), pAttrib->m_HoleRadius);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("DIANETER"), csString);

	csString.Format(_T("%lf"), pAttrib->m_FlatDistanceFromCenter);
	GETAPP.WriteProfileStringW(_T("ROUNDHOLE_1_FLAT"), _T("CENT_TO_FLAT"), csString);
}

DocFileParseToken SRndHole2FlatAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
{
	return Token;
}

//------------------- Grid Attributes ---------------------------------

DocFileParseToken SGridAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
{
	return Token;
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

DocFileParseToken SRullerAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
{
	return Token;
}

void SRullerAttributes::LoadSettings(SRullerAttributes* pAttrib)
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

void SRullerAttributes::SaveSettings(SRullerAttributes* pAttrib)
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

DocFileParseToken SPointAttributes::Parse(DocFileParseToken Token, CLexer* pLex)
{
	return Token;
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
	pAttrib->m_colorNormal = GETAPP.GetProfileIntW(_T("POINT"), _T("NORMAL_COLOR"), RGB(32,128,192));
	pAttrib->m_colorSelected = GETAPP.GetProfileIntW(_T("POINT"), _T("SELECTED_COLOR"), RGB(255,0,80));
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
	GETAPP.WriteProfileInt(_T("POINT"), _T("NORMAL_COLOR"), pAttrib->m_colorNormal);
	GETAPP.WriteProfileInt(_T("POINT"), _T("SELECTED_COLOR"), pAttrib->m_colorSelected);
}
