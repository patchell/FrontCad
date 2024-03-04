#pragma once

class CFrontCadDoc;


class CFileParser
{
	//-----------------------------
	// Lexer Atributes
	//-----------------------------
	int m_InputBuffer[2048];
	int m_BufferPosition;
	int m_NumberOfCharsInBuffer;
	BOOL m_EOF;
	//-----------
	int m_UnGetBuff;
	int m_LexIndex;
	char m_LexBuff[256];
	int m_Line;
	int m_Col;
	int m_LastCol;
	int m_LexValueInt;
	double m_LexValueDouble;
	struct KEYWORD {
		int m_Token;
		const char* m_pName;
	};
	static inline KEYWORD KeyWords[] = {
		{TOKEN_BASE_OBJ,"BASE_OBJ"},
		{TOKEN_ARC,"ARC"},
		{TOKEN_ARC_CENTER,"ARC_CENTER"},
		{TOKEN_CIRCLE,"CIRCLE"},
		{TOKEN_RECT,"RECT"},
		{TOKEN_RNDRECT,"RNDRECT"},
		{TOKEN_POLY,"POLY"},
		{TOKEN_LINE,"LINE"},
		{TOKEN_HOLERND,"HOLERND"},
		{TOKEN_HOLERECT,"HOLERECT"},
		{TOKEN_HOLE_1FLAT,"HOLE_1FLAT"},
		{TOKEN_HOLE_2FLAT,"HOLE_2FLAT"},
		{TOKEN_LIBPART,"LIBPART"},
		{TOKEN_TEXT,"TEXT"},
		{TOKEN_POINT,"POINT_1"},
		{TOKEN_VERTEX_POINT,"VERTEX_POINT"},
		{TOKEN_START_POINT,"START_POINT"},
		{TOKEN_END_POINT,"END_POINT"},
		{TOKEN_BACKGROUND_COLOR,"BACKGROUND_COLOR"},
		{TOKEN_LINE_COLOR,"LINE_COLOR"},
		{TOKEN_TEXT_COLOR,"TEXT_COLOR"},
		{TOKEN_FILL_COLOR,"FILL_COLOR"},
		{TOKEN_SELECTED_COLOR,"SELECTED_COLOR"},
		{TOKEN_RED,"RED"},
		{TOKEN_GREEN,"GREEN"},
		{TOKEN_BLUE,"BLUE"},
		{TOKEN_FONT,"FONT"},
		{TOKEN_FONT_WEIGHT,"FONT_WEIGHT"},
		{TOKEN_FONTHEIGHT,"FONTHIEGHT"},
		{TOKEN_FONTWIDTH,"FONTWIDTH"},
		{TOKEN_FONT_FORMAT,"FONT_FORMAT"},
		{TOKEN_FONT_NAME,"FONT_NAME"},
		{TOKEN_RADIUS,"RADIUS"},
		{TOKEN_FLATDIST,"FLATDIST"},
		{TOKEN_DRAWFILE,"DRAWFILE"},
		{TOKEN_VERTEX,"VERTEX"},
		{TOKEN_LIBFILE,"LIBFILE"},
		{TOKEN_RECTWIDTH,"RECTWIDTH"},
		{TOKEN_RECTHIEGHT,"RECTHIEGHT"},
		{TOKEN_ANGLE,"ANGLE"},
		{TOKEN_BK_TRANSPARENT,"TRANSPARENT"},
		{TOKEN_REFERENCE,"REFERENCE"},
		{TOKEN_BITMAP,"BITMAP"},
		{TOKEN_FILENAME,"FILENAME"},
		{TOKEN_ARROW,"ARROW"},
		{TOKEN_LENGTH,"LENGTH"},
		{TOKEN_ORG,"ORG"},
		{TOKEN_DIMENSION,"DIMENSION"},
		{TOKEN_PRINTRECT,"PRINTRECT"},
		{TOKEN_SIZE,"SIZE"},
		{TOKEN_SCALE,"SCALE"},
		{TOKEN_LINE_WIDTH,"LINE_WIDTH"},
		{TOKEN_ARROW_LENGTH,"ARROW_LENGTH"},
		{TOKEN_ARROW_WIDTH,"ARROW_WIDTH"},
		{TOKEN_SELECTED_UNITS,"SELECTED_UNITS"},
		{TOKEN_ATTRIB_ARC,"ATTRIB_ARC"},
		{TOKEN_ATTRIB_ARCCENTER,"ATTRIB_ARCCENTER"},
		{TOKEN_ATTRIB_ARROW,"ATTRIB_ARROW"},
		{TOKEN_ATTRIB_BITMAP,"ATTRIB_BITMAP"},
		{TOKEN_ATTRIB_DIMENSION,"ATTRIB_DIMENSION"},
		{TOKEN_ATTRIB_ELLIPSE,"ATTRIB_ELLIPSE"},
		{TOKEN_ATTRIB_GRID,"ATTRIB_GRID"},
		{TOKEN_ATTRIB_HOLERECT,"ATTRIB_HOLERECT"},
		{TOKEN_ATTRIB_HOLERND1F,"ATTRIB_HOLERND1F"},
		{TOKEN_ATTRIB_HOLERND2F,"ATTRIB_HOLERND2F"},
		{TOKEN_ATTRIB_HOLERND,"ATTRIB_HOLERND"},
		{TOKEN_ATTRIB_LINE,"ATTRIB_LINE"},
		{TOKEN_ATTRIB_ORIGIN,"ATTRIB_ORIGIN"},
		{TOKEN_ATTRIB_POINT,"ATTRIB_POINT"},
		{TOKEN_ATTRIB_POLYGON,"ATTRIB_POLYGON"},
		{TOKEN_ATTRIB_RECT,"ATTRIB_RECT"},
		{TOKEN_ATTRIB_RNDRECT,"ATTRIB_RNDRECT"},
		{TOKEN_ATTRIB_RULERS,"ATTRIB_RULERS"},
		{TOKEN_ATTRIB_TEXT,"ATTRIB_TEXT"},
		{TOKEN_NUMINT,"<int number>"},
		{TOKEN_NUMDOUBLE,"<double number>"},
		{TOKEN_STRING,"<string>"},
		{TOKEN_ERRORToken,"<Error>"},
		{0,0}
	};
public:
	CFileParser();
	virtual ~CFileParser();
	BOOL Create(CString& csInputFileName);
	int Lex(CFile* pcfInFile);
	BOOL Accept(CFile* pF, int LookaHead, int Token);
	int Expect(CFile* pF, int Token, int LookaHead);
	//-------------------------------------
	// Parsing Primative Utillity Methods
	//-------------------------------------
	int Color(
		CFile* pcfInFile,
		int TargetToken,	//type of color object 
		COLORREF& ColorParam,		//Color Value to set 
		int Token		//Lookahead Token
	);
	int IntValue(
		CFile* pcfInFile,
		int TargetToken,	//type of color object 
		int& IntegerValue,			//Integer Value to set 
		int Token		//Lookahead Token
	);
	int UINTValue(
		CFile* pcfInFile,
		int TargetToken,	//type of color object 
		UINT& UINTValue,			//Integer Value to set 
		int Token		//Lookahead Token
	);
	int DoubleValue(
		CFile* pcfInFile,
		int TargetToken,		//type of color object 
		double& IntegerValue,			//Double Value to set 
		int Token			//Lookahead Token
	);
	int StringValue(
		CFile* pcfInFile,
		int TargetToken,		//type of color object 
		CString& csS,
		int Token		//Lookahead Token
	);
	//----------------------------------------------------
	// Parsing Methods
	//----------------------------------------------------
	BOOL ParseLibFile(CFile* pcfInFile, CCadLibrary** ppLib);
	BOOL ParseDrawFile(CFile* pcfInFile, CCadDrawing* pDrawing);
	int DrawFileAttributes(CFile* pcfInFile, int  Token, CCadObject* pO);
	int DrawFileAttributes2(CFile* pcfInFile, int  Token, CCadObject* pO);
	int DrawFileAttributes3(CFile* pcfInFile, int  Token, CCadObject* pO);
//	int PrintRect(int  Token, CCadObject* pO);
	//----------------------------------------
	// File Save Utils
	//----------------------------------------
	static char* SaveString(char* s, int n, int  Token, char* string);
	static char* SaveString(char* s, int n, int  Token, CString& csString);
	static char* SaveSize(char* s, int n, int  Token, CDoubleSize& size);
	static char* SaveColor(char* s, int n, int  Token, COLORREF color);
	static char* SaveIntValue(char* s, int n, int  Token, int& value);
	static char* SaveUINTValue(char* s, int n, int  Token, UINT& value);
	static char* SaveDoubleValue(char* s, int n, int Token, double& value);
	static char* SaveDoubleValue(char* s, int n, double& v);
	//--------------------------------------------------
	// Lexer Methods
	//--------------------------------------------------
	int GetFromInput(CFile* pcfInFile);
	int GetChar(CFile* pcfInFile);
	int UnGetChar(int c);
	int LookUp(const char* pS);
	static const char* TokenLookup(int TokenVal);
	BOOL IsValidDigit(int c);
	BOOL IsDoubleValue(int c);
	const char* GetLexBuff() { return m_LexBuff; }

	int GetLine() { return m_Line; }
	int GetCol() { return m_Col; }
	int GetIntLexValue() { return m_LexValueInt; }
	double GetDoubleLexValue() { return m_LexValueDouble; }

};

